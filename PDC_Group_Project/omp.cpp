/*
    PDC Group Project:
        Sarvesh (IIT2019030)
        Rajat Mehra (IIT2019034)
        Abhishek Bhaware (IIT2019037)
        Ritik Mehra (IIT2019038)
*/

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

/*
    Function for the LU Decomposition

    mat is the matrix that will decompose into two matrices
    the matrix "mat" will decompose in matrices d_mat1, d_mat2
    
    The size of all the matrices is N x N
*/
void LUDecomposition(double** mat, double** L, double** U, int N){

    int numOfThreads;
    cout<<"Enter the number of threads: ";
    cin>>numOfThreads;

    omp_set_num_threads(numOfThreads);

    #pragma omp parallel shared(mat, L, U)
    {
        for(int i = 0; i < N; i++){

            // calculations for Lower Triangular Matrix
            #pragma omp for schedule(static)
            for(int j = 0; j < N; j++){
                //when j < i, then we'll put the value 0 for L[i][j]
                if (j < i){
                    L[j][i] = 0;
                }
                //otherwise, do some math to get the right value
                else{
                    L[j][i] = mat[j][i];
                    for(int k = 0; k < i; k++){
                        //deduct from the current l cell the value of these 2 values multiplied
                        L[j][i] -= (L[j][k] * U[k][i]);
                    }
                }
            }

            // calculations for Upper Triangular Matrix
            #pragma omp for schedule(static)
            for(int j = 0; j < N; j++){
                //when j < i, then we'll put the value 0 for U[i][j]
                if(j < i){
                    U[i][j] = 0;
                }
                //when j = i, then we'll put the value 1 for U[i][j]
                else if(j == i){
                    U[i][j] = 1;
                }
                //when i > j then we'll do calculations to find the current element of the matrix
                else{
                    U[i][j] = mat[i][j] / L[i][i];
                    for(int k = 0; k < i; k++){
                        U[i][j] -= ((L[i][k] * U[k][j]) / L[i][i]);
                    }
                }
            }
        }
    }
}

//function to print the matrices
void printMat(double** mat, int N){
    int i, j;
    for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			//cout << left << setw(9) << setprecision(3) << mat[i][j] << left <<  setw(9);
            printf("%.3f\t", mat[i][j]);
		}
		cout<<endl;
	}
}

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

    int N;
    cout<<"\nEnter the size of the matrix: ";
    cin>>N;

	// declare and initialsie the matrix and fill with random values
    double **mat = new double* [N];
    int i, j;
    for(i = 0; i < N; i++){
		mat[i] = new double[N];
	}

    // fill the matrix with ranodm values
    for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			mat[i][j] = ((rand() % 10) + 1) ;
		}
	}

    /*
        check if the matrix is invertible or not
        To do so we have used the concept of diagonally dominant matrix

        The diagonally dominant matrices are always invertible. A matrix is said to be
        diagonally dominant if the diagonal element in all the rows have an absolute
        value greater than the sum of the absolute values of all other elements in that
        particular row
    */
    int cnt = 0;
    double sum = 0;
	for(i = 0; i < N; i++, cnt++){
		for(j = 0; j < N; j++){
			sum += abs(mat[i][j]);
		}
		sum -= abs(mat[i][cnt]);

        if(sum <= 0){
		    mat[i][cnt] = sum + ((rand()%5)+1);
        }
        sum = 0;
	}

    // original matrix 
	cout<<"\nThe Original Matrix is: \n";
	printMat(mat, N);

    // declare and initialse the upper and lower diagonal matrices
    double **L = new double* [N];
    double **U = new double* [N];
    for(i = 0; i < N; i++){
		L[i] = new double[N];
        U[i] = new double[N];
	}

    // starting time
    clock_t begin = clock();

    //now perform the LU Decomposition
    LUDecomposition(mat, L, U, N);

    // ending time
	clock_t end = clock();
	

    //print l and u
	cout<<"\nThe resultant matrices are:\n";
    cout<<"\nL Matrix is:\n";
	printMat(L, N);
	cout<<"\nU Matrix is:\n";
	printMat(U, N);
    
	
    // time taken to execute the program
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to evaluate using OpenMP  :%.6f Seconds\n", time_spent);


    // for the correctness of algorithm, we may multiply the matrix L and U and verify it with input matrices
    double **m1 = new double* [N];
    for(i = 0; i < N; i++){
		m1[i] = new double[N];
	}
    mulMat(L, U, m1, N);
    /*
    cout<<"\n---------------------------\n";
    cout << "Multiplication of given two matrices is:\n";
    //printMat(m1, N);
    */

    int flag = 1;
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            if(abs(mat[i][j] - m1[i][j]) >= 0.05){
                flag = 0;
                break;
            }
        }
    }
    
    if(flag)cout<<"\nCORRECT\n";
    else cout<<"\nINCORRECT\n";



	return 0;
}