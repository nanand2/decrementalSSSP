#include "naive.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>   
#include <iostream>
#include <string>

using namespace std;

Naive::Naive(Graph* g, int sourceNode):  g(g),s(sourceNode), n(g->getNumNodes()) {

    //init
    bfsTree = new Graph(0, n); //init empty bfsTree
    //preprocessing
    runBFS(); //build bfsTree 
}

Naive::~Naive(){
    delete bfsTree;
}

void Naive::runBFS(){
    //run BFS and fill out bfsTree
    //init distance from source array
    distance.clear();
    for (int i=0; i<n; i++){
        distance.insert(pair<int, int>(i, -1)); //-1 sentinel for infinity
    }
    distance[s] = 0;

    map<int, int> parent; //(child: parent)

    //add root to levels map
    queue<int> q;
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
                parent[*it] = curr;
                q.push(*it);

                // update levels/ bfsTree
                bfsTree->insertEdge(curr, *it);
                bfsTree->insertEdge(*it, curr);
            }
        }
    }
}



int Naive::query(int i){
    cout<< "found distance = "<< distance[i]<<endl;
    return distance[i];
}


int Naive::queryRandomNode(){
    //srand seeded in constructor for Graph-- messy
    int i = g->getRandomNode();
    cout << "querying node "<< i << endl;
    return query(i);
}


void Naive::deleteEdge(int i, int j){
    if (g->hasEdge(i,j)){
        g->deleteEdge(i, j); //delete edge in underlying graph
        g->deleteEdge(j, i); //delete edge in underlying graph
        delete bfsTree; //delete bfsTree
        bfsTree = new Graph(0, n); //init empty bfsTree
        runBFS(); //rebuild bfsTree
    }
}


void Naive::deleteRandomEdge(){
    pair<int, int> ns;
    ns = g->deleteRandomEdge(); //delete edge in underlying graph
    delete bfsTree; //delete bfsTree
    bfsTree = new Graph(0, n); //init empty bfsTree
    runBFS(); //rebuild bfsTree
}







