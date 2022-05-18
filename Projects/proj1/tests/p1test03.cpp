// File: p1test03.cpp
//
// Test edge cases of InnerCB class
//

#include <iostream>
using namespace std ;

#include "InnerCB.h"

int main() {

   int result ;
   InnerCB B(7) ;

   B.enqueue(11) ;
   B.enqueue(12) ;
   B.enqueue(13) ;
   B.enqueue(14) ;
   B.enqueue(15) ;
   B.enqueue(16) ;
   B.enqueue(17) ;

   cout << "Capacity = " << B.capacity() << endl ;
   cout << "Size = " << B.size() << endl ;

   if ( B.isFull() ) {
     cout << "Correct, InnerCB B should be full.\n" ;
   } else {
     cout << "Huh? Why isn't InnerCB B full??\n" ;
   }


   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;
   result = B.dequeue() ;

   cout << "Capacity = " << B.capacity() << endl ;
   cout << "Size = " << B.size() << endl ;

   if ( B.isEmpty() ) {
     cout << "Correct, InnerCB B should be empty.\n" ;
   } else {
     cout << "Huh? Why isn't InnerCB B empty??\n" ;
   }

   return 0 ;
}
