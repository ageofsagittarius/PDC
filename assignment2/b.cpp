/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Matrix multiplication with vector:
		Time is calculated using clock() function
	
	Method Used: Pthread
		Number of threads used : 2, 4, 8, 16
*/

#include <bits/stdc++.h>
#include <pthread.h>
#include <stdlib.h>
using namespace std;

#define thread 16

double mat[138][138];
double vec[138];
double ans[138];

struct data{
    int l,r;
};

void *mul(void *str){
    struct data *t = (struct data*)str;
    int ll = t -> l;
    int rl  = t -> r;
    for(int i = ll; i < rl; i++){
        ans[i] = mat[i][i] * vec[i];
    }
}

int main(){

    // starting time
    clock_t begin = clock();

    ifstream fileinput("inputfile.mtx");
    int rows, columns, lines;

    while(fileinput.peek() == '%')
        fileinput.ignore(2048, '\n');

    // reading first line that stores num of rows, num of columns and num of lines
    fileinput>>rows>>columns>>lines;

    memset(mat, 0, sizeof(mat));

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
    //         cout<<mat[r][c]<<" ";
    //     }
    //     cout<<endl;
    // }

    ifstream file("vector.txt");
    string data = "";
    
    int l= 0;
    while(getline(file, data, ','))
    {
        int value = stoi(data);
        vec[l++] = value;
    }

    file.close();

    // for(int i = 0; i < 138; i++)
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

    pthread_t th[thread];
    int lim =0;
    int div = 138/thread;

    for(int i=0;i<thread;i++){
        struct data *temp = (struct data*) malloc(sizeof(struct data));
        temp -> l = lim*div;
        temp -> r = min(138,lim*div+div);
        lim++;
        int  rc = pthread_create(&th[i], NULL, mul, (void *)temp);

        pthread_join(th[i], NULL);
    }

    for(int i = 0; i < 138; i++)
    {
        cout<<ans[i]<<" ";
    }

    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to multiply using pthread  :%.6f Seconds\n",time_spent);
}

/*

    PS C:\Users\ageofsagittarius\Desktop\Assignment 2> g++ b.cpp -o mult -lpthread
    PS C:\Users\ageofsagittarius\Desktop\Assignment 2> ./mult
    0.755918 -0.325148 0.00103017 1.26242 0.252252 -1.49809 -6.79878 2.39239 3.16555 -1.49186 -0.657184 -1.60807 0.378394 -0.748113 -0.0154968 
    0.457518 0.670194 -13.7464 0.474922 0.5395 -7.60159 -0.0699254 0.0210106 1.34126 0.243773 -2.93591 5.1992 -2.84595 1.83699 2.22816 1.81867 
    1.31801 -0.519863 0.223857 -0.567833 0.0165912 2.02677 1.50911 13.8692 -2.72047 -1.66686 6.42378 -2.98748 5.41084 26.6226 -4.4864 -2.00862 
    1.35354 -4.83743 4.9911 45.3386 1.8012 4.74796 -35.7178 -3.82431 5.9771 -23.867 2.42517 1.39467 8.21859 0.432578 -0.00318072 31.3845 -1.11912 0.481357 9.74601 -0.681514 -0.232873 0.0213747 0.736132 0.0272722 -1.5241 0.983109 0.22865 -5.67805 -0.0469082 0.350175 3.76285 0.0423265 0.146179 5.25021 -0.859184 -0.827984 -2.48407 0.268577 -0.541081 1.60469 -0.510899 -0.368184 -1.91057 -3.86654 0.679605 -6.55086 2.30441 
    -2.27205 -9.45656 -4.68618 3.39554 23.2404 1.94275 -0.985837 28.1608 1.39576 -4.23256 30.7589 1.77477 -4.43803 35.8965 1.29992 3.98559 -13.6084 -5.04825 3.39716 31.081 0.17257 0.303916 -0.251996 1.00099 0.606965 1.83105 -0.17105 0.735471 1.31551 -0.73438 0.803324 1.89861 -0.874013 0.937502 0 0 0 0 0 0 0 0 0 0
    Time taken to multiply using pthread  :0.026000 Seconds

    Thread       Time
    16           0.00100 s
    8            0.00178 s
    4            0.00219 s
    2            0.00359 s

*/
