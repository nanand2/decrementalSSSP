#include "graph.h"
#include <stddef.h>
#include <vector>
#include <map>
#include <queue>
#include <condition_variable> 
#include <thread>
#include <iostream>
#include <set>




/* Defines the ESTree class-- an Even-Shiloach tree on an input graph for decremental SSSP
 * Computes BFS tree (also represented as a graph) from input graph + source node
 * This implementation is built over STL sets has an added O(log(n)) factor from priority queue
 * operations. Algorithm can be extended easily to weighted case. See Fast ES tree for O(mn) updates
 */



class ESTree {
public:
    
    
 
    ESTree(Graph* g, int sourceNode);
    
    /**
     * Cleans up all memory allocated by the graph.
     */
    ~ESTree();
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
    int d;
    void updateLevels();
    std::set<std::pair<int, int>> ** heaps; //pointers to array of priority queues for each node in graph
    std::set<std::pair<int, int>> Q; //global priority queue for nodes whose levels might need to be updated
    int* levelLabel; //maps node i to level L_j in bfsTree
    std:: map<int, int> distance;
    
    
};