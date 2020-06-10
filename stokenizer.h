#ifndef STOKENIZER_H
#define STOKENIZER_H

#include "token.h"


const int MAX_COLUMNS = 500;
const int MAX_ROWS = 500;
const int MAX_BUFFER = 200;

const bool TOKEN_DEBUG = false;

class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);

    void set_string(string str);

    //  Returns the current position in the buffer
    int pos();


private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    void mark_cell(int state, int _table[][MAX_COLUMNS], char start, char end,
                   int nextState);

    void mark_state(int _table[][MAX_COLUMNS], int state, bool success);

    //extract the longes
    // t string that match
    //     one of the acceptable token types
    bool get_token(string& token, int &endState);

    //  Sets the _bufferLen variable with the length of the inputted string
    void setBufferLen();

    //  Checks if the _pos has passed the '\0' character signifying that every
    //  token has been extracted
    bool isBufferDone();

    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    unsigned long _bufferLen;       // Holds the length of the string in the
                                    //  bufffer


    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];

    bool _done;                     //  Keeps track of whether there are more
                                    //  tokens to extract or not

    bool DEBUG = false;
};


int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];


/**********************************************************
*
* Constructor STokenizer: Class STokenizer
*_________________________________________________________
* Constructs an empty STokenizer
*_________________________________________________________
* PRE-CONDITIONS
*
*
* POST-CONDITIONS
*
*
***********************************************************/
STokenizer::STokenizer()
{
    //  PROCESSING: Initialize private member variables
    _pos = 0;
    _done = true;
    _bufferLen = 0;

    //  PROCESSING: Generate the table
    make_table(_table);

}


/**********************************************************
*
* Constructor STokenizer: Class STokenizer
*_________________________________________________________
* Constructs a STokenizer with the specified C-string
* to go into the buffer
*_________________________________________________________
* PRE-CONDITIONS
*
* int str[]: The string that will be inserted into the buffer
*
* POST-CONDITIONS
*
*
***********************************************************/
STokenizer::STokenizer(char str[])
{

    //  PROCESSING: Initialize private member variables
    _pos = 0;
    _done = false;
    _bufferLen = 0;


    assert(strlen(str) < MAX_BUFFER);


    //  PROC: Copy the specified string into the buffer
    strcpy(_buffer, str);

    //  PROC: Sets the length of the buffer in the _bufferLen variable
    setBufferLen();

    //  PROCESSING: Generate the table
    make_table(_table);
}


/**********************************************************
*
* Mutator set_string(): Class STokenizer
*_________________________________________________________
* Inserts a string into the STokenizer buffer to be tokenized.
*_________________________________________________________
* PRE-CONDITIONS
*
* char str[]: The string that will be inserted into the buffer
*
* POST-CONDITIONS
*
*
***********************************************************/
void STokenizer::set_string(char str[])
{
    assert(strlen(str) <= MAX_BUFFER);

    //  PROC: Copy the specified string into the buffer
    strcpy(_buffer, str);

    //  PROC: Set the position back to 0 to handle the new buffer
    _pos = 0;

    //  PROC: Sets the length of the buffer in the _bufferLen variable
    setBufferLen();
}

/**********************************************************
*
* Mutator set_string(): Class STokenizer
*_________________________________________________________
* Inserts a string into the STokenizer buffer to be tokenized.
*_________________________________________________________
* PRE-CONDITIONS
*
* string str: The string that will be inserted into the buffer
*
* POST-CONDITIONS
*
*
***********************************************************/
void STokenizer::set_string(string str)
{
    assert(str.size() <= MAX_BUFFER);

    for(unsigned int i = 0; i < str.size(); i++)
    {
        _buffer[i] = str.at(i);
    }

    _buffer[str.size()] = '\0';

    //  PROC: Set the position back to 0 to handle the new buffer
    _pos = 0;

    //  PROC: Sets the length of the buffer in the _bufferLen variable
    setBufferLen();
}


