
#include <bits/stdc++.h>
using namespace std;


__global__ void func(double *one,int * two,int *three,double *four,double *five,int row)
{   
    int start = blockIdx.x * blockDim.x + threadIdx.x;
    int jump = blockDim.x * gridDim.x;
    
    for (int i = start; i < row; i += jump) {
        double sum = 0.0;
    
        for (int j = two[i]; j < two[i+1]; j++) {
            sum += one[j] * five[three[j]];
        }
        
        four[i] = sum;
    }
}

int main()
{
    ifstream matrixStream;
    matrixStream.open("../input/assignment9-dataset/inputfile.mtx");

    // ignoring the unnecessary things
    while (matrixStream.peek() == '%')
        matrixStream.ignore(2048, '\n');

    int row, column, count_v, r, c;
    double k;

    // taking row col and number of nonzero elements
    matrixStream >> row >> column >> count_v;

    cout << "Rows: " << row << " "
         << "Columns: " << column << " Non zero elements: " << count_v << endl;

    double **matrix = (double **)malloc(row * sizeof(double *));

    for (int i = 0; i < row; i++)
    {
        matrix[i] = (double *)malloc(column * sizeof(double));
    }

    // reading the elemnts of the matrix
    for (int i = 0; i < row; i++)
    {
        matrixStream >> r >> c >> k;
        r--;
        c--;
        matrix[r][c] = k;
    }

    // reading from the elements
    // reading from txt file
    ifstream vector_stream;
    vector_stream.open("../input/assignment9-dataset/vector.txt");

    double *in_vector = (double *)malloc(row * sizeof(double));

    string data = "";
    for (int i = 0; i < row; i++)
    {
        getline(vector_stream, data, ',');
        in_vector[i] = stod(data);
    }
    
    printf("The given vector is: \n");
    for(int i = 0; i < column; i++){
        cout<<in_vector[i]<<", ";
    }
    cout<<endl<<endl;
    

    double *matrixValues = (double *)malloc(count_v * sizeof(double));
    int *rMatrix = (int *)malloc(row + 1 * sizeof(int));
    int *cMatrix = (int *)malloc(count_v * sizeof(int));
    double *answer = (double *)malloc(row * sizeof(double));
    
    for (int i = 0; i < count_v; i++)
    {
        matrixValues[i] = 0;
        cMatrix[i] = 0;
    }
    int ind = 0;
    int cnt = 0;
    rMatrix[0] = 0;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (matrix[i][j] != 0)
            {
                matrixValues[ind++] = matrix[i][j];
                cnt++;
            }
        }

        rMatrix[i + 1] = cnt;
    }

    for (int i = 0; i < count_v; i++) cMatrix[i] = i;
    
    double *one = (double *)malloc(count_v * sizeof(double));
    int *two = (int *)malloc(row + 1 * sizeof(int));
    int *three = (int *)malloc(count_v * sizeof(int));
    double *four = (double *)malloc(row * sizeof(double));
    double *five = (double *)malloc(row * sizeof(double));
    
    // Allocate device memory 
    cudaMalloc((void**)&one, sizeof(double) * count_v);
    cudaMalloc((void**)&two, sizeof(int) * row+1);
    cudaMalloc((void**)&three, sizeof(int) * count_v);
    cudaMalloc((void**)&four, sizeof(double) * row);
    cudaMalloc((void**)&five,sizeof(double)*row);
    
    // Transfer data from host to device memory
    cudaMemcpy(one, matrixValues, sizeof(double) * count_v, cudaMemcpyHostToDevice);
    cudaMemcpy(two, rMatrix, sizeof(int) * row+1, cudaMemcpyHostToDevice);
    cudaMemcpy(three, cMatrix, sizeof(int) * count_v, cudaMemcpyHostToDevice);
    cudaMemcpy(four, answer, sizeof(double) * row, cudaMemcpyHostToDevice);
    cudaMemcpy(five, in_vector, sizeof(double) * row, cudaMemcpyHostToDevice);
    
    // Executing kernel 
    int size_k_b = row;
    int size_g_k = 1;
    func<<<size_g_k,size_k_b>>>(one, two,three,four,five,row);
    
    //Transfer data back to host memory
    cudaMemcpy(answer, four, sizeof(double) * row, cudaMemcpyDeviceToHost);
    
    printf("The CSR format : \n");
    printf("The element array is: \n");
    for(int i = 0; i < count_v; i++){
        cout<<matrixValues[i]<<", ";
    }
    cout<<endl<<endl;
    
    printf("The column array is: \n");
    for(int i = 0; i < count_v; i++){
        cout<<cMatrix[i]<<", ";
    }
    cout<<endl<<endl;
    
    printf("The non-zero array is: \n");
    for(int i = 0; i < count_v; i++){
        cout<<rMatrix[i]<<", ";
    }
    cout<<endl<<endl;
    
    double seq[row];
    
    for (int i = 0; i < row; i++) {
        double sum = 0.0;
        for (int j = rMatrix[i]; j < rMatrix[i+1]; j++) {
            sum += matrixValues[j] * in_vector[cMatrix[j]];
        }
        seq[i] = sum;
    }
    
    printf("The resultant vector found using sequential is: \n");
    for(int i = 0; i < row; i++){
        cout<<seq[i]<<", ";
    }
    cout<<endl<<endl;
    
    printf("The resultant vector found using parallel(cuda) is: \n");
    for(int i = 0; i < row; i++){
        cout<<answer[i]<<", ";
    }
    cout<<endl<<endl;
    
    
    bool flag = true;
    
    for(int i = 0; i < row; i++){
        if(seq[i] != answer[i]){
            flag = false;
            break;
        }
    }
    
    if(flag) cout<<"CORRECT the output of CUDA matches with that of sequential";
    else cout<<"INCORRECT the output doesn't match";
    
    
    return 0;
}
