#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include "tinyxml2.h"
#include <iostream>
#include <assert.h>
#include "topology.hpp"

struct Edge{
    int n1;
    int n2;
    int weight;

    Edge(): n1(0), n2(0), weight(0) {}
    Edge(int _n1, int _n2, int _weight): n1(_n1), n2(_n2), weight(_weight) {}
};

struct Graph {
    std::vector<Point> nodes;
    std::vector<Edge> edges;

    Graph(tinyxml2::XMLDocument& input);
    Graph(std::vector<Point> _nodes, bool full);
    Graph MST();
    void add_and_connect(Point _point);
    void dump();
    int count_cost();
    void cleanup();
};

bool edge_cmp(Edge e1, Edge e2);

#endif