/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o show_binary_file show_binary_file.cpp
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int
main(int argc, char* argv[])
{
    bool selectColumns = false;

    if (argc < 3) {
        std::cerr <<"Invalid command line." << argv[0] <<" <binary float data file> <number of columns> <every show every n-th row> [<column index to print>] [<column index to print>] [...] "<< std::endl;
        exit(1);
    }
    string filename = argv[1];
    unsigned int columns = atoi( argv[2]);
    int every = atoi( argv[3]);
    if (every < 1) every = 1;
    unsigned int number = 0;

    if (argc > 4) {
    	selectColumns = true;
    	number=argc-4;
    }
    std::vector <unsigned int> selectVec(number);
    for (unsigned int ll =0 ;ll < number; ll++) {
        selectVec[ll] = atoi(argv[ll+4])-1;
    }

    float buffer[columns];
    unsigned int rows = 0;

    ifstream file (filename.c_str(), std::ios::binary);
    if (!file.good()) {
        std::cerr <<"Something is bad with reading from file "<<filename<<std::endl;
        exit(2);
    }

    while (file.eof() == 0) {
        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));

        if (rows%every == 0) {
            if (true == selectColumns) {
                for (unsigned int ll =0 ;ll < number; ll++) {
                    std::cout << buffer[selectVec[ll]] << "\t";
                }
            } else {
                for (unsigned int ll = 0; ll < columns; ll++) {
                    std::cout << buffer[ll] << "\t";
                }
            }
            std::cout << std::endl;
        }
        rows++;
    }
    std::cerr << "Completed and printed (float) "<<rows<<" rows with "<<columns<<" columns from file "<<filename<<"."<<std::endl;
}








