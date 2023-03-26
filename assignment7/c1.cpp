/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Matrix multiplication with vector:
		Time is calculated using clock() function
	
	Method Used: Sequential
*/

#include <bits/stdc++.h>
#include <pthread.h>
#include <omp.h>
#include <sstream>
#include <fstream>

using namespace std;

void printMatrix(vector<vector<double>> &mat) {
    for (int r = 0; r < mat.size(); r++){
        for(int c = 0; c < mat[0].size(); c++){
            cout<<mat[r][c]<<" ";
        }
        cout<<endl;
    }
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

	vector<vector<double>> ans(m, vector<double>(n, 0.0));

	for (int i = 0; i < arr2.size() - 1; i++) {
		for (int j = arr2[i]; j < arr2[i + 1]; j++) {
			int cid = arr3[j];
			for (int k = 0; k < n; k++) {
				ans[i][k] += arr1[j] * vec[cid][k];
			}
		}
	}

	printMatrix(ans);

    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to multiply using sequential  :%.6f Seconds\n",time_spent);

    return 0;
}