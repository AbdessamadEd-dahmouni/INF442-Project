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


void SCCseq(Graph & g,Graph & gt,set<int> & vertices, ofstream& file){
	if(vertices.empty()) return;
	set<int> succ;
	set<int> pred;
	map<int,bool> visited;
	int index=rand()%vertices.size();
	int v;
	for(set<int>::iterator it=vertices.begin(); it!=vertices.end();it++){
		if(index==0){ v=*it; break;}
		index--;
	}
	for(set<int>::iterator it=vertices.begin(); it!=vertices.end(); it++)
		visited[*it] = false;

	g.DFS2(v, visited,vertices,succ);
	
	for(set<int>::iterator it=vertices.begin(); it!=vertices.end(); it++)
		visited[*it] = false;
		
	gt.DFS2(v,visited,vertices,pred);
	set<int> s1;
	set<int> s2;
	set<int> s3;
	set<int> s4;
	set<int> st;

	set_intersection(succ.begin(),succ.end(),pred.begin(),pred.end(),inserter(s1,s1.end()));  //	S1=Succ(v) inter Pred(v)
	set_difference(succ.begin(),succ.end(),pred.begin(),pred.end(),inserter(s2,s2.end()));	//	S2=Succ(v)\Pred(v)
	set_difference(pred.begin(),pred.end(),succ.begin(),succ.end(),inserter(s3,s3.end())); //	S3=Pred(v)\Succ(v)
	set_union(succ.begin(),succ.end(),pred.begin(),pred.end(),inserter(st,st.end()));	//	S4=vertices\(Succ(v) union Pred(v))
	set_difference(vertices.begin(),vertices.end(),st.begin(),st.end(),inserter(s4,s4.end()));

	for(set<int>::iterator it=s1.begin();it!=s1.end();it++)
		file<<*it<<" ";
	file<<endl<<"-"<<endl;
	SCCseq(g,gt,s2,file);
	SCCseq(g,gt,s3,file);
	SCCseq(g,gt,s4,file);
}

int main(int argc, char *argv[])
{
	ofstream file("SCC.txt");
	Graph g("test.txt");
	set<int> vertices=g.vertices();
	Graph gt=g.getTranspose();
	cout<<g;
	SCCseq(g,gt,vertices,file);
	return 0;
}
