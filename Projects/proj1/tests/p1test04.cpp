// File: p1test04.cpp
//
// Test inspect() function of InnerCB class
//

#include <iostream>
using namespace std ;

#include "InnerCB.h"

bool InnerCB::inspect (int* &buf, int &cap, int &size, int &start, int &end) {
   buf = m_buffer ;
   cap = m_capacity ;
   size = m_size ;
   start = m_start ;
   end = m_end ;

   return true ;
}

int main() {

   int result ;
   InnerCB B(7) ;

   B.enqueue(11) ;
   B.enqueue(12) ;
   B.enqueue(13) ;
   B.enqueue(14) ;
   B.enqueue(15) ;
   B.enqueue(16) ;

   result = B.dequeue() ;
   result = B.dequeue() ;

   int *buf ;
   int cap, size, start, end ;
   B.inspect(buf,cap,size,start,end) ;

   cout << "buf   = " << buf  << endl ;
   cout << "cap   = " << cap  << endl ;
   cout << "size  = " << size  << endl ;
   cout << "start = " << start << endl ;
   cout << "end   = " << end   << endl ;

   cout << buf[start] << "  " ;
   cout << buf[start+1] << "  " ;
   cout << buf[start+2] << "  " ;
   cout << buf[start+3] << "  " ;

   cout << endl ;

   return 0 ;
}
