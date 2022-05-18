#include <cstddef>
#include <cstring>

#include "Point.h"
#include "BBox.h"
#include "QTQuad.h"

using namespace std;


static const char *dirNames[] = {"Bottom-Left", "Bottom-Right",
			   "Top-Left", "Top-Right"};

void
QTQuad::dump(const BBox &bounds) {
    cout << "[QTQuad bounds=" << bounds;
    if (m_cells == NULL) { // Internal node
	cout << " is INTERNAL:\n";
	for (int q = 0; q < QTQ_NUM_QUADS; q++) {
	    cout << dirNames[q] << " child:" << endl;
	    if (m_quads[q] == NULL) {
		cout << "[NULL]" << endl;
	    } else {
		BBox kidBBox(bounds);  // Use parent's bounds as base

		kidBBox.m_dim /= 2;    // cut it in half
		if (q & 1) {
		    kidBBox.m_bL.m_x += kidBBox.m_dim;
		}
		if (q & 2) {
		    kidBBox.m_bL.m_y += kidBBox.m_dim;
		}
		m_quads[q]->dump(kidBBox);
	    }
	}
    } else {
	cout << " is GRID LEAF:\n";
	for (int y = QTQ_GRID_DIM - 1; y >= 0; --y) {
	    cout << "[";
	    for (int x = 0; x < QTQ_GRID_DIM; x++) {
		char c;

		if (m_cells[x][y] < 0) {
		    c = '-';
		} else if (m_cells[x][y] > 15) {
		    c = '*';
		} else if (m_cells[x][y] > 9) {
		    c = 'a' + m_cells[x][y] - 10;
		} else {
		    c = '0' + m_cells[x][y];
		}
		cout << c;
	    }
	    cout << "]" << endl;
	}
    }
    cout << "]" << endl;
}
