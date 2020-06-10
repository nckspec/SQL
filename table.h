#ifndef TABLE_H
#define TABLE_H


//  NOTES: The ostream on this object does not have a const


#include "vector.h"
#include "record.h"
#include "mmap.h"
#include "parser.h"
#include "condition.h"
#include <type_traits>
#include "shaunting_yard.h"
#include <iomanip>
#include <sstream>




//  TODO: Create a copy constructor that creates a whole new file instead
//  of keeping the same file. Right now it keeps the same file so that
//  we can store the Table into the map of the SQL class without making a new
//  file. The fix is to use .insert on the map when inserting so that it
//  put the Table directly in without having to call any type of copy constructor
//  or assignment operator

using namespace std;

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);
bool is_padded(string str);
bool is_number(string str);
string pad_number(string str);

Vector<string> conditions_to_vector(string str);

const int FILE_NAME_SIZE = 20;
const int RECORD_SIZE = 30;
const int MAX_FIELDS = 10;
const bool TABLE_DEBUG = false;
const int NUM_PADDING_SIZE = 4;

class Table
{

public:

    ~Table();

    Table();

    friend class Table;


    Table(string name);
    Table(string name, Vector<string> field_names);
    Table(string name, Vector<string> field_names, Vector<Record> records);
//    Table(const Table &table);

    void insert(Vector<string> field_inserts);
    string select_all();
    Table select(Parser parser);
    Vector<long> select_conditions(Vector<string> conditions);

    Vector<unsigned int> select_fields(Vector<string> fields);

//    Table & operator =(const Table &table);


    Vector<Record> get_records(Vector<long> record_nums);
    Record get_record(long record_num);
    Vector<Record> get_records();

    void erase();

    friend ostream & operator << (ostream &out,       //  IN: Ostream object
                                   Table &table)
    {
        out << table.select_all();
        return out;
    }

    void remove_temp_files();

    bool drop_record(long record_num);

    void drop_records(Vector<long> record_nums);
    void drop_records(mmap<string, string> &parse_table);

    string get_filename() const;







private:

    string name;
    Vector<mmap<string, long > > value_map;

    //  PROC: This is a map that allows you to put in the field type and it
    //  will provide you with an int that points to the associated index
    //  in the value_map
    map<string, unsigned int> field_map;



    Vector<string> field_names;
    Vector<long> record_nums;           //  CALC: Holds the record_nums of
                                        //  every non deleted record in the
                                        //  table
    size_t record_size;
    char file_name[FILE_NAME_SIZE];


    void copy(const Table &table);

    void insert_field_map(string field, string value, long record_num);
    void setup_field_map(Vector<string> field_names);


    bool DEBUG = false;



};

string Table::get_filename() const
{
    return file_name;
}

void Table::erase()
{
    remove(file_name);
}

Table::Table()
{

}


/**********************************************************
*
* Accessor get_records(): Class Table
*_________________________________________________________
* Get the specified records from the database
*_________________________________________________________
* PRE-CONDITIONS
*
* Vector record_nums    :   The record numbers for the records
*                           that we are retreiving
*
* POST-CONDITIONS
* - This will return the Records as a vector of Record objects
*
*
***********************************************************/
Vector<Record> Table::get_records(Vector<long> record_nums)
{
    Vector<Record> records;     //  CALC: Vector holds all of the records that
                                //  we are fetching

    fstream file;

    if(TABLE_DEBUG)
    {
        cout << "\n\nTable::get_records(Vector<unsigned int> record_nums)\n";
        cout << "record_nums: " << record_nums.to_string() << endl << endl;

    }

    //  PROC: open the file
    open_fileRW(file, file_name);

    //  PROC: Loop through each record number in the record_nums array.
    //  Create a record, insert the record from the database into the record
    //  object and then insert the record into the vector
    for(unsigned int i = 0; i < record_nums.size(); i++)
    {
        Record rec;
        rec.read(file, record_nums.at(i));
        records.push_back(rec);
    }

    //  PROC: Close the file
    file.close();

    //  PROC: Output the records vector
    return records;
}

