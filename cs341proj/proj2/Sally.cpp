// File: Sally.cpp
//
// CMSC 341 Fall 2018 Project 2
//
// Implementation of member functions of Sally Forth interpreter
//

#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <stdexcept>
#include <cstdlib>
using namespace std ;

#include "Sally.h"


// Basic Token constructor. Just assigns values.
//
Token::Token(TokenKind kind, int val, string txt) {
   m_kind = kind ;
   m_value = val ;
   m_text = txt ;
}


// Basic SymTabEntry constructor. Just assigns values.
//
SymTabEntry::SymTabEntry(TokenKind kind, int val, operation_t fptr) {
   m_kind = kind ;
   m_value = val ;
   m_dothis = fptr ;
}


// Constructor for Sally Forth interpreter.
// Adds built-in functions to the symbol table.
//
Sally::Sally(istream& input_stream) : istrm(input_stream) {
      // use member initializer to bind reference

      //original Keywords
      symtab["DUMP"] = SymTabEntry(KEYWORD, 0, &doDUMP);
      //Arithmatic Operations
      symtab["+"]    = SymTabEntry(KEYWORD, 0, &doPlus);
      symtab["-"]    = SymTabEntry(KEYWORD, 0, &doMinus);
      symtab["*"]    = SymTabEntry(KEYWORD, 0, &doTimes);
      symtab["/"]    = SymTabEntry(KEYWORD, 0, &doDivide);
      symtab["%"]    = SymTabEntry(KEYWORD, 0, &doMod);
      symtab["NEG"]  = SymTabEntry(KEYWORD, 0, &doNEG);
      //Printing Operations
      symtab["."]    = SymTabEntry(KEYWORD, 0, &doDot);
      symtab["SP"]   = SymTabEntry(KEYWORD, 0, &doSP);
      symtab["CR"]   = SymTabEntry(KEYWORD, 0, &doCR);

      //Stack Operations
      symtab["DUP"]  = SymTabEntry(KEYWORD, 0, &doDUP);
      symtab["DROP"] = SymTabEntry(KEYWORD, 0, &doDROP);
      symtab["SWAP"] = SymTabEntry(KEYWORD, 0, &doSWAP);
      symtab["ROT"]  = SymTabEntry(KEYWORD, 0, &doROT);

      //Variable Operations
      symtab["SET"]  = SymTabEntry(KEYWORD, 0, &doSET);
      symtab["@"]    = SymTabEntry(KEYWORD, 0, &doAt);
      symtab["!"]    = SymTabEntry(KEYWORD, 0, &doAssign);

      //Comparision Operations
      symtab["<"]    = SymTabEntry(KEYWORD, 0, &doLess);
      symtab["<="]   = SymTabEntry(KEYWORD, 0, &doLoE);
      symtab["=="]   = SymTabEntry(KEYWORD, 0, &doEqu);
      symtab["!="]   = SymTabEntry(KEYWORD, 0, &doNotEqu);
      symtab[">="]   = SymTabEntry(KEYWORD, 0, &doGoE);
      symtab[">"]    = SymTabEntry(KEYWORD, 0, &doGreat);

      //Logical Operations
      symtab["AND"]  = SymTabEntry(KEYWORD, 0, &doAND);
      symtab["OR"]   = SymTabEntry(KEYWORD, 0, &doOR);
      symtab["NOT"]  = SymTabEntry(KEYWORD, 0, &doNOT);

      //Control Structures
      symtab["IFTHEN"] = SymTabEntry(KEYWORD, 0, &doIFTHEN);
      symtab["ELSE"]   = SymTabEntry(KEYWORD, 0, &doELSE);
      symtab["ENDIF"]  = SymTabEntry(KEYWORD, 0, &doENDIF);
      //Loops
      symtab["DO"]     = SymTabEntry(KEYWORD, 0, &doDO);
      symtab["UNTIL"]  = SymTabEntry(KEYWORD, 0, &doUNTIL);

      loopMode = false;
}


