#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main()
{
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

    
    
    // OpenMP Parallel For Directive And Critical Section
    double parallelSum = 0;
    #pragma omp parallel for shared(sum)
        for (int i = 0; i < n; i++) {
        #pragma omp critical
    		parallelSum += array[i];
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