#include <iostream>
using namespace std ;

//
// add your implementation of the initialize() function here
// (do not edit any other line of this file)
//
void initialize(int *&p, int s) {
    p = new int[s];
    for(int i=0; i<s; i++){
        p[i] = i;
    }
}

int main() {
   int *ptr ;
   int n = 10 ;

   initialize(ptr,n) ;

   for (int i=0 ; i < n ; i++) {
      cout << "ptr[" << i << "] = " << ptr[i] << endl  ;
   }

   int sum = 0 ;
   for (int i=0 ; i < n ; i++) {
      sum += ptr[i] ;
   }
   cout << "Sum = " << sum << endl ;

   delete [] ptr ;

   int *ptr2 ;
   initialize(ptr2,15) ;

   sum = 0 ;
   for (int i=0 ; i < 15 ; i++) {
      sum += ptr2[i] ;
   }
   cout << "Sum = " << sum << endl ;

   delete [] ptr2 ;
}