// This function should be called when tkBuffer is empty.
// It adds tokens to tkBuffer.
//
// This function returns when an empty line was entered 
// or if the end-of-file has been reached.
//
// This function returns false when the end-of-file was encountered.
// 
// Processing done by fillBuffer()
//   - detects and ignores comments.
//   - detects string literals and combines as 1 token
//   - detetcs base 10 numbers
// 
//
bool Sally::fillBuffer() {
   string line ;     // single line of input
   int pos ;         // current position in the line
   int len ;         // # of char in current token
   long int n ;      // int value of token
   char *endPtr ;    // used with strtol()


   while(true) {    // keep reading until empty line read or eof

      // get one line from standard in
      //
      getline(istrm, line) ;   

      // if "normal" empty line encountered, return to mainLoop
      //
      if ( line.empty() && !istrm.eof() ) {
         return true ;
      }

      // if eof encountered, return to mainLoop, but say no more
      // input available
      //
      if ( istrm.eof() )  {
         return false ;
      }


      // Process line read

      pos = 0 ;                      // start from the beginning

      // skip over initial spaces & tabs
      //
      while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
         pos++ ; 
      }

      // Keep going until end of line
      //
      while (line[pos] != '\0') {

         // is it a comment?? skip rest of line.
         //
         if (line[pos] == '/' && line[pos+1] == '/') break ;

         // is it a string literal? 
         //
         if (line[pos] == '.' && line[pos+1] == '"') {

            pos += 2 ;  // skip over the ."
            len = 0 ;   // track length of literal

            // look for matching quote or end of line
            //
            while(line[pos+len] != '\0' && line[pos+len] != '"') {
               len++ ;
            }

            // make new string with characters from
            // line[pos] to line[pos+len-1]
            string literal(line,pos,len) ;  // copy from pos for len chars

            // Add to token list
            //
            tkBuffer.push_back( Token(STRING,0,literal) ) ;  

            // Different update if end reached or " found
            //
            if (line[pos+len] == '\0') {
               pos = pos + len ;
            } else {
               pos = pos + len + 1 ;
            }

         } else {  // otherwise "normal" token

            len = 0 ;  // track length of token

            // line[pos] should be an non-white space character
            // look for end of line or space or tab
            //
            while(line[pos+len] != '\0' && line[pos+len] != ' ' && line[pos+len] != '\t') {
               len++ ;
            }

            string literal(line,pos,len) ;   // copy form pos for len chars
            pos = pos + len ;

            // Try to convert to a number
            //
            n = strtol(literal.c_str(), &endPtr, 10) ;

            if (*endPtr == '\0') {
               tkBuffer.push_back( Token(INTEGER,n,literal) ) ;
            } else {
               tkBuffer.push_back( Token(UNKNOWN,0,literal) ) ;
            }
         }

         // skip over trailing spaces & tabs
         //
         while( line[pos] != '\0' && (line[pos] == ' ' || line[pos] == '\t') ) {
            pos++ ; 
         }

      }
   }
}



// Return next token from tkBuffer.
// Call fillBuffer() if needed.
// Checks for end-of-file and throws exception 
//
Token Sally::nextToken() {
      Token tk;
      bool more = true ;

      while(more && tkBuffer.empty()) {
         more = fillBuffer() ;
      }
      if ( !more && tkBuffer.empty()) {
         throw EOProgram("End of Program") ;
      }

      tk = tkBuffer.front() ;
      tkBuffer.pop_front() ;
      if(loopMode) loopList.push_back(tk);
      return tk ;
}


