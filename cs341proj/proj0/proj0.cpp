// File: proj0.cpp
//
// CMSC 341 Fall 2018 Project 0
//
// Buggy program to be tested with gdb

#include <iostream>
#include <cmath>
#include "proj0.h"

using namespace std;

int main() {
   int previous, current, size;
   int *ptr ;
   previous = 0 ;

   for (int i = 1; i < 35; i++) {
      current = chaos(i);
      size = current - previous;

      ptr = new int[size];  // allocate array

      previous = current;
      delete[] ptr;
   }

   return 0;
}
