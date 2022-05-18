#ifndef BBOX_H
#define BBOX_H

#include <iostream>
#include "Point.h"

//
// Definition of struct to manage 2-D bounding box
// Assumptions:
//   1) bounding box dimension is power of 2.
//      This is not checked, but behavior is unpredictable if it isn't.
//   2) Values in this box would be
//      [m_bL.m_x .. m_bL.m_x+m_dim), [m_bL.m_y .. m_bL.m_y+m_dim)
//      (i.e., lower bound is inclusive, but upper bound is exclusive)
//
// Provided by Prof. Park for CMSC 341
//

class BBox {
public:
    BBox();
    BBox(const Point &bottomLeft, unsigned int dim);

    bool inBounds(const Point &pt) const;
    static unsigned int roundUpPow2(unsigned int val);

    Point m_bL;         // bottom-left corner of bounding box
    unsigned int m_dim; // bounding box size (assumes square box)

    friend std::ostream& operator<<(std::ostream &sout, const BBox &bBox);

};

#endif
