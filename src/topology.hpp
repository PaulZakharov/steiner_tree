#ifndef _TOP_H_
#define _TOP_H_

#include <assert.h>
#include <iostream>
#include <array>
#include <vector>
#include "tinyxml2.h"

struct Point{
    int x;
    int y;
    int layer;
    Point(): x(0), y(0), layer(0){}
    Point(int _x, int _y, int _layer);
    Point(const Point& other): x(other.x), y(other.y), layer(other.layer){}
    Point operator = (const Point& other) {
        this->layer = other.layer;
        this->x = other.x;
        this->y = other.y;
        return *this;
    }
    void to_xml(tinyxml2::XMLDocument& doc, int layer);
};

struct Segment {
    enum Layer {
        m2=0, m3
    };
    int x1;
    int x2;
    int y1;
    int y2;
    Layer layer;
    Segment(): x1(0), x2(0), y1(0), y2(0), layer(m2) {}
    Segment(int _x1, int _x2, int _y1, int _y2, Layer _layer);
    void to_xml(tinyxml2::XMLDocument& doc);
};

struct Hanan {
    std::vector<int> xs;
    std::vector<int> ys;
    int x_size;
    int y_size;
    Hanan(std::vector<Point>& pins);
    Hanan(): xs({}), ys({}), x_size(0), y_size(0) {}
};

#endif