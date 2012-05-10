/* generate_white_noise.cpp - generates white noise */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o show_binary_file show_binary_file.cpp
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include <vector>

#include "mbase/Gaussian_rnd_dist.h"
#include "mbase/Sequential_statistics.h"

#define NUM_NON_COLUMN_PARAM 3

int
main(int argc, char* argv[])
{
    mbase::Gaussian_rnd_dist rnd;
    float value = 0.0;



    if (argc < NUM_NON_COLUMN_PARAM) {
        std::cerr << "Invalid arguments specified."<< std::endl
             << argv[0] << " <target file name> <num of random numbers> " << std::endl
             << "generates a binary file with 'float' normal random numbers."<< std::endl;
        exit(1);
    }
    std::string filename = argv[1];
    mbase::Msize numNumbers = atoi ( argv[2]);


    std::ofstream file (filename.c_str(), std::ios::binary);
    if (!file.good()) {
        std::cerr <<"Something is bad with writing to file "<<filename<< std::endl;
        exit(2);
    }


    mbase::Sequential_statistics stats;

    for (mbase::Msize ll = 0; ll < numNumbers; ll++) {
        value = float(rnd.RndVal());
        stats.Add(value);
        file.write( reinterpret_cast<char*>(&value), sizeof(float) );
    }


    std::cout << "Mean " << stats._average() << std::endl;
    std::cout << "StdDev " << sqrt(stats._variance()) << std::endl;
    std::cout << "Max val " << stats._max() << std::endl;
    std::cout << "Min val " << stats._min() << std::endl;
    std::cout << "In "<< stats._counter() << " values." << std::endl;
    file.close();

    return (0);
}
