/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Merge two sorted vectors
		Time is calculated using clock() function
	
	Method Used: Sequential

    User Inputs:
        size of arr1, elements of arr1
        size of arr2, elements of arr2
*/

#include <bits/stdc++.h>
using namespace std;

int n, m;
int t = 0;
vector<int> arr1, arr2;
vector<int> res;

void merge(){
    int l = 0, r = 0;
    while(l < arr1.size() or r < arr2.size()){
        if(l < arr1.size() and r < arr2.size()){
            if(arr1[l] < arr2[r])
                res[t++] = arr1[l++];
            else
                res[t++] = arr2[r++];
        }
        else if(l < arr1.size())
            res[t++] = arr1[l++];
        else
            res[t++] = arr2[r++];
    }
}

int main(){

    cout << "The size of array 1 is: ";
    cin >> n;
    arr1.resize(n);
    cout << "Enter array elements: ";
    for(int i = 0; i < n; i++) cin>>arr1[i];

    cout << "The size of array 2 is: ";
    cin >> m;
    cout << "Enter array elements: ";
    arr2.resize(m);
    for(int i = 0; i < m; i++) cin>>arr2[i];
    
    res.resize(n+m);

    // starting time
    clock_t begin = clock();

    merge();
    for(auto &s: res)cout<<s<<"  ";

    // ending time
	clock_t end = clock();
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime taken to multiply using sequential  :%.6f Seconds\n", time_spent);

    return 0;
}

/*
    PS C:\Users\ageofsagittarius\Desktop\Sem 7\PDC\IIT2019038_Assignment3> g++ seq.cpp -std=c++11
    PS C:\Users\ageofsagittarius\Desktop\Sem 7\PDC\IIT2019038_Assignment3> ./a.exe
    The size of array 1 is: 5
    Enter array elements: 1 2 3 4 5
    The size of array 2 is: 3
    Enter array elements: 1 2 3
    1  1  2  2  3  3  4  5  
    Time taken to multiply using sequential  :0.002000 Seconds
*/