#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

int numOfThreads;
int n;

pthread_mutex_t sumLock;

int sum = 0;

vector<int> arr;

struct data_structure{
    int idx;
    int newIdx;
};

void *fun(void * args){
    struct data_structure *data = (struct data_structure *)args;
	int i = data -> idx;
    int id = data -> newIdx;
    
    pthread_mutex_lock(&sumLock);
    
    int curSum = 0;

    curSum += arr[i];
    curSum += arr[id];

    arr[id] = curSum;
    
    pthread_mutex_unlock(&sumLock);
}

int main(){
    
    // taking the input of array size
    cout<<"Enter the number of elements: ";
    cin>>n;

    arr.resize(n);

    // filling the array with 1 to n elements
    for(int i = 0; i < n; i++){
        arr[i] = i + 1;
    }
    
    // finding array sum using serial method
    // int arrSum = 0;
    // for(auto c: arr)arrSum += c;
    
    pthread_mutex_init(&sumLock, NULL);
    
    pthread_t threads[n];
    
    for(int i = 2; i <= n; i *= 2){
        
        for(int j = 0; j < n; j += i){
            struct data_structure *data = (struct data_structure*)malloc(sizeof(struct data_structure));
            data -> idx = j + i / 2;
            data -> newIdx = j;

            pthread_create(&threads[j], NULL, fun, data);
        }

        for(int j = 0; j < n; j += i){
            pthread_join(threads[j], NULL);
        }
        
    }

    cout<<"The sum of the array is: "<<arr[0]<<endl;
    
    // serial sum
    // cout<<"The sum of the array is: "<<arrSum<<endl;
    
    return 0;
}