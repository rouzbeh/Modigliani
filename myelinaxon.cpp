/* NetTrader - visualisation, scientific and financial analysis and simulation system
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

#include <ntbp_auxfunc.h>
#include <plplot/plplot.h>
#include <plplot/plstream.h>

using namespace std;

/* Global */
Json::Value hillock_parameters;
Json::Value node_parameters;
Json::Value paranode_parameters;
Json::Value internode_parameters;
Json::Value simulation_parameters;
Json::Value config_root; // will contains the root value after parsing.

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 */
void read_config(string fileName) {
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

	/*Hillokc*/
	if (config_root.get("hillock", false).asBool())
		hillock_parameters = config_root["hillock_parameters"];

	/* Nodes */
	if (config_root.get("node", false).asBool()) {
		node_parameters = config_root["node_parameters"];
	}

	/* Paranodes */
	if (config_root.get("paranode", false).asBool())
		paranode_parameters = config_root["paranode_parameters"];

	/* Internodes */
	if (config_root.get("internode", false).asBool())
		internode_parameters = config_root["internode_parameters"];

	simulation_parameters = config_root["simulation_parameters"];

}

/**
 * Let's go!
 *
 * @param filename
 * @return
 */
int simulate(string fileName) {
	read_config(fileName);
	string timedOutputFolder;

	NTsize numCompartments = hillock_parameters["numComp"].asUInt()
			+ (node_parameters["numNd"].asUInt() - 1)
					* (node_parameters["numComp"].asUInt()
							+ 2 * paranode_parameters["numComp"].asUInt()
							+ internode_parameters["numComp"].asUInt())
			+ node_parameters["numComp"].asUInt()
			+ paranode_parameters["numComp"].asUInt();

	// We write each compartment's potential and currents into a single file.
	ofstream TimeFile, LengthPerCompartmentFile, TypePerCompartmentFile,
			ConfigUsedFile;
	vector<ofstream*> pot_current_files(numCompartments);
	if (simulation_parameters.get("sampN", 0).asUInt() > 0) {
		timedOutputFolder = createOutputFolder(
				simulation_parameters["outputFolder"].asString());

		std::ifstream ifs(fileName, std::ios::binary);
		string temp_string = timedOutputFolder;
		std::ofstream ofs(temp_string.append("/conf.json").c_str(),
				std::ios::binary);

		ofs << ifs.rdbuf();
		ofs.close();
		ifs.close();

		openOutputFile(timedOutputFolder, "Time", TimeFile);
		openOutputFile(timedOutputFolder, "TypePerCompartment",
				TypePerCompartmentFile);
		openOutputFile(timedOutputFolder, "LengthPerCompartment",
				LengthPerCompartmentFile);
		openOutputFile(timedOutputFolder, "ConfigUsed", ConfigUsedFile, ".m");
		printConfig(ConfigUsedFile, node_parameters, paranode_parameters,
				internode_parameters, simulation_parameters, config_root);
		TimeFile << "% in ms" << endl;

		int counter = 0;
		for (auto ci = pot_current_files.begin(); ci != pot_current_files.end();
				++ci) {
			*ci = openOutputFile(timedOutputFolder, "compartment", counter++,
					".bin");
		}
	}

	cout << "Eleak set to " << config_root.get("eLeak", 0).asDouble() << " mV."
			<< endl;

	// Read input file only once. Store its content in memory.
	ifstream dataFile(simulation_parameters["inputFile"].asString().c_str(),
			ios::binary);
	if (dataFile.fail()) {
		cerr << "Could not open input file "
				<< simulation_parameters["inputFile"].asString().c_str()
				<< endl;
		exit(EXIT_IO_ERROR);
	}
	int count = 1000000;
	vector<float> inputData(count);
	int index = 0;

	while (dataFile.good()) {
		if (index < count) {
			char tmp[100];
			dataFile.getline(tmp, 100);
			std::sscanf(tmp, "%f", &inputData[index]);
			index++;
		} else {
			count += 1000000;
			inputData.resize(count);
		}
	}
	dataFile.close();
	//NTsize compartmentCounter;
	cout << "Assembling neuron..." << endl;
	vector<NTsize> nodes_vec(0);
	vector<NTsize> nodes_paranodes_vec(0);
	/* *** Trials loop *** */
	for (NTsize lTrials = 0;
			lTrials < simulation_parameters["numTrials"].asUInt(); lTrials++) {
		/* Model setup */
		nodes_vec.clear();
		nodes_paranodes_vec.clear();
		NTBP_membrane_compartment_sequence_o oModel = create_axon(config_root,
				TypePerCompartmentFile, LengthPerCompartmentFile, nodes_vec,
				nodes_paranodes_vec);

		if (!lTrials) {
			TypePerCompartmentFile.close();
			LengthPerCompartmentFile.close();
		}
		oModel.Init();

		/* Information measurement init */
		numCompartments = hillock_parameters["numComp"].asUInt()
				+ (node_parameters["numNd"].asUInt() - 1)
						* (node_parameters["numComp"].asUInt()
								+ 2 * paranode_parameters["numComp"].asUInt()
								+ internode_parameters["numComp"].asUInt())
				+ node_parameters["numComp"].asUInt()
				+ paranode_parameters["numComp"].asUInt();
		cerr << "Total number of compartments(computed)" << numCompartments
				<< endl;
		//cerr << "Total number of compartments(assembled)" << compartmentCounter
		//	<< endl;
		cerr << "Total number of compartments(in oModel)"
				<< oModel._numCompartments() << endl;
		vector<NTreal> leakCurrVec(numCompartments);
		vector<NTreal> naCurrVec(numCompartments);
		vector<NTreal> kCurrVec(numCompartments);

		/* Graphics init */
		plstream* pls = new plstream();

		if (simulation_parameters["useVis"].asInt() > 0) {
			// Initialize plplot.
			pls->sdev("wxwidgets");
			pls->scolbg(255,255,255);
			pls->scol0(1, 0, 0, 0);
			pls->init();
		}

		/* *** SIMULATION ITERATION LOOP *** */
		cerr << "MainLoop started" << endl;
		float timeVar = 0;
		NTreal inpCurrent = 0.0;

		NT_uniform_rnd_dist_o uniformRnd;

		NTreal timeInMS = 0;
		int dataRead = 0;
		for (NTsize lt = 0; lt < simulation_parameters["numIter"].asUInt();
				lt++) {
			timeInMS += oModel._timeStep();
			timeVar = timeInMS;
			// Write number of columns
			if (simulation_parameters["sampN"].asInt() > 0 && lt == 0
					&& lTrials == 0) {
				for (unsigned int ll = 0; ll < numCompartments; ++ll) {
					NTsize number_of_currents =
							oModel.compartmentVec[ll]->currentVec.size() + 1;
					pot_current_files[ll]->write(
							reinterpret_cast<char*>(&number_of_currents),
							sizeof(NTsize));
				}
			}
			/* the "sampling ratio" used for "measurement" to disk */
			if (simulation_parameters["sampN"].asInt() > 0
					&& lt % simulation_parameters["sampN"].asInt() == 0) {
				for (unsigned int ll = 0; ll < numCompartments; ++ll) {
					oModel.WriteCompartmentData(pot_current_files[ll], ll);
				}
				TimeFile << timeVar << endl;
			}

			PLFLT voltVec[oModel._numCompartments()];
			PLFLT x[oModel._numCompartments()];
			x[0]=0;

			if (simulation_parameters["useVis"].asInt() > 0) {
				if(lt == 0){
					for (NTsize lc = 1; lc < numCompartments; lc++) {
						x[lc]=x[lc-1]+oModel.compartmentVec[lc]->_length();
					}
					pls->env(0, x[numCompartments-1], -100, 100, 0, 0);
				}
				if (lt % simulation_parameters["useVis"].asInt() == 0) {
					pls->clear();
					pls->box("abcnt", 0, 0, "anvbct", 0, 0);
					for (NTsize ll = 0; ll < oModel._numCompartments(); ll++) {
						voltVec[ll] = oModel.compartmentVec[ll]->_vM();
					}
					for (NTsize lc = 0; lc < numCompartments; lc++) {
						if (NTisnan(voltVec[lc])) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is NaN." << endl;
							return (EXIT_V_TOO_HIGH);
						} else if (voltVec[lc] > 200.0 /* mV */) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is " << voltVec[lc] << "." << endl;
							return (EXIT_V_TOO_HIGH);
						}
					}
					pls->line((PLINT) oModel._numCompartments(), x, voltVec);
					pls->flush();
				}
			}
			if (lt % simulation_parameters["readN"].asInt() == 0) {
				inpCurrent = (inputData[dataRead]
						* simulation_parameters["inpISDV"].asDouble())
						+ simulation_parameters["inpI"].asDouble();
				dataRead++;
				cout << lt << "\t" << inpCurrent << endl;
			}
			oModel.InjectCurrent(inpCurrent, 1);

			oModel.Step();
		}

	} // lTrials
	cerr << "Simulation completed." << endl;
	cerr << "Number of compartments=" << numCompartments << endl;
	ConfigUsedFile << "Number_of_compartments=" << numCompartments << endl;
	ConfigUsedFile.close();
	for (auto ci = pot_current_files.begin(); ci != pot_current_files.end();
			++ci) {
		auto file = *ci;
		file->close();
		delete file;
	}

	return 0;
}

