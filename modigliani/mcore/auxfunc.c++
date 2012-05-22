/**\file auxfunc.cpp -  Auxiliary function implementation for heterogeneous object creation
 * by Ahmed Ali Neishabouri &copy; created 01.4.2012
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998-2005 Ahmed Aldo Faisal    
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

#include "auxfunc.h"

using namespace std;

namespace mcore {
mbase::Real corrected_channel_density(mbase::Real chDensity, mbase::Real compArea) {
	mbase::Real chPerCompartment = compArea * chDensity;
	mbase::Real pChFloor = (ceil(chPerCompartment) - chPerCompartment);

	mbase::Uniform_rnd_dist uniRnd;
	/* compute number of channels, such that average density is achieved */
	mbase::Real indChDensity = (
			uniRnd.RndVal() > pChFloor ?
					ceil(chPerCompartment) / compArea :
					floor(chPerCompartment) / compArea);

	return (indChDensity);
}

/**
 * Creates a new folder in the output directory
 * and puts a timestamp in its name.
 * @param output Folder The folder in which to create the new folder.
 * @return Name of the newly created folder
 */
string createOutputFolder(string outputFolder) {
	/* open files */
	time_t rawtime;
	struct tm * timeinfo;
	stringstream ss(stringstream::in | stringstream::out);
	char dateString[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	const char* format = "%b%d_%H%M%S";
	size_t max_size = 80;
	strftime(dateString, max_size, format, timeinfo);
	ss << outputFolder << dateString;
	string folderName;
	ss >> folderName;
	stringstream ss2(stringstream::in | stringstream::out);
	ss2 << folderName << "/compartments/";
	string temp_folder_name;
	ss2 >> temp_folder_name;

	boost::filesystem::create_directories(temp_folder_name);

	return (folderName);
}

/**
 * Formats and prints current parameters into the ofstream given as input.
 * @param out
 */
void printConfig(ofstream& out, Json::Value node_parameters,
		Json::Value paranode_parameters, Json::Value internode_parameters,
		Json::Value simulation_parameters, Json::Value config_root) {
	out << "global_diameter" << " = " << config_root["diameter"].asDouble()
			<< ";" << std::endl;
	out << "global_eLeak" << " = " << config_root["eLeak"].asDouble() << ";"
			<< std::endl;
	out << "global_temperature" << " = "
			<< config_root["temperature"].asDouble() << ";" << std::endl;
	out << "global_vBase" << " = " << config_root["vBase"].asDouble() << ";"
			<< std::endl;
	out << "node_cm" << " = " << node_parameters["Cm"].asDouble() << ";"
			<< std::endl;
	out << "node_gLeak" << " = " << node_parameters["GLeak"].asDouble() << ";"
			<< std::endl;
	out << "node_length" << " = " << node_parameters["length"].asDouble() << ";"
			<< std::endl;
	out << "node_num" << " = " << node_parameters["numNd"].asUInt() << ";"
			<< std::endl;
	out << "node_numComp" << " = " << node_parameters["numComp"].asDouble()
			<< ";" << std::endl;
	out << "node_potassiumAlg" << " = " << node_parameters["chKAlg"].asUInt()
			<< ";" << std::endl;
	out << "node_potassiumConductance" << " = "
			<< node_parameters["chKCond"].asDouble() << ";" << std::endl;
	out << "node_potassiumDensity" << " = "
			<< node_parameters["chKDen"].asDouble() << ";" << std::endl;
	out << "node_potassiumReversalPotential" << " = "
			<< node_parameters["chKRevPot"].asDouble() << ";" << std::endl;
	out << "node_ra" << " = " << node_parameters["Ra"].asDouble() << ";"
			<< std::endl;
	out << "node_sodiumAlg" << " = " << node_parameters["chNaAlg"].asUInt()
			<< ";" << std::endl;
	out << "node_sodiumConductance" << " = "
			<< node_parameters["chNaCond"].asDouble() << ";" << std::endl;
	out << "node_sodiumDensity" << " = "
			<< node_parameters["chNaDen"].asDouble() << ";" << std::endl;
	out << "node_sodiumReversalPotential" << " = "
			<< node_parameters["chNaRevPot"].asDouble() << ";" << std::endl;
	out << "paranode_cm" << " = " << paranode_parameters["Cm"].asDouble() << ";"
			<< std::endl;
	out << "paranode_gLeak" << " = " << paranode_parameters["GLeak"].asDouble()
			<< ";" << std::endl;
	out << "paranode_length" << " = "
			<< paranode_parameters["length"].asDouble() << ";" << std::endl;
	out << "paranode_numComp" << " = "
			<< paranode_parameters["numComp"].asDouble() << ";" << std::endl;

	out << "internode_cm" << " = " << internode_parameters["Cm"].asDouble()
			<< ";" << std::endl;
	out << "internode_gLeak" << " = "
			<< internode_parameters["GLeak"].asDouble() << ";" << std::endl;
	out << "internode_length" << " = "
			<< internode_parameters["length"].asDouble() << ";" << std::endl;
	out << "internode_numComp" << " = "
			<< internode_parameters["numComp"].asDouble() << ";" << std::endl;

	out << "simulation_samplerate" << " = "
			<< simulation_parameters["sampN"].asUInt() << ";" << std::endl;
	out << "simulation_timestep_inms" << " = "
			<< simulation_parameters["timeStep"].asDouble() << ";" << std::endl;
	out << "simulation_number_of_iterations" << " = "
			<< simulation_parameters["numIter"].asUInt() << ";" << std::endl;
	out << "simulation_duration" << " = "
			<< simulation_parameters["timeStep"].asDouble()
					* simulation_parameters["numIter"].asUInt() << ";" << std::endl;
	out << "simulation_trials" << " = "
			<< simulation_parameters["numTrials"].asUInt() << ";" << std::endl;
}

/**
 * Creates a compartment using the parameters supplied in the parameters structs supplied.
 * @return The constructed compartment.
 */
Custom_cylindrical_compartment* createCompartment(Json::Value config_root,
		Json::Value simulation_parameters, Json::Value compartment_parameters) {

	Custom_cylindrical_compartment *tmpPtr = new Custom_cylindrical_compartment(
			compartment_parameters["length"].asDouble() /* muMeter */,
			config_root["diameter"].asDouble() /* muMeter */,
			compartment_parameters["Cm"].asDouble()/*muFarad/cm^2 */,
			compartment_parameters["Ra"].asDouble() /* ohm cm */,
			config_root["temperature"].asDouble());

	bool randomise_densities = simulation_parameters["randomise_densities"].asBool();

	// Read a list of currents for each compartments
	Json::Value currents = compartment_parameters["currents"];
	for (unsigned int index = 0; index < currents.size(); ++index) {
		Json::Value current = currents[index];

		if ("leak" == current["type"].asString()) {
			tmpPtr->AttachCurrent(
					new HH_sga_leak_current(tmpPtr->_area(),
							current["GLeak"].asDouble(),
							config_root["eLeak"].asDouble()), NTBP_LEAK);
			continue;
		}

		if ("file" == current["type"].asString()) {
			mbase::Real indDensity = corrected_channel_density(
					current["chDen"].asDouble(), tmpPtr->_area());
			File_based_stochastic_multi_current * file_current =
					new File_based_stochastic_multi_current(tmpPtr->_area(),
							(randomise_densities ? indDensity:current["chDen"].asDouble()) /* mum^-2 */,
							current["chCond"].asDouble() * 1e-9 /* pS */,
							config_root["vBase"].asDouble() /* mV */,
							current["chRevPot"].asDouble() /* mV */,
							simulation_parameters["timeStep"].asDouble(),
							config_root["temperature"].asDouble() /* C */,
							current["chModel"].asString());
			file_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
			tmpPtr->AttachCurrent(file_current, NTBP_IONIC);
			continue;
		}

		if ("lua" == current["type"].asString()) {
			if (1 == current["chAlg"].asInt()) {
				Lua_based_deterministic_multi_current * lua_current =
						new Lua_based_deterministic_multi_current(
								tmpPtr->_area(),
								current["chDen"].asDouble() /* mum^-2 */,
								current["chCond"].asDouble() * 1e-9 /* pS */,
								config_root["vBase"].asDouble() /* mV */,
								current["chRevPot"].asDouble() /* mV */,
								simulation_parameters["timeStep"].asDouble(),
								config_root["temperature"].asDouble() /* C */,
								current["chModel"].asString());
				lua_current->SetSimulationMode(NTBP_DETERMINISTIC);
				tmpPtr->AttachCurrent(lua_current, NTBP_IONIC);
				continue;
			} else if (4 == current["chAlg"].asInt()) {
				mbase::Real indDensity = corrected_channel_density(
						current["chDen"].asDouble(), tmpPtr->_area());
				Lua_based_stochastic_multi_current * lua_current =
						new Lua_based_stochastic_multi_current(tmpPtr->_area(),
								(randomise_densities ? indDensity:current["chDen"].asDouble()) /* mum^-2 */,
								current["chCond"].asDouble() * 1e-9 /* pS */,
								config_root["vBase"].asDouble() /* mV */,
								current["chRevPot"].asDouble() /* mV */,
								simulation_parameters["timeStep"].asDouble(),
								config_root["temperature"].asDouble() /* C */,
								current["chModel"].asString());
				lua_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
				tmpPtr->AttachCurrent(lua_current, NTBP_IONIC);
				continue;
			}
		}
	}
	return (tmpPtr);
}

/**
 * Opens a new file in write mode.
 * @param output Folder The folder in which to create the new file.
 * @param prefix File name prefix
 * @param outStream Will contain an ofstream pointing to the newly created file.
 */
void openOutputFile(string outputFolder, string prefix, ofstream& outStream,
		string extension) {
	/* open files */
	stringstream ss(stringstream::in | stringstream::out);
	ss << outputFolder << "/" << prefix << extension;
	string temp_name;
	ss >> temp_name;

	outStream.open(temp_name.c_str(), ios::binary);

	if (outStream.fail()) {
		std::cerr << "Could not open output file " << prefix << std::endl;
		std::exit(EXIT_IO_ERROR);
	}
}

/**
 * Opens a new file in write mode.
 * @param output Folder The folder in which to create the new file.
 * @param prefix File name prefix
 * @param outStream Will contain an ofstream pointing to the newly created file.
 */
ofstream* openOutputFile(string outputFolder, string prefix, int counter,
		string extension) {
	/* open files */
	stringstream ss(stringstream::in | stringstream::out);
	ss << outputFolder << "/compartments/" << prefix << "_" << counter
			<< extension;
	string temp_name;
	ss >> temp_name;

	ofstream* out_stream = new ofstream(temp_name.c_str(), ios::binary);

	if (out_stream->fail()) {
		std::cerr << "Could not open output file " << temp_name << std::endl;
		std::exit(EXIT_IO_ERROR);
	}
	return (out_stream);
}

Membrane_compartment_sequence* create_axon(Json::Value config_root,
		ofstream& TypePerCompartmentFile, ofstream& LengthPerCompartmentFile) {

	string lua_script = config_root["anatomy_lua"].asString();
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dostring(L, lua_script.c_str());

	auto compartment_types = std::vector<int>();
	lua_getglobal(L, "compartments");
	/* table is in the stack at index 't' */
	lua_pushnil(L); /* first key */
	while (lua_next(L, -2) != 0) {
		/* uses 'key' (at index -2) and 'value' (at index -1) */
		int found = lua_tonumber(L, -1);
		compartment_types.push_back(found);
		lua_pop(L, 1);
	}
	lua_close(L);

	auto oModel = new Membrane_compartment_sequence();
	oModel->update_timeStep(
			config_root["simulation_parameters"]["timeStep"].asDouble() /* mSec */);
	oModel->StepNTBP();

	Json::Value compartments_parameters = config_root["compartments_parameters"];

	for (std::vector<int>::iterator it = compartment_types.begin();
			it != compartment_types.end(); it++) {
		TypePerCompartmentFile << *it << std::endl;
		LengthPerCompartmentFile
				<< compartments_parameters[*it]["length"].asDouble() << std::endl;
		oModel->PushBack(
				createCompartment(config_root,
						config_root["simulation_parameters"],
						compartments_parameters[*it]));
	}

	return (oModel);

}

}
