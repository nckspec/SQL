#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"

template <class T>
class Queue{
public:
    Queue():_head(nullptr), _tail(nullptr){}

    ~Queue();
    Queue(const Queue<T>& other);
    Queue<T> &operator =(const Queue& rhs);

    void push(T item);
    T pop();
    bool empty();
    T front();

    friend ostream& operator <<(ostream& outs, const Queue& q){
        return print_list(q._head, outs);

    }

private:
    node<T>* _head;
    node<T>* _tail;
};


/**********************************************************
*
* Deconstructor ~Queue(): Class Queue
*_________________________________________________________
* Deletes the linked list associated with the Queue
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*
***********************************************************/
template <class T>
Queue<T>::~Queue()
{
    //  PROC: Delete List and then set the head and the tail to null to finish
    //  the deletion
    delete_all(_head);
    _head = nullptr;
    _tail = nullptr;

}

/**********************************************************
*
* Constructor Queue(): Class Queue
*_________________________________________________________
* Creates a copy of a Queue into the new Queue
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   Returns the new queue
***********************************************************/
template <class T>
Queue<T>::Queue(const Queue<T>& other)
{

    //  PROC: Create a copy of the list used in "other" using other's head
    //  PROC: The output will be the last node from the "other" list and
    //  we will store that in the tail of this new list
    _tail = copy_list(other._head, _head);

}

/**********************************************************
*
* Method operator=(): Class Queue
*_________________________________________________________
* Makes a copy of the queue to the right of the = and puts
* its into the queue on the left of the =
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   Returns the new queue
***********************************************************/
template <class T>
Queue<T> & Queue<T>::operator=(const Queue<T>& rhs)
{
    //  PROC: Create a copy of the list used in "other" using other's head
    //  PROC: The output will be the last node from the "other" list
    //  and we will store that in the tail of this new list
    _tail = copy_list(rhs._head, _head);


    return *this;

}

/**********************************************************
*
* Method push(): Class Queue
*_________________________________________________________
* Puts an item into the queue
*_________________________________________________________
* PRE-CONDITIONS
*   T item: The item that will be going into the queue
*
*
* POST-CONDITIONS
*
***********************************************************/
template <class T>
void Queue<T>::push(T item)
{
    //  PROC: If the Queue is empty, then we set this new node to be the head
    //  and the tail
    if(_head == nullptr)
    {
        _head = new node<T>(item);
        _tail = _head;
    }

    //  PROC: If the Queue has nodes in it, then we set the new node
    //  to be following the tail
    else
    {
        //  PROC: We assert that _tail has something in it. If it doesn't
        //  then the program comes to an end
        assert(_tail);

        //  PROC: Create a new node following tail
        _tail->_next = new node<T>(item);

        //  PROC: Point the _tail to the new node
        _tail = _tail->_next;
    }

}

/**********************************************************
*
* Method pop(): Class Queue
*_________________________________________________________
* Deletes the node at the head of the list
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   Returns the item from the node
***********************************************************/
template <class T>
T Queue<T>::pop()
{

    T item;


    if(_head != nullptr)
    {
        //  PROC: Delete the first node of the linked list and return it's item
        item = delete_head(_head);

        //  PROC: If the last node is popped off, then we set the _tail to null
        //  as well so that we can completely deallocate the memory
        if(_head == nullptr)
        {
            _tail = nullptr;
        }


        //  PROC: Return the item from the deleted node
        return item;

    }

    //  PROC: Return a NULL since nothing was in the stack
    return T();


}

/**********************************************************
*
* Method front(): Class Queue
*_________________________________________________________
* Returns the item in the first node of the list
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   returns item
***********************************************************/
template <class T>
T Queue<T>::front()
{


    if(_head != nullptr)
    {
        //  PROC: Returns the 1st node of the list
        return _head->_item;
    }

}


/**********************************************************
*
* Method empty(): Class Queue
*_________________________________________________________
* Returns true if the Queue is empty, false if not
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   Returns bool true if empty false if not
***********************************************************/
template <class T>
bool Queue<T>::empty()
{

    //  PROC: Check if the list is empty by checking if the _head is null
    if(_head == nullptr)
    {
        return true;
    }

    else
    {
        return false;
    }

}


#endif // QUEUE_H
