/*  File Name: HashTable.h
    Created By: Brendan Cain
    Date: 12-07-18 5:00pm
    Description: 
    header file that includes the protoytpes for HashTable.cpp
*/
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "primes.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace std;

#define MIN_TABLE_SIZE 101
#define MAX_TABLE_SIZE 199999
#define MAX_PROBE 10 //max amount of linear probes before rehashing occurs
#define MAX_LOAD 0.5 //max load factor before rehashing occurs
#define WRAP_UP_PERCENT 0.03 /*minimum percentage of items stored
in the old table before the rest of the items are rehashed*/

class HashTable {

    //sentinel value for whenever an item is deleted
    //needed in order for linear probing to work properly
    static char * const DELETED;

    public:

        HashTable(int n=MIN_TABLE_SIZE);

        ~HashTable();

        HashTable(HashTable& other);

        const HashTable& operator=(HashTable rhs);

        void insert(const char *str);

        bool find(const char *str);

        char * remove(const char *str);

        void dump();

        int totalItems();

        void wrapUpRehash();

        //grading functions
        bool isRehashing();
        int tableSize(int table=0);
        int size(int table=0);
        const char * at(int index, int table=0);

    private:

        void secondHashInit();

        void rehash(char *str);

        void rehashCluster(int index);

        void rehashThirdTable();

        void rehashThirdTable(char *str);

        unsigned int hashCode(const char *str);

        int hashFunct(unsigned int hc, int hashCap);

        double loadFactor(int table=0);

        bool strcomp(const char *a, const char*b);

        char ** m_oldHash;
        int m_sizeOld;
        int m_capOld;

        char ** m_newHash;
        int m_sizeNew;
        int m_capNew;

        bool m_rehashMode;
        bool cpyFlag;
};

#endif