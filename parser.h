#ifndef PARSER_H
#define PARSER_H

#include "stokenizer.h"
#include "mmap.h"
#include "map.h"
#include "vector.h"

//  PROC: The AND must come after the OR for the order of operations to be correct
//  (for precedence)
enum keys {START = 1, SELECT, INSERT, MAKE, CREATE, BATCH, DROP, FIELDS, TABLE,  WHERE, TYPE, ALL, FROM, ALPHA,
           ASTERISK, SYMBOL, COMMA, OPERATION, VALUES, INTO, QUOTATIONS,
           OPEN_PARENTHESES, CLOSED_PARENTHESES, OR, AND};



enum states {SELECT_STATE = 30, INSERT_STATE = 50, MAKE_TABLE_STATE = 70, BATCH_STATE = 90, DROP_STATE = 95};

const bool P_DEBUG = false;

class Parser
{

public:
    Parser();

    Parser(char buf[MAX_BUFFER])
    {
        strcpy(buffer, buf);
        make_table(_table);
        create_token_map();
    }

    Parser(string str)
    {
        strcpy(buffer, str.c_str());
        buffer[str.size()] = '\0';

        make_table(_table);
        create_token_map();
    }

    void parse();



    mmap<string, string> get_parse_table();


private:


    char buffer[MAX_BUFFER];


    string remove_quotations(string str);

    void create_token_map();

    static map<string, int> token_map;

    mmap<string, string> parse_tree;
    bool successful;

    void reset();

    void make_table(int _table[MAX_ROWS][MAX_COLUMNS]);

    static int _table[MAX_ROWS][MAX_COLUMNS];

    void mark_cell(int state, int _table[][MAX_COLUMNS], int start, int end,
                   int nextState);

    void mark_state(int _table[][MAX_COLUMNS], int state, bool success);


};

int Parser::_table[MAX_ROWS][MAX_COLUMNS];

map<string, int> Parser::token_map;


mmap<string, string> Parser::get_parse_table()
{
    return parse_tree;
}