/**********************************************************
*
* Accessor get_records(): Class Table
*_________________________________________________________
* Get the specified records from the database
*_________________________________________________________
* PRE-CONDITIONS
*
* Vector record_nums    :   The record numbers for the records
*                           that we are retreiving
*
* POST-CONDITIONS
* - This will return the Records as a vector of Record objects
*
*
***********************************************************/
Vector<Record> Table::get_records()
{
    Vector<Record> records;     //  CALC: Vector holds all of the records that
                                //  we are fetching


    if(TABLE_DEBUG)
    {
        cout << "\n\nTable::get_records(Vector<unsigned int> record_nums)\n";
        cout << "table_size: " << record_nums.size() << endl << endl;

    }


    //  PROC: Loop through each record number in the record_nums array.
    //  Create a record, insert the record from the database into the record
    //  object and then insert the record into the vector
    for(unsigned int i = 0; i < record_nums.size(); i++)
    {
        try {

            records.push_back(get_record(record_nums.at(i)));

        } catch (string str) {

            cout << str << endl;

        }



    }


    //  PROC: Output the records vector
    return records;
}

/**********************************************************
*
* Accessor get_record(): Class Table
*_________________________________________________________
* Get the specified record from the database
*_________________________________________________________
* PRE-CONDITIONS
*
* long record_num   :  A record_num that represents the record
*                      we are retreiving from the database
*
* POST-CONDITIONS
* - This will return the record as a record object
*
*
***********************************************************/
Record Table::get_record(long record_num)
{


    fstream file;
    Record rec;

    if(TABLE_DEBUG)
    {
        cout << "\n\ncalling get_record()\n";
        cout << "record_num: " << record_num << endl << endl;
    }

    assert(record_num <= record_nums.back());

    //  PROC: Check that the file exists
    if(file_exists(file_name))
    {
        //  PROC: open the file
        open_fileRW(file, file_name);

        //  PROC: If the record exists and is not dropped, then return the record
        if(rec.read(file, record_num) && !rec.is_deleted())
        {
            //  PROC: Close the file
            file.close();

            //  PROC: Output the records vector
            return rec;
        }

        //  PROC: If the record could not be opened, throw an error
        else
        {
            file.close();
            throw string("Record could not be read!");
        }
    }

    //  PROC: If the file could not be opened, throw an error.
    else
    {

        throw string("The file does not exist!");
    }





}

void Table::setup_field_map(Vector<string> field_names)
{
    if(TABLE_DEBUG)
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

    if(TABLE_DEBUG)
    {
        cout << "\n\end of setup_field_map(Vector<string> field_names)\n";
        cout << "field_map: \n" << field_map << endl << endl;

        for(unsigned int i = 0; i < value_map.size(); i++)
        {
            cout << value_map.at(i) << endl;
        }

    }
}

void Table::insert_field_map(string field, string value, long record_num)
{
    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled Table::insert_field_map(string field, string value, unsigned int record_nums)\n";
        cout << "field : " << field << endl;
        cout << "value : " << value << endl;
        cout << "record_nums: " << record_num << endl << endl;
    }

    //  PROC: If the input element is a number, then pad it with
    //  4 0's
    if(is_number(value) && !is_padded(value))
    {
        value = pad_number(value);

    }



    //  PROC: Insert the record num into the multimap at the field/value
    //  associated
    value_map.at(field_map[field])[value] += record_num;

    if(TABLE_DEBUG)
    {
        cout << "\n end of Table::insert_field_map(string field, string value, unsigned int record_nums)\n";
        cout << "field_map: \n" << field_map << endl << endl;

        for(unsigned int i = 0; i < value_map.size(); i++)
        {
            cout << value_map.at(i) << endl;
        }

    }
}

//Table::Table(const Table &table)
//{
//    if(TABLE_DEBUG)
//    {
//        cout << "\n\ncalled Table::Table(const Table &table)\n";
//        cout << "AKA the copy constructor\n\n";
//    }

//    copy(table);
//}

//Table & Table::operator =(const Table &table)
//{
//     copy(table);
//     return *this;
//}


