#include <iostream>
#include "QTQuad.h"
#include "QuadTree.h"

using namespace std;

void QuadTree::dump() {
    cout << "============================================================\n"
	 << "START Dump of quadtree: true bounds = " << m_bounds << "\n"
	 << "------------------------------------------------------------"
	 << endl;
    m_root->dump(m_qBounds);
    cout << "------------------------------------------------------------\n"
	 << "END Dump of quadtree\n"
	 << "============================================================"
	 << endl;
}
