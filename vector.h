#ifndef VECTOR_H
#define VECTOR_H

//  IMPORTANT: This was last updated 05/22/20. Make sure that this is
//  the latest version (Check your Github)


/**************************************************************************
* Author:           Nicholas Spector
* Assignment:       Final Project
* Class:            CS 003A
* Section:          MW:1:00pm-2:25pm; TR:1:00pm-3:05pm
* Due Date:         6/9/19
**************************************************************************/



#include <iostream>
#include <stdlib.h>
#include "array_functions.h"
#include <string>
#include "stokenizer.h"



bool V_DEBUG = false;


using namespace std;

/**************************************************************************
* Assignment: Final Project
*_________________________________________________________________________
*
* This program will allow the user to choose from 6 data types (int, double,
* string, complex, rational, and date) and insert those data types into lists.
* You can insert data, sort the data, remove data, and copy the lists.
* ________________________________________________________________________
* INPUT:
*   inputLine           :       This line will hold the values that will be
*                               inserted into the lists in the program
*
*
*
* OUTPUT:
*   displayText         :       This LineBox will hold the printed linked lists
*
****************************************************************************/

template <class T>
class Vector
{

    private:
        unsigned sz;        //  The size of the vector
        unsigned cap;       //  The capacity of the vector
        T *data;            //  Dynami Array that holds elements of vector

        /***************
        ** MUTATORS **
        ***************/
        void expand();
        void expand( unsigned amount   //  IN: The amount to expand the capacity
                                       //  of the vector
                     );

        bool DEBUG = false;

    public:

        /*****************
        ** CONSTRUCTORS **
        ******************/
        Vector();
        Vector(string *arr);
        Vector(initializer_list<T> list);
//        Vector( unsigned size );    //  IN: The size to be set for the vector
//        Vector( unsigned size,      //  IN: The size to be set for the vector
//                   T value);        //  IN: The value to fill vector with

        Vector(const Vector<T> &v);

        /*****************
        ** DECONSTRUCTORS **
        ******************/
        ~Vector();

        /***************
        ** ACCESSORS **
        ***************/
        unsigned size() const;
        unsigned capacity() const;
        bool empty() const;
        const T & at(unsigned index  //  IN: The position in vector to access
                       ) const;
        const T & back() const;
        const T & front() const;

        Vector<T> create_union(const Vector<T> &rightVector);
        Vector<T> create_intersection(const Vector<T> &rightVector);

        void print();


        /***************
        ** MUTATORS **
        ***************/
        T & at( unsigned index);       //  IN: The position in vector to access

        void insert (unsigned index,   //  IN: The position in vector to access
                     T value );        //  IN: The value to fill vector with

        void erase( unsigned index);   //  IN: The position in vector to erase

        T & front();

        T & back();

        void assign ( unsigned n,      //  IN: The size to assign to the vector
                      T value );       //  IN: The value to fill vector with

        void push_back( T value );     //  IN: The value to put into the vector

        void pop_back();

        void clear();

        void resize( unsigned size,    //  IN: The size to use when resizing
                                       //  vector
                     T value );        //  IN: The value to fill the vector with
                                       //  if bigger than before

        void resize( unsigned size);   //  IN: The size to use when resizing
                                       //  vector

        void reserve( unsigned n );    //  IN: The minimum capacity to set the
                                       //  vector to

        void copy_v(const Vector<T> &source);

        string to_string() const;

        Vector<T> operator +=(T &rh);
        Vector<T>& operator =(const Vector<T>& rhs);

};

template <typename T>
void Vector<T>::print()
{
    for(unsigned int i = 0; i < size(); i++)
    {
        cout << at(i) << endl;
    }
}

template <typename T>
Vector<T>::Vector(string *arr)
{

    //  PROCESSING: Initialize size and capacity to 0 and set the data
    //  pointer to NULL
    sz = 0;
    cap = 0;
    data = nullptr;

    //  PROCESSING: Deallocate memory from data;
    if(V_DEBUG)
    {
        cout << "\n\nCalling constructor on Vector!\n\n";
        cout << "memory address: " << this;
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;
        cout << "data: ";
        print_array(data, sz);
        cout << endl;
        cout << "data memory address: " << data << endl;
        if(data == nullptr)
        {
            cout << "\n\ndata is equal to nullptr\n\n";
        }
    }

    for(unsigned int i = 0; arr[i] != ""; i++)
    {
        cout << "arr: " << arr[i] << endl;
        push_back(arr[i]);
    }
}

template <typename T>
Vector<T>::Vector(initializer_list<T> list)
{

    //  PROCESSING: Initialize size and capacity to 0 and set the data
    //  pointer to NULL
    sz = 0;
    cap = 0;
    data = nullptr;

    //  PROCESSING: Deallocate memory from data;
    if(V_DEBUG)
    {
        cout << "\n\nCalling constructor on Vector!\n\n";
        cout << "memory address: " << this;
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;
        cout << "data: ";
        print_array(data, sz);
        cout << endl;
        cout << "data memory address: " << data << endl;
        if(data == nullptr)
        {
            cout << "\n\ndata is equal to nullptr\n\n";
        }
    }

    for(unsigned int i = 0; i < list.size(); i++)
    {

        push_back(*(list.begin() + i));
    }
}

