/* Modigliani
 * Version:  2.0
 * Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
 * Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#include <mcore/auxfunc.h>

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 */
Json::Value read_config(string fileName) {
	Json::Value config_root;
	// Remember that data file should have more lines than Num iterations.
	Json::Reader config_reader;
	ifstream config_doc;
	config_doc.open(fileName.c_str(), ifstream::in);
	bool parsingSuccessful = config_reader.parse(config_doc, config_root);
	if (!parsingSuccessful) {
		// report to the user the failure and their locations in the document.
		std::cerr << "Failed to parse configuration\n"
				<< config_reader.getFormatedErrorMessages();
		exit(1);
	}
	return (config_root);
}


int main(int argc, char* argv[]) {
	if (argc<2){
		std::cerr << "You need to specify a config file" << std::endl;
		return(1);
	}

	string filename = argv[1];
	Json::Value config_root = read_config(filename);
	double min = -90;
	double max = -40;
	double current_guess;
	double current_result = 100000;

	ofstream temp;
	temp.open("/dev/null");

	while (current_result > -79.9 || current_result < -80.1) {
		/* Model setup */
		current_guess = (min + max) / 2;
		std::cout << "Trying with " << current_guess << std::endl;
		config_root["eLeak"] = current_guess;
		config_root["node_parameters"]["numNd"] = 5;
		std::vector<mbase::Size_t> nodes_vec, nodes_paranodes_vec;
		/* Model setup */
		mcore::Membrane_compartment_sequence* oModel = mcore::create_axon(
				config_root, temp, temp);

		oModel->Init();

		std::cerr << "Total number of compartments(in oModel)"
				<< oModel->_numCompartments() << std::endl;

		/* ***********************  Main loop **************************** */
		std::cerr << "MainLoop started" << std::endl;

		double sum = 0;
		for (mbase::Size_t lt = 0;
				lt < config_root["simulation_parameters"]["numIter"].asUInt();
				lt++) {

			/* the "sampling ratio" used for "measurement" to disk */
			if (lt % 10000 == 0) {
				sum = 0;
				for (mbase::Size_t ll = 0; ll < oModel->_numCompartments();
						ll++) {
					sum += oModel->compartmentVec[ll]->_vM();
				}
				std::cout << "Mean voltage = "
						<< sum / oModel->_numCompartments() << std::endl;
			}

			if (lt == 10000) {
				mbase::Real inpCurrent =
						(5
								* config_root["simulation_parameters"]["inpISDV"].asDouble())
								+ config_root["simulation_parameters"]["inpI"].asDouble();
				oModel->InjectCurrent(inpCurrent, 1);
			} else {
				oModel->InjectCurrent(0, 1);
			}

			oModel->step();
		}
		current_result = sum / oModel->_numCompartments();
		if (current_result > -80) {
			max = current_guess;
		}
		if (current_result < -80) {
			min = current_guess;
		}
	}
	std::cerr << "Simulation completed. Found " << current_guess << std::endl;
	return (0);
}
