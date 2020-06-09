#ifndef SHAUNTING_YARD_H
#define SHAUNTING_YARD_H

#include "vector.h"
#include "map.h"
#include "stack.h"
#include "queue.h"
#include "condition.h"
#include "parser.h"
#include <string>
#include <iostream>

//  TODO: ERROR: If OPS equals 0, then the program crashes. Most likely because
//  the stack will pop() a 0 if there is nothing there.

using namespace std;

//enum op {OR=3, AND, OPS=10};

enum op { OPS=10};

class Shaunting_Yard
{
public:
    Shaunting_Yard(Vector<string> infix, map<string, unsigned int> field_map, Vector<mmap<string, long > > value_map);

    Vector<string> get_postfix();

    Vector<long> rpm();


    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                  const Shaunting_Yard &yard)
    {
        out << 5;

        return out;
    }


private:

    bool is_operator(string str);

    Vector<string> infix;
    map<string, unsigned int> operator_map;      //  CALC: Map that links each operator
                                        //  to a number that represents its
                                        //  precedence. The higher the number
                                        //  the higher the precedence

    map<string, unsigned int> field_map;
    Vector<mmap<string, long > > value_map;
//    template<typename T>
//    double calculate(T left, T right, string operation);

    Condition calculate(Condition &left, Condition &right, string operation);



    const bool DEBUG = false;

};


Condition Shaunting_Yard::calculate(Condition &left, Condition &right, string operation)
{
    Condition result;     //  OUT: Holds the result of the Condition


    if(DEBUG)
    {
        cout << "\n\ncalling calculate():\n";
        cout << "left: " << left.get_record_nums().to_string() << " right: " << right.get_record_nums().to_string() << endl;
    }

    //  proc: if the operation is 'add' then add the left and the right numbers
    if(operation == "and")
    {
        result = left && right;
    }

    else if(operation == "or")
    {
        result = left || right;
    }


    if(DEBUG)
    {
        cout << "result: " << result.get_record_nums().to_string() << endl << endl;
    }

    return result;
}


//template<typename T>
//double Shaunting_Yard::calculate(T left, T right, string operation)
//{
//    double result;     //  OUT: Holds the result of the calculation

//    result = 0;

//    if(DEBUG)
//    {
//        cout << "\n\ncalling calculate():\n";
//        cout << "left: " << left << " right: " << right << endl;
//    }

//    //  proc: if the operation is 'add' then add the left and the right numbers
//    if(operation == "+")
//    {
//        result = left + right;
//    }

//    else if(operation == "-")
//    {
//        result = left - right;
//    }

//    else if(operation == "/")
//    {
//        result = left / right;
//    }

//    else if(operation == "*")
//    {
//        result = left * right;
//    }

//    else
//    {
//        assert(0);
//    }

//    if(DEBUG)
//    {
//        cout << "result: " << result << endl << endl;
//    }

//    return result;
//}


Vector<long> Shaunting_Yard::rpm()
{
    Vector<string> postfix;
    Stack<Condition> conditions;
    Queue<string> operators;
    string element;

    Condition left;           //  CALC: The condition that is on the left in the equation
    Condition right;          //  CALC: The condtiion that is on the right in the equation

    postfix = get_postfix();

    if(DEBUG)
    {
        cout << "postfix: " << postfix.to_string() << endl;
    }

    //  PROC: Loop through the postfix expression
    for(unsigned int i = 0; i < postfix.size(); i++)
    {
        element = postfix.at(i);

        //  PROC: If the elment is an operator, then perform the operation on
        //  the top two conditions of the stack (top of the stack being on the right
        //   and the one below on the left)
        if(operator_map.contains(element))
        {
            //  PROC: Pop the numbers stack and convert into an integer and store
            //  them in the variables that represent where they are in the equation
            right = conditions.pop();
            left = conditions.pop();

            //  PROC: Calculate the result of using the operator in between both
            //  the left and right numbers. Push this result back into the numbers
            //  stack
            conditions.push(calculate(left, right, element));



        }

        //  PROC: Or else push it into the numbers stack since it is a number
        else
        {
            conditions.push(Condition(element, field_map, value_map));

        }
    }

    return conditions.pop().get_record_nums();

//    cout << "\n\operators: ";

//    while(!operators.empty())
//    {
//        cout << operators.pop() << endl;
//    }

}


