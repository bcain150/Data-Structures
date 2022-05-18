//Brendan Cain
//CMSC 341 Proj3
//QuadTree Class implementation

#include "QuadTree.h"

/*  A default constructor that initializes a QuadTree properly, 
    as a 16 x 16 grid with origin (0, 0). It should run in O(1) time.
*/
QuadTree::QuadTree() {
    //creates a new root
    m_root = new QTQuad();
    //creates default bounds
    m_bounds = BBox(Point(0,0), 16);
    m_qBounds = BBox(Point(0,0), 16);
}

/*  A 1-parameter constructor that initializes a QuadTree to the request
     size, at the request origin. It should run in O(1) time.
*/
QuadTree::QuadTree(const BBox &bounds) {
    m_root = new QTQuad();

    m_bounds = bounds;
    m_qBounds = BBox(Point(0,0), bounds.roundUpPow2(bounds.m_dim));
}

/*  A destructor that cleans up the entire tree, recovering all 
    dynamically allocated space. Your implementation must not leak memory.
*/
QuadTree::~QuadTree() {
    delete m_root;
    m_root = NULL;
}

/*  A member function to get the value stored at 2-D coordinate position 
    in a QuadTree. Returns the cell's value.
    calls get function from QTQuad
*/
int QuadTree::get(const Point &pt) {
    if(!(m_bounds.inBounds(pt))) // checks to see if the point is out of range
        throw out_of_range("The point you requested is out of range or does not exist");

    else {
        //creates a point with respect to (0, 0)
        Point p = Point(pt.m_x-m_bounds.m_bL.m_x, pt.m_y-m_bounds.m_bL.m_y);
        //makes a call to the recursive QTQuad get function
        return m_root->get(p, m_qBounds);
    }
}

/*  A member function that goes to a 2-D Coordinate position and stores
    the specified value at that point. Calls the set function from QTQuad
    m_root.
*/
void QuadTree::set(const Point &pt, int data) {
    if(!(m_bounds.inBounds(pt))) // checks to see if the point is out of range
        throw out_of_range("The point you requested is out of range or does not exist");
    else {
        //puts the point in terms of (0,0)
        Point p = Point(pt.m_x-m_bounds.m_bL.m_x, pt.m_y-m_bounds.m_bL.m_y);
        //if the data is 0 then we need to try and prune the tree
        if (data == 0) m_root->prune(pt, m_qBounds);
        else m_root->set(p, data, m_qBounds); //calls the recursive QTQuad set function
    }
}

/*  A member function to modify the value stored at 2-D coordinate 
    position in a QuadTree by a requested amount. The delta is signed,
    so this function can actually increment or decrement, by any amount.
    increment() without a second argument increases the value by 1.
    Returns the cell's new value.
*/
int QuadTree::increment(const Point &pt, int delta) {
    if(!(m_bounds.inBounds(pt))) // checks to see if the point is out of range
        throw out_of_range("The point you requested is out of range or does not exist");
    Point p = Point(pt.m_x-m_bounds.m_bL.m_x, pt.m_y-m_bounds.m_bL.m_y);
    if(delta != 0) {
        m_root->incPrune = false;
        return m_root->increment(p, delta, m_qBounds);
    }
    else return m_root->get(p, m_qBounds);
}

/*  A member function to clear a QuadTree by setting all the cells to 0.
    It does this by actually pruning the entire tree, making all the cells
    virtual, and the tree very compact.
*/
void QuadTree::clearAll() {
    m_root->clearAll(); 
}


////////////////////////////////////QUADTREE ITERATORS//////////////////////////