int get_resting_potential(string fileName) {
	read_config(fileName);
	double min = -90;
	double max = -40;
	double current_guess;
	double current_result = 100000;

	ofstream temp;
	temp.open("/dev/null");

	while (current_result > -79.9 || current_result < -80.1) {
		/* Model setup */
		current_guess = (min + max) / 2;
		cout << "Trying with " << current_guess << endl;
		config_root["eLeak"] = current_guess;
		config_root["node_parameters"]["numNd"] = 5;
		vector<NTsize> nodes_vec, nodes_paranodes_vec;
		/* Model setup */
		NTBP_membrane_compartment_sequence_o oModel = create_axon(config_root,
				temp, temp, nodes_vec, nodes_paranodes_vec);

		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = hillock_parameters["numComp"].asUInt()
				+ (node_parameters["numNd"].asUInt() - 1)
						* (node_parameters["numComp"].asUInt()
								+ 2 * paranode_parameters["numComp"].asUInt()
								+ internode_parameters["numComp"].asUInt())
				+ node_parameters["numComp"].asUInt()
				+ paranode_parameters["numComp"].asUInt();
		cerr << "Total number of compartments(computed)" << numCompartments
				<< endl;
		//cerr << "Total number of compartments(assembled)" << compartmentCounter
		//	<< endl;
		cerr << "Total number of compartments(in oModel)"
				<< oModel._numCompartments() << endl;

		/* ***********************  Main loop **************************** */
		cerr << "MainLoop started" << endl;

		double sum = 0;
		for (NTsize lt = 0; lt < simulation_parameters["numIter"].asUInt();
				lt++) {

			/* the "sampling ratio" used for "measurement" to disk */
			if (lt % 10000 == 0) {
				sum = 0;
				for (NTsize ll = 0; ll < oModel._numCompartments(); ll++) {
					sum += oModel.compartmentVec[ll]->_vM();
				}
				cout << "Mean voltage = " << sum / oModel._numCompartments()
						<< endl;
			}

			if (lt == 10000) {
				NTreal inpCurrent = (5
						* simulation_parameters["inpISDV"].asDouble())
						+ simulation_parameters["inpI"].asDouble();
				oModel.InjectCurrent(inpCurrent, 1);
			} else {
				oModel.InjectCurrent(0, 1);
			}

			oModel.Step();
		}
		current_result = sum / oModel._numCompartments();
		if (current_result > -80) {
			max = current_guess;
		}
		if (current_result < -80) {
			min = current_guess;
		}
	}
	cerr << "Simulation completed. Found " << current_guess << endl;
	return 0;
}

