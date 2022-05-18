// File: p1test10.cpp
//
// Check copy constructor, destructor and assignment operator
//

#include <iostream>
using namespace std ;

#include "CBofCB.h"


bool InnerCB::inspect (int* &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffer ;
   cap = m_capacity ;
   size = m_size ;
   start = m_start ;
   end = m_end ;

   return true ;
}


bool InnerCBEqualityTest (InnerCB& A, InnerCB& B) {
   int *bufA ;
   int capA, sizeA, startA, endA ;
   A.inspect(bufA,capA,sizeA,startA,endA) ;

   int *bufB ;
   int capB, sizeB, startB, endB ;
   B.inspect(bufB,capB,sizeB,startB,endB) ;

   if (capA != capB) {
      cout << "*** Error: capacities differ\n" ;
      cout << "    capA = " << capA << endl ;
      cout << "    capB = " << capB << endl ;
      return false ;
   }

   if (sizeA != sizeB) {
      cout << "*** Error: sizes differ\n" ;
      cout << "    sizeA = " << sizeA << endl ;
      cout << "    sizeB = " << sizeB << endl ;
      return false ;
   }

   if (startA != startB) {
      cout << "*** Error: starting indices differ\n" ;
      cout << "    startA = " << startA << endl ;
      cout << "    startB = " << startB << endl ;
      return false ;
   }

   if (endA != endB) {
      cout << "*** Error: ending indices differ\n" ;
      cout << "    endA = " << endA << endl ;
      cout << "    endB = " << endB << endl ;
      return false ;
   }

   int loc ;
   for (int i=0 ; i < sizeA ; i++) {
      loc = (startA + i) % capA ;
      if (bufA[loc] != bufB[loc]) {
	 cout << "*** Error: stored values differ\n" ;
	 cout << "    bufA[" << loc << "] = " << bufA[loc] << endl ;
	 cout << "    bufB[" << loc << "] = " << bufB[loc] << endl ;
	 return false ;
      }
   }

   return true ;
}


bool CBofCB::inspect (InnerCB** &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffers ;
   cap = m_obCapacity ;
   size = m_obSize ;
   start = m_oldest ;
   end = m_newest ;

   return true ;
}


bool CBofCBEqualityTest(CBofCB &A, CBofCB &B) {

   InnerCB **bufA ;
   int capA, sizeA, startA, endA ;
   A.inspect(bufA,capA,sizeA,startA,endA) ;

   InnerCB **bufB ;
   int capB, sizeB, startB, endB ;
   B.inspect(bufB,capB,sizeB,startB,endB) ;

   if (capA != capB) {
      cout << "*** Error: Outer buffer capacities differ\n" ;
      cout << "    capA = " << capA << endl ;
      cout << "    capB = " << capB << endl ;
      return false ;
   }

   if (sizeA != sizeB) {
      cout << "*** Error: Outer buffer sizes differ\n" ;
      cout << "    sizeA = " << sizeA << endl ;
      cout << "    sizeB = " << sizeB << endl ;
      return false ;
   }

   if (startA != startB) {
      cout << "*** Error: Outer buffer starting indices differ\n" ;
      cout << "    startA = " << startA << endl ;
      cout << "    startB = " << startB << endl ;
      return false ;
   }

   if (endA != endB) {
      cout << "*** Error: Outer buffer ending indices differ\n" ;
      cout << "    endA = " << endA << endl ;
      cout << "    endB = " << endB << endl ;
      return false ;
   }

   int loc ;
   for (int i=0 ; i < sizeA ; i++) {
      loc = (startA + i) % capA ;
      if (! InnerCBEqualityTest( *(bufA[loc]), *(bufB[loc]) )) {
	 cout << "*** Error: Inner buffers hold different values at loc =" 
	      << loc << endl ;
	 return false ;
      }
   }

   return true ;
}


int main() {

   int data=1 ;
   CBofCB B ;

   // cout << "\n-------------------\n" ;
   // cout << "Add 50 items\n" ;
   for (int i=1 ; i <= 50 ; i++) {
      B.enqueue(data++) ;
   }
   // reportSizes(B) ;

   // cout << "\n-------------------\n" ;
   // cout << "Remove 15 items\n" ;
   for (int i=1 ; i <= 15 ; i++) {
      B.dequeue() ;
   }
   // reportSizes(B) ;

   // cout << "\n-------------------\n" ;
   // cout << "Add 1000 items\n" ;
   for (int i=1 ; i <= 1000 ; i++) {
      B.enqueue(data++) ;
   }
   // reportSizes(B) ;

   // cout << "\n-------------------\n" ;
   // cout << "Remove 200 items\n" ;
   for (int i=1 ; i <= 200 ; i++) {
      B.dequeue() ;
   }
   // reportSizes(B) ;

   CBofCB A(B) ;   // use copy constructor

   if (CBofCBEqualityTest(A,B)) {
      cout << "Passed equality test 1\n" ;
   } else {
      cout << "*** Failed equality test 1\n" ;
   }

   CBofCB *ptr1, *ptr2 ;
   ptr1 = new CBofCB(B) ;
   ptr2 = new CBofCB(*ptr1) ;
   delete ptr1 ;    // calls destructor

   if (CBofCBEqualityTest(*ptr2,B)) {
      cout << "Passed equality test 2\n" ;
   } else {
      cout << "*** Failed equality test 2\n" ;
   }

   delete ptr2 ;
  
   CBofCB X ;

   for (int i=1 ; i <= 43 ; i++) {
      X.enqueue(data++) ;
   }

   for (int i=1 ; i <= 12 ; i++) {
      X.dequeue() ;
   }

   for (int i=1 ; i <= 122 ; i++) {
      X.enqueue(data++) ;
   }

   for (int i=1 ; i <= 51 ; i++) {
      X.dequeue() ;
   }

   X = B ;   // uses assignment operator

   if (CBofCBEqualityTest(X,B)) {
      cout << "Passed equality test 3\n" ;
   } else {
      cout << "*** Failed equality test 3\n" ;
   }

   CBofCB Y ;

   for (int i=1 ; i <= 21 ; i++) {
      Y.enqueue(data++) ;
   }

   for (int i=1 ; i <= 9 ; i++) {
      Y.dequeue() ;
   }

   Y = X ;   // uses assignment operator again

   if (CBofCBEqualityTest(Y,B)) {
      cout << "Passed equality test 4\n" ;
   } else {
      cout << "*** Failed equality test 4\n" ;
   }

   return 0 ;
}
