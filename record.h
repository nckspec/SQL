#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

const bool R_DEBUG = false;




class Record{
public:


    Record(){

        //  PROC: Initiate the c-string with a null at the beginning
//        record[0] = NULL;

        //  PROC: Initialize the record number with -q
        recno = -1;
    }

    Record(Vector<string> input){

        input.insert(0, "YES");

        //  PROC: Initiate record with nulls
        for(unsigned int i = 0; i < MAX_ROWS; i++)
        {
            record[i][0] = NULL;
        }


        if(R_DEBUG)
        {
            cout << "\n\ncalled Record() constructor\n\n";
            cout << "input: " << input.to_string() << endl << endl;
        }

        for(unsigned int i = 0; i < input.size(); i++)
        {




            strcpy(record[i], input.at(i).c_str());

        }

        if(R_DEBUG)
        {
            cout << "\n\nRecord Constructor\n\n";

            cout << "\n\nFunction Record: write()\n\n";
            cout << "Record array: ";
            print_array(record, MAX_ROWS);
            cout << endl;

            cout << "\n\nEnd of Record Constructor\n\n";
        }

    }


    Record(string s){

        //  PROC: Initiate the Record object with a string that gets converted
        //  to c string
//        strcpy(record, s.c_str());
    }
    long write(fstream& outs);
    bool read(fstream& ins, long recno);


    Vector<string> get_fields(Vector<unsigned int> field_num) const;

    Vector<string> get_fields() const;

    bool is_deleted();



    friend ostream& operator<<(ostream& outs,
                               const Record& r);
private:

    bool exists_in_file(fstream &ins, long recno);

    static const int MAX = 10;
    static const int MAX_ROWS = 20;
    static const int MAX_COLS = 50;
    int recno;
    char record[MAX_ROWS][MAX_COLS];
};


Vector<string> Record::get_fields() const
{
    Vector<string> output;      //  CALC: Holds the fields that will be extracted
                                //  from this record

    unsigned int index;         //  CALC: The index that will be used to traverse
                                //  through the data array

    //  PROC: Initialize Variable with 1 since we are skipping 0 since it
    //  contains info on whether the record has been deleted or not
    index = 1;

    //  PROC: Keep inserting record fields into output until we get to a NULL
    //  character or we go through all of the rows of the data array
    while(record[index][0] != NULL || index == MAX_ROWS)
    {
        output.push_back(record[index]);
        index++;
    }

    return output;
}

/**********************************************************
*
* Class: record : Vector<string> get_fields()
*_________________________________________________________
* The function will return a vector of the specified fields
*_________________________________________________________
* PRE-CONDITIONS
* field_num     :   This will be a vector of numbers that reference
*                   the fields that we want to return
*
* POST-CONDITIONS
*   - Returns a vector of the fields from the record
*
***********************************************************/
Vector<string> Record::get_fields(Vector<unsigned int> field_num) const
{
    Vector<string> output;      //  CALC: Holds the fields that will be extracted
                                //  from this record

    //  PROC: Loop through each field num and put the corresponding field in
    //  the output vector.
    for(unsigned int i = 0; i < field_num.size(); i++)
    {
        assert(field_num.at(i) < MAX_ROWS);
        assert(record[field_num.at(i)] != NULL);

        //  PROC: Add a +1 to whatever field number is being requested so that
        //  we can ignore the first field which stores whether or not the
        //  record has been deleted or not
        output.push_back(record[field_num.at(i) + 1]);

    }

    return output;
}


long Record::write(fstream &outs){

    //  PROC: Move to the end of the file
    outs.seekg(0, outs.end);


    //  PROC: Get the position that you are at in the file
    long pos = outs.tellp();


    //  PROC: Insert each field entry into the file
    for(unsigned int i = 0; i < MAX_ROWS; i++)
    {
        //  PROC: Write the record to the end of the file
        outs.write(record[i], sizeof(record[i]));
    }


    if(R_DEBUG)
    {
        cout << "\n\n------------------------\n\n";
        cout << "\n\nFunction Record: write()\n\n";
        cout << "pos: " << pos << endl;
        cout << "pos/sizeof(record): " << pos / sizeof(record) << endl;
        cout << "Record array: ";
        print_array(record, MAX_ROWS);
        cout << endl;
        cout << "\n\nEnd of Function Record: read()\n\n";

    }

    //  PROC: Return the record #
    return pos/sizeof(record);
}

