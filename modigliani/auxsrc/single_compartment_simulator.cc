/**
 * @file single_compartment_simulator.cc
 * Can simulate arbitrary axons
 * @version  2.0
 * @author Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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

#include <modigliani_core/aux_func.h>

/**
 * Runs a simulation using parameters in the given json file.
 *
 * This program only simulates a single compartment. If run using
 * a configuration file made for axon_simulator, the first compartment
 * found in compartment_parameters will be used. electrods_lua and
 * anatomy_lua are not used.
 *
 * @param filename
 * @return Status
 */
int Simulate(string fileName) {
	Json::Value config_root = modigliani_core::read_config(fileName);
	string timedOutputFolder;

	// We write each compartment's potential and currents into a single file.
	ofstream TimeFile, log_file;

	ofstream* pot_current_file;
	if (config_root["simulation_parameters"].get("sampN", 0).asUInt() > 0) {
		timedOutputFolder =
				modigliani_core::createOutputFolder(
						config_root["simulation_parameters"]["outputFolder"].asString());

		std::ifstream ifs(fileName, std::ios::binary);
		string temp_string = timedOutputFolder;
		std::ofstream ofs(temp_string.append("/conf.json").c_str(),
				std::ios::binary);

		ofs << ifs.rdbuf();
		ofs.close();
		ifs.close();

		modigliani_core::openOutputFile(timedOutputFolder, "Time", TimeFile);
		modigliani_core::openOutputFile(timedOutputFolder, "log", log_file, ".log");
		TimeFile << "% in ms" << std::endl;
		modigliani_core::openOutputFile(timedOutputFolder, "", log_file, ".log");
		pot_current_file = modigliani_core::openOutputFile(timedOutputFolder,
				"compartment", 0, ".bin");
	} else {
		modigliani_core::openOutputFile("/tmp", "log", log_file, ".log");
	}

	// Read input file only once. Store its content in memory.
	ifstream dataFile(
			config_root["simulation_parameters"]["inputFile"].asString().c_str(),
			ios::binary);
	if (dataFile.fail()) {
		std::cerr << "Could not open input file "
				<< config_root["simulation_parameters"]["inputFile"].asString().c_str()
				<< std::endl;
		exit(1);
	}

	std::vector<float> inputData(1000000);
	modigliani_base::Size index = 0;
	while (dataFile.good()) {
		if (index < inputData.size()) {
			char tmp[100];
			dataFile.getline(tmp, 100);
			sscanf(tmp, "%f", &inputData[index]);
			index++;
		} else {
			inputData.resize(inputData.size() + 100000);
		}
	}
	dataFile.close();

	// Trials loop
	for (modigliani_base::Size lTrials = 0;
			lTrials < config_root["simulation_parameters"]["numTrials"].asUInt();
			lTrials++) {
		/* Model setup */
		Json::Value compartments_parameters =
				config_root["compartments_parameters"];
		Json::Value simulation_parameters = config_root["simulation_parameters"];
		modigliani_core::Custom_cylindrical_compartment* oModel =
				modigliani_core::create_compartment(config_root, simulation_parameters,
						compartments_parameters[0u]);

		// SIMULATION ITERATION LOOP
		std::cerr << "MainLoop started" << std::endl;
		float timeVar = 0;
		modigliani_base::Real inpCurrent = 0.0;

		modigliani_base::Uniform_rnd_dist uniformRnd;

		modigliani_base::Real timeInMS = 0;
		int dataRead = 0;
		for (modigliani_base::Size lt = 0;
				lt < config_root["simulation_parameters"]["numIter"].asUInt();
				lt++) {
			timeInMS += oModel->_timeStep();
			timeVar = timeInMS;
			// Write number of columns
			if (config_root["simulation_parameters"]["sampN"].asInt() > 0
					&& lt == 0 && lTrials == 0) {
				modigliani_base::Size number_of_currents = oModel->NumberCurrents()
						+ 1;
				pot_current_file->write(
						reinterpret_cast<char*>(&number_of_currents),
						sizeof(modigliani_base::Size));
			}
			/* the "sampling ratio" used for "measurement" to disk */
			if (config_root["simulation_parameters"]["sampN"].asInt() > 0
					&& lt
							% config_root["simulation_parameters"]["sampN"].asInt()
							== 0) {
				modigliani_base::Size counter = 0;
				modigliani_base::Size number_of_currents = oModel->NumberCurrents();
				float data[1 + number_of_currents];
				data[0] = oModel->vm();
				for (modigliani_base::Size ll = 1; ll - 1 < number_of_currents; ++ll) {
					data[ll] = oModel->AttachedCurrent(ll);
				}
				pot_current_file->write(reinterpret_cast<char*>(data),
						(1 + number_of_currents) * sizeof(float));
			}
			if (!lTrials)
				TimeFile << timeVar << std::endl;

			if (lt % config_root["simulation_parameters"]["readN"].asInt()
					== 0) {
				inpCurrent =
						(inputData[dataRead]
								* config_root["simulation_parameters"]["inpISDV"].asDouble())
								+ config_root["simulation_parameters"]["inpI"].asDouble();
				dataRead++;
			}
			oModel->InjectCurrent(inpCurrent);

			oModel->step(oModel->vm());
		}

		delete oModel;
	} // lTrials
	log_file << "Simulation completed." << std::endl;
	log_file.close();
	pot_current_file->close();
	delete pot_current_file;

	return (0);
}

int main(int argc, char* argv[]) {
	return (Simulate(argv[2]));
}
