/*  File Name: HashTable.cpp
    Created By: Brendan Cain
    Date: 12-07-18 6:10pm
    Description: 
    -implementation for HashTable.h
    -includes function definitions
    -a HashTable object that consists of an incremental rehash feature
    -maximum size of 2e5 - 1 (199,999)
*/

#include "HashTable.h"


//sentinel value for whenever an item is delted from the table
char * const HashTable::DELETED = (char *) &DELETED;


//--------REQUIRED MEMBER FUNCTIONS--------
/*  Constructor for HashTable Object
    -takes in requested initial table capacity n
    -if n is less than the min size then set it to min size
    -if n is greater than max size throw an out of range exception
    -round up n to the nearest prime number
    -allocate 1st hash table and initialize to NULL
    -initialize all other m_variables to 0, NULL, or false.
*/
HashTable::HashTable(int n) {
    if(n < MIN_TABLE_SIZE) n = MIN_TABLE_SIZE;
    if(n > MAX_TABLE_SIZE) 
        throw out_of_range("The table size you requested is too large.\n");
    
    m_capOld = roundUpPrime(n); //rounds up the request to the nearest prime
    m_oldHash = new char *[m_capOld]; //allocates elements
    //initializes all elements to NULL
    for(int i=0; i<m_capOld; i++) m_oldHash[i] = NULL;

    //initializes other member variables
    m_sizeOld = 0;
    m_newHash = NULL;
    m_capNew = 0;
    m_rehashMode = false;
    cpyFlag = false;
}

/*  Destructor:
    -deallocates cstrings with free() function
    -calls delete on dynamically allocated arrays
    -only deletes newHash table if it's currently in rehashMode
*/
HashTable::~HashTable() {
    //loops through entire oldHash table
    for(int i=0; i<m_capOld; i++) {
        //frees the cstring if slot is not NULL or DELETED
        if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED) {
            free(m_oldHash[i]);
            m_oldHash[i] = NULL;
            m_sizeOld--;
        }
        //breaks if we've freed all possible cstrings
        if(m_sizeOld <= 0) break;
    }

    //delete the oldHash table array
    delete[] m_oldHash;
    m_oldHash = NULL;

    if(m_rehashMode) {
        //loops through entire oldHash table
        for(int i=0; i<m_capNew; i++) {
            //frees the cstring if slot is not NULL or DELETED
            if(m_newHash[i] != NULL && m_newHash[i] != DELETED) {
                free(m_newHash[i]);
                m_newHash[i] = NULL;
                m_sizeNew--;
            }
            //breaks if we've freed all possible cstrings
            if(m_sizeNew <= 0) break;
        }
        //delete the newHash table array
        delete[] m_newHash;
        m_newHash = NULL;
    }
}

HashTable::HashTable(HashTable& other) {
    
    if(other.isRehashing()) {
        other.cpyFlag = true;
        other.wrapUpRehash();
        other.cpyFlag = false;
    }

    m_capOld = other.m_capOld;
    m_oldHash = new char *[m_capOld]; //allocates elements

    //initializes other member variables
    m_sizeOld = other.m_sizeOld;
    m_newHash = NULL;
    m_capNew = 0;
    m_rehashMode = false;
    cpyFlag = false;

    for(int i=0; i<m_capOld; i++) {
        if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED)
            m_oldHash[i] = strdup(other.at(i));
    }
}

const HashTable& HashTable::operator=(HashTable rhs) {
    if(this != &rhs) {
        
        if(rhs.isRehashing()) {
            rhs.cpyFlag = true;
            rhs.wrapUpRehash();
            rhs.cpyFlag = false;
        }

        if(m_rehashMode) {
            cpyFlag = true;
            if(isRehashing()) wrapUpRehash();
            cpyFlag = false;
        }

        for(int i=0; i<m_capOld; i++) {
            if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED) free(m_oldHash[i]);
        }

        m_capOld = rhs.m_capOld;
        m_sizeOld = rhs.m_sizeOld;

        for(int i=0; i<m_capOld; i++) {
            if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED)
                m_oldHash[i] = strdup(rhs.at(i));
        }
    }
    return *this;
}

