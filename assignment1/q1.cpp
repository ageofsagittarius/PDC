/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Count of primes:
		Time is calculated using clock() function
	
	Method Used: Pthread
		Number of threads used : 2, 4, 8, 16
*/

#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

int n;
int numOfThreads;
int numOfPrimes;

void *countPrimes(void *td) {
    long thread_id;
    thread_id = (long) td;
    static int prime = 0;
    int left = 1 + (n - 1) * prime / numOfThreads;
    int right = 1 + (n - 1) * (prime + 1) / numOfThreads;
    prime++;

    bool flag;
    cout<<"Thread "<<prime<<" : ";

    clock_t begin = clock();

    while (left <= right) {
        /* Assume isPrime is true */
        flag = true;
        for (int i = 2; i * i <= left; i++){
            if(left % i == 0){
                flag = false;
                break;
            }
        }
        if(flag){cout<<left<<" ";numOfPrimes++;}
        left++;
    }
    
    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to count primes  :%.6f Seconds\n",time_spent);

}

int main() {

    /* Thread Prime Checker */
    
    cout<<"The value of 'n' is: ";
    cin>>n;
    
    n = pow(2, n);
    
    cout<<"The number of threads are: ";
    cin>>numOfThreads;

    numOfPrimes = 0;

    pthread_t threads[numOfThreads];
    int rc;
    long t;
    for (int th = 0; th < numOfThreads; th++) {
        pthread_create(&threads[th], NULL, countPrimes, (void *)th);        
        pthread_join(threads[th], NULL);
    }
    
    cout<<"Total num of Primes: "<<numOfPrimes<<"\n";

    return 0;
}

/*
	The value of 'n' is: 10
    The number of threads are: 4
    
    Thread 1 : 1 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97 101 103 107 109 113 127 131 137 139 149 151 157 163 167 173 179 181 191 193 197 199 211 223 227 229 233 239 241 251
    Time taken to count primes  :0.009000 Seconds
    
    Thread 2 : 257 263 269 271 277 281 283 293 307 311 313 317 331 337 347 349 353 359 367 373 379 383 389 397 401 409 419 421 431 433 439 443 
    449 457 461 463 467 479 487 491 499 503 509
    Time taken to count primes  :0.008000 Seconds
    
    Thread 3 : 521 523 541 547 557 563 569 571 577 587 593 599 601 607 613 617 619 631 641 643 647 653 659 661 673 677 683 691 701 709 719 727 
    733 739 743 751 757 761
    Time taken to count primes  :0.003000 Seconds
    
    Thread 4 : 769 773 787 797 809 811 821 823 827 829 839 853 857 859 863 877 881 883 887 907 911 919 929 937 941 947 953 967 971 977 983 991 
    997 1009 1013 1019 1021
    Time taken to count primes  :0.005000 Seconds
    
    Total num of Primes: 173

*/