template <typename T>
string Vector<T>::to_string() const
{
    string output;

    output = "";

    for(unsigned int i = 0; i < sz; i++)
    {
        if constexpr(is_same<T, int>::value == true || is_same<T, unsigned int>::value == true || is_same<T, long>::value == true)
        {
            output = output + std::to_string(data[i]);
        }

        if constexpr (is_same<T, string>::value == true)
        {
            output = output + data[i];
        }

        if(i + 1 < sz)
        {
            output = output +  ", ";
        }
    }

    return output;
}

template <typename T>
Vector<T> Vector<T>::operator +=(T &rh)
{
    push_back(rh);
}





/****************************************************************
* Vector Class
* This class creates a vector object. The Vector object is a dynamic array
* that allows you to expand and contract it.
***************************************************************/

/******************************
** CONSTRUCTOR & DESTRUCTOR **
******************************/
/****************************************************************
* Vector ();
* Constructor; Initialize class attributes
* Parameters: none
* Return: An empty vector
***************************************************************/
/****************************************************************
* Vector (unsigned size);
* Constructor; Initialize class attributes
* Parameters: size - The specified size for the vector
* Return: A vector with a set size initialized to 0
***************************************************************/
/****************************************************************
* Vector (unsigned size, int value);
* Constructor; Initialize class attributes
* Parameters: size  - The specified size for the vector
*             value - The specified value to fill the vector with
* Return: A vector with a set size initialized to the value
***************************************************************/
/****************************************************************
* ~Vector ();
* Deconstructor; Initialize class attributes
* Parameters: none
* Return: A deconstructed object with the data array deallocted
***************************************************************/

/**************
** MUTATORS **
**************/
/*******************************************************************
* void & at(unsigned index);
*
* Mutator; This method allows you to store an integer in a specific
* element on the vector
*------------------------------------------------------------------
* Parameter: index  -   The index of the element to insert into
*------------------------------------------------------------------
* Return: A vector with an updated element
*******************************************************************/
/*******************************************************************
* void insert(unsigned index, int value);
*
* Mutator; This method allows you to insert an element into a vector
* at a specified index
*------------------------------------------------------------------
* Parameter: index  -   The index of the element to insert into
*            value  -   The value to insert into the vector
*------------------------------------------------------------------
* Return: A vector with a new element inserted into it
*******************************************************************/
/*******************************************************************
* void erase(unsigned index);
*
* Mutator; This method will remove an integer from the specified
* index on the vector and the move all of the values from
* the left over one notch so that the vector remains continuous.
*------------------------------------------------------------------
* Parameter: index   -  The index of which to remove the
*                       integer from the vector
*------------------------------------------------------------------
* Return: A vector with a removed element
*******************************************************************/
/*******************************************************************
* void & front();
*
* Mutator; This method will allow you to replace the first element of
* the vector
*------------------------------------------------------------------
* Parameter:
*------------------------------------------------------------------
* Return: The first element of the vector
*******************************************************************/
/*******************************************************************
* void & back();
*
* Mutator; This method will allow you to replace the last element of
* the vector
*------------------------------------------------------------------
* Parameter:
*------------------------------------------------------------------
* Return: The last element of the vector
*******************************************************************/
/*******************************************************************
* void & assign();
*
* Mutator; This method will assigns new content to the vector.
*------------------------------------------------------------------
* Parameter: n      :     The size of the new content for the vector
*            value  :     The value to assign to the new elements in the vector
*------------------------------------------------------------------
* Return: A vector with new content assigned to it
*******************************************************************/
/*******************************************************************
* void & push_back();
*
* Mutator; This method will add another integer to the top of the stack.
* It will also double the capacity if it reaches the end.
*------------------------------------------------------------------
* Parameter: value  -  The integer to insert into the vector
*------------------------------------------------------------------
* Return: A vector with the specified integer added to the end of it
*******************************************************************/
/*******************************************************************
* void & pop_back();
*
* Mutator; This method will remove an integer from the top of the stack
*------------------------------------------------------------------
* Parameter:
*------------------------------------------------------------------
* Return: A vector with an integer removed from it
*******************************************************************/
/*******************************************************************
* void & clear();
*
* Mutator; This method set the size of the vector to 0, thus removing
* all of the stored integers from the vector
*------------------------------------------------------------------
* Parameter:
*------------------------------------------------------------------
* Return: An empty vector
*******************************************************************/
/*******************************************************************
* void & resize();
*
* Mutator; This method allows you expand the capacity of the data array
* by a specified amount
*------------------------------------------------------------------
* Parameter: amount  -   The amount to add to the array
*------------------------------------------------------------------
* Return: A new data array with a new size
*******************************************************************/
/*******************************************************************
* void & resize();
*
* Mutator; This method allows you expand the capacity of the data array
* by a specified amount and fill the new elements with
* a specified value
*------------------------------------------------------------------
* Parameter: amount  -   The amount to add to the array
*            value   -   The value to fill the vector with
*------------------------------------------------------------------
* Return: A new data array with a new size that is filled with
*         a specified value
*******************************************************************/
/*******************************************************************
* void & reserve();
*
* Mutator; This method reserves a minimum specified capacity. It could be
* more than the minimum if doubling the current cacacity will provide
* a greater capacity than the minimum
*------------------------------------------------------------------
* Parameter:    n    -   The minumum capacity to set for the vector
*------------------------------------------------------------------
* Return: A new data array with a new size that is filled with
*         a specified value
*******************************************************************/
/*******************************************************************
* void & expand();
*
* Mutator; This method doubles the capacity of the data array
*------------------------------------------------------------------
* Parameter:
*------------------------------------------------------------------
* Return: A new data array with a doubled capacity
*******************************************************************/
/*******************************************************************
* void & expand();
*
* Mutator; This method allows you expand the capacity of the data array
* by a specified amount
*------------------------------------------------------------------
* Parameter:    amount   -   The amount to add to the array
*------------------------------------------------------------------
* Return: A new data array with a new size
*******************************************************************/