/*  HashTable Insert Operation:
    -will attempt to wrap up a rehash
    -will not insert anything if str is already in HashTable obj
    -if rehash mode, inserts string in new hash table
    -checks load factor, turns on rehash mode if over max
    -inserts into old hash table through linear probing
    -turns on rehash mode if old hash table probs more than max probe
    -calls rehashCluser on any hash that hits max probe
*/
void HashTable::insert(const char *str) {
    //attempts to move everything to the new hash and make it the old
    wrapUpRehash();

    //if rehashing and it's also not in the old table rehash str to new table
    if(m_rehashMode && !find(str)) rehash(strdup(str));

    //if the loadFactor isn't greater than the max load factor start rehash
    else if(loadFactor() > MAX_LOAD) {
        m_rehashMode = true;
        secondHashInit();
        //only rehash current str if its not in data structure
        if(!find(str)) rehash(strdup(str));
    }

    else{ //tries to insert in old hash table
        int hash = hashFunct(hashCode(str), m_capOld);
        int p = 0; //probe counter
        int i = 0; //the index currently being accessed
        //probe the hash for an empty spot
        while(p < MAX_PROBE) {
            i = (hash + p)%m_capOld; //reset hashing index accounting for probe
            //do nothing str is already there
            if(strcomp(str, m_oldHash[i])) break;
            //insert if theres an empty spot
            else if(m_oldHash[i] == NULL || m_oldHash[i] == DELETED) {
                m_oldHash[i] = strdup(str);
                m_sizeOld++;
                break;
            }
            p++; //increment linear probe count
        }

        if(p >= MAX_PROBE) { //if there was more than 10 probes start rehash
            m_rehashMode = true;
            secondHashInit();
            rehash(strdup(str));
            rehashCluster(hash);
        }
    }
}

/*  HashTable Find Operation:
    -triggers incremental rehashing if max load factor is reached or 
     max probe length is reached in old hash table
    -attempts to wrap up the rehashing sequence at the beginning
    -will rehash any cluster that is "touched" whenever incremental
     rehashing is triggered before return statement
    -could give up on incremental rehashing if max probe length is reached
    when probing for str in the second hash. This causes rehashThirdTable()
    to be called.
*/
bool HashTable::find(const char *str) {
    //attempts to move everything to the new hash and make it the old
    wrapUpRehash();

    //if load factor goes over max turn on increamental rehashing
    if(loadFactor() > MAX_LOAD) {
        m_rehashMode = true;
        secondHashInit();
    }

    int hash = hashFunct(hashCode(str), m_capOld);
    int p = 0;
    int i = (hash + p)%m_capOld;


    //probe the hash for the str
    while(p < MAX_PROBE) {
        i = (hash + p)%m_capOld; //reset hashing index accounting for probe
        //str was not found in old hash because probe hit a NULL
        if(m_oldHash[i] == NULL) {
            //if not rehashing then str is not in data structure
            if(!m_rehashMode) return false;
            break; //otherwise break out and check new hash table
        }
        //str was found
        if(strcomp(str, m_oldHash[i])) return true;
        p++; //increment linear probe count
    }

    //if incremental rehashing is triggered then the loop is "paused"
    //and the second hash is immediately initialized
    if(!m_rehashMode) { //if this evaluates to true then p must be >= 10
        m_rehashMode = true;
        secondHashInit();
    }

    i = (hash + p)%m_capOld;
    // continue checking the rest of the cluster
    while(m_oldHash[i] != NULL && i != hash) {
        //if the string is found rehash and return true
        if(strcomp(str, m_oldHash[i])) {
            rehashCluster(hash);
            return true;
        }
        p++; //update probe
        i = (hash + p)%m_capOld;
    }//if this loop ends before true is returned then str is 100% not in old hash

    if(!size(1)) {//if size of new hash == 0
        rehashCluster(hash);
        return false; //if the 2nd table is empty then return false
    }
    //otherwise search new hash table for str
    int oldHash = hash;
    hash = hashFunct(hashCode(str), m_capNew);
    p = 0;
    i = hash;

    //loop while the probe isnt null and it hasnt wrapped back around to itself
    while(m_newHash[i] != NULL && (i != hash || p == 0)) {
        //if the string is found rehash and return true
        if(strcomp(str, m_newHash[i])) {
            //if theres a long probe giveup on incremental rehashing
            if(p >= MAX_PROBE) rehashThirdTable();
            //otherwise we want to rehash the old cluster we found
            else rehashCluster(oldHash);
            return true;
        }

        p++; //update probe
        i = (hash + p)%m_capNew;
    } //was not found in any of the hash tables after completely probing each.

    //if the second hash table reached a max probe give up on incremental rehashing
    if(p >= MAX_PROBE) rehashThirdTable();
    //otherwise rehash the cluster that was "touched" in old hash table.
    else rehashCluster(oldHash);
    return false;
}

