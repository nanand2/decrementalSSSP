#include "graph.h"
#include <stddef.h>
#include <vector>
#include <map>
#include <queue>
#include <condition_variable> 
#include <thread>
#include <iostream>
#include <set>
#include <unordered_set>


/* Defines the Fast ESTree class-- an Even-shiloach tree on an input graph for decremental SSSP with O(mn) updates
 * Computes BFS tree (also represented as a graph) from input graph + source node
 * Computes the three requisite "N" sets for each node in the graph
 */



class FastESTree {
public:
    
    

    FastESTree(Graph* g, int sourceNode);
    ~FastESTree();
    void runBFS(int s);
    void deleteHandler(int i, int j);
    void deleteEdge(int i, int j);
    void deleteRandomEdge();
    int  query(int i);
    int queryRandomNode();

    Graph* bfsTree; //bfsTree nodes array
    Graph* g;
    bool verbose = false;
    
private:
    
    int s;
    int n;
    int m;
    void updateLevels();

    std::unordered_set<int> ** N1; //pointers to array of neighbor vectors for each node in graph
    std::unordered_set<int> ** N2; 
    std::unordered_set<int> ** N3; 
    std::queue<int> Q; //global priority queue for nodes whose levels might need to be updated
    int* levelLabel; //maps node i to level L_j in bfsTree
    std:: map<int, int> distance;
    
    
};