// The main interpreter loop of the Sally Forth interpreter.
// It gets a token and either push the token onto the parameter
// stack or looks for it in the symbol table.
//
//
void Sally::mainLoop() {

   Token tk ;
   map<string,SymTabEntry>::iterator it ;

   try {
      while( 1 ) {
         tk = nextToken() ;
         if (tk.m_kind == INTEGER || tk.m_kind == STRING) {

            // if INTEGER or STRING just push onto stack
            params.push(tk) ;

         } else {
            it = symtab.find(tk.m_text) ;
            
            if ( it == symtab.end() )  {   // not in symtab

               params.push(tk) ;

            } else if (it->second.m_kind == KEYWORD)  {

               // invoke the function for this operation
               // only if its processing or its one of the if statements
               it->second.m_dothis(this) ;

            } else if (it->second.m_kind == VARIABLE) {

               // variables are pushed as tokens
               //
               tk.m_kind = VARIABLE ;
               params.push(tk) ;

            } else {
               // default action
               //
               params.push(tk) ;

            }
         }
      }

   } catch (EOProgram& e) {

      cerr << "End of Program\n" ;
      if ( params.size() == 0 ) {
         cerr << "Parameter stack empty.\n" ;
      } else {
         cerr << "Parameter stack has " << params.size() << " token(s).\n" ;
      }
  
   } catch (out_of_range& e) {

      cerr << "Parameter stack underflow??\n" ;
      cerr << e.what() << endl; //prints out which operation caused it
      this->mainLoop(); 
      //goes back to the main loop again to continue executing but 
      //skips over error
   } catch (invalid_argument& e) {
         //catches invalid argument throws and continues execution
         cerr << "Invalid Arguments\n";
         cerr << e.what() << endl;
         this->mainLoop();

   } catch (logic_error& e) {
         //catches assignment error throws and continues execution
         cerr << "Assignment Error\n";
         cerr << e.what() << endl;
         this->mainLoop();

   } catch (...) {

      cerr << "Unexpected exception caught\n" ;
   }
}

// -------------------------------------------------------


