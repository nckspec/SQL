#ifndef SQL_H
#define SQL_H

#include <string>
#include <iostream>
#include "parser.h"
#include "table.h"

#include <algorithm>





using namespace std;

class SQL
{

public:

    SQL();
    void run();
    void run_command(string command);

    ~SQL() {
        remove_temp_files();
    }


private:

    map<string, Table> tables;

    void load_table(string table_name);

    bool make_table(string table_name, Vector<string> table_fields);

    const bool DEBUG = false;

    void remove_temp_files();
    void run_batch_file(string file_name);




};

void SQL::remove_temp_files()
{
    for(map<string, Table>::Iterator it = tables.begin(); it != tables.end(); it++)
    {
        (*it).value.remove_temp_files();
    }
}




bool SQL::make_table(string table_name, Vector<string> table_fields)
{
    //  PROC: Check that the table does not already exist
    if(!tables.contains(table_name))
    {
        tables.insert(table_name, Table(table_name, table_fields));


        //  PROC: Write the table name to the table that holds all of the
        //  table names (table.sql)
        tables["tables"].insert({table_name});

        return true;

    }

    else
    {
        return false;
        throw "The table already exists!";
    }

}

void SQL::load_table(string table_name)
{
    //  PROC: Try to construct a table using the constructor that
    //  attempts to read from the table's sql file. If the table
    //  does not exist or it is corrupted, it will throw an exception
    try {

        Table table(table_name);
        tables[table_name] = table;
        cout << "\n\nLoaded table '" << table_name << "'\n\n";

    } catch (string str)
    {
        cout << str << endl;
    }

}

SQL::SQL()
{
    Vector<Record> records;
    string table_name = "tables";

    //  PROC: Either open or create tables.sql
    try
    {
        if(DEBUG)
        {
            cout << "\n\nOpening tables.sql\n\n";
        }

        //  PROC: Try to open tables.sql. If it can be opened, store it in
        //  the tables map
        Table table(table_name);
        tables[table_name] = table;

    } catch (string str)
    {
        cout << str << endl;


        if(DEBUG)
        {
            cout << "\n\nCreating tables.sql\n\n";
        }

        //  PROC: Since the tables.sql does not exist. Create it with the field
        //  'name' that will hold the name of all tables
        Table table(table_name, {"name"});
        tables[table_name] = table;

    }

    try {

        //  PROC: Get all of the records from 'tables'
        records = tables["tables"].get_records();


    } catch (string str) {



    }



    //  PROC: Loop through each record from the table 'tables' which holds all
    //  of the names of the table for the database
    for(unsigned int i = 0; i < records.size(); i++)
    {
        if(DEBUG)
        {
            cout << "\n\nRecord: " << records.at(i).get_fields() << endl;

        }

        //  PROC: Get the 1st field 'name' from the record and call load_table
        //  with the name
        load_table(records.at(i).get_fields().at(0));
    }

}

void SQL::run_batch_file(string file_name)
{

    ifstream file;
    string line;

    //  PROC: Check if the file exists
    if(file_exists(file_name.c_str()))
    {

        cout << "opening file";

        //  PROC: open the file
        file.open(file_name.c_str());

        cout << endl;

        while(!file.eof())
        {
            getline(file, line);


//            for(unsigned int i = 0; i < line.size(); i++)
//            {
//                if(line.at(i) == '\n' || line.at(i) == '\t')
//                {
//                    line.erase(line.begin() + i);
//                }
//            }

            line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());


            cout << line << endl;

            run_command(line);

        }




        file.close();

    }

    else
    {
        throw string("The file could not be opened!");
    }



}

