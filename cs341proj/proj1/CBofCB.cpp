//Proj 1: Circular Buffer of Circular Buffers Class Implementation
//By: Brendan Cain
//CMSC 341

#include "CBofCB.h"
#include <stdexcept>
#include <iostream>

using namespace std;


/*  This is the default constructor for the CBofCB class. 
    It should initialize all of the data members and set up a single 
    inner circular buffer that holds 10 int values. 
    Note that m_buffers is an array of pointers to InnerCB. 
    It is NOT a dynamically allocated array. You do need to initialize 
    the array of pointers to NULL.
*/
CBofCB::CBofCB() {
    //allocates space for the first InnerCB with 10 int vals (default)
    m_buffers[0] = new InnerCB();
    //sets the rest of the pointers in the array to null
    for(int i=1; i<m_obCapacity; i++) m_buffers[i] = NULL;

    //sets other member vars
    m_obSize = 1;
    m_oldest = 0;
    m_newest = 0;
}

/*  This is the copy constructor for the CBofCB class. You have to make 
    a complete copy of the other object. Make good use of the InnerCB 
    copy constructor. The admonition to not use the CBofCB assignment operator
    to implement this copy constructor also holds here.
*/
CBofCB::CBofCB(const CBofCB& other) {
    //initialize mem variables
    m_obSize = other.m_obSize;
    m_oldest = other.m_oldest;
    m_newest = other.m_newest;

    //for loop that allocates new memory for each InnerCB and orients them
    //with the start at index 0 of the new outer buffer.
    for(int i=0; i<m_obCapacity; i++) {
        if(other.m_buffers[i] != NULL) m_buffers[i] = new InnerCB(*other.m_buffers[i]);
        else m_buffers[i] = NULL;
    }
}

/*  This is the destructor. You must deallocate space used by the InnerCB 
    objects that the array of pointers in m_buffers points to. 
    Remember that you don't do that by calling the InnerCB destructor explicitly, 
    because you never call destructors explicitly.
*/
CBofCB::~CBofCB() {
    int index = 0; //temp variable
     //loops through outer buffer, deletes data and sets pointers to null
    for(int i=0; i<m_obSize; i++) {
        index = (m_oldest+i) % m_obCapacity;
        delete m_buffers[index];
        m_buffers[index] = NULL;
    }
}

/*  This member function adds the value in the data parameter to the 
    data structure. Recall from the discussion above that you always add to the 
    newest InnerCB. You may have to create a new InnerCB if the current newest
    InnerCB is full. This new newest array should have twice the capacity of the
    previous newest array. If this cannot be done because the outer circular 
    buffer is also full, then just throw an overflow_error exception.
*/
void CBofCB::enqueue(int data) {
    
    //check to see if the data structure is full.
    if(isFull()) 
        throw overflow_error("There is no more space in the data structure!");

    try { //try to enqueue data into the newest InnerCB
        m_buffers[m_newest]->enqueue(data);
    }catch(exception e) { //catch it by creating a new InnerCB
        //saves the size of the previous InnerCB and updates the newest index
        int prevCap = m_buffers[m_newest]->capacity();
        m_newest = (m_newest+1) % m_obCapacity;

        //allocates memory for the newest InnerCB obj at the newest index
        //with double the capacity that the previous InnerCB obj had
        m_buffers[m_newest] = new InnerCB(prevCap*2);
        
        //update outer buffer size and enqueue data into newest InnerCB
        m_obSize++;
        m_buffers[m_newest]->enqueue(data);
    }
}

//  remove item from this data structure
int CBofCB::dequeue() {
    int retVal; //return value that has been dequeued
    //if there is nothing to dequeue then throw an exception
    if(isEmpty())
        throw underflow_error("There is no data in the queue!");
    
    //update return value before attempting to change m_oldest
    retVal = m_buffers[m_oldest]->dequeue();
    //check to see if that was the last entry in the oldest InnerCB
    //but not the last entry in the data structure.
    if(m_buffers[m_oldest]->isEmpty() && !isEmpty()) {
        delete m_buffers[m_oldest]; //delete the old one and set it to NULL
        m_buffers[m_oldest] = NULL;
        m_obSize--; //decrement the size and move the oldest InnerCB index up
        m_oldest = (m_oldest+1) % m_obCapacity;
    }
    return retVal; // return the dequeue
}

