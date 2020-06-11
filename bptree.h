#ifndef BPTREE_H
#define BPTREE_H


#include "array_functions.h"
#include "vector.h"

//  IMPORTANT: This was last updated 05/25/20. Make sure that this is
//  the latest version (Check your Github)

const int MIN = 1;
const int MAX = MIN * 2;
const int MAX_DATA = MAX + 1;
const int MAX_SUBSET = MAX + 2;

template <class T>
class bptree
{
private:

    void takeLeftSubsetData(unsigned int index);
    void takeRightSubsetData(unsigned int index);
    void mergeLeftSubset(unsigned int index);
    void mergeRightSubset(unsigned int index);

    bool loose_erase(T item);

    void remove_biggest(T &removed_entry);


    void fix_shortage(unsigned int index);

    bool ignore_duplicates;                 //  CALC: If set to true will not
                                            //  add duplicates to the tree

    bptree<T> *next;

    bool is_size_valid();
    bool are_entries_ordered();
    bool is_breadcrumb_in_leaf(T breadcrumb);

    int sz;




public:

    class Iterator
    {
        private:
            bptree<T> *tree;

            unsigned int keyPtr;

            T & get() const
            {

                if(DEBUG)
                {
                    cout << "\n\n------------------------";
                    cout << "\nCalling get() const\n\n";
                    cout << "keyPtr: " << keyPtr << endl;
                    cout << "tree: ";
                    tree->print_debug();
                }

                assert(keyPtr < tree->data_count);

                return tree->data[keyPtr];
            }

            T & get()
            {

                if(DEBUG)
                {
                    cout << "\n\n------------------------";
                    cout << "\nCalling get()\n\n";
                    cout << "keyPtr: " << keyPtr << endl;
                    cout << "tree: ";
                    tree->print_debug();
                }

                assert(keyPtr < tree->data_count || tree->data_count == 0);
                return tree->data[keyPtr];
            }



        public:


            void copy(const Iterator &source)
            {
                tree = source.tree;
                keyPtr = source.keyPtr;
            }

            Iterator()
            {

                tree = nullptr;

                keyPtr = 0;
            }

            Iterator(bptree<T> &tree)
            {
                this->tree = &tree;

                keyPtr = 0;
            }

            Iterator(const bptree<T> *tree)
            {


                this->tree = const_cast<bptree<T>* > (tree);

                keyPtr = 0;
            }

            Iterator(bptree<T> *tree, unsigned int keyPtr)
            {

                assert(keyPtr < tree->data_count);

                this->tree = tree;

                this->keyPtr = keyPtr;
            }

            Iterator(const Iterator &source)
            {


                copy(source);
            }




            Iterator  & operator++();
            Iterator operator++(int);

            Iterator & operator =(const Iterator& rhs)
            {
                copy(rhs);

                return *this;
            }

            bool is_null()
            {

                //  PROC: If the tree is null then return true
                if(tree == nullptr)
                {
                    return true;
                }

                else
                {
                    return false;
                }
            }

            bool operator!=(const Iterator &rh)
            {
                if(tree != rh.tree || keyPtr != rh.keyPtr)
                {
                    return true;
                }

                else
                {
                    return false;
                }
            }


            T & operator*()
            {

                //  PROC: Print a debug on the tree if The iterator uses
                //  this operator with the DEBUG flag on
                if(DEBUG)
                {
                    *(*tree);
                }

                //  PROC: When the iterator is dereferenced. return a reference
                //  to the current data element that the iterator is pointing to
                return get();
            }


    };

    T data[MAX + 1];
    bptree<T> *subset[MAX_SUBSET];

    unsigned int child_count;
    unsigned int data_count;

    bool loose_insert(T item);
    bool is_leaf() const;

    void insert(T item);
    void erase(T item);

    Vector<T> to_vector() const;
    void test_tree(unsigned int level = 1);


    ~bptree();

    bptree(bool ignore_duplicates = true);

    bptree(T newData[], unsigned int dataSize, bptree<T> *newSubset[], unsigned int subsetSize, bool ignore_duplicates = true);
    bptree(const bptree<T> &source);



    void print(unsigned int level = 0);
    void print_debug(unsigned int level = 0);


//    void copy(bptree &newTree);

    void copy(const bptree<T> &source, bptree<T> *&last_node, unsigned int level = 0);

    void clear();

    int size() const;

    Iterator begin() const;
    Iterator end() const;

    T & get(const T& item);
    T & get_existing(const T& item);
    bool contains(const T item);

    void fix_excess(unsigned int index);
    int findSubsetIndex(T item);
    Iterator find(T item);

    Iterator find_upper_bound(T item);

    Iterator find_lower_bound(T item);

    bptree<T>& operator =(const bptree<T>& rhs);
    void operator *();
    bptree<T>& operator +=(const T &rhs);

    template <typename U>
    friend ostream& operator <<(ostream& outs, bptree<U>& tree);






};

template<typename T>
void bptree<T>::operator *()
{
    print_debug();
}

template<typename T>
bptree<T>& bptree<T>::operator +=(const T &rhs)
{
    insert(rhs);

    return *this;
}


template<typename T>
typename bptree<T>::Iterator & bptree<T>::Iterator::operator++()
{
    assert(keyPtr < tree->data_count || tree->data_count == 0);

    //  PROC: If the iterator is not pointing to a blank tree
    if(!is_null())
    {
        //  PROC: Increment the keyPtr
        keyPtr++;

        //  PROC: If the keyPtr has moved to a data element that
        //  is not in the node, then we reset it to 0 and move to
        //  the next node
        if(keyPtr == tree->data_count)
        {
            keyPtr = 0;
            tree = tree->next;
        }
    }

    return *this;
}

template<typename T>
typename bptree<T>::Iterator bptree<T>::Iterator::operator++(int)
{



    bptree<T>::Iterator ref = *this;
    this->operator++();
    return ref;
}



template <typename T>
typename bptree<T>::Iterator bptree<T>::begin() const
{





    if(DEBUG)
    {
        cout << "-----------------------";
        cout << "\n\nCalling begin()\n\n";
    }

    //  PROC: Check that the node that we are at is not blank
    if(data_count > 0)
    {
        //  PROC: If this is not a leaf, call this function on the first child
        //  of this tree (the left child)
        if(!is_leaf())
        {
            if(DEBUG)
            {

                cout << "\ncalling subset[0]->begin()\n";
            }
            return subset[0]->begin();
        }

        //  PROC: If this is a leaf, return the btree
        else
        {
            if(DEBUG)
            {

                cout << "\n\nReturning iterator pointing to this node\n\n";
                cout << "memory address: " << this << endl;
                cout << "data: ";
                print_array(data, data_count);
                cout << endl;

                cout << "data_count: " << data_count << endl;
                cout << "child_count: " << child_count << endl;
            }


            typename bptree<T>::Iterator it(this);

            return it;
        }
    }

    //  PROC: If the node is blank, return a blank iterator
    else
    {
        return typename bptree<T>::Iterator();
    }


}


template <typename T>
typename bptree<T>::Iterator bptree<T>::end() const
{


    if(DEBUG)
    {
        cout << "-----------------------";
        cout << "\n\nCalling end()\n\n";
    }

    return bptree<T>::Iterator();

}

template <typename T>
int bptree<T>::size() const
{
    return sz;
}

template <typename T>
bool bptree<T>::are_entries_ordered()
{

    T data;         //  CALC: Will temporarily hold the data so that
                    //  we can traverse to the next entry while still
                    //  having access to the old one.

    if(DEBUG)
    {
        cout << "\n\n----------------------------\n\n";
        cout << "Calling are_entries_ordered()\n\n";
    }


    //  PROC: Initialize Variables
    data = T();

    //  PROC: Iterate through all entries of the bptree
    for(bptree<T>::Iterator it = begin(); it != end(); ++it)
    {
        if(DEBUG)
        {
            cout << "are_entries_ordered() data:\n";
            cout << "iterator: " << *it << endl;
            cout << "data: " << data << endl;
            cout << endl;
        }

        //  PROC: If the current value is less than the previous, then return
        //  false;
        if(*it < data)
        {
            return false;
        }

        data = *it;
    }

    if(DEBUG)
    {
        cout << "End of are_entries_ordered()\n\n";
        cout << "\n\n----------------------------\n\n";
    }

    return true;

}

template <typename T>
bool bptree<T>::is_breadcrumb_in_leaf(T breadcrumb)
{
    unsigned int amount;     //  CALC: Holds the amount of times the breadcrumb
                             //  exists in the leaves

    //  PROC: Initialize the variable
    amount = 0;

    if(DEBUG)
    {
        cout << "\n\n----------------------------\n\n";
        cout << "Calling is_breadcrumb_in_leaf()\n\n";
    }

    //  PROC: Iterate through all entries of the bptree
    for(bptree<T>::Iterator it = begin(); it != end(); ++it)
    {
        //  PROC: If the breadcrumb is equal to the current entry that the
        //  iterator is on, then increment amount
        if(*it == breadcrumb)
        {
            amount++;
        }
    }

    if(DEBUG)
    {
        cout << "End of is_breadcrumb_in_leaf()\n\n";
        cout << "\n\n----------------------------\n\n";
    }

    //  PROC: If the amount is equal to 1, then we have found 1 entry in the leaves
    //  return true, or else return false
    if(amount == 1)
    {
        return true;
    }

    else
    {
        return false;
    }



}

