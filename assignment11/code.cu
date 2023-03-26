
/*
    IIT2019038
    Ritik Mehra
    
    PDC Assignment 11: Implementation of histogramming parallel algorithms
*/

#include<bits/stdc++.h>
using namespace std;

int N = 1024;
int numOfBins = 16;
int segmentSize = (256 / numOfBins);


// parallel algorithm1
__global__ void algo1(int* input , int *output, int N, int segmentSize){
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    int idx = input[id];
    atomicAdd(&output[idx / segmentSize] , 1);
}

void parallelAlgo1(int *input, int *answer){
    int *output = (int*) malloc(numOfBins * sizeof(int));
    
    for(int i = 0; i < numOfBins; i++){
        output[i] = 0;
    }
    
    clock_t begin = clock();
    printf("\n*** Algo1 ***\n");
    
    //Allocating the memory on the device
    int *arr, *ans;
    
    cudaMalloc(&arr, sizeof(int) * N);
    cudaMalloc(&ans, sizeof(int) * numOfBins);
    
    //Copying the array to the device
    cudaMemcpy(arr, input, sizeof(int) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(ans, output, sizeof(int) * numOfBins, cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    
    int numOfThreads = 512;
    int blockSize = 2 * numOfThreads;
    int n;
    if(N % blockSize == 0){
        n = N;
    }
    else{
        n = (1 + N / blockSize) * blockSize;
    }
    int gridSize = n/blockSize;
    numOfThreads = N;
    algo1<<<gridSize, numOfThreads>>>(arr, ans, N, segmentSize);
    cudaDeviceSynchronize();

    // Copy the result back
    cudaMemcpy(output, ans, numOfBins * sizeof(int), cudaMemcpyDeviceToHost);
    
    // ending time
    clock_t end = clock();
    
    printf("The resultant histogram using algo1 is: \n");
    for(int i = 0; i < numOfBins; i++){
        cout<<output[i]<<" \t";
    }
    cout<<endl;

    bool flag = true;
    for(int i = 0; i < numOfBins; i++){
        if(output[i] != answer[i]){
            flag = false;
            break;
        }
    }
    
    if(flag) printf("CORRECT\n");
    else printf("INCORRECT\n");
    
    //Free the allocated memory
    cudaFree(arr);
    cudaFree(ans);
    
    free(output);
    free(input);
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution Time  :%.6f Seconds\n", time_spent);
}


// parallel algorithm 2: Using global memory
// Both the input data and output array are stored in the GPU global memory area.
__global__ void algo2(int* input, int* output, int N, int segmentSize) {
    int blockId = blockIdx.x * blockDim.x;
    int threadId = threadIdx.x;
    
    // find the id
    int arrayId = blockId + threadId;
    
    int jump = gridDim.x * blockDim.x;
    
    int currIdx;
    for(int i = arrayId; i < N; i += jump) {
        // find the index
        currIdx = input[i];
        atomicAdd(&output[currIdx / segmentSize], 1);
    }
}

void parallelAlgo2(int *input, int *answer){
    int *output = (int*) malloc(numOfBins * sizeof(int));
    
    for(int i = 0; i < numOfBins; i++){
        output[i] = 0;
    }
    
    clock_t begin = clock();
    printf("\n*** Algo2 ***\n");
    
    //Allocating the memory on the device
    int *arr, *ans;
    
    cudaMalloc(&arr, sizeof(int) * N);
    cudaMalloc(&ans, sizeof(int) * numOfBins);
    
    //Copying the array to the device
    cudaMemcpy(arr, input, sizeof(int) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(ans, output, sizeof(int) * numOfBins, cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    
    int numOfThreads = 512;
    int blockSize = 2 * numOfThreads;
    int n;
    if(N % blockSize == 0){
        n = N;
    }
    else{
        n = (1 + N / blockSize) * blockSize;
    }
    int gridSize = n/blockSize;
    
    algo2<<<gridSize, numOfThreads>>>(arr, ans, N, segmentSize);
    cudaDeviceSynchronize();

    // Copy the result back
    cudaMemcpy(output, ans, numOfBins * sizeof(int), cudaMemcpyDeviceToHost);
    
    // ending time
    clock_t end = clock();
    
    printf("The resultant histogram using algo2 is: \n");
    for(int i = 0; i < numOfBins; i++){
        cout<<output[i]<<" \t";
    }
    cout<<endl;

    bool flag = true;
    for(int i = 0; i < numOfBins; i++){
        if(output[i] != answer[i]){
            flag = false;
            break;
        }
    }
    
    if(flag) printf("CORRECT\n");
    else printf("INCORRECT\n");
    
    //Free the allocated memory
    cudaFree(arr);
    cudaFree(ans);
    
    free(output);
    free(input);
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution Time  :%.6f Seconds\n", time_spent);
}


// parallel algorithm 3: Using shared memory
// Each thread here has its own private copy of the bucket array
__global__ void algo3(int* input, int* output, int *res, int N, int segmentSize, int numOfBins) {
    int blockId = blockIdx.x * blockDim.x;
    int threadId = threadIdx.x;
    
    // find the id
    int arrayId = blockId + threadId;
    
    // Initalize the shared memory to 0
    if (threadId < numOfBins) {
        res[threadId] = 0;
    }
    
    __syncthreads();
    
    int jump = gridDim.x * blockDim.x;
    
    for (int i = arrayId; i < N; i += jump) {
        //find the idx for histogram
        int currIdx = input[i];
        atomicAdd(&res[(currIdx / segmentSize)], 1);
    }
    
    __syncthreads();

    //store the results back
    if (threadId < numOfBins) {
        atomicAdd(&output[threadId], res[threadId]);
    }
}

void parallelAlgo3(int *input, int *answer){
    int *output = (int*) malloc(numOfBins * sizeof(int));
    
    for(int i = 0; i < numOfBins; i++){
        output[i] = 0;
    }
    
    printf("\n*** Algo3 ***\n");
    
    //Allocating the memory on the device
    int *arr, *ans, *res;
    
    cudaMalloc(&arr, sizeof(int) * N);
    cudaMalloc(&ans, sizeof(int) * numOfBins);
    cudaMalloc(&res, sizeof(int) * numOfBins);
    
    //Copying the array to the device
    cudaMemcpy(arr, input, sizeof(int) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(ans, output, sizeof(int) * numOfBins, cudaMemcpyHostToDevice);
    cudaDeviceSynchronize();
    
    int numOfThreads = 512;
    int blockSize = 2 * numOfThreads;
    int n;
    if(N % blockSize == 0){
        n = N;
    }
    else{
        n = (1 + N / blockSize) * blockSize;
    }
    int gridSize = n/blockSize;
    
    clock_t begin = clock();
    
    algo3<<<gridSize, numOfThreads>>>(arr, ans, res, N, segmentSize, numOfBins);
    cudaDeviceSynchronize();

    // Copy the result back
    cudaMemcpy(output, ans, numOfBins * sizeof(int), cudaMemcpyDeviceToHost);
    
    // ending time
    clock_t end = clock();
    
    printf("The resultant histogram for algo3 is: \n");
    for(int i = 0; i < numOfBins; i++){
        cout<<output[i]<<" \t";
    }
    cout<<endl;
    
    bool flag = true;
    for(int i = 0; i < numOfBins; i++){
        if(output[i] != answer[i]){
            flag = false;
            break;
        }
    }
    
    if(flag) printf("CORRECT\n");
    else printf("INCORRECT\n");
    
    //Free the allocated memory
    cudaFree(arr);
    cudaFree(ans);
    cudaFree(res);
    
    free(output);
    free(input);
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution Time  :%.6f Seconds\n", time_spent);
}

int main() {
    
    // starting time
    clock_t begin = clock();
    
    printf("The size of the array is: %d \n", N);
    printf("#bins: %d \n", numOfBins);

    //Create pointers and allocate arrays
    int *input = (int*) malloc(N * sizeof(int));
    int *input1 = (int*) malloc(N * sizeof(int));
    int *input2 = (int*) malloc(N * sizeof(int));
    int *input3 = (int*) malloc(N * sizeof(int));
    
    //Fill the input array with numbers
    for(int i = 0; i < N; i++){ 
        input[i] = rand() % 256;
        input1[i] = input[i];
        input2[i] = input[i];
        input3[i] = input[i];
    }
    for(int i = 0; i < N; i++){
        input1[i] = input[i];
    }
    for(int i = 0; i < N; i++){
        input2[i] = input[i];
    }
    for(int i = 0; i < N; i++){
        input3[i] = input[i];
    }
    
    printf("The input array is: \n");
    for(int i = 0; i < N; i++){
        cout<<input[i]<<" \t";
    }
    cout<<endl;
    
    // sequential
    printf("\n*** Sequential ***\n");
    
    int *seqOutput = (int*) malloc(numOfBins * sizeof(int));
    
    for(int i = 0; i < N; i++){
        int currIdx = input[i] / segmentSize;
        seqOutput[currIdx] += 1;
    }
    
    printf("The resultant histogram for using sequential approach is: \n");
    for(int i = 0; i < numOfBins; i++){
        cout<<seqOutput[i]<<" \t";
    }
    cout<<endl;
    
    // ending time
    clock_t end = clock();
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution Time  :%.6f Seconds\n", time_spent);
    
    
    // parallel algorithm 1
    parallelAlgo1(input1, seqOutput);
    
    
    // parallel algorithm 2
    parallelAlgo2(input2, seqOutput);
    
    
    // parallel algorithm 2
    parallelAlgo3(input3, seqOutput);
    
    
    return 0;
}

/*
    The output for all the three algorithms does match with that of the sequential and hence is "CORRECT".
    
    The fastest algorithm is the Parallel Algo 3. It took 0.000030 Seconds to execute as compared to algo2 
    and algo1 which took 0.000204 Seconds and 0.172672 Seconds to execute.
    
    In the algorithm 3 it uses, the GPU shared memory. Every thread is given its own copy of the result
    array. The input array is stored in the global memory and the thread's own result array is stored in the
    shared memory that leads to faster calculation. Finally when all the threads are done with processing and are synchronized the final
    result is updated with the sum of all of the results obtained from the threads.
*/
