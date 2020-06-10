#ifndef CONDITION_H
#define CONDITION_H

#include <string>
#include "stokenizer.h"
#include "vector.h"
#include "map.h"
#include "mmap.h"



using namespace std;

const bool CONDITION_DEBUG = false;

class Condition
{
public:

    Condition();
    Condition(string condition, map<string, unsigned int> &field_map, Vector<mmap<string, long > > &value_map);
    Condition(Vector<long> record_nums);


    string get_field() const
    {
        return field;
    }
    string get_value() const
    {
        return value;
    }

    string get_operator() const
    {
        return operation;
    }

    Vector<long> get_record_nums();


    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                  const Condition &cond)
    {
        out << "field: " << cond.get_field() << " value: " << cond.get_value() << " operator: " << cond.get_operator();
        return out;
    }

    bool is_expression () const
    {
        return expression;
    }



private:

    STokenizer tokenizer;

    string field;
    string operation;
    string value;
    bool expression;
    Vector<long> record_nums;

    map<string, unsigned int> field_map;
    Vector<mmap<string, long > > value_map;

    void convert_string(string condition);



};

Vector<long> Condition::get_record_nums()
{


    Vector<long> record_nums_temp;       //  CALC: Holds the record_nums that we
                                    //  accumulate while moving the iterator
                                    //  key to key


    if(CONDITION_DEBUG)
    {
        cout << "\n\nfield: " << field << endl;
        cout << "operation: " << operation << endl;
        cout << "value: " << value << endl;
        cout << "field_index: " << field_map[get_field()] << endl;
    }

    //  PROC: If the expression is true and record_nums is empty, then we
    //  know that this Condition has not been evaluated yet, so we evaluate it
    //  by returning the vector of records that match the expression
    if(is_expression() == true && record_nums.empty())
    {
        if(get_operator() == "=")
        {
            return value_map.at(field_map[get_field()])[get_value()].to_vector();
        }

        //  PROC: Find all of the record numbers for values greater than the
        //  specified value
        else if(get_operator() == ">")
        {
            //  PROC: Create an iterator that points to the value after
            //  the specified value
            mmap<string, long>::Iterator it = value_map.at(field_map[get_field()]).upper_bound(get_value());

            //  PROC: Iterate through the end of the map, making a union with
            //  each multi pair that we encounter
            while(it != value_map.at(field_map[get_field()]).end())
            {
                record_nums_temp = record_nums_temp.create_union((*it).values.to_vector());
                it++;
            }

            return record_nums_temp;
        }

        //  PROC: Find all of the record numbers for values greater than or equal to the
        //  specified value
        else if(get_operator() == ">=")
        {
            //  PROC: Create an iterator that points to either the specifed
            //  value or the value after them
            mmap<string, long>::Iterator it = value_map.at(field_map[get_field()]).lower_bound(get_value());

            //  PROC: Iterate through the end of the map, making a union with
            //  each multi pair that we encounter
            while(it != value_map.at(field_map[get_field()]).end())
            {
                record_nums_temp = record_nums_temp.create_union((*it).values.to_vector());
                it++;
            }

            return record_nums_temp;
        }

        else if(get_operator() == "<")
        {
            //  PROC: Create an iterator pointing to the beginning of the map
            mmap<string, long>::Iterator it = value_map.at(field_map[get_field()]).begin();

            //  PROC: Iterate right to the value specified
            while(it != value_map.at(field_map[get_field()]).lower_bound(get_value()))
            {
                record_nums_temp = record_nums_temp.create_union((*it).values.to_vector());
                it++;
            }

            return record_nums_temp;
        }

        else if(get_operator() == "<=")
        {
            //  PROC: Create an iterator pointing to the beginning of the map
            mmap<string, long>::Iterator it = value_map.at(field_map[get_field()]).begin();


            //  PROC: Iterate right to the value specified
            while(it != value_map.at(field_map[get_field()]).upper_bound(get_value()))
            {
                record_nums_temp = record_nums_temp.create_union((*it).values.to_vector());
                it++;
            }

            return record_nums_temp;
        }

    }

    //  PROC: If the expression is equal to false, then this condition has
    //  already been evaluated and contains a vector of records. Return
    //  the private member variable record_nums
    else if(is_expression() == false)
    {
        return record_nums;
    }

    else
    {
        assert(0);
    }
}

