//
// Definition of struct to manage 2-D bounding box
// Assumptions:
//   1) bounding box dimension is power of 2
//   2) bounding box is aligned to some multiple of dimension
//   3) Values in box are [m_bL.m_x .. m_bL.m_x+m_dim) (i.e., lower bound
//      is inclusive, upper bound is exclusive)
// Provided by Prof. Park for CMSC 341
//

#include "BBox.h"
#include "Point.h"


BBox::BBox() {}  // Needed because of existence of other constructors

//
// Supplemental constructor for creating pre-filled object
//
BBox::BBox(const Point &bottomLeft, unsigned int dim) {
    m_bL = bottomLeft;
    m_dim = dim;
}

//
// Determines whether pt is within the bounding box.
//
// Return value:
//   true if in bounding box, else false
//
bool
BBox::inBounds(const Point &pt) const {
    return (pt.m_x >= m_bL.m_x && pt.m_x <= m_bL.m_x + ((int) m_dim - 1) &&
	    pt.m_y >= m_bL.m_y && pt.m_y <= m_bL.m_y + ((int) m_dim - 1));
}
    
//
//
//
unsigned int
BBox::roundUpPow2(unsigned int val) {
    for (unsigned int i = 1; i; i <<= 1) { // stop if we wrap
	if (i >= val) {
	    return i;
	}
    }
    return 0;
}


std::ostream& operator<<(std::ostream &sout, const BBox &bBox) {
    sout << "{botLeft=" << bBox.m_bL << ", dim=" << bBox.m_dim << "}";
    return sout;
}

