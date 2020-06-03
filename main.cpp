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




int main()
{
    SQL *s;

    s = new SQL;



    s->run();

    delete s;




    return 0;
}
