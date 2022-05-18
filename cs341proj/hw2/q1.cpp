#include <iostream>
using namespace std ;

class Two {
public:
  int part1 ;
  int part2 ;
} ;

int main() {

   Two * dp[10] ;

   // fill in code here
   for(int i=0; i < 10; i++){
     dp[i] = new Two();
     dp[i] -> part1 = 0;
     dp[i] -> part2 = 0;

     delete dp[i];
   }

   return 0 ;
}