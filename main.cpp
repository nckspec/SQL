#include <iostream>
#include "sql.h"
#include <string.h>

/**************************************************************************
* Author:           Nicholas Spector
* Assignment:       SQL Class
* Class:            CS8
* Due Date:         6/1/20
**************************************************************************/

using namespace std;

const bool SQL_DEBUG = false;

#include <cstdlib>

/*
  TODO: Modify string_to_vector so that it doesn't separate numbers from
  words

  - Table()::get_records() returns a vector of records and could possibly crash
  the program if it is used on a massive table. It would be better if only one
  record was retreived at one time. This is only used when opening the tables.sql
  table that holds all of the tables, so it is unlikely that there would be many
  records in that

*/


/*
 *
 * - Make it so that the SQL class saves all of the names of created tables into
 * a table that it reads from when constructed and then loads each of the table
 * objects into the map
 *
 *
 *
 *
 *
 */

//  BUGS:
//  If you don't put an end quote when, the program will enter an endless loop
//  ex. batch "file name.txt
//  FIX: Update the tokenizer so that it updates _pos even if it did not
//  get a successful token
//

//  - If you do not close a parenthesis in your conditions, then the program
//  will crash. If you put a closed parenthesis without open, the program will
//  crash

Vector<mmap<string, long > > value_map;

//  PROC: This is a map that allows you to put in the field type and it
//  will provide you with an int that points to the associated index
//  in the value_map
map<string, unsigned int> field_map;

void insert_field_map(string field, string value, long record_num)
{
    if(SQL_DEBUG)
    {
        cout << "\n\ncalled Table::insert_field_map(string field, string value, unsigned int record_nums)\n";
        cout << "field : " << field << endl;
        cout << "value : " << value << endl;
        cout << "record_nums: " << record_num << endl << endl;
    }

    //  PROC: Insert the record num into the multimap at the field/value
    //  associated
    value_map.at(field_map[field])[value] += record_num;

    if(SQL_DEBUG)
    {
        cout << "\n end of Table::insert_field_map(string field, string value, unsigned int record_nums)\n";
        cout << "field_map: \n" << field_map << endl << endl;

        for(unsigned int i = 0; i < value_map.size(); i++)
        {
            cout << value_map.at(i) << endl;
        }

    }
}

void setup_field_map(Vector<string> field_names)
{
    if(SQL_DEBUG)
    {
        cout << "\n\ncalled setup_field_map(Vector<string> field_names)\n";
        cout << "field_names: " << field_names.to_string() << endl << endl;
    }

    //  PROC: Create the field_map and the value_map
    for(unsigned int i = 0; i < field_names.size(); i++)
    {
        //  PROC: Put the field name as the index of the map and use i to
        //  store the associated index for the vector
        field_map[field_names.at(i)] = i;

        //  PROC: Store empty mmaps into the value_map
        value_map.push_back(mmap<string, long>());
    }

    if(SQL_DEBUG)
    {
        cout << "\n\end of setup_field_map(Vector<string> field_names)\n";
        cout << "field_map: \n" << field_map << endl << endl;

        for(unsigned int i = 0; i < value_map.size(); i++)
        {
            cout << value_map.at(i) << endl;
        }

    }
}

void test_finding_conditions()
{
    setup_field_map({"first", "last", "major"});



    insert_field_map("first", "alex roger", 1);
    insert_field_map("last", "sierra", 1);
    insert_field_map("major", "art", 1);

    insert_field_map("first", "alex", 2);
    insert_field_map("last", "anderson", 2);
    insert_field_map("major", "cs", 2);

    insert_field_map("first", "roger roger", 3);
    insert_field_map("last", "math", 3);
    insert_field_map("major", "chinese", 3);

    insert_field_map("first", "nick", 4);
    insert_field_map("last", "spector", 4);
    insert_field_map("major", "cs", 4);

    insert_field_map("first", "nick", 5);
    insert_field_map("last", "zach", 5);
    insert_field_map("major", "LA", 5);

    insert_field_map("first", "roger", 6);
    insert_field_map("last", "spector", 6);
    insert_field_map("major", "house wife", 6);

    insert_field_map("first", "john", 7);
    insert_field_map("last", "anderson", 7);
    insert_field_map("major", "cs", 7);

    insert_field_map("first", "thomas the train", 8);
    insert_field_map("last", "sierra", 8);
    insert_field_map("major", "cs", 8);




    Vector<string> commands;

    commands.push_back("select * from employees where last=spector or last=math and first=roger");

    commands.push_back("select * from employees where first=alex or first = nick and last=zach");

    commands.push_back("select * from employees where first=\"roger roger\" or first = nick and last=zach");

    commands.push_back("select * from employees where first=alex");

    commands.push_back("select * from employees where first>nick");

    commands.push_back("select * from employees where major=\"house wife\" or first=thomas");


    commands.push_back("select * from employees where first=\"thomas the train\" or (first=\"john\" and last=\"anderson\") or last=sierra");

    commands.push_back("select * from employees where (first >= alex and first < nick) and (last=sierra or first=john)");

    commands.push_back("select * from employees where (first=\"thomas the train\" or first=\"john\") and last=\"anderson\"");\
    for(unsigned int i = 0; i < commands.size(); i++)
    {


        cout << "\n\n----------------------------------------------------\n\n";
        Parser p(commands.at(i));

        p.parse();

        mmap<string, string> m = p.get_parse_table();


        cout << m << endl;

        string conditions = m["conditions"].to_vector().to_string();

        cout << "conditions: " << conditions << endl;

        try {

            cout << "conditions_to_vector: " << conditions_to_vector(conditions).to_string() << endl;

            Shaunting_Yard yard(conditions_to_vector(conditions), field_map, value_map);

            cout << "postfix: " << yard.get_postfix().to_string() << endl;
            cout << "postfix: " << yard.get_postfix() << endl;

            cout << "\n\nrecord_nums: " << yard.rpm().to_string() << endl;

        } catch (string str) {

            cout << str << endl;

        }

    }
}

void test_parser()
{

    Vector<string> tests;

    tests.push_back("batch woah.txt");
    tests.push_back("batch \"this is a test.txt\"");
    tests.push_back("batch \"_!select (2).text\"");
    tests.push_back("batch \"_!select.txt\"");

    tests.push_back("batch test");

    tests.push_back("batch this is a test.txt");


    for(unsigned int i = 0; i < tests.size(); i++)
    {
        Parser p(tests.at(i));

        p.parse();

        mmap<string, string> parse_tree = p.get_parse_table();

        cout << parse_tree << endl;
    }


}


int main()
{
    SQL *s;

    s = new SQL;



    s->run();

    delete s;

//    test_finding_conditions();






    return 0;
}