template <typename T>
bool bptree<T>::is_size_valid()
{
    unsigned int size;      //  CALC: This will hold the size of each entry in
                            //  the bottom leaves that we will find by using
                            //  the iterator to move through the leaves
    bool valid;             //  CALC: Keeps track of whether the tree is valid


    if(DEBUG)
    {
        cout << "\n\n----------------------------\n\n";
        cout << "Calling is_size_valid()\n\n";
    }

    //  PROC: Initialize Variables
    size = 0;

    //  PROC: Assume that the tree is valid until proven invalid
    valid = true;

    //  PROC: Iterate through the entries in the leaves (the actual data of the
    //  tree and increment size to keep track of how many entries exist)
    for(bptree<T>::Iterator it = begin(); it != end(); ++it)
    {
        if(DEBUG)
        {
            cout << endl << *it << endl;
        }

        size++;
    }

    //  PROC: If the size that we have calculated using the iterator is not
    //  the same as the size that has been calculated during inserts and erasures
    //  then we return false
    if(this->size() != size)
    {
        valid = false;
    }

    if(DEBUG)
    {
        cout << "End of is_size_valid()\n\n";
        cout << "\n\n----------------------------\n\n";
    }

    return valid;
}

/**********************************************************
*
* Class: bptree : void test_tree()
*_________________________________________________________
* This function will run tests on the tree to ensure that
* it is completely valid
*_________________________________________________________
* PRE-CONDITIONS
* list     :       The vector that will hold the sorted bptree
*
* POST-CONDITIONS
*   - If the tree is valid, nothing will happen. If it is not
*   valid, an assert statement will end the program.
*
***********************************************************/
template <typename T>
void bptree<T>::test_tree(unsigned int level)
{
    static size_t size;             //  CALC: Holds the current size of
                                        //  all of the counted entries
    static unsigned int maxLevel;   //  CALC: Keeps track of the greatest
                                        //  level that the tree could reach


    if(DEBUG)
    {
        cout << "------------------------";
        cout << "\nCalling test_tree():\n";
        cout << "level: " << level << endl;
        cout << "data: ";
        print_array(data, data_count);
        cout << endl;
        cout << "subset: ";
        print_array(subset, child_count);
        cout << endl;
    }

    //  PROC: If we are at the root, lets run some preliminary tests.
    if(level == 1)
    {

        //  PROC: Check that the size of the tree is valid
        assert(is_size_valid());

        //  PROC: Are the entries ordered?
        assert(are_entries_ordered());



    }

    //  PROC: If we are not on the root node, make sure that the data_count
    //  is at the min but not above the max
    if(level > 1)
    {
        //  PROC: If data_count is greater than or equal to the minimum and less than
        //  or equal to the data_count
        assert(MIN <= data_count && MAX >= data_count);
    }


    //  PROC: IF the node has children
    if(!is_leaf())
    {
        //  PROC: The child_count must be equal to data_count + 1
        assert(child_count == data_count + 1);

        //  PROC: Loop through each data count. Count it by incrementing size
        //  and check that it is in sorted order
        for(unsigned int i = 0; i < data_count; i++)
        {


            //  PROC: Check that this breadcrumb exists in the leaves
            assert(is_breadcrumb_in_leaf(data[i]));

            //  PROC: Check that the subset exists before we dereference its
            //  pointer
            assert(subset[i] != nullptr);

            //  PROC: Check that the data elment is greater than that last
            //  element its left subset
            assert(data[i] > subset[i]->data[subset[i]->data_count - 1]);

            //  PROC: Check that the subset exists before we dereference its
            //  pointer
            assert(subset[i + 1] != nullptr);

            //  PROC: Check that the data element is less than or equal
            //  (remember it is okay for the right subtree's first element to be
            //  the same) to the first element of the right subset
            assert(data[i] <= subset[i + 1]->data[0]);



        }

        //  PROC: Loop through each child, calling test_tree() on those
        //  children
        for(unsigned int i = 0; i < child_count; i++)
        {
            //  PROC: Check that the subset exists before we dereference its
            //  pointer
            assert(subset[i] != nullptr);

            //  PROC: call test_tree() on the child with a level + 1 higher
            //  than the current level
            subset[i]->test_tree(level + 1);
        }

    }

    //  PROC: If the node is a leaf
    else
    {
        //  PROC: If maxLevel = 0 then it has not been initiated yet. Initiate
        //  it.
        if(maxLevel == 0)
        {
            maxLevel = level;
        }

        //  PROC: If it is not 0, then it has been iniitated. Assert that the
        //  current level is equal to max Level. (The tree cannot be unbalanced).
        else
        {
            assert(level == maxLevel);
        }
    }

    //  PROC: Add the data_count of this node to the overall size of the tree
    size += data_count;



    if(DEBUG)
    {
        if(level == 1)
        {
            cout << "\nThis is the final metrics for the tree:\n";
        }
        cout << "\nsize: " << size << endl;
        cout << "maxLevel: " << maxLevel << endl;

        cout << "End of test_tree()\n";
        cout << "---------------------\n\n";
    }


    //  PROC: If the function has returned to the root node, reset the
    //  static variables
    if(level == 1)
    {

        size = 0;
        maxLevel = 0;
    }

}

/**********************************************************
*
* Class: bptree : void to_vector()
*_________________________________________________________
* This function will convert the bptree into a sorted vector.
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - A vector with all of the values in the tree sorted will
*   be returned
*
***********************************************************/
template <typename T>
Vector<T> bptree<T>::to_vector() const
{
    Vector<T> list;     //  CALC: The Vector that will hold all of the values
                        //  from the tree


    for(bptree<T>::Iterator it = begin(); it != end(); ++it)
    {
        list.push_back(*it);
    }

    return list;


//    if(DEBUG)
//    {
//        cout << "-----------------------";
//        cout << "\n\nCalling to_vector()\n\n";
//    }

//    //  PROC: If this is not a leaf, call this function on the first child
//    //  of this tree (the left child)
//    if(!is_leaf())
//    {
//        if(DEBUG)
//        {

//            cout << "\ncalling subset[0]->begin()\n";
//        }
//        return subset[0]->begin();
//    }

//    //  PROC: If this is a leaf, return the btree
//    else
//    {
//        if(DEBUG)
//        {

//            cout << "\n\nReturning iterator pointing to this node\n\n";
//            cout << "memory address: " << this << endl;
//            cout << "data: ";
//            print_array(data, data_count);
//            cout << endl;

//            cout << "data_count: " << data_count << endl;
//            cout << "child_count: " << child_count << endl;
//        }

//        bptree<T> *tree = this;
//        unsigned keyPtr = 0;

//        while(tree != nullptr)
//        {

//        }

//    }


}



/**********************************************************
*
* Class: bptree : operatr =()
*_________________________________________________________
* This overloads the assignment operator so that it makes
* a deepy copy of the tree.
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - It will deep copy the tree from the right hand side to
*   the left
*
***********************************************************/
template <typename T>
bptree<T>& bptree<T>::operator =(const bptree<T>& rhs)
{
    if(DEBUG)
    {
        cout << "\nAssignment operator called!\n";
    }

    bptree<T> *last_node = nullptr;

    copy(rhs, last_node);

    return *this;
}

/**********************************************************
*
* Class: bptree : function get(T item)
*_________________________________________________________
* This will return return a reference to the item
*_________________________________________________________
* PRE-CONDITIONS
* item      :       The item we are looking for
*
* POST-CONDITIONS
*   - It will return a reference to the spot where the item
*   is held in bptree
*
***********************************************************/
template <typename T>
T & bptree<T>::get(const T& item)
{
    //  PROC: If the tree contains the item, return the memory address of the
    //  item
    if(contains(item))
    {
        return get_existing(item);
    }

    //  PROC: If the tree does not cotain the item, insert the item
    else
    {
        insert(item);
        return get_existing(item);
    }
}

/**********************************************************
*
* Class: bptree : function get_existing(T item)
*_________________________________________________________
* This will return return a reference to the item
*_________________________________________________________
* PRE-CONDITIONS
* item      :       The item we are looking for
*
* POST-CONDITIONS
*   - It will return ture if the item is in the bptree
*   and false if it is not
*
*
***********************************************************/
template <typename T>
T & bptree<T>::get_existing(const T& item)
{

    //  PROC: Return the memory address of the item
    return *find(item);

}

/**********************************************************
*
* Class: bptree : function contains(T item)
*_________________________________________________________
* This will check if the bptree contains the item
*_________________________________________________________
* PRE-CONDITIONS
* item      :       The item we are looking for
*
* POST-CONDITIONS
*   - It will return ture if the item is in the bptree
*   and false if it is not
*
*
***********************************************************/
template <typename T>
bool bptree<T>::contains(T item)
{
    //  PROC: If find does not return nullptr (it returns a reference) then
    //  the tree does contain the item
    if(find(item) != bptree<T>::Iterator())
    {
        return true;
    }

    //  PROC: If it does return a nullptr, then return false
    else
    {
        return false;
    }

}


