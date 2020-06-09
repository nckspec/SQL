#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <vector>

using namespace std;

template <class T>
struct node{
    T _item;
    node<T>* _next;
    node(const T& item = T(), node<T>* next = NULL):_item(item), _next(next){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]->";
        return outs;
    }
};

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head);

//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head);

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head);

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy);

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item);

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item);

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head);

//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs=cout);


template <class T>
node<T>* init_head(node<T>* &head)
{

    head = nullptr;

    return head;


}

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy)
{

    //  PROC: Just so we can catch if the user is trying to copy an empty
    //  list
    assert(head != nullptr);

    //  PROC: Make sure there is a list in existence that can be copied
    if(head != nullptr)
    {

        //  PROC: Create temporary pointer that will be used to traverse
        //  through the new linked list (aka cpy)
        node<T> *temp = nullptr;

        //  PROC: Copy the 1st item of the original list into temp
        temp = new node<T>(head->_item);

        //  PROC: Point cpy to temp. cpy will not be touched after this and the
        //  list will be generated using temp
        cpy = temp;


        //  PROC: Move to the next node in the original list
        head = head->_next;

        //  PROC: Continue copying the orignal list until we get to the end
        while(head != nullptr)
        {
            //  PROC: Create a new node following the temp node that was
            //  created and copy the item from the original linked list
            temp->_next = new node<T>(head->_item);

            //  PROC: Point temp at this new node that was made
            temp = temp->_next;

            //  PROC: Move to the next node in the original list
            head = head->_next;
        }

       //  PROC: Return the last node
        return temp;

    }







}

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head)
{



    assert(head != nullptr);

    //  PROC: Make sure there is a head to delete
    if(head != nullptr)
    {


        //  PROC: Create temporary pointer which will point to the current node
        //  being deleted
        node<T> *temp = nullptr;

        //  PROC: Create temporary variable that will hold the item in the node
        T item = head->_item;

        //  PROC: Point the temp at the head node
        temp = head;

        //  PROC: point the head to the next node since that will be the
        //  new head
        head = head->_next;

        //  PROC: Delete the old head
        delete temp;
        temp = nullptr;

        //  PROC: Return the item of the node that is being deleted
        return item;

    }

    return T();






}


//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head)
{


    //  PROC: Keep Deleting the head until the list is completely empty
    while(head != nullptr)
    {
        delete_head(head);
    }

}

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item)
{


    //  PROC: Allocate memory for a node that will hold the new item
    node<T> *temp = nullptr;
    temp = new node<T> (item);

    //  PROC: if the head is null then we set the new node as the head
    if(head == nullptr)
    {
        head = temp;
    }




    //  PROC:if the after node is null, then we insert the new item at the head
    else if(after == nullptr)
    {
        //  PROC:point the new node to the head
        temp->_next = head;

        //  PROC:point the head to the new node
        head = temp;

    }

    else
    {
        //  PROC: Point the new node to the node after, after
        temp->_next = after->_next;

        //  PROC: Point the after node to the new node
        after->_next = temp;

    }

    //  PROC: Return new node
    return temp;



}

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item)
{
    //  PROC: Allocate memory for a node that will hold the new item
    node<T> *temp = nullptr;
    temp = new node<T> (item);

    //  PROC: Point the new node at the current head
    temp->_next = head;

    //  PROC: Point the head pointer at the new node
    head = temp;

    //  PROC: Return new head
    return head;

}


//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs)
{

    //  OUTPUT:  Start at the beginning of the list. Until we get to
    //  the end of the list, keep printing out the nodes
    while(head != nullptr)
    {
        outs << *head;
        head = head->_next;
    }

    outs << "|||";

    return outs;

}

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head)
{
    //  PROC: Checks if the list is empty or not
    if(head == nullptr)
    {
        return true;
    }

    else
    {
        return false;
    }
}





#endif // LIST_H
