// File: p1test08.cpp
//
// Big test of CBofCB class
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
   cout << "Add 50 items\n" ;
   for (int i=1 ; i <= 50 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 15 items\n" ;
   for (int i=1 ; i <= 15 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 1000 items\n" ;
   for (int i=1 ; i <= 1000 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 200 items\n" ;
   for (int i=1 ; i <= 200 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 5000 items\n" ;
   for (int i=1 ; i <= 5000 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 2000 items\n" ;
   for (int i=1 ; i <= 2000 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 100000 items\n" ;
   for (int i=1 ; i <= 100000 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 50000 items\n" ;
   for (int i=1 ; i <= 50000 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Add 70000 items\n" ;
   for (int i=1 ; i <= 70000 ; i++) {
      B.enqueue(data++) ;
   }
   reportSizes(B) ;

   cout << "\n-------------------\n" ;
   cout << "Remove 123827 items\n" ;
   for (int i=1 ; i <= 123827 ; i++) {
      B.dequeue() ;
   }
   reportSizes(B) ;

   cout << "\n\n-----------------------------------------\n" ;
   cout << "Final dump:\n" ;
   B.dump() ;

   return 0 ;
}
