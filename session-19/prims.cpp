#include<bits/stdc++.h>
using namespace std;

int spanningTree(int V, vector<vector<int>>& edges) {

    vector<vector<pair<int,int>>> adj(V);

    for (auto &e : edges) {
        int u = e[0], v = e[1], w = e[2];
        adj[u].push_back({w,v});
        adj[v].push_back({w,u});
    }

    vector<bool> visited(V,false);

    priority_queue<pair<int,int>,
                   vector<pair<int,int>>,
                   greater<pair<int,int>>> pq;

    pq.push({0,0});
    int minCost = 0;
    int countVisited = 0;

    while(!pq.empty()) {

        auto top = pq.top();
        pq.pop();

        int weight = top.first;
        int node = top.second;

        if(visited[node]) continue;

        minCost += weight;
        visited[node] = true;
        countVisited++;

        for(auto &pr : adj[node]) {
            int nextW = pr.first;
            int nextV = pr.second;

            if(!visited[nextV])
                pq.push({nextW,nextV});
        }
    }

    // check connectivity
    if(countVisited != V)
        return -1;

    return minCost;
}

int main() {
    int V, E;

    cout<<"Enter number of vertices and edges: ";
    cin >> V >> E;

    vector<vector<int>> edges(E, vector<int>(3));

    cout<<"Enter edges (u v w):"<<endl;
    for(int i = 0; i < E; i++) {
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }

    int ans = spanningTree(V, edges);

    if(ans == -1)
        cout<<"Graph is disconnected, MST not possible";
    else
        cout<<"MST cost = "<<ans;

    return 0;
}