int test() {

	NTBP_lua_based_stochastic_multi_current_o * lua_current =
			new NTBP_lua_based_stochastic_multi_current_o(0, 0 /* mum^-2 */,
					0/* pS */, 0/* mV */, 0/* mV */, 0.1, 6.3/* C */,
					"/home/man210/Dropbox/workspace/ChannelGenerators/src/lua/SGA_sodium.lua");
	lua_current->SetSimulationMode(NTBP_DETERMINISTIC);

	NTBP_file_based_stochastic_multi_current_o * file_current =
			new NTBP_file_based_stochastic_multi_current_o(0, 0 /* mum^-2 */,
					0/* pS */, 0/* mV */, 0/* mV */, 0.1, 6.3/* C */,
					"/home/man210/thesis/channels/SGA_sodium.json");
	file_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);

	NTsize length = floor(200 / 0.01 + 0.5) + 1;
	for (NTsize i = 1; i <= 8; ++i) {
		for (NTsize j = 1; j <= 8; ++j) {
			for (NTsize k = 0; k < length; k++) {
				NTreal diff =
						NTBP_lua_based_stochastic_multi_current_o::probability_matrix_map["/home/man210/Dropbox/workspace/ChannelGenerators/src/lua/SGA_sodium.lua"]->getTransitionProbability(
								k, i, j)
								- NTBP_file_based_stochastic_multi_current_o::probability_matrix_map["/home/man210/thesis/channels/SGA_sodium.json"]->getTransitionProbability(
										k, i, j);
				if (diff > 0.01 || diff < -0.01) {
					cout << "Merde "
							<< NTBP_lua_based_stochastic_multi_current_o::probability_matrix_map["/home/man210/Dropbox/workspace/ChannelGenerators/src/lua/SGA_sodium.lua"]->getTransitionProbability(
									k, i, j) << " =/= "
							<< NTBP_file_based_stochastic_multi_current_o::probability_matrix_map["/home/man210/thesis/channels/SGA_sodium.json"]->getTransitionProbability(
									k, i, j) << " from " << i << " to " << j
							<< " at " << (-100 + 0.01 * k) << endl;
				}
			}
		}
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if (strcmp(argv[1], "resting") == 0) {
		return get_resting_potential(argv[2]);
	}
	if (strcmp(argv[1], "simulate") == 0) {
		return simulate(argv[2]);
	}
	if (strcmp(argv[1], "test") == 0) {
		return test();
	}
	return 1;
}
