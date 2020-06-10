#include <iostream>
#include "sql.h"

/**************************************************************************
* Author:           Nicholas Spector
* Assignment:       SQL Class
* Class:            CS8
* Due Date:         6/1/20
**************************************************************************/

using namespace std;

#include <cstdlib>

//  TODO: Modify string_to_vector so that it doesn't separate numbers from
//  words
//

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



void test_parser()
{

    Vector<string> tests;

    tests.push_back("batch woah.txt");
    tests.push_back("batch \"this is a test.txt\"");
    tests.push_back("batch \"_!select (2).text\"");
    tests.push_back("batch \"_!select.txt\"");

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
//    SQL *s;

//    s = new SQL;



//    s->run();

//    delete s;

    test_parser();






    return 0;
}
