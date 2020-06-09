#ifndef ARRAY_FUNCTIONS_H
#define ARRAY_FUNCTIONS_H

#include <iostream>
//#include "../The_BTree/mmap.h"



//  IMPORTANT: This was last updated 05/22/20. Make sure that this is
//  the latest version (Check your Github)

bool DEBUG = false;

using namespace std;


unsigned int dummy;
template <class T>
bool find_array(T item, T *arr, unsigned int size, unsigned int &index = dummy);

template <class T>
void quick_sort(T *arr, unsigned int size);

/**********************************************************
*
* Function : quick_sort()
*_________________________________________________________
* This sorts a list by picking an arbitrary "pivot" and sorting
* everything smaller than the pivot to the left of the pivot and
* everything greater to the right. It then recursively performs
* this on the list to the left of the pivot and it recursively performs
* this to the list to the right.
*_________________________________________________________
* PRE-CONDITIONS
* arr  :   The array to sort
* size :   The size of the array
*
* POST-CONDITIONS
*   - A sorted array
*
***********************************************************/
template <class T>
void quick_sort(T *arr, unsigned int size)
{

    T pivot;                //  CALC: Number that will be used as a reference
                            //  when sorting into the smallerList and the
                            //  biggerList
    T smallList[size];      //  CALC: List that will hold numbers smaller than
                            //  the pivot
    T bigList[size];        //  CALC: List that will hold numbers bigger than
                            //  the pivot
    int smallListSize;      //  CALC: Will keep track of the size of the smaller
                            //  list
    int bigListSize;        //  CALC: Will keep track of the size of the bigger
                            //  list
    int smallListIndex;     //  CALC: Will keep track of the current index
                            //  we are working with on the smaller list
    int bigListIndex;       //  CALC: Will keep track of the current index
                            //  we are working on with the bigger list
    int index;              //  CALC: Keeps track of the index that we are on
                            //  with the list: arr when doing the final combine
                            //  of the small List and big list


    //  PROC: Initialize Variable
    pivot = T();
    smallListSize = 0;
    bigListSize = 0;
    smallListIndex = 0;
    bigListIndex = 0;
    index = 0;



    //  PROC: Only sort if the list is bigger than 1 element. This will
    //  end the recursion loop once it has singled down to a one element
    //  list
    if(size > 1)
    {
        //  PROC: Store the last element as the pivot
        pivot = arr[size - 1];

        if(DEBUG)
        {
            cout << "------------------------------";
            cout << "\n\nRunning quick_sort():\n";
            cout << "list: ";
            for(int i = 0; i < size; i++)
            {
                cout << arr[i] << ", ";
            }
            cout << endl;
        }

        if(DEBUG)
        {
            cout << "pivot: " << pivot << endl;
        }


        //  PROC: Process each element in the array, (except the last one
        //  since that is the pivot and we don't want duplicates)
        for(int i = 0; i < size - 1; i++)
        {
            //  PROC: Check if the current element is smaller than the pivot
            if(arr[i] < pivot)
            {
                //  PROC: Insert the current eleent into the smallList
                smallList[smallListIndex] = arr[i];

                //  PROC: Increment the size of the small List
                smallListSize++;

                //  PROC: Increment the index of the small list
                smallListIndex++;
            }

            //  PROC: If the current elemenet is bigger than the pivot then
            //  we put it into the bigger list
            else
            {
                bigList[bigListIndex] = arr[i];

                //  PROC: Increment the size of the big list
                bigListSize++;

                //  PROC: Increment the index of the big list
                bigListIndex++;
            }


        }

        if(DEBUG)
        {
            cout << "small list: ";
            for(int i = 0; i < smallListSize; i++)
            {
                cout << smallList[i] << ", ";
            }
            cout << endl;

        }

        if(DEBUG)
        {
            cout << "big list: ";
            for(int i = 0; i < bigListSize; i++)
            {
                cout << bigList[i] << ", ";
            }
            cout << endl << endl;

        }

        //  PROC: Sort the smaller list and the bigger list
        quick_sort(smallList, smallListSize);
        quick_sort(bigList, bigListSize);

        //  PROC: Loop through each element in the small list placing it into
        //  the main array
        for(int i = 0; i < smallListSize; i++)
        {
            arr[index] = smallList[i];

            //  PROC: Increment index so we can move to the next element of
            //  the main array
            index++;
        }

        //  PROV: Insert the pivot in between the small list and the big list
        //  and increment the index
        arr[index] = pivot;
        index++;


        //  PROC: Loop through each element in the big list placing it into
        //  the main array
        for(int i = 0; i < bigListSize; i++)
        {
            arr[index] = bigList[i];

            //  PROC: Increment index so we can move to the next element of
            //  the main array
            index++;
        }

        if(DEBUG)
        {
            cout << "Final List: ";
            for(int i = 0; i < size + 1; i++)
            {
                cout << arr[i] << ", ";
            }
            cout << endl << endl;

        }




    }


}