/***************
** ACCESSORS **
***************/
/****************************************************************
* unsigned size() const;
*
* Accessor; This method returns the size of the vector
* --------------------------------------------------------------
* Parameters: none
* --------------------------------------------------------------
* Return: The size of the vector is returned
***************************************************************/
/****************************************************************
* unsigned capacity() const;
*
* Accessor; This method returns the capacity of a vector
* --------------------------------------------------------------
* Parameters: none
* --------------------------------------------------------------
* Return: The capacity is returned as an unsigned int
***************************************************************/
/****************************************************************
* unsigned empty() const;
*
* Accessor; Checks if the vector is empty
* --------------------------------------------------------------
* Parameters: none
* --------------------------------------------------------------
* Return: Returns true if the vector is empty and false if is not
***************************************************************/
/****************************************************************
* const int & at(unsigned index) const;
*
* Accessor; This method return an element from the vector
* --------------------------------------------------------------
* Parameters: none
* --------------------------------------------------------------
* Return: A constant will be returned with the contents of the element
***************************************************************/
/****************************************************************
* const int & back() const;
*
* Accessor; Returns the last element of the vector
* --------------------------------------------------------------
* Parameters: none
* --------------------------------------------------------------
* Return: Returns the last element of the vector
***************************************************************/
/****************************************************************
* const int & front() const;
*
* Accessor; Returns the first element of the vector
* --------------------------------------------------------------
* Parameters: none
* --------------------------------------------------------------
* Return: Returns the first element of the vector
***************************************************************/


/**************************************************************************
*  AUTHOR          : Nicholas Spector
*  Assignement 5   : Vector
*  CLASS           : CS003A
*  SECTION         : MTWTh: 1:00am - 3:05pm
*  Due Date        : 4/2/19
**************************************************************************/




/**************************************************************************
* Assignment 5: Vector
*_________________________________________________________________________
*
* This class simulates the vector class. It creates a dynamic array with
* features allowing you to add more to it, change its size, and insert
* elements into it.
* ________________________________________________________________________
* INPUT:
*   index                   :   The index of the element being manipulated
*   size                    :   The specified size being manipulated in the
*                               vector
*   value                   :   The value to insert into the vector
*   n                       :   The minimum capacity to set for the vector
*
*
*
* OUTPUT:
*   .at()                   :   Prints out the specified element
*   .size()                 :   Prints out size of vector
*   .capacity()             :   Prints out capacity of vector
*   .front()                :   Prints out the first element of the vector
*   .back()                 :   Prints out the last element of the vector
*
****************************************************************************/


/**********************************************************
*
* Constructor Vector(): Class Vector
*_________________________________________________________
* Constructs a Vector Object with a capacity of 0 and size of 0
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   An empty Vector Object
***********************************************************/
template <class T>
Vector<T>::Vector()
{

    //  PROCESSING: Initialize size and capacity to 0 and set the data
    //  pointer to NULL
    sz = 0;
    cap = 0;
    data = nullptr;

    //  PROCESSING: Deallocate memory from data;
    if(V_DEBUG)
    {
        cout << "\n\nCalling constructor on Vector!\n\n";
        cout << "memory address: " << this;
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;
        cout << "data: ";
        print_array(data, sz);
        cout << endl;
        cout << "data memory address: " << data << endl;
        if(data == nullptr)
        {
            cout << "\n\ndata is equal to nullptr\n\n";
        }
    }


}

template <typename T>
void Vector<T>::copy_v(const Vector<T> &source)
{



    //  PROC: Transfer over private memeber variables
    sz = source.sz;
    cap = source.cap;

    delete [] data;

    //  PROC: If the vector on the right has something in it, copy it over
    if(cap > 0)
    {
        //  PROC: Allocate room for the data
        data = new T[cap];

        //  PROC: Copy data over
        for(unsigned int i = 0; i < sz; i++)
        {
            data[i] = source.data[i];
        }
    }


}

template <typename T>
Vector<T>& Vector<T>::operator =(const Vector<T>& rhs)
{
    sz = 0;
    cap = 0;
    data = nullptr;

    copy_v(rhs);
    return *this;
}

template <typename T>
Vector<T>::Vector(const Vector<T> &v)
{
    //  PROCESSING: Initialize size and capacity to 0 and set the data
    //  pointer to NULL
    sz = 0;
    cap = 0;
    data = nullptr;

    copy_v(v);


}

////**********************************************************
////*
////* Constructor Vector(): Class Vector
////*_________________________________________________________
////* Constructs a Vector Object with a set size and capacity to match
////*_________________________________________________________
////* PRE-CONDITIONS
////*   size        :       The size to be set for the vector
////* POST-CONDITIONS
////*   An vector with a set size
////***********************************************************/
//template <class T>
//Vector<T>::Vector( unsigned size ) //  IN: The size to be set for the vector
//{
//    T *temp;

//    //  PROCESSING: Set the size of the vector and set the capacity to be
//    //  twice the size;
//    sz = size;
//    cap = size;

//    //  PROCESSING: Allocate memory for the data array
//    data = new T[cap];
//    temp = new T;

//    //  PROCESSING: Initializes the vector with 0's up to the declared size
//    for(unsigned int i = 0; i < size; i++)
//    {

