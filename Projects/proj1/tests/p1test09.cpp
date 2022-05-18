// File: p1test09.cpp
//
// Accuracy test of CBofCB class
//

#include <iostream>
#include <cstdlib>
#include <list>
using namespace std ;

#include "CBofCB.h"


void checkOne(list<int> &L, CBofCB& B) {
   if ( L.size() != B.size() ) {
      cout << "*** ERROR *** sizes of L & B do not match\n" ;
      cout << "    L.size() = " << L.size() << endl ;
      cout << "    B.size() = " << B.size() << endl ;
   }

   int dataL = L.front() ;
   L.pop_front() ;
   int dataB = B.dequeue() ;
   if ( dataL != dataB ) {
      cout << "*** ERROR *** data in L & B do not match\n" ;
      cout << "    dataL = " << dataL << endl ;
      cout << "    dataB = " << dataB << endl ;
   }
}


int main() {

   int data ;
   CBofCB B ;
   list<int> L ;

   srand(38701341) ;

   cout << "\n-------------------\n" ;
   cout << "Add 50 items\n" ;

   for (int i=1 ; i <= 50 ; i++) {
      data = rand() ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Remove 15 items\n" ;
   for (int i=1 ; i <= 15 ; i++) {
      checkOne(L,B) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Add 1000 items\n" ;
   for (int i=1 ; i <= 1000 ; i++) {
      data = rand() ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Remove 200 items\n" ;
   for (int i=1 ; i <= 200 ; i++) {
      checkOne(L,B) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Add 5000 items\n" ;
   for (int i=1 ; i <= 5000 ; i++) {
      data = rand() ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Remove 2000 items\n" ;
   for (int i=1 ; i <= 2000 ; i++) {
      checkOne(L,B) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Add 100000 items\n" ;
   for (int i=1 ; i <= 100000 ; i++) {
      data = rand() ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Remove 50000 items\n" ;
   for (int i=1 ; i <= 50000 ; i++) {
      checkOne(L,B) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Add 70000 items\n" ;
   for (int i=1 ; i <= 70000 ; i++) {
      data = rand() ;
      L.push_back(data) ;
      B.enqueue(data) ;
   }

   cout << "\n-------------------\n" ;
   cout << "Remove 123827 items\n" ;
   for (int i=1 ; i <= 123827 ; i++) {
      checkOne(L,B) ;
   }


   cout << "\n-------------------\n" ;
   cout << "Remove remaining items\n" ;
   int n = B.size() ;
   for (int i=1 ; i <= n ; i++) {
      checkOne(L,B) ;
   }

   return 0 ;
}