/**********************************************************
*
* Function : void print_array()
*_________________________________________________________
* This function will print an array to the screen.
*_________________________________________________________
* PRE-CONDITIONS
* a[]  :   The array to print
* size :   The size of the array
*
* POST-CONDITIONS
*   - An array printed to the screen
*
***********************************************************/
template <class T>
void print_array(const T a[], int size)
{

    //  PROC: Loop through each element of the array printing it to the screen
    for(int i = 0; i < size; i++)
    {
        cout << a[i] << ", ";
    }
}

/**********************************************************
*
* Function : void copy_array()
*_________________________________________________________
* This function will copy an array to another array.
*_________________________________________________________
* PRE-CONDITIONS
* sourceArr     :   The array that is being copied
* destArr       :   The array that is having elements copied to it
* size          :   The size of both arrays
*
* POST-CONDITIONS
*   - The destArr array is a mirror copy of the sourceArr
*
***********************************************************/
template <class T>
void copy_array(const T *sourceArr, T *destArr, int size)
{

    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\nCalling Copy Array:\n";
        cout << "Source array: ";
        print_array(sourceArr, size);
        cout << endl;
    }

    //  PROC: Loop through each element of the source array, copying
    //  each element into the destination array
    for(int i = 0; i < size; i++)
    {
        destArr[i] = sourceArr[i];
    }

    if(DEBUG)
    {

        cout << "destination array: ";
        print_array(destArr, size);
        cout << "\nEnd of copy_array()\n";
        cout << "\n---------------------------\n";
    }
}

/**********************************************************
*
* Function : T remove_item()
*_________________________________________________________
* This function will remove an item from an array and then
* it will move over elements from the right to fill the gap
*_________________________________________________________
* PRE-CONDITIONS
* item      :   The item to remove
* arr       :   The array to remove from
* size      :   The size of the array
*
* POST-CONDITIONS
*   - The destArr array is a mirror copy of the sourceArr
*
***********************************************************/
template <class T>
bool remove_item(T item, T *arr, unsigned int &size)
{
    if(DEBUG)
    {
        cout << "\n\nCalling remove_item(): \n";
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "item: " << item << endl;
        cout << "starting size: " << size << endl;

    }

    //  PROC: Loop through the array looking for the item
    for(unsigned int i = 0; i < size; i++)
    {
        //  PROC: If we find the item, set it to null. Then move everything
        //  over to take its place
        if(arr[i] == item)
        {
            //  PROC: Set the element to null and reduce the referenc variable's
            //  size
            arr[i] = T();

            //  PROC: Reduce the size of the array
            size--;

            //  PROC: Loop starting from the index of the element we are
            //  deleting to the end of the array. Take the next element and
            //  place it in the current spot. The size has already been reduced
            //  but we need the last element in the array (we are not actually
            //  reaching out of bounds).
            for(unsigned int h = i; h < size; h++)
            {
                arr[h] = arr[h + 1];
            }


        }
    }


    if(DEBUG)
    {
        cout << "ending size: " << size << endl;
        cout << "final array: ";
        print_array(arr, size);
        cout << "\nEnd of remove_item()\n";
        cout << "\n---------------------------\n";
    }

}


