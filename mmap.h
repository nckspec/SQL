#ifndef MMAP_H
#define MMAP_H

//  IMPORTANT: This was last updated 05/25/20. Make sure that this is
//  the latest version (Check your Github)

#include "bptree.h"
#include "vector.h"
#include "array_functions.h"
#include <iomanip>

const bool MMAP_DEBUG = false;




template<typename K, typename V>
struct MultiPair
{



    typedef bptree<V> values_base;

    K key;                  //  CALC: The key. Which is the link to the values

    values_base values;        //  CALC: A tree that will hold the values


    MultiPair();

    MultiPair(K key);





    /**********************************************************
    *
    * Class: MultiPair : void gatValues()
    *_________________________________________________________
    * This function will return the items stored in values as
    * an ordered vector.
    *_________________________________________________________
    * PRE-CONDITIONS
    *
    * POST-CONDITIONS
    *   - A vector will be returned with the values from the tree
    *
    ***********************************************************/
    Vector<V> getValues() const
    {

        if(DEBUG)
        {
            cout << "\n\ngetValues():\n\n";

            cout << values.to_vector().to_string() << endl << endl;
        }

        return values.to_vector();




    }

    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                  const MultiPair<K, V> &pair)
    {
        out << "[" << pair.key << ": ";
        cout << pair.getValues().to_string();
        cout << "]";
        return out;
    }


    bool operator <(MultiPair<K, V>& rh)
    {
        //  PROC: If the key on the left is less than the key on the right
        //  return true
        if(key < rh.key)
        {
            return true;
        }

        //  PROC: If the key on the left is greater than or equal to the
        //  key on the right, return false
        else
        {
            return false;
        }
    }



    bool operator ==(MultiPair<K, V>& rh)
    {
        if(key == rh.key)
        {
            return true;
        }

        else
        {
            return false;
        }
    }

    bool operator !=(MultiPair<K, V>& rh)
    {
        if(key != rh.key)
        {
            return true;
        }

        else
        {
            return false;
        }
    }

};

template<typename K, typename V>
MultiPair<K, V>::MultiPair()
{
    key = K();

}

template<typename K, typename V>
MultiPair<K, V>::MultiPair(K key)
{
    this->key = key;

}

template<typename K, typename V>
class mmap
{



public:

    typedef bptree<MultiPair<K, V> > map_base;


    class Iterator{
        public:
        Iterator()
        {
            if(MMAP_DEBUG)
            {
                cout << "\n\nCalling Constructor for map::Iterator()\n\n";

            }
        }

        Iterator(const mmap<K,V> &m)
        {
            if(MMAP_DEBUG)
            {
                cout << "\n\nCalling Constructor for map::Iterator(const map<K,V> m)\n\n";

            }

            _it = m.tree.begin();
        }
        Iterator(typename map_base::Iterator it)
        {
            if(MMAP_DEBUG)
            {
                cout << "\n\nCalling Constructor for map::"
                        "Iterator(typename map_base::Iterator it)\n\n";

            }

            _it = it;
        }



        Iterator & operator ++(int unused);
        Iterator & operator ++();
        MultiPair<K, V> & operator *();


        bool operator !=(const Iterator& rhs)
        {
            if(_it != rhs._it)
            {
                return true;
            }

            else {
                return false;
            }
        }

        private:
            typename map_base::Iterator _it;
        };


    //  ITERATORS
    mmap<K, V>::Iterator begin() const;
    mmap<K, V>::Iterator end() const;

    mmap<K, V>::Iterator upper_bound(K key);
    mmap<K, V>::Iterator lower_bound(K key);


    //  Mutators
    V & get(K entry);
    V & get(K entry) const;
    void insert(K entry);
    void erase(K key);
    void clear();

    //  OPERATORS
    mmap<K, V>& operator +=(const mmap<K, V> &rh);
    void operator *();
    template <typename A, typename B>
    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                  const mmap<A, B> &map);



    //  ACCESSORS
    bool contains(K key);
    void print() const;

    Iterator find(K key);
    int size() const;
    bool empty() const;


    //  ELEMENT ACCESS
    typename MultiPair<K, V>::values_base & operator[] (K index);
    typename MultiPair<K, V>::values_base & operator[] (K key) const;





private:
    map_base tree;


};

