// File: p1test06.cpp
//
// Simple test of CBofCB class
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

   B.dump() ;

   return 0 ;
}