/**********************************************************
*
* Function : void insert_item()
*_________________________________________________________
* This function will insert an item into an array
* at a specified index.
*_________________________________________________________
* PRE-CONDITIONS
* item      :   The item to be inserted into array
* arr       :   The array to be inserted to
* &size     :   The size of the array
*
* POST-CONDITIONS
*   - The item will be inserted into the array. The array will be
*   sorted. The reference variable size will be updated to reflect
*   this insertion
*
***********************************************************/
template <class T>
void insert_item(T item, T *arr, unsigned int &size)
{

    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\nCalling insert_item(): \n";
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "item: " << item << endl;
        cout << "starting size: " << size << endl;

    }

    //  PROC: Make room for the new item
    size++;

    //  PROC: Place the item in this new spot.
    arr[size - 1] = item;

    //  PROC: Sort the array
    quick_sort(arr, size);

    if(DEBUG)
    {
        cout << "\n\nCalling insert_item (2nd)(): \n";
        cout << "ending size: " << size << endl;
    }

    if(DEBUG)
    {
        cout << "final array: ";
        print_array(arr, size);
        cout << "\nEnd of insert_index()\n";
        cout << "\n---------------------------\n";
    }


}

/**********************************************************
*
* Function : void insert_item_index()
*_________________________________________________________
* This function will insert an item into an array
* at a specified index. The element that is currently at that
* index and everything to the right of it will be moved over to
* the right.
*_________________________________________________________
* PRE-CONDITIONS
* item      :   The item to insert into the array
* arr       :   The array to insert into
* index     :   The index that we are inserting into
* size      :   The size that the array will be after we insert into it
*
* POST-CONDITIONS
*   - An array with the item inserted into it at the index specified.
*   All of the elements will be moved over to make room.
*
***********************************************************/
template <class T>
void insert_item_index(T item, unsigned int index, T *arr, unsigned int &size)
{

    assert(index <= size);

    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\nCalling insert_item(): \n";
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "item: " << item << endl;

    }

    //  PROC: Increase the size for the new element
    size++;

    //  PROC: Start at the last index of the array. Continue looping until i
    //  is equal to the index
    for(unsigned int i = size - 1; i > index; i--)
    {
        //  PROC: Copy the prior element over to the current element
        arr[i] = arr[i - 1];
    }

    //  PROC: Insert the item into the open spot
    arr[index] = item;

    if(DEBUG)
    {
        cout << "\nEnd of insert_item_index()\n";
        cout << "\n---------------------------\n";
    }


}

template <class T>
bool delete_item(T *arr, unsigned int size, T item)
{

    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\ncalling delete_item():\n";
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "size: " << size << endl;
        cout << "Item: " << item << endl;
    }

    //  PROC: Loop through each element of the array
    for(int h = 0; h < size; h++)
    {
        //  PROC: If we have found an elemetn that matches the item
        if(arr[h] == item)
        {

            //  PROC: Decrease the size by 1 to reflect that the element
            //  is being deleted
            size--;

            //  PROC: Loop through the rest of array, moving over each element
            //  one to the left
            for(int i = h; i < size; i++)
            {
                arr[i] =  arr[i + 1];
            }


        }
    }

    if(DEBUG)
    {
        cout << "\nEnd of delete_item()\n";
        cout << "\n---------------------------\n";
    }


}