//        data[i] = *temp;
//    }

//}


////**********************************************************
////*
////* Constructor Vector(): Class Vector
////*_________________________________________________________
////* Constructs a Vector Object with a set size filled with a
////* specified default value
////*_________________________________________________________
////* PRE-CONDITIONS
////*   size        :       The size to be set for the vector
////*   value       :       The value to fill the vector with
////* POST-CONDITIONS
////*   A filled vector
////***********************************************************/
//template <class T>
//Vector<T>::Vector( unsigned size,   //  IN: The size to be set for the vector
//                         T value)   //  IN: The value to fill vector with
//{
//    //  PROCESSING: Set the size of the vector and set the capacity to be
//    //  twice the size;
//    sz = size;
//    cap = size;

//    //  PROCESSING: Allocate memory for the data array
//    data = new int[cap];

//    //  PROCESSING: Initializes the vector with the declared value
//    for(unsigned int i = 0; i < size; i++)
//    {

//        data[i] = value;
//    }
//}


/**********************************************************
*
* Deconstructor Vector(): Class Vector
*_________________________________________________________
* Deconstructs the Vector object by deallocating the
* memory used by data.
*_________________________________________________________
* PRE-CONDITIONS
* POST-CONDITIONS
*   A deconstructed Vector object and more memory availability
***********************************************************/
template <class T>
Vector<T>::~Vector()
{
    //  PROCESSING: Deallocate memory from data;
    if(V_DEBUG)
    {
        cout << "\n\nCalling Destructor on Vector!\n\n";
        cout << "memory address: " << this;
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;
        cout << "data: ";
        print_array(data, sz);
        cout << endl;
        cout << "data memory address: " << data << endl;
        if(data == nullptr)
        {
            cout << "\n\ndata is equal to nullptr\n\n";
        }
    }

//    delete [] data;


    if(V_DEBUG)
    {
        cout << "\n\nEnd of Destructor on Vector!\n\n";
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;
        if(data == nullptr)
        {
            cout << "\n\ndata is equal to nullptr\n\n";
        }
    }
}



/**********************************************************
*
* Method push_back(): Class Vector
*_________________________________________________________
* This method will add another integer to the top of the stack.
* It will also double the capacity if it reaches the end.
*_________________________________________________________
* PRE-CONDITIONS
*   value       :       The integer to insert into the vector
*
* POST-CONDITIONS
*   - A vector with the specified integer added to the end of it
***********************************************************/
template <class T>
void Vector<T>::push_back( T value )  //  IN: The value to put into the vector
{

    //  PROCESSING: Expand the data array times two
    if(sz >= cap && sz > 0)
    {
        expand();

    }

    //  PROCESSING: Expand the capacity by one
    else if(sz == 0)
    {
        expand(1);
    }

    //  PROCESSING: Increment the record of size by 1
    sz++;

    //  PROCESSING: Add the new value to the end of the stack
    data[sz - 1] = value;

}

/**********************************************************
*
* Method pop_back(): Class Vector
*_________________________________________________________
* This method will remove an integer from the top of the stack
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - A vector with an integer removed from it
***********************************************************/
template <class T>
void Vector<T>::pop_back()
{
    //  PROCESSING: If the vector is empty, then exit
    if(empty())
        exit(1);

    //  PROCESSING: Reduce size by one
    sz--;
}


/**********************************************************
*
* Method clear(): Class Vector
*_________________________________________________________
* This method set the size of the vector to 0, thus removing
* all of the stored integers from the vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - An empty vector
***********************************************************/
template <class T>
void Vector<T>::clear()
{

    //  PROCESSING: Reduce size to 0;
    sz = 0;
}


/**********************************************************
*
* Method erase(): Class Vector
*_________________________________________________________
* This method will remove an integer from the specified
* index on the vector and the move all of the values from
* the left over one notch so that the vector remains continuous.
*_________________________________________________________
* PRE-CONDITIONS
*   index       :       The index of which to remove the
*                       integer from the vector
*
* POST-CONDITIONS
*   - A vector with a removed element
***********************************************************/
template <class T>
void Vector<T>::erase( unsigned index ) //  IN: The position in vector to erase
{

     T *temp;       //  IN: The blank object that will be inserted into the
                    //  array to overwrite


    //  PROCESSING: If the specified index is out of range , then exit
    if(index >= sz)
    {
        exit(1);

    }

    //  PROCESSING: Move every element to the right of the chosen index to the
    //  right
    for(unsigned int i = index; i < sz - 1; i++)
    {
        data[i] = data[i + 1];
    }

    //  PROCESSING: Allocate memory for the blank object
    temp = new T;

    data[sz - 1] = *temp;





    //  PROCESSING: Reduce the size by 1.
    sz--;
}


/**********************************************************
*
* Method at(): Class Vector
*_________________________________________________________
* This method return an element from the vector
*_________________________________________________________
* PRE-CONDITIONS
*   index       :       The index of the element to return
*
* POST-CONDITIONS
*   - A constant will be returned with the contents of the element
***********************************************************/
template <class T>
const T & Vector<T>::at(unsigned index    //  IN: The position in vector to
                                          //  access
                          ) const
{

    //  PROCESSING: If index is within bounds, return the element
    if(index < sz)
        return data[index];

    //  PROCESSING: If the user tries to access an out of bound element,
    //  then exit
    else
    {
        assert(0);
    }
}