void Table::copy(const Table &table)
{
    if(TABLE_DEBUG)
    {
        cout << "\n\n---------------------------\n\n";
        cout << "\n\n Copy()\n\n";
        cout << "(Source)name: " << table.name << endl;
        cout << "(Source)record_size: " << table.record_size << endl;
        cout << "(Source)table_size: " << table.record_nums.size() << endl;
        cout << "(Source)file_name: " << table.file_name << endl;
        cout << "(Source)field_names: " << const_cast<Table&>(table).field_names.to_string() << endl;
        cout << "\n\n---------------------------\n\n";
    }

    unsigned int index;

    //  PROC: Copy every private member variable except for the fstream
    //  object since it cant be copied
    name = table.name;
    field_names = table.field_names;
    record_size = table.record_size;
    strcpy(file_name, table.file_name);
    index = 0;

    //  PROC: If the file exists, keep adding a bigger number to the end of
    //  the file name until we find a filename that does not exist
    while(file_exists(file_name))
    {
        index++;
        //  PROC: Store the name of the table into file_name member variable with a
        //  .bin at the end
        strcpy(file_name, (name + to_string(index) + string(".sql")).c_str());

        if(TABLE_DEBUG)
        {
            cout << "filename: " << file_name << endl;
        }
    }

    //  PROC: Setup the field_map
    setup_field_map(field_names);


    if(!name.empty())
    {
        //  PROC: Insert each record from the table being copied into the new table
        for(unsigned int i = 0; i < record_nums.size(); i++)
        {
            insert(const_cast<Table&>(table).get_record(record_nums.at(i)).get_fields());
        }
    }



    if(TABLE_DEBUG)
    {
        cout << "\n\n---------------------------\n\n";
        cout << "\n\n Copy()\n\n";
        cout << "(Dest)name: " << name << endl;
        cout << "(Dest)record_size: " << record_size << endl;
        cout << "(Dest)table_size: " << record_nums.size() << endl;
        cout << "(Dest)file_name: " << file_name << endl;
        cout << "(Dest)field_names: " << field_names.to_string() << endl;
        cout << "\n\n---------------------------\n\n";
    }


}

Table::Table(string name)
{
    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled Table::Table(string name)\n";
        cout << "name: " << name << endl;
    }

    fstream file;
    unsigned int index;
    Vector<string> record_values;   //  CALC: Holds the values of each record
                                    //  loaded

    index = 0;


    //  PROC: Size has to be less than or equal to the max file name size
    assert(name.size() <= FILE_NAME_SIZE);

    //  PROC: Initialize Variables
    this->name = name;
    record_size = RECORD_SIZE;



    //  PROC: Store the name of the table into file_name member variable with a
    //  .bin at the end
    strcpy(file_name, (name + string(".sql")).c_str());


    if(TABLE_DEBUG)
    {
        cout << "file_name: " << file_name << endl;
    }


    //  PROC: If the file exists, then we open the file
    if(file_exists(file_name))
    {


        Record rec;

        //  PROC: open the file
        open_fileRW(file, file_name);

        //  PROC: If record 0 exists, we load it and then store it as the
        //  field names of this table (since the first record always holds
        //  the field names)
        if(rec.read(file, 0))
        {
            field_names = rec.get_fields();
            setup_field_map(field_names);

            if(TABLE_DEBUG)
            {
                cout << "field_names: " << field_names.to_string() << endl;
            }

            //  PROC: Index will keep track as we traverse through the records
            //  it will correspond with the record number
            index = 1;

            //  PROC: Begin reading the records until there are no more records to
            //  read.
            while(rec.read(file, index))
            {

                //  PROC: Make sure that the record has not been deleted
                if(!rec.is_deleted())
                {
                    //  PROC: Add the record num to the record_nums vector that
                    //  keeps track of all record numbers
                    record_nums.push_back(index);

                    //  PROC: Load the values of the record into this vector
                    record_values = rec.get_fields();

                    //  PROC: Loop through each value and store the record num
                    //  into the map.
                    for(unsigned int i = 0; i < record_values.size(); i++)
                    {
                        //  PROC: Insert the field name, the value, and the record number
                        insert_field_map(field_names.at(i), record_values.at(i), index);
                    }
                }




                index++;
            }
        }

        else
        {
            file.close();
            throw string("The table file does not contain record 0!");
        }

        file.close();


    }

    else
    {
        throw string("The table that is being opened does not exist!");
    }



    if(TABLE_DEBUG)
    {
        cout << "\n\n---------------------------\n\n";
        cout << "\n\nTable Constructor\n\n";
        cout << "name: " << name << endl;
        cout << "record_size: " << record_size << endl;
        cout << "table_size: " << record_nums.size() << endl;
        cout << "file_name: " << file_name << endl;
        cout << "field_names: " << field_names.to_string() << endl;
        cout << "\n\n---------------------------\n\n";
    }




}

