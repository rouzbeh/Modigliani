/* print_interspike_intervals_in_binary_file.cpp - output isi analysis for binary data files. First column must contain a time index.*/
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o print_interspike_intervals_in_binary_file print_interspike_intervals_in_binary_file.cpp
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

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

int main(int argc, char* argv[]) {
	bool selectColumns = false;

	if (argc < NUM_NON_COLUMN_PARAM || atoi(argv[2])<0) {
		std::cerr
				<< "First columns is time. Invalid command line."
				<< argv[0]
				<< "  <number of columns> <binary float data file> <upstroke threshold> <down stroke threshold> [<column index to print 2...>] [<column index to print 3...>] [...] "
				<< std::endl;
		exit(1);
	}
	string filename = argv[1];
	unsigned int columns = atoi(argv[2]);
	float upThreshold = atof(argv[3]);
	float downThreshold = atof(argv[4]);

	unsigned int number = 0;
	if (argc > NUM_NON_COLUMN_PARAM) {
		selectColumns = true;
		number = argc - NUM_NON_COLUMN_PARAM;
	}

	std::vector<unsigned int> selectVec(argc - NUM_NON_COLUMN_PARAM);

	std::vector<std::vector<float> > isiListVec;

	std::vector<std::vector<float> >::iterator itVec;
	std::vector<float>::iterator itList;

	//unsigned int ll;
	for (unsigned int ll = 0; ll < number; ll++) {
		if (atoi(argv[ll + NUM_NON_COLUMN_PARAM]) == 1) {
			std::cerr
					<< "You selected the first column to be checked for spikes, it should contain the time."
					<< std::endl;
		}
		selectVec[ll] = atoi(argv[ll + NUM_NON_COLUMN_PARAM]) - 1;
	}

	for (itVec = isiListVec.begin(); itVec != isiListVec.end(); itVec++) {
		for (itList = itVec->begin(); itList != itVec->end(); itList++) {
			itVec->resize(0);
		}
	}

	float buffer[columns];
	float spikeTime[columns];
	bool spiking[columns];

	for (unsigned int ll = 0; ll < columns; ll++) {
		buffer[ll] = 0;
		spikeTime[ll] = 0;
		spiking[ll] = false;
	}

	int rows = 0;

	std::vector<int> indexVec;

	if (true == selectColumns) {
		isiListVec.resize(argc - NUM_NON_COLUMN_PARAM);
	} else {
		isiListVec.resize(columns);
	}

	ifstream file(filename.c_str(), std::ios::binary);
	if (!file.good()) {
		std::cerr << "Something is bad with reading from file " << filename << std::endl;
		exit(2);
	}

	while (file.eof() == 0) {
		file.read(reinterpret_cast<char*>(buffer), columns * sizeof(float));
		if (true == selectColumns) {
			for (unsigned int ll = 0; ll < number; ll++) {
				if (1
						== check_for_spike(buffer[selectVec[ll]], upThreshold,
								downThreshold, &(spiking[selectVec[ll]]))) {
					(isiListVec[ll]).push_back(
							buffer[0] - spikeTime[selectVec[ll]]);
					spikeTime[selectVec[ll]] = buffer[0];
				}
			}
		} else {
			for (unsigned int ll = 1; ll < columns; ll++) {
				if (1
						== check_for_spike(buffer[ll], upThreshold, downThreshold,
								&(spiking[ll]))) {
					(isiListVec[ll]).push_back(buffer[0] - spikeTime[ll]);
					spikeTime[ll] = buffer[0];
				}
			}
		}
		std::cerr << "Analysing row " << rows << std::endl;
		rows++;
	}

	std::cerr << "Data output" << std::endl;

	for (itVec = isiListVec.begin(); itVec != isiListVec.end(); itVec++) {
		for (itList = itVec->begin(); itList != itVec->end(); itList++) {
			std::cout << *itList << std::endl;
		}
		std::cout << std::endl << std::endl;
	}

	std::cerr << "Completed and printed (float) " << rows
			<< " rows  columns from file " << filename << " with " << columns
			<< "." << std::endl;

	return (0);

}