/**********************************************************
*
* Accessor done(): Class STokenizer
*_________________________________________________________
* Lets the user know if the tokenizer has finished extracting
* all of the tokens
*_________________________________________________________
* PRE-CONDITIONS
*
* int str[]: The string that will be inserted into the buffer
*
* POST-CONDITIONS
* - returns true if there are no more tokens
* - returns false if there are more tokens
*
*
***********************************************************/
bool STokenizer::done()
{

    //  PROC: Checks the _done private member variable and returns it to the
    //  user
    if(_done == true)
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
* Mutator make_table(): Class STokenizer
*_________________________________________________________
* Creates a table that will allow us to identify alphabetical
* characters, numbers, spaces, and punctuation.
*_________________________________________________________
* PRE-CONDITIONS
*
* int _table[][]: The table that we are modifying
*
* POST-CONDITIONS
*
*
***********************************************************/
void STokenizer::make_table(int _table[MAX_ROWS][MAX_COLUMNS])
{




    for(int h = 0; h < MAX_ROWS; h++)
    {
        for(int i = 0; i < MAX_COLUMNS; i++)
        {
            _table[h][i] = -1;
        }
    }


    //  PROC: Mark the starting state directing it to point
    //  alphabetical character to point to the ALPHA State
    mark_cell(START_STATE, _table, 'a', 'z', ALPHA_STATE);
    mark_cell(START_STATE, _table, 'A', 'Z', ALPHA_STATE);

    //  PROC: Mark all numbers in the starting state to go to the NUMBER_STATE
    mark_cell(START_STATE, _table, '0', '9', NUMBER_STATE);

    //  PROC: Mark the space character in the START_STATE to bring us to
    //  the SPACE_STATE
    mark_cell(START_STATE, _table, ' ', ' ', SPACE_STATE);


    //  PROC: this will allow the tokenizer to identify a string of words
    //  surrounded by '' as a token
    mark_cell(START_STATE, _table, '\'', '\'', APOSTROPHE_STATE);
    mark_cell(APOSTROPHE_STATE, _table, ' ', '~', APOSTROPHE_STATE);
    mark_cell(APOSTROPHE_STATE, _table, '\'', '\'', APOSTROPHE_STATE + 1);
    mark_state(_table, APOSTROPHE_STATE + 1, true);

    //  PROC: this will allow the tokenizer to identify a string of words
    //  surrounded by "" quotation marks as a token
    mark_cell(START_STATE, _table, '\"', '\"', QUOTATION_STATE);
    mark_cell(QUOTATION_STATE, _table, ' ', '~', QUOTATION_STATE);
    mark_cell(QUOTATION_STATE, _table, '\"', '\"', QUOTATION_STATE + 1);
    mark_state(_table, QUOTATION_STATE + 1, true);


    //  PROC: This will tokenize the conditional operator <= and >=
    mark_cell(START_STATE, _table, '>', '>', CONDITIONAL_OPERATOR_STATE);
    mark_cell(START_STATE, _table, '<', '<', CONDITIONAL_OPERATOR_STATE);
    mark_state(_table, CONDITIONAL_OPERATOR_STATE, true);
    mark_cell(CONDITIONAL_OPERATOR_STATE, _table, '=', '=', CONDITIONAL_OPERATOR_STATE  + 1);
    mark_state(_table, CONDITIONAL_OPERATOR_STATE + 1, true);




    //  PROC: Any time an alphabetical character is received in the ALPHA state
    //  we keep it in the alpha state since we want to collect all alphabetical
    //  characters for the token. Also mark the ALPHA_STATE to be a success
    //  state so that even if we only get one alphabetical character it will be
    //  considered a token
    mark_state(_table, ALPHA_STATE, true);
    mark_cell(ALPHA_STATE, _table, 'a', 'z', ALPHA_STATE);
    mark_cell(ALPHA_STATE, _table, 'A', 'Z', ALPHA_STATE);

    //  PROC: Marks the number state so that any number received keeps us
    //  in the number state
    mark_state(_table, NUMBER_STATE, true);
    mark_cell(NUMBER_STATE, _table, '0', '9', NUMBER_STATE);

    //  PROC: If a '.' is found, then we go into the next NUMBER_STATE which
    //  will check if there are more numbers following it. If there are more
    //  numbers following it then we assume we have found a decimal number
    mark_cell(NUMBER_STATE, _table, '.', '.', DECIMAL_STATE);

    //  PROC: The Decimal number state is a fail state because if we get
    //  anything besides a number, we want to exclude the '.'
    mark_state(_table, DECIMAL_STATE, false);
    mark_state(_table, NUMBER_STATE_END, true);
    mark_cell(DECIMAL_STATE, _table, '0', '9', NUMBER_STATE_END);
    mark_cell(NUMBER_STATE_END, _table, '0', '9', NUMBER_STATE_END);


    //  PROC: Marks the space state so that any spaces received keeps us in
    //  the space state
    mark_state(_table, SPACE_STATE, true);
    mark_cell(SPACE_STATE, _table, ' ', ' ', SPACE_STATE);


}


/**********************************************************
*
* Mutator mark_cell(): Class STokenizer
*_________________________________________________________
* Marks the specified state so that the specified characters will point
* to the another specified state when receivied by the machine
*_________________________________________________________
* PRE-CONDITIONS
* int state: The state that we will be marking
*
* int _table[][]: The table that we are modifying
*
* char start: The starting character that is being marked
*
* char end: The ending character that is being marked
*
* int nextState: The state that these characters will travel to
* when the machine receives them
*
* POST-CONDITIONS
*
*
***********************************************************/
void STokenizer::mark_cell(int state, int _table[][MAX_COLUMNS],
                           char start, char end, int nextState)
{

    //  PROCESSING: Start at the ascii number associated with the first
    //  character that we are marking and then loop until we get to the last
    //  character we are marking
    for(int i = int(start); i <= int(end); i++)
    {

        //  PROC: Mark the table at the specified state at the column number
        //  that is associated with the character we are marking. Mark it with
        //  the next state
        _table[state][i] = nextState;


    }
}


/**********************************************************
*
* Mutator mark_state(): Class STokenizer
*_________________________________________________________
* Marks the state as a success or a fail state
*_________________________________________________________
* PRE-CONDITIONS
* int[][] _table: The table that we are modifying
*
* int state: The state that we will be marking
*
* bool success: A bool that represents whether we are marking it as a
*               success state or a fail state
*
* POST-CONDITIONS
*
*
***********************************************************/
void STokenizer::mark_state(int _table[][MAX_COLUMNS], int state, bool success)
{

    //  PROC: If the state is suppose to be a success state, mark the first
    //  column with '1'
    if(success == true)
    {
        _table[state][0] = 1;
    }

    //  PROC: If the state is suppose to be a fail state, mark the first
    //  column with '0'
    else
    {
        _table[state][0] = 0;
    }


}


/**********************************************************
*
* Mutator get_token(): Class STokenizer
*_________________________________________________________
* Extracts a token from the buffer using the private member
* table
*_________________________________________________________
* PRE-CONDITIONS
* int &token: a reference variable that will hold the extracted token
* int &endState: a reference variable that will hold the last state
*                the machine was in before it failed.
*
* POST-CONDITIONS
*
*
***********************************************************/
bool STokenizer::get_token(string& token, int &endState)
{

//    assert(_pos <= _bufferLen);

    //  PROCESSING: If the buffer has not been fully extracted then we extract
    //  another token
    if(done() == false)
    {

        //    assert(_buffer[_pos - 1] != '\0');

            if(TOKEN_DEBUG)
            {
                cout << "\n\ncalled get_token(): STokenizer\n\n";
                cout << "token: " << token << endl;
            }

            //  CALC: Holds the current token
            string tempToken = "";

            //  CALC: Holds the success token
            string successToken = "";

            //  CALC: Holds the current state
            int state = 0;

            //  CALC: Holds the ascii number associated with the current
            //  character
            int asciiChar = 0;

            //  CALC: Holds the position of the character following the success
            //  token
            int pos = 0;


            //  PROC: Make the initial state the specified start_state
            state = START_STATE;

            //  PROC: Initialize the pos with the _pos
            pos = _pos;


            //  PROC:  Loop through each character of the string that we are
            //  looking for a token in
            for(int i = _pos; i < MAX_BUFFER; i++)
            {

                //  PROC: Add the character that we are on to the temp token
                //  string
                tempToken = tempToken + _buffer[i];

                //  PROC: Convert the character into an ascii number
                asciiChar = int(_buffer[i]);

                if(TOKEN_DEBUG)
                {
                    cout << "\n\ncharacter: " << _buffer[i] << endl;
                    cout << "asciiChar: " << asciiChar << endl;
                }


                //  PROC: If the character is not associated with a failstate
                //  and is a valid ascii character
                if(_table[state][asciiChar] != -1 && asciiChar > 0
                        && asciiChar < 300)
                {



                    //  PROC: Move to the next state
                    state = _table[state][asciiChar];

                    //  PROC: Check if the current state is a success state
                    if(_table[state][0] == 1)
                    {
                        //  PROC: Store the current token into the success token
                        successToken = successToken + tempToken;

                        //  PROC: Blank the token variable since we stored it's
                        //  contents in the success token
                        tempToken = "";

                        //  PROCESSING: Store the current state back into our
                        //  reference variable: endState since this was the last
                        //  successful state
                        endState = state;

                        //  PROC: Keep track of the position of the character
                        //  following the current success token
                        pos = i + 1;

                    }

                }

                //  PROCESSING: If an character is inputted that does not exist
                //  in the table, we end the loop
                else
                {
                    i = MAX_BUFFER;


                }






            }

            if(state == 0)
            {
                ++pos;
                successToken = tempToken;
            }

            //  PROCESSING: Store the positiion of the character following the
            //  success token the private member variable that keeps track of
            //  the position
            _pos = pos;



            //  PROCESSING: If we have reached the end of the buffer, we set
            //  _done to true to signify that there are no more tokens to
            //  extract
            if(isBufferDone())
            {
                _done = true;
            }


            //  PROCESSING: If a toke was extracted, return true, or else return
            //  false
            if(successToken != "")
            {
                token = successToken;
                return true;
            }

            else
            {
                return false;
            }

    }




}


/**********************************************************
*
* Accessor isBufferDone(): Class STokenizer
*_________________________________________________________
* Checks if we have extracted every token from the buffer.
* We check this by seeing if we have arrived at 1 position
* past the Null character '\0'. This is done so that the
* While loop used to extract the tokens will be able to run
* one more time, allowing it to print that last token that
* was extracted.
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
* - _done will be modified to reflect whether the buffer
*   is done or not
*
*
***********************************************************/
bool STokenizer::isBufferDone()
{

    //  PROCESSING: If the previous character that _pos is pointing to is a null
    //  '\0' then we know that every token has been extracted
    if(_buffer[_pos] == '\0')
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
* Mutator setBufferLen: Class STokenizer
*_________________________________________________________
* Sets the _bufferLen with the length of the buffer as
* it is. If there is a buffer, then we set _done to false
*_________________________________________________________
* PRE-CONDITIONS
* - A _buffer must be set
*
* POST-CONDITIONS
* - _bufferLen is set
* - _done is set
*
*
***********************************************************/
void STokenizer::setBufferLen()
{

    //  PROC: Set the bufferLen to the private member variable
    _bufferLen = strlen(_buffer);

    //  PROC: If there is a buffer then we change _done to false
    if(_bufferLen > 0)
    {
        _done = false;
    }

}

/**********************************************************
*
* Accessor pos(): Class STokenizer
*_________________________________________________________
* Returns the position in the buffer
*_________________________________________________________
* PRE-CONDITIONS
*
* POST-CONDITIONS
* - return the position
*
*
***********************************************************/
int STokenizer::pos()
{

    return _pos;

}


/**********************************************************
*
* Operator >>: Class STokenizer
*_________________________________________________________
* The extraction operator will extract one token from the
* buffer and place it in the token on the right
*_________________________________________________________
* PRE-CONDITIONS
* - s: the STokenizer on the left
* - t: the token to place the extracted token into on the right
*
* POST-CONDITIONS
*
*
***********************************************************/
STokenizer& operator >> (STokenizer& s, Token& t)
{
    string token;   //  CALC: Will hold the found token
    int endState;   //  CALC: Will hold the last state the machine was in

    //  PROCESSING: Initialize the variables
    token = "";
    endState = 0;

    //  PROCESSING: If there is another token to be extracted, then we insert
    //  that token into the token object
    if(s.get_token(token, endState))
    {
        //  PROCESSING: Place the extracted token and the ending state
        //  (signifying the type of token) into a Token object
        t = Token(token, endState);
    }



    return s;

}



#endif // STOKENIZER_H
