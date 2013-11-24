/**
 * @file show_binary_file.cc
 * @brief print a binary, columns organised float data file (without formatting) to stdout
 *
 * Compile with: g++ -o show_binary_file show_binary_file.cpp
 *
 * Copyright 2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

int main(int argc, char *argv[]) {
  bool selectColumns = false;

  if (argc < 3 || atoi(argv[2]) < 0) {
    std::cerr << "Invalid command line." << argv[0]
      << " <binary float data file> <number of columns> "
      << "<every show every n-th row> [<column index to print>]"
      << " [<column index to print>] [...] "
      << std::endl;
    exit(1);
  }
  std::string filename = argv[1];
  unsigned int columns = atoi(argv[2]);
  int every = atoi(argv[3]);

  if (every < 1)
    every = 1;
  unsigned int number = 0;
  if (argc > 4) {
    selectColumns = true;
    number = argc - 4;
  }
  std::vector < unsigned int >selectVec(argc - 4);
  for (unsigned int ll = 0; ll < number; ll++) {
    selectVec[ll] = atoi(argv[ll + 4]) - 1;
  }

  float buffer[columns];
  unsigned int rows = 0;

  std::ifstream file(filename.c_str(), std::ios::binary);
  if (!file.good()) {
    std::cerr << "Something is bad with reading from file " << filename << std::
      endl;
    exit(2);
  }

  while (file.eof() == 0) {
    file.read(reinterpret_cast < char *>(buffer), columns * sizeof(float));

    if (rows % every == 0) {
      if (true == selectColumns) {
        for (unsigned int ll = 0; ll < number; ll++) {
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
  std::
    cerr << "Completed and printed (float) " << rows << " rows with " << columns
    << " columns from file " << filename << "." << std::endl;
}