Table::Table(string name, Vector<string> field_names)
{
    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled Table::Table(string name, Vector<string> field_names)\n";
        cout << "name: " << name << endl;
        cout << "field_names: " << field_names.to_string() << endl << endl;
    }

    unsigned int index;
    fstream file;

    //  PROC: Size has to be less than or equal to the max file name size
    assert(name.size() <= FILE_NAME_SIZE);

    //  PROC: Initialize Variables
    this->name = name;
    this->field_names = field_names;
    record_size = RECORD_SIZE;


    //  PROC: Store the name of the table into file_name member variable with a
    //  .bin at the end
    strcpy(file_name, (name + string(".sql")).c_str());
    index = 0;



    //  PROC: If the file exists, keep adding a bigger number to the end of
    //  the file name until we find a filename that does not exist
    while(file_exists(file_name))
    {
        index++;

        this->name = name + to_string(index);

        //  PROC: Store the name of the table into file_name member variable with a
        //  .bin at the end
        strcpy(file_name, (this->name + string(".sql")).c_str());

        if(TABLE_DEBUG)
        {
            cout << "filename: " << file_name << endl;
        }
    }



    //  PROC: Setup the field_map
    setup_field_map(field_names);


    if(TABLE_DEBUG)
    {
        cout << "\n\n---------------------------\n\n";
        cout << "\n\nTable Constructor\n\n";
        cout << "name: " << name << endl;
        cout << "record_size: " << record_size << endl;
        cout << "table_size: " << record_nums.size() << endl;
        cout << "file_name: " << file_name << endl;
        cout << "field_names: " << field_names.to_string() << endl;
        cout << "\n\n---------------------------\n\n";
    }



    //  PROC: open the file
    open_fileRW(file, file_name);

    //  PROC: Insert the names of the fields into record 0
    Record r(field_names);
    r.write(file);

    file.close();


}

void Table::drop_records(mmap<string, string> &parse_table)
{
    assert(parse_table.contains("command") && parse_table["command"].to_vector().at(0) == "delete");

    Vector<long> record_nums;       //  PROC: Holds the record numbers that
                                    //  match the conditions we are deleting

    //  PROC: Get the record numbers that match the conditions passed
    //  with the delete command
    record_nums = select_conditions(parse_table["conditions"].to_vector());

    //  PROC: Delete the records
    drop_records(record_nums);

}

bool Table::drop_record(long record_num)
{
    fstream outs;       //  CALC: File object used to access the sql file for
                        //  table

    Record rec;         //  CALC: Will be used to mark the specific record
                        //  as being dropped from the table


    if(DEBUG)
    {
        cout << "\n\ncalled Table::drop_record()\n";
        cout << "record_num: " << record_num << endl << endl;
    }

    //  PROC: Opened the sql file for the table
    open_fileRW(outs, file_name);

    //  PROC: Drop the record
    if(rec.mark_dropped(outs, record_num))
    {
        //  PROC: Close the file
        outs.close();

        //  PROC: Find the record_num in the vector that holds all of the
        //  record_nums of the table and remove it
        for(unsigned int i = 0; i < record_nums.size(); i++)
        {
            if(record_nums.at(i) == record_num)
            {
                record_nums.erase(i);
            }
        }

        if(DEBUG)
        {
            cout << "\n\n end of Table::drop_record()\n";
            cout << "record_num: " << record_num << endl << endl;
            cout << "record_nums: " << record_nums.to_string() << endl;
        }

        return true;
    }

    //  PROC: If we can't mark the record as dropped
    else
    {
        //  PROC: Close the file
        outs.close();

        if(DEBUG)
        {
            cout << "\n\n end of Table::drop_record()\n";
            cout << "record_num: " << record_num << endl << endl;
        }

        return false;
    }




}

