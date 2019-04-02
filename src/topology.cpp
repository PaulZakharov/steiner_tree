#include "topology.hpp"
#include <string.h>

Point::Point(int _x, int _y, int _layer) {
    this->x = _x;
    this->y = _y;
    this->layer = _layer;
}

Segment::Segment(int _x1, int _x2, int _y1, int _y2, Layer _layer) {
     if ((_x1 != _x2 & _layer == m3) | (_y1 != _y2 & _layer == m2)) {
        std::cout << "Contradictory properties of Segment!" << std::endl;
        std::cout << _x1 << " " << _y1 << " " << _x1 << " " << _y1 << " " << _layer << std::endl;
        assert(0);
    } 
    this->x1 = _x1;
    this->y1 = _y1;
    this->x2 = _x2;
    this->y2 = _y2;
    this->layer = _layer;
}

void Segment::to_xml(tinyxml2::XMLDocument& doc) {
    tinyxml2::XMLElement* res = doc.NewElement("segment");
    res->SetName("segment");
    res->SetAttribute("x1", this->x1);
    res->SetAttribute("y1", this->y1);
    res->SetAttribute("x2", this->x2);
    res->SetAttribute("y2", this->y2);
    switch (this->layer)
    {
        case Segment::Layer::m2:
            res->SetAttribute("layer", "m2");
            break;
        case Segment::Layer::m3:
            res->SetAttribute("layer", "m3");
            break;
        default:
            std::cout << x1 << x2 << y1 << y2 << layer << "Unlayered segment!" << std::endl;
            assert(0);
            break;
    }
    (doc.FirstChildElement("root"))->FirstChildElement("net")->InsertEndChild(res);
    return;
}

Hanan::Hanan(std::vector<Point>& pins){
    xs = {};
    ys = {};
    bool found;
    int i, j;
    for (auto& pin: pins) {
        found = false;
        for (i=0; i < xs.size(); i++) {
            if (pin.x == xs[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            xs.push_back(pin.x);
        }
        found = false;
        for (i=0; i < ys.size(); i++) {
            if (pin.y == ys[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            ys.push_back(pin.y);
        }
    }
    x_size = xs.size();
    y_size = ys.size();
}

void Point::to_xml(tinyxml2::XMLDocument& doc, int layer) {
    tinyxml2::XMLElement* res = doc.NewElement("point");
    res->SetName("point");
    res->SetAttribute("x", this->x);
    res->SetAttribute("y", this->y);
    switch (layer)
    {
        case 1:
            res->SetAttribute("type", "pin");
            res->SetAttribute("layer", "pins");
            break;
        case 2:
            res->SetAttribute("type", "via");
            res->SetAttribute("layer", "pins_m2");
            break;
        case 3:
            res->SetAttribute("type", "via");
            res->SetAttribute("layer", "m2_m3"); 
            break;   
        default:
            std::cout << "Point::to_xml misuse!" << std::endl;
            assert(0);
            break;
    }
    (doc.FirstChildElement("root"))->FirstChildElement("net")->InsertEndChild(res);
    return;
}


/*for (const auto& x : ISA_table) {
        if (x.match(raw))
            return x;
    }*/