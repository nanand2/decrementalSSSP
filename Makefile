CXXFLAGS = -std=c++11 -Wall -Werror -O0 -g
CXX = g++

OBJECTS = main.o graph.o estree.o fast_estree.o naive.o randAPSP.o 

default: run-tests

run-tests: $(OBJECTS); $(CXX) $(CXXFLAGS) -o $@ $^ -pthread

main.o: main.cc graph.h estree.h randAPSP.h 

graph.o: graph.cc graph.h

estree.o: estree.cc estree.h graph.h

naive.o: naive.cc naive.h graph.h

fast_estree.o: fast_estree.cc graph.h

randAPSP.o: randAPSP.cc randAPSP.h estree.h 

clean:; rm -f run-tests *.o *~