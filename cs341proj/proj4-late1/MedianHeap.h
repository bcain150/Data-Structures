//MedianHeap.h
//Header File Made by Brendan Cain
//11/20/18
//definitions for the MedianHeap class

#ifndef MEDIAN_HEAP_H
#define MEDIAN_HEAP_H

#include "Heap.h"
#include "Heap.cpp"

template <class T>
class MedianHeap {
    public:

        MedianHeap(bool (*lt)(const T&, const T&), bool (*gt)(const T&, const T&), int cap=100);

        MedianHeap(const MedianHeap<T>& otherH);

        ~MedianHeap();

        const MedianHeap<T> operator=(const MedianHeap<T> &rhs);

        int size();

        int capacity();

        void insert(const T& item);

        T getMedian();

        T getMin();
        
        T getMax();

        bool deleteItem(T& givenItem, bool (*equalTo)(const T&, const T&));

        void dump();

        //grading functions
        int maxHeapSize();

        int minHeapSize();

        T locateInMaxHeap(int pos);
        
        T locateInMinHeap(int pos);

        bool equal(const T&, const T&);        
    private:
    
        Heap<T> m_min;
        Heap<T> m_max;

        T m_maxEntry;
        T m_minEntry;
        T m_medianEntry;
        
        int m_capacity;
        int m_size;

};

#endif