void Parser::parse()
{
    STokenizer tokenizer(buffer);
    bptree<string>::Iterator it;
    Token token;

    string condition;
    string values;

    bool value_on_deck = false;

    mmap<string, string> temp_parse_tree;

    int state = 0;




    //  PROC: Loop through each token
    while(!tokenizer.done())
    {
        tokenizer >> token;

        //  PROC: Check that the token is not a space
        if(token.type_string() != "SPACE")
        {

            if(P_DEBUG)
            {
                cout << "\n\nNot a space!\n\n";
                cout << "state: " << state << endl;
                cout << "token: " << token << endl;
                cout << "token int: " << token_map[token.token_str()] << endl;
                cout << "Enums: \n";
                cout << "Select: " << SELECT << endl;
                cout << "* : " << ASTERISK << endl;

                cout << "From: " << FROM << endl;

                cout << "SYMBOL: " << SYMBOL << endl;

            }

            //  PROC: Check if the token_map contains a recognized command,
            //  parameter. Use the specified command to access the next state
            if(token_map[token.token_str()] != 0)
            {
                if(P_DEBUG)
                {
                    cout << "found in token map\n";
                }

                state = _table[state][token_map[token.token_str()]];
            }

            //  If not, then we assume that the parameter is
            //  just a symbol (a symbol can be a table name, field name, anything
            //  but a recognized command. Use the SYMBOL to access the next
            //  state
            else {
                state = _table[state][SYMBOL];
            }

            if(P_DEBUG)
            {
                cout << "new state: " << state << endl;
            }


            //  PROC: Depending on the state that we are in, store
            //  associated parameter into the parse_tree
            switch(state) {

            case SELECT_STATE:
                temp_parse_tree["command"] += token.token_str();
                break;


            case SELECT_STATE + 1:
                temp_parse_tree["fields"] += token.token_str();
                break;

            case SELECT_STATE + 4:
                temp_parse_tree["table"] += token.token_str();
                break;


            case SELECT_STATE + 6:
                condition = condition + token.token_str() + ",";
                break;

            case SELECT_STATE + 7:
                condition = condition + token.token_str();
                break;


            case SELECT_STATE + 8:
                //  PROC: Append the token to condition. Since this is the
                //  last state, append the full condition to the parse_tree
                condition = condition + token.token_str();
                break;

            case SELECT_STATE + 9:

                //  PROC: Put single quotes at the beginning of the value
                condition = condition + "'";
                break;

            case SELECT_STATE + 10:


                //  PROC: If there is already a value after the quotations,
                //  then put a space between that value and the next part of
                //  the value string
                if(value_on_deck)
                {
                    condition = condition + " " + token.token_str();
                }

                //  PROC: If there is no value after the quotations, insert
                //  the value without spaces so that it will be surrounded by
                //  'example' without any spaces.
                else
                {
                    condition = condition + token.token_str();
                    value_on_deck = true;
                }



                break;

            case SELECT_STATE + 11:

                //  PROC: Since we have passed from state 10, we can set value
                //  on deck to false
                value_on_deck = false;

                //  PROC: End the value with a single quote
                condition = condition + "'";

                //  PROC: If there is already conditions stored in the temp
                //  parse tree, then we append the new conditions. Or elese
                //  store the conditions for the first time
                if(temp_parse_tree.contains("conditions"))
                {

                    (*it).append(condition);

                    //  PROC: Clear condition
                    condition = "";
                }
                else
                {
                    temp_parse_tree["conditions"] += condition;
                    it = temp_parse_tree["conditions"].begin();

                    //  PROC: Clear condition
                    condition = "";
                }

                break;

            case SELECT_STATE + 12:

                //  PROC: Store a closed parentheses
                condition = condition + "," + token.token_str();
                break;

            case SELECT_STATE + 13:

                //  PROC: store the and || or operator
                condition = condition + "," + token.token_str() + ",";
                break;

            case SELECT_STATE + 14:

                condition = condition + token.token_str();

                //  PROC: If there is already conditions stored in the temp
                //  parse tree, then we append the new conditions. Or elese
                //  store the conditions for the first time
                if(temp_parse_tree.contains("conditions"))
                {

                    (*it).append(condition);

                    //  PROC: Clear condition
                    condition = "";
                }
                else
                {
                    temp_parse_tree["conditions"] += condition;
                    it = temp_parse_tree["conditions"].begin();

                    //  PROC: Clear condition
                    condition = "";
                }

                break;


            case SELECT_STATE + 15:
                //  PROC: Append the token to condition. Since this is the
                //  last state, append the full condition to the parse_tree
                condition = condition + "," +  token.token_str();

                //  PROC: If there is already conditions stored in the temp
                //  parse tree, then we append the new conditions. Or elese
                //  store the conditions for the first time
                if(temp_parse_tree.contains("conditions"))
                {

                    (*it).append(condition);

                    //  PROC: Clear condition
                    condition = "";
                }
                else
                {
                    temp_parse_tree["conditions"] += condition;
                    it = temp_parse_tree["conditions"].begin();

                    //  PROC: Clear condition
                    condition = "";
                }

                break;




            case INSERT_STATE:
                temp_parse_tree["command"] += token.token_str();
                break;

            case INSERT_STATE + 2:
                temp_parse_tree["table"] += token.token_str();
                break;

            case INSERT_STATE + 4:
                if(temp_parse_tree.contains("values"))
                {

                    (*it).append(", " + token.token_str());
                }
                else
                {
                    temp_parse_tree["values"] += token.token_str();
                    it = temp_parse_tree["values"].begin();
                }
                break;




            case MAKE_TABLE_STATE:
                temp_parse_tree["command"] += "make table";
                break;

            case MAKE_TABLE_STATE + 2:
                temp_parse_tree["table"] += token.token_str();
                break;

            case MAKE_TABLE_STATE + 4:

                //  PROC: If a field has already been placed into the parse
                //  tree, then we append it to that element's string
                //  (We do this because we do not want the bplustree to sort
                //  the fields. we need them in order received). We only
                //  maintain one element in the values of the map and we
                //  append to it
                if(temp_parse_tree.contains("fields"))
                {

                    (*it).append("," + token.token_str());
                }

                //  PROC: If no field has been appended yet. Add the token
                //  to the multimap and then create an iterator that points to
                //  it so that we can append if we receive another value
                else
                {
                    temp_parse_tree["fields"] += token.token_str();
                    it = temp_parse_tree["fields"].begin();
                }
                break;


            case BATCH_STATE:
                temp_parse_tree["command"] += token.token_str();
                break;


            case BATCH_STATE + 1:
               //  PROC: If symbol has already been placed into the parse
               //  tree, then we append it to that element's string
               //  (We do this because we do not want the bplustree to sort
               //  the fields. we need them in order received). We only
               //  maintain one element in the values of the map and we
               //  append to it
               if(temp_parse_tree.contains("filename"))
               {

                   (*it).append(remove_quotations(token.token_str()));
               }

               //  PROC: If no symbol has been appended yet. Add the token
               //  to the multimap and then create an iterator that points to
               //  it so that we can append if we receive another value
               else
               {
                   temp_parse_tree["filename"] += remove_quotations(token.token_str());
                   it = temp_parse_tree["filename"].begin();
               }
               break;



            case DROP_STATE:
                temp_parse_tree["command"] += token.token_str();
                it = temp_parse_tree["command"].begin();
                break;

            case DROP_STATE + 1:
                //  PROC: If symbol has already been placed into the parse
                //  tree, then we append it to that element's string
                //  (We do this because we do not want the bplustree to sort
                //  the fields. we need them in order received). We only
                //  maintain one element in the values of the map and we
                //  append to it
                if(temp_parse_tree.contains("command"))
                {

                    (*it).append(" " + remove_quotations(token.token_str()));
                }

                break;


            case DROP_STATE + 2:
                temp_parse_tree["table"] += token.token_str();
                break;



            }









            //  PROC: Since we are done appending to our temp_parse_tree,
            //  check if we are in a success state so that we can store
            //  our temp_parse_tree into our success parse_tree
            if(_table[state][0] == 1)
            {
                if(P_DEBUG)
                {
                    cout << "we have entered a success state\n";

                    cout << "insert the temp_parse_tree_into the main parse_tree\n";
                }


                parse_tree = temp_parse_tree;
            }



        }


    }


}

