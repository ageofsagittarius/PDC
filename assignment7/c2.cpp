/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Matrix multiplication with vector:
		Time is calculated using clock() function
	
	Method Used: Parallel using Pthread
*/

#include <bits/stdc++.h>
#include <pthread.h>
#include <omp.h>
#include <sstream>
#include <fstream>

using namespace std;

struct data_structure {
	vector<vector<double>> mat1;
	vector<vector<double>> mat2;
	
    int left, right;
};

int numOfThreads;
vector<vector<double>> ans;

void printMatrix(vector<vector<double>> &mat) {
    for (int r = 0; r < mat.size(); r++){
        for(int c = 0; c < mat[0].size(); c++){
            cout<<mat[r][c]<<" ";
        }
        cout<<endl;
    }
}

void *CalculateProd(void *str) {
	data_structure *curData = (data_structure *)str;
	
    vector<vector<double>> mat1 = curData -> mat1;
	vector<vector<double>> mat2 = curData -> mat2;
	
    int left = curData -> left;
	int right = curData -> right;

	for (int i = left; i < right; i++)
    {
		for (int j = mat1[2][i]; j < mat1[2][i + 1]; j++)
        {
			int cid = mat1[1][j];
			for (int k = 0; k < mat2.size(); k++)
            {
				ans[i][k] += mat1[0][j] * mat2[cid][k];
			}
		}
	}

	// printMat(P_prod);
	return NULL;
}

int main(){
    
    ifstream fileinput("inputfile.mtx");
    int rows, columns, lines;

    while(fileinput.peek() == '%')
        fileinput.ignore(2048, '\n');

    // reading first line that stores num of rows, num of columns and num of lines
    fileinput>>rows>>columns>>lines;

    // Create your matrix:
    vector<vector<double>> mat(rows, vector<double> (columns, 0.0));

    // reading data from lines
    for (int i = 0; i < lines; i++)
    {
        int r, c; double value;
        fileinput>>r>>c>>value;
        mat[r - 1][c - 1] = value;
    }

    fileinput.close();

    // printMatrix(mat);

    vector<double> arr1;
    vector<double> arr2;
    vector<double> arr3;
    
    int nonZero = 0;
 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (mat[i][j] != 0) {
                arr1.push_back(mat[i][j]);
                arr3.push_back(j);
                // Count Number of Non Zero elements in row i
                nonZero++;
            }
        }
        arr2.push_back(nonZero);
    }

	for(int i = 0; i < arr1.size(); i++){
		cout<<arr1[i]<<" ";
	}
	cout<<endl<<endl;
	for(int i = 0; i < arr2.size(); i++){
		cout<<arr2[i]<<" ";
	}
	cout<<endl<<endl;
	for(int i = 0; i < arr3.size(); i++){
		cout<<arr3[i]<<" ";
	}
	cout<<endl<<endl;

    ifstream myfile;
	myfile.open("matrix.txt");

	vector<vector<double>> vec;

	int count = 0;
	vector<double> temp;
	while (!myfile.eof()) {
		string a = "";
		getline(myfile, a, ',');
		for (int i = 0; i < a.size(); i++) {
			if (a[i] == '.')
				continue;
			if (a[i] < '0' or a[i] > '9')
				a.erase(a.begin() + i);
		}
		if (count == rows) {
			vec.push_back(temp);
			temp.clear();
			count = 0;
		} else
			temp.push_back(stod(a)), count++;
	}
	vec.push_back(temp);

	myfile.close();
    // printMatrix(vec);

    
    // starting time
    clock_t begin = clock();

	int m = vec.size(), n = vec[0].size();

	ans.resize(m, vector<double>(n, 0.0));

	cout << "Enter the number of threads : ";
	cin >> numOfThreads;

	pthread_t threads[numOfThreads];

	int left = 0, right = arr1.size() / numOfThreads;

	vector<vector<double>> sumMat;
    sumMat.push_back(arr1); sumMat.push_back(arr3); sumMat.push_back(arr2);

	for (int t = 0; t < numOfThreads; t++)
    {
		data_structure *curData = new data_structure();
		curData -> mat1 = sumMat;
		curData -> mat2 = vec;
		curData -> left = left;
		curData -> right = right;

		pthread_create(&threads[t], NULL, CalculateProd, (void *)curData);

		left = right;
		right += arr1.size() / numOfThreads;
	}

	for(int th = 0; th < numOfThreads; th++)
    {
		pthread_join(threads[th], NULL);
	}

	printMatrix(ans);

    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to multiply using Pthread (parallel)  :%.6f Seconds\n",time_spent);

    return 0;
}