/*
	IIT2019038

	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2


	Matrix Multilpication:
		Matrix Size - 1024*1024
		Time is calculated using clock() function
	
	Method Used: Pthread
		Number of threads used : 2, 4, 8, 16
*/

#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

// Matrix size
#define N 1024

// Num of threads
int numOfThreads;

// Matrices of order N*N
int mat1[N][N];
int mat2[N][N];

// Resulatnt Matrix
int res_mat[N][N] = {0};

// Structure for storing row and column
struct data_structure{
	int row;		//row
	int column;		//column
};

void * multiply(void *arg){	
	//the structure that holds our data
	struct data_structure *data = (struct data_structure *)arg;
	
	int i = data -> row;
	int j = data -> column;
	
	int k, mul = 0;
	for (k = 0; k < N; k++)
		mul += (mat1[i][k] * mat2[k][j]);
	
	res_mat[i][j] = mul;
}

// function to dispaly matrix
void display(int mat[N][N]){
    int i, j;
    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]){

	// Create Matrices and filling them with random integers
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            mat1[i][j] = rand() % 10;
            mat2[i][j] = rand() % 10;
        }
    }

    cout<<"The number of threads are: ";
    cin>>numOfThreads;

    /*
        // Displaying both the Matrices
        display(mat1);
        display(mat2);
    */

	printf("Method Used\t\t  :Pthread\n");
    printf("Number of Threads\t  :%d\n", numOfThreads);

	// Starting time
    clock_t begin = clock();

	pthread_t threads[numOfThreads];

	// Calculating Matrix Multiplication
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			for (int th = 0; th < numOfThreads; th++){
				struct data_structure *data = (struct data_structure*)malloc(sizeof(struct data_structure));
				data -> row = i;
				data -> column = j;
				
				// now create the thread passing it data as a parameter	
				// set of thread attributes
				pthread_attr_t attr;	
				
				// get the default attributes
				pthread_attr_init(&attr);	
				
				//create thread
				pthread_create(&threads[th], &attr, multiply, data);
				
				// make sure parent waits for all thread to complete
				// join thread
				pthread_join(threads[th], NULL);
				
			}
		}
	}
	
	// ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("Time taken to Multiply  :%.6f Seconds\n",time_spent);

	// displaying resultant matrix
	// display(res_mat);
    
    return(0);
}

/*
	Execution Time for different number of threads:

		Num of Threads          Time Taken for Execution
			2                       58.473586 Seconds
			4                       115.645320 Seconds
			8                       227.422347 Seconds
			16                      446.997827 Seconds
	
	The Output of the code above looks like this:
			Method Used		        :Pthread
			Number of Threads	    :2
			Time taken to Multiply  :58.473586 Seconds

*/