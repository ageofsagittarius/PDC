
#include <bits/stdc++.h>
using namespace std;

__global__ void prefixScanBleloch(int* input, int* output, int* pSum, int *sum, int n, int* up)
{
    int blockID = blockIdx.x * blockDim.x;
    int threadID = threadIdx.x;
    
    int arrayID = blockID + threadID;
    int offset = 1;
    
    sum[2 * threadID]   = input[2 * arrayID];
    sum[2 * threadID + 1] = input[2 * arrayID + 1];
    
    // Upsweep
    for(int i = n / 2; i > 0; i = i / 2){
        __syncthreads();
        if(threadID < i){
            int id1 = offset * (2 * threadID + 1) - 1;
            int id2 = offset * (2 * threadID + 2) - 1;
            sum[id2] *= sum[id1];
        }
        offset = offset * 2;
    }
    
    up[2 * arrayID] = sum[2 * threadID];
    up[2 * arrayID + 1] = sum[2 * threadID + 1];
    
    if(threadID == 0) {
        if(pSum != NULL){
            pSum[blockIdx.x] = sum[n-1];
        }
        sum[n-1] = 1;
    }
    
    // Downsweep
    for (int i = 1; i < n; i = i * 2){
        offset = offset / 2;
        __syncthreads();
        if (threadID < i){
            int id1 = offset * (2 * threadID + 1) - 1;
            int id2 = offset * (2 * threadID + 2) - 1;
            int temp = sum[id1];
            sum[id1]  = sum[id2];
            sum[id2] *= temp;
        }
    }
    __syncthreads();

    output[2 * arrayID] = sum[2 * threadID];
    output[2 * arrayID + 1] = sum[2 * threadID + 1];
}

int main()
{
    // the size of the array
    int size = 8;
    
    printf("The size of the array is: %d \n", size);
    
    //Create pointers and allocate arrays
    int *input = (int*) malloc(size * sizeof(int));
    int *output = (int*) malloc(size * sizeof(int));
    
    int *up = (int*) malloc(size * sizeof(int));
    
    for(int i = 0; i < size; i++){
        input[i] = i + 1;
    }
    
    printf("\nThe input array is: \n");
    for(int i = 0; i < size; i++){
        cout << input[i] << "\t";
    }
    cout<<endl;
    
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
    
    int *arr, *ans, *sum, *newSum, *upPtr;
    cudaMalloc(&arr, sizeof(int) * size);
    cudaMalloc(&upPtr, sizeof(int) * size);
    cudaMalloc(&ans, sizeof(int) * n);
    cudaMalloc(&sum, sizeof(int) * gridSize);
    cudaMalloc(&newSum, sizeof(int) * gridSize);
    
    printf("\nNow Copying the memory to device.\n");
    cudaMemcpy(arr, input, sizeof(int) * size, cudaMemcpyHostToDevice);
    cudaMemcpy(ans, output, sizeof(int) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(upPtr, up, sizeof(int) * n, cudaMemcpyHostToDevice);
    
    printf("\nNow performing scan using cuda\n");
    // perform scan using cuda
    prefixScanBleloch<<<gridSize, numOfThreads>>>(arr, ans, sum, newSum, blockSize, upPtr);
    cudaDeviceSynchronize();
    
    printf("\nCopying the results back.\n");
    // copy scan result back to output
    cudaMemcpy(output, ans, sizeof(int)*size, cudaMemcpyDeviceToHost);
    
    cudaMemcpy(up, upPtr, sizeof(int)*size, cudaMemcpyDeviceToHost);
    
    
    printf("\nThe result of the upsweep is: \n");
    for(int i = 0; i < size; i++){
        cout<<up[i]<<" \t";
    }
    cout<<endl;
    
    printf("\nThe resultant prefixSum array is: \n");
    for(int i = 0; i < size; i++){
        cout<<output[i]<<" \t";
    }
    cout<<endl<<endl;

    //Free the allocated memory
    cudaFree(arr);
    cudaFree(ans);
    cudaFree(sum);
    cudaFree(newSum);
    
    free(input);
    free(output);
    
    return 0;
}
