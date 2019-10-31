#include <bits/stdc++.h>
using namespace std;

void topological_sort(int src,vector<vector<int>>& graph,vector<bool>& vis,vector<int>& stack){
    vis[src]=true;
    for(int e=0;e<graph[src].size();e++){
        int nbr=graph[src][e];
        if(vis[nbr]==false){
            topological_sort(nbr,graph,vis,stack);
        }
    }
    stack.push_back(src);
}
int main() {
    vector<vector<int>> graph{
        {1,4},{2},{3},{},{3},{4,6},{3}
    };
    vector<int> res;
    vector<bool> visited(graph.size(),false);

    for(int i=0;i<graph.size();i++){
        if(visited[i]==false)
       topological_sort(i,graph,visited,res);
    }
    for(int i=res.size()-1;i>=0;i--){
        cout<<res[i]<<" ";
    }
	return 0;
}
