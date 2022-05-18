// file: CBofCB.h
//
// UMBC CMSC 341 Fall 2018 Project 1
//
// Header file for Circular Buffer of Circular Buffer.
// See project description for details.
//

#ifndef _CBOFCB_H_
#define _CBOFCB_H_

#include "InnerCB.h" 

class CBofCB {

public:
   // default constructor
   CBofCB() ;

   // copy constructor
   CBofCB(const CBofCB& other) ;

   // destructor
   ~CBofCB() ;

   // add item to this data structure
   void enqueue(int data) ;

   // remove item from this data structure
   int dequeue() ;

   // returns true if cannot add more items
   bool isFull() ; 

   // returns true if no items stored in data structure
   bool isEmpty() ;

   // number of items in the data structure as a whole.
   // Note: not the number of InnerCB's
   int size() ;

   // overloaded assignment operator
   const CBofCB& operator=(const CBofCB& rhs) ;

   // debugging function, prints out contents of data structure
   void dump() ;

   // grading function. Do not implement!
   bool inspect (InnerCB** &buf, int &cap, int &size, int &start, int &end) ;

private :

   // max number of Inner Circular Buffers
   //
   static const int m_obCapacity=7 ;

   // array of pointers to InnerCB's.
   // Each entry of the array is a pointer.
   // Note: array itself is NOT dynamically allocated
   InnerCB * m_buffers[m_obCapacity] ;

   int m_obSize ;  // number of inner circular buffers in the outer CB
   int m_oldest ;  // index of the oldest circular buffer (start)
   int m_newest ;  // index of the newest circular buffer (end)
} ;

#endif
