/*
	IIT2019038
	Device Specifications -
		Intel(R) Core(TM) i5-8265U CPU @ 1.60GHz
		RAM               :  8 GB
		CPU               :  8
		Threads per core  :  2

	Number of connected components
		Time is calculated using clock() function
	
	Method Used: Parallel using pthreads

    Approach:
        The columns of the grid are divided among the threads and then the result of each thread is merged at last.
        The overlapping components of each thread is merged at last by considering it a graph again
*/

#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

int numOfThreads, n;
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};
int color;

vector<vector<int>> graph;
set<int> groups;

map<int, set<int>> newGraph;
map<int, int> visit;

struct data_structure{
    int left;
    int right;
};

void bfs(int x, int y, int color, int left, int right){

    if(graph[x][y] != 1)
        return;

    queue<pair<int, int>> q;
    q.push({x, y});

    while(!q.empty()){
        int curX = q.front().first;
        int curY = q.front().second;
        q.pop();

        graph[curX][curY] = color;

        for(int i = 0; i < 4; i++){
            int newX = curX + dx[i];
            int newY = curY + dy[i];

            if(newX < 0 or newY < left or newX >= n or newY >= right or graph[newX][newY] != 1)
                continue;
            
            q.push({newX, newY});
        }
    }
}

void *dfs(void *str) {
    struct data_structure * cur_data = (struct data_structure *) str;

    int lft = cur_data -> left;
    int rht = cur_data -> right;

    for(int i = 0; i < n; i++) {
        for(int j = lft; j < rht; j++) {
            if(graph[i][j] == 1){
                groups.insert(color);
                bfs(i, j, color, lft, rht);
                color++;
            }
        }
    }
}

void print() {
    cout << "\n";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cout << graph[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

// void help(int i){
//     if(visit[i])
//         return;

//     visit[i] = true;

//     for (auto it = newGraph[i].begin(); it != newGraph[i].end(); it++){
//         help(*it);
//     }
// }

void traverse(int &node){
    if(visit[node] == 1)
        return;
    
    visit[node] = 1;
    for(auto it: newGraph[node]){
        traverse(it);
    }
}

int main(){
    cout<<"The value of n is: ";
	cin>>n;

    cout<<"The number of threads are: ";
    cin>>numOfThreads;

    color = 2;
    graph.resize(n);
    for(int i = 0; i < n; i++)
    {
        graph[i].resize(n);
    }

    groups.clear();
    newGraph.clear();
    visit.clear();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            graph[i][j] = rand() % 2;
        }
    }

    // print();

    // starting time
    clock_t begin = clock();

    pthread_t threads[numOfThreads];

    struct data_structure work_ranges[numOfThreads];
    
    int current_start, range;
    current_start = 0;
    range = n / numOfThreads;
    
    int cur = 0;
    int limit = n / numOfThreads;
    for(int i = 0; i < numOfThreads; i++) {
        struct data_structure *str = (struct data_structure*) malloc(sizeof(struct data_structure));
        str -> left = cur;
        cur += limit;
        if(i == numOfThreads - 1)
            str -> right = n;
        else
            str -> right = cur;
        
        // cout<<"created and joined\n";

        pthread_create(&threads[i], NULL, dfs, (void *)str);
        pthread_join(threads[i], NULL);
        
        work_ranges[i].left = current_start;
        work_ranges[i].right = current_start + range;
        current_start += range;
    }

    work_ranges[numOfThreads - 1].right = n;
    
    // for(int i = 0; i < numOfThreads; i++) {
    //     pthread_create(&threads[i], NULL, dfs, &work_ranges[i]);
    // }

    // for(int i = 0; i < numOfThreads; i++) {
    //     pthread_join(threads[i], NULL);
    // }

    // print();
    // cout << "hello  --- ";
    
    // for (auto it = groups.begin(); it != groups.end(); it++){
    //     cout << *it << " ";
    // }
    // cout << "\n";
    
    cur = 0;
    limit = n / numOfThreads;
    for(int th = 0; th < numOfThreads - 1; th++){
        cur += limit;
        for(int i = 0; i < n; i++){
            int x = graph[i][cur];
            int y = graph[i][cur - 1];

            if(x == 0 or y == 0) continue;

            newGraph[y].insert(x);
            newGraph[x].insert(y);
        }
    }

    // for (int j=0; j<numOfThreads-1; j++){
    //     // cout << work_ranges[j].right - 1 << ":" << work_ranges[j].right << " ";
    //     for (int i=0; i<n; i++){
    //         if (graph[i][work_ranges[j].right]!=0 and graph[i][work_ranges[j].right-1]!=0) {
    //             newGraph[graph[i][work_ranges[j].right-1]].insert(graph[i][work_ranges[j].right]);
    //             newGraph[graph[i][work_ranges[j].right]].insert(graph[i][work_ranges[j].right-1]);
    //         }
    //     }
    // }
    // cout << "\n";
    // for (auto it = newGraph.begin(); it != newGraph.end(); it++){
    //     cout << it->first << ": ";
    //     for (auto jit = it->second.begin(); jit != it->second.end(); jit++){
    //         cout << *jit << " ";
    //     }
    //     cout << "\n";
    // }
    
    int components = 0;
    for(auto it: groups){
        if(it == 0 or visit[it] == 1)
            continue;
        
        traverse(it);
        components++;
    }
    
    // for (auto it = groups.begin(); it != groups.end(); it++){
    //     if (*it!=0 and !visit[*it]) {
    //         help(*it);
    //         components++;
    //     }
    // }

    cout<< "The number of connected components are: " << components << "\n";

    // ending time
	clock_t end = clock();
    
    // Time taken for complete execution
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\nTime taken to evaluate using sequential  :%.6f Seconds\n",time_spent);
}