void Sally::doPlus(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for +.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value + p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMinus(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for -.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value - p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doTimes(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for *.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value * p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doDivide(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for /.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value / p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doMod(Sally *Sptr) {
   Token p1, p2 ;

   if ( Sptr->params.size() < 2 ) {
      throw out_of_range("Need two parameters for %.") ;
   }
   p1 = Sptr->params.top() ;
   Sptr->params.pop() ;
   p2 = Sptr->params.top() ;
   Sptr->params.pop() ;
   int answer = p2.m_value % p1.m_value ;
   Sptr->params.push( Token(INTEGER, answer, "") ) ;
}


void Sally::doNEG(Sally *Sptr) {
   Token p ;

   if ( Sptr->params.size() < 1 ) {
      throw out_of_range("Need one parameter for NEG.") ;
   }
   p = Sptr->params.top() ;
   Sptr->params.pop() ;
   Sptr->params.push( Token(INTEGER, -p.m_value, "") ) ;
}


void Sally::doDot(Sally *Sptr) {

   Token p ;
   if ( Sptr->params.size() < 1 ) {
      throw out_of_range("Need one parameter for .") ;
   }

   p = Sptr->params.top() ;
   Sptr->params.pop() ;

   if (p.m_kind == INTEGER) {
      cout << p.m_value ;
   } else {
      cout << p.m_text ;
   }
}


void Sally::doSP(Sally *Sptr) {
   cout << " " ;
}


void Sally::doCR(Sally *Sptr) {
   cout << endl ;
}

//Dumps the entire contents of the parameter stack without actually
//manipulating it in any way
void Sally::doDUMP(Sally *Sptr) {
      Token p;
      cout << "param stack: ";
      for(stack<Token> dump = Sptr->params; !dump.empty(); dump.pop()) {
            p = dump.top();
            //if its an integer print out the value otherwise
            if (p.m_kind == INTEGER) cout << p.m_value << ", ";
            //assume its a string and print out the text
            else cout << p.m_text << ", ";
      }
      cout << endl;
} 

//takes the top parameter on the stack and pushes a duplicate 
//value on top
void Sally::doDUP(Sally *Sptr) {
      Token p;
      if (Sptr->params.empty())
            throw out_of_range("Need one parameter for DUP.");
      p = Sptr->params.top();
      Sptr->params.push(Token(INTEGER, p.m_value, ""));
}

//pops the first parameter off the stack
void Sally::doDROP(Sally *Sptr) {
      if (Sptr->params.empty())
            throw out_of_range("Need one parameter for DUP.");
      Sptr->params.pop();
}

//swaps the first 2 parameters on the stack
void Sally::doSWAP(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for SWAP.");
      
      //gets first two parameters from stack
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      //swaps them
      Sptr->params.push(p1);
      Sptr->params.push(p2);
}

//essentially a 3 parameter swap, rotates the first 3 parameters in the
//ex: if stack is 1, 2, 3 then a ROT will make it 3, 1, 2.
void Sally::doROT(Sally *Sptr) {
      Token p1, p2, p3;
      if (Sptr->params.size() < 3)
            throw out_of_range("Need three parameters for ROT.");

      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();
      p3 = Sptr->params.top();
      Sptr->params.pop();

      Sptr->params.push(p2);
      Sptr->params.push(p1);
      Sptr->params.push(p3);
}

/*This function initializes a variable into the symbol map
throws an exception if there arent enough parameters, syntax is invalid,
or you try call SET on the same variable twice (double initialization.)
*/
void Sally::doSET(Sally *Sptr) {
      Token p1, p2;
      //throws an exception if there arent enough parameters
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for SET.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      //throws error if incorrect syntax or set has already been called
      //for this variable
      if (Sptr->symtab.find(p1.m_text) != Sptr->symtab.end())
            throw logic_error("Cannot initialize a variable name has already been initialized.");
      else if (p1.m_kind != UNKNOWN)
            throw invalid_argument("1st parameter must be the variable name.");
      else if (p2.m_kind != INTEGER)
            throw invalid_argument("2nd parameter must be an integer.");

      //initializes a variable name in the symbol map
      Sptr->symtab[p1.m_text] = SymTabEntry(VARIABLE, p2.m_value);
}

//fetches the value stored as a specific variable and then pushes it onto the parameter stack
void Sally::doAt(Sally *Sptr) {
      Token p1;
      map<string,SymTabEntry>::iterator it;
      //checks the amount of parameters
      if (Sptr->params.size() < 1)
            throw out_of_range("Need a parameter for @.");
      p1 = Sptr->params.top();
      Sptr->params.pop();

      //checks whether the parameter is a variable and if it exists for access
      if(p1.m_kind != VARIABLE)
            throw invalid_argument("The parameter used by @ must be a variable.");
      it = Sptr->symtab.find(p1.m_text);
      if(it == Sptr->symtab.end()) 
            throw invalid_argument("The variable you tried to access does not exist.");
      
      //pushes the value that the variable is accessing onto the stack
      Sptr->params.push(Token(INTEGER, it->second.m_value, ""));
}

//Assigns a new value to a variable that has already been initialized
void Sally::doAssign(Sally *Sptr) {
      Token p1, p2;
      //throws an exception if there arent enough parameters
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for !.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      //throws error if incorrect syntax or set has already been called
      //for this variable
      if (p1.m_kind != VARIABLE)
            throw logic_error("Assignment attempted to a non-variable or a variable that does not exist.");
      else if (p2.m_kind != INTEGER)
            throw invalid_argument("2nd parameter must be an integer.");
      
      //assigns the value of the variable in the symbol map
      Sptr->symtab[p1.m_text].m_value = p2.m_value;
}

//does the less than boolean operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doLess(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for <.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");
      
      Sptr->params.push( Token(INTEGER, p2.m_value < p1.m_value));
}

//does the less than or equal to boolean operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doLoE(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for <=.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");
      
      Sptr->params.push( Token(INTEGER, p2.m_value <= p1.m_value));
}

//does the equal to boolean operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doEqu(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for ==.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");
      
      Sptr->params.push( Token(INTEGER, p2.m_value == p1.m_value));
}

//does the not equal to boolean operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doNotEqu(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for !=.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");
      
      Sptr->params.push( Token(INTEGER, p2.m_value != p1.m_value));
}