Vector<string> Shaunting_Yard::get_postfix()
{

    Stack<string> operator_stack;       //  CALC: Holds the operators as we
                                        //  perform postfix algorithmn
    Vector<string> postfix;             //  CALC: Holds the postfix output

    string element;                     //  CALC: Holds each element of the infix
                                        //  expression as we iterate through it

    if(DEBUG)
    {
        cout << "\n\ncalled get_postfix()\n\n";
        cout << "infix: " << infix.to_string() << endl << endl;
    }

    //  PROC: Loop through each element of the infix expression
    for(unsigned int i = 0; i < infix.size(); i++)
    {




        element = infix.at(i);

        if(DEBUG)
        {
            cout << "Loop #: " << i << endl;
            cout << "element: " << element << endl;

        }

        if(is_operator(element))
        {


            if(DEBUG)
            {
                cout << "Inside";

            }


            //  PROC: Check if the current element is greater in precedenece
            //  than the element at the top of the stack in the operator_stack
            if(operator_map[element] > operator_map[operator_stack.top()])
            {
                if(DEBUG)
                {
                    cout << "\n\nCurrent operator is greater than the one"
                            " on top of stack\n\n";
                    cout << "element: " << element << endl;
                }
                //  PROC: Push the element to the top of the operator stack
                operator_stack.push(element);
            }

            //  PROC: Check if the current element is lesser in precedenece
            //  than the element at the top of the stack in the operator_stack
            else if(operator_map[element] < operator_map[operator_stack.top()])
            {
                if(DEBUG)
                {
                    cout << "\n\nCurrent operator is less than the one"
                            " on top of stack\n\n";
                    cout << "element: " << element << endl;
                }

                //  PROC: While the current element is less than or equal to
                //  the top of the stack, pop the top of the stack and insert
                //  into the postfix vector
                while(operator_map[element] <= operator_map[operator_stack.top()])
                {
                    //  PROC: Pop the operator from the operator_stack and push it into
                    //  the postfix vector
                    postfix.push_back(operator_stack.pop());
                }


                //  PROC: Now push the new element into the operator stack
                operator_stack.push(element);
            }

            //  PROC: If the element is equal to the operator at the top of the
            //  stack in precedence, then pop stack element into postfix vector
            //  and then insert new operator into operator stack
            else if(operator_map[element] == operator_map[operator_stack.top()])
            {
                if(DEBUG)
                {
                    cout << "\n\nCurrent operator is equal to the one"
                            " on top of stack\n\n";
                    cout << "element: " << element << endl;
                }

                //  PROC: Pop the operator from the operator_stack and push it into
                //  the postfix vector
                postfix.push_back(operator_stack.pop());

                //  PROC: Now push the new element into the operator stack
                operator_stack.push(element);
            }

            //  PROC: If the stack is empty, then push the operator into it
            else if(operator_stack.empty())
            {

                operator_stack.push(element);
            }

            else {
                assert(0);
            }

        }

        //  PROC: If the element is not an operator, then we assume it is a number
        else
        {
            if(DEBUG)
            {
                cout << "\n\nThe element is not an operator and is an expreession\n";
                cout << "element: " << element << endl;
            }

            postfix.push_back(element);
        }
    }

    //  PROC: Pop all of the operators in the operator stack to the end of the
    //  postfix vector
    while(!operator_stack.empty())
    {
        postfix.push_back(operator_stack.pop());
    }

    return postfix;
}



bool Shaunting_Yard::is_operator(string str)
{

    if(DEBUG)
    {
        cout << "\n\nCalling is_operator(string str)\n\n";

    }

    if(operator_map.contains(str))
    {
        return true;
    }

    else
    {
        return false;
    }
}


Shaunting_Yard::Shaunting_Yard(Vector<string> infix, map<string, unsigned int> field_map, Vector<mmap<string, long > > value_map)
{
    this->infix = infix;



    //  PROC: insert each operator to the map and then assign the number that
    //  correlates to its order in the vector (since this list is suppose to
    //  be the operators from least to greatest.
//    for(unsigned int i = 0; i < operators.size(); i++)
//    {
//        //  PROC: The operator vector contains Pairs. The key is the actual operator
//        //  as a string and the value is its precedence
//        operator_map[operators.at(i).key] = operators.at(i).value;
//    }

    operator_map["and"] = AND;
    operator_map["or"] = OR;

    this->field_map = field_map;
    this->value_map = value_map;


}





#endif // SHAUNTING_YARD_H
