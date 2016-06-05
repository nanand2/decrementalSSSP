#include "graph.h"
#include <cstdlib>
#include <algorithm>    // std::random_shuffle
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

Graph::Graph(int m, int n): n(n), m(m) {
    if ((m!=0) && (unsigned int ) m < (unsigned int ) n-1){
        cout << "too few edges specified" << endl;
        exit(1);
    }

    if ((unsigned int ) m > (unsigned int ) (n*(n-1))/2){
        cout << "too many edges specified" << endl;
        exit(1);
    }

    array = new struct node*[n]; //new array of node pointers
	for (int i=0; i< n; i++){
		array[i] = new struct node;
		array[i]->val = i;
		array[i]->next = nullptr;
	}
    if (m != 0){
        // cout << "building graph" << endl;
        setEdges(m);
         // cout << "done building graph" << endl;
    }

    //init array of nodes (shuffled)
    for (int i=0; i<m; i++){
        nodes.push_back(i % n);
    }
    random_shuffle(nodes.begin(), nodes.end());
}

Graph::~Graph(){
    deleteAllEdges();
    delete [] array;
}

/* function sets random edges in graph by first building random MST
 * and then filling in remaining edges at random */
void Graph::setEdges(int m){

    vector<int> nodes;
    for (int i=0; i< n; i++){
        nodes.push_back(i);

    }
    random_shuffle(nodes.begin(), nodes.end());
    int curr = nodes[n-1];
    nodes.pop_back();
    for (int i= n-2; i>=0; i--){
        int next = nodes[i];
        insertEdge(curr, next);
        insertEdge(next, curr);
        edges.push(pair<int,int>(next, curr));
        curr = next;
    }
       //add in random remaining edges
    int n1= 0;
    int n2= 0;

    //add random (m-n) edges in graph
    for(int i=n-1; i< m; i++){
        while(n1==n2 || hasEdge(n1, n2)){
            n1 = rand() % n;
            n2 = rand() % n;
        }
        insertEdge(n1, n2);
        insertEdge(n2, n1);
        edges.push(pair<int,int>(n1,n2));
    }

    for (int i=0; i<n; i++){ //set components vector; all in same component (bc we built S.T.)
        component[i] = 0;
    }
    componentNum = 1;
}



int Graph::getNumNodes(){
    return n;
}

int Graph::getNumEdges(){
    return m;
}


set<int> Graph::returnAdjacentNodes(int i){
    set<int> adj;
    struct node* curr= array[i];
    while(curr->next != nullptr){
        curr = curr->next;
         adj.insert(curr->val);
    }
    return adj;
}



bool Graph::isEmpty(){
    for (int i=0; i<n; i++){
        if (array[i]->next != nullptr){
            return false;
        }
    }
    return true;
}



void Graph::printGraph(){
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if (i!=j){
                if (hasEdge(i,j)){
                    cout << "edge " << i<< ", " << j << " in graph"<< endl;
                }
            }
        }
    }
}


void Graph::insertEdge(int i, int j){
	if(!hasEdge(i,j)){
		struct node* curr = array[i];
		while(curr->next != nullptr){
			curr = curr->next;
		}
		curr->next = new struct node;
		curr->next->val = j;
		curr->next->next = nullptr;
    }
}


bool Graph::hasEdge(int i, int j){
	struct node* curr= array[i];
	while(curr->next != nullptr){
		curr = curr->next;
		if (curr->val == j){
			return true;
		}
	}
	return false;
}



void Graph::deleteAllEdges(){
    struct node* curr;
//    struct node* next;
    struct node* prev;
    for (int i=0; i< n; i++){ //clear linked list
        curr = array[i];
        // array[i]->next = nullptr;
        while (curr != nullptr){
            prev = curr;
            curr = curr->next;
            delete prev;

        }
    }
    queue<pair<int, int>>().swap(edges); //clear vector of edges
    numEdges = 0;
}


void Graph::deleteEdge(int i, int j){
	if (hasEdge(i,j)){
		struct node* curr = array[i];
		struct node * prev;
		while(curr->next != nullptr){
			prev = curr;
			curr = curr->next;
			if (curr->val == j){
				prev->next = curr->next;
				delete curr;
				return;
			}
		}
	}
}



void Graph::deleteComponent(int i){
    //delete all edges in component i belongs to
    set<int> adj = returnAdjacentNodes(i);
    for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){
        deleteEdge(i, *it);
        deleteEdge(*it, i);
        deleteComponent(*it);
    }

}

bool Graph::DFS(int i, int j, bool visited[]){
    visited[i] = true;
    set<int> adj = returnAdjacentNodes(i);  
    //add children to front of queue
    for(set<int>::iterator it = adj.begin(); it != adj.end(); it ++){
        if (*it == j){
            delete [] visited;
            return true;}
        if (!visited[*it]){
            return DFS(*it, j, visited);
        }
    }
    delete [] visited;
    return false;
}

bool Graph::isConnected(int i, int j){
    //check if i is connected to j. run DFS from i and return false if j not founds
    bool *visited = new bool[n];
    for (int i=0; i<n ;i++){
        visited[i] = false;
    }
    return DFS(i, j, visited);

}

pair<int, int> Graph::deleteRandomEdge(){
    if (edges.empty()){ 
        cout << "dude, graph is empty"<< endl;
        exit(1);
    }
    pair<int, int> ns = edges.front();
    edges.pop();
    // cout<< "deleting edge("<<ns.first<<","<<ns.second<<")"<<endl;
    deleteEdge(ns.first, ns.second);
    deleteEdge(ns.second, ns.first);
    numEdges--;
    return ns;
}

int Graph::getRandomNode(){
    int i = nodes[((int)nodes.size())-1];
    // nodes.pop_back();
    random_shuffle(nodes.begin(), nodes.end());
    return i;
}




