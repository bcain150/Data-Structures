// File: p1test07.cpp
//
// Second test of CBofCB class
//

#include <iostream>
using namespace std ;

#include "CBofCB.h"

int main() {

   int result ;
   CBofCB B ;

   B.enqueue(11) ;
   B.enqueue(12) ;
   B.enqueue(13) ;
   B.enqueue(14) ;
   B.enqueue(15) ;
   B.enqueue(16) ;
   B.enqueue(17) ;

   result = B.dequeue() ;
   cout << "result should be 11: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 12: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 13: " << result << endl ;

   B.enqueue(18) ;
   B.enqueue(19) ;
   B.enqueue(20) ;
   B.enqueue(21) ;
   B.enqueue(22) ;
   B.enqueue(23) ;
   B.enqueue(24) ;
   B.enqueue(25) ;
   B.enqueue(26) ;
   B.enqueue(27) ;
   B.enqueue(28) ;
   B.enqueue(29) ;
   B.enqueue(30) ;
   B.enqueue(31) ;
   B.enqueue(32) ;

   result = B.dequeue() ;
   cout << "result should be 14: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 15: " << result << endl ;

   // B.dump() ;

   result = B.dequeue() ;
   cout << "result should be 16: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 17: " << result << endl ;
   result = B.dequeue() ;

   B.enqueue(33) ;
   B.enqueue(34) ;
   B.enqueue(35) ;

   cout << "result should be 18: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 19: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 20: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 21: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 22: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 23: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 24: " << result << endl ;

   B.enqueue(36) ;
   B.enqueue(37) ;
   B.enqueue(38) ;

   result = B.dequeue() ;
   cout << "result should be 25: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 26: " << result << endl ;
   result = B.dequeue() ;
   cout << "result should be 27: " << result << endl ;

   B.dump() ;

   // add 3, remove 3, repeat a bit
   for (int i=39 ; i < 64 ; i+=3) {
      B.enqueue(i) ;
      B.enqueue(i+1) ;
      B.enqueue(i+2) ;
      B.dequeue() ;
      B.dequeue() ;
      B.dequeue() ;
   }

   B.dump() ;

   return 0 ;
}