/**********************************************************
*
* Class: bptree : Destructor ~bptree()
*_________________________________________________________
* This will deallocate all of the memory of the bptree.
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - A the bptree and its memory will be completely cleared.
*
*
***********************************************************/
template <typename T>
bptree<T>::~bptree()
{
    if(DEBUG)
    {
        cout << "\n\nCalling the destructor for the bptree\n";
    }

    clear();
}

/**********************************************************
*
* Class: bptree : function clear()
*_________________________________________________________
* This function will clear the bptree (deep delete).
* It will go into each subptree and clear it as well.
* It will deallocate mememory as well.
*
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - A the bptree and its memory will be completely cleared.
*
*
***********************************************************/
template <typename T>
void bptree<T>::clear()
{
    //  PROC: Loop through each child of the node. Call clear on it, then delete
    //  it
    for(int i = 0; i < child_count; i++)
    {
        subset[i]->clear();
        delete subset[i];
        subset[i] = nullptr;
    }

    //  PROC: Set the child_count to 0, so that when the node is deleted and
    //  the clear() function is called again on it, it doesn't try to delete
    //  its children for the second time. (This will result in a dereferencing
    //  null pointer error)
    child_count = 0;
    data_count = 0;
    sz = 0;


}

/**********************************************************
*
* Class: bptree : function copy(bptree &newTree)
*_________________________________________________________
* This function will copy the bptree (deep copy)
*_________________________________________________________
* PRE-CONDITIONS
* &newTree      :       A blank tree that will have this tree
*                       copied to it
*
* POST-CONDITIONS
*   - A new bptree will be created from this bptree (passed in by reference)
*
*
***********************************************************/
//template <typename T>
//void bptree<T>::copy(bptree &newTree)
//{

//    bptree *temp;            //  CALC: This will hold the memory address for
//                            //  each new subset tree that we are making. This
//                            //  memory address will then be passed to the same
//                            //  subptree of the source node so that we can make
//                            //  a copy to it

//    //  PROC: Initialize variables
//    temp = nullptr;

//    //  PROC: Store the data count and the child count in the new tree
//    newTree.data_count = data_count;
//    newTree.child_count = child_count;

//    //  PROC: Copy the data array to the new tree
//    copy_array(data, newTree.data, data_count);

//    //  PROC: Copy the subset to the newTree (IMPORTANT: This needs to be
//    //  a deep copy because we are dealing with dynamic memory)
//    //  Loop through each subset, make a new bptree, attach the new bptree
//    //  to the newTree, then call the copy method of subset of the source tree
//    //  with the new bptree as a parameter
//    for(unsigned int i = 0; i < child_count; i++)
//    {
//        temp = nullptr;

//        //  PROC: Create a new tree
//        temp = new bptree<T>(ignore_duplicates);

//        //  PROC: Attach this new tree to newTree's subset
//        newTree.subset[i] = temp;

//        //  PROC: Make a call to the source's subptree with this new subptree's
//        //  memory address
//        subset[i]->copy(*temp);

//    }


//}

/**********************************************************
*
* Class: btree : function copy(btree &source)
*_________________________________________________________
* This function will copy the btree (deep copy)
*_________________________________________________________
* PRE-CONDITIONS
* &source      :       The source tree that we are copying from
*
* POST-CONDITIONS
*   - A new btree will be created from this btree (passed in by reference)
*
*
***********************************************************/
template <typename T>
void bptree<T>::copy(const bptree &source, bptree<T> *&last_node, unsigned int level)
{

    bptree<T> *temp;        //  CALC: This will hold the memory address for
                            //  each new subset tree that we are making. This
                            //  memory address will then be passed to the same
                            //  subtree of the source node so that we can make
                            //  a copy to it

    if(DEBUG)
    {
        cout << "----------------------";
        cout << "\n\nCalling copy()\n\n";
        cout << "level: " << level << endl;
        cout << "size: " << sz << endl;
        cout << "data: ";
        print_array(data, data_count);
        cout << "\ndata_count: " << data_count << endl;
        cout << "child_count: " << child_count << endl;
        cout << "source data: ";
        print_array(source.data, source.data_count);
        cout << "\nsource data_count: " << source.data_count << endl;
        cout << "source child_count: " << source.child_count << endl;
    }

    //  PROC: Initialize variables
    temp = nullptr;

    //  PROC: Store the data count, child_count, ignore_duplicates, and size
    //  into this new tree
    data_count = source.data_count;
    child_count = source.child_count;
    ignore_duplicates = source.ignore_duplicates;
    sz = source.sz;

    //  PROC: Copy the data array from source to this tree
    copy_array(source.data, data, source.data_count);

    //  PROC: Copy the subset from the source (IMPORTANT: This needs to be
    //  a deep copy because we are dealing with dynamic memory)
    //  Loop through each subset, make a new btree, attach the new btree
    //  to the this tree, then call the copy on this new branch with the
    //  source's subset as the parameter
    for(unsigned int i = 0; i < source.child_count; i++)
    {
        temp = nullptr;

        //  PROC: Create a new tree
        temp = new bptree<T>(source.ignore_duplicates);

        //  PROC: Attach this new tree to this tree's subset
        subset[i] = temp;

        //  PROC: Make a call to copy of this new subtree and use the source's
        //  subset as the parameter
        subset[i]->copy(*source.subset[i], last_node, level + 1);


    }

    //  PROC: If we are on a leaf
    if(is_leaf())
    {
        //  PROC: If the last_node is not nullptr (we are not on the first node)
        //  then we point to this current node
        if(last_node != nullptr)
        {
            last_node->next = this;
        }

        //  PROC: Insert this node's memory address into the last_node pointer
        last_node = this;
    }

    if(DEBUG)
    {
        cout << "----------------------";
        cout << "\n\nEnd of copy()\n\n";
        cout << "level: " << level << endl;
        cout << "size: " << sz << endl;
        cout << "data: ";
        print_array(data, data_count);
        cout << "\ndata_count: " << data_count << endl;
        cout << "child_count: " << child_count << endl;
        cout << "source data: ";
        print_array(source.data, source.data_count);
        cout << "\nsource data_count: " << source.data_count << endl;
        cout << "source child_count: " << source.child_count << endl;
    }


}

/**********************************************************
*
* Class: bptree : operator <<(ostream& outs, bptree<T>& tree)
*_________________________________________________________
* This overloaded operator will print out the bptree to
* the screen
*_________________________________________________________
* PRE-CONDITIONS
* level     :       The level within the recursion loop you are
*
* POST-CONDITIONS
*   - A bptree will be printed to the screen
*
*
***********************************************************/
template <typename T>
ostream& operator <<(ostream& outs, bptree<T>& tree)
{

    if(DEBUG)
    {
        cout << endl << endl;
        tree.print_debug();
    }

    tree.print();



    return outs;
}


