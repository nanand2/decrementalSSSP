#include "estree.h"
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
using namespace std;

ESTree::ESTree(Graph* g, int sourceNode):  g(g),s(sourceNode), n(g->getNumNodes()), m(g->getNumEdges()){

    //init
    heaps = new set<pair<int, int>>* [n];
    for (int i=0; i<n; i++){ //init neighbor heaps
        heaps[i] = new set<pair<int, int>>;
    }
    bfsTree = new Graph(0, n); //init empty shortest paths tree
    levelLabel = new int[n]; //init levelLabel array of ints

    //preprocessing
    runBFS(s); //build initial shortest paths tree and get levels of each node rel to source
    //init neighbor heaps
    for (int i= 0; i< n; i++){
        levelLabel[i] = distance[i];
        set<int> adj = g->returnAdjacentNodes(i); //find all adjacent nodes in graph
        for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){  //for every neighboring edge
            //insert neighbor *it into heap N(i) with key l(*it)
            heaps[i]->insert(pair<int, int>(levelLabel[*it], *it));
        }
    }
}

 ESTree::~ESTree(){
    delete bfsTree;
    for (int i=0; i<n; i++){ //del neighbor heaps
        delete heaps[i];
    }
    delete [] heaps;
    delete levelLabel;
 }


void ESTree::runBFS(int s){
    //run BFS and fill out bfsTree
    distance.clear(); 
    map<int, int> parent; //(child: parent)
    for (int i=0; i<n; i++){
        distance.insert(pair<int, int>(i, -1)); //-1 sentinel for infinity
        parent.insert(pair<int, int>(i, -1)); //-1 sentinel for no parent
    }
    //add root to levels map
    levelLabel[s] = 0;
    queue<int> q;
    distance[s] = 0;
    q.push(s);
    while(!q.empty()){
        //get current node
        int curr = q.front();
        int level = levelLabel[curr];
        q.pop();
        set<int> adj = g->returnAdjacentNodes(curr); //find all adjacent nodes in graph

        //iterative step of BFS, update distances
        for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){
            if (distance[*it] == -1){
                distance[*it] = distance[curr] + 1;
                parent[*it] = curr;
                q.push(*it);

                // update levels/ bfsTree
                bfsTree->insertEdge(curr, *it);
                bfsTree->insertEdge(*it, curr);
                levelLabel[*it] = level+1;
            }
        }
    }
}

void ESTree::deleteHandler(int i, int j){

    //insert i and j into heap Q with keys l(i), l(j) respectively
    if (levelLabel[i]<m+1){//check that we are in root's component
        if (i!=s){Q.insert(pair<int, int>(levelLabel[i], i));}
        if (j!=s){Q.insert(pair<int, int>(levelLabel[j], j));}

        //remove key i from N(j) and key j from N(i)
        heaps[i]->erase(pair<int, int>(levelLabel[j], j));
        heaps[j]->erase(pair<int, int>(levelLabel[i], i));

        updateLevels(); //call updateLevels
    }
}

void ESTree::updateLevels(){
    while (!Q.empty()){
        //get node y with min key l(y) in Q 
        int y = Q.begin()->second;
        Q.erase( Q.begin()); //pop front node
        if (verbose){cout<< "dropping node " << y<< "level: "<< levelLabel[y] << endl;}
        //drop y down a level (find a neighbor, add neighbor level + 1 to level of y)
        int l_pr;
        if (heaps[y]->empty()){
            l_pr = m + 1; //now node is part of separate component
        }
        else{
            int z = heaps[y]->begin()->second;
            l_pr = levelLabel[z] + 1;
        }
        if (l_pr > levelLabel[y]){
            int old_l = levelLabel[y];
            levelLabel[y] = l_pr;
            if (l_pr >= m + 1){ //our sentinel for infinity is simply an impossible distance in the graph
                levelLabel[y] =  m + 1;
            }
            //for each neighbor x of y, update key of y in x's neighbor heap 
            set<pair<int,int>> adj = *heaps[y]; //find all adjacent nodes in graph
            for(set<pair<int,int>>::iterator it = adj.begin(); it != adj.end(); it ++){
                    heaps[it->second]->erase(pair<int, int>(old_l, y));
                    heaps[it->second]->insert(pair<int, int>(levelLabel[y], y));
                    //insert x into Q if Q doesn't contain x
                    if (Q.find(*it) == Q.end() && it->first<m+1){
                        Q.insert(*it);
                    }
            }
        }

    }
}



int ESTree::query(int i){
    //O(1) query

    if (levelLabel[i] >= m+1){
        if (verbose){cout << "querying node from " << s<< " to "<< i << " estimate: " << -1 << endl;}
        return -1;
    }
    if (verbose) {cout << "querying node from " << s<< " to "<< i <<  " estimate: " << levelLabel[i] << endl;}
    return levelLabel[i];
}


int ESTree::queryRandomNode(){
    int i = g->getRandomNode();
    if (verbose) {cout << "querying node "<< i << " estimate: " << query(i) << endl;}
    return query(i);
}


void ESTree::deleteEdge(int i, int j){
    if (g->hasEdge(i,j)){
        g->deleteEdge(i, j); //delete edge in underlying graph
        g->deleteEdge(j, i); //delete edge in underlying graph
        deleteHandler(i,j);
    }
}


void ESTree::deleteRandomEdge(){
    pair<int, int> ns;
    ns = g->deleteRandomEdge(); //delete edge in underlying graph
    deleteHandler(ns.first, ns.second); //
}