/**********************************************************
*
* Method at(): Class Vector
*_________________________________________________________
* This method allows you to store an integer in a specific
* element on the vector
*_________________________________________________________
* PRE-CONDITIONS
*   index       :       The index of the element to insert into
*
* POST-CONDITIONS
*   - A vector with an updated element
***********************************************************/
template <class T>
T & Vector<T>::at(unsigned index)     //  IN: The position in vector to access
{

    if(V_DEBUG)
    {
        cout << "\n\nCalling Vector:at()\n\n";
        cout << "data: " << to_string() << endl;
    }

    //  PROCESSING: If index is within bounds, return the element
    if(index < sz)
        return data[index];

    //  PROCESSING: If the user tries to access an out of bound element,
    //  then exit
    else
    {
        assert(0);
    }
}

/**********************************************************
*
* Method insert(): Class Vector
*_________________________________________________________
* This method allows you to insert an element into a vector
* at a specified index
*_________________________________________________________
* PRE-CONDITIONS
*   index       :       The index of the element to insert into
*   value       :       The value to insert into the vector
*
* POST-CONDITIONS
*   - A vector with a new element inserted into it
***********************************************************/
template <class T>
void Vector<T>::insert(unsigned index, //  IN: The position in vector to access
                       T value)        //  IN: The value to fill vector with
{


    if(DEBUG)
    {
        cout << "\n\nCalled Vector::insert()\n\n";
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;

        cout << "data: " << to_string() << endl;
    }

    //  PROCESSING: If the size is the same as the capacity, then doubele the
    //  capacity
    if(sz == cap)
    {

        if(DEBUG)
        {
            cout << "\nCalling expand(cap)\n";
        }

        expand(cap);
    }

    //  PROC: If the size is 0, then just expand enough for one element
    else if(sz == 0)
    {
        if(DEBUG)
        {
            cout << "\nCalling expand()\n";
        }

        expand(1);
    }

    //  PROCESSING: If the specified index is within range of the size of the
    //  vector
    if(index < sz  && index >= 0)
    {
        if(DEBUG)
        {
            cout << "\nThe index is within range\n";
        }

        //  PROCESSING: Move every element to the right of the chosen index to the
        //  right
        for(unsigned int i = sz; i > index; i--)
        {



            data[i] = data[i - 1];

        }

        //  PROCESSING: Place new element into the chosen position
        data[index] = value;

        //  PROCESSING: Increment sz by 1 to reflect the newly inserted element
        sz++;

    }

    //  PROC: If the size of the vector is 0 or the index is equal to size (
    //  meaning the user is trying to insert at the end of the vector) then
    //  just do a push_back
    else if(sz == 0 || index == sz)
    {
        push_back(value);
    }

    if(DEBUG)
    {
        cout << "\n\End of Vector::insert()\n\n";
        cout << "size: " << sz << endl;
        cout << "cap: " << cap << endl;

        cout << "data: " << to_string() << endl;
    }





}

/**********************************************************
*
* Method size(): Class Vector
*_________________________________________________________
* This method returns the size of the vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - The size of the vector is returned
***********************************************************/
template <class T>
unsigned Vector<T>::size() const
{
    //  PROCESSING: Returns size
    return sz;
}

/**********************************************************
*
* Method capacity(): Class Vector
*_________________________________________________________
* This method returns the capacity of a vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - The capacity is returned as an unsigned int
***********************************************************/
template <class T>
unsigned Vector<T>::capacity( ) const
{
    //  PROCESSING: Returns capacity
    return cap;
}

/**********************************************************
*
* Method expand(): Class Vector
*_________________________________________________________
* This method allows you expand the capacity of the data array
* by a specified amount
*_________________________________________________________
* PRE-CONDITIONS
*   amount      :       The amount to add to the array
*
* POST-CONDITIONS
*   - A new data array with a new size
***********************************************************/
template <class T>
void Vector<T>::expand(unsigned amount)   //  IN: The amount to expand the
                                          //  capacity of the vector
{

    T *temp;              //  CALC: Create temporary pointer to hold the
                            //  data array while we allocate new memory for it

    temp = nullptr;

    //  PROCESSING: Add the amount to the object's capacity
    cap += amount;

    //  PROCESSING: Allocate memory for the temp array
    temp = new T[cap];

    //  PROCESSING: Transfer all data from the data array to the temp array
    for(unsigned i = 0; i < sz; i++)
    {
        temp[i] = data[i];


    }

    if(V_DEBUG)
    {
        cout << "\n\nMemory Address before delete: " << data << endl;
    }
    //  PROCESSING: Deallocate memory from data
    delete [] data;

    //  PROC: Point data to nullptr
    data = nullptr;

    //  PROCESSING: Reallocate new memory for data
    data = new T[cap];

    //  PROCESSING: Transfer all data back from temp array to data
    for(unsigned i = 0; i < sz; i++)
    {
        data[i] = temp[i];

    }

    if(V_DEBUG)
    {
        cout << "\n\nMemory Address before delete: " << temp << endl;
    }
    //  PROCESSING: Delete temp array
    delete [] temp;
    temp = nullptr;

}


