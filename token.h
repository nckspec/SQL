#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf
#include <stdio.h>
#include <string.h>
#include <cassert>



using namespace std;

const int ALPHA_STATE = 1;
const int START_STATE = 0;
const int SPACE_STATE = 10;
const int NUMBER_STATE = 20;
const int DECIMAL_STATE = 21;
const int NUMBER_STATE_END = 22;

const int APOSTROPHE_STATE = 30;

const int QUOTATION_STATE = 40;

const int CONDITIONAL_OPERATOR_STATE = 60;

const int UNKNOWN_STATE = -1;

class Token
{

    public:
        Token();
        Token(string str, int type);
        friend ostream& operator <<(ostream& outs, const Token& t);
        const Token operator+(const Token &other) const;
//        friend Token operator = (const Token &t1, const Token &t2);
        int type() const;
        string type_string();
        string token_str() const;
    private:
        string _token;
        int _type;

};


/**********************************************************
*
* Constructor Token(): Class Token
*_________________________________________________________
* Constructs a blank object
*_________________________________________________________
* PRE-CONDITIONS
* str: The token
* type: an integer representing the type
*
* POST-CONDITIONS
*
*
***********************************************************/
Token::Token()
{
    _token = "";
    _type = -1;

}

/**********************************************************
*
* Constructor Token(string str, int type): Class Token
*_________________________________________________________
* Constructs an object with the token and its type
*_________________________________________________________
* PRE-CONDITIONS
* str: The token
* type: an integer representing the type
*
* POST-CONDITIONS
*
*
***********************************************************/
Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}

/**********************************************************
*
* Accessor type_string(): Class Token
*_________________________________________________________
* Returns the type of the token in word form
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
* - A string that represents the type as a word
*
*
***********************************************************/
string Token::type_string()
{

    //  PROC: If we get a type that does not fit our criteria
//    assert(type() > 0 && type() < 5);

    //  PROC: If the type is equal to a 'word' or letters from the alphabet
    //  then return "ALPHA"
    if(type() == ALPHA_STATE)
    {
        return "ALPHA";
    }

    //  PROC: If the type is a number then we return"NUMBER"
    else if(type() == NUMBER_STATE || type() == NUMBER_STATE_END)
    {
        return "NUMBER";
    }

    //  PROC: If the type is a space then we return "SPACE"
    else if(type() == SPACE_STATE)
    {
        return "SPACE";
    }

    //  PROC: If the type is a <=, <, >, >= then we return "CONDITION"
    else if(type() == CONDITIONAL_OPERATOR_STATE || type() == CONDITIONAL_OPERATOR_STATE + 1)
    {
        return "CONDITION";
    }

    //  PROC: If the type is a space then we return "UNKNOWN"
    else
    {
        return "UNKNOWN";
    }




}

/**********************************************************
*
* Accessor token_str(): Class Token
*_________________________________________________________
* Returns the token
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
* - A string that contains the token
*
*
***********************************************************/
string Token::token_str() const
{

    return _token;

}

/**********************************************************
*
* Accessor type(): Class Token
*_________________________________________________________
* Returns the type of the token as an integer that is
* associated with an arbitrary constant
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
* - An integer between 1-4
*
*
***********************************************************/
int Token::type() const
{

    return _type;

}

/**********************************************************
*
* Operator <<: Class Token
*_________________________________________________________
* This allows us to use the output stream operator to output
* the token
*_________________________________________________________
* PRE-CONDITIONS
* outs: The output stream operator
* t: The token object that is to be outputted
*
* POST-CONDITIONS
* - Prints the token
*
*
***********************************************************/
ostream& operator <<(ostream& outs, const Token& t)
{

    //  PROC: This will print the token
    outs << "|" << t.token_str() << "|";

    return outs;
}

/**********************************************************
*
* Operator +: Class Token
*_________________________________________________________
* The + operator becomes overloaded so that if two tokens are
* of the same type, the strings can be added together. This is
* used when a token is broken because the buffer was not big enough
* to hold the whole token
*_________________________________________________________
* PRE-CONDITIONS
* other: The token that is being added on the right of the +
*        operator
*
* POST-CONDITIONS
* - Returns a token with the string of both token combined
*
*
***********************************************************/
const Token Token::operator+(const Token &other) const
{
    //  PROCESSING: If both the tokens have the same type, we combine them into
    //  one token
    if(type() == other.type())
    {
        return Token(token_str() + other.token_str(), type());
    }

    else
    {
        return Token();
    }


}

//Token operator =(const Token &t1, const Token &t2)
//{
//    Token temp;     //  CALC: A temporary token that will be formed by the two
//                    //  tokens that are being added together

//    temp = Token(t1.)


//    return temp;

//}




#endif // TOKEN_H
