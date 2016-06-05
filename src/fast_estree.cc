#include "fast_estree.h"
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

FastESTree::FastESTree(Graph* g, int sourceNode):  g(g),s(sourceNode), n(g->getNumNodes()), m(g->getNumEdges()){

    //init
    N1 = new unordered_set<int>* [n];
    N2 = new unordered_set<int>* [n];
    N3 = new unordered_set<int>* [n];

    bfsTree = new Graph(0, n); //init empty shortest paths tree
    levelLabel = new int[n]; //init levelLabel array of ints
    
    //preprocessing
    runBFS(s); //build initial shortest paths tree and get levels of each node rel to source

    //build neighbor sets
    for (int i= 0; i< n; i++){
        N1[i] = new unordered_set<int>();
        N2[i] = new unordered_set<int>();
        N3[i] = new unordered_set<int>();
        levelLabel[i] = distance[i];
        set<int> adj = g->returnAdjacentNodes(i); //find all adjacent nodes in graph
        for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){  //for every neighboring edge
            //insert neighbor *it into heap N(i) with key l(*it)
            if (levelLabel[*it] == levelLabel[i]){
                N2[i]->insert(*it);
            }
            else if (levelLabel[*it] < levelLabel[i]){
                N1[i]->insert(*it);
            }
            else{
                N3[i]->insert(*it);
            }
        }
    }
}

 FastESTree::~FastESTree(){
    delete bfsTree;
    for (int i=0; i<n; i++){ //del neighbor heaps
        delete N1[i];
        delete N2[i];
        delete N3[i];
    }
    delete [] N1;
    delete [] N2;
    delete [] N3;
    delete levelLabel;
 }


void FastESTree::runBFS(int s){
    //run BFS and fill out bfsTree
    distance.clear(); 
    for (int i=0; i<n; i++){
        distance.insert(pair<int, int>(i, -1)); //-1 sentinel for infinity
    }
    //add root to levels map
    levelLabel[s] = 0;
    queue<int> q;
    distance[s] = 0;
    q.push(s);
    while(!q.empty()){
        //get current node
        int curr = q.front();
        q.pop();
        set<int> adj = g->returnAdjacentNodes(curr); //find all adjacent nodes in graph

        //iterative step of BFS, update distances
        for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){
            if (distance[*it] == -1){
                distance[*it] = distance[curr] + 1;
                q.push(*it);

                // update levels/ bfsTree
                bfsTree->insertEdge(curr, *it);
                bfsTree->insertEdge(*it, curr);
                levelLabel[*it] = levelLabel[curr]+1;
            }
        }
    }
}

void FastESTree::deleteHandler(int i, int j){

    if (levelLabel[i] == levelLabel[j]){
         //remove key i from N2(j) and key j from N2(i)
        N2[i]->erase(j);
        N2[j]->erase(i);
        return; //done here, no update necessary
    }
    else if (levelLabel[i]< levelLabel[j]){
         //remove key i from N1(j) and key j from N3(i)
        N3[i]->erase(j);
        N1[j]->erase(i);
        if (N1[j]->empty()==false){return;}
        else{
            Q.push(j);
            updateLevels(); //call updateLevels}
            return;
        }
    }
    else{
         //remove key i from N3(j) and key j from N1(i)
        N1[i]->erase(j);
        N3[j]->erase(i);
        if (N1[i]->empty()==false){return;}
        // if (i!=s){
        else{
            Q.push(i);
            updateLevels();
            return;
        }
    }
}

void FastESTree::updateLevels(){
    while (Q.empty()==false){
        //get any node y in the queue
        int y = Q.front();
        Q.pop();
        //y drops one level
        if (verbose) {cout<<"droppping node "<< y<< "to level"<< levelLabel[y]+ 1 <<endl;}
        levelLabel[y] += 1;
        //for each neighbor in N2, remove j from the neighbor's N2 and place in neighbor's N3
        for (unordered_set<int>::iterator it = N2[y]->begin(); it != N2[y]->end(); ++it) {
    
            N3[*it]->insert(y);
            N2[*it]->erase(y);
        }
        N1[y]->swap(*N2[y]); //update N set, N2 emptied
        N2[y]->clear();
        //for each neighbor in N3, remove j from the neighbor's N1 and place in neighbor's N2
        for (unordered_set<int>::iterator it = N3[y]->begin(); it != N3[y]->end(); ++it) {
    
            N2[*it]->insert(y);
            N1[*it]->erase(y);
            if (N1[*it]->empty() && levelLabel[*it]<m+1){ //determine if you need to drop this node
                if (verbose){cout<<"pushing node "<< *it<<" back onto the queue"<<endl;}
                    Q.push(*it);
            }
        }
        N2[y]->swap(*N3[y]);//update N2, N3 emptied
        N3[y]->clear(); //clear N3 set, redundant
        if (N1[y]->empty() && levelLabel[y]<m+1){
            if (verbose) {cout<<"pushing node "<< y<<" back onto the queue"<<endl;}
            Q.push(y);
        }
    }
}

int FastESTree::query(int i){
    //O(1) query
    if (levelLabel[i]> m){
        return -1;
    }
    return levelLabel[i];
}


int FastESTree::queryRandomNode(){
    //srand seeded in constructor for Graph-- messy
    int i = g->getRandomNode();
    if (verbose) {cout << "querying node "<< i << " estimate: " << query(i) << endl;}
    return query(i);
}

void FastESTree::deleteEdge(int i, int j){
    if (g->hasEdge(i,j)){
        g->deleteEdge(i, j); //delete edge in underlying graph
        g->deleteEdge(j, i); //delete edge in underlying graph
        deleteHandler(i,j);
    }
}


void FastESTree::deleteRandomEdge(){
    pair<int, int> ns;
    ns = g->deleteRandomEdge(); //delete edge in underlying graph
    deleteHandler(ns.first, ns.second); //
}







