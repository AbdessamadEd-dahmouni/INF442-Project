#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
class Graph
{
	public :
	map<int,list<int> > adj; 
	
	void fillOrder(int v, map<int,bool> & visited, stack<int> &Stack);

	void DFS(int v, map<int,bool> & visited,ofstream& file);
	void DFS2(int v,map<int,bool> & visited,set<int> const & vertices,set<int> & succ);
	Graph(string s);
	Graph(){
	}
	Graph(int V,double p);
	void addEdge(int v, int w);

	void printSCCs(ofstream& file);

	Graph getTranspose();
	set<int> vertices();
};

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
			if(adj.find(b)==adj.end()) adj[b]=list<int>();
        	adj[a].push_back(b);
	}
}

Graph Graph::getTranspose()
{
	Graph g;
	for (map<int,list<int> >::iterator it=adj.begin(); it!=adj.end(); it++)
	{
		if (g.adj.find(it->first)==g.adj.end()) g.adj[it->first]=list<int>();
		for(list<int>::iterator i = (it->second).begin(); i != (it->second).end(); ++i)
		{
			g.adj[*i].push_back(it->first);
		}
	}
	return g;
}

void Graph::addEdge(int v, int w)
{
	adj[v].push_back(w);
}

set<int> Graph::vertices(){
	set<int> v;
	for(map<int,list<int> >::iterator it=adj.begin();it!=adj.end();it++)
		v.insert(it->first);
	return v;
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

void Graph::DFS(int v,map<int,bool> & visited,ofstream& file)
{	
	visited[v] = true;
	file << v << " ";

	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFS(*i, visited,file);
}

void Graph::fillOrder(int v, map<int,bool> & visited, stack<int> &Stack)
{
	visited[v] = true;

	list<int>::iterator i;
	for(i = adj[v].begin(); i != adj[v].end(); ++i)
		if(!visited[*i])
			fillOrder(*i, visited, Stack);

	Stack.push(v);
}

void Graph::DFS2( int v,map<int,bool> & visited,set<int> const & vertices,set<int> & succ)
{	
    visited[v] = true;
	succ.insert(v);
	for (list<int>::iterator i = adj[v].begin(); i != adj[v].end(); ++i)
		if (vertices.find(*i)!=vertices.end() && !visited[*i]){
			DFS2(*i, visited, vertices, succ);
	}
}