char * HashTable::remove(const char *str) {
    //attempts to move everything to the new hash and make it the old
    wrapUpRehash();

    //if load factor goes over max turn on increamental rehashing
    if(loadFactor() > MAX_LOAD) {
        m_rehashMode = true;
        secondHashInit();
    }

    int hash = hashFunct(hashCode(str), m_capOld);
    int p = 0;
    int i = (hash + p)%m_capOld;
    char * ret;


    //probe the hash for the str
    while(p < MAX_PROBE) {
        i = (hash + p)%m_capOld; //reset hashing index accounting for probe
        //str was not found in old hash because probe hit a NULL
        if(m_oldHash[i] == NULL) {
            //if not rehashing then str is not in data structure
            if(!m_rehashMode) return NULL;
            break; //otherwise break out and check new hash table
        }
        //str was found
        if(strcomp(str, m_oldHash[i])) {
            ret = m_oldHash[i];
            m_oldHash[i] = DELETED;
            return ret;
        }
        p++; //increment linear probe count
    }

    //if incremental rehashing is triggered then the loop is "paused"
    //and the second hash is immediately initialized
    if(!m_rehashMode) { //if this evaluates to true then p must be >= 10
        m_rehashMode = true;
        secondHashInit();
    }

    i = (hash + p)%m_capOld;
    // continue checking the rest of the cluster
    while(m_oldHash[i] != NULL && i != hash) {
        //if the string is found rehash and return true
        if(strcomp(str, m_oldHash[i])) {
            rehashCluster(hash);
            ret = m_oldHash[i];
            m_oldHash[i] = DELETED;
            return ret;
        }
        p++; //update probe
        i = (hash + p)%m_capOld;
    }//if this loop ends before true is returned then str is 100% not in old hash

    if(!size(1)) {//if size of new hash == 0
        rehashCluster(hash);
        return NULL; //if the 2nd table is empty then return false
    }
    //otherwise search new hash table for str
    int oldHash = hash;
    hash = hashFunct(hashCode(str), m_capNew);
    p = 0;
    i = hash;

    //loop while the probe isnt null and it hasnt wrapped back around to itself
    while(m_newHash[i] != NULL && (i != hash || p == 0)) {
        //if the string is found rehash and return true
        if(strcomp(str, m_newHash[i])) {
            //if theres a long probe giveup on incremental rehashing
            if(p >= MAX_PROBE) rehashThirdTable();
            //otherwise we want to rehash the old cluster we found
            else rehashCluster(oldHash);
            ret = m_newHash[i];
            m_newHash[i] = DELETED;
            return ret;
        }

        p++; //update probe
        i = (hash + p)%m_capNew;
    } //was not found in any of the hash tables after completely probing each.

    //if the second hash table reached a max probe give up on incremental rehashing
    if(p >= MAX_PROBE) rehashThirdTable();
    //otherwise rehash the cluster that was "touched" in old hash table.
    else rehashCluster(oldHash);
    return NULL;
}

