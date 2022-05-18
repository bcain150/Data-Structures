// File: p1test02.cpp
//
// Bigger test of InnerCB class
//

#include <iostream>
using namespace std ;

#include "InnerCB.h"

int main() {

   int result ;
   InnerCB B(73) ;

   for (int i=0 ; i < 47 ; i++) {
      B.enqueue(100+i) ;
   }

   // add 3 items, then remove 3 items. repeat 500x.
   for (int i=0 ; i < 500 ; i++) {
      B.enqueue(1000+i) ;
      B.enqueue(2000+i) ;
      B.enqueue(3000+i) ;
      B.dequeue() ;
      B.dequeue() ;
      B.dequeue() ;
   }

   // add 4 items, then remove 4 items. repeat 500x.
   for (int i=0 ; i < 500 ; i++) {
      B.enqueue(4000+i) ;
      B.enqueue(5000+i) ;
      B.enqueue(6000+i) ;
      B.enqueue(7000+i) ;
      B.dequeue() ;
      B.dequeue() ;
      B.dequeue() ;
      B.dequeue() ;
   }


   // add & remove 7 items in a pattern. repeat 500x.
   for (int i=0 ; i < 500 ; i++) {
      B.enqueue(8000+i) ;
      B.dequeue() ;
      B.dequeue() ;
      B.enqueue(9000+i) ;
      B.enqueue(10000+i) ;
      B.dequeue() ;
      B.dequeue() ;
      B.dequeue() ;
      B.enqueue(11000+i) ;
      B.enqueue(12000+i) ;
      B.enqueue(13000+i) ;
      B.enqueue(14000+i) ;
      B.dequeue() ;
      B.dequeue() ;
   }


   B.dump() ;

   return 0 ;
}
