#ifndef MAP_H
#define MAP_H

#include "bptree.h"
#include <iomanip>


//  TODO: On the DEBUG statement in the ostream operator, the out << statement was
//  commented out because there was a conflict with the Table Class

const bool MAP_DEBUG = true;

using namespace std;

template<typename K, typename V>
struct Pair
{

    K key;

    V value;

    Pair<K, V>()
    {
        key = K();
        value = V();
    }

    Pair<K, V>(K key)
    {
        this->key = key;
        value = V();
    }

    Pair<K , V>(K key, V value)
    {
        this->key = key;
        this->value = value;
    }

    V getValue() const
    {
        return value;
    }

    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                  const Pair<K, V> &pair)
    {
        if(MAP_DEBUG)
        {
//            out << "[" << pair.key << ", " << pair.getValue() << "]";
        }

        else
        {
//            out << pair.getValue();
        }

        return out;
    }


    bool operator <(Pair<K, V>& rh)
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

    bool operator >(Pair<K, V>& rh)
    {

        //  PROC: If the key on the left is less than the key on the right
        //  return true
        if(key > rh.key)
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

    bool operator <=(Pair<K, V>& rh)
    {

        //  PROC: If the key on the left is less than the key on the right
        //  return true
        if(key <= rh.key)
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

    bool operator ==(Pair<K, V>& rh)
    {

        //  PROC: If the key on the left is less than the key on the right
        //  return true
        if(key == rh.key)
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


    Pair<K, V> operator + (const Pair<K, V> & rhs)
    {
        value = value + rhs.value;
        return *this;
    }

    bool operator +=(Pair<K, V>& rh)
    {

        //  PROC: Replace the value with the value on the right hand side
        value = rh.value;


    }


};



template<typename K, typename V>
class map
{


public:

    typedef bptree< Pair<K, V> > map_base;

    class Iterator{
    public:
        friend class map;


        Iterator()
        {
            if(MAP_DEBUG)
            {
                cout << "\n\nCalling Constructor for map::Iterator()\n\n";

            }
        }

        Iterator(const map<K,V> &m)
        {
            if(MAP_DEBUG)
            {
                cout << "\n\nCalling Constructor for map::Iterator(const map<K,V> m)\n\n";

            }

            _it = m.tree.begin();
        }
        Iterator(typename map_base::Iterator it)
        {
            if(MAP_DEBUG)
            {
                cout << "\n\nCalling Constructor for map::"
                        "Iterator(typename map_base::Iterator it)\n\n";

            }

            _it = it;
        }



        Iterator & operator ++(int unused);
        Iterator & operator ++();
        Pair<K, V> & operator *();
//        bool operator ==(const Iterator& rhs)
//        {
//            if(_it == rhs._it)
//            {
//                return true;
//            }

//            else {
//                return false;
//            }
//        }

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
    Iterator begin();
    Iterator end();

    //  ACCESSOR
    Iterator find(K key);
    bool contains(K key);
    void print() const;
    int size() const;


    //  MUTATORS
    V & get(K entry);
    V & get(K entry) const;
    void insert(K key, V entry);
    void erase(K key);

    //  OPERATORS
    V& operator[] (K index);
    template <typename A, typename B>
    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                  const map<A, B> &map);
    void operator *()
    {
        cout << tree;
        *tree;
    }










private:
    map_base tree;


};

template<typename K, typename V>
typename map<K, V>::Iterator map<K, V>::begin()
{
    return map<K, V>::Iterator(tree.begin());
}

template<typename K, typename V>
typename map<K, V>::Iterator map<K, V>::end()
{
    return map<K, V>::Iterator(tree.end());
}



template<typename K, typename V>
typename map<K, V>::Iterator & map<K, V>::Iterator::operator++(int unused)
{
    _it++;

    return *this;
}

template<typename K, typename V>
typename map<K, V>::Iterator & map<K, V>::Iterator::operator++()
{
    _it++;

    return *this;
}




template<typename K, typename V>
Pair<K,V> & map<K, V>::Iterator::operator*()
{

    return *_it;
}

template<typename K, typename V>
int map<K, V>::size() const
{
    return tree.size();
}

template<typename K, typename V>
ostream & operator << (ostream &out, const map<K, V> &pair)
{
    pair.print();
    return out;
}

template<typename K, typename V>
V& map<K, V>::operator[] (K key)
{
    return get(key);
}

template<typename K, typename V>
V & map<K, V>::get(K key)
{
    return tree.get(Pair<K, V>(key, V())).value;
}

template<typename K, typename V>
V & map<K, V>::get(K key) const
{
    return tree.get(Pair<K, V>(key, V())).value;
}

template<typename K, typename V>
void map<K, V>::insert(K key, V entry)
{
    //  PROC: Insert into the tree a pair with the key and entry specified
    tree.insert(Pair<K, V>(key, entry));
}

template<typename K, typename V>
typename map<K, V>::Iterator map<K, V>::find(K key)
{
    return typename map<K, V>::Iterator(tree.find(Pair<K,V>(key)));
}

template<typename K, typename V>
void map<K, V>::erase(K key)
{
    tree.erase(Pair<K, V>(key));
}

template<typename K, typename V>
bool map<K, V>::contains(K key)
{
    return tree.contains(Pair<K, V>(key));
}

template<typename K, typename V>
void map<K, V>::print() const
{

    Vector<Pair<K, V> > pairs;    //  CALC: This vector will hold the
                                  //  pairs in sorted order

    //  PROC: Convert the tree into a vector
    pairs = tree.to_vector();

    //  OUTPUT: Label the table (Keys / Values)
    cout << setw(15) << left << "Keys" << "Value" << endl;

    for(unsigned int i = 0; i < pairs.size(); i++)
    {
        cout << setw(15);
        cout << left << pairs.at(i).key;

        cout <<  pairs.at(i).value << endl;


    }
}



#endif // MAP_H