/*  Returns true if it is not possible to add any more items to the 
    data structure. This would occur if every entry of m_buffers is already 
    pointing to an InnerCB and the newest InnerCB is full. 
    (This also implies that all except the oldest InnerCB are full, too.)
*/
bool CBofCB::isFull() {
    return (m_obSize >= m_obCapacity && m_buffers[m_newest]->isFull());
}

//  Returns true if there are no items stored in the data structure anywhere.
bool CBofCB::isEmpty() {
    return (m_obSize == 1 && m_buffers[m_newest]->isEmpty());
}

/*  Returns the number of items stored in the data structure. 
    Note that this is not the number of InnerCB objects that you have
    pointed to by pointers in the m_buffers array. It is the total number 
    of int values you have stored in all of the InnerCB objects.
*/
int CBofCB::size() {
    int s=0; //total number of entries in the entire data structure

    //updates the total size by getting the size of each InnerCB
    for(int i=0; i<m_obSize; i++)
        s += m_buffers[(m_oldest+i)%m_obCapacity]->size();
    
    return s;    
}

/*  This is the overloaded assignment operator for the CBofCB class. 
    Read the reminders about assignment operators written in the specifications 
    of the assignment operator for the InnerCB class. They also apply here.
    Before the assignment, the left hand side (LHS) and 
    the right hand side (RHS) might not have pointers to InnerCBs of the same 
    capacity. However, after the assignment, the LHS should be an exact
    duplicate of the RHS. They should have the same value for m_obSize,
    each allocated InnerCB should have the same capacity and be referenced by
    the same location in m_buffers, and each InnerCB on the RHS should 
    have an exact duplicate on the LHS.
*/
const CBofCB& CBofCB::operator=(const CBofCB& rhs) {
    if(this != &rhs) { //checks for self assignment
        //sets member variables
        m_obSize = rhs.m_obSize;
        m_oldest = rhs.m_oldest;
        m_newest = rhs.m_newest;
        // loops through right side and sets left side
        for(int i=0; i<m_obCapacity; i++) {
            //if the right side is NULL and the left side isnt, mem is freed

            if(rhs.m_buffers[i] == NULL && m_buffers[i] != NULL)
                delete m_buffers[i];
            //if the the right isnt null but the left is, mem is created
            else if(rhs.m_buffers[i] != NULL && m_buffers[i] == NULL)
                m_buffers[i] = new InnerCB(rhs.m_buffers[i]->capacity());
            
            //1-the 1st statement occurs and the deleted pointer is set to NULL
            //2-the 2nd statement occurs and InnerCB's assignment operator
            //transfers over exact information from the RHS InnerCB
            //3-Both are NULL and NULL gets set to NULL
            //4-Both arent NULL and the InnerCB's assignment opperator handles
            //any difference in InnerCB capacity and transfers over exact data
            if(rhs.m_buffers[i] != NULL) *m_buffers[i] = *rhs.m_buffers[i];
            else m_buffers[i] = NULL;
        }
    }
    return *this; //returns the adapted object
}

/*  As before, this is a debugging function that prints out the contents
    of the entire data structure. Make good use of InnerCB::dump().
    See sample output below for suggested format.
*/


void CBofCB::dump() {
    cout << "---------------------------------------------" << endl;
    cout << "Outer Circular Buffer dump(): m_obSize = " << m_obSize << endl;

    int index = 0;
    for(int i=0; i<m_obSize; i++) {
        index = (m_oldest+i) % m_obCapacity;
        cout << "[" << index << "]";
        m_buffers[index]->dump();
    }
    cout << "---------------------------------------------" << endl;
}