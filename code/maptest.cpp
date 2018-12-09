// C++ Implementation of Kosaraju's algorithm to print all SCCs
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <map>
#include <mpi.h>
using namespace std;

class Graph
{
	public :
	map<int,list<int> > adj; // An array of adjacency lists

	// Fills Stack with vertices (in increasing order of finishing
	// times). The top element of stack has the maximum finishing 
	// time
	void fillOrder(int v, map<int,bool> & visited, stack<int> &Stack);

	// A recursive function to print DFS starting from v
	void DFSUtil(int v, map<int,bool> & visited,ofstream& file);
	Graph(string s);
	Graph(){
	}
	Graph(int V,double p);
	void addEdge(int v, int w);

	// The main function that finds and prints strongly connected
	// components
	void printSCCs(ofstream& file);

	// Function that returns reverse (or transpose) of this graph
	Graph getTranspose();
};

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
Graph::Graph(string s)
{
	ifstream data(s.c_str());
	string line;
	getline(data,line);
	getline(data,line);
	getline(data,line);
	/*stringstream iss(line);
        iss >> line;
	iss>>line;
	iss>>line;
	V =atoi(line.c_str());
	cout<<V;*/
	getline(data,line);
	int a,b;
	while(data>>a>>b){
			if(adj.find(a)==adj.end()) adj[a]=list<int>();
        	adj[a].push_back(b);
	}
}
// A recursive function to print DFS starting from v
void Graph::DFSUtil(int v,map<int,bool> & visited,ofstream& file)
{	
	// Mark the current node as visited and print it
	visited[v] = true;
	file << v << " ";

	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited,file);
}

Graph Graph::getTranspose()
{
	Graph g;
	for (map<int,list<int> >::iterator it=adj.begin(); it!=adj.end(); it++)
	{
		// Recur for all the vertices adjacent to this vertex
		list<int>::iterator i;
		for(i = (it->second).begin(); i != (it->second).end(); ++i)
		{
			g.adj[*i].push_back(it->first);
		}
	}
	return g;
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w); // Add w to v’s list.
}

void Graph::fillOrder(int v, map<int,bool> & visited, stack<int> &Stack)
{
	// Mark the current node as visited and print it
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for(i = adj[v].begin(); i != adj[v].end(); ++i)
		if(!visited[*i])
			fillOrder(*i, visited, Stack);

	// All vertices reachable from v are processed by now, push v 
	Stack.push(v);
}

// The main function that finds and prints all strongly connected 
// components
void Graph::printSCCs(ofstream& file)
{
	stack<int> Stack;

	// Mark all the vertices as not visited (For first DFS)
	map<int,bool> visited;
	//bool *visited = new bool[adj.size()];
	for(map<int,list<int> >::iterator it= adj.begin(); it!=adj.end(); it++)
		visited[it->first] = false;

	// Fill vertices in stack according to their finishing times
	for(map<int,list<int> >::iterator it= adj.begin(); it!=adj.end(); it++)
		if(visited[it->first] == false)
			fillOrder(it->first, visited, Stack);

	// Create a reversed graph
	Graph gr = getTranspose();

	// Mark all the vertices as not visited (For second DFS)
	for(map<int,list<int> >::iterator it= adj.begin(); it!=adj.end(); it++)
		visited[it->first] = false;

	// Now process all vertices in order defined by Stack
	while (Stack.empty() == false)
	{
		// Pop a vertex from stack
		int v = Stack.top();
		Stack.pop();

		// Print Strongly connected component of the popped vertex
		if (visited[v] == false)
		{
			gr.DFSUtil(v, visited,file);
			file <<endl<< "-"<<endl;
		}
	}
}

ostream& operator<<(ostream& os, const Graph& g){
	os<<endl;
	for (map<int,list<int> >::const_iterator it=g.adj.begin(); it!=g.adj.end(); it++)
	{	os<<(it->first)<<":";
		for(list<int>::const_iterator i = (it->second).begin(); i != (it->second).end(); ++i)
		{
			os<<" "<<*i;
		}
		os<<endl<<"-"<<endl;
	}
	return os;
}
// Driver program to test above functions
int main(int argc, char *argv[])
{
	// Create a graph given in the above diagram
	//Graph g("WikiTalk.txt");
	int myrank, nprocs;
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	Graph g;
	int n=20;
	double p=0.17;
	int k=n/nprocs;
	int r=n%nprocs;
	double x;
	srand(myrank);
	if(k<r){
	for(int i=myrank*(k+1);i<(myrank+1)*(k+1);i++)
			g.adj[i]=list<int>();
	for(int i=myrank*(k+1);i<(myrank+1)*(k+1);i++)
		for(int j=0;j<n;j++){
			x=rand()/(double) RAND_MAX;
			if(x<p) g.adj[i].push_back(j);
		}
	}
	else {
	for(int i=r+myrank*k;i<r+(myrank+1)*k;i++)
			g.adj[i]=list<int>();
	for(int i=r+myrank*k;i<r+(myrank+1)*k;i++)
		for(int j=0;j<n;j++){
			x=rand()/(double) RAND_MAX;
			if(x<p) g.adj[i].push_back(j);
		}		
	}
	cout << "The following is part "<< myrank << " of the graph : "<< g;
	//cout << "Following are strongly connected components in given graph rank : "<<myrank<<"\n";
	//if(k<r) cout<<g.adj[myrank*(k+1)]<<endl;
	//else cout<<g.adj[r+myrank*k]<<endl;
	//ofstream file("SCC.txt");
	//g.printSCCs(file);
	MPI_Finalize();
	return 0;
}
