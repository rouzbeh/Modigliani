/**
 * @file axon_simulator
 * Can simulate arbitrary axons
 * @version  2.0
 * @author Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
 * @author Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
 *
 * @section LICENSE
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

#ifdef WITH_PLPLOT
#include <plplot/plplot.h>
#include <plplot/plstream.h>
#endif
#include <mcore/auxfunc.h>

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 * @return A JSON structure containing the parameters
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

std::vector<mbase::Size_t> get_electrods(Json::Value root) {
	auto outvec = std::vector<mbase::Size_t>(100);
	string lua_script = root["electrods_lua"].asString();
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, lua_script.c_str());

	lua_getglobal(L, "electrods");
	/* table is in the stack at index 't' */
	lua_pushnil(L); /* first key */
	while (lua_next(L, -2) != 0) {
		/* uses 'key' (at index -2) and 'value' (at index -1) */
		int found = lua_tonumber(L, -1);
		outvec.push_back(found);
		lua_pop(L, 1);
	}
	lua_close(L);
	return (outvec);
}

/**
 * Runs a simulation using parameters in the given json file.
 *
 * @param filename
 * @return
 */
int simulate(string fileName) {
	mbase::Size_t numCompartments;
	Json::Value config_root = read_config(fileName);
	string timedOutputFolder;
	// What compartments to save
	auto electrods_vec = get_electrods(config_root);
	// We write each compartment's potential and currents into a single file.
	ofstream TimeFile, LengthPerCompartmentFile, TypePerCompartmentFile,
			log_file;

	std::vector<ofstream*> pot_current_files;
	if (config_root["simulation_parameters"].get("sampN", 0).asUInt() > 0) {
		timedOutputFolder =
				mcore::createOutputFolder(
						config_root["simulation_parameters"]["outputFolder"].asString());

		std::ifstream ifs(fileName, std::ios::binary);
		string temp_string = timedOutputFolder;
		std::ofstream ofs(temp_string.append("/conf.json").c_str(),
				std::ios::binary);

		ofs << ifs.rdbuf();
		ofs.close();
		ifs.close();

		mcore::openOutputFile(timedOutputFolder, "Time", TimeFile);
		mcore::openOutputFile(timedOutputFolder, "TypePerCompartment",
				TypePerCompartmentFile);
		mcore::openOutputFile(timedOutputFolder, "LengthPerCompartment",
				LengthPerCompartmentFile);
		mcore::openOutputFile(timedOutputFolder, "log", log_file, ".log");
		TimeFile << "% in ms" << std::endl;

		for_each(electrods_vec.begin(), electrods_vec.end(),
				[&pot_current_files,timedOutputFolder](mbase::Size_t ll) {
					pot_current_files.push_back(mcore::openOutputFile(timedOutputFolder, "compartment",
									ll, ".bin"));
				});
	}
	else{
		mcore::openOutputFile("/tmp", "log", log_file, ".log");
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
	mbase::Size_t index = 0;
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

	/* *** Trials loop *** */
	for (mbase::Size_t lTrials = 0;
			lTrials < config_root["simulation_parameters"]["numTrials"].asUInt();
			lTrials++) {
		/* Model setup */
		mcore::Membrane_compartment_sequence* oModel = mcore::create_axon(
				config_root, TypePerCompartmentFile, LengthPerCompartmentFile);

		if (!lTrials) {
			TypePerCompartmentFile.close();
			LengthPerCompartmentFile.close();
		}
		oModel->Init();

#ifdef WITH_PLPLOT
		PLFLT voltVec[oModel->_numCompartments()];
		PLFLT x[oModel->_numCompartments()];
		x[0] = 0;
#endif

		numCompartments = oModel->_numCompartments();
		log_file << "Total number of compartments(in oModel)"
				<< numCompartments << std::endl;
		std::vector<mbase::Real> leakCurrVec(numCompartments);
		std::vector<mbase::Real> naCurrVec(numCompartments);
		std::vector<mbase::Real> kCurrVec(numCompartments);

#ifdef WITH_PLPLOT
		/* Graphics init */
		plstream* pls = 0;
		if (config_root["simulation_parameters"]["useVis"].asInt() > 0) {
			pls = new plstream();
			// Initialize plplot.
			pls->sdev("wxwidgets");
			pls->scolbg(255, 255, 255);
			pls->scol0(1, 0, 0, 0);
			pls->init();
		}
#endif

		/* *** SIMULATION ITERATION LOOP *** */
		std::cerr << "MainLoop started" << std::endl;
		float timeVar = 0;
		mbase::Real inpCurrent = 0.0;

		mbase::Uniform_rnd_dist uniformRnd;

		mbase::Real timeInMS = 0;
		int dataRead = 0;
		for (mbase::Size_t lt = 0;
				lt < config_root["simulation_parameters"]["numIter"].asUInt();
				lt++) {
			timeInMS += oModel->_timeStep();
			timeVar = timeInMS;
			// Write number of columns
			if (config_root["simulation_parameters"]["sampN"].asInt() > 0
					&& lt == 0 && lTrials == 0) {
				mbase::Size_t counter = 0;
				for (auto ci = electrods_vec.begin(); ci != electrods_vec.end();
						ci++) {
					mbase::Size_t number_of_currents =
							oModel->compartmentVec[*ci]->currentVec.size() + 1;
					pot_current_files[counter++]->write(
							reinterpret_cast<char*>(&number_of_currents),
							sizeof(mbase::Size_t));
				}
			}
			/* the "sampling ratio" used for "measurement" to disk */
			if (config_root["simulation_parameters"]["sampN"].asInt() > 0
					&& lt
							% config_root["simulation_parameters"]["sampN"].asInt()
							== 0) {
				mbase::Size_t counter = 0;
				for (auto ci = electrods_vec.begin(); ci != electrods_vec.end();
						ci++) {
					oModel->WriteCompartmentData(pot_current_files[counter++],
							*ci);
				}
				if (!lTrials)
					TimeFile << timeVar << std::endl;
			}
#ifdef WITH_PLPLOT
			if (config_root["simulation_parameters"]["useVis"].asInt() > 0) {
				if (lt == 0) {
					for (mbase::Size_t lc = 1; lc < numCompartments; lc++) {
						x[lc] = x[lc - 1]
								+ oModel->compartmentVec[lc]->_length();
					}
					pls->env(0, x[numCompartments - 1], -100, 100, 0, 0);
				}
				if (lt % config_root["simulation_parameters"]["useVis"].asInt()
						== 0) {
					pls->clear();
					pls->box("abcnt", 0, 0, "anvbct", 0, 0);
					for (mbase::Size_t ll = 0; ll < oModel->_numCompartments();
							ll++) {
						voltVec[ll] = oModel->compartmentVec[ll]->_vM();
					}
					for (auto iv : voltVec) {
						if (mbase::Misnan(iv)) {
							log_file << "ERROR at t=" << timeVar
									<< " voltage is NaN." << std::endl;
							std::exit(1);
						} else if (iv > 200.0 /* mV */) {
							log_file << "ERROR at t=" << timeVar
									<< " voltage in compartment  is " << iv
									<< "." << std::endl;
							std::exit(1);
						}
					}
					pls->line((PLINT) oModel->_numCompartments(), x, voltVec);
					pls->flush();
				}
			}
#endif
			if (lt % config_root["simulation_parameters"]["readN"].asInt()
					== 0) {
				inpCurrent =
						(inputData[dataRead]
								* config_root["simulation_parameters"]["inpISDV"].asDouble())
								+ config_root["simulation_parameters"]["inpI"].asDouble();
				dataRead++;
			}
			oModel->InjectCurrent(inpCurrent, 1);

			oModel->step();
		}
#ifdef WITH_PLPLOT
		if (pls)
			delete pls;
#endif
		delete oModel;
	} // lTrials
	log_file << "Simulation completed." << std::endl;
	log_file << "Number_of_compartments=" << numCompartments << std::endl;
	log_file.close();
	for (auto ci = pot_current_files.begin(); ci != pot_current_files.end();
			++ci) {
		auto file = *ci;
		file->close();
		delete file;
	}

	return (0);
}

int test() {
	volatile mbase::Real result = 0;
	mbase::Binomial_rnd_dist rnd(0.3, 100);

//	for (int i=0; i< 100000000; i++){
//		result = rnd.RndVal();
//	}

	return (result == 0);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "What shall I do?" << std::endl;
		exit(1);
	}
	if (strcmp(argv[1], "simulate") == 0) {
		return (simulate(argv[2]));
	}
	if (strcmp(argv[1], "test") == 0) {
		return (test());
	}
	return (1);
}
