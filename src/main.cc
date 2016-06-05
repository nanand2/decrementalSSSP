#include "randAPSP.h"
#include "naive.h"
#include "estree.h"
#include <iostream>
#include <string>
#include <chrono>
using namespace std;



/* run main function by calling ./run_tests */
/* DEFAULT: runs deletes on 100 node graph for 100-4950 edges, returns costs */
int main(int argc, char *argv[]){ //some tests for the graph
     //build sparse graph
    vector<std::thread> ts;  // worker   thread handles
    srand((int(time(0))));
    // int n = stoi(argv[1]);
    // int m = stoi(argv[2]);
    cout << "testing Fast ES tree algorithm on 100 node graph over 100 deletes" << endl;
    cout << "script prints edges, total update cost in ns " << endl;
    for (int j= 100; j<=4950; j+=0){
      Graph* g = new Graph(j, 100);
      FastESTree * f = new FastESTree(g,1);
      std::chrono::high_resolution_clock::duration totalDelete = std::chrono::high_resolution_clock::duration::zero();        
      for (int i=0; i<100; i++){
        auto start = std::chrono::high_resolution_clock::now();
        f->deleteRandomEdge();    
        if (i %10 ==0) {f->queryRandomNode();}
        auto end = std::chrono::high_resolution_clock::now();
        totalDelete += end - start;
      }
      double totDel = std::chrono::duration_cast<std::chrono::nanoseconds>(totalDelete).count() ;
      // double deleteOps = totDel / (double) (100);
      cout << j << " "<< totDel <<endl;
      delete f;
      delete g;
      j+=100;
    }
}