#include "Point.h"
//
// Definition of a simple struct to manage 2-D point coordinates
// Provided by Prof. Park for CMSC 341
// Should be self-explanatory.  Note that "==" is overloaded
//

Point::Point() {}  // Needed because of existence of other constructors

Point::Point(int x, int y) {
    m_x = x;
    m_y = y;
}

// Note: automatically get copy constructor

bool
Point::operator== (const Point &other) const {
    return (m_x == other.m_x && m_y == other.m_y);
}

std::ostream& operator<<(std::ostream &sout, const Point &pt) {
    sout << "(" << pt.m_x << ", " << pt.m_y << ")";
    return sout;
}