template <class T>
bool find_array_greater(T item, T *arr, unsigned int size, unsigned int &index)
{
    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\ncalling find_array():\n";
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "size: " << size << "\nindex: " << index << endl;
    }

    //  PROC: Loop through the array
    for(unsigned int i = 0; i < size; i++)
    {
        //  PROC: If the item matches an element in the array, return
        //  by reference its index and return true
        if(item < arr[i])
        {


            index = i;


            if(DEBUG)
            {
                cout << "Index Found! index: " << index << endl;
            }

            return true;
        }
    }

    index = size - 1;
    return false;

    if(DEBUG)
    {
        cout << "Index not found." << index << endl;
    }

    if(DEBUG)
    {
        cout << "\nEnd of find_array()\n";
        cout << "\n---------------------------\n";
    }

    //  PROC: If nothing was found, return false;

}

/**********************************************************
*
* Function : bool find_array()
*_________________________________________________________
* This function will search an array for an item. If it finds
* it, it will return true and return the index of the item
* by reference. If it doesn't find the array, it will just
* return false.
*_________________________________________________________
* PRE-CONDITIONS
* item      :   The item to insert into the array
* arr       :   The array to insert into
* index     :   The index that we are inserting into
* size      :   The size that the array will be after we insert into it
*
* POST-CONDITIONS
*   - An array with the item inserted into it at the index specified.
*   All of the elements will be moved over to make room.
*
***********************************************************/
template <class T>
bool find_array(T item, T *arr, unsigned int size, unsigned int &index)
{

    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\ncalling find_array():\n";
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "size: " << size << "\nindex: " << index << endl;
    }

    //  PROC: Loop through the array
    for(unsigned int i = 0; i < size; i++)
    {
        //  PROC: If the item matches an element in the array, return
        //  by reference its index and return true
        if(item == arr[i])
        {


            index = i;


            if(DEBUG)
            {
                cout << "Index Found! index: " << index << endl;
            }

            return true;
        }
    }

    if(DEBUG)
    {
        cout << "Index not found." << index << endl;
    }

    if(DEBUG)
    {
        cout << "\nEnd of find_array()\n";
        cout << "\n---------------------------\n";
    }

    //  PROC: If nothing was found, return false;
    return false;
}

template <class T>
void init_array(T *arr, int size)
{
    for(int i = 0; i < size; i++)
    {
        arr[i] = T();
    }
}

/**********************************************************
*
* Function : void array_insert()
*_________________________________________________________
* This function will insert an item into an array at a
* specified index
*_________________________________________________________
* PRE-CONDITIONS
* item      :   The item to insert into the array
* index     :   The index that we are inserting into
* size      :   The size of the array
*
* POST-CONDITIONS
*   - An array with a newly inserted item at the index
*
***********************************************************/
template <class T>
void array_insert(T item, unsigned int index, T *arr, unsigned int size)
{

    //  PROC: Assert that the index is within the bounds of the array
    assert(index < size);




    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\ncalling array_insert():\n";
        cout << "item: " << item << endl;
        cout << "array: ";
        print_array(arr, size);
        cout << endl << "index: " << index << endl;

    }

    //  PROC: If the index in the array is empty
    if(arr[index] == T())
    {
        if(DEBUG)
        {
            cout << "The spot we are inserting to is empty, so we don't have"
                    " to move anything over\n";
        }

        //  PROC: Store the item in the array at the specified index
        arr[index] = item;
    }

    //  PROC: If the index in the array is not empty
    else
    {
        //  PROC: Make sure that the last element in the array is empty
        //  before we start moving everything over
        assert(arr[size - 1] == T());

        if(DEBUG)
        {
            cout << "The index is not open so we have to move over everything"
                    " to the right of it over one spot.\n";
        }

        //  PROC: Start at the end of the array. Move every element over until
        //  we get to the index that we are inserting this new item into
        for(unsigned int i = size - 1; i > index; i--)
        {
            //  PROC: Take the current element, and place the element before
            //  it in its place
            arr[i] = arr[i - 1];
        }

        //  PROC: Insert the item into the newly open spot in the array
        arr[index] = item;

    }

    if(DEBUG)
    {
        cout << "\nEnd of array_insert()\n";
        cout << "\n---------------------------\n";
    }
}

