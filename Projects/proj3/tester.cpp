#include "QuadTree.h"


int main() {

    QuadTree q = QuadTree(BBox(Point(0,0), 64));

    q.set(Point(10, 10), 20);
    // q.set(Point(10, 15), 25);
    // q.set(Point(50, 34), 13);
    // q.set(Point(62, 34), 13);        
    // q.set(Point(16, 45), 13);
    // q.set(Point(58, 34), 13);
    // q.set(Point(23, 12), 13);
    // q.set(Point(39, 4), 13);

    QuadTree::iterator it = q.begin();

    cout << *it << endl;
    

    return 0;
}