#include<bits/stdc++.h>
using namespace std;

class DSU{
public:
    vector<int> p, r;
    DSU(int n){
        p.resize(n);
        r.resize(n,0);
        for(int i=0;i<n;i++) p[i]=i;
    }
    int find(int x){
        if(p[x]==x) return x;
        return p[x]=find(p[x]);
    }
    void unite(int a,int b){
        a=find(a);
        b=find(b);
        if(a==b) return;
        if(r[a]<r[b]) p[a]=b;
        else if(r[b]<r[a]) p[b]=a;
        else{
            p[b]=a;
            r[a]++;
        }
    }
};

int spanningTree(int V, vector<vector<int>>& edges){

    sort(edges.begin(),edges.end(),[](auto &a,auto &b){
        return a[2] < b[2];
    });

    DSU dsu(V);
    int minCost = 0;
    int edgesUsed = 0;

    for(auto &e : edges){
        int u = e[0];
        int v = e[1];
        int w = e[2];

        if(dsu.find(u) != dsu.find(v)){
            minCost += w;
            dsu.unite(u,v);
            edgesUsed++;
        }
    }

    if(edgesUsed != V-1)
        return -1;

    return minCost;
}

int main(){
    int V,E;

    cout<<"Enter number of vertices and edges: ";
    cin>>V>>E;

    vector<vector<int>> edges(E, vector<int>(3));

    cout<<"Enter edges (u v w):"<<endl;
    for(int i=0;i<E;i++){
        cin>>edges[i][0]>>edges[i][1]>>edges[i][2];
    }

    int ans = spanningTree(V,edges);

    if(ans==-1)
        cout<<"Graph is disconnected, MST not possible";
    else
        cout<<"MST cost = "<<ans;

    return 0;
}