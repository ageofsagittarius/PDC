/*
    IIT2019038
    Ritik Mehra
    PDC Assignment-13

    Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2
    
    Problem Statement: Find the value of Pi using the Dart Board Method
    
    Instructions to run:
        mpic++ mpi-c .c -o mpi.o
        nvcc -c cuda.cu -o cuda.o
        mpic++ mpi.o cuda.o -lcudart
*/


#include <bits/stdc++.h>
using namespace std;


//kernel function to calculate the value of pi
__global__ void estimatePI(double *valueX, double *valueY, int *countNum, int gridSize, int N) {
    
    // shared array for shared memory
    __shared__ int shMem[512];
    
    int tid = threadIdx.x;
    int bid = blockIdx.x * blockDim.x;

    // the index and the jump from the index is
    int arrayId = tid + bid;
    int jump = blockDim.x * gridSize;
    
    int cnt = 0;
    for(int i = arrayId; i < N; i += jump) {
        double x = valueX[i];
        double y = valueY[i];
        
        double x1 = x * x;
        double y1 = y * y;
        
        if(x1 + y1 <= 1.0) {
            cnt += 1;
        }
    }
    
    shMem[tid] = cnt;
    
    __syncthreads();
    
    if(tid == 0) {
        int numOfHits = 0;    
        for(int j = 0; j < blockDim.x; j++) {      
            numOfHits += shMem[j];    
        }
        countNum[blockIdx.x] = numOfHits;  
    }
}

double randomNum(){
    double pt = (double)rand()/RAND_MAX;

    return pt;
}

int main() {

    int N = 10000000;
    //cout<<"Enter the value of N: ";
    //cin>>N;

    // allocate space to hold random values  
    double *arrayX = (double*) malloc(N * sizeof(double));
    double *arrayY = (double*) malloc(N * sizeof(double));
    
    //Initialize vector with random values    
    for (int i = 0; i < N; ++i) {
        arrayX[i] = randomNum();
        arrayY[i] = randomNum();
    }
    
    // Send random values to the GPU    
    double* valueX;
    double* valueY;
    
    // allocate the array values in the device
    cudaMalloc(&valueX, N * sizeof(double));
    cudaMalloc(&valueY, N * sizeof(double));
    
    // copy the array values in the device
    cudaMemcpy(valueX, arrayX, N * sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(valueY, arrayY, N * sizeof(double), cudaMemcpyHostToDevice);
    
    // Launch kernel to count samples that fell inside unit circle    
    int numOfThreads = 512;
    int gridSize = N / (1000 * numOfThreads);
    int blockSize = gridSize * sizeof(int);
    
    int *countNum;
    
    cudaMalloc(&countNum, blockSize);
    
    // call the kernel function
    estimatePI<<<gridSize, numOfThreads>>>(valueX, valueY, countNum, gridSize, N);
    
    int* res = new int[gridSize];
    // copy the array values back to the host
    cudaMemcpy(res, countNum, blockSize, cudaMemcpyDeviceToHost);
    
    int cnt = 0;
    for (int i = 0 ; i < gridSize; i++) {
        cnt += res[i];
    }
    
    cudaFree(valueX);
    cudaFree(valueY);
    cudaFree(countNum);

    double pi_calculated = ((double)cnt / (double)N) * 4.0;
    printf("The value of PI by calculation is: %f \n", pi_calculated);


    return 0;
}
