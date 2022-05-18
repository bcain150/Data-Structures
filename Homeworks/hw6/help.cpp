


#include <iostream>

using namespace std;

int main(){

    int x;
    int sec;
    int val;
    do {
        cout << "Enter a number: ";

        cin >> x;

        val = (3 * x + 5) % 11;

        cout << "the hash of this number is: " << val << endl;

        cout << "secondary hash? (1 for yes, 0 for no): ";
        cin >> sec;

        if(sec) {
            for(int i=0; i<11; i++) {
                cout << "can you put " << x << " at position " << (val + (7 - (i % 7)))%11 << "? (1 for yes, 0 for no): ";
                cin >> sec;
                if(sec) i = 11;
            }
        }
    }while(x > 0);

    return 0;
}