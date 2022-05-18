// File: p1test12.cpp
//
// Timing test #2
//

#include <iostream>
using namespace std ;

#include "CBofCB.h"

bool CBofCB::inspect (InnerCB** &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffers ;
   cap = m_obCapacity ;
   size = m_obSize ;
   start = m_oldest ;
   end = m_newest ;

   return true ;
}


void reportSizes(CBofCB &B) {
   InnerCB** buffers ;
   int cap ;
   int size ;
   int start ;
   int end ;

   B.inspect(buffers, cap, size, start, end) ;
   
   cout << "Total size = " << B.size() << endl ;

   int loc ;
   for (int i=0 ; i < size ; i++) {
      loc = (start + i) % cap ;
      cout << "m_buffers[" << loc << "]->size() = " 
           << buffers[loc]->size() << endl ;
   }
}



int main() {

   int data=1 ;
   CBofCB B ;

   cout << "\n-------------------\n" ;
   cout << "Add 1270 items\n" ;
   for (int i=1 ; i <= 1270 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 630 items\n" ;
   for (int i=1 ; i <= 630 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 80640 items\n" ;
   for (int i=1 ; i <= 80640 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 40320 items\n" ;
   for (int i=1 ; i <= 40320 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 5160960 items\n" ;
   for (int i=1 ; i <= 5160960 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 2580480 items\n" ;
   for (int i=1 ; i <= 2580480 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 330301440 items\n" ;
   for (int i=1 ; i <= 330301440 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 165150720 items\n" ;
   for (int i=1 ; i <= 165150720 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 335,544,310 items\n" ;
   for (int i=1 ; i <= 335544310 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 503316470 items\n" ;
   for (int i=1 ; i <= 503316470 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Total of 2 * 335,544,310 items added and removed\n" ;


   return 0 ;
}