/**********************************************************
*
* Function : void merge()
*_________________________________________________________
* This function will merge two arrays together. It will take
* the contents of the source array and insert it into
* the end of the destination array.
*_________________________________________________________
* PRE-CONDITIONS
* sourceArr     :   The array that is being inserted into the destination array
* destArr       :   The array that is receiving the source array into the end
*                   of it
* sourceSize    :   The size of the source array
*
* POST-CONDITIONS
*   - The destination array will have the source array inserted
*   - into the end of it
*
***********************************************************/
template <class T>
void merge(T *sourceArr, unsigned int sourceSize, T *destArr,  unsigned int maxDestSize, unsigned int &destSize)
{



    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\nCalling merge():\n\n";

        cout << "sourceSize: " << sourceSize << endl;
        cout << "maxDestSize: " << maxDestSize << endl;
        cout << "destSize: " << destSize << endl;

        cout << "Source Array: ";
        print_array(sourceArr, sourceSize);
        cout << endl;

        cout << "Destination Array: ";
        print_array(destArr, destSize);
        cout << endl;

    }

    assert(sourceSize + destSize <= maxDestSize);


    //  PROC: Loop through each element of the source array
    for(unsigned int i = 0; i < sourceSize; i++)
    {
        //  PROC: Increment the size of the destination array
        destSize++;

        //  PROC: Insert the element to the end of the destination array
        destArr[destSize - 1] = sourceArr[i];
    }



    if(DEBUG)
    {
        cout << "\nEnd of Merge()\n";
        cout << "\n---------------------------\n";
    }





}

/**********************************************************
*
* Function : void split()
*_________________________________________________________
* This function will split an array in half.
* The source array will is the array that will be split. The
* 1st half of it will remain in the source array. The 2nd
* half will remain in the destination array
* If it cannot be split evenly, the excess item will remain
* in the source array.
*_________________________________________________________
* PRE-CONDITIONS
* sourceArr     :   The array that is being splite (source)
* destArr       :   The array that is receiving the 2nd half (destination)
* sourceSize    :   The size of the source array
*
* POST-CONDITIONS
*   - The source array will have the 1st half of the source array
*   with the excess element. The destination array will have
*   the 2nd half.
*
***********************************************************/
template <class T>
void split(T *sourceArr, T *destArr, int sourceSize)
{



    //  PROC: The size of the destination will be the second half the the
    //  sourceSize. We subtract sourceSize moudulus 2 to account for an uneven
    //  split in which we would want the source to have an extra element
    int destSize = sourceSize - (sourceSize / 2) - (sourceSize % 2);

    //  PROC: This markes the starting element in the source array that will
    //  end up in the destination array. We add source size modulus 2 to account
    //  for an uneven split in which we need the index to move up to account
    //  for the source having the extra element
    int destIndex = (sourceSize / 2) + (sourceSize % 2);

    //  PROC: Used to iterate through the destination array
    int index = 0;

    if(DEBUG)
    {
        cout << "\n---------------------------\n";
        cout << "\n\nCalling split():\n";
        cout << "destSize: " << destSize << endl;
        cout << "destIndex: " << destIndex << endl;
    }



    //  PROC: Loop through each elemeent that will end up in the desination arr
    //  and place it in the destination array
    for(int i = destIndex; i < sourceSize; i++)
    {
        destArr[index] = sourceArr[i];
        sourceArr[i] = T();

        index++;

    }

    if(DEBUG)
    {
        cout <<
                "Source array: ";
        print_array(sourceArr, sourceSize);

        cout << "\nDestination array: ";
        print_array(destArr, destSize);
    }

    if(DEBUG)
    {
        cout << "\nEnd of split()\n";
        cout << "\n---------------------------\n";
    }




}





#endif // ARRAY_FUNCTIONS_H
