/*!
 * @file generate_white_noise.cc
 * @brief Generates white noise
 *
 * @author Ahmed Aldo Faisal
 * Copyright 2001 Ahmed Aldo Faisal
 *
 * Compile with: g++ -o show_binary_file show_binary_file.cpp
 */
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "modigliani_base/sequential_statistics.h"


#define NUM_NON_COLUMN_PARAM 3

int
main(int argc, char* argv[]) {
    boost::random::mt19937 rng;
    boost::random::normal_distribution<> norm =
        boost::random::normal_distribution<>();
    rng.seed(time(NULL));

    float value = 0.0;

    if (argc < NUM_NON_COLUMN_PARAM) {
        std::cerr << "Invalid arguments specified."
                  << std::endl
                  << argv[0]
                  << " <target file name> <num of random numbers> [<ascii>]"
                  << std::endl
                  << "generates a binary file with 'float' normal "
                  << "random numbers."
                  << std::endl;
        exit(1);
    }
    std::string filename = argv[1];
    modigliani_base::Size numNumbers = atoi(argv[2]);

    bool ascii = false;
    auto mode = std::ios::binary;
    if (argc > NUM_NON_COLUMN_PARAM) {
      ascii = true;
      mode = std::ios::out;
    }
    std::ofstream output_file(filename.c_str(), mode);
    if (!output_file.good()) {
        std::cerr << "Something is bad with writing to file "
                  << filename
                  << std::endl;
        exit(2);
    }


    modigliani_base::Sequential_statistics stats;

    for (modigliani_base::Size ll = 0; ll < numNumbers; ll++) {
      value = static_cast<float>(norm(rng));
      stats.Add(value);
      if (argc > ascii)
        output_file << value << std::endl;
      else
        output_file.write(reinterpret_cast<char*>(&value), sizeof(value));
    }


    std::cout << "Mean " << stats.average() << std::endl;
    std::cout << "StdDev " << sqrt(stats.variance()) << std::endl;
    std::cout << "Max val " << stats.max() << std::endl;
    std::cout << "Min val " << stats.min() << std::endl;
    std::cout << "In "<< stats.counter() << " values." << std::endl;
    output_file.close();

    return (0);
}
