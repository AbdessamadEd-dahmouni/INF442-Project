#include "Graph.hpp"
#include <mpi.h>
using namespace std;

void SCCpar(Graph & g,Graph & gt,set<int> & vertices,int myid,int & n,int & ntot,int & nprocs, int p[]){
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
		cout<<*it<<" ";
	cout<<endl<<"-"<<endl;
	
	n=n+s1.size();
	p[myid]=1;
	int size=s2.size();
	MPI_Send(&size,1,MPI_INT,3*myid+1,0,MPI_COMM_WORLD);
	if(size!=0){
		MPI_Send(&n,1,MPI_INT,3*myid+1,0,MPI_COMM_WORLD);
		MPI_Send(&p,nprocs,MPI_INT,3*myid+1,0,MPI_COMM_WORLD);
		int* mes2=new int[size];
		copy(s2.begin(),s2.end(),mes2);
		MPI_Send(&mes2,size,MPI_INT,3*myid+1,0,MPI_COMM_WORLD);
		delete[] mes2;
	}
	
	size=s3.size();
	MPI_Send(&size,1,MPI_INT,3*myid+2,0,MPI_COMM_WORLD);
	if(size!=0){
		MPI_Send(&n,1,MPI_INT,3*myid+2,0,MPI_COMM_WORLD);
		MPI_Send(&p,nprocs,MPI_INT,3*myid+2,0,MPI_COMM_WORLD);
		int* mes3=new int[size];
		copy(s3.begin(),s3.end(),mes3);
		MPI_Send(&mes3,size,MPI_INT,3*myid+2,0,MPI_COMM_WORLD);
		delete[] mes3;
	}
	
	size=s4.size();
	MPI_Send(&size,1,MPI_INT,3*myid+3,0,MPI_COMM_WORLD);
	if(size!=0){
		MPI_Send(&n,1,MPI_INT,3*myid+3,0,MPI_COMM_WORLD);
		MPI_Send(&p,nprocs,MPI_INT,3*myid+3,0,MPI_COMM_WORLD);
		int* mes4=new int[size];
		copy(s4.begin(),s4.end(),mes4);
		MPI_Send(&mes4,size,MPI_INT,3*myid+3,0,MPI_COMM_WORLD);
		delete[] mes4;
	}
	size=0;
	if(n==ntot){
		for(int i=0;i<nprocs;i++){
			if(p[i]==0){
				MPI_Send(&size,1,MPI_INT,i,0,MPI_COMM_WORLD);
			}
		}
	}
}

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	int myid, nprocs;
	MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    int *p=new int[nprocs];
	fill(p,p+nprocs,0);
	Graph g("test.txt");
	int ntot=g.vertices().size();
	Graph gt=g.getTranspose();
	int n=0;
	if(myid==0){
		//ofstream file("SCC.txt");
		set<int> vertices;
		vertices=g.vertices();
		SCCpar(g,gt,vertices,myid,n,ntot,nprocs,p);
	}else if(myid==1){
		//ofstream file("SCC.txt",ios::app);
		int size;
		MPI_Recv(&size,1,MPI_INT,(myid-1)/3,0,MPI_COMM_WORLD,&status);
		cout<<"sent1";
		if(size!=0){
			MPI_Recv(&n,1,MPI_INT,(myid-1)/3,0,MPI_COMM_WORLD,&status);
			MPI_Recv(&p,nprocs,MPI_INT,(myid-1)/3,0,MPI_COMM_WORLD,&status);
			int * mes=new int[size];
			MPI_Recv(&mes,size,MPI_INT,(myid-1)/3,0,MPI_COMM_WORLD,&status);
			set<int> vertices(mes, mes+size);
			SCCpar(g,gt,vertices,myid,n,ntot,nprocs,p);
			delete[] mes;
		}
	}
	delete[] p;
	MPI_Finalize();
	return 0;
}
