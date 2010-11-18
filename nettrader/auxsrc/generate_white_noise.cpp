/* generate_white_noise.cpp - generates white noise */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o show_binary_file show_binary_file.cpp
 */

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include <vector>

#include "nt_gaussian_rnd_dist_obj.h"
#include "nt_sequential_statistics_obj.h"

#define NUM_NON_COLUMN_PARAM 3

int
main(int argc, char* argv[])
{
    NT_gaussian_rnd_dist_o rnd;
    float value = 0.0;



    if (argc < NUM_NON_COLUMN_PARAM) {
        cerr << "Invalid arguments specified."<< endl
             << argv[0] << " <target file name> <num of random numbers> " << endl
             << "generates a binary file with 'float' normal random numbers."<< endl;
        exit(1);
    }
    string filename = argv[1];
    NTsize numNumbers = atoi ( argv[2]);


    ofstream file (filename.c_str(), ios::binary);
    if (!file.good()) {
        cerr <<"Something is bad with writing to file "<<filename<<endl;
        exit(2);
    }


    NT_sequential_statistics_o stats;

    for (NTsize ll = 0; ll < numNumbers; ll++) {
        value = float(rnd.RndVal());
        stats.Add(value);
        file.write( reinterpret_cast<char*>(&value), sizeof(float) );
    }


    cout << "Mean " << stats._average() << endl;
    cout << "StdDev " << sqrt(stats._variance()) << endl;
    cout << "Max val " << stats._max() << endl;
    cout << "Min val " << stats._min() << endl;
    cout << "In "<< stats._counter() << " values." << endl;
    file.close();

    return 0;
}