void HashTable::dump() {
    cout << "-----HASH ONE-----" << endl;
    int hash = 0;
    int j;

    cout << "Capcity: " << m_capOld << endl;
    cout << "Size: " << m_sizeOld << endl;
    cout << "Load Factor: " << loadFactor() << endl;

    for(int i=0; i<m_capOld; i++) {
        cout << "H1[" << i << "] = ";
        if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED) {
            cout << "\"";
            hash = hashFunct(hashCode(m_oldHash[i]), m_capOld);
            j = 0;
            while(m_oldHash[i][j] != '\0') {
                cout << m_oldHash[i][j];
                j++;
            }
            cout << "\" (" << hash << ")";
        }
        cout << endl;
    }

    if(m_rehashMode) {
        cout << "\n-----HASH TWO-----" << endl;
        cout << "Capcity: " << m_capNew << endl;
        cout << "Size: " << m_sizeNew << endl;
        cout << "Load Factor: " << loadFactor(1) << endl;

        for(int i=0; i<m_capNew; i++) {
            cout << "H2[" << i << "] = ";
            if(m_newHash[i] != NULL && m_newHash[i] != DELETED) {
                cout << "\"";
                hash = hashFunct(hashCode(m_newHash[i]), m_capNew);
                j = 0;
                while(m_newHash[i][j] != '\0') {
                    cout << m_newHash[i][j];
                    j++;
                }
                cout << "\" (" << hash << ")";
            }
            cout << endl;
        }   
    }
}

//--------REQUIRED GRADING FUNCTIONS--------

bool HashTable::isRehashing() {
    return m_rehashMode;
}

int HashTable::tableSize(int table) {
    if(table) return m_capNew;
    else return m_capOld;
}

int HashTable::size(int table) {
    if(table) return m_sizeNew;
    else return m_sizeOld;
}

const char * HashTable::at(int index, int table) {
    if(table) {
        if(!m_rehashMode) return NULL;
        else if(index < 0 || index > m_capNew)
            throw out_of_range("The index requested is out of range.");
        else return m_newHash[index];
    }
    else {
        if(index < 0 || index > m_capOld)
            throw out_of_range("The index requested is out of range.");
        else return m_oldHash[index];
    }
}

//--------OTHER MEMBER FUNCTIONS--------

//PUBLIC

//Returns the total number of items in the hash table
int HashTable::totalItems() {
    return m_sizeOld + size(1);
}

//PRIVATE
/*  Initializer for the Secondary Hash Table
    -capacity = the closest prime to 4*m_sizeOld
    -if the new capacity is = to the oldCapacity then roundup to next prime
    -if the required capacity is > max table size throws an exception
    -allocates array of char pointers with size of the new capacity
    -initializes everything to NULL
*/
void HashTable::secondHashInit() {
    int capacity = roundUpPrime(m_sizeOld*4);
    //if the new hash table capacity = the old capcaity: round up to next prime
    if(capacity == m_capOld) capacity = roundUpPrime(capacity++);
    //if the capcity is ever returned as 0 then its > MAX table size
    if(!capacity)
        throw out_of_range("The required size of the secondary hash table is too large.\n");

    m_capNew = capacity;
    m_newHash = new char *[m_capNew];

    //initialize everything to NULL
    for(int i=0; i < m_capNew; i++) m_newHash[i] = NULL;
}

