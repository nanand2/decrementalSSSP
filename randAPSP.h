#include "fast_estree.h"
#include <stddef.h>
#include <vector>
#include <map>
#include <queue>
#include <condition_variable> 
#include <thread>
#include <iostream>
#include <set>




/* Defines the randAPSP class-- which uses ESTree + BFS + hitting set lemma to 
 * get O(mn^(1.5)logn) total update and O(sqrt(n)) query time.
 * c is a paramter which adjusts the probability of algorithm correctness ~ (1/n^c)
 */

class randAPSP {
public:
    
    

    randAPSP(Graph* g, int c);
    ~randAPSP();
    void deleteHandler(int i, int j);
    void deleteEdge(int i, int j);
    void deleteRandomEdge();
    int  query(int i, int j);
    int queryRandomNodes();
    std::pair<std::pair<int,int>,int> queryRandomNodesTest(); //tester function



    Graph* bfsTree; //bfsTree nodes array
    Graph* g;
    bool verbose = false;
    
private:
    void buildHittingSet();
    void runBFS(int s, int d);
    int c; //hitting set parameter
    int n; //number of nodes
    int m; //number of edges
    int d; //depth of BFS tree
    int hs; //hitting set size
    void updateLevels();
    std::vector<int> hittingSet;
    FastESTree** esTrees; 
    Graph** bfsTrees;
    int** distances;
    std::set<std::pair<int, int>> ** heaps; //pointers to array of priority queues for each node in graph
    std::set<std::pair<int, int>> Q; //global priority queue for nodes whose levels might need to be updated
    int* levelLabel; //maps node i to level L_j in bfsTree
    std:: map<int, int> distance;
    
    
};