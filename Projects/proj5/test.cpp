// File: test3.cpp
//
// Insertions triggers incremental rehashing
// Further insertions make us give up on 
// rehashing and consolidate items into a single
// hash table.
//

#include <iostream>
#include <cstdlib>
using namespace std ;

#include "HashTable.h"


int main() {
   HashTable T(107) ;

   T.insert("undulation") ;     //  9
   T.insert("impertinent") ;    // 10
   T.insert("maladies") ;       // 10 -> 11
   T.insert("dominions") ;	// 12

   T.insert("waspish") ;        // 52
   T.insert("wildebeest") ;     // 52 -> 53
   T.insert("reaction") ;       // 52 -> 54

   T.insert("pawns") ;          // 43
   T.insert("vacuously") ;      // 43 -> 44
   T.insert("firth") ;          // 43 -> 45
   T.insert("manipulator") ;    // 43 -> 46
   T.insert("dreariness") ;     // 43 -> 47

   T.insert("insanity") ;       // 105
   T.insert("enthronement") ;   // 105 -> 106
   T.insert("falsifiers") ;     // 105 -> 0
   T.insert("ignominious") ;    // 105 -> 1
   T.insert("mummified") ;      // 105 -> 2

   T.insert("tributes") ;	// 21
   T.insert("skulduggery") ;	// 22
   T.insert("convulse") ;	// 23
   T.insert("frothed") ;	// 24
   T.insert("horrify") ;	// 25
   T.insert("blackmailers") ;	// 26
   T.insert("defenestrated") ;	// 27
   T.insert("garrison") ;	// 23 -> 28
   T.insert("lidless") ;	// 22 -> 29
   T.insert("eye") ;            // 21 -> 30, should trigger inc. rehash


   cout << "----------------------------------------------------\n" ;
   cout << "Hash table after rehash triggered\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nDo some insertions to make long linear probe sequence in the second table.\n\n" ;

   T.insert("wildcat") ;        // 18 (new table)
   T.insert("weightlessness") ; // 69 (new table)
   T.insert("sorceress") ;      // 33 (new table)
   T.insert("enchantress") ;    // 33 (new table) really.
   T.insert("witchery") ;       // 67 -> 68 (new table)
   T.insert("heliosphere") ;    // 67 -> 72 (new table)
   T.insert("obstruct") ;       // 67 -> 73 (new table)

   cout << "----------------------------------------------------\n" ;
   cout << "Hash table insertions.\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;

   cout << "\n\nOne more insertion in slot 67 should make us give up on rehashing.\n\n" ;
   T.insert("peripatetic") ;    // 67 -> 77 (new table)

   cout << "----------------------------------------------------\n" ;
   cout << "Hash table giving up on rehashing.\n" ;
   cout << "----------------------------------------------------\n" ;
   T.dump() ;
   cout << "----------------------------------------------------\n" ;

}