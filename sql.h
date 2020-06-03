#ifndef SQL_H
#define SQL_H

#include <string>
#include <iostream>
#include "../includes/parser.h"
#include "../includes/table.h"





using namespace std;

class SQL
{

public:


    void run();
    void run_command(string command);

    ~SQL() {
        tables["employees"].remove_temp_files();
        remove("employees.sql");
    }


private:

    map<string, Table> tables;

    const bool DEBUG = false;




};

void SQL::run_command(string input)
{



    string table;               //  PROC: Holds the name of the table
    string command;             //  PROC: Holds the name of the command
    Vector<string> fields;      //  PROC: Holds the fields
    Vector<string> conditions;  //  PROC: Holds the conditions

    mmap<string, string> parse_tree;    //  PROC: The command parsed into a map

    //  PROC: Feed the command input to the parser
    Parser parser(input);

    //  PROC: Parse the input
    parser.parse();

    //  PROC: Retreive the parse_tree
    parse_tree = parser.get_parse_table();



    if(!parse_tree.empty())
    {
        //  PROC: Store the table, command, fields, and conditions into separate
        //  variables
        table = parse_tree["table"].to_vector().at(0);
        command = parse_tree["command"].to_vector().at(0);
        fields = parse_tree["fields"].to_vector();
        conditions = parse_tree["conditions"].to_vector();

        if(DEBUG)
        {
            cout << "\n\ncalled run_command(string command)\n";
            cout << "input: " << input << endl;
            cout << "parse_tree: \n" << parse_tree << endl << endl;
            cout << "table: " << table << endl;
            cout << "command: " << command << endl;
            cout << "fields: " << fields.to_string() << endl;
            cout << "conditions: " << conditions.to_string() << endl;
        }


        //  PROC: Make sure that the parse_tree only has 1 command and 1 table
        assert(parse_tree["command"].to_vector().size() == 1);
        assert(parse_tree["table"].to_vector().size() == 1);


        //  PROC: If the command is "select" then perform a select
        if(command == "select")
        {
            if(tables.contains(table))
            {
                try {



                    Table tab = tables[table].select(parser);


                    tab.select_all();













                } catch (const char* msg) {

                    cout << msg << endl;
                    cout << "select [fields] from [table]\n";
                    cout << "select [fields] from [table] where [field]=[value]\n";

                }

            }

        }
    }




}


void SQL::run()
{

//    Table table("employees", {"first", "last", "major"});

    tables.insert("employees", Table("employees", {"first", "last", "major"}));

    tables["employees"].insert({"Alex", "Rodriguex", "Art"});
    tables["employees"].insert({"Nick", "Spector", "CS"});
    tables["employees"].insert({"Zachary", "Sierra", "CS"});
    tables["employees"].insert({"Alex", "Anderson", "Engineer"});
    tables["employees"].insert({"Alex", "Jones", "Pyschology"});
    tables["employees"].insert({"Nick", "M", "loser"});
    tables["employees"].insert({"John", "Sierra", "CS"});
    tables["employees"].insert({"David", "Koresh", "art"});


    tables["employees"].select_all();





    cout << "\n\nThis is a table called 'employees' with 8 records in it.\n";
    cout << "At the moment, only the 'select' command works. You can not filter"
            " for fields (only * works). where is fully functioning.\n\n";

    while(1)
    {
        string str;

        cout << endl;

        cout << "Command: ";
        getline(cin, str);

        if(str == "exit")
        {


            break;
        }



        run_command(str);

        cout << endl;


    }
}


#endif // SQL_H
