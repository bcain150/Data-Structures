// File: driver2cpp
//
// CMSC 341 Fall 2018 Project 2
//
// Simple driver program to call the Sally Forth interpreter
// This version accepts user input for filename of Sally Forth
// source code.
//


#include <iostream>
#include <fstream>
using namespace std ;

#include "Sally.h"

int main(int argc, char *argv[]) {
   if(argc != 2) {
       cerr << "error incorrect number of arguments" << endl;
       return 0;
   }

   ifstream ifile(argv[1]) ;

   Sally S(ifile) ;

   S.mainLoop() ;

   ifile.close() ;
   return 0 ;
}