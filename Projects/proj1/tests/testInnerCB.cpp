
#include "InnerCB.h"
#include <stdexcept>
#include <iostream>

using namespace std;

int main() {
    InnerCB x = InnerCB();

    x.dump();
    for(int i=0; i<x.capacity(); i++) {
        x.enqueue(x.capacity()-i);
        x.dump();
    }

    cout << x.dequeue() << " dequeued" << endl;
    cout << x.dequeue() << " dequeued" << endl;
    cout << x.dequeue() << " dequeued" << endl;
    cout << x.dequeue() << " dequeued" << endl;

    x.dump();

    InnerCB y = InnerCB(x);
    InnerCB z = InnerCB(15);

    y.dump();
    z.dump();

    z = z;

    cout << "test self assignment\n" << endl;
    z.dump();

    cout << "test assignment opperator\n" << endl;
    z = x;
    z.dump();
    
    return 0;
}