template<typename K, typename V>
typename mmap<K, V>::Iterator mmap<K, V>::begin() const
{
    return mmap<K, V>::Iterator(tree.begin());
}

template<typename K, typename V>
typename mmap<K, V>::Iterator mmap<K, V>::end() const
{
    return mmap<K, V>::Iterator(tree.end());
}

template<typename K, typename V>
typename mmap<K, V>::Iterator mmap<K, V>::upper_bound(K key)
{
    return typename mmap<K, V>::Iterator(tree.find_upper_bound(MultiPair<K,V>(key)));
}

template<typename K, typename V>
typename mmap<K, V>::Iterator mmap<K, V>::lower_bound(K key)
{
    return typename mmap<K, V>::Iterator(tree.find_lower_bound(MultiPair<K,V>(key)));
}

template<typename K, typename V>
typename mmap<K, V>::Iterator & mmap<K, V>::Iterator::operator++(int unused)
{
    _it++;

    return *this;
}

template<typename K, typename V>
typename mmap<K, V>::Iterator & mmap<K, V>::Iterator::operator++()
{
    _it++;

    return *this;
}




template<typename K, typename V>
MultiPair<K,V> & mmap<K, V>::Iterator::operator*()
{

    return *_it;
}

template<typename K, typename V>
void mmap<K, V>::operator *()
{
    *tree;
    cout << tree;

}

template<typename K, typename V>
int mmap<K, V>::size() const
{
    return tree.size();
}

template<typename K, typename V>
bool mmap<K, V>::empty() const
{
    if(size() == 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}

template<typename K, typename V>
void mmap<K, V>::clear()
{
    tree.clear();
}


template<typename K, typename V>
mmap<K, V>& mmap<K, V>::operator +=(const mmap<K, V> &rh)
{


    return *this;

}





template<typename K, typename V>
typename MultiPair<K, V>::values_base & mmap<K, V>::operator[] (K key) const
{
    if(DEBUG)
    {
        cout << "\n\nCalling operator[]\n\n";
    }

    return tree.get(MultiPair<K, V>(key)).values;
}

template<typename K, typename V>
typename MultiPair<K, V>::values_base & mmap<K, V>::operator[] (K key)
{
    if(DEBUG)
    {
        cout << "\n\nCalling operator[]\n\n";
    }

    return tree.get(MultiPair<K, V>(key)).values;
}


//template<typename K, typename V>
//V & mmap<K, V>::get(K key) const
//{

//    //  PROC: point mpair to the memory location of where we just stored
//    //  the new multi pair within the tree
//    MultiPair<K, V> *mpair = &tree.get(MultiPair<K, V>(key));

//    //  PROC: Insert a blank value into the multi pair's values Vector
//    mpair->values.push_back(V());

//    //  PROC: Return a reference to the blank value that was inserted into
//    //  the Vector
//    return mpair->values.at(mpair->values.size() - 1);


//}


template<typename K, typename V>
void mmap<K, V>::erase(K key)
{
    tree.erase(MultiPair<K, V>(key));
}

template<typename K, typename V>
bool mmap<K, V>::contains(K key)
{
    return tree.contains(MultiPair<K, V>(key));
}

template<typename K, typename V>
typename mmap<K, V>::Iterator mmap<K, V>::find(K key)
{
    return typename mmap<K, V>::Iterator(tree.find(MultiPair<K,V>(key)));
}


//  PROC: This will print the map as a table. The left column will be the keys
//  and the column to the right will list the values of each key.
//  Everything will be in sorted order
template<typename K, typename V>
void mmap<K, V>::print() const
{
    Vector<MultiPair<K, V> > mpairs;    //  CALC: This vector will hold the
                                        //  multi pairs in sorted order

    //  PROC: Convert the tree into a vector
    mpairs = tree.to_vector();

    //  OUTPUT: Label the table (Keys / Values)
    cout << setw(15) << left << "Keys" << "Values" << endl;

    for(unsigned int i = 0; i < mpairs.size(); i++)
    {
        cout << setw(15);
        cout << left << mpairs.at(i).key;

        cout <<  mpairs.at(i).values.to_vector().to_string() << endl;


    }
}

template<typename K, typename V>
ostream & operator << (ostream &out,       //  IN: Ostream object
                              const mmap<K, V> &map)
{
    map.print();

    return out;
}




#endif // MMAP_H
