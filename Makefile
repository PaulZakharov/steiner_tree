all: main
	-$(RM) *.o libtinyxml2.a

main: main.o graph.o topology.o routed.o libtinyxml2.a
	g++ -o main main.o graph.o topology.o routed.o libtinyxml2.a

main.o: src/main.cpp
	g++ -c -o main.o src/main.cpp

graph.o: src/graph.cpp src/graph.hpp
	g++ -c -o graph.o src/graph.cpp

topology.o: src/topology.cpp src/topology.hpp
	g++ -c -o topology.o src/topology.cpp	

routed.o: src/routed.cpp src/routed.hpp
	g++ -c -o routed.o src/routed.cpp

clean:
	-$(RM) *.o main libtinyxml2.a

libtinyxml2.a: tinyxml2.o
	ar cr $@ $^
	ranlib $@

tinyxml2.o: src/tinyxml2.cpp src/tinyxml2.h
	g++ -c -o tinyxml2.o src/tinyxml2.cpp