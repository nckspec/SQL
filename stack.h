#ifndef STACK_H
#define STACK_H

#include "list.h"

template <class T>
class Stack{
public:
    Stack():_top(NULL){}

    ~Stack();
    Stack(const Stack<T>& other);
    Stack<T>& operator =(const Stack<T>& rhs);

    void push(T item);
    T pop();
    T top();
    bool empty();
    friend ostream& operator << (ostream& outs, const Stack& s){
        return print_list(s._top, outs);
    }
private:
    node<T>* _top;
};


/**********************************************************
*
* Deconstructor ~Stack(): Class Stack
*_________________________________________________________
* Deletes the linked list associated with the Stack
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*
***********************************************************/
template <class T>
Stack<T>::~Stack()
{

    //  PROC: Delete the linkedlist associate with the Stack and set the
    //  _top to null
    delete_all(_top);
    _top = nullptr;

}

/**********************************************************
*
* Constructor Stack(): Class Stack
*_________________________________________________________
* Constructs a new Stack with a copy of another stack
*_________________________________________________________
* PRE-CONDITIONS
*   other: The stack to be copied
*
* POST-CONDITIONS
*   A new stack that is a duplicate of another
*
***********************************************************/
template <class T>
Stack<T>::Stack(const Stack<T>& other)
{
    //  PROC: Create a copy of the list used in "other" using other's _top
    copy_list(other._top, _top);
}

/**********************************************************
*
* method operator=(): Class Stack
*_________________________________________________________
* Makes a copy of the Stack to the right of the = and puts
* its into the queue on the left of the =
*_________________________________________________________
* PRE-CONDITIONS
*   rhs: The Stack on the right side of the =
*
*
* POST-CONDITIONS
*   Returns the new queue
*
***********************************************************/
template <class T>
Stack<T> & Stack<T>::operator=(const Stack<T>& rhs)
{
    //  PROC: Create a copy of the list used in "other" using other's _top
    copy_list(rhs._top, _top);

    return *this;


}

/**********************************************************
*
* Method push(): Class Stack
*_________________________________________________________
* Puts an item into the Stack
*_________________________________________________________
* PRE-CONDITIONS
*   T item: The item that will be going into the Stack
*
*
* POST-CONDITIONS
*
***********************************************************/
template <class T>
void Stack<T>::push(T item)
{
    //  PROC: Create a temporary pointer for our new node that
    //  will be pushed to the top
    node<T> *temp;

    //  PROC: Insert the item into this new node
    temp = new node<T>(item);

    //  PROC: Point the new node to the top of the stack
    temp->_next = _top;

    //  PROC: Point the _top pointer to the new top aka temp
    _top = temp;

}

/**********************************************************
*
* Method pop(): Class Stack
*_________________________________________________________
* Pops an item out of the stack
*_________________________________________________________
* PRE-CONDITIONS
*
*
*
* POST-CONDITIONS
*   Returns the item of the node popped off the stack
*
***********************************************************/
template <class T>
T Stack<T>::pop()
{
    //  PROC: Holds the item of the node being deleted
    T item;

    //  PROC: If the stack has something in it then we pop and return the item
    if(_top != nullptr)
    {
        //  PROC: Store the deleted node's item into this variable
        item = delete_head(_top);

        //  PROC: Return the item from the deleted node popped off the top of
        //  stack
        return item;

    }

    return T();

}

/**********************************************************
*
* Method top(): Class Stack
*_________________________________________________________
* Returns the item from the top of the stack
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   Returns the item from the top of the stack
*
***********************************************************/
template <class T>
T Stack<T>::top()
{
    if(_top != nullptr)
    {
        //  PROC: Return the item at the top of the stack
        return _top->_item;
    }

    else
    {
        return T();
    }


}

/**********************************************************
*
* Method empty(): Class Stack
*_________________________________________________________
* Returns a bool stating if the Stack is empty or not
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*   Returns True if the stack is empty and false if not
*
***********************************************************/
template <class T>
bool Stack<T>::empty()
{
    //  PROC: Check if the list is empty by checking if the _top is null
    if(_top == nullptr)
    {
        return true;
    }

    else
    {
        return false;
    }

}

#endif // STACK_H