/**********************************************************
*
* Class: bptree : function fix_excess(unsigned int level)
*_________________________________________________________
* This will fix the node with the excess entry.
*_________________________________________________________
* PRE-CONDITIONS
* level     :       The level within the recursion loop you are
*
* POST-CONDITIONS
*   - A copy of the excess (middle entry) will be moved to the parent.
*   The excess and and everything to the right of it will be placed into
*   a new node that will be to the right of the original node.
*   The original node will have everything to the left of the excess.
*   The original node (left node) will now be pointing to the right node
*   and the right node will be pointing to whatever the left node was pointing
*   to. This will create a linked list
*
*
***********************************************************/
template<typename T>
void bptree<T>::fix_excess(unsigned int index)
{


    //  PROC: Point child to the child whose excess we are fixing
    bptree *child = subset[index];
    bptree *rightChild = nullptr;

    assert(index < child_count);

    //  PROC: Only fix excess if the data_count is greater than the max
    //  and the child is not a leaf
    if(child->data_count > MAX && !child->is_leaf())
    {
        if(DEBUG)
        {
            cout << "\n\n----------------------------\n";
            cout << "\nInitiating Fix Excess:\n";
            cout << "Index: " << index << endl;
        }

        assert(index < child_count);



        //  PROC: Store the index of the excess item that we will be removing
        //  from the child and adding to the parent
        unsigned int excessItemIndex = child->data_count / 2;



        //  PROC: Define an array that will hold the 2nd half of the child's
        //  data that we are splitting
        T dataArr2[MAX + 1];

        //  PROC: Store the size that the 2nd half of the split array will be
        unsigned int dataArrSize2 = child->data_count / 2;

        //  PROC: Define an array that will hold the 2nd half of the child's
        //  subset that we are splitting
        bptree *subsetArr2[MAX + 2];

        //  PROC: Store the size that the 2nd half of the subset array will be
        unsigned int subsetArrSize2 = child->child_count / 2;

        if(DEBUG)
        {
            cout << "\n\nFix Excess() beginning of function:";
            cout << "\nexcessItemIndex: " << excessItemIndex << endl;
            cout << "dataArrSize2: " << dataArrSize2 << endl;
            cout << "subsettArrSize2: " << subsetArrSize2 << endl;
            cout << "data count : " << data_count << endl;
            cout << "child_count: " << child_count << endl;
        }



        //  PROC: Split the child's data array into two. The 1st half stays
        //  with the child and the 2nd half goes to dataArr2
        split(child->data, dataArr2, child->data_count);

        //  PROC: Split the child's subset array into two. The 1st half stays
        //  with the child and the 2nd half goes to subsetArr2
        split(child->subset, subsetArr2, child->child_count);



        //  PROC: Add the excess data item to the parent and increase the
        //  data_count
        insert_item(child->data[excessItemIndex], data, data_count);

        //  PROC: Remove the excess item from the child
        child->data[excessItemIndex] = T();


        //  PROC: Create a new node with the 2nd half of the child's data and
        //  subset. Place this node to the right of the original child.
        //  (array_insert() will move over all of the element to the right
        //  so that we can make room)
        insert_item_index(new bptree(dataArr2, dataArrSize2, subsetArr2, subsetArrSize2, ignore_duplicates), index + 1, subset, child_count);

        //  PROC: Modify the data count and child count for the original child
        //  since we have to successfully split it
        child->data_count = child->data_count / 2;
        child->child_count = (child->child_count / 2) + (child->child_count % 2);

        if(DEBUG)
        {
            cout << "\n\nFix Excess() end of function:";
            cout << "\nexcessItemIndex: " << excessItemIndex << endl;
            cout << "dataArrSize2: " << dataArrSize2 << endl;
            cout << "subsettArrSize2: " << subsetArrSize2 << endl;
            cout << "data count : " << data_count << endl;
            cout << "child_count: " << child_count << endl;
            cout << "\n\n----------------------------\n";
        }
    }

    //  PROC: If the data_count is greater than the max and the child is a leaf
    else if(child->data_count > MAX && child->is_leaf())
    {
        if(DEBUG)
        {
            cout << "\n\n----------------------------\n";
            cout << "\nInitiating Fix Excess (on a leaf):\n";
            cout << "Index: " << index << endl;
        }

        assert(child->child_count == 0);



        //  PROC: Store the index of the excess item that we will be removing
        //  from the child and adding to the parent
        unsigned int excessItemIndex = child->data_count / 2;



        //  PROC: Define an array that will hold the 2nd half of the child's
        //  data that we are splitting
        T dataArr2[MAX + 1];

        //  PROC: Store the size that the 2nd half of the split array will be
        //  after the initial split ( before we add the excess to it)
        unsigned int dataArrSize2 = child->data_count / 2;


        if(DEBUG)
        {
            cout << "\n\nFix Excess (on a leaf)() beginning of function:";
            cout << "\nexcessItemIndex: " << excessItemIndex << endl;
            cout << "dataArrSize2: " << dataArrSize2 << endl;
            cout << "data count : " << data_count << endl;
            cout << "child_count: " << child_count << endl;
        }



        //  PROC: Split the child's data array into two. The 1st half stays
        //  with the child and the 2nd half goes to dataArr2
        split(child->data, dataArr2, child->data_count);


        //  PROC: Create a new node with the 2nd half of the child's data and
        //  subset. Place this node to the right of the original child.
        //  (array_insert() will move over all of the element to the right
        //  so that we can make room)
        insert_item_index(new bptree(dataArr2, dataArrSize2, {}, 0, ignore_duplicates), index + 1, subset, child_count);

        //  PROC: Point rightChild to the newly created right child node
        rightChild = subset[index + 1];

        //  PROC: Add the excess data to the newly created right node
        insert_item(child->data[excessItemIndex], rightChild->data, rightChild->data_count);

        //  PROC: Add another copy of the excess data to the parent
        insert_item(child->data[excessItemIndex], data, data_count);

        //  PROC: Modify the data count for the original child
        //  since we have to successfully split it
        //  (The + 1 represents the excess that is still in the source)
        child->data_count = (child->data_count / 2) + 1;

        //  PROC: Remvoe the excess data from the original node
        remove_item(child->data[excessItemIndex], child->data, child->data_count);


        //  PROC: Point the right child's next (newly created node) to the
        //  next of the child node. Then point the child's next to the right
        //  child (the newly created node)
        rightChild->next = child->next;
        child->next = rightChild;



        if(DEBUG)
        {
            cout << "\n\nFix Excess() (for leaf nodes) end of function:";
            cout << "\nexcessItemIndex: " << excessItemIndex << endl;
            cout << "dataArrSize2: " << dataArrSize2 << endl;
            cout << "data count : " << data_count << endl;
            cout << "\n\n----------------------------\n";
        }
    }



}


/**********************************************************
*
* Class: bptree : function findSubsetIndex(T item)
*_________________________________________________________
* This will find the appropriate index that we must recursively
* enter to find a possible spot for the item (or find an item).
*_________________________________________________________
* PRE-CONDITIONS
* item      :       The item we are inserting or searching for
*
* POST-CONDITIONS
*   - Either -1 will be returned because we could not find a spot
*   or an appropriate index will be returned
*
*
***********************************************************/
template<typename T>
int bptree<T>::findSubsetIndex(T item)
{

    unsigned int index;       //  CALC: Used to traverse through the data array
    int foundIndex;           //  OUT: Holds the correct index once it is found
    bool found;               //  CALC: Keeps track of whether or not we have
                              //  found the index or not



    if(DEBUG)
    {
        cout << "\n\n----------------------------";
        cout << "\n\nCalling findSubsetIndex():\n";
        cout << "item: " << item << endl;
    }


    unsigned int it;



    //  PROC: Initialize Variable
    index = 0;
    foundIndex = -1;

    found = false;

    //  PROC: Continue looking for an index until we either run out of possible
    //  subsets or an index is found
    while(index < child_count && found == false)
    {

        if(DEBUG)
        {
            cout << "\ncurrent index: " << index << endl;
        }

        //  PROC: If the current item is less than the current data element
        //  then we have found an available index for it
        if(item < data[index])
        {
            if(DEBUG)
            {
                cout << item << " is less than " << data[index] << endl;
                cout << "Found!\n";
            }

            found = true;
            foundIndex = index;
        }

        //  PROC: If the item is equal to the data element then we go to
        //  its right subset
        else if(item == data[index])
        {
            if(DEBUG)
            {
                cout << item << " is equal to " << data[index] << endl;
                cout << "Found!\n";
            }

            found = true;

            //  PROC: Add + 1 since the data we are looking for will be in
            //  the subset to the right of the element that the item is equal
            //  to
            foundIndex = index + 1;
        }

        //  PROC: If we have reached the end of the data array, then we return
        //  this index
        else if(index + 1 == child_count)
        {
            if(DEBUG)
            {
                cout << item << " is greater than the last data element so we "
                                " go to the last possible subset" << endl;
                cout << "Found!\n";
            }

            found = true;
            foundIndex = index;
        }

        //  PROC: If we have not found the correct index, increment index
        else
        {
            index++;
        }
    }

    if(DEBUG)
    {
        cout << "----------------------------";
        cout << "foundIndex: " << foundIndex << endl;
        cout << "\n\nEnd of findSubsetIndex():\n";
        cout << "----------------------------\n";
    }


    return foundIndex;
}

/**********************************************************
*
* Class: bptree : function print_debug(unsigned int level)
*_________________________________________________________
* This will print out information about each node of the
* bptree
*_________________________________________________________
* PRE-CONDITIONS
* level     :       The level within the recursion loop you are
*
* POST-CONDITIONS
*   - Each node of the bptree's info will be printed to the screen
*
*
***********************************************************/
template<typename T>
void bptree<T>::print_debug(unsigned int level)
{
    cout << endl;

    //  PROC: Check if node is a leaf. If it is not then we are going to
    //  recursively print all of its subptrees
    if(!is_leaf())
    {
        //  PROC: For each child of the node
        for(int i = child_count - 1; i >= 0; i--)
        {
            //  PROC: If we are halfway through printing the node's children
            //  then we print the data from the node itself
            if(i == ((child_count / 2) - 1) || child_count == 1)
            {

                cout << "memory address: " << this << endl;
                cout << "data: ";
                for(int i = 0; i < data_count; i++)
                {
                    cout << data[i] << ", ";
                }

                cout << endl;

                cout << "subset: ";
                for(int i = 0; i < MAX + 2; i++)
                {
                    cout << subset[i] << ", ";
                }

                cout << endl;
                cout << "child_count: " << child_count << endl;
                cout << "data_count: " << data_count << endl;
                cout << "next: " << next << endl << endl;


            }

            //  PROC: Print another child
            subset[i]->print_debug(level + 1);

        }

        cout << endl;
    }

    //  PROC: If the node is a leaf, then simply print it
    else
    {
        cout << "memory address: " << this << endl;
        cout << "data: ";
        cout << "data_count: " << data_count << endl;
        for(int i = 0; i < data_count; i++)
        {
            cout << data[i] << ", ";
        }
        cout << endl;
        cout << "child_count: " << child_count << endl;
        cout << "data_count: " << data_count << endl;
        cout << "next: " << next << endl << endl;
    }
}