/**********************************************************
*
* Method expand(): Class Vector
*_________________________________________________________
* This method doubles the capacity of the data array
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - A new data array with a doubled capacity
***********************************************************/
template <class T>
void Vector<T>::expand()
{

    T *temp;              //  CALC: Create temporary pointer to hold the
                            //  data array while we allocate new memory for it

    temp = nullptr;

    //  PROCESSING: Add the amount to the object's capacity
    cap *= 2;

    //  PROCESSING: Allocate memory for the temp array
    temp = new T[cap];

    //  PROCESSING: Transfer all data from the data array to the temp array
    for(unsigned i = 0; i < sz; i++)
    {
        temp[i] = data[i];

    }

    if(V_DEBUG)
    {
        cout << "\n\nMemory Address before delete: " << data << endl;
    }

    //  PROCESSING: Deallocate memory from data
    delete [] data;
    data = nullptr;

    //  PROCESSING: Reallocate new memory for data
    data = new T[cap];

    //  PROCESSING: Transfer all data back from temp array to data
    for(unsigned i = 0; i < sz; i++)
    {
        data[i] = temp[i];
    }

    if(V_DEBUG)
    {
        cout << "\n\nMemory Address before delete: " << temp << endl;
    }
    //  PROCESSING: Delete temp array
    delete [] temp;
    temp = nullptr;

}

/**********************************************************
*
* Method resize(): Class Vector
*_________________________________________________________
* This method allows you expand the capacity of the data array
* by a specified amount
*_________________________________________________________
* PRE-CONDITIONS
*   amount      :       The amount to add to the array
*
* POST-CONDITIONS
*   - A new data array with a new size
***********************************************************/
template <class T>
void Vector<T>::resize( unsigned size )    //  IN: The size to use when resizing
                                           //  vector
{

    //  PROCESSING: If the specified size is less than the current size, then
    //  make the specified size the new vector's size and that's it
    if(size < cap)
    {
        sz = size;
    }

    //  PROCESSING: If the size is greater than or equal to capacity
    else if(size >= cap)
    {
        //  PROCESSING: If the increase in size is less than capacity,
        //  it will be more fruitful for us to double capacity
        if(size - cap < cap)
        {
            expand();
        }

        //  PROCESSING: If the increase in size is greater than doubling
        //  capacity, then we should increase the capacity by that specified
        //  increase
        else if(size - cap > cap)
        {
            expand(size - cap);
        }

    }

    //  PROCESSING: Fill the new elements with 0s
    for(unsigned i = sz; i < size; i++)
    {
        data[i] = 0;
    }

    //  PROCESSING: Set sz to the new size
    sz = size;
}


/**********************************************************
*
* Method resize(): Class Vector
*_________________________________________________________
* This method allows you expand the capacity of the data array
* by a specified amount and fill the new elements with
* a specified value
*_________________________________________________________
* PRE-CONDITIONS
*   amount      :       The amount to add to the array
*   value       :       The value to fill the vector with
*
* POST-CONDITIONS
*   - A new data array with a new size that is filled with
*   a specified value
***********************************************************/
template <class T>
void Vector<T>::resize( unsigned size,   //  IN: The size to use when resizing
                                         //  vector
                        T value )        //  IN: The value to fill the vector
                                         //  with if bigger than before
{

    //  PROCESSING: If the specified size is less than the current size, then
    //  make the specified size the new vector's size and that's it
    if(size <= sz)
    {
        sz = size;
    }

    //  PROCESSING: If the size is greater than or equal to capacity
    else if(size > cap)
    {

        //  PROCESSING: If the increase in size is less than capacity,
        //  it will be more fruitful for us to double capacity
        if(size - cap < cap)
        {
            expand();
        }

        //  PROCESSING: If the increase in size is greater than doubling
        //  capacity, then we should increase the capacity by that specified
        //  increase
        else if(size - cap > cap)
        {
            expand(size - cap);
        }

    }

    //  PROCESSING: Fill the new elements with the specified value
    for(unsigned i = sz; i < size; i++)
    {


        data[i] = value;
    }

    //  PROCESSING: Set sz to the new size
    sz = size;

}

/**********************************************************
*
* Method reserve(): Class Vector
*_________________________________________________________
* This method reserves a minimum specified capacity. It could be
* more than the minimum if doubling the current cacacity will provide
* a greater capacity than the minimum
*_________________________________________________________
* PRE-CONDITIONS
*   n      :       The minumum capacity to set for the vector
*
* POST-CONDITIONS
*   - A vector with a minimum capacity of n
***********************************************************/
template <class T>
void Vector<T>::reserve( unsigned n )    //  IN: The minimum capacity to set the
                                         //  vector to
{

    //  PROCESSING: Checks if the minimum specified is bigger than the
    //  current capacity
    if(n > cap)
    {

        //  PROCESSING: If the minimum is less than doubling the current
        //  capacity, then just simply double the current capacity
        if(n - cap < cap)
        {
            expand();
        }

        //  PROCESSING: If the new size being added is greater than double
        //  the capacity, then use that number to expand the capacity
        else if(n - cap > cap)
        {
            expand(n - cap);
        }

    }

}

/**********************************************************
*
* Method assign(): Class Vector
*_________________________________________________________
* Assigns new content to the vector.
*_________________________________________________________
* PRE-CONDITIONS
*   n      :       The size of the new content for the vector
*   value  :       The value to assign to the new elements in the vector
*
* POST-CONDITIONS
*   - A vector with new content assigned to it
***********************************************************/
template <class T>
void Vector<T>::assign( unsigned n,   //  IN: The size to assign to the vector
                        T value )     //  IN: The value to fill vector with
{

    //  PROCESSING: If the size of the new content is greater than capacity,
    //  then allocate a greater capacity
    if(n > cap)
    {

        //  PROCESSING: If doubling the capacity would bring the greatest
        //  then do that
        if(n < cap * 2)
        {
            expand();
        }

        //  PROCESSING: If the new size being added is greater than double
        //  the capacity, then use that number to expand the capacity
        else if(n > cap * 2)
        {
            expand(n - cap);
        }

    }

    //  PROCESSING: Set the vector's size as the specified size: n
    sz = n;

    //  PROCESSING: Fill up the vector with the specified value
    for(unsigned i = 0; i < sz; i++)
    {
        data[i] = value;
    }

}