void Parser::create_token_map()
{
    token_map["select"] = SELECT;

    token_map["*"] = ASTERISK;

    token_map["from"] = FROM;
    token_map[","] = COMMA;

    token_map["where"] = WHERE;

    token_map["="] = OPERATION;
    token_map[">"] = OPERATION;

    token_map[">="] = OPERATION;
    token_map["<"] = OPERATION;
    token_map["<="] = OPERATION;

    token_map["values"] = VALUES;

    token_map["insert"] = INSERT;

    token_map["into"] = INTO;

    token_map["and"] = AND;

    token_map["or"] = OR;

    token_map["("] = OPEN_PARENTHESES;

    token_map[")"] = CLOSED_PARENTHESES;

    token_map["\""] = QUOTATIONS;

    token_map["create"] = CREATE;
    token_map["make"] = MAKE;
    token_map["table"] = TABLE;
    token_map["fields"] = FIELDS;


    token_map["batch"] = BATCH;

    token_map["drop"] = DROP;


}

/**********************************************************
*
* Mutator make_table(): Class Parser
*_________________________________________________________
* Creates a table that will allow us to identify commands,
* tables, and conditions
*_________________________________________________________
* PRE-CONDITIONS
*
* int _table[][]: The table that we are modifying
*
* POST-CONDITIONS
*
*
***********************************************************/
void Parser::make_table(int _table[MAX_ROWS][MAX_COLUMNS])
{

    enum keys key;


    for(int h = 0; h < MAX_ROWS; h++)
    {
        for(int i = 0; i < MAX_COLUMNS; i++)
        {
            _table[h][i] = -1;
        }
    }

    //  PROC: We will mark the starting state. The first command entered
    //  will move us out of this state to that commands state
    mark_cell(START_STATE, _table, SELECT, SELECT, SELECT_STATE);
    mark_cell(START_STATE, _table, INSERT, INSERT, INSERT_STATE);
    mark_cell(START_STATE, _table, MAKE, MAKE, MAKE_TABLE_STATE);
    mark_cell(START_STATE, _table, CREATE, CREATE, MAKE_TABLE_STATE);
    mark_cell(START_STATE, _table, BATCH, BATCH, BATCH_STATE);
    mark_cell(START_STATE, _table, DROP, DROP, DROP_STATE);





    //  PROC: This is the beginning of the Select state. If we get a field or
    //  an asterisk, we will move to the next state.
    mark_cell(SELECT_STATE, _table, ASTERISK, ASTERISK, SELECT_STATE + 1);
    mark_cell(SELECT_STATE, _table, SYMBOL, SYMBOL, SELECT_STATE + 1);


    //  PROC: This is the Field state. We need from or a comma to move to the
    //  next state.
    mark_cell(SELECT_STATE + 1, _table, FROM, FROM, SELECT_STATE + 3);
    mark_cell(SELECT_STATE + 1, _table, COMMA, COMMA, SELECT_STATE + 2);

    //  PROC: This is the Comma state. We need a field after this state and will
    //  return to the field state
    mark_cell(SELECT_STATE + 2, _table, SYMBOL, SYMBOL, SELECT_STATE + 1);

    //  PROC: This is the FROM state. we need to get a table to get to the next
    //  state
    mark_cell(SELECT_STATE + 3, _table, SYMBOL, SYMBOL, SELECT_STATE + 4);

    //  PROC: Mark state 4 as a success state
    mark_state(_table, SELECT_STATE + 4, true);

    //  PROC: This is a SUCCESS STATE. It is possible to get a WHERE after this
    //  but if we dont receive anything, we have enough information to perform
    //  a select command
    mark_cell(SELECT_STATE + 4, _table, WHERE, WHERE, SELECT_STATE + 5);

    //  PROC: This is a WHERE STATE. We are looking for a symbol
    mark_cell(SELECT_STATE + 5, _table, SYMBOL, SYMBOL, SELECT_STATE + 7);
    mark_cell(SELECT_STATE + 5, _table, OPEN_PARENTHESES, OPEN_PARENTHESES, SELECT_STATE + 6);

    mark_cell(SELECT_STATE + 6, _table, SYMBOL, SYMBOL, SELECT_STATE + 7);

    mark_cell(SELECT_STATE + 7, _table, OPERATION, OPERATION, SELECT_STATE + 8);

    mark_cell(SELECT_STATE + 8, _table, SYMBOL, SYMBOL, SELECT_STATE + 14);
    mark_cell(SELECT_STATE + 8, _table, QUOTATIONS, QUOTATIONS, SELECT_STATE + 9);

    mark_cell(SELECT_STATE + 9, _table, SYMBOL, SYMBOL, SELECT_STATE + 10);

    mark_cell(SELECT_STATE + 10, _table, SYMBOL, SYMBOL, SELECT_STATE + 10);
    mark_cell(SELECT_STATE + 10, _table, QUOTATIONS, QUOTATIONS, SELECT_STATE + 11);

    mark_state(_table, SELECT_STATE + 11, true);
    mark_cell(SELECT_STATE + 11, _table, CLOSED_PARENTHESES, CLOSED_PARENTHESES, SELECT_STATE + 12);
    mark_cell(SELECT_STATE + 11, _table, AND, AND, SELECT_STATE + 13);
    mark_cell(SELECT_STATE + 11, _table, OR, OR, SELECT_STATE + 13);

    mark_state(_table, SELECT_STATE + 12, true);
    mark_cell(SELECT_STATE + 12, _table, AND, AND, SELECT_STATE + 13);
    mark_cell(SELECT_STATE + 12, _table, OR, OR, SELECT_STATE + 13);

    mark_cell(SELECT_STATE + 13, _table, SYMBOL, SYMBOL, SELECT_STATE + 7);
    mark_cell(SELECT_STATE + 13, _table, OPEN_PARENTHESES, OPEN_PARENTHESES, SELECT_STATE + 6);

    mark_state(_table, SELECT_STATE + 14, true);
    mark_cell(SELECT_STATE + 14, _table, AND, AND, SELECT_STATE + 13);
    mark_cell(SELECT_STATE + 14, _table, OR, OR, SELECT_STATE + 13);
    mark_cell(SELECT_STATE + 14, _table, CLOSED_PARENTHESES, CLOSED_PARENTHESES, SELECT_STATE + 15);

    mark_state(_table, SELECT_STATE + 15, true);
    mark_cell(SELECT_STATE + 15, _table, AND, AND, SELECT_STATE + 13);
    mark_cell(SELECT_STATE + 15, _table, OR, OR, SELECT_STATE + 13);








//    //  PROC: INSERT_INTO command

    mark_cell(INSERT_STATE, _table, INTO, INTO, INSERT_STATE + 1);

    mark_cell(INSERT_STATE + 1, _table, SYMBOL, SYMBOL, INSERT_STATE + 2);

    mark_cell(INSERT_STATE + 2, _table, VALUES, VALUES, INSERT_STATE + 3);

    mark_cell(INSERT_STATE + 3, _table, SYMBOL, SYMBOL, INSERT_STATE + 4);
    mark_state(_table, INSERT_STATE + 4, true);

    mark_cell(INSERT_STATE + 4, _table, COMMA, COMMA, INSERT_STATE + 5);

    mark_cell(INSERT_STATE + 5, _table, SYMBOL, SYMBOL, INSERT_STATE + 4);


    //  PROC: MAKE TABLE COMMAND
    mark_cell(MAKE_TABLE_STATE, _table, TABLE, TABLE, MAKE_TABLE_STATE + 1);
    mark_cell(MAKE_TABLE_STATE + 1, _table, SYMBOL, SYMBOL, MAKE_TABLE_STATE + 2);
    mark_cell(MAKE_TABLE_STATE + 2, _table, FIELDS, FIELDS, MAKE_TABLE_STATE + 3);
    mark_cell(MAKE_TABLE_STATE + 3, _table, SYMBOL, SYMBOL, MAKE_TABLE_STATE + 4);


    mark_cell(MAKE_TABLE_STATE + 4, _table, COMMA, COMMA, MAKE_TABLE_STATE + 5);
    mark_state(_table, MAKE_TABLE_STATE + 4, true);

    mark_cell(MAKE_TABLE_STATE + 5, _table, SYMBOL, SYMBOL, MAKE_TABLE_STATE + 4);


    //  PROC: BATCH COMMAND
    mark_cell(BATCH_STATE, _table, SYMBOL, SYMBOL, BATCH_STATE + 1);
    mark_cell(BATCH_STATE + 1, _table, SYMBOL, SYMBOL, BATCH_STATE + 1);
    mark_state(_table, BATCH_STATE + 1, true);

    //  PROC: DROP TABLE COMMAND
    mark_cell(DROP_STATE, _table, TABLE, TABLE, DROP_STATE + 1);
    mark_cell(DROP_STATE + 1, _table, SYMBOL, SYMBOL, DROP_STATE + 2);
    mark_state(_table, DROP_STATE + 2, true);










}

/**********************************************************
*
* Mutator mark_cell(): Class STokenizer
*_________________________________________________________
* Marks the specified state so that the specified command will point
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
void Parser::mark_cell(int state, int _table[][MAX_COLUMNS],
                           int start, int end, int nextState)
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
void Parser::mark_state(int _table[][MAX_COLUMNS], int state, bool success)
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

string Parser::remove_quotations(string str)
{
    //  PROC: Remove the quotation marks at the beginning and the end of the
    //  string
//    if(str.at(0) == '\"' && str.at(str.size() - 1) == '\"')
//    {
//        str.erase(str.begin());
//        str.erase(str.end() - 1);
//    }

    //  PROC: Removes all quotation marks from string
    str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());

    return str;
}

#endif // PARSER_H
