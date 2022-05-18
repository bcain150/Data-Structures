// file: InnerCB.h
//
// UMBC CMSC 341 Fall 2018 Project 1
//
// Header file for Inner Circular Buffer.
// See project description for details.
//

#ifndef _INNERCB_H_
#define _INNERCB_H_

class InnerCB {

public:
   // Constructor, default size is 10.
   InnerCB(int n=10) ;

   // Copy constructor
   InnerCB(const InnerCB& other) ;

   // Destructor
   ~InnerCB() ;

   // Add item to circular buffer
   void enqueue(int data) ;

   // Remove item from circular buffer
   int dequeue() ;

   // True if no space left in buffer
   bool isFull() ; 

   // True if buffer holds no items
   bool isEmpty() ;

   // return maximum number of items this buffer can hold
   int capacity() ;

   // return number of items currently held in the buffer
   int size() ;

   // overloaded assignment operator
   const InnerCB& operator=(const InnerCB& rhs) ;

   // debugging function. Prints out contents.
   void dump() ;

   // grading function used to examine private data members.
   // Do not implement!
   bool inspect (int* &buf, int &cap, int &size, int &start, int &end) ;

private :
   int *m_buffer ;   // pointer to dynamically allocate array for buffer 
   int m_capacity ;  // length of the allocated space pointed by m_buffer
   int m_size ;      // # of items in the buffer
   int m_start ;     // index of the first (oldest) item in the buffer
   int m_end ;       // index of the last (newest) item in the buffer
} ;

#endif
