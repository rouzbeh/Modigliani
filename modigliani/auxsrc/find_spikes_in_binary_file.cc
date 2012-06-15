/* find_spikes_in_binary_file.cpp - output spike analysis  for binary data files. First column contains a time index.*/
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o find_spikes_in_binary_file find_spikes_in_binary_file.cpp
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "modigliani_base/aux_math_func.h"

#define NUM_NON_COLUMN_PARAM 5 // including argv[0] ==  executable
int check_for_spike(float v, float upThreshold, float downThreshold,
		bool* spiking) {
	if (v > upThreshold) {
		if (*spiking == true) {
			*spiking = true;
			return (0);
		} else {
			*spiking = true;
			return (1);
		}
	} else if (v < downThreshold) {
		if (*spiking == true) {
			*spiking = false;
			return (-1);
		} else {
			*spiking = false;
			return (0);
		}

	}
	return (0);
}

int ntaux_isnan(double value) {
	int status = (value != value);
	return (status);
}

int ntaux_isinf(const double x) {
	double y = x - x;
	int s = (y != y);

	if (s && x > 0)
		return (+1);
	else if (s && x < 0)
		return (-1);
	else
		return (0);
}

int main(int argc, char* argv[]) {
	bool selectColumns = false;

	if (argc < NUM_NON_COLUMN_PARAM) {
		std::cerr
				<< "Invalid command line."
				<< argv[0]
				<< "  <binary float data file> <number of columns in data file> <upstroke threshold> <down stroke threshold> [<compartment index 1 to print ...>] [<compartment index 2 to print ...>] [...] "
				<< std::endl;
		std::cerr
				<< "NOTE: Usually the first two columns of every data file are time and current data."
				<< std::endl;
		std::cerr
				<< "      and then the index ought to beg reater or equal 3, whith 3 designating the first voltage column"
				<< std::endl;
		exit(1);
	}
	std::string filename = argv[1];
	unsigned int columns = atoi(argv[2]);
	float upThreshold = atof(argv[3]);
	float downThreshold = atof(argv[4]);

	std::cerr << "Processing file " << filename << " ..." << std::endl;

	unsigned int number = 0;
	if (argc > NUM_NON_COLUMN_PARAM) {
		selectColumns = true;
		number = argc - NUM_NON_COLUMN_PARAM;
	}
	std::vector<unsigned int> selectVec(argc - NUM_NON_COLUMN_PARAM);

	unsigned int ll;
	for (ll = 0; ll < number; ll++) {
		if (atoi(argv[ll + NUM_NON_COLUMN_PARAM]) == 1)
			std::cerr
					<< "You selected the first column to be checked for spikes, it should contain the time and no voltage trace."
					<< std::endl;
		if (atoi(argv[ll + NUM_NON_COLUMN_PARAM]) == 2)
			std::cerr
					<< "You selected the second column to be checked for spikes, it may contain current data and no voltage trace."
					<< std::endl;
		selectVec[ll] = atoi(argv[ll + NUM_NON_COLUMN_PARAM]) - 1;
	}

	float buffer[columns];
	unsigned int rows = 0;

	bool spiking[columns];

	for (ll = 0; ll < columns; ll++) {
		spiking[ll] = false;
	}

	std::ifstream file(filename.c_str(), std::ios::binary);
	if (!file.good()) {
		std::cerr << "Something is bad with reading from file " << filename << std::endl;
		exit(2);
	}

	float v = 0;
	while (file.eof() == 0) {
		file.read(reinterpret_cast<char*>(buffer), columns * sizeof(float));
		if (true == selectColumns) {
			// std::cout << buffer[0] << "\t";
			for (ll = 0; ll < number; ll++) {
				v = buffer[selectVec[ll]];
				if (ntaux_isnan(v))
					std::cerr << "Warning - Column " << selectVec[ll] << " is NaN."
							<< std::endl;
				else if (ntaux_isinf(v))
					std::cerr << "Warning - Column " << selectVec[ll] << " is inf."
							<< std::endl;
				else
					std::cout
							<< check_for_spike(v, upThreshold, downThreshold,
									&(spiking[selectVec[ll]])) << "\t";
			}
		} else {
			// std::cout << buffer[0] << "\t";
			for (ll = 1; ll < columns; ll++) {
				v = buffer[ll];
				if (ntaux_isnan(v))
					std::cerr << "Warning - Column " << ll << " is NaN." << std::endl;
				else if (ntaux_isinf(v))
					std::cerr << "Warning - Column " << ll << " is +/- inf." << std::endl;
				else
					std::cout
							<< check_for_spike(v, upThreshold, downThreshold,
									&(spiking[ll])) << "\t";
			}
		}
		std::cout << std::endl;
		rows++;
	}

	std::cerr << "Completed and printed (float) " << rows << " rows with " << columns
			<< " columns from file " << filename << "." << std::endl;
	return (0);
}

