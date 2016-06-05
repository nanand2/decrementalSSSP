#include "randAPSP.h"
#include "naive.h"
#include "estree.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "ostreamlock.h"
using namespace std;




// void randomTest(double p){
//     //generate either delete with prob p or query with prob 1-p
//     int r = rand() % 2;
//     if (r==0){
//         e->deleteRandomEdge();
//     }
//     else{
//         e->queryRandomNode();
//     }
// }

// void test(int m, void* e){
   
//     //time ES tree
//     for (int i=0; i<m; i++){
//        e->deleteRandomEdge();
//     }

// }

mutex timex;


int main(int argc, char *argv[]){ //some tests for the graph
     //build sparse graph
    vector<std::thread> ts;  // worker   thread handles
    srand((int(time(0))));
    int n = stoi(argv[1]);
    int m = stoi(argv[2]);
    // n=300;
    n=1000;
    m=2500;

    cout << "testing es alg" << endl;
    // for (int j= 100; j<=4950; j+=0){
        // ts.push_back(thread([m, n, j](){
        int wrong = 0;
        int right = 0;
        // int p;
        // int q;
        // int d;
        // n=j;

         Graph* g = new Graph(m, n);
         // g->printGraph();
        // Graph* g = new Graph(10,10, 4);
         // Naive * na = new Naive(g, 1);
        // ESTree * na = new ESTree(g, 1);
          randAPSP* r = new randAPSP(g);
        std::chrono::high_resolution_clock::duration totalDelete = std::chrono::high_resolution_clock::duration::zero();
         for (int k= 0; k<1; k++){

        

        // FastESTree * na = new FastESTree(g, 1);
         //time ES tree
        
            for (int i=0; i<1000; i++){
               // cout <<i<<endl;
                // timex.lock();
               // na->deleteRandomEdge();
               // r->deleteRandomEdge();
               // g->printGraph();
               // na->queryRandomNode();
              auto start = std::chrono::high_resolution_clock::now();

               pair<pair<int, int>, int> pr = r->queryRandomNodesTest();               
               auto end = std::chrono::high_resolution_clock::now();

               Naive * na1 = new Naive(g, pr.first.first);
               int d2  = na1->query(pr.first.second);
               r->deleteRandomEdge();
               cout << "rand estimate: "<< pr.second << " naive estimate (exact): " << d2 <<endl;
               if (pr.second == d2){
                    right++;
               }
               else{
                    wrong++; 
               }
               totalDelete += end - start;
               delete na1;
               // timex.unlock();
            }
            
        // delete na;
        }

        double totDel = std::chrono::duration_cast<std::chrono::nanoseconds>(totalDelete).count() ;
        double deleteOps = totDel / (double) (100);
        cout << oslock<< n << " " << m << " "<< totDel << ", " << deleteOps <<endl <<osunlock;
        cout << "num right: " << right << "num wrong: " << wrong << endl;
        delete r;
    // }));
    //     if (j<100){
    //         j+=10;
    //     }
    //     else if (j<5000) {
    //         j+= 100;
    //     }
    //     else if (j<10000){
    //         j+=1000;
    //     }
    //     else{
    //         j+= 5000;
    //     }

    // }
    // for(auto& t: ts){
    //     t.join();
    // }

   

    // std::chrono::high_resolution_clock::duration totalDeleteN = std::chrono::high_resolution_clock::duration::zero();
    // cout << "naive"<<endl;
    // Graph * g = new Graph(m, n, 4);
    // Naive * na = new Naive(g, 1);
    //  //time ES tree
    // for (int i=0; i<min(100,m); i++){
    //     auto start = std::chrono::high_resolution_clock::now();
    //    na->deleteRandomEdge();
    //    auto end = std::chrono::high_resolution_clock::now();
    //    totalDeleteN += end - start;
    // }
    // double totDel = std::chrono::duration_cast<std::chrono::nanoseconds>(totalDeleteN).count();
    // double deleteOps =  totDel / (double) min(m,100);
    // cout <<  totDel << ", " << deleteOps <<endl;
    // g->~Graph();



  //   Naive * n = new Naive(g,1);
  //   // for (int i=0; i)
  //   g->printGraph();

  //   n->bfsTree->printGraph();
  //   // ssh_exchange_identification: Connection closed by remote host

  //   cout << "distance to 2:"<< n->distance[2] << endl;
  //   cout << "distance to 3:"<< n->distance[3] << endl;
  //   cout << "distance to 4:"<< n->distance[4] << endl;

  //   n->deleteRandomEdge();
  //   n->deleteRandomEdge();
  //   n->deleteRandomEdge();

  //   n->queryRandomNode();
  //    n->queryRandomNode();
  //     n->queryRandomNode();

  //     n->deleteRandomEdge();
  //   n->deleteRandomEdge();
  // n->queryRandomNode();
  //     n->queryRandomNode();


    
}