/*  Rehash Operation:
    -called whenever something needs to be inserted into the new hash table
    -a pointer to the dynamically allocated c string is passed in
    -if the str already exists in the table the pointer gets freed
    -will give up on incremental rehashing if max load factor is reached or if
     max probe length is reached
    -probes until it finds a NULL space, a DELETED space, a copy of the str, or
     if the max probe length is reached
*/
void HashTable::rehash(char *str) {
    if(loadFactor(1) > MAX_LOAD)
        rehashThirdTable(str);
    else {
        int hash = hashFunct(hashCode(str), m_capNew);
        int p = 0;
        int i = hash;

        //while the max probe length has not been reached
        while(p < MAX_PROBE) {
            i = (hash + p)%m_capNew; //probed index accounting for wrap around
            //checks to make sure the str is not already in the new hash
            if(strcomp(str, m_newHash[i])) {
                free(str); //frees if it is (passed by a function looking at oldHash)
                break;
            }
            //if the spot is empty place it in the new hash
            else if(m_newHash[i] == NULL || m_newHash[i] == DELETED) {
                m_newHash[i] = str;
                m_sizeOld--;
                m_sizeNew++; //update size of new table
                break; //break out of loop
            }
            p++; //update probe length
        }

        //if the max probe is triggered then give up on incremental rehashing
        if(p >= MAX_PROBE)
            rehashThirdTable(str);
    }
}

/*  Rehash Cluster:
    -takes in an index that a cluster exists at
    -probes forward in the cluster and rehashes all non deleted data accounting
     for wrap around until it hits a NULL
    -then probes backward and rehashes all non deleted data accounting for warparound
    -calls the rehash() function    
*/
void HashTable::rehashCluster(int index) {
    int i = index;
    //moves forward in the cluster and rehashes until it hits NULL
    while(m_oldHash[i] != NULL) {
        if(m_oldHash[i] != DELETED) rehash(m_oldHash[i]);
        m_oldHash[i] = NULL;
        //update index accounting for wrap around
        i++;
        i %= m_capOld;
    }
    //if index is 0 then loop around to end to check backward in the cluster
    i = (!index) ? m_capOld-1 : (index-1);
    //moves backward in the cluster and rehashes until it hits NULL
    while(m_oldHash[i] != NULL) {
        if(m_oldHash[i] != DELETED) rehash(m_oldHash[i]);
        m_oldHash[i] = NULL;

        i--; //decrement index (going backwards)
        i = (i<0) ? m_capOld-1 : i; //if it's less than zero, wrap around
    }
}

