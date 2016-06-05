#include "randAPSP.h"
#include <sys/types.h>
#include <signal.h>
#include <mutex>
#include <ctime>
#include <cstdlib>
#include <algorithm>    // std::sort
#include <string>
 #include <unistd.h>
 #include <sys/wait.h>
#include <semaphore.h>
#include <iterator>
#include <math.h>       /* log */
using namespace std;

randAPSP::randAPSP(Graph* g, int c):  g(g), c(c), n(g->getNumNodes()), m(g->getNumEdges()) {

    ///init

    //construct randomized hitting set S 
    buildHittingSet();
    
    //build ES Trees and init distance arrays for BFS
    esTrees = new FastESTree*[hs];
    for (int i=0; i<hs; i++){
        esTrees[i] = new FastESTree(g, hittingSet[i]);
        
    }

    distances = new int*[n];
    //build bfsTrees
    bfsTrees = new Graph*[n];
    for (int i=0; i<n; i++){
        distances[i] = new int[n];
        bfsTrees[i] = new Graph(0, n); //init graph to be empty
        runBFS(i, d); //run BFS from node i up to depth d. 
    }

}

 randAPSP::~randAPSP(){
     for (int i=0; i<n; i++){
        delete distances[i];
        delete bfsTrees[i];
    }
    delete [] bfsTrees;
    delete [] distances;

    for (int i=0; i<hs; i++){ //del neighbor heaps
        delete esTrees[i];
    }
    delete [] esTrees;
  
 }

void randAPSP::buildHittingSet(){
    vector<int> nodes;
    for (int i=0; i<n; i++){
        nodes.push_back(i);
    }
    random_shuffle(nodes.begin(), nodes.end());
    hs = min (n, c*(int) sqrt((double) n *log(n)));
    for (int i=0; i<hs; i++){ //select first s nodes in shuffled vector
        hittingSet.push_back(nodes[i]);
    }
}




void randAPSP::runBFS(int s, int d){
    //run BFS and fill out bfsTree
    for (int i=0; i<n; i++){
        distances[s][i] = -1; //-1 sentinel for infinity
    }
    int* levelLabel = new int[n];
    //add root to levels map
    levelLabel[s] = 0;
    queue<int> q;
    distances[s][s] = 0;
    q.push(s);
    while(!q.empty()){
        //get current node
        int curr = q.front();
        int level = levelLabel[curr];
        q.pop();
        set<int> adj = g->returnAdjacentNodes(curr); //find all adjacent nodes in graph

        //iterative step of BFS, update distances
        for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){
            if (distances[s][*it] == -1){
                distances[s][*it] = distances[s][curr] + 1;
                q.push(*it);

                // update levels/ bfsTree
                bfsTrees[s]->insertEdge(curr, *it);
                bfsTrees[s]->insertEdge(*it, curr);
                levelLabel[*it] = level+1;
                if (level+1 > d){
                    delete levelLabel;
                    return;
                }
            }
        }
    }
    delete levelLabel;
    return;
   
}


int randAPSP::query(int i , int j){
    //O(sqrt(n) logn) query
    set<int> distanceEstimates; // = new int[hs + 1];
    //query each ES tree for d(s,u) + d(s,v) = D(u,v)
    int di;
    int dj;
    for (int k=0; k<hs; k++){
        di = esTrees[k]->query(i);
        dj = esTrees[k]->query(j);
        if ((di != -1) && (di <=g->getNumEdges()) && (dj != -1) && (dj<=g->getNumEdges())){
            distanceEstimates.insert(di+dj); //insert distance estimate after double checking nodes are in graph
        }
    }
    //query BFS tree for i for d(i,j)
    if (distances[i][j] != -1){
        distanceEstimates.insert(distances[i][j]); //insert BFS estimate distance into
    }

    if (distanceEstimates.empty()){ //by chance if no estimates are great
        return -1;
    }
    else{
        return *distanceEstimates.begin(); //return min over all estimates from ES trees and bfs Trees
    }
}


void randAPSP::deleteHandler(int i, int j){

    //update all ES-trees
    for (int i=0; i<hs; i++){
        esTrees[i]->deleteHandler(i,j);
    }
    //reconstruct all bfs Trees
    for (int i=0; i<n; i++){
        delete bfsTrees[i]; 
        bfsTrees[i] = new Graph(0, n); //init graph to be empty
        runBFS(i, d); //run BFS from node i up to depth d. 
    }
}
int randAPSP::queryRandomNodes(){
    //srand seeded in constructor for Graph-- messy
    int i = g->getRandomNode();
    int j = g->getRandomNode();
    // cout << "querying nodes "<< i << " and "<< j<< " estimate: " << query(i,j) << endl;
    return query(i,j);
}

pair<pair<int,int>,int> randAPSP::queryRandomNodesTest(){
    //special return function allows us to test how often algorithm is correct
    int i = g->getRandomNode();
    int j = g->getRandomNode();
    // cout << "querying nodes "<< i << " and "<< j<< " estimate: " << query(i,j) << endl;
    return pair<pair<int,int>, int>(make_pair(make_pair(i,j), query(i,j)));
}

void randAPSP::deleteEdge(int i, int j){
    if (g->hasEdge(i,j)){
        g->deleteEdge(i, j); //delete edge in underlying graph
        g->deleteEdge(j, i); //delete edge in underlying graph

        deleteHandler(i,j);
    }
}


void randAPSP::deleteRandomEdge(){
    pair<int, int> ns;
    ns = g->deleteRandomEdge(); //delete edge in underlying graph
    deleteHandler(ns.first, ns.second); //
}