void Table::drop_records(Vector<long> record_nums)
{

    if(DEBUG)
    {
        cout << "\n\n called Table::drop_records()\n";
        cout << "record_num: " << record_nums.to_string() << endl << endl;
    }

    for(unsigned int i = 0; i < record_nums.size(); i++)
    {
        if(drop_record(record_nums.at(i)))
        {
            cout << "\n\nRecord #" << record_nums.at(i) << " dropped!\n\n";
        }
    }

}

Table::Table(string name, Vector<string> field_names, Vector<Record> records)
{
    unsigned int index;     //  CALC: Holds the number that will be appended
                            //  to the end of the file name until we find a
                            //  file that does not exist

    //  PROC: Initialize Variables
    index = 0;
    fstream file;

    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled Table::Table(string name, Vector<Record> records)\n";
        cout << "name: " << name << endl;
        cout << "records:\n";
        records.print();
    }

    //  PROC: Initialize Variables
    this->name = name;
    record_size = RECORD_SIZE;


    //  PROC: Size has to be less than or equal to the max file name size
    assert(name.size() <= FILE_NAME_SIZE);


    //  PROC: Store the name of the table into file_name member variable with a
    //  .bin at the end
    strcpy(file_name, (name + string(".sql")).c_str());

    //  PROC: If the file exists, keep adding a bigger number to the end of
    //  the file name until we find a filename that does not exist
    while(file_exists(file_name))
    {
        index++;
        //  PROC: Store the name of the table into file_name member variable with a
        //  .bin at the end
        strcpy(file_name, (name + to_string(index) + string(".sql")).c_str());

        if(TABLE_DEBUG)
        {
            cout << "filename: " << file_name << endl;
        }
    }

    //  PROC: Store the field names into the object
    this->field_names = field_names;

    //  PROC: Setup the field_map
    setup_field_map(field_names);


    if(TABLE_DEBUG)
    {
        cout << "\n\n---------------------------\n\n";
        cout << "\n\nTable Constructor\n\n";
        cout << "name: " << name << endl;
        cout << "record_size: " << record_size << endl;
        cout << "table_size: " << record_nums.size() << endl;
        cout << "file_name: " << file_name << endl;
        cout << "field_names: " << field_names.to_string() << endl;
        cout << "\n\n---------------------------\n\n";
    }



    //  PROC: open the file
    open_fileRW(file, file_name);

    //  PROC: Insert the names of the fields into record 0
    record_nums.push_back(0);
    Record r(field_names);
    r.write(file);

    file.close();

    //  PROC: Insert all of the records that were passed in from the vector
    for(unsigned int i = 0; i < records.size(); i++)
    {
        //  PROC: Insert using the get_fields() vector from each record.
        insert(records.at(i).get_fields());
    }


}

void Table::remove_temp_files()
{
    char tempFileName[15];      //  CALC: Holds a temp file name representing
                                //  all of the file names following the table
                                //  file

    unsigned int index;         //  CALC: Index to move through each temp file

    //  PROC: Initialize variable
    index = 1;

    //  PROC: Point to the first possible temp file that would exist
    strcpy(tempFileName, (name + to_string(index) + string(".sql")).c_str());

    cout << "filename: " << tempFileName << endl;

    //  PROC: While temp files exist, go through each one removing the temp files
    while(file_exists(tempFileName))
    {


        remove(tempFileName);

        index++;

        strcpy(tempFileName, (name + to_string(index) + string(".sql")).c_str());
    }
}

