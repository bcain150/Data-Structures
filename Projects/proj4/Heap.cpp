//Heap.cpp
//Made by Brendan Cain
// 11/20/18
//Implementations for the Heap class

#include "Heap.h"

template <class T>
Heap<T>::Heap(bool (*cmp)(const T&, const T&), int cap) {
    m_compare = cmp;
    m_capacity = cap;
    m_heapArray = new T[cap];
    m_curr = 1;
    m_size = 0;
}

template <class T>
Heap<T>::Heap(const Heap<T>& other) {
    m_size = other.m_size;
    m_capacity = other.m_capacity;
    m_curr = other.m_curr;
    m_compare = other.m_compare;

    m_heapArray = new T[m_capacity];
    for(int i=1; i<other.m_size+1; i++)
        m_heapArray[i] = other.m_heapArray[i];
}

template <class T>
const Heap<T> Heap<T>::operator=(const Heap<T> &rhs) {
    if(m_heapArray != NULL)
        delete [] m_heapArray;
    
    m_heapArray = new T[rhs.m_capacity];

    m_size = rhs.size;
    m_capacity = rhs.m_capacity;
    m_compare = rhs.m_compare;
    m_curr = rhs.m_curr;

    for(int i=1; i<m_size+1; i++) m_heapArray[i] = rhs.m_heapArray[i];
}

template <class T>
Heap<T>::~Heap() {
    delete [] m_heapArray;
    m_heapArray = NULL;
}

template <class T>
void Heap<T>::insert(const T& item) {
    if(!isFull()) {
        m_heapArray[m_curr] = item;

        int i = m_curr;
        while(i > 1 && m_compare(m_heapArray[i], m_heapArray[i/2])) {
            m_heapArray[i] = m_heapArray[i/2];
            m_heapArray[i/2] = item;
            i /=2;
        }
        m_curr++;
        m_size++;
    }
    else throw overflow_error("The Heap is FULL! The item cannot be added");
}

template <class T>
int Heap<T>::search(const T& item, bool (*equalTo)(const T&, const T&)) {
    for(int i=1; i<m_size+1; i++) {
        if(equalTo(item, m_heapArray[i])) return i;
    }
    return 0;
}

template <class T>
bool Heap<T>::remove(const T& item, bool (*equalTo)(const T&, const T&)) {
    int index = 0;
    for(int i=1; i<m_size+1; i++) {
        if(equalTo(item, m_heapArray[i])) {
            index = i;
            break;
        }
    }
    if(index == 0) return false;

    m_curr--;
    m_size--;

    T temp;

    m_heapArray[index] = m_heapArray[m_curr];
    while(index*2 <= m_size && !m_compare(m_heapArray[index], m_heapArray[index*2])){
        if(m_compare(m_heapArray[index*2], m_heapArray[index*2 +1]) || m_size <= (index*2)) {
            temp = m_heapArray[index*2];
            m_heapArray[index*2] = m_heapArray[index];
            m_heapArray[index] = temp;
            index = index*2;
        }
        else {
            temp = m_heapArray[index*2 + 1];
            m_heapArray[index*2 + 1] = m_heapArray[index];
            m_heapArray[index] = temp;
            index = index*2 + 1;
        }
    }
    return true;
}

template <class T>
T Heap<T>::remove(int index) {
    if(index > m_size || index < 1) 
        throw out_of_range("The index requested does not exist");

    m_curr--;
    m_size--;

    T temp;
    T ret = m_heapArray[index];

    m_heapArray[index] = m_heapArray[m_curr];
    while(index*2 <= m_size && !m_compare(m_heapArray[index], m_heapArray[index*2])){
        if(m_compare(m_heapArray[index*2], m_heapArray[index*2 +1]) || m_size <= (index*2)) {
            temp = m_heapArray[index*2];
            m_heapArray[index*2] = m_heapArray[index];
            m_heapArray[index] = temp;
            index = index*2;
        }
        else {
            temp = m_heapArray[index*2 + 1];
            m_heapArray[index*2 + 1] = m_heapArray[index];
            m_heapArray[index] = temp;
            index = index*2 + 1;
        }
    }
    return ret;
}

template <class T>
void Heap<T>::dump() {
    for(int i=1; i<m_size+1; i++)
        cout << i << ": " << m_heapArray[i] << endl;
}

template<class T>
bool Heap<T>::isFull() {
    return m_size >= m_capacity;
}

template<class T>
int Heap<T>::capacity() {
    return m_capacity;
}

template<class T>
int Heap<T>::size() {
    return m_size;
}

template<class T>
T Heap<T>::get(int index) {
    if(index > m_size || index < 1) 
        throw out_of_range("The index requested does not exist");
    
    return m_heapArray[index];
}

