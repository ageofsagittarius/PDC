
/*
    PDC Group Project: 2022
    Ritik Mehra
    Abhishek Bhaware
    Rajat Mehra
    Sarvesh
*/

#include <bits/stdc++.h>
using namespace std;

// kernel function
__global__ void fun1(double *matrix, int N, int idx){

    for(int i = idx * N + idx + 1 ; i < idx * N + N; i++){
        matrix[i] = (matrix[i] / matrix[idx * N + idx]);
    }
}

//kernel function that uses shared memory for the LU Calculation and storing result in row-array format
__global__ void fun2(double *mat, int N, int idx, int size){
    extern __shared__ double sharedMem[];

    int tid = threadIdx.x;
    int bid = blockIdx.x;
    
    if(tid == 0){
        for(int i = idx; i < N; i++){
            sharedMem[i] = mat[(idx * N) + i];
        }
    }
    
    __syncthreads();
    
    int arrayId = (size * bid) + tid; 
    
    int curIdx = arrayId * N;
    int left = curIdx + idx, right = curIdx + N;
    
    if(curIdx > idx * N){
        for(int i = left + 1; i < right; i++){
            mat[i] -= mat[left] * sharedMem[i - curIdx];
        }
    }
    
    __syncthreads();
    
    }

//function to print the matrices
void printMat(double** mat, int N){
    int i, j;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            printf("%.3f\t", mat[i][j]);
        }
        cout<<endl;
    }
}

//function used for matrix multiplication
void mulMat(double** mat1, double**mat2, double** res, int N){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = 0;
            for (int k = 0; k < N; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

int main(){
    //the size of the matrix
    int N = 10;
    cout<<"The dimension of the matrix is: "<<N<<endl;
    //allocating memory on host
    double *input = new double[N * N];
    double *output = new double[N * N];
    
    for (int i = 0; i <= (N * N); i++){
        input[i] =(rand() % 10) + 1;
    }
    
    int cnt = 0;
    double sum = 0;
    for(int i = 0; i < N; i++, cnt++){
        for(int j = i*N; j < i*N + N; j++){
            sum += abs(input[j]);
        }
        sum -= abs(input[i*N + cnt]);
        input[i*N + cnt] = sum + (rand() % 5) + 1;
        sum = 0;
    }
    
    //starting time
    clock_t begin = clock();
    
    // allocate memory on device i.e. GPU
    double *mat;
    cudaMalloc(&mat, N * N * sizeof(double));
    
    // copy from host to device
    cudaMemcpy(mat, input, N * N * sizeof(double), cudaMemcpyHostToDevice);
    
    int numOfThreads = 8;
    int numOfBlock = N / numOfThreads;
    if(N % numOfThreads){
        numOfBlock++;
    }

    for(int i = 0; i < N; i++){
        fun1<<<1, 1>>>(mat, N, i);
        fun2<<<numOfBlock, numOfThreads, N * sizeof(double)>>>(mat, N, i, numOfThreads);
    }
    
    // copying back from device to host
    cudaMemcpy(output, mat, N * N * sizeof(double), cudaMemcpyDeviceToHost);
    
    //ending time
    clock_t end = clock();
    
    // declare and initialise the matrices
    double **matrix = new double*[N];
    double **mm = new double*[N];
    double **L = new double*[N];
    double **U = new double*[N];
    
    for(int i = 0; i < N; i++){
        matrix[i] = new double[N];
        mm[i] = new double[N];
        L[i] = new double[N];
        U[i] = new double[N];
    }
    
    //putting the values in the matrices from the output obtained
    
    //original matrix
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            mm[i][j] = input[i * N + j];
            matrix[i][j] = output[i * N + j];
        }
    }
    
    //filling the lower and upper diagonal matrix i.e L and U
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            for(int k = 0; k < N; k++){
                if(i >= k){
                    L[i][k] = output[i * N + k];
                }
                else{
                    L[i][k] = 0;
                }

                if(k == j){
                    U[k][j] = 1;
                }
                else if(k < j){
                    U[k][j] = output[k * N + j];
                }
                else{
                    U[k][j] = 0;
                }
            }
        }
    }
    
    cout<<"The original matrix is: \n";
    printMat(mm, N);
    
    //print the resultant matrices i.e. L and U
    cout<<"\nThe resultant matrices are:\n";
    cout<<"\nL Matrix is:\n";
    printMat(L, N);
    cout<<"\nU Matrix is:\n";
    printMat(U, N);
    
    // time taken to execute the program
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to evaluate using sequential  :%.6f Seconds\n", time_spent);
    
    // for the correctness of algorithm, we may multiply the matrix L and U and verify it with input matrices
    double **m1 = new double* [N];
    for(int i = 0; i < N; i++){
        m1[i] = new double[N];
    }
    mulMat(L, U, m1, N);
    cout<<"\n---------------------------\n";
    //cout << "Multiplication of given two matrices is:\n";
    //printMat(m1, N);


    int flag = 1;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(abs(mm[i][j] - m1[i][j]) >= 0.05){
                flag = 0;
                break;
            }
        }
    }
    
    if(flag)cout<<"\nCORRECT\n";
    else cout<<"\nINCORRECT\n";

    
    cudaFree(mat);
    
    
    return 0;
}
