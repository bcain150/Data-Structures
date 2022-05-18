//Heap.h
//Header File Made by Brendan Cain
//11/20/18
//definitions for theHeap class

#ifndef HEAP_H
#define HEAP_H

#include <stdexcept>
#include <iostream>

using namespace std;

template <class T>
class Heap {

    public:

        Heap(bool (*cmp)(const T&, const T&), int cap);

        Heap(const Heap<T>& other);

        const Heap<T> operator=(const Heap<T> &rhs);

        ~Heap();

        void insert(const T& item);
        
        bool remove(const T& item, bool (*equalTo)(const T&, const T&));

        T remove(int index);

        int search(const T& item, bool (*equalTo)(const T&, const T&));

        T get(int index);

        int size();

        bool isFull();

        int capacity();

        void dump();

    private:
        int m_size;
        int m_capacity;
        T * m_heapArray;
        bool (*m_compare)(const T&, const T&);
        int m_curr;
};

#endif