/*  WrapUpRehash():
    -called at the beginnng of every insert, find, and remove operation
    -checks to see if the old hash hold less than 3% of the data
    -if so it goes through every element in the array and moves it to the newHash
    -then deallocates oldHash pointer array of char pointers
    -sets the old hash table pointer to the new hashtable pointer and sets
     the new hash table pointer to NULL.
    -turns off rehashMode
*/
void HashTable::wrapUpRehash() {
    //will evaluate to true if total items != 0 and the percentange of items
    //in the first hash table is less than the wrap up percentange
    if((totalItems() && (double)size()/totalItems() < WRAP_UP_PERCENT) || cpyFlag) {
        //probe entire table and rehash the last strings to the new hashtable
        for(int i=0; i<m_capOld; i++) {
            if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED) {
                rehash(m_oldHash[i]);
                m_sizeOld--;
                if(m_sizeOld <= 0) break;
            }
            
        }

        //after, delete the allocated array from the old hash and set the old hash
        //table pointer to the new hash table pointer, set newHash to NULL
        delete[] m_oldHash;
        m_oldHash = m_newHash;
        m_newHash = NULL;
        //End rehash mode
        m_rehashMode = false;
        m_sizeOld = m_sizeNew;
        m_sizeNew = 0;
        m_capOld = m_capNew;
        m_capNew = 0;
    }
}
/*  Rehashes Everything to a 3rd Table:
    -only called if we give up on incremental rehashing
     because max load or max probe is reached on 2nd hash
    -rehashes everything from both tables into a table about 4*total size
    -deallocates duplicate strs (can occur when called by a rehashCluster)
    -deallocates older newHash and oldHash arrays
    -sets oldHash pointer = to thirdHash
    -turns off rehashMode
*/
void HashTable::rehashThirdTable() {
    char ** thirdHash;
    int capacity = roundUpPrime(4*totalItems());
    int size = 0;

    if(!capacity) 
        throw out_of_range("The required size of the new hash table is too large.\n");
    
    //allocate memory and initialize to NULL
    thirdHash = new char *[capacity];
    for(int i=0; i<capacity; i++) thirdHash[i] = NULL;

    int hash = 0;
    int p;
    int j;
    for(int i=0; i<m_capOld; i++) { //rehashes everything in the old Hash table
        if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED) {
            hash = hashFunct(hashCode(m_oldHash[i]), capacity);
            j = hash;
            p = 0;

            //linear probe a NULL spot is found
            while(thirdHash[j] != NULL) {
                p++;
                j = (hash + p)%capacity;
            }
            //places pointer to cstring in thirdHash table and incr. size of 3rd table
            thirdHash[j] = m_oldHash[i];
            size++;
            m_sizeOld--; //decrements size of oldHash

            if(m_sizeOld <= 0) break; //terminates rehasing of oldTable
        }
    }

    //deallocate oldHash (items do not need to be deallocated because they are now in
    //the thirdHash array)
    delete[] m_oldHash;
    m_oldHash = NULL;

    //rehash everything in new table
    for(int i=0; i<m_capNew; i++) {
        //if a str exists at this spot start rehashing
        if(m_newHash[i] != NULL && m_newHash[i] != DELETED) {
            hash = hashFunct(hashCode(m_newHash[i]), capacity);
            j = hash;
            p = 0;

            //probe until a null space is found or a duplicate
            while(thirdHash[j] != NULL) {
                //duplicates can arise when reclustering calls this funct
                if(strcomp(thirdHash[j], m_newHash[i])) {
                    //free duplicate
                    free(m_newHash[i]);
                    m_newHash[i] = NULL;
                    break;
                }
                //update linear probe
                p++;
                j = (hash + p)%capacity;
            }
            //if there wasnt a duplicate then place cstring pointer into thirdHash
            if(m_newHash[i] != NULL) {
                thirdHash[j] = m_newHash[i];
                size++; //increment size of thirdHash
            }
            m_sizeNew--;//decrement size of m_newHash everytime an item is moved

            if(m_sizeNew <= 0) break; //terminate if the size reaches 0
        }
    }//rehashing of both oldHash table and newHash table is complete

    //deallocate the newHash array and set to NULL
    delete[] m_newHash;
    m_newHash = NULL;

    //reset new capacity and sizes, set old capacity to 3rd table's capacity and
    //set old size to 3rd table's size
    m_capNew = 0;
    m_sizeNew = 0;
    m_capOld = capacity;
    m_sizeOld = size;
    //make the old hash point to the 3rd hashes memory location
    m_oldHash = thirdHash;

    m_rehashMode = false; //turn off rehashMode
}

