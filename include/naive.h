#ifndef NAIVE_CPP
#define NAIVE_CPP

#include <stddef.h>
#include <vector>
#include <map>
#include <set>
#include "graph.h"


/* Defines the Naive class over Graph G with source node s, which naively solves decremental SSSP 
 * for unweighted graphs in O(m^2) time by running the static SSSP algorithm after every deletion. 
 * underlying static algorithm is DBFS and runs in O(m+ n) time. 
 */


class Naive {
public:
    

    Naive(Graph* g, int sourceNode);
    
    ~Naive();
    void runBFS();
    void deleteEdge(int i, int j);
    void deleteRandomEdge();
    int  query(int i);
    int queryRandomNode();


    Graph* bfsTree; //bfsTree nodes array
    Graph* g; //pointer to underlying graph
    std::map<int, int> distance;  //node-node distance map

    
private:
    
    int s;
    int n;
    int m;
    int d;



    
    
    
};

#endif 