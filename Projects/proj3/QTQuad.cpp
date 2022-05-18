//Brendan Cain
//CMSC 341 Proj3
//QTQuad Class implementation

#include "QTQuad.h"


/*  A constructor that initializes a QTQuad properly. 
    It should run in O(1) time.
*/
QTQuad::QTQuad() {
    for(int i=0; i<QTQ_NUM_QUADS; i++) m_quads[i] = NULL;
    m_cells = NULL;
    incPrune = false;
}

//Destructor that only deletes something if its not NULL
QTQuad::~QTQuad() {
    if(m_cells != NULL) { //if it's a leaf node
        delete[] m_cells;
        m_cells = NULL;
    }
    else { //if it's an internal node
        for(int i=0; i<QTQ_NUM_QUADS; i++) {
            if(m_quads[i] != NULL) {
                delete m_quads[i];
                m_quads[i] = NULL;
            }
        }
    }
}

/*  helper function that returns the quadrant that a point would be in
    given the current bounds. returns an enum in the form of an int
    that represents the index in the current QTQuad's m_quads member var
*/
int QTQuad::getQuad(const Point &pt, const BBox &bounds) {
    int bhalf = bounds.m_dim/2;
    Point bL = Point(bounds.m_bL.m_x, bounds.m_bL.m_y);

    if((pt.m_y-bL.m_y) < bhalf) {
        if((pt.m_x-bL.m_x) < bhalf) return I;
        else return II;
    }
    else {
        if((pt.m_x-bL.m_x) < bhalf) return III;
        else return IV;
    }
}

/*  a helper function that takes in the current QTQuad bounds and
    the desired quadrant to move into and returns the modified BBox
    object that represents the bounds of the quadrant specified
*/
BBox QTQuad::getBounds(const BBox &bounds, int quad) {
    Point bL;
    int bhalf = bounds.m_dim/2; //half the dimensions for the next quadrant
    switch(quad) {  // modify the bounds depending on what quadrant you're going to
        case I:
            bL = Point(bounds.m_bL.m_x, bounds.m_bL.m_y);
            break;
        case II:
            bL = Point(bounds.m_bL.m_x + bhalf, bounds.m_bL.m_y);
            break;
        case III:
            bL = Point(bounds.m_bL.m_x, bounds.m_bL.m_y + bhalf);
            break;
        case IV:
            bL = Point(bounds.m_bL.m_x + bhalf, bounds.m_bL.m_y + bhalf);
            break;
        default:
            bL = Point(0, 0);
    }
    return BBox(bL, bhalf);
}

/*  A member function to get the value stored at 2-D coordinate position 
    in a QuadTree. Returns the cell's value. Recursive.
*/
int QTQuad::get(const Point &pt, const BBox &bounds) {
    if(m_cells == NULL) { //if this is an internal node this must be true
        int q = getQuad(pt, bounds); //gets the quadrant that the point is looking for
        //if there are no more child quads then everything in that quadrant is 0
        if(m_quads[q] == NULL) return 0;
        //return the value at a lower sub quadtrant with bounds relative to Quad q
        else return m_quads[q]->get(pt, getBounds(bounds, q));
    }
    //if m_cells is not NULL than this must be a leaf node with cell data
    return m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y];
}

/*  A member function to set the value stored at 2-D coordinate position 
    in a QuadTree. Only handles non zero numbers. Recursive
*/
void QTQuad::set(const Point &pt, int data, const BBox &bounds) {
    int q = getQuad(pt, bounds); //gets the quadrant that the point is looking for
    //if the quadrant is not null then recurse further
    if(m_quads[q] != NULL) m_quads[q]->set(pt, data, getBounds(bounds, q));
    //otherwise the branch doesnt exist or you have reached the leaf
    else if(bounds.m_dim == QTQ_GRID_DIM) { //is it at the leaf
        //if so check to see if its allocated if not allocate it,
        //and set all points to zero.
        if(m_cells == NULL) {
            m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
            for(int i=0; i<QTQ_GRID_DIM; i++) {
                for(int j=0; j<QTQ_GRID_DIM; j++) m_cells[i][j] = 0;
            }
        }
        //set point in grid
        m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y] = data;
    }
    //if its not at the leaf then allocate a new branch or quadrant
    else {
        m_quads[q] = new QTQuad();
        m_quads[q]->set(pt, data, getBounds(bounds, q)); //recurse further
    }
}

