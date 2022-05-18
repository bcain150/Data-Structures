// File: p1test01.cpp
//
// Simple test of InnerCB class
//

#include <iostream>
using namespace std ;

#include "InnerCB.h"

int main() {

   int result ;
   InnerCB B(7) ;

   B.enqueue(13) ;
   B.enqueue(14) ;
   B.enqueue(15) ;
   B.enqueue(16) ;
   B.enqueue(17) ;

   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;

   B.enqueue(21) ;
   B.enqueue(22) ;
   B.enqueue(23) ;
   B.enqueue(24) ;
   B.enqueue(25) ;

   B.dump() ;

   return 0 ;
}
