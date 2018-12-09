#include "Graph.hpp"
#include <mpi.h>
using namespace std;


int main(int argc, char *argv[])
{
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
	MPI_Finalize();
	return 0;
}
