#ifndef GRAPH_CPP
#define GRAPH_CPP

#include <stddef.h>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>


/* Defines the Graph class for an unweighted graph of n nodes and m edges.
 * Graph is implemented as an adjacency list, where neighbors of node n are stored
 * in a linked list 
*/


/* node in adjacency list*/
 struct node{
 	int val; //node number
 	node * next; //next node in LL
 };

class Graph {
public:


	/**
	 * Given a number of edges m and a number of nodes n, constructs a random 
	 * undirected, unweighted graph 
	 **/
	 Graph(int m, int n);

	
	~Graph();
    void insertEdge(int i, int j);
    bool hasEdge(int i, int j);
    void deleteEdge(int i, int j);
    void printGraph();
    int getNumNodes();
    int getNumEdges();
    bool DFS(int i, int j, bool visited[]);
    bool isConnected(int i, int j);
    void deleteAllEdges();
    bool isEmpty();
    void deleteComponent(int i);
    void renameComponent(int i);
    std::map<int, int> component; //map node to component number
    std::pair<int, int> deleteRandomEdge();
    std::set<int> returnAdjacentNodes(int i);
    std::queue<std::pair<int, int>> edges;
    int componentNum = 0; //tells you the next available component number
    int getRandomNode();



private:
	struct node** array; //array of nodes (define graph)
	void setEdges(int m);
    int n;
    int m;
    int numEdges;
    std::vector<int> nodes; //array of nodes in the graph (to get random)

	


};


#endif