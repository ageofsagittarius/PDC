/*
    IIT2019038
    Ritik Mehra
    PDC Assignment-12

    Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2
    
    Problem Statement: Find the value of Pi using the Dart Board Method
    
    Implemented using MPI
*/


#include <bits/stdc++.h>
#include <omp.h>
#include <mpi.h>

using namespace std;


double randomNum(){
    double pt = (double)rand()/RAND_MAX;

    return pt;
}

void init(){
    int a1;
    char **a2;
    MPI_Init(&a1, &a2);
}

int main() {

    int N;
    printf("Enter the value of N: ");
    scanf("%d", &N);
    int count = 0;
    int pid, jump;
    
    // MPI Process will call it at start
    init();
    
    // finding the process id(rank) and the count of processes number
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &jump);

    // runnin a for loop for the processes
    for(int i = 0; i < N; i++){
        double pt1 = randomNum();
        double pt2 = randomNum();
        
        double pt1_2 = (pt1 * pt1);
        double pt2_2 = (pt2 * pt2);
        
        double sum = pt1_2 + pt2_2;
        double sqr = sqrt(sum);
        
        //incrementing count value if the point is inside the circle
        if(sqr <= 1){
            count += 1;
        }
    }
    }
    int newCount;
    MPI_Reduce(&count, &newCount, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    int newN;
    MPI_Reduce(&N, &newN, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi_calculated;
    if(pid == 0){
        pi_calculated = ((double)count / (double)N) * 4.0;
        printf("The value of PI by calculation is: %f \n", pi_calculated);
    }

    // MPI Process will call it at end
    MPI_Finalize();


    return 0;
}

/*
    Sample Input/Output:
    Enter the value of N: 1002
    The value of PI by calculation is: 3.133733 
    
    Enter the value of N: 1000000
    The value of PI by calculation is: 3.141664
    
    Enter the value of N: 102937364
    The value of PI by calculation is: 3.141699
    
*/