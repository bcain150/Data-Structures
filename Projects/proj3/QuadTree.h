#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"

class QuadTree {

public:

    // Nested class definition for associated iterator type
    //
    class iterator {
	// if pointing to quadrants, should traverse in quadrant order:
	// quadrant I, then II, then III, then IV, depth-first recursively;
	// at grid leaf level, traverse cells in row-major form
	// ([0][0], [0][1], etc.)
    public:
	iterator();
	~iterator();
	bool operator==(const QuadTree::iterator &other);
	bool operator!=(const QuadTree::iterator &other);
	// Following two operators move on to next non-zero cell
	iterator &operator++();          // Prefix: e.g. "++it"
	iterator operator++(int dummy);  // Postfix: "it++"
	Point &operator*();

	const iterator &operator=(const iterator &rhs);

    private:
	//
	// YOU CAN ADD OTHER MEMBER DATA, HELPER FUNCTIONS, ETC.
	// AS NEEDED HERE.
	//
    //the point that the iterator is currently pointing to.
    Point m_curr;
    //coordinates to the point in the grid QTQuad
    int m_gridX;
    int m_gridY;
    //dynamically allocated array that holds the path to the grid 
    //node via quadrants
    int *m_quadPath;
    int m_depth;

	friend QuadTree;
    };
    
    //
    // Now, the stuff for the main class:
    //
    QuadTree();  // Creates a 16x16 default board
    QuadTree(const BBox &bounds);
    ~QuadTree();

    // Retrieve cell value
    int get(const Point &pt);

    // Set cell value
    void set(const Point &pt, int data);

    // Modify cell value by a signed delta amount; faster then get() + set()
    int increment(const Point &pt, int delta = 1); // 

    // Clear all cells in tree
    void clearAll();

    // return iterator to first non-zero cell; == end() if qtree empty
    iterator begin();

    // iterator pointing beyond last cell;
    iterator end();

    // Print out structure of tree
    void dump();

    // Grading function.  Do not implement!
    bool inspect(QTQuad * &root, BBox &bounds, BBox &m_qBounds);

private:

    // Root of the entire QTQuad tree; always non-NULL
    QTQuad *m_root;

    // Holds the actual bounds of the board requested by the user
    BBox m_bounds;

    // Holds the expanded power-of-2 dimensioned bounding box that defines
    // the root quadrant of the tree
    BBox m_qBounds;

    //
    // YOU CAN ADD ANY OTHER CONSTRUCTORS, MEMBER FUNCTIONS, MEMBER DATA, ETC.
    // AS NEEDED HERE.
    //


};

#endif
