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
		Number of threads used : 2, 4, 8, 16
*/

#include <bits/stdc++.h>
using namespace std;

int main(){

    // starting time
    clock_t begin = clock();

    ifstream fileinput("inputfile.mtx");
    int rows, columns, lines;

    while(fileinput.peek() == '%')
        fileinput.ignore(2048, '\n');

    // reading first line that stores num of rows, num of columns and num of lines
    fileinput>>rows>>columns>>lines;

    // Create your matrix:
    vector<vector<double>> mat(rows, vector<double> (columns, 0));

    // reading data from lines
    for (int i = 0; i < lines; i++)
    {
        int r, c; double value;
        fileinput>>r>>c>>value;
        mat[r - 1][c - 1] = value;
    }

    fileinput.close();

    // for (int r = 0; r < rows; r++){
    //     for(int c = 0; c < columns; c++){
    //         // cout << mat[m + n * columns]<< ' ';
    //         cout<<mat[r][c]<<" ";
    //     }
    //     cout<<endl;
    // }

    ifstream file("vector.txt");
    string data = "";
    vector<int> vec;
    
    while(getline(file, data, ','))
    {
        int value = stoi(data);
        vec.push_back(value);
    }

    file.close();

    // for(int i = 0; i < vec.size(); i++)
    // {
    //     cout<<vec[i]<<", ";
    // }


    vector<int> A;
    vector<int> IA = { 0 }; // IA matrix has N+1 rows
    vector<int> JA;
    int nonZero = 0;
 
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (mat[i][j] != 0) {
                A.push_back(mat[i][j]);
                JA.push_back(j);
                // Count Number of Non Zero
                // Elements in row i
                nonZero++;
            }
        }
        IA.push_back(nonZero);
    }


    vector<double> ans;
    for (int i = 0; i < rows; i++) 
    {
        double curCell = 0;
        for(int j = 0; j < rows; j++)
        {
            curCell += (mat[i][j] * vec[j]);
        }
        ans.push_back(curCell);
    }

    for(int i = 0; i < ans.size(); i++)
    {
        cout<<ans[i]<<" ";
    }

    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to multiply using sequential  :%.6f Seconds\n",time_spent);
}

/*

    [Running] cd "c:\Users\ageofsagittarius\Desktop\Assignment 2\" && g++ a.cpp -o a && "c:\Users\ageofsagittarius\Desktop\Assignment 2\"a
    
    0.755918 -0.325148 0.00103017 1.26242 0.252252 -1.49809 -6.79878 2.39239 3.16555 -1.49186 -0.657184 -1.60807 
    0.378394 -0.748113 -0.0154968 0.457518 0.670194 -13.7464 0.474922 0.5395 -7.60159 -0.0699254 0.0210106 1.34126 
    0.243773 -2.93591 5.1992 -2.84595 1.83699 2.22816 1.81867 1.31801 -0.519863 0.223857 -0.567833 0.0165912 
    2.02677 1.50911 13.8692 -2.72047 -1.66686 6.42378 -2.98748 5.41084 26.6226 -4.4864 -2.00862 1.35354 
    -4.83743 4.9911 45.3386 1.8012 4.74796 -35.7178 -3.82431 5.9771 -23.867 2.42517 1.39467 8.21859 0.432578 
    -0.00318072 31.3845 -1.11912 0.481357 9.74601 -0.681514 -0.232873 0.0213747 0.736132 0.0272722 -1.5241 
    0.983109 0.22865 -5.67805 -0.0469082 0.350175 3.76285 0.0423265 0.146179 5.25021 -0.859184 -0.827984 
    -2.48407 0.268577 -0.541081 1.60469 -0.510899 -0.368184 -1.91057 -3.86654 0.679605 -6.55086 2.30441 -2.27205 
    -9.45656 -4.68618 3.39554 23.2404 1.94275 -0.985837 28.1608 1.39576 -4.23256 30.7589 1.77477 -4.43803 35.8965 
    1.29992 3.98559 -13.6084 -5.04825 3.39716 31.081 0.17257 0.303916 -0.251996 1.00099 0.606965 1.83105 -0.17105 
    0.735471 1.31551 -0.73438 0.803324 1.89861 -0.874013 0.937502 1.09508 0.618092 -0.355409 -5.73256 0.710163 
    0.959974 0.932712 0.30667 -0.0495628 5.35799 
    
    Time taken to multiply using sequential  :0.004000 Seconds

*/
