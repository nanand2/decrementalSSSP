CXXFLAGS =  -std=c++11  -Wall -Werror -O0 -g

CXX = g++

OBJECTS = main.o graph.o estree.o fast_estree.o naive.o randAPSP.o 

default: run-tests

run-tests: $(OBJECTS); $(CXX) $(CXXFLAGS) -o $@ $^ -pthread

main.o: src/main.cc include/graph.h include/estree.h include/randAPSP.h 

graph.o: src/graph.cc include/graph.h

estree.o: src/estree.cc include/estree.h include/graph.h

naive.o: src/naive.cc include/naive.h include/graph.h

fast_estree.o: src/fast_estree.cc include/graph.h

randAPSP.o: src/randAPSP.cc include/randAPSP.h include/estree.h 

clean:; rm -f run-tests *.o *~