Condition:: Condition()
{
    expression = false;
}


Condition::Condition(string condition, map<string, unsigned int> &field_map, Vector<mmap<string, long > > &value_map)
{
    //  PROC: Set expression to true since we are constructing an expression
    expression = true;

    //  PROC: Store the field map and value map
    this->field_map = field_map;
    this->value_map = value_map;

    //  PROC: Store the string's field, operator, and value
    convert_string(condition);

    //  PROC: If the field map does not contain the field throw an error
    if(!field_map.contains(get_field()))
    {
        string str = "The field used in condition: " + condition + " is not a valid field!";
        throw str;
    }

    //  PROC: If the value map does not contain the field throw an error
//    if(!value_map.at(field_map[get_field()]).contains(get_value()))
//    {
//        string str = "The value used in condition: " + condition + " is not a value field!";
//        throw str;;
//    }

}

Condition::Condition(Vector<long> record_nums)
{
    expression = false;
    this->record_nums = record_nums;
}


void Condition::convert_string(string condition)
{
    Token token;
    Vector<string> condition_list;

    //  PROC: Tokenize the string
    tokenizer.set_string(condition);

    if(CONDITION_DEBUG)
    {
        cout << "\n\ncalled convert_string(string condition)\n";
        cout << "condition: " << condition << endl << endl;
    }


    //  PROC: Put tokens into vector excluding spaces
    while(!tokenizer.done())
    {
        tokenizer >> token;

        if(token.token_str() != " ")
        {
            condition_list.push_back(token.token_str());
        }



    }

    if(CONDITION_DEBUG)
    {
        cout << "condition_list: " << condition_list.to_string() << endl;
        cout << "condition_list size: " << condition_list.size() << endl;
    }

    //  PROC: The condition must be equal to 3 (field, operator, value)
    assert(condition_list.size() == 3);

    //  PROC: Assert that the condition is an equal sign and assert
    //  that the size of the vector is 3 because there will be 3 tokens
    //  the field, the operator, and the value (ex. last = Harris )
    assert(condition_list.at(1) == "=" || condition_list.at(1) == ">"
           || condition_list.at(1) == ">=" || condition_list.at(1) == "<" || condition_list.at(1) == "<=");
    assert(condition_list.size() == 3);

    //  PROC: Remove the ' from the valued

//    value.erase(remove_if(value.begin(), value.end(), "'"), value.end());

    //  PROC: Insert the field, value, and operator into their member variables
    field = condition_list.at(0);
    operation = condition_list.at(1);
    value = condition_list.at(2);

    //  PROC: Remove the ' from the value string (some value strings will contain
    //  multiple words, so we surround them with ' to differentiate them from
    //  the other tokens
    for(unsigned int i = 0; i < value.size(); i++)
    {
        if(value.at(i) == '\'')
        {

            value.erase(value.begin() + i);
        }
    }

    //  PROC: Remove the quotation marks at the beginning and the end of the
    //  value in the Condition expression
    if(value.at(0) == '\"' && value.at(value.size() - 1) == '\"')
    {
        value.erase(value.begin());
        value.erase(value.end() - 1);
    }

    if(CONDITION_DEBUG)
    {
        cout << "field: " << field << endl;
        cout << "operation: " << operation << endl;
        cout << "value: " << value << endl;
    }


}

Condition operator&&(Condition &left, Condition &right)
{

    return Condition(left.get_record_nums().create_intersection(right.get_record_nums()));

}

Condition operator||(Condition &left, Condition &right)
{

    return Condition(left.get_record_nums().create_union(right.get_record_nums()));

}






#endif // CONDITION_H
