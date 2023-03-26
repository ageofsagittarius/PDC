#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

/*
In this part, we were asked to use parallel and critical directives.
We just need to add the critical command to parallel directive code. The critical section in this problem is 
where we are adding the sum of chunks in our parallelSum variable.
*/

int main(){
    
    // size of array
    int n = 1000;
    
    // array initialisation
	double* array = new double[n];
	for(int i = 0; i < n; i++)
		array[i] = i;
    
    
    // finding sum using serialization
	double serialSum = 0;
	for(int i = 0; i < n; i++)
		serialSum += array[i];
	
    
    // OpenMP Parallel For only Parallel directive
    double parallelSum = 0;
    int th = 200;
    #pragma omp parallel num_threads(5)
    {
        // thread id
        int id = omp_get_thread_num();

        // the range of elements to be taken for sum
    	int left = (id * th);
        int right = (id + 1) * th;

        #pragma omp critical
        for(int i = left; i < right; i++){
                parallelSum += array[i];
        }
    }


    // finding sum using Formula
	double formula_sum = (double)n*(n-1)/2;

	std::cout << "N: " << n << std::endl;
	std::cout << "Serial Sum:    " << serialSum << std::endl;
	std::cout << "Parallel Sum:    " << parallelSum << std::endl;
	std::cout << "Formula Sum: " << formula_sum << std::endl;

	delete[] array;
	
	return 0;
}