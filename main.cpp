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


int main()
{
    SQL *s;

    s = new SQL;



    s->run();

    delete s;

//    Condition c({1, 3, 4, 5, 6, 10, 11, 12});

//    Condition d({1, 2, 5, 8, 10, 12});

//    Condition e = c && d;

//    cout << e.get_record_nums().to_string() << endl;






    return 0;
}