/*  Helper function that handles setting a value to zero and recursively
    pruning it. If the node is pruned it returns true, otherwise false
*/
bool QTQuad::prune(const Point &pt, const BBox &bounds) {
    int q = getQuad(pt, bounds); //gets the quadrant that the point is looking for
    //if the quadrant exists recurse down
    if(m_quads[q] != NULL) {
        //checks to see if the next quadrant has been pruned
        if(m_quads[q]->prune(pt, getBounds(bounds, q))) { 
            //if it has then delete it
            delete m_quads[q];
            m_quads[q] = NULL;
            //checks to see if every quadrant in this node is null
            for(int i=1; i<QTQ_NUM_QUADS; i++)
                if(m_quads[(q+i)%QTQ_NUM_QUADS] != NULL) return false;
            //if it is null then prune this node too. otherwise stop prunning
            return true;
        }
        //if it wasnt pruned stop pruning
        else return false;
    }
    //if it doesnt exist see if you are at leaf
    else if(bounds.m_dim == QTQ_GRID_DIM) {
        if(m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y] != 0) {
            //if the point is not zero set it to zero.
            m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y] = 0;
            //if you set a point in the grid to zero check the rest of the grid
            //for a non zero number.
            for(int i=0; i<QTQ_GRID_DIM; i++) {
                for(int j=0; j<QTQ_GRID_DIM; j++) {
                    //if one is found the grid stays
                    if(m_cells[i][j] != 0) return false;
                }
            }
            return true; //if no zero is found return true to delete this QTQuad
        }
        //if the point were looking for is already zero do not prune
        //because there must be another point in the grid that is not zero
        //for the grid to even exist
        else return false;
    }
    //if you arent at the leaf and the next quadrant doesnt exist
    //then this quadrant is a quadrant of all zeros
    else return false;
}

/*  A member function to modify the value stored at 2-D coordinate 
    position in a QuadTree by a requested amount. The delta is signed, 
    so this function can actually increment or decrement, by any amount. 
    increment() without a second argument increases the value by 1. 
    Returns the cell's new value. delta will never be zero
*/
int QTQuad::increment(const Point &pt, int delta, const BBox &bounds) {
    int q = getQuad(pt, bounds); //gets the quadrant that the point is looking for
    int data;
    //if the quadrant is not null then recurse further
    if(m_quads[q] != NULL) {
        data = m_quads[q]->increment(pt, delta, getBounds(bounds, q));
        //check boolean value to see if you need to delete or prune further
        if(m_quads[q]->incPrune) {
            //delete m_quads[q] instance of QTQuad if need to prune.
            delete m_quads[q];
            m_quads[q] = NULL;
            //check if you need to prune further up
            for(int i=1; i<QTQ_NUM_QUADS; i++) {
                if(m_quads[(q+i)%QTQ_NUM_QUADS] != NULL)
                    return data;
            }
            //update the prune boolean for increment funct to delete obj when this
            //instance of the recursive function returns
            incPrune = true;
            return data;
        }
        return data; //return data if prune was uneccessary.
    }
    //otherwise the branch doesnt exist or you have reached the leaf
    else if(bounds.m_dim == QTQ_GRID_DIM) { //is it at the leaf
        //if so check to see if its allocated if not allocate it,
        //and set all points to zero.
        if(m_cells == NULL) {
            m_cells = new int[QTQ_GRID_DIM][QTQ_GRID_DIM];
            for(int i=0; i<QTQ_GRID_DIM; i++) {
                for(int j=0; j<QTQ_GRID_DIM; j++) m_cells[i][j] = 0;
            }
            //sets the cell in m_cell and returns
            m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y] = delta;
            return delta;
        }
        //increment point in grid
        m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y] += delta;
        data = m_cells[pt.m_x-bounds.m_bL.m_x][pt.m_y-bounds.m_bL.m_y];
        //(pruning isnt necessary if true)
        if(data != 0) return data; //return if the resulting value isnt 0
        //see if pruning is necessary if function did not yet return
        //that means resulting value "data" is 0        
        for(int i=0; i<QTQ_GRID_DIM; i++) {
            for(int j=0; j<QTQ_GRID_DIM; j++) {
                if(m_cells[i][j] != 0) return data;
            }
        }
        //update the prune boolean for increment funct to delete obj when this
        //instance of the recursive function returns
        incPrune = true;
        return data;
    }
    //if its not at the leaf then allocate a new branch or quadrant
    else {
        m_quads[q] = new QTQuad();
        return m_quads[q]->increment(pt, data, getBounds(bounds, q)); //recurse further
    }
}

/*  A member function to clear a QuadTree by setting all the cells to 0.
    It does this by actually pruning the entire tree, making all the cells
    virtual, and the tree very compact.
*/
void QTQuad::clearAll() {
    for(int i=0; i<QTQ_NUM_QUADS; i++) {
        if(m_quads[i] != NULL) {
            m_quads[i]->clearAll();
            delete m_quads[i];
            m_quads[i] = NULL;
        }
    }
}