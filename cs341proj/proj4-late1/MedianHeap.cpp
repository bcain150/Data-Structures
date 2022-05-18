//MedianHeap.cpp
//Made by Brendan Cain
// 11/20/18
//Implementations for the MedianHeap class

#include "MedianHeap.h"

template<class T>
MedianHeap<T>::MedianHeap(bool (*lt)(const T&, const T&), bool (*gt)(const T&, const T&), int cap=100) {
    if(cap%2 != 0) cap++; //must be even, makes things more simple 

    m_min = Heap<T>(lt, cap/2);
    m_max = Heap<T>(gt, cap/2);

    int m_capacity = cap;
    int m_size = 0;
}

template<class T>
void MedianHeap<T>::insert(const T& item) {
    if(m_size >= m_capacity)
        throw out_of_range("The Median Heap cannot hold anymore data.");
 
    T temp;
    if(m_size < 1) {
        m_min.insert(item);
        m_size++;
        m_medianEntry = item;
        m_maxEntry = item;
        m_minEntry = item;
    }
    else if(item < m_medianEntry) {
        m_max.insert(item);
        if((m_max.size() - m_min.size()) > 1) {
            temp = m_max.remove(1);
            m_min.insert(temp);
        }
    }
    else {
        m_min.insert(item);
        if((m_min.size() - m_max.size()) > 1) {
            temp = m_min.remove(1);
            m_max.insert(temp);
        }
    }

    m_size++; //increase size

    //updating m_maxEntry / m_minEntry
    if(m_maxEntry < item) m_maxEntry = item;
    if(m_minEntry > item) m_minEntry = item;

    //updating m_medianEntry
    if(m_min.size() == m_max.size()) { //if both heaps are the same size
        //then the median is the average of the two roots
        m_medianEntry = (m_min.get(1) + m_max.get(1)) /2;
    }
    else if(m_min.size() < m_max.size()) //if the max heap is bigger
        m_medianEntry = m_max.get(1); //then the max root is the median
    else
        m_medianEntry = m_min.get(1); //the min root is the median
}

template <class T>
int MedianHeap<T>::size() {
    return m_size;
}

template <class T>
int MedianHeap<T>::capacity() {
    return m_capacity;
}

template <class T>
T MedianHeap<T>::getMedian() {
    return m_medianEntry;
}

template <class T>
T MedianHeap<T>::getMax() {
    return m_maxEntry;
}

template <class T>
T MedianHeap<T>::getMin() {
    return m_minEntry;
}

template <class T>
void MedianHeap<T>::dump() {

    cout << "Median: " << m_medianEntry << endl;
    cout << "Max: " << m_maxEntry << endl;
    cout << "Min: " << m_minEntry << endl;

    cout << "\nMin Heap:" << endl;
    m_min.dump();

    cout << "\nMax Heap:" << endl;
    m_max.dump();
}


template <class T>
bool MedianHeap<T>::equal(const T &x, const T &y) {
    return x == y;
}