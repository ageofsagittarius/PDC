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
    
    Serial Implementation
*/


#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

double randomNum(){
    double pt = (double)rand()/RAND_MAX;

    return pt;
}

int main() {
    int N;
    printf("Enter the value of N: ");
    scanf("%d", &N);
    int count = 0;
	
    double pi_calculated;

	for(int i = 0; i < N; i++){
        //finding the random points
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

	pi_calculated = ((double)count / (double)N) * 4.0;          //p = 4(m/n)

    cout<<"The value of PI by calculation is: "<<pi_calculated<<endl;
    
    return 0;
}