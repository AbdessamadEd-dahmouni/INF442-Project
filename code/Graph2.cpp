#include "Graph.hpp"
using namespace std;


Graph::Graph(int V,double p){
	double x;
	for(int i=0;i<V;i++)
			adj[i]=list<int>();
	for(int i=0;i<V;i++)
		for(int j=0;j<V;j++){
			x=rand()/(double) RAND_MAX;
			if(x<p) adj[i].push_back(j);
		}
}

void Graph::printSCCs(ofstream& file)
{
	stack<int> Stack;

	map<int,bool> visited;
	for(map<int,list<int> >::iterator it= adj.begin(); it!=adj.end(); it++)
		visited[it->first] = false;

	for(map<int,list<int> >::iterator it= adj.begin(); it!=adj.end(); it++)
		if(visited[it->first] == false)
			fillOrder(it->first, visited, Stack);

	Graph gr = getTranspose();

	for(map<int,list<int> >::iterator it= adj.begin(); it!=adj.end(); it++)
		visited[it->first] = false;

	while (Stack.empty() == false)
	{
		int v = Stack.top();
		Stack.pop();

		if (visited[v] == false)
		{
			gr.DFS(v, visited,file);
			file <<endl<< "-"<<endl;
		}
	}
}

int main()
{
	Graph g(15,0.17);

	ofstream file("SCC.txt");
	g.printSCCs(file);

	return 0;
}
