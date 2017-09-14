// MinEdgeForEulerian.cpp : Defines the entry point for the console application.
/*
Given a graph containing N nodes and M bi-directional edges, where each edge connects two different nodes. 
Find out the minimum number of new edges to be constructed such that you can start from node 1 and visit all 
the edges and return to node 1, and in this process you should not traverse any edge more than once. Remember 
that there can be multiple edges between two nodes and the graph can have disconnected components, and it should
also be ensured that all the nodes in the graph are visited.
http://practice.geeksforgeeks.org/problems/connecting-nodes/0
*/

#include "stdafx.h""
#include <iostream>
#include <stack>
#include <vector>


using namespace std;

int max(int a, int b) { return a > b ? a : b;  }

void update_degree(vector<int> &degree, vector<vector<int> > &graph)
{
	for (int i = 1; i<graph.size(); i++)
	{
		vector<int> adj = graph[i];
		for (int j = 0; j<adj.size(); j++)
		{
			degree[adj[j]]++;
		}
	}
}

void DFS(int v, vector<vector<int> > &graph, vector<bool> &visited, stack<int> &S)
{
	visited[v] = true;
	vector<int> adj = graph[v];
	for (int i = 0; i<adj.size(); i++)
	{
		if (!visited[adj[i]])
		{
			DFS(adj[i], graph, visited, S);
			S.push(adj[i]);
		}
	}
}

int DFS_CountOddVertices(int v, vector<vector<int> > &graph, vector<int> &degree, vector<bool> &visited)
{
	//cout << v << " ";
	visited[v] = true;
	int ans = 0;
	if (degree[v] % 2 == 1) ans++;
	vector<int> adj = graph[v];
	for (int i = 0; i<adj.size(); i++)
	{
		if (!visited[adj[i]])
		{
			ans += DFS_CountOddVertices(adj[i], graph, degree, visited);
		}
	}

	return ans;
}

int min_edge_required(int N, vector<vector<int> > &graph)
{
	//1. Count of num of SCCs
	//2. count of num of vertices with odd degree in each SCCs
	vector<int> degree(N + 1, 0);
	update_degree(degree, graph);
	//print Degree
	/*
	for(int i=1; i<=N; i++)
	{
	cout << "Degree["<<i<<"] = " << degree[i] << endl;
	}
	*/
	//Now count of the SCCs
	stack<int> S;
	vector<bool> visited(N + 1, false);
	for (int i = 1; i <= N; i++)
	{
		if (!visited[i])
		{
			DFS(i, graph, visited, S);
			S.push(i);
		}
	}
	//Now Transpose the graph
	vector<vector<int> > t(N + 1);
	for (int i = 1; i<graph.size(); i++)
	{
		vector<int> adj = graph[i];
		for (int j = 0; j<adj.size(); j++)
		{
			t[adj[j]].push_back(i);
		}
	}

	//reset the visited
	visited = vector<bool>(N + 1, false);

	//Now pop the item from stack and start visiting the transpose graph
	int oddVertices = 0;
	while (!S.empty())
	{
		int v = S.top();
		S.pop();
		if (!visited[v])
		{
			//Now this is a new SCCs..count the odd num of vertices
			int temp = DFS_CountOddVertices(v, graph, degree, visited);
			//cout << "Odd  Vertices : " << temp << endl;
			oddVertices += max(2, temp);
			//cout << endl;
		}
	}

	return oddVertices / 2;
}

int main() {

	int N, M;
	cin >> N >> M;
	vector<vector<int> > graph(N + 1);
	int u, v;
	for (int i = 1; i <= M; i++)
	{
		cin >> u >> v;
		graph[v].push_back(u);
		graph[u].push_back(v);
	}

	cout << min_edge_required(N, graph) << endl;
	
	return 0;
}
