ecc: main.o checks.o clique.o connection.o edge.o graph.o io.o node.o
	g++ -std=c++17 *.o -o ecc


main.o: main.cc
	g++ -std=c++17 main.cc -o main.o

checks.o: checks.cc checks.h
	g++ -std=c++17 checks.cc -o checks.o

clique.o: clique.cc clique.h
	g++ -std=c++17 clique.cc -o clique.o

connection.o: connection.cc connection.h
	g++ -std=c++17 connection.cc -o connection.o

edge.o: edge.cc edge.h
	g++ -std=c++17 edge.cc -o edge.o

graph.o: graph.cc graph.h
	g++ -std=c++17 graph.cc -o graph.o

io.o: io.cc io.h
	g++ -std=c++17 io.cc -o io.o

node.o: node.cc node.h
	g++ -std=c++ node.cc -o node.o

clean:
	rm *.o 