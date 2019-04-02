#include "routed.hpp"

Routed::Routed(Graph& graph) {
    points = graph.nodes;
    Point* p1;
    Point* p2;
    segments = {};
    for (int i=0; i<graph.edges.size(); i++) {
        p1 = &(points[graph.edges[i].n1]);
        p2 = &(points[graph.edges[i].n2]);
        if ((p1->x != p2->x) && (p1->y != p2->y)) {
            p1->layer |= 4;
            p2->layer |= 2;
            segments.push_back(Segment(p1->x, p1->x, p1->y, p2->y, Segment::Layer::m3));
            segments.push_back(Segment(p1->x, p2->x, p2->y, p2->y, Segment::Layer::m2));
            points.push_back(Point(p1->x, p2->y, 6));            
        } else if ((p1->x != p2->x) && (p1->y == p2->y)) {
            p1->layer |= 2;
            p2->layer |= 2;
            segments.push_back(Segment(p1->x, p2->x, p1->y, p2->y, Segment::Layer::m2));
        } else if ((p1->x == p2->x) && (p1->y != p2->y)) {
            p1->layer |= 4;
            p2->layer |= 4;
            segments.push_back(Segment(p1->x, p2->x, p1->y, p2->y, Segment::Layer::m3));
        } else {
            std::cout << "Two equal points or a loop in a tree!!" << std::endl;
            assert(0);
        }
    }

}

void Routed::to_xml(const char* path, int min_x, int min_y, int max_x, int max_y){
    tinyxml2::XMLDocument output;
    tinyxml2::XMLElement* root = output.NewElement("root");
    tinyxml2::XMLElement* net = output.NewElement("net");
    tinyxml2::XMLElement* grid = output.NewElement("grid");
    grid->SetAttribute("min_x", min_x);
    grid->SetAttribute("max_x", max_x);
    grid->SetAttribute("min_y", min_y);
    grid->SetAttribute("max_y", max_y);
    Segment dummy(0, 0, 0, 0, Segment::Layer::m2);
    output.InsertFirstChild(root);
    root->InsertFirstChild(grid);
    root->InsertEndChild(net);
    tinyxml2::XMLElement* temp;
    for (int i=0; i<points.size(); i++) {
        switch (points[i].layer)
        {
            case 3: //pin and via to m2
                //std::cout << "Point written 3" << std::endl;
                points[i].to_xml(output, 1);
                points[i].to_xml(output, 2);
                break;
            case 5: //pin and vias to m3, dummy m2 segment
                //std::cout << "Point written 5" << std::endl;
                points[i].to_xml(output, 1);
                points[i].to_xml(output, 2);
                points[i].to_xml(output, 3);
                dummy.x1 = points[i].x;
                dummy.x2 = points[i].x;
                dummy.y1 = points[i].y;
                dummy.y2 = points[i].y;
                dummy.to_xml(output);
                break;
            case 6: //via m2-m3
                //std::cout << "Point written 6" << std::endl;
                points[i].to_xml(output, 3);
                break;
            case 7: //pin and vias to m2, m3
                //std::cout << "Point written 7" << std::endl;
                points[i].to_xml(output, 1);
                points[i].to_xml(output, 2);
                points[i].to_xml(output, 3);
                break;
            default:
                std::cout << "Incorrect layer value! " << points[i].x << " " << points[i].y << " " << points[i].layer << std::endl;
                assert(0);
                break;
        }
    }
    for (int i=0; i<segments.size(); i++) {
        segments[i].to_xml(output);
    }
    output.SaveFile(path);
    return;
}

void Routed::dump(){
    for (const auto& point: points) {
        std::cout << point.x << point.y << point.layer << std::endl;
    }
    for (const auto& edge: segments) {
        std::cout << edge.x1 << edge.x2 << edge.y1 << edge.y2 << edge.layer << std::endl;
    }
}