
#include<bits/stdc++.h>
using namespace std;

__global__ void prefixScanHillis(int* input, int* output, int offset, int size){

    // calculating the index of the array
    int idx = (gridDim.x * blockDim.x) * blockIdx.y + blockDim.x * blockIdx.x + threadIdx.x;
    
    // checking for the index being out of bounds
    if (idx < size){
        output[idx] = input[idx];
        if (idx >= offset){
            output[idx] *= input[idx - offset];
        }
    } 
}

int main(){

    // the size of the array
    int size = 8;
    
    printf("The size of the array is: %d \n", size);

    //Create pointers and allocate arrays
    int *input = (int*) malloc(size * sizeof(int));
    int *output = (int*) malloc(size * sizeof(int));
    
    int *arr, *ans;
    
    cudaMalloc(&arr, size * sizeof(int));
    cudaMalloc(&ans, size * sizeof(int));
    
    //Fill the input array with numbers
    for(int i = 0; i < size; i++){ 
        input[i] = i + 1;
    }
    
    printf("\nThe input array is: \n");
    for(int i = 0; i < size; i++){
        cout<<input[i]<<" \t";
    }
    cout<<endl;
    
    printf("\nNow Copying the memory to device.\n");
    cudaMemcpy(arr, input, sizeof(int) * size, cudaMemcpyHostToDevice);
    cudaMemcpy(ans, output, sizeof(int) * size, cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    
    int numOfThreads = 1024;
    int blockSize = 2 * numOfThreads;
    int n;
    if(size % blockSize == 0){
        n = size;
    }
    else{
        n = (1 + size / blockSize) * blockSize;
    }
    int gridSize = n/blockSize;
    
    int offset = 1, limit = 2 * size;
    //int gridSize = 1, blockSize = size;
    int* temp;
    while(offset < limit){

        prefixScanHillis <<<gridSize, numOfThreads>>>(arr, ans, offset, size);
        cudaDeviceSynchronize();

        offset = offset * 2;

        temp = arr;
        arr = ans;
        ans = temp;
    }

    printf("\nCopying the results back.\n");
    cudaMemcpy(output, ans, size * sizeof(int), cudaMemcpyDeviceToHost);
    
    printf("\nThe resultant prefixSum array is: \n");
    for(int i = 0; i < size; i++){
        cout<<output[i]<<" \t";
    }
    cout<<endl<<endl;

    //Free the allocated memory
    cudaFree(arr);
    cudaFree(ans);
    
    free(input);
    free(output);
    
    return 0;
}