/*  this function returns an iterator that points to the first non-zero 
    element in the quadTree
*/
QuadTree::iterator QuadTree::begin() {
    QuadTree::iterator it = iterator();
    QTQuad *temp = m_root;
    int dim = m_qBounds.m_dim;
    //save bL corner of the bounds
    BBox bounds = BBox(m_bounds.m_bL, dim);
    //demension must be bigger than the grid dimension for begin to be down
    // a branch otherwise the grid is m_cells of temp
    if(m_qBounds.m_dim != QTQ_GRID_DIM) {
        //initialize new quadPath array to the depth of the tree
        //for loop finds the depth of the tree
        for(it.m_depth = 0; dim > QTQ_NUM_QUADS; it.m_depth++) dim = dim/2;
        it.m_quadPath = new int[it.m_depth]; //(only needed if branches exist)

        //go down the tree and keep track of when you go further down
        for(int i=0; i<it.m_depth; i++) {
            for(int j=0; j<QTQ_NUM_QUADS; j++) {
                if(temp->m_quads[j] != NULL) {
                    //updates the bounds
                    bounds = temp->m_quads[j]->getBounds(bounds, j);
                    //updates the path to the grid leaf node and the current node
                    temp = temp->m_quads[i];
                    it.m_quadPath[i] = j;
                    break; //breaks out of inner for loop to find the direction
                    //of the next quadrant
                }
                else it.m_quadPath[i] = -1;
            }
            //if no path was set then there is no data in the quadtree
            if(it.m_quadPath[i] = -1) return end();
        }
    }
    
    //get the coordinates in the leaf node grid
    for(int i=0; i<QTQ_GRID_DIM; i++) {
        for(int j=0; j<QTQ_GRID_DIM; j++) {
            if(temp->m_cells[i][j] != 0) {
                it.m_gridX = i;
                it.m_gridY = j;
            }
        }
    }
    //set point that the iterator reflects
    it.m_curr = Point(bounds.m_bL.m_x + it.m_gridX, bounds.m_bL.m_y + it.m_gridY);
    return it;
}

/*  returns the iterator that points to the end of the quadtree.
    pointing "to the end" is essentially just having the point
    that is immidiately off the grid to the top right and diagonal
*/
QuadTree::iterator QuadTree::end() {
    int dim = m_qBounds.m_dim;
    QuadTree::iterator it = iterator();
    //the end iterator hols a point that does not exist on the 2D grid
    it.m_curr = Point(m_bounds.m_bL.m_x + dim, m_bounds.m_bL.m_y + dim);
    return it;
}

/*  Quadtree iterator constructor
    does not allocate the path array, that is done by begin();
*/
QuadTree::iterator::iterator() {
    m_gridX = 0;
    m_gridY = 0;
    m_quadPath = NULL;
    m_depth = 0;
}

/*  QuadTree iterator destructor
    just deletes the path array as its the only dynamically allocated variable
*/
QuadTree::iterator::~iterator() {
    if(m_quadPath != NULL) {
        delete [] m_quadPath;
        m_quadPath = NULL;
    }
}

/*  derefrencing operator
    returns the point that the iterator is refrencing
*/
Point &QuadTree::iterator::operator*() {
    return m_curr;
}

/*  equivalency comparison operator
    compares both x and y values of each respective refrenced point
    and returns the boolean value
*/
bool QuadTree::iterator::operator==(const QuadTree::iterator &other) {
    return m_curr.m_x == other.m_curr.m_y && m_curr.m_y == other.m_curr.m_y;
}

/*  not-equal-to comparison operator
    compares both x and y values of each respective refrenced point
    and returns the boolean value
*/
bool QuadTree::iterator::operator!=(const QuadTree::iterator &other) {
    return m_curr.m_x != other.m_curr.m_y && m_curr.m_y != other.m_curr.m_y;
}

/*  Assignment operator for a QuadTree iterator
    sets all member variables to that of the right hand side
    allocates memory if needed
*/
const QuadTree::iterator &QuadTree::iterator::operator=(const iterator &rhs) {
    m_curr = Point(rhs.m_curr.m_x, rhs.m_curr.m_y);
    m_gridX = rhs.m_gridX;
    m_gridY = rhs.m_gridY;
    m_depth = rhs.m_depth;

    if(m_quadPath == NULL) m_quadPath = new int[m_depth];
    for(int i=0; i<m_depth; i++) m_quadPath[i] = rhs.m_quadPath[i];

    return *this;
}

// /*  Prefix increment operator for the QuadTree iterator
// */
// QuadTree::iterator QuadTree::iterator::&operator++() {

// }

// /*  Prefix increment operator for the QuadTree iterator
// */
// QuadTree::iterator QuadTree::iterator::operator++(int dummy) {

// }



