/**
 * Author: Mike Nguyen
 * Date: 2023-05-31
 * Source: CP-Algorithms
 * Description: Compute Minkowski sum of 2 convex polygons. 
 * Status: tested
*/
#include "../geometry/Point.h"

using P = Point<ll>;

void rotate_polygon(vector<P>& poly) {
    size_t pos = 0;
    for(size_t i = 1; i < poly.size(); i++){
        if(poly[i].y < poly[pos].y || (poly[i].y == poly[pos].y && poly[i].x < poly[pos].x))
            pos = i;
    }
    rotate(begin(poly), begin(poly)+pos, end(poly));
}

vector<P> minkowski(vector<P> poly1, vector<P> poly2) {
    rotate_polygon(poly1); rotate_polygon(poly2);
    size_t n = poly1.size(), m = poly2.size();
    poly1.push_back(poly1[0]);
    poly1.push_back(poly1[1]);
    poly2.push_back(poly2[0]);
    poly2.push_back(poly2[1]);
    vector<P> ret;
    size_t i = 0, j = 0;
    while (i < n || j < m) {
        ret.push_back(poly1[i] + poly2[j]);
        auto c = (poly1[i+1] - poly1[i]).cross(poly2[j+1]-poly2[j]);
        i += c >= 0 && i < n;
        j += c <= 0 && j < m;
    }
    return ret;
}