Table::~Table()
{
    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled Table::~Table()\n";
    }

//    remove(file_name);







//    STokenizer tokenizer;
//    Token token;

//    tokenizer.set_string(file_name);

//    //  PROC: Tokenize the file name. If one of the tokens is a number, remove
//    //  the table file
//    while(!tokenizer.done())
//    {
//        tokenizer >> token;

//        if(token.type_string() == "NUMBER")
//        {
//            remove(file_name);
//        }

//    }


}




void Table::insert(Vector<string> field_inserts)
{
    long record_num;        //  CALC: The record number of the newly written
                            //  record

    fstream file;

    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled Table::insert()\n";
        cout << "field_inserts: " << field_inserts.to_string() << endl << endl;
    }

    //  PROC: Initialize Variable
    record_num = 0;

    //  PROC: Confirm that the amount of the values being inserted is equal
    //  to the fields that are available in this table
    if(field_inserts.size() == field_names.size())
    {


        //  PROC: open the file
        open_fileRW(file, file_name);

        //  PROC: Create a record object and write it to the end of the file
        Record r(field_inserts);
        record_num = r.write(file);

        //  PROC: Push the record_num into the record_nums vector that holds
        //  all record_nums
        record_nums.push_back(record_num);

        //  PROC: Loop through each inserted value and store the record num
        //  into the map
        for(unsigned int i = 0; i < field_inserts.size(); i++)
        {
            //  PROC: Insert the field name, the value, and the record number
            insert_field_map(field_names.at(i), field_inserts.at(i), record_num);
        }



        //  PROC: Close the file
        file.close();
    }

    else
    {
        throw string("The amount of values being inserted is not equal to the amount "
        "of available fields!");

    }






}




/**********************************************************
*
* Accessor select_conditions(): Class Table
*_________________________________________________________
* Get the the record numbers for the records that meet
* the specified conditions
*_________________________________________________________
* PRE-CONDITIONS
*
* Vector conditions    :   A vector of strings where each one
*                          is a condition (ex. first=nick)
*
* POST-CONDITIONS
* - This will a Vector<long> with the record numbers of each
*   record number in the table that meets the conditions
*
*
***********************************************************/
Vector<long> Table::select_conditions(Vector<string> conditions)
{

    Vector<long> record_nums;           //  OUT: The record numbers that meet the
                                        //  conditions
    string field;                       //  CALC: Holds the field of the condition
    string value;                       //  CALC: Holds the value of the condition
    string operation;                   //  CALC: Holds the operation of the condition
    unsigned int field_index;           //  CALC: Holds the index that points to the
                                        //  map that holds the values for the field
    Vector<Condition> conditions_list;  //  CALC: Holds the Condition objects



    if(TABLE_DEBUG)
    {
        cout << "\n\nselect_conditions() called\n";
        cout << "condtions: " << conditions.to_string() << endl << endl;
    }

    Shaunting_Yard yard(conditions_to_vector(conditions.to_string()), field_map, value_map);




//    Shaunting_Yard yard(conditions)

    return yard.rpm();



}

//  PROC: Converts a vectors of fields to its corresponding column number