/**********************************************************
*
* Method empty(): Class Vector
*_________________________________________________________
* Checks if the vector is empty
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - Returns true if the vector is empty and false if is not
***********************************************************/
template <class T>
bool Vector<T>::empty() const
{
    //  PROCESSING: If the size of the vector is 0, then return true
    if(sz == 0)
    {
        return true;
    }

    //  PROCESSING: If the size of the vector greater than 0, then return false
    else
    {
        return false;
    }

}

/**********************************************************
*
* Method front(): Class Vector
*_________________________________________________________
* Returns the first element of the vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - Returns the first element of the vector
***********************************************************/
template <class T>
T & Vector<T>::front()
{
    //  PROCESSING: Returns the first element
    return data[0];
}

/**********************************************************
*
* Method front(): Class Vector
*_________________________________________________________
* Returns the first element of the vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - Returns the first element of the vector
***********************************************************/
template <class T>
const T & Vector<T>::front() const
{
    //  PROCESSING: Returns the first element
    return data[0];
}

/**********************************************************
*
* Method back(): Class Vector
*_________________________________________________________
* Returns the last element of the vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - Returns the last element of the vector
***********************************************************/
template <class T>
T & Vector<T>::back()
{
    //  PROCESSING: Returns the last element
    return data[sz - 1];
}

/**********************************************************
*
* Method back(): Class Vector
*_________________________________________________________
* Returns the last element of the vector
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - Returns the last element of the vector
***********************************************************/
template <class T>
const T & Vector<T>::back() const
{
    //  PROCESSING: Returns the last element
    return data[sz - 1];
}

template <class T>
void print_vector(Vector<T> list)
{
    for(unsigned int i = 0; i < list.size(); i++)
    {
        cout << list.at(i) << ", ";
    }
}

string remove_quotations(string str)
{
    //  PROC: Remove the quotation marks at the beginning and the end of the
    //  string
    if(str.at(0) == '\"' && str.at(str.size() - 1) == '\"')
    {
        str.erase(str.begin());
        str.erase(str.end() - 1);
    }

    return str;
}

/**********************************************************
*
* Function string_to_vector(string str)
*_________________________________________________________
* Converts a string into a vector
*_________________________________________________________
* PRE-CONDITIONS
* string str    :   A string of words that are separated by commas
*
* POST-CONDITIONS
*   - Returns a vector of words (that were separated by commas in the string)
***********************************************************/
Vector<string> string_to_vector(string str)
{
    Vector<string> output;      //  CALC: This will hold the vector of strings
                                //  that will be outputted
    STokenizer tokenizer;       //  CALC: This will break up a string into tokens
    Token token;                //  CALC: token that will hold each token from
                                //  tokenizer

    //  PROC: Put the string in the tokenizer
    tokenizer.set_string(str);

    if(V_DEBUG)
    {
        cout << "\n\ncalled string_to_vector()\n\n";
        cout << "string: " << str << endl;
    }

    while(!tokenizer.done())
    {
        tokenizer >> token;

        if(V_DEBUG)
        {
            cout << "token: " << token.token_str() << endl;
        }

        //  PROC: If the token is not a commma and not a space then store it
        //  in the vector (so only the words are acquired.
        if(token.token_str() != "," && token.token_str() != " ")
        {
            output.push_back(remove_quotations(token.token_str()));
        }


    }

    if(V_DEBUG)
    {
        cout << "output vector: " << output.to_string() << endl;
    }

    return output;


}


/**********************************************************
*
* Function create_intersection()
*_________________________________________________________
* Creates a interesection with another vector
*_________________________________________________________
* PRE-CONDITIONS
* rightVector       :    The Vector that we are making the
*                        intersection with
*
* POST-CONDITIONS
*   - Returns a vector that is an intersection between this
*   vector and the right vector
***********************************************************/
template <class T>
Vector<T> Vector<T>::create_intersection(const Vector<T> &rightVector)
{
    Vector<T> *leftVector;    //  PROC: a pointer that points to the left vector
                              //  (the calling vector)
    unsigned int leftIndex;   //  PROC: The index that will traverse through
                              //  the left vector
    unsigned int rightIndex;  //  PROC: The index that will traverse through
                              //  the right vector
    T leftElement;            //  PROC: Holds the element of the left vector
    T rightElement;           //  PROC: Holds the element of the right vector
    Vector<T> output;         //  PROC: The vector that will hold the intersection

    //  PROC: Initialize Variables
    leftVector = this;
    leftIndex = 0;
    rightIndex = 0;
    leftElement = T();
    rightElement = T();

    if(V_DEBUG)
    {
        cout << "\n\ncalled Vector::create_intersection()\n\n";
        cout << "leftVector: " << leftVector->to_string() << endl;
        cout << "rightVector: " << rightVector.to_string() << endl;
    }

    //  PROC: While both of the vectors still have elements to traverse through
    //  if one of them reaches the end, we have finished the intersection
    while(leftIndex < leftVector->size() && rightIndex < rightVector.size())
    {

        leftElement = leftVector->at(leftIndex);
        rightElement = rightVector.at(rightIndex);


        if(V_DEBUG)
        {
            cout << "\nleftElement: " << leftElement << endl;
            cout << "leftIndex: " << leftIndex << endl;
            cout << "rightElement: " << rightElement << endl;
            cout << "rightIndex: " << rightIndex << endl;

        }

        //  PROC: If the elements equal each other. Store one of them into
        //  output Vector
        if(leftElement == rightElement)
        {

            if(V_DEBUG)
            {
                cout << "\nBoth elements are equal\n\n";

            }

            //  PROC: Store the left element into the output vector
            output.push_back(leftElement);

            //  PROC: Move both pointers forward
            leftIndex++;
            rightIndex++;


        }

        //  PROC: If the left element is less than the right. move the left
        //  pointer forward
        else if(leftElement < rightElement)
        {
            if(V_DEBUG)
            {
                cout << "\nleftelement is less than right.\n\n";


            }

            //  PROC: Move the left index forward
            leftIndex++;


        }

        //  PROC: If the right element is less than the left. move the right
        //  pointer forward
        else if(rightElement < leftElement)
        {
            if(V_DEBUG)
            {
               cout << "\nrightElement is less than left.\n\n";


            }

            //  PROC: Push the right into the vector and move the pointer
            //  to the next element
            rightIndex++;

        }

    }

    return output;



}

