/* show_binary_file.cpp - print a binary, columns organised float data file (without formatting) to stdout */
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o show_binary_file show_binary_file.cpp
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

int
main(int argc, char* argv[])
{
    bool selectColumns = false;

    if (argc < 3) {
        cerr <<"Invalid command line." << argv[0] <<" <binary float data file> <number of columns> <every show every n-th row> [<column index to print>] [<column index to print>] [...] "<< endl;
        exit(1);
    }
    string filename = argv[1];
    int columns = atoi( argv[2]);
    int every = atoi( argv[3]);

    if (every < 1) every = 1;

    if (argc > 4) selectColumns = true;
    vector <unsigned int> selectVec(argc-4);
    for (unsigned int ll =0 ;ll < argc-4; ll++) {
        selectVec[ll] = atoi(argv[ll+4])-1;
    }

    float buffer[columns];
    unsigned int rows = 0;

    ifstream file (filename.c_str(), ios::binary);
    if (!file.good()) {
        cerr <<"Something is bad with reading from file "<<filename<<endl;
        exit(2);
    }

    while (file.eof() == 0) {
        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));

        if (rows%every == 0) {
            if (true == selectColumns) {
                for (unsigned int ll =0 ;ll < argc-4; ll++) {
                    cout << buffer[selectVec[ll]] << "\t";
                }
            } else {
                for (unsigned int ll = 0; ll < columns; ll++) {
                    cout << buffer[ll] << "\t";
                }
            }
            cout << endl;
        }
        rows++;
    }
    cerr << "Completed and printed (float) "<<rows<<" rows with "<<columns<<" columns from file "<<filename<<"."<<endl;
}