/**********************************************************
*
* Class: bptree : function print(unsigned int level)
*_________________________________________________________
* This will print out the bptree to the user's screen
*_________________________________________________________
* PRE-CONDITIONS
* level     :       The level within the recursion loop you are
*
* POST-CONDITIONS
*   - A bptree will be printed to the screen
*
*
***********************************************************/
template<typename T>
void bptree<T>::print(unsigned int level)
{

    cout << endl;

    //  PROC: Check if node is a leaf. If it is not then we are going to
    //  recursively print all of its subptrees
    if(!is_leaf())
    {
        //  PROC: For each child of the node
        for(int i = child_count - 1; i >= 0; i--)
        {
            //  PROC: If we are halfway through printing the node's children
            //  then we print the data from the node itself
            if(i == ((child_count / 2) - 1) || child_count == 1)
            {
                //  PROC: Create space to replicate going down the tree
                for(int i = 0; i < level * 4; i++)
                {
                    cout << " ";
                }
                cout << "|";
                print_array(data, data_count);
                cout << "|" << endl;
            }

            //  PROC: Print another child
            subset[i]->print(level + 1);

        }

        cout << endl;
    }

    //  PROC: If the node is a leaf, then simply print it
    else
    {
        //  PROC: Create space to replicate going down the tree
        for(int i = 0; i < level * 4; i++)
        {
            cout << " ";
        }
        cout << "|";
        print_array(data, data_count);
        cout << "|" << endl;
    }






}


/**********************************************************
*
* Class: bptree : Constructor bptree(bool ignore_duplicates)
*_________________________________________________________
* This constructor will contruct a blank bptree
*_________________________________________________________
* PRE-CONDITIONS
* ignore_duplicates     :   whether or not we will ignore duplicates
*
* POST-CONDITIONS
*   - A bptree will be generated with blank data and subset arrays
*
***********************************************************/
template <class T>
bptree<T>::bptree(bool ignore_duplicates)
{
    //  PROC: Initialize Variables
    child_count = 0;
    data_count = 0;
    sz = 0;
    next = nullptr;

    //  PROC: Set the parameter for ignore duplicates
    this->ignore_duplicates = ignore_duplicates;

    init_array(data, MAX_DATA);
    init_array(subset, MAX_SUBSET);

}

/**********************************************************
*
* Class: bptree : Constructor bptree(T newData[], unsigned int dataSize,
* bptree<T> *newSubset[], unsigned int subsetSize, bool ignore_duplicates)
*_________________________________________________________
* This contructor will create a branch of a tree with the specified
* data and subset pointers. (This will not make a deep copy).
*_________________________________________________________
* PRE-CONDITIONS
* newData[]             :   The data that we are passing into the tree
* dataSize              :   The size of newData
* *newSubset[]          :   An array of bptree pointers. The subset info we are
*                           passing
* subsetSize            :   The size of newSubset
* ignore_duplicates     :   whether or not we will ignore duplicates
*
* POST-CONDITIONS
*   - A bptree will be created be created with the data and the subset
*   pointers (shallow copy)
*
***********************************************************/
template <class T>
bptree<T>::bptree(T newData[], unsigned int dataSize, bptree<T> *newSubset[], unsigned int subsetSize, bool ignore_duplicates)
{
    //  PROC: Initialize Variables
    child_count = subsetSize;
    data_count = dataSize;
    sz = 0;
    next = nullptr;

    init_array(data, MAX_DATA);
    init_array(subset, MAX_SUBSET);

    //  PROC: Set the ignore duplicates bool
    this->ignore_duplicates = ignore_duplicates;

    //  PROC: Copy the data array and the subset array to the new bptree
    copy_array(newData, data, dataSize);
    copy_array(newSubset, subset, subsetSize);

}

template <class T>
bptree<T>::bptree(const bptree<T> &source)
{

    if(DEBUG)
    {
        cout << "\n\nCalling the copy constructor for bptree\n\n";
    }

    //  PROC: Initialize Variables
    child_count = 0;
    data_count = 0;
    sz = 0;
    next = nullptr;

    //  PROC: Set the parameter for ignore duplicates
    this->ignore_duplicates = ignore_duplicates;

    init_array(data, MAX_DATA);
    init_array(subset, MAX_SUBSET);

    bptree<T> *last_node = nullptr;
    //  PROC: Initialize Variables
    copy(source, last_node);
}

