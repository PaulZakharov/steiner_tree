#include "graph.hpp"
#include <algorithm>

Graph::Graph(tinyxml2::XMLDocument& input) {
    tinyxml2::XMLElement* net = input.RootElement()->FirstChildElement("net");
    if (net == NULL) {
        std::cout << "Net element not found in input file!" << std::endl;
        assert(0);
    }
    nodes = {};
    edges = {};
    int x_temp, y_temp; 
    int layer_temp;
    int w;
    tinyxml2::XMLElement* point = net->FirstChildElement("point");
    while(point != NULL) {
        x_temp = point->IntAttribute("x");
        y_temp = point->IntAttribute("y");
        layer_temp = 1;
        if (strcmp(point->Attribute("type"), "pin") != 0 | strcmp(point->Attribute("layer"), "pins") != 0) {
            std::cout << "Unexpected point type in input file!" << std::endl;
            assert(0);
        }
        nodes.push_back(Point(x_temp, y_temp, layer_temp));
        point = point -> NextSiblingElement("point");
    }
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = i+1; j < nodes.size(); j++) {
            w = abs(nodes[i].x-nodes[j].x) + abs(nodes[i].y-nodes[j].y);
            edges.push_back(Edge(i, j, w));
        }
    }
}

void Graph::add_and_connect(Point _point) {
    nodes.push_back(_point);
    int w;
    int index = nodes.size()-1;
    for (int i = 0; i < index; i++) {
        w = abs(nodes[i].x-nodes[index].x) + abs(nodes[i].y-nodes[index].y);
        edges.push_back(Edge(i, index, w));
    }
}

Graph Graph::MST() {
    Graph ret(this->nodes, false);
    std::vector<Edge> old_edges(this->edges);
    std::vector<int> IDs(ret.nodes.size());
    int i, j;
    int chosen = 0;
    int temp;
    int sz = (this->edges).size();
    std::sort(old_edges.begin(), old_edges.end(), edge_cmp);
    for (i=0; i < ret.nodes.size(); i++) IDs[i] = i;
    for (i=0; i < sz; i++) {
        if (IDs[old_edges[i].n1] != IDs[old_edges[i].n2]) {
            temp = IDs[old_edges[i].n1];
            for (j = 0; j < (ret.nodes).size(); j++) {
                if (IDs[j] == temp) {
                    IDs[j] = IDs[old_edges[i].n2];
                }
            }
            chosen++;
            ret.edges.push_back(old_edges[i]);
        }
        if (chosen == ret.nodes.size()-1) break;
    }
    return ret;
}

void Graph::dump() {
    for (int i=0; i<nodes.size(); i++) {
        std::cout << nodes[i].x << " " << nodes[i].y << std::endl;
    }
    for (int i=0; i<edges.size(); i++) {
        std::cout << edges[i].n1 << " " << edges[i].n2 << " " << edges[i].weight << std::endl;
    }
}

Graph::Graph(std::vector<Point> _nodes, bool full) {
    nodes = _nodes;
    edges = {};
    int w;
    if (full) {
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = i+1; j < nodes.size(); j++) {
                w = abs(nodes[i].x-nodes[j].x) + abs(nodes[i].y-nodes[j].y);
                edges.push_back(Edge(i, j, w));
            }
        }
    }
}

int Graph::count_cost() {
    int ret = 0;
    for (const auto& edge: edges) {
        ret += edge.weight;
    }
    return ret;
}

void Graph::cleanup(){
    std::vector<int> degrees(nodes.size(), 0);
    for (const auto& edge: edges) {
        degrees[edge.n1]++;
        degrees[edge.n2]++;
    }
    for (int i = 0; i < nodes.size(); i++){
        if (degrees[i] <= 2 & (nodes[i].layer % 2 != 1)) {
            for (int j = 0; j < edges.size(); j++) {
                if (edges[j].n1 == i | edges[j].n2 == i) edges.erase(edges.begin()+j);
            }
            nodes.erase(nodes.begin()+i);
        }
    }
}

bool edge_cmp(Edge e1, Edge e2){
    return  e1.weight < e2.weight;
}