/**********************************************************
*
* Function create_union()
*_________________________________________________________
* Creates a union with another vector
*_________________________________________________________
* PRE-CONDITIONS
* rightVector       :    The Vector that we are making the
*                        union with
*
* POST-CONDITIONS
*   - Returns a vector that is a union between this vector
*   and the right vector
***********************************************************/
template <class T>
Vector<T> Vector<T>::create_union(const Vector<T> &rightVector)
{
    Vector<T> *leftVector;    //  PROC: a pointer that points to the left vector
                              //  (the calling vector)
    unsigned int leftIndex;   //  PROC: The index that will traverse through
                              //  the left vector
    unsigned int rightIndex;  //  PROC: The index that will traverse through
                              //  the right vector
    T leftElement;            //  PROC: Holds the element of the left vector
    T rightElement;           //  PROC: Holds the element of the right vector
    Vector<T> output;         //  PROC: The vector that will hold the union

    //  PROC: Initialize Variables
    leftVector = this;
    leftIndex = 0;
    rightIndex = 0;
    leftElement = T();
    rightElement = T();

    if(V_DEBUG)
    {
        cout << "\n\ncalled Vector::create_union()\n\n";
        cout << "leftVector: " << leftVector->to_string() << endl;
        cout << "left size: " << leftVector->size() << endl;
        cout << "rightVector: " << rightVector.to_string() << endl;
        cout << "right size: " << rightVector.size() << endl;
    }

    //  PROC: If the left vector is empty, store the right vector into output
    if(leftVector->empty())
    {
        output = rightVector;
    }

    //  PROC: if the right vector is empty, store the left vector into output
    else if(rightVector.empty())
    {
        output = *leftVector;
    }

    //  PROC: If neither of the vector's are empty, then we must find the
    //  union between tem
    else
    {
        //  PROC: While we still have elements to traverse through on any of the
        //  vectors
        while(leftIndex < leftVector->size() || rightIndex < rightVector.size())
        {
            //  PROC: store the current element of the left and right vector
            if(leftIndex < leftVector->size())
            {
                leftElement = leftVector->at(leftIndex);
            }
            if(rightIndex < rightVector.size())
            {
                rightElement = rightVector.at(rightIndex);
            }


            if(V_DEBUG)
            {
                cout << "\nleftElement: " << leftElement << endl;
                cout << "leftIndex: " << leftIndex << endl;
                cout << "rightElement: " << rightElement << endl;
                cout << "rightIndex: " << rightIndex << endl;

            }

            //  PROC: If the elements equal each other. Store one of them into
            //  output Vector
            if(leftElement == rightElement)
            {

                if(V_DEBUG)
                {
                    cout << "\nBoth elements are equal\n\n";

                }

                //  PROC: Store the left element into the output vector
                output.push_back(leftElement);

                //  PROC: Move both pointers forward
                leftIndex++;
                rightIndex++;


            }

            //  PROC: If the left element is less than the right.  Or we have
            //  fully iterated through the right vector (then we should just push
            //  the rest of the left vector into the output)
            else if((leftElement < rightElement && leftIndex < leftVector->size()) || rightIndex == rightVector.size())
            {
                if(V_DEBUG)
                {
                    if(rightIndex == rightVector.size())
                    {
                        cout << "\nthe right index == rightvector.size()\n\n";
                    }
                    else
                    {
                        cout << "\nleftelement is less than right.\n\n";
                    }


                }

                //  PROC: Push the left element into the output
                output.push_back(leftElement);

                //  PROC: Move the left index forward
                leftIndex++;


            }

            //  PROC: If the right element is less than the left.  Or we have
            //  fully iterated through the left vector (then we should just push
            //  the rest of the right vector into the output)
            else if((rightElement < leftElement && rightIndex < rightVector.size()) || leftIndex == leftVector->size())
            {
                if(V_DEBUG)
                {
                    if(leftIndex == leftVector->size())
                    {
                        cout << "\nthe left index == leftvector.size()\n\n";
                    }
                    else
                    {
                        cout << "\nrightElement is less than left.\n\n";
                    }


                }

                //  PROC: Push the right into the vector and move the pointer
                //  to the next element
                output.push_back(rightElement);
                rightIndex++;

            }

        }
    }



    return output;


}





#endif // VECTOR_H
