#ifndef _ROUTED_H_
#define _ROUTED_H_

#include <vector>
#include "topology.hpp"
#include "graph.hpp"
#include "tinyxml2.h"

struct Routed {
    std::vector<Point> points;
    std::vector<Segment> segments;

    Routed(Graph& graph);
    void to_xml(const char* path, int min_x, int min_y, int max_x, int max_y);
    void dump();
};

#endif