/**********************************************************
*
* Accessor select_fields(): Class Table
*_________________________________________________________
* Get the column numbers of each field specified in the
* specified vector
*_________________________________________________________
* PRE-CONDITIONS
*
* Vector<strign> fields     :   The names of the fields (columns) that
*                               we want to retreive from a table
*
* POST-CONDITIONS
* - This will return a Vector of unsigned ints that correlate to each
*   field(column) that we want to retreive from the table
*
*
***********************************************************/
Vector<unsigned int> Table::select_fields(Vector<string> fields)
{

    Vector<unsigned int> field_nums;        //  OUT: Holds the column numbers
                                            //  that correspond with each field

    unsigned field_num;                     //  CALC: Will temporarily hold the
                                            //  column number before added to
                                            //  the vector


    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled select_fields()\n";
        cout << "fields: " << fields.to_string() << endl << endl;
    }

    //  PROC: Initialize variables
    field_num = 0;

    if(fields.size() == 0)
    {
        throw string("No fields were specified");
    }

    //  PROC: If the only field speicifed is '*" then we simply return all of
    //  the field_nums
    if(fields.size() == 1 && fields.at(0) == "*")
    {
        for(unsigned int i = 0; i < field_names.size(); i++)
        {
            field_nums.push_back(i);
        }

    }

    //  PROC: If the field specified is not a '*" then we get the field_nums
    //  for the fields specified
    else
    {
        //  PROC: Loop through each field and retreive the field number (the column
        //  that the field is in
        for(unsigned int i = 0; i < fields.size(); i++)
        {
            //  PROC: Check that the field exists in the field_map
            if(field_map.contains(fields.at(i)))
            {
                //  PROC: Get the field numbeer from the field_map
                field_num = field_map[fields.at(i)];

                //  PROC: Add the field_num to the vector
                field_nums.push_back(field_num);
            }

            else
            {
                throw string("Field does not exist! (before from)");
            }


        }
    }



    return field_nums;

}


/**********************************************************
*
* Accessor select(): Class Table
*_________________________________________________________
* This takes in the parse_table as a parameter.
*_________________________________________________________
* PRE-CONDITIONS
*
* mmap<string, string> parse_table  :   A parse_table that has 'select'
*                                       as the value of the command key
*
* POST-CONDITIONS
* - This will return a new Table object (and file)
* that meets the conditions and field requirements
* that exist in the parse_table
*
*
***********************************************************/
Table Table::select(Parser parser)
{

    Table table;

    //  PROC: Return parse table
    mmap<string, string> parse_table = parser.get_parse_table();

    Vector<long> record_nums;
    Vector<unsigned int> field_nums;


    //  PROC: If there were conditions specifed (Where) then we get the record
    //  numbers for the records that meet those conditions
    if(parse_table.contains("conditions"))
    {
        record_nums = select_conditions(parse_table["conditions"].to_vector());
    }

    //  PROC: If no conditions were specifed. then store all of the record numbers
    //  into the record_nums vector
    else
    {

        record_nums = this->record_nums;

    }

    //  PROC: Get the field_numbers for the fields that are being requested
    //
    field_nums = select_fields(parse_table["fields"].to_vector());

    //  PROC: If the field_nums is equal to the size fo field_names, then we
    //  know that the * was the field. Therefore we pass the same exact field_names
    //  that this object was built with since we are not filtering any fields
    if(field_nums.size() == field_names.size())
    {
        table = Table(name, field_names);
    }

    //  PROC: Since the field_nums is not equal in size to field_names, then we
    //  know that certain fields were specifed and we need to filter for them.
    //  pass in those specific fields when constructing the new table
    else
    {
        //  PROC: Create new table with the specifed fields. The fields are assumed
        //  to be valid because an error would have been throw
        table = Table(name, parse_table["fields"].to_vector());
    }

    //  PROC: Insert the records that are being requested into the new table
    for(unsigned int i = 0; i < record_nums.size(); i++)
    {


        try {

            //  PROC: This uses get_record to get the specifed record using the
            //  record_num. then it gets the specified fields using get_fields,
            //  using the field_nums that we are seeking
            table.insert(get_record(record_nums.at(i)).get_fields(field_nums));

        } catch (string str) {

        }

    }




    return table;



}

