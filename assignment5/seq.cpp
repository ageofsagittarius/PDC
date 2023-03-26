/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Number of connected components
		Time is calculated using clock() function
	
	Method Used: Sequential
*/


#include <bits/stdc++.h>
using namespace std;

void bfs(vector<vector<int>> &graph, vector<int> &vis, int &node, int &n){
    vis[node] = 1;
    queue<int> q;
    q.push(node);

    while(!q.empty()){
        int cur = q.front();
        q.pop();

        for(int i = 0; i < n; i++){
            if(vis[i] == 1 or graph[cur][i] == 0) continue;
            q.push(i);
            vis[i] = 1;
        }
    }
}

int main(){
    cout<<"The value of n is: ";
	int n;
	cin>>n;

	vector<vector<int>> graph(n, vector<int> (n));
	for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            graph[i][j] = rand() % 2;
        }
    }

    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < n; j++){
    //         cout<<graph[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }

    // starting time
    clock_t begin = clock();

    int ans = 0;

    vector<int> vis(n, 0);
    for(int i = 0; i < n; i++){
        if(vis[i] == 0){
            bfs(graph, vis, i, n);
            ans++;
        }
    }

    cout<<"The number of connected components are: "<<ans<<endl;

    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to evaluate using sequential  :%.6f Seconds\n",time_spent);
}

/*
    The number of nodes are: 1000
    The number of connected components are: 1

    Time taken to evaluate using sequential  :0.004000 Seconds
*/

/*
    The number of nodes are: 10000
    The number of connected components are: 3

    Time taken to evaluate using sequential  :0.453000 Seconds
*/