/*  Rehashes Everything to a 3rd Table:
    -overloaded version with first hashed string
    -called when we give up on incremental rehashing
     because max load or max probe is reached on 2nd hash
    -rehashes everything from both tables into a table about 4*total size
    -deallocates duplicate strs (can occur when called by a rehashCluster)
    -deallocates older newHash and oldHash arrays
    -sets oldHash pointer = to thirdHash
    -turns off rehashMode
*/
void HashTable::rehashThirdTable(char *str) {
    char ** thirdHash;
    int capacity = roundUpPrime(4*totalItems());
    int size = 0;

    if(!capacity) 
        throw out_of_range("The required size of the new hash table is too large.\n");
    
    //allocate memory and initialize to NULL
    thirdHash = new char *[capacity];
    for(int i=0; i<capacity; i++) thirdHash[i] = NULL;

    int hash = 0;
    int p;
    int j;

    int firstHash = hashFunct(hashCode(str), capacity);
    //hashes the first string
    thirdHash[firstHash] = str; //(cant have any collision!)
    size++;

    for(int i=0; i<m_capOld; i++) { //rehashes everything in the old Hash table
        if(m_oldHash[i] != NULL && m_oldHash[i] != DELETED) {
            hash = hashFunct(hashCode(m_oldHash[i]), capacity);
            j = hash;
            p = 0;

            //checks hash first so strcmp isnt called as often
            if(hash == firstHash && strcomp(str, m_oldHash[i])) {
                //frees this str if a duplicate with the first str is found
                free(m_oldHash[i]);
                m_oldHash[i] = NULL;
            }
            else {//otherwise it is probed
                //linear probe a NULL spot is found
                while(thirdHash[j] != NULL) {
                    p++;
                    j = (hash + p)%capacity;
                }
                //places pointer to cstring in thirdHash table and incr. size of 3rd table
                thirdHash[j] = m_oldHash[i];
                size++;
            }
            m_sizeOld--; //decrements size of oldHash
            if(m_sizeOld <= 0) break; //terminates rehasing of oldTable
        }
    }

    //deallocate oldHash (items do not need to be deallocated because they are now in
    //the thirdHash array)
    delete[] m_oldHash;
    m_oldHash = NULL;

    //rehash everything in new table
    for(int i=0; i<m_capNew; i++) {
        //if a str exists at this spot start rehashing
        if(m_newHash[i] != NULL && m_newHash[i] != DELETED) {
            hash = hashFunct(hashCode(m_newHash[i]), capacity);
            j = hash;
            p = 0;

            //probe until a null space is found or a duplicate
            while(thirdHash[j] != NULL) {
                //duplicates can arise when reclustering calls this funct
                if(strcomp(thirdHash[j], m_newHash[i])) {
                    //free duplicate
                    free(m_newHash[i]);
                    m_newHash[i] = NULL;
                    break;
                }
                //update linear probe
                p++;
                j = (hash + p)%capacity;
            }
            //if there wasnt a duplicate then place cstring pointer into thirdHash
            if(m_newHash[i] != NULL) {
                thirdHash[j] = m_newHash[i];
                size++; //increment size of thirdHash
            }
            m_sizeNew--;//decrement size of m_newHash everytime an item is moved

            if(m_sizeNew <= 0) break; //terminate if the size reaches 0
        }
    }//rehashing of both oldHash table and newHash table is complete

    //deallocate the newHash array and set to NULL
    delete[] m_newHash;
    m_newHash = NULL;

    //reset new capacity and sizes, set old capacity to 3rd table's capacity and
    //set old size to 3rd table's size
    m_capNew = 0;
    m_sizeNew = 0;
    m_capOld = capacity;
    m_sizeOld = size;
    //make the old hash point to the 3rd hashes memory location
    m_oldHash = thirdHash;

    m_rehashMode = false; //turn off rehashMode
}

//returns the hashcode of a c-string
//this function's implementation was given
unsigned int HashTable::hashCode(const char *str) {
    unsigned int val = 0 ;
    const unsigned int thirtyThree = 33 ;  // magic number from textbook

    int i = 0 ;
    while (str[i] != '\0') {
        val = val * thirtyThree + str[i] ;
        i++ ;
    }
    return val ;
}

//The hashfunction calculation that maps the hash code to the index returned
//using the modulus of the capacity of the table.
int HashTable::hashFunct(unsigned int hc, int hashCap) {
    return hc % hashCap;
}

/*  returns the load factor (LF) of the desired table
    -table = 0 means the LF of the old table will be returned
    -table >= 1 means the LF of the new table will be returned
    -if the HashTable isnt in rehash mode and table >= 1 then the old table's
     LF is returned
*/
double HashTable::loadFactor(int table) {
    if(table) {
        if(m_capNew == 0) return 0;
        return (double)m_sizeNew/m_capNew;
    }
    //returns current LF of old hash table
    else return (double)m_sizeOld/m_capOld;
}

/*  String Compare:
    -calls c function strcmp()
    -handles if NULL or DELETED are accidentally passed in because their behavior
     is undefined for strcmp()
    -nots strcmp because it returns the difference between the 2 strings
*/
bool HashTable::strcomp(const char *a, const char *b) {
    if(a == NULL || a == DELETED || b == NULL || b == DELETED) return false;
    else return !strcmp(a,b);
}