/**********************************************************
*
* Class: bptree : bool is_leaf()
*_________________________________________________________
* This function will notify the user if the node is a leaf
* or not.
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
*   - It will return true if it is a leaf and false if not
*
***********************************************************/
template <class T>
bool bptree<T>::is_leaf() const
{

    //  PROC: If the there are no children then the node is a leaf. If more
    //  than one child exists then it is not a leaf
    if(child_count == 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}

/**********************************************************
*
* Class: bptree : bool loose_insert()
*_________________________________________________________
* This function will insert the item into the tree, disregarding
* that the node that is being inserted into may have an excess.
*_________________________________________________________
* PRE-CONDITIONS
* item     :       The item that we are inserting
*
* POST-CONDITIONS
*   - The tree will have the item inserted, but it may not
*   be valid
*
***********************************************************/
template <class T>
bool bptree<T>::loose_insert(T item)
{
    int index;              //  CALC: Used to hold the index of the subset that
                            //  we will insert into if we can't insert into this
                            //  specific node
    unsigned int findIndex; //  CALC: The index that the find_array funciton
                            //  will hold the index of the found item in
    bool done;              //  CALC: Holds whether the insert was accomplished
                            //  or not

    //  PROC: Initialize Variables
    index = 0;
    findIndex = 0;
    done = false;

    if(DEBUG)
    {
        cout << "\n\n----------------------------\n";
        cout << "\n\ncalling loose_insert():\n";
        cout << "item: " << item << endl;
        cout << "data count : " << data_count << endl;
        cout << "child_count: " << child_count << endl;

    }


    assert(data_count <= MAX + 1);

    //  PROC: If we find that the item is already in the data array and
    //  we are at a leaf, then we update the item
    if(find_array(item, data, data_count, findIndex) && is_leaf())
    {

        //  PROC: If we are suppose to ignore duplicates return false
        if(ignore_duplicates)
        {
            return false;
        }

        //  PROC: If we are suppose to not ignore duplicates. Append the item
        //  to the element in the array
        else
        {
//            data[findIndex] += item;
            return true;
        }







    }

    //  PROC: If this current node is a leaf, then we must insert into it
    else if(is_leaf())
    {
        //  PROC: Increase the data count and then insert into the data array
        insert_item(item, data, data_count);
        return true;
    }

    else
    {
        //  PROC: Store the index of the subset that correlates with the subptree
        //  our item should end up in
        index = findSubsetIndex(item);




        //  PROC: Check that our index is greater than -1 (meaning that one
        //  was found)
        if(index > -1)
        {

            //  PROC: Make sure that the subset has a bptree in it before we
            //  try to make a method call
            assert(subset[index] != nullptr);

            //  PROC: Call loose_insert() on the subptree that the item belongs
            //  in.
            done = subset[index]->loose_insert(item);

            //  PROC: Fix the excess if there are too many data keys in the
            //  child node
            fix_excess(index);

            return done;

        }
    }

    if(DEBUG)
    {
        cout << "\n\nEnd of loose_insert():\n";
        cout << "----------------------------\n\n";
    }


}

/**********************************************************
*
* Class: bptree : Iterator find()
*_________________________________________________________
* This function will find an item and return a pointer to it
*_________________________________________________________
* PRE-CONDITIONS
* item     :       The item that we are searching for
*
* POST-CONDITIONS
*   - A pointer will be returned pointed to the the item's
*   memory address
*
***********************************************************/
template <class T>
typename bptree<T>::Iterator bptree<T>::find(T item)
{
    unsigned int itemIndex;         //  CALC: Will hold the index of the found item
                                    //  if found
    int subsetIndex;                //  CALC: Will hold the index of the subset
                                    //  that we will be accessing if we have to
                                    //  move to another branch

    //  PROC: Initialize Variables
    itemIndex = 0;
    subsetIndex = 0;

    if(DEBUG)
    {

        cout << "\n\nCalling find():\n";
    }



    //  PROC: If we can find the item in this node's data array
    //  and we are at a leaf, then we update the item
    if(find_array(item, data, data_count, itemIndex) && is_leaf())
    {
        return bptree<T>::Iterator(this, itemIndex);
    }

    //  PROC: If the node is a leaf then we end the search by returning nullptr
    else if(is_leaf())
    {
        return bptree<T>::Iterator();
    }

    //  PROC: Continue the search in the appropriate tree that the item
    //  could be in
    else
    {
        //  PROC: Store the index of the subset that correlates with the subptree
        //  our item would be in
        subsetIndex = findSubsetIndex(item);

        //  PROC: If an appropriate index was found then enter into it and
        //  return it's findings
        if(subsetIndex > -1)
        {

            //  PROC: Make sure that the subset has a bptree in it before we
            //  try to make a method call
            assert(subset[subsetIndex] != nullptr);

            return subset[subsetIndex]->find(item);

        }

    }

    if(DEBUG)
    {
        cout << "\n\nEnd of find():\n";
        cout << "-------------------------";
    }
}

/**********************************************************
*
* Class: bptree : Iterator find_lower_bound()
*_________________________________________________________
* This function will return an iterator to either the item
* itself, or if the item does not exist, the next item
*_________________________________________________________
* PRE-CONDITIONS
* item     :       The item that we are searching for
*
* POST-CONDITIONS
*   - A pointer will be returned pointed to either the item's
*   memory address or the next item's memory adress
*
***********************************************************/
template <class T>
typename bptree<T>::Iterator bptree<T>::find_lower_bound(T item)
{
    unsigned int itemIndex;         //  CALC: Will hold the index of the found item
                                    //  if found
    int subsetIndex;                //  CALC: Will hold the index of the subset
                                    //  that we will be accessing if we have to
                                    //  move to another branch

    //  PROC: Initialize Variables
    itemIndex = 0;
    subsetIndex = 0;

    if(DEBUG)
    {

        cout << "\n\nCalling find_lower_bound():\n";
    }



    //  PROC: If we are at a leaf
    if(is_leaf())
    {

        if(DEBUG)
        {
            cout << "\n\nWe are at a leaf.\n";
        }

        //  PROC: If we find the item in the leaf, return an iterator to it
        if(find_array(item, data, data_count, itemIndex))
        {
            if(DEBUG)
            {
                cout << "\n\nWe found the item in the leaf node!\n";
            }

            return bptree<T>::Iterator(this, itemIndex);
        }

        //  PROC: If we find an item that is greater than the one we are looking
        //  for, return an iterator to it
        if(find_array_greater(item, data, data_count, itemIndex))
        {


            if(DEBUG)
            {
                cout << "\n\nWe found the next greater item in the leaf node!\n";
            }
            return bptree<T>::Iterator(this, itemIndex);


        }

        //  PROC: If we could not find an item greater than it in that leaf node
        //  keep iterating until we do, or we reach the end
        else
        {

            if(DEBUG)
            {
                cout << "\n\nWe found an item that is less than the item "
                        " we are looking for. Keep iterating till we find"
                        " an item that is greater than the item we are looking"
                        " for\n";
            }

            bptree<T>::Iterator it = bptree<T>::Iterator(this, itemIndex);
            while(it != end() && *it < item)
            {
                it++;
            }

            return it;
        }
    }



    //  PROC: Continue the search in the appropriate tree that the item
    //  could be in
    else
    {
        //  PROC: Store the index of the subset that correlates with the subptree
        //  our item would be in
        subsetIndex = findSubsetIndex(item);

        //  PROC: If an appropriate index was found then enter into it and
        //  return it's findings
        if(subsetIndex > -1)
        {

            //  PROC: Make sure that the subset has a bptree in it before we
            //  try to make a method call
            assert(subset[subsetIndex] != nullptr);

            return subset[subsetIndex]->find_lower_bound(item);

        }

    }

    if(DEBUG)
    {
        cout << "\n\nEnd of find():\n";
        cout << "-------------------------";
    }
}


/**********************************************************
*
* Class: bptree : Iterator find_upper_bound()
*_________________________________________________________
* This function will return an iterator to the item after the
* item whether it exists or not
*_________________________________________________________
* PRE-CONDITIONS
* item     :       The item that we are searching for
*
* POST-CONDITIONS
*   - A pointer will be returned pointed to after the item
*   whether it exists or not
*
***********************************************************/
template <class T>
typename bptree<T>::Iterator bptree<T>::find_upper_bound(T item)
{
    unsigned int itemIndex;         //  CALC: Will hold the index of the found item
                                    //  if found
    int subsetIndex;                //  CALC: Will hold the index of the subset
                                    //  that we will be accessing if we have to
                                    //  move to another branch

    //  PROC: Initialize Variables
    itemIndex = 0;
    subsetIndex = 0;

    if(DEBUG)
    {

        cout << "\n\nCalling find_upper_bound():\n";
    }



    //  PROC: If we are at a leaf
    if(is_leaf())
    {

        if(DEBUG)
        {
            cout << "\n\nWe are at a leaf.\n";
        }

        //  PROC: If we find the item in the leaf, return an iterator to it
        if(find_array(item, data, data_count, itemIndex))
        {
            if(DEBUG)
            {
                cout << "\n\nWe found the item in the leaf node!\n";
            }

            bptree<T>::Iterator it = bptree<T>::Iterator(this, itemIndex);

            it++;

            return it;

        }

        //  PROC: If we find an item that is greater than the one we are looking
        //  for, return an iterator to it
        if(find_array_greater(item, data, data_count, itemIndex))
        {


            if(DEBUG)
            {
                cout << "\n\nWe found the next greater item in the leaf node!\n";
            }
            return bptree<T>::Iterator(this, itemIndex);


        }

        //  PROC: If we could not find an item greater than it in that leaf node
        //  keep iterating until we do, or we reach the end
        else
        {

            if(DEBUG)
            {
                cout << "\n\nWe found an item that is less than the item "
                        " we are looking for. Keep iterating till we find"
                        " an item that is greater than the item we are looking"
                        " for\n";
            }

            bptree<T>::Iterator it = bptree<T>::Iterator(this, itemIndex);
            while(it != end() && *it < item)
            {
                it++;
            }

            return it;
        }
    }



    //  PROC: Continue the search in the appropriate tree that the item
    //  could be in
    else
    {
        //  PROC: Store the index of the subset that correlates with the subptree
        //  our item would be in
        subsetIndex = findSubsetIndex(item);

        //  PROC: If an appropriate index was found then enter into it and
        //  return it's findings
        if(subsetIndex > -1)
        {

            //  PROC: Make sure that the subset has a bptree in it before we
            //  try to make a method call
            assert(subset[subsetIndex] != nullptr);

            return subset[subsetIndex]->find_upper_bound(item);

        }

    }

    if(DEBUG)
    {
        cout << "\n\nEnd of find():\n";
        cout << "-------------------------";
    }
}

/**********************************************************
*
* Class: bptree : void insert()
*_________________________________________________________
* This function will insert an item into the tree
*_________________________________________________________
* PRE-CONDITIONS
* item     :       The item that we are inserting
*
* POST-CONDITIONS
*   - The tree will contain the new item and it will be completely
*   valid
*
***********************************************************/
template <class T>
void bptree<T>::insert(T item)
{
    bptree<T> *temp = nullptr;

    if(DEBUG)
    {
        cout << "\n\nCalling insert(1st)():\n";
        cout << "item: " << item << endl;
        cout << "data count : " << data_count << endl;
        cout << "child_count: " << child_count << endl;
//        cout << "data: ";
//        print_array(data, data_count);
//        cout << endl << "children: ";
//        print_array(subset, child_count);
//        cout << "\n";
    }

    //  PROC: Perform a loose insert on the tree. If successful, increase the
    //  size variable for the tree
    if(loose_insert(item))
    {

        if(DEBUG)
        {
            cout << "\n\nIncreasing size of tree (sz)\n\n";
            cout << "new size: " << sz << endl;
        }

        sz++;
    }




    //  PROC: If the root now has too many children, then we fix the excess.
    if(data_count > MAX)
    {
        temp = new bptree<T>(ignore_duplicates);

        //  PROC: Store the tree into a temporary tree
        copy_array(data, temp->data, MAX + 1);
        copy_array(subset, temp->subset, MAX + 2);
        temp->child_count = child_count;
        temp->data_count = data_count;

        //  PROC: Now clear the tree
        init_array(data, MAX + 1);
        init_array(subset, MAX_SUBSET);
        child_count = 0;
        data_count = 0;

        //  PROC: Now make the temporary tree the cleared node's child
        child_count++;
        subset[0] = temp;

        //  PROC: Now call fix excess
        fix_excess(0);

    }

    if(DEBUG)
    {
        cout << "\n\nEnd of insert():\n";
        cout << "item: " << item << endl;
        cout << "data count : " << data_count << endl;
        cout << "child_count: " << child_count << endl;
        cout << "data: ";
        print_array(data, data_count);
        cout << endl << "children: ";
        print_array(subset, child_count);
        cout << "\n";
    }

}

/**********************************************************
*
* Class: bptree : void takeLeftSubsetData()
*_________________________________________________________
* This function will take an extra data entry of the the left
* subset of the specified subset, move it up to the parent, and
* then take the parent's excess data entry (from the end of the data array
* and put it in the specified subset.
*_________________________________________________________
* PRE-CONDITIONS
* index     :       The index of the subset that is taking the excess
*                   entry from the left subset
*
* POST-CONDITIONS
*   - The parent will drop the data entry that is less than
*   the specified subset's data entries and it will insert it
*   into the specified subset. The excess entry from the left subset
*   will be removed and placed into the spot in the parent that was dropped.
*
***********************************************************/
template <class T>
void bptree<T>::takeLeftSubsetData(unsigned int index)
{

    assert(subset[index - 1] != nullptr);
    assert(subset[index] != nullptr);
    assert(index - 1 >= 0);

    bptree<T> *leftSubset;       //  CALC: Pointer that will point to the left
                                //  left subset


    leftSubset = subset[index - 1];


    if(DEBUG)
    {
        cout << "\n\nCalling takeLeftSubsetData():\n";
        cout << "Index: " << index << endl;
        cout << "child_count: " << child_count << endl;
        cout << "data_count: " << data_count << endl;

        cout << "data: ";
        print_array(subset[index]->data, subset[index]->data_count);
        cout << endl;

        cout << "children: ";
        print_array(subset[index]->subset, subset[index]->child_count);
        cout << endl;

        cout << "\nright-data: ";
        print_array(subset[index - 1]->data, subset[index]->data_count);
        cout << endl;

        cout << "\nright-children: ";
        print_array(subset[index - 1]->subset, subset[index]->child_count);
        cout << endl;
    }


    //  PROC: Add the parent's data entry (the one that is less than the subset)
    //  into the subset
    insert_item(data[index - 1], subset[index]->data, subset[index]->data_count);

    //  PROC: Remove the data entry from the parent that is less than the subset
    //
    remove_item(data[index - 1], data, data_count);

    //  PROC: Add the excess entry from the left subset into the parent's
    //  data (right where we removed the data entry)
    insert_item(leftSubset->data[leftSubset->data_count - 1], data, data_count);

    //  PROC: Remove the excess entry from the left subset
    remove_item(leftSubset->data[leftSubset->data_count - 1], leftSubset->data,
            leftSubset->data_count);


    //  PROC: If the left subset has children then we move over the child
    //  that corresponds with the excess data entry that we moved - over to
    //  the node with a shortage
    if(!leftSubset->is_leaf())
    {
        //  PROC: Take the child subset that comes after the excess entry from the
        //  left subset and insert it into the front of the children of the subset
        //  with the shortage
        insert_item_index(leftSubset->subset[leftSubset->child_count - 1], 0,
                subset[index]->subset, subset[index]->child_count);

        //  PROC: Remove the child subset that comes after the excess entry
        //  from the left subset
        remove_item(leftSubset->subset[leftSubset->child_count - 1],
                leftSubset->subset, leftSubset->child_count);


    }

    if(DEBUG)
    {
        cout << "data: ";
        print_array(subset[index]->data, subset[index]->data_count);
        cout << endl;

        cout << "children: ";
        print_array(subset[index]->subset, subset[index]->child_count);
        cout << endl;

        cout << "\nright-data: ";
        print_array(subset[index - 1]->data, subset[index]->data_count);
        cout << endl;

        cout << "\nright-children: ";
        print_array(subset[index - 1]->subset, subset[index]->child_count);
        cout << endl;

        cout << "\n\nEnd of takeLeftSubsetData()\n";
        cout << "------------------------------\n";
    }



}

/**********************************************************
*
* Class: bptree : void takeRightSubsetData()
*_________________________________________________________
* This function will take an extra data entry of the the right
* subset of the specified subset, move it up to the parent, and
* then take the parent's excess data entry (front the front of the data array)
* and put it in the specified subset (at the end).
*_________________________________________________________
* PRE-CONDITIONS
* index     :       The index of the subset that is taking the excess
*                   entry from the right subset
*
* POST-CONDITIONS
*   - The parent will drop the data entry that is greater than
*   the specified subset's data entries and it will insert it
*   into the specified subset. The excess entry from the right subset
*   will be removed and placed into the spot in the parent that was dropped.
*
***********************************************************/
template <class T>
void bptree<T>::takeRightSubsetData(unsigned int index)
{

    assert(subset[index + 1] != nullptr);
    assert(subset[index] != nullptr);
    assert(index + 1 < child_count);

    if(DEBUG)
    {
        cout << "data: ";
        print_array(subset[index]->data, subset[index]->data_count);
        cout << endl;

        cout << "children: ";
        print_array(subset[index]->subset, subset[index]->child_count);
        cout << endl;

        cout << "\nright-data: ";
        print_array(subset[index + 1]->data, subset[index]->data_count);
        cout << endl;

        cout << "\nright-children: ";
        print_array(subset[index + 1]->subset, subset[index]->child_count);
        cout << endl;
    }

    bptree<T> *rightSubset;       //  CALC: Pointer that will point to right
                                 //  subset


    rightSubset = subset[index + 1];


    //  PROC: Add the parent's data entry (the one that is greater than the subset)
    //  into the subset
    insert_item(data[index], subset[index]->data, subset[index]->data_count);

    //  PROC: Remove the data entry from the parent that is greater than the subset
    //
    remove_item(data[index], data, data_count);

    //  PROC: Add the excess entry from the right subset into the parent's
    //  data (right where we removed the data entry) - Take from the front
    //  of the data array
    insert_item(rightSubset->data[0], data, data_count);

    //  PROC: Remove the excess entry from the right subset
    remove_item(rightSubset->data[0], rightSubset->data,
            rightSubset->data_count);


    //  PROC: If the right subset has children then we move over the child
    //  that corresponds with the excess data entry that we moved - over to
    //  the node with a shortage
    if(!rightSubset->is_leaf())
    {
        //  PROC: Take the child subset that comes before the excess entry from the
        //  right subset and insert it into the end of the children of the subset
        //  with the shortage
        insert_item_index(rightSubset->subset[0], subset[index]->child_count,
                subset[index]->subset, subset[index]->child_count);

        //  PROC: Remove the child subset that comes before the excess entry
        //  from the right subset
        remove_item(rightSubset->subset[0], rightSubset->subset,
                rightSubset->child_count);


    }

    if(DEBUG)
    {
        cout << "data: ";
        print_array(subset[index]->data, subset[index]->data_count);
        cout << endl;

        cout << "children: ";
        print_array(subset[index]->subset, subset[index]->child_count);
        cout << endl;

        cout << "\nright-data: ";
        print_array(subset[index + 1]->data, subset[index]->data_count);
        cout << endl;

        cout << "\nright-children: ";
        print_array(subset[index + 1]->subset, subset[index]->child_count);
        cout << endl;

        cout << "\n\nEnd of takeRightSubsetData()\n";
        cout << "------------------------------\n";
    }



}

/**********************************************************
*
* Class: bptree : void mergeLeftSubset()
*_________________________________________________________
* This function will merge a specified subet with the subset
* that exists to the left of it.
*_________________________________________________________
* PRE-CONDITIONS
* index     :       The index of the subset that needs merging
*
* POST-CONDITIONS
*   - The subset on the left will have a new data entry from the parents data
*   set. It will pull the entry that is greater than it.
*   The subset will then be merged into the data set of the
*   short subset . The subsets to the right of it will be moved over to
*   take its place
*
***********************************************************/
template <class T>
void bptree<T>::mergeLeftSubset(unsigned int index)
{



    unsigned int leftIndex;    //  CALC: The index of the left subset
    bptree<T> *leftSubset;      //  CALC: A pointer to the left subset
    bptree<T> *shortSubset;     //  CALC: A pointer to the subset with a shortage



    //  PROC: Calculate the index of the left subset
    leftIndex = index - 1;



    //  PROC: Point leftSubset to the left Subset
    leftSubset = subset[leftIndex];

    assert(leftSubset != nullptr);

    //  PROC: Point subset to the subset that has a shortage
    shortSubset = subset[index];


    if(DEBUG)
    {
        cout << "\n\nCalling mergeLeftSubset():\n";
        cout << "Index: " << index << endl;
    }


    //  PROC: Insert the first data entry that corresponds with the index into
    //  the left subset's data array. This also updates the data_count
    insert_item(data[index - 1], leftSubset->data, leftSubset->data_count);

    //  PROC: Remove the data entry that corresponds with the index
    remove_item(data[index - 1], data, data_count);

    //  PROC: Merge the left data set with the subset with the shortage
    merge(shortSubset->data, shortSubset->data_count, leftSubset->data, MAX + 1,
          leftSubset->data_count);
    merge(shortSubset->subset, shortSubset->child_count, leftSubset->subset,
          MAX_SUBSET, leftSubset->child_count);


//  This was possibly here before because my remove_item didn't exist.
//    subset[index]->clear();

    //  PROC: Remove the leftSubset (moves over everything to the right of it)
    remove_item(subset[index], subset, child_count);

    if(DEBUG)
    {
        cout << "children: ";
        print_array(subset, child_count);
        cout << "\n\nEnd of mergeLeftSubset():\n";
        cout << "------------------------------\n";
    }


}

/**********************************************************
*
* Class: bptree : void mergeRightSubset()
*_________________________________________________________
* This function will merge a specified subset with the subset
* that exists to the right of it.
*_________________________________________________________
* PRE-CONDITIONS
* index     :       The index of the subset that needs merging
*
* POST-CONDITIONS
*   - The subset on the right will have a new data entry from the beginning
*   of its parent data set. It will then be merge into the data set that is
*   short. The subsets to the right of it will be moved over to take its place
*
***********************************************************/
template <class T>
void bptree<T>::mergeRightSubset(unsigned int index)
{
    unsigned int rightIndex;    //  CALC: The index of the right subset
    bptree<T> *rightSubset;      //  CALC: A pointer to the right subset
    bptree<T> *shortSubset;      //  CALC: A pointer to the subset with a shortage



    //  PROC: Calculate the index of the right subset
    rightIndex = index + 1;



    //  PROC: Point rightSubset to the right Subset
    rightSubset = subset[rightIndex];

    assert(rightSubset != nullptr);

    //  PROC: Point subset to the subset that has a shortage
    shortSubset = subset[index];


    if(DEBUG)
    {
        cout << "\n\nCalling mergeRightSubset():\n";
        cout << "Index: " << index << endl;
        cout << "shortSubset: " << shortSubset << endl;
        cout << "rightSubset: " << rightSubset << endl;

    }


    //  PROC: Insert the first data entry that corresponds with the index into
    //  the right subset's data array. This also updates the data_count
    insert_item(data[index], rightSubset->data, rightSubset->data_count);

    //  PROC: Remove the data entry that corresponds with the index
    remove_item(data[index], data, data_count);



    //  PROC: Merge the right data set with the subset with the shortage
    merge(rightSubset->data, rightSubset->data_count, shortSubset->data, MAX + 1, shortSubset->data_count);
    merge(rightSubset->subset, rightSubset->child_count, shortSubset->subset, MAX_SUBSET, shortSubset->child_count);




    //  PROC: Remove the rightSubset (moves over everything to the right of it)
    remove_item(rightSubset, subset, child_count);

    if(DEBUG)
    {
        cout << "\nEnd of mergeRightSubset():\n";
        cout << "------------------------------\n";
    }


}

/**********************************************************
*
* Class: bptree : void remove_biggest()
*_________________________________________________________
* This function will find the biggest data entry in a subptree,
* it will remove it, and return it by reference.
*_________________________________________________________
* PRE-CONDITIONS
* removed_entry     :   The reference variable that will contain the
*                       removed entry
*
* POST-CONDITIONS
*   - The subptree will have its greatest data entry removed and
*   placed in the reference variable. The data count will be reduced
*   for that far right child node that the entry is being removed from.
*
***********************************************************/
template <class T>
void bptree<T>::remove_biggest(T &removed_entry)
{

    if(DEBUG)
    {
        cout << "\n\n------------------------------\n";
        cout << "Calling remove_biggest():\n";
        cout << "data: ";
        print_array(data, data_count);
        cout << "\nchildren: ";
        print_array(subset, child_count);
        cout << "\ndata_count: " << data_count << endl;
        cout << "child_count: " << child_count << endl;

    }

    //  PROC: If this is not a leaf, then enter into the right most child
    if(!is_leaf())
    {
        //  PROC: Go into the right most child and get the biggest
        subset[child_count - 1]->remove_biggest(removed_entry);

        //  PROC: Fix a possible shortage
        fix_shortage(child_count - 1);
    }

    //  PROC: If we are in a leaf, then we get the greatest data entry.
    else
    {
        //  PROC: Store the greatest data entry into removed_entry
        removed_entry = data[data_count - 1];

        //  PROC: Set the greatest data entry to null
        data[data_count - 1] = T();

        //  PROC: Decrease the data count
        data_count--;
    }

    if(DEBUG)
    {
        cout << "\n\nEnd of remove_biggest():\n";
        cout << "------------------------------\n";
    }


}


template <class T>
bool bptree<T>::loose_erase(T item)
{

    int index;               //  CALC: Used to hold the index of the subset that
                             //  we will try to erase from if we can't erase
                             //  from this one

    unsigned int dataIndex;  //  CALC: This will hold the index of the data
                             //  entry if we find one

    bool done;               //  CALC: Holds whether the erase was accomplished
                             //  or not

    //  PROC: Initialize Variables
    index = 0;
    dataIndex = 0;
    done = false;

    if(DEBUG)
    {
        cout << "\n\nCalling loose_erase():\n\n";
        cout << "item: " << item << endl;
    }

    //  PROC: The tree must be intact.
    //    assert(data_count >= MIN);

    //  PROC: If we find the item in the data array, then we remove it
    if(find_array(item, data, data_count, dataIndex))
    {
        //  PROC: If the node is not a leaf and a child exists
        if(!is_leaf())
        {
            //  PROC: Remove the greatest data entry from the left subptree and
            //  then store it in that target.
            subset[dataIndex]->remove_biggest(data[dataIndex]);
            fix_shortage(dataIndex);



        }

        else
        {
            remove_item(item, data, data_count);
        }

        //  PROC: Return true since we removed item from the data
        return true;

    }

    //  PROC: If this current node is a leaf, and we couldn't find the data
    //  then we return false;
    else if(is_leaf())
    {
        return false;
    }

    else
    {
        //  PROC: Store the index of the subset that correlates with the subptree
        //  our item might be in
        index = findSubsetIndex(item);

        if(DEBUG)
        {
            cout << "Subset Index: " << index << endl;
        }

        //  PROC: Check that our index is greater than -1 (meaning that one
        //  was found)
        if(index > -1)
        {

            //  PROC: Make sure that the subset has a bptree in it before we
            //  try to make a method call
            assert(subset[index] != nullptr);

            //  PROC: Call loose_erase() on the subptree that the item belongs
            //  in.
            done = subset[index]->loose_erase(item);

            //  PROC: Fix the excess if there are not enough data keys in the
            //  child node
            fix_shortage(index);

            return done;

        }
    }

    if(DEBUG)
    {
        cout << "\nend of loose erase():\n";
        cout << "item: " << item << endl;
        cout << "------------------------------\n";
    }

}

/**********************************************************
*
* Class: bptree : void fix_shortage()
*_________________________________________________________
* This function will fix a shortage on a node that has one less
* data entry than the Minimum that is defined.
*_________________________________________________________
* PRE-CONDITIONS
* index     :       The index of the subset that needs its shortage fixed
*
* POST-CONDITIONS
*   - The subset will now have at least a minimum of data entries
*
***********************************************************/
template <class T>
void bptree<T>::fix_shortage(unsigned int index)
{



    if(DEBUG)
    {
        cout << "\n\nCalling fix_shortage():\n";
        cout << "index: " << index << endl << endl;
        cout << "data_count: " << data_count << endl;
        cout << "child_count: " << child_count << endl;

    }

    assert(index < child_count);

    //  PROC: If the subset's data_count is less than the minimum
    if(subset[index]->data_count < MIN)
    {
        //  PROC: If a left subset exists and it has a data entry to spare
        if(index > 0 && subset[index - 1]->data_count > MIN)
        {
            assert(subset[index - 1] != nullptr);
            takeLeftSubsetData(index);
        }

        //  PROC: If the right subset exists and it has a data entry to spare
        else if(index + 1 < child_count && subset[index + 1]->data_count > MIN)
        {
            assert(subset[index + 1] != nullptr);
            takeRightSubsetData(index);
        }

        //  PROC: If a left subset exists then we merge with the left subset
        else if(index > 0 && subset[index - 1] != nullptr)
        {
            mergeLeftSubset(index);
        }

        //  PROC: If a right subset exists then we merge with the right subset
        else if(index + 1 < child_count && subset[index + 1] != nullptr)
        {
            mergeRightSubset(index);
        }
    }

    if(DEBUG)
    {
        cout << "\nEnd of fix_shortage():\n";
        cout << "------------------------------\n";
    }





}

template <class T>
void bptree<T>::erase(T item)
{
    if(DEBUG)
    {
        cout << "\n\nCalling erase():\n\n";
        cout << "item: " << item << endl;
    }

    //  PROC: Perform a loose erase on the tree. If successful, decrement
    //  the size variable for the tree
    if(loose_erase(item))
    {
        sz--;
    }

    //  PROC: If the root is empty and there is only one child,
    //  then we copy the contents of the child into the root, essentially
    //  removing the root.
    if(data_count == 0 && child_count == 1)
    {
        //  PROC: Store the data_count and child_count of the child
        //  into the root
        data_count = subset[0]->data_count;
        child_count = subset[0]->child_count;

        //  PROC: Copy the data from the child into the root
        //  Copy the subset from the child into the root
        copy_array(subset[0]->data, data, data_count);
        copy_array(subset[0]->subset, subset, child_count);
    }

    if(DEBUG)
    {
        cout << "\nEnd of erase\n";
        cout << "------------------------------\n";
    }

}

template <typename T>
void remove_duplicates(Vector<T> &v)
{
    for(int i = 0; i < v.size(); i++)
    {
        for(int h = i + 1; h < v.size(); h++)
        {
            if(v.at(i) == v.at(h))
            {
                v.erase(v.begin() + h);
            }
        }
    }
}

template <typename T>
void vector_erase(Vector<T> &v, T target)
{
    for(int i = 0; i < v.size(); i++)
    {
        if(v.at(i) == target)
        {
            v.erase(v.begin() + i);
        }
    }
}

template <typename T>
bool compare_tree_to_list(bptree<T> *tree, T entry, bool insert)
{
    Vector<T> list;
    static Vector<T> checklist;

    list = tree->to_vector();

    if(insert)
    {
        checklist.insert(checklist.end(), entry);
        sort(checklist.begin(), checklist.end());
        remove_duplicates(checklist);
    }
    else
    {
        vector_erase(checklist, entry);
    }

    if(DEBUG)
    {
        cout << endl << endl;
        for(int i = 0; i < list.size(); i++)
        {
            cout << list.at(i) << ", ";
        }

        cout << endl << endl;

        for(int i = 0; i < checklist.size(); i++)
        {
            cout << checklist.at(i) << ", ";
        }
    }



    for(int i = 0; i < list.size(); i++)
    {
        if(list.at(i) != checklist.at(i))
        {
            return false;
        }
    }

    return true;
}





#endif // BPTREE_H
