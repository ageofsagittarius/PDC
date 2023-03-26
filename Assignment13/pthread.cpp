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
    
    Implemented using MPI + Pthread
    To run this open the terminal where the file is stored
        Then write the command mpic++ pthread.cpp
        After it run the command ./a.out
*/


#include <bits/stdc++.h>
#include <pthread.h>
#include <mpi.h>

using namespace std;

struct data_structure{
    int left;
    int right;
};

int N;
int cnt;
int numOfThreads;
int pid, jump = 0;

pthread_mutex_t piLock;

double randomNum(){
    double pt = (double)rand()/RAND_MAX;

    return pt;
}

void *calculatePI(void *str)
{
    struct data_structure cur_data_structure = (struct data_structure)str;

    int lft = cur_data_structure -> left;
    int rht = cur_data_structure -> right;

    for (int i = lft; i < rht; i++){
        //finding the random points
        double pt1 = randomNum();
        double pt2 = randomNum();

        pthread_mutex_lock(&piLock);
        
        double pt1_2 = (pt1 * pt1);
        double pt2_2 = (pt2 * pt2);
        
        double sum = pt1_2 + pt2_2;
        double sqr = sqrt(sum);        
        //incrementing count value if the point is inside the circle
        if(sqr <= 1){
            cnt += 1;
        }
        
        pthread_mutex_unlock(&piLock);
    }
}

void init(){
    int a1;
    char **a2;
    MPI_Init(&a1, &a2);
}

int main() {
    pthread_mutex_init(&piLock, NULL);

    printf("Enter the value of N: ");
    scanf("%d", &N);
    cnt = 0;
	
    cout<<"Enter the number of threads: ";
    cin>>numOfThreads;

    double pi_calculated;

    pthread_t threads[numOfThreads];
    
    // MPI Process will call it at start
    init();
    
    // finding the process id(rank) and the count of processes number
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &jump);

    int lft = 0, rht = N / numOfThreads;
    
	for(int i = 0; i < numOfThreads; i++){
        struct data_structure str = (struct data_structure) malloc(sizeof(struct data_structure));
        str -> left = lft;
        str -> right = rht;

        pthread_create(&threads[i], NULL, calculatePI, str);

        lft = rht + 1;
        rht = lft + N/numOfThreads;

        if(rht > N) rht = N;
    }

    for(int i = 0; i < numOfThreads; i++){
        pthread_join(threads[i], NULL);
    }

    int newCount;
    MPI_Reduce(&cnt, &newCount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    int newN;
    MPI_Reduce(&N, &newN, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	if(pid == 0){
        pi_calculated = ((double)cnt / (double)N) * 4.0;
        printf("The value of PI by calculation is: %f \n", pi_calculated);
    }

    
    // MPI Process will call it at end
    MPI_Finalize();
    
    return 0;
}


/*
    Sample Input/Output:
    Enter the value of N: 192819372
    Enter the number of threads: 16
    The value of PI by calculation is: 3.141558

    Enter the value of N: 198203
    Enter the number of threads: 4
    The value of PI by calculation is: 3.141163
    
    Enter the value of N: 9283172
    Enter the number of threads: 8
    The value of PI by calculation is: 3.141503
    
    Enter the value of N: 102937364
    The value of PI by calculation is: 3.141699
    
*/