//does the greater than or equal to boolean operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doGoE(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for >=.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");
      
      Sptr->params.push( Token(INTEGER, p2.m_value >= p1.m_value));
}

//does the greater than boolean operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doGreat(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for >.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");
      
      Sptr->params.push( Token(INTEGER, p2.m_value > p1.m_value));
}

//does the and logical operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doAND(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for AND.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");

      Sptr->params.push( Token(INTEGER, p2.m_value && p1.m_value));
}

//does the or logical operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doOR(Sally *Sptr) {
      Token p1, p2;
      if (Sptr->params.size() < 2)
            throw out_of_range("Need two parameters for OR.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();
      p2 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER || p2.m_kind != INTEGER)
            throw invalid_argument("Both parameters must be integers.");

      Sptr->params.push( Token(INTEGER, p2.m_value || p1.m_value));
}

//does the not logical operation and pushes the result onto the stack
// false = 0, true = 1
void Sally::doNOT(Sally *Sptr) {
      Token p1;
      if (Sptr->params.empty())
            throw out_of_range("Need a single parameter for NOT.");
      
      p1 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER)
            throw invalid_argument("The parameter must be an integer.");

      Sptr->params.push( Token(INTEGER, !p1.m_value));
}

//Function that Checks to see if the top parameter is a boolean true.
//if it is then it will process upto its own ELSE statement
//otherwise it skips everything up to its own else statement
//and continues processing up to its endIF
void Sally::doIFTHEN(Sally *Sptr) {
      Token p1;
      if (Sptr->params.empty())
            throw out_of_range("Need a single parameter for IFTHEN.");

      p1 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER)
            throw invalid_argument("The parameter must be an integer.");
      
      if (!p1.m_value) { //if the value is 0 (false)
            int ifCnt = 0; //count increments if an ifthen is seen decrements if endif is
            p1 = Sptr->nextToken();
            while (p1.m_text != "ELSE" || ifCnt > 0) {
                  if (p1.m_text == "IFTHEN") ifCnt++;
                  else if (p1.m_text == "ENDIF") ifCnt--;
                  p1 = Sptr->nextToken();
            }
      }
}

//is only called when an IFTHEN function is true
//used to skip everything below it up to itself up to its own ENDIF
void Sally::doELSE(Sally *Sptr) {
      Token p1;
      int ifCnt = 0; //count increments if an ifthen is seen decrements if endif is
      p1 = Sptr->nextToken();
      while (p1.m_text != "ENDIF" || ifCnt > 0) {
            if (p1.m_text == "IFTHEN") ifCnt++;
            else if (p1.m_text == "ENDIF") ifCnt--;
            p1 = Sptr->nextToken();
      }
}

void Sally::doENDIF(Sally *Sptr) {
      //DOES NOTHING ONLY GETS RID OF THE ENDIF TOKEN IN THE STACK
}

void Sally::doDO(Sally *Sptr) {
      Sptr->listIt = Sptr->tkBuffer.begin();
      Sptr->loopMode = true;
}

void Sally::doUNTIL(Sally *Sptr) {
      Token p1;
      if (Sptr->params.empty())
            throw out_of_range("Need a single parameter for UNTIL.");

      p1 = Sptr->params.top();
      Sptr->params.pop();

      if (p1.m_kind != INTEGER)
            throw invalid_argument("The parameter must be an integer.");
      
      if(p1.m_value) {
            Sptr->tkBuffer.splice(Sptr->listIt, Sptr->loopList);

      }
      else {
            cout << "END THIS" << endl;
            Sptr->loopMode = false;
           
      }
}