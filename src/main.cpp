#include <iostream>
#include "tinyxml2.h"
#include "topology.hpp"
#include "graph.hpp"
#include "routed.hpp"
#include <string>

int count_cost(const Graph& counted) {}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Input file not specified" << std::endl;
        return -1;
    }
    std::string filepath(argv[1]);
    std::string filename = filepath.substr(0, filepath.size()-4);
    std::string fileext = filepath.substr(filepath.size()-4, 4);
    std::cout << "File: " << filename << std::endl;
    if (fileext.compare(".xml") != 0) {
        std::cout << "Input file doesn't have .xml extension" << std::endl;
        return -1;
    }
    tinyxml2::XMLDocument input;
	if (input.LoadFile(argv[1]) != 0) {
        std::cout << "XML Parsing error" << std::endl;
        return -1;
    }

	tinyxml2::XMLElement* el_grid = input.RootElement()->FirstChildElement("grid");
    tinyxml2::XMLElement* el_net = input.RootElement()->FirstChildElement("net");
    
    int min_x = el_grid->IntAttribute("min_x");
    int max_x = el_grid->IntAttribute("max_x");
    int min_y = el_grid->IntAttribute("min_y");
    int max_y = el_grid->IntAttribute("max_y");
	//std::cout << "Grid parameters:" << min_x << " " << max_x << " " << min_y << " " << max_y << std::endl;
    
    int cost, new_cost;
    bool found = true;
    Graph initial(input);
    Graph* expanded = new Graph(initial.nodes, true);
    *expanded = expanded->MST();
    //expanded->dump();
    cost = expanded->count_cost();
    std::cout << "Initial Cost: " << cost << std::endl;
    std::cout << "_______________" << std::endl;
    int i_min, j_min;
    Hanan grid(initial.nodes);
    while (found) {
        found = false;
        for (int i = 0; i < grid.x_size; i++){
            for (int j = 0; j < grid.y_size; j++) {
                Point* candidate = new Point(grid.xs[i], grid.ys[j], 0);
                *expanded = Graph(initial.nodes, true);
                expanded->add_and_connect(*candidate);
                *expanded = expanded->MST();
                new_cost = expanded->count_cost();
                if (new_cost < cost) {
                    found = true;
                    i_min = i;
                    j_min = j;
                    cost = new_cost;
                }
                delete candidate;
            }
        }
        if (found) {
            Point* added = new Point(grid.xs[i_min], grid.ys[j_min], 0);
            *expanded = Graph(initial.nodes, true);
            expanded->add_and_connect(*added);
            *expanded = expanded->MST();
            expanded->cleanup();
            initial = Graph(expanded->nodes, true);
            delete added;
        }
    }
    initial = initial.MST();
    //initial.cleanup();
    //initial.dump();
    new_cost = initial.count_cost();
    std::cout << "Cost: " << new_cost << std::endl;
    Routed out(initial);
    std::string filepath_out = filename + "_out.xml";
    //std::cout << filepath_out.c_str() << std::endl;
    out.to_xml(filepath_out.c_str(), min_x, min_y, max_x, max_y);
    return 0;
}