string Table::select_all()
{

    stringstream output;  //  OUTPUT: The string that will hold all of the records
                          //  of the table; formatted

    Vector<string> fields;


    output << "\nTable_Name: " << name << ", records: " << record_nums.size() << endl;

    try {


        try {

            //  PROC: Get the fields of the table
            fields = get_record(0).get_fields();

        } catch (string str) {

            cout << str << endl;

        }


        output << setw(3) << left << "#";

        //  PROC: Loop through each field, appending it to the string steam
        //  with formattng
        for(unsigned int i = 0; i < fields.size(); i++)
        {
            output << setw(15) << left << fields.at(i);
        }

        output << endl;

        for(unsigned int i = 0; i < fields.size(); i++)
        {
            for(unsigned int h = 0; h < 15; h++)
            {
                output << "-";
            }
        }

        output << endl;


    } catch (string exception) {

        cout << exception << endl;
    }

    //  PROC: Loop through all of the record numbers of the table

    for(unsigned int i = 0; i < record_nums.size(); i++)
    {
        //  PROC: Get the records fields, loop through them, formatting them
        //  and  appending them to the string stream
        try {
            fields = get_record(record_nums.at(i)).get_fields();


            output << setw(3) << left << record_nums.at(i);

            for(unsigned int i = 0; i < fields.size(); i++)
            {
                output << setw(15) << left << fields.at(i);
            }


            output << "\n";
        } catch (string exception) {

            cout << exception << endl;
        }


    }


    return output.str();






}


bool is_number(string str)
{
    //  PROC: Loop through each character of the string
    for(unsigned int i = 0; i < str.size(); i++)
    {
        //  PROC: If the character is not a digit, return false beacuse the
        //  string is not a number
        if(!isdigit(str.at(i)))
        {
            return false;
        }
    }

    //  PROC: If none of the characters were not numbers, then the string
    //  is a number
    return true;
}

bool is_padded(string str)
{

    //  PROC: If the string is the size of the padding and is a number
    //  then we know it is padded
    if(str.size() == NUM_PADDING_SIZE && is_number(str))
    {
        return true;
    }

    //  PROC: If the string size is not equal then return false
    else
    {
        return false;
    }
}

string pad_number(string str)
{
    assert(is_number(str));

    //  PROC: If the number is not padded, then pad
    while(!is_padded(str))
    {
        str.insert(str.begin(), '0');

        if(TABLE_DEBUG)
        {
            cout << "\n\nPadding the element with 0's because it is a"
                    " number.\n";
            cout << "element: " << str << endl;
        }


    }

    return str;



}


/**********************************************************
*
* Function conditions_to_vector(string str)
*_________________________________________________________
* Converts a string into a vector
*_________________________________________________________
* PRE-CONDITIONS
* string str    :   A string that contains conditions (separated by and/or
*                   operators). Each conditions and operator is separated
*                   by a comma
*                   ex. first=nick,and,last=roger,or,major=cs
*
* POST-CONDITIONS
*   - Returns a vector of words (that were separated by commas in the string)
***********************************************************/
Vector<string> conditions_to_vector(string str)
{
    Vector<string> output;      //  CALC: This will hold the vector of strings
                                //  that will be outputted
    STokenizer tokenizer;       //  CALC: This will break up a string into tokens
    Token token;                //  CALC: token that will hold each token from
                                //  tokenizer
    string token_str;            //  CALC: Holds the token's string after
                                //  it has been extracted

    string temp;


    //  PROC: Put the string in the tokenizer
    tokenizer.set_string(str);

    if(TABLE_DEBUG)
    {
        cout << "\n\ncalled conditions_to_vector(string str)\n\n";
        cout << "string: " << str << endl;
    }

    //  PROC: Loop until each token has been extracted
    while(!tokenizer.done())
    {
        //  PROC: Extract token and place its string into token_str
        tokenizer >> token;
        token_str = token.token_str();


        if(TABLE_DEBUG)
        {
            cout << "token: " << token_str << endl;
        }

        //  PROC: If the token is not a commma and not a space then store it
        //  in the vector (so only the words are acquired.
        if(token_str != ",")
        {

            //  PROC: If the input element is a number, then pad it with
            //  4 0's
            if(is_number(token_str) && !is_padded(token_str))
            {

                token_str = pad_number(token_str);
            }

            //  PROC: append to temp
            temp.append(token_str);
        }

        //  PROC: If we reach a comma, push the temp string into the vector
        else
        {


            output.push_back(temp);
            temp = "";
        }


    }


    //  PROC: Push whatever is still in temp into output (this will most likely
    //  not be empty because in the loop, it only pushes to the vector if it
    //  reaches a comma.
    output.push_back(temp);

    return output;


}



#endif // TABLE_H