void SQL::run_command(string input)
{



    string table;               //  PROC: Holds the name of the table
    string command;             //  PROC: Holds the name of the command
    Vector<string> fields;      //  PROC: Holds the fields
    Vector<string> conditions;  //  PROC: Holds the conditions
    Vector<long> record_nums;   //  PROC: Used to hold record_nums that we
                                //  pull when targeting specific records

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
        if(parse_tree.contains("table"))
        {
            table = parse_tree["table"].to_vector().at(0);
            assert(parse_tree["table"].to_vector().size() == 1);
        }


        if(parse_tree.contains("command"))
        {
            command = parse_tree["command"].to_vector().at(0);

            //  PROC: Make sure that the parse_tree only has 1 command and 1 table
            assert(parse_tree["command"].to_vector().size() == 1);
        }



        if(parse_tree.contains("fields"))
        {
            fields = parse_tree["fields"].to_vector();
        }

        if(parse_tree.contains("conditions"))
        {
            fields = parse_tree["conditions"].to_vector();
        }



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






        //  PROC: If the command is "select" then perform a select
        if(command == "select")
        {
            //  PROC: Check that the table exists
            if(tables.contains(table))
            {
                try {


                    //  PROC: Create a new table with the records that were
                    //  requested from the original
                    Table tab = tables[table].select(parser);

                    //  PROC: List the records
                    cout << tab << endl;

                    cout << endl;




                } catch (string msg) {

                    cout << msg << endl;
                    cout << "select [fields] from [table]\n";
                    cout << "select [fields] from [table] where [field]=[value]\n";

                }

            }

        }

        if(command == "insert")
        {
            //  PROC: Check that the table exists
            if(tables.contains(table))
            {
                //  PROC: Attempt to insert the new record into the table
                try
                {
                    //  PROC: We call string_to_vector() because the vector
                    //  received is onyl going to be one element where all of
                    //  the values are separated by commas
                    tables[table].insert(string_to_vector(parse_tree["values"].to_vector().to_string()));
//                    cout << tables[table] << endl;
                }

                catch (const char* msg)
                {
                    cout << msg << endl;
                    cout << "insert into [table] values [value1,value2,value3]\n";
                }
            }
        }


        if(command == "make table")
        {
            //  PROC: Make the table with the provided table name and fields
            //  (fields has to be converted into a string and back to a vector
            //  because it only has one element where all the fields are separated
            //  by commas)
            if(make_table(table, string_to_vector(fields.to_string())))
            {
                cout << "\n\nTable '" << table << "' has been created\n\n";
            }
        }

        if(command == "batch")
        {

            try {

                if(parse_tree.contains("filename"))
                {
                    run_batch_file(parse_tree["filename"].to_vector().at(0));
                }



            } catch (string exc) {

                cout << "Could not run batch file!\nError: " << exc << endl;

            }

        }

        if(command == "drop table")
        {



            //  PROC: Query the table 'tables.sql' for the record # of the record
            //  in 'tables.sql' that holds the name of the table we are trying
            //  to dlete
            record_nums = tables["tables"].select_conditions({"name=" + table});


            //  PROC: If a record # was returned, then we delete the table
            //  name from 'tables.sql' then we remove its temp files, then
            //  we delete the database file itself

            tables["tables"].drop_records(record_nums);
            tables[table].remove_temp_files();
            remove(tables[table].get_filename().c_str());

                cout << "\n\nTable: " << table << " has been dropped.\n\n";


            //  PROC: Put a blank table into the tables map
            tables[table] = Table();


        }

        if(command == "delete")
        {
            //  PROC: If the table exists
            if(tables.contains(table))
            {
                //  PROC: Drop the records from the table
                tables[table].drop_records(parse_tree);

            }

        }

    }




}


void SQL::run()
{

//    Table table("employees", {"first", "last", "major"});

//    tables.insert("employees", Table("employees", {"first", "last", "major"}));

//    tables["employees"].insert({"Alex", "Rodriguex", "Art"});
//    tables["employees"].insert({"Nick", "Spector", "CS"});
//    tables["employees"].insert({"Zachary", "Sierra", "CS"});
//    tables["employees"].insert({"Alex", "Anderson", "Engineer"});
//    tables["employees"].insert({"Alex", "Jones", "Pyschology"});
//    tables["employees"].insert({"Nick", "M", "loser"});
//    tables["employees"].insert({"John", "Sierra", "CS"});
//    tables["employees"].insert({"David", "Koresh", "art"});


//    tables["employees"].select_all();

//    tables["tables"].insert({"employees"});


//    if(make_table("newtable", {"first", "second", "third"}))
//    {
//        cout << "new table made: " << "big_table" << endl << endl;
//    }


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

        if(str == "batch")
        {
            run_batch_file("test.txt");
        }





        run_command(str);

        cout << endl;


    }
}


#endif // SQL_H
