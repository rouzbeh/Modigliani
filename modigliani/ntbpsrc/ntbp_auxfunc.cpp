/**\file ntbp_auxfunc.cpp - NTBP auxiliary function implementation for heterogeneous object creation
 * by Ahmed Aldo Faisal &copy; created 22.4.2005  
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

/* $Id: ntbp_auxfunc.cpp,v 1.1 2005/04/25 13:55:27 face Exp $
 * $Log: ntbp_auxfunc.cpp,v $
 * Revision 1.1  2005/04/25 13:55:27  face
 * *** empty log message ***
 *
 */

#include "ntbp_auxfunc.h"
using namespace std;

NTreal corrected_channel_density(NTreal chDensity, NTreal compArea) {
	NTreal chPerCompartment = compArea * chDensity;
	NTreal pChFloor = (ceil(chPerCompartment) - chPerCompartment);

	NT_uniform_rnd_dist_o uniRnd;
	/* compute number of channels, such that average density is achieved */
	NTreal indChDensity = (
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
			<< ";" << endl;
	out << "global_eLeak" << " = " << config_root["eLeak"].asDouble() << ";"
			<< endl;
	out << "global_hillock" << " = " << config_root["hillock"].asBool() << ";"
			<< endl;
	out << "global_internode" << " = " << config_root["internode"].asBool()
			<< ";" << endl;
	out << "global_node" << " = " << config_root["node"].asBool() << ";"
			<< endl;
	out << "global_paranode" << " = " << config_root["paranode"].asBool() << ";"
			<< endl;
	out << "global_temperature" << " = "
			<< config_root["temperature"].asDouble() << ";" << endl;
	out << "global_vBase" << " = " << config_root["vBase"].asDouble() << ";"
			<< endl;
	out << "node_cm" << " = " << node_parameters["Cm"].asDouble() << ";"
			<< endl;
	out << "node_gLeak" << " = " << node_parameters["GLeak"].asDouble() << ";"
			<< endl;
	out << "node_length" << " = " << node_parameters["length"].asDouble() << ";"
			<< endl;
	out << "node_num" << " = " << node_parameters["numNd"].asUInt() << ";"
			<< endl;
	out << "node_numComp" << " = " << node_parameters["numComp"].asDouble()
			<< ";" << endl;
	out << "node_potassiumAlg" << " = " << node_parameters["chKAlg"].asUInt()
			<< ";" << endl;
	out << "node_potassiumConductance" << " = "
			<< node_parameters["chKCond"].asDouble() << ";" << endl;
	out << "node_potassiumDensity" << " = "
			<< node_parameters["chKDen"].asDouble() << ";" << endl;
	out << "node_potassiumReversalPotential" << " = "
			<< node_parameters["chKRevPot"].asDouble() << ";" << endl;
	out << "node_ra" << " = " << node_parameters["Ra"].asDouble() << ";"
			<< endl;
	out << "node_sodiumAlg" << " = " << node_parameters["chNaAlg"].asUInt()
			<< ";" << endl;
	out << "node_sodiumConductance" << " = "
			<< node_parameters["chNaCond"].asDouble() << ";" << endl;
	out << "node_sodiumDensity" << " = "
			<< node_parameters["chNaDen"].asDouble() << ";" << endl;
	out << "node_sodiumReversalPotential" << " = "
			<< node_parameters["chNaRevPot"].asDouble() << ";" << endl;
	out << "paranode_cm" << " = " << paranode_parameters["Cm"].asDouble() << ";"
			<< endl;
	out << "paranode_gLeak" << " = " << paranode_parameters["GLeak"].asDouble()
			<< ";" << endl;
	out << "paranode_length" << " = "
			<< paranode_parameters["length"].asDouble() << ";" << endl;
	out << "paranode_numComp" << " = "
			<< paranode_parameters["numComp"].asDouble() << ";" << endl;

	out << "internode_cm" << " = " << internode_parameters["Cm"].asDouble()
			<< ";" << endl;
	out << "internode_gLeak" << " = "
			<< internode_parameters["GLeak"].asDouble() << ";" << endl;
	out << "internode_length" << " = "
			<< internode_parameters["length"].asDouble() << ";" << endl;
	out << "internode_numComp" << " = "
			<< internode_parameters["numComp"].asDouble() << ";" << endl;

	out << "simulation_samplerate" << " = "
			<< simulation_parameters["sampN"].asUInt() << ";" << endl;
	out << "simulation_timestep_inms" << " = "
			<< simulation_parameters["timeStep"].asDouble() << ";" << endl;
	out << "simulation_number_of_iterations" << " = "
			<< simulation_parameters["numIter"].asUInt() << ";" << endl;
	out
			<< "simulation_duration"
			<< " = "
			<< simulation_parameters["timeStep"].asDouble()
					* simulation_parameters["numIter"].asUInt() << ";" << endl;
	out << "simulation_trials" << " = "
			<< simulation_parameters["numTrials"].asUInt() << ";" << endl;
}

/**
 * Creates a compartment using the parameters supplied in the parameters structs supplied.
 * @return The constructed compartment.
 */
NTBP_custom_cylindrical_compartment_o* createCompartment(
		Json::Value config_root, Json::Value simulation_parameters,
		Json::Value compartment_parameters) {

	NTBP_custom_cylindrical_compartment_o *tmpPtr =
			new NTBP_custom_cylindrical_compartment_o(
					compartment_parameters["length"].asDouble() /* muMeter */,
					config_root["diameter"].asDouble() /* muMeter */,
					compartment_parameters["Cm"].asDouble()/*muFarad/cm^2 */,
					compartment_parameters["Ra"].asDouble() /* ohm cm */,
					config_root["temperature"].asDouble());

	// Read a list of currents for each compartments
	Json::Value currents = compartment_parameters["currents"];
	for (unsigned int index = 0; index < currents.size(); ++index) {
		Json::Value current = currents[index];

		if ("leak" == current["type"].asString()) {
			tmpPtr->AttachCurrent(
					new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
							current["GLeak"].asDouble(),
							config_root["eLeak"].asDouble()), NTBP_LEAK);
			continue;
		}

		if ("file" == current["type"].asString()) {
			NTreal indDensity = corrected_channel_density(
					current["chDen"].asDouble(), tmpPtr->_area());
			NTBP_file_based_stochastic_multi_current_o * file_current =
					new NTBP_file_based_stochastic_multi_current_o(
							tmpPtr->_area(), indDensity /* mum^-2 */,
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
				NTreal indDensity = corrected_channel_density(
						current["chDen"].asDouble(), tmpPtr->_area());
				NTBP_lua_based_deterministic_multi_current_o * lua_current =
						new NTBP_lua_based_deterministic_multi_current_o(
								tmpPtr->_area(), indDensity /* mum^-2 */,
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
				NTreal indDensity = corrected_channel_density(
						current["chDen"].asDouble(), tmpPtr->_area());
				NTBP_lua_based_stochastic_multi_current_o * lua_current =
						new NTBP_lua_based_stochastic_multi_current_o(
								tmpPtr->_area(), indDensity /* mum^-2 */,
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
		cerr << "Could not open output file " << prefix << endl;
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
		cerr << "Could not open output file " << temp_name << endl;
		std::exit(EXIT_IO_ERROR);
	}
	return (out_stream);
}

NTBP_membrane_compartment_sequence_o create_axon(Json::Value config_root,
		ofstream& TypePerCompartmentFile, ofstream& LengthPerCompartmentFile,
		vector<NTsize>& nodes_vec, vector<NTsize>& nodes_paranodes_vec) {

	Json::Value hillock_parameters;
	Json::Value node_parameters;
	Json::Value paranode_parameters;
	Json::Value internode_parameters;
	Json::Value simulation_parameters;

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
	NTBP_membrane_compartment_sequence_o oModel;
	oModel.UpdateTimeStep(
			simulation_parameters["timeStep"].asDouble() /* mSec */);
	oModel.StepNTBP();

	NTsize compartmentCounter = 1;
	/* *** MODEL CREATION LOOP *** */

	// Generate an axon hillock
	if (config_root.get("hillock", false).asBool()) {
		for (NTsize lcomp = 0; lcomp < hillock_parameters["numComp"].asUInt();
				lcomp++) {
			TypePerCompartmentFile << compartmentCounter++ << " 0" << endl;
			LengthPerCompartmentFile << hillock_parameters["length"].asDouble()
					<< endl;
			oModel.PushBack(
					createCompartment(config_root, simulation_parameters,
							hillock_parameters));
		}
	}

	/* Create a Node, followed by Paranode, Internode, Paranode */
	for (NTsize lnd = 0; lnd < node_parameters["numNd"].asUInt(); lnd++) {
		/* Create a Node compartment */
		for (NTsize lcomp = 0; lcomp < node_parameters["numComp"].asUInt();
				lcomp++) {
			TypePerCompartmentFile << compartmentCounter++ << " 1" << endl;
			LengthPerCompartmentFile << node_parameters["length"].asDouble()
					<< endl;
			nodes_vec.push_back(compartmentCounter - 2);
			nodes_paranodes_vec.push_back(compartmentCounter - 2);
			oModel.PushBack(
					createCompartment(config_root, simulation_parameters,
							node_parameters));
		}

		/* Create a Paranode compartment */
		if (config_root.get("paranode", false).asBool()) {
			for (NTsize lcomp = 0;
					lcomp < paranode_parameters["numComp"].asUInt(); lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 2" << endl;
				LengthPerCompartmentFile
						<< paranode_parameters["length"].asDouble() << endl;
				nodes_paranodes_vec.push_back(compartmentCounter - 2);
				oModel.PushBack(
						createCompartment(config_root, simulation_parameters,
								paranode_parameters));
			}
		}

		if ((node_parameters["numNd"].asUInt() - 1) == lnd) {
			break;
		}

		/* Create an Internode compartment */
		if (config_root.get("internode", false).asBool()) {
			for (NTsize lcomp = 0;
					lcomp < internode_parameters["numComp"].asUInt(); lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 3" << endl;
				LengthPerCompartmentFile
						<< internode_parameters["length"].asDouble() << endl;
				oModel.PushBack(
						createCompartment(config_root, simulation_parameters,
								internode_parameters));
			}
		}

		if (config_root.get("paranode", false).asBool()) {
			for (NTsize lcomp = 0;
					lcomp < paranode_parameters["numComp"].asUInt(); lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 2" << endl;
				LengthPerCompartmentFile
						<< paranode_parameters["length"].asDouble() << endl;
				nodes_paranodes_vec.push_back(compartmentCounter - 2);
				oModel.PushBack(
						createCompartment(config_root, simulation_parameters,
								paranode_parameters));
			}
		}

	}

	return (oModel);
}