bool Record::is_deleted()
{

    if(strcmp(record[0], "NO") == 0)
    {
        return true;
    }

    else
    {
        return false;
    }

}

bool Record::exists_in_file(fstream &ins, long recno)
{

    ins.seekg(0, ins.end);

    //  PROC: Calculate the position of the record in the database file by
    //  multiplying the record number we want by its size
    long pos= recno * sizeof(record);



    //  PROC: If the end of the file is less than pos
    if(ins.tellp() <= pos)
    {
        return false;
    }

    else {
        return true;
    }
}

bool Record::read(fstream &ins, long recno)
{


    //  PROC: Calculate the position of the record in the database file by
    //  multiplying the record number we want by its size
    long pos= recno * sizeof(record);

    //  PROC: If the
    if(exists_in_file(ins, recno))
    {
        //  PROC: Move to the position in the bindary file
        ins.seekg(pos, ios_base::beg);


        //  PROC: read the record into the cstring
        for(unsigned int i = 0; i < MAX_ROWS; i++)
        {
            ins.read(record[i], sizeof(record[i]));
        }


        if(R_DEBUG)
        {
            cout << "\n\n------------------------\n\n";
            cout << "\n\nFunction Record: read()\n\n";
            cout << "sizeof(record): " << sizeof(record) << endl;
            cout << "recno: " << recno << endl;
            cout << "pos: " << pos << endl;
            cout << "\n\nEnd of Function Record: read()\n\n";

        }


        return true;
    }

    else
    {
        return false;

    }



}

//  PROC: Print out the record
ostream& operator<<(ostream& outs,
                    const Record& r){

    //  PROC: Print out each row of the record file (This will print out each
    //  field of the record)
    for(unsigned int i = 0; i < 20; i++)
    {
        outs << r.record[i] << " ";
    }

    return outs;
}


/**********************************************************
*
* Class: record : void file_exists()
*_________________________________________________________
* The function will check if a file exists
*_________________________________________________________
* PRE-CONDITIONS
* filename  :       The file name
*
* POST-CONDITIONS
*   - Returns true if the file exists and false if not
*
***********************************************************/
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

/**********************************************************
*
* Class: record : void open_fileRW()
*_________________________________________________________
* The function will open a file for reading and writing
*_________________________________________________________
* PRE-CONDITIONS
* f         :       The file object
* filename  :       The file name
*
* POST-CONDITIONS
*   - A fstream object will be returned by reference with the
*   file opened that is specified in the filename
*
***********************************************************/
void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}

/**********************************************************
*
* Class: record : void open_fileW()
*_________________________________________________________
* The function will open a file for reading
*_________________________________________________________
* PRE-CONDITIONS
* f         :       The file object
* filename  :       The file name
*
* POST-CONDITIONS
*   - A fstream object will be returned by reference with the
*   file opened that is specified in the filename
*
***********************************************************/
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

void test()
{
    cout <<endl<<endl<<endl<< "================================" << endl;
    string list[10] = {"zero",
                       "one",
                       "two",
                       "three",
                       "four",
                       "five",
                       "six",
                       "seven",
                       "eight",
                       "nine"};

    for (int i = 0; i< 10; i++){
        cout<<list[i]<<endl;
    }

    fstream f;
    //reset the file:
    open_fileW(f, "record_list.bin");
    for (int i= 0; i<10; i++){
        Record r(list[i]);
        int recno = r.write(f);
        cout<<"["<<list[i] <<"]"<< " was written into file as record: "<<recno<<endl;
    }
    f.close();



    Record r;
    //open the file for reading and writing.
    open_fileRW(f, "record_list.bin" );
    r.read(f, 3);
    cout<<"record 3: "<<r<<endl;
    r.read(f, 6);
    cout<<"record 6: "<<r<<endl;

    cout <<endl<<endl<<endl<< "================================" << endl;

}

#endif // RECORD_H
