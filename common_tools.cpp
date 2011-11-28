/*
 * common-tools.cpp
 *
 *  Created on: 8 Sep 2011
 *      Author: man210
 */

#include "common_tools.h"

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
	strftime(dateString, 80, "%b%d_%H%M%S", timeinfo);
	ss << outputFolder << "/" << dateString;
	string folderName;
	ss >> folderName;

	boost::filesystem::create_directories(folderName);

	return folderName;
}

/**
 * Formats and prints current parameters into the ofstream given as input.
 * @param out
 */
void printConfig(ofstream& out, Json::Value hillock_parameters,
		Json::Value node_parameters, Json::Value paranode_parameters,
		Json::Value internode_parameters, Json::Value simulation_parameters,
		Json::Value config_root) {
	out << "global_diameter" << " = " << config_root["diameter"].asDouble()
			<< ";" << endl;
	out << "global_eLeak" << " = " << config_root["eLeak"].asDouble() << ";" << endl;
	out << "global_hillock" << " = " << config_root["hillock"].asBool() << ";" << endl;
	out << "global_internode" << " = " << config_root["internode"].asBool()
			<< ";" << endl;
	out << "global_node" << " = " << config_root["node"].asBool() << ";" << endl;
	out << "global_paranode" << " = " << config_root["paranode"].asBool()
			<< ";" << endl;
	out << "global_temperature" << " = "
			<< config_root["temperature"].asDouble() << ";" << endl;
	out << "global_vBase" << " = " << config_root["vBase"].asDouble() << ";" << endl;
	out << "node_cm" << " = " << node_parameters["Cm"].asDouble() << ";" << endl;
	out << "node_gLeak" << " = " << node_parameters["GLeak"].asDouble() << ";" << endl;
	out << "node_length" << " = " << node_parameters["length"].asDouble()
			<< ";" << endl;
	out << "node_num" << " = " << node_parameters["numNd"].asUInt() << ";" << endl;
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
	out << "node_ra" << " = " << node_parameters["Ra"].asDouble() << ";" << endl;
	out << "node_sodiumAlg" << " = " << node_parameters["chNaAlg"].asUInt()
			<< ";" << endl;
	out << "node_sodiumConductance" << " = "
			<< node_parameters["chNaCond"].asDouble() << ";" << endl;
	out << "node_sodiumDensity" << " = "
			<< node_parameters["chNaDen"].asDouble() << ";" << endl;
	out << "node_sodiumReversalPotential" << " = "
			<< node_parameters["chNaRevPot"].asDouble() << ";" << endl;
	out << "paranode_cm" << " = " << paranode_parameters["Cm"].asDouble()
			<< ";" << endl;
	out << "paranode_gLeak" << " = " << paranode_parameters["GLeak"].asDouble()
			<< ";" << endl;
	out << "paranode_length" << " = "
			<< paranode_parameters["length"].asDouble() << ";" << endl;
	out << "paranode_numComp" << " = "
			<< paranode_parameters["numComp"].asDouble() << ";" << endl;
	out << "paranode_potassiumAlg" << " = "
			<< paranode_parameters["chKAlg"].asUInt() << ";" << endl;
	out << "paranode_potassiumConductance" << " = "
			<< paranode_parameters["chKCond"].asDouble() << ";" << endl;
	out << "paranode_potassiumDensity" << " = "
			<< paranode_parameters["chKDen"].asDouble() << ";" << endl;
	out << "paranode_potassiumReversalPotential" << " = "
			<< paranode_parameters["chKRevPot"].asDouble() << ";" << endl;
	out << "paranode_ra" << " = " << paranode_parameters["Ra"].asDouble()
			<< ";" << endl;
	out << "paranode_sodiumAlg" << " = "
			<< paranode_parameters["chNaAlg"].asUInt() << ";" << endl;
	out << "paranode_sodiumConductance" << " = "
			<< paranode_parameters["chNaCond"].asDouble() << ";" << endl;
	out << "paranode_sodiumDensity" << " = "
			<< paranode_parameters["chNaDen"].asDouble() << ";" << endl;
	out << "paranode_sodiumReversalPotential" << " = "
			<< paranode_parameters["chNaRevPot"].asDouble() << ";" << endl;
	out << "internode_cm" << " = " << internode_parameters["Cm"].asDouble()
			<< ";" << endl;
	out << "internode_gLeak" << " = "
			<< internode_parameters["GLeak"].asDouble() << ";" << endl;
	out << "internode_length" << " = "
			<< internode_parameters["length"].asDouble() << ";" << endl;
	out << "internode_numComp" << " = "
			<< internode_parameters["numComp"].asDouble() << ";" << endl;
	out << "internode_potassiumAlg" << " = "
			<< internode_parameters["chKAlg"].asUInt() << ";" << endl;
	out << "internode_potassiumConductance" << " = "
			<< internode_parameters["chKCond"].asDouble() << ";" << endl;
	out << "internode_potassiumDensity" << " = "
			<< internode_parameters["chKDen"].asDouble() << ";" << endl;
	out << "internode_potassiumReversalPotential" << " = "
			<< internode_parameters["chKRevPot"].asDouble() << ";" << endl;
	out << "internode_ra" << " = " << internode_parameters["Ra"].asDouble()
			<< ";" << endl;
	out << "internode_sodiumAlg" << " = "
			<< internode_parameters["chNaAlg"].asUInt() << ";" << endl;
	out << "internode_sodiumConductance" << " = "
			<< internode_parameters["chNaCond"].asDouble() << ";" << endl;
	out << "internode_sodiumDensity" << " = "
			<< internode_parameters["chNaDen"].asDouble() << ";" << endl;
	out << "internode_sodiumReversalPotential" << " = "
			<< internode_parameters["chNaRevPot"].asDouble() << ";" << endl;
	out << "simulation_samplerate" << " = "
			<< simulation_parameters["sampN"].asUInt() << ";" << endl;
	out << "simulation_timestep_inms" << " = "
			<< simulation_parameters["timeStep"].asDouble() << ";" << endl;
	out << "simulation_number_of_iterations" << " = "
			<< simulation_parameters["numIter"].asUInt() << ";" << endl;
	out << "simulation_duration" << " = "
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

	/* Leak current is number 0 */
	tmpPtr->AttachCurrent(
			new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
					compartment_parameters["GLeak"].asDouble(),
					config_root["eLeak"].asDouble()), NTBP_LEAK);

	/* Channel current is number 1 */
	if (compartment_parameters["chNaDen"].asDouble() > 0) {
		NTreal indSodiumDensity = NTBP_corrected_channel_density(
				compartment_parameters["chNaDen"].asDouble(), tmpPtr->_area());
		NTBP_file_based_multi_current_o * na_current =
				new NTBP_file_based_multi_current_o(
						tmpPtr->_area(),
						indSodiumDensity /* mum^-2 */,
						compartment_parameters["chNaCond"].asDouble()
								* 1e-9 /* pS */,
						config_root["vBase"].asDouble() /* mV */,
						compartment_parameters["chNaRevPot"].asDouble() /* mV */,
						simulation_parameters["timeStep"].asDouble(),
						config_root["temperature"].asDouble() /* C */,
						compartment_parameters["chNaModel"].asString());
		na_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		tmpPtr->AttachCurrent(na_current, NTBP_IONIC);

	} else
		tmpPtr->AttachCurrent(
				new NTBP_hh_sga_leak_current_o(tmpPtr->_area(), 0, 0),
				NTBP_LEAK);

	if (compartment_parameters["chKDen"].asDouble() > 0) {
		NTreal indPotassiumDensity = NTBP_corrected_channel_density(
				compartment_parameters["chKDen"].asDouble(), tmpPtr->_area());
		NTBP_file_based_multi_current_o * k_current =
				new NTBP_file_based_multi_current_o(
						tmpPtr->_area(),
						indPotassiumDensity /* mum^-2 */,
						compartment_parameters["chKCond"].asDouble()
								* 1e-9 /* pS */,
						config_root["vBase"].asDouble() /* mV */,
						compartment_parameters["chKRevPot"].asDouble() /* mV */,
						simulation_parameters["timeStep"].asDouble(),
						config_root["temperature"].asDouble() /* C */,
						compartment_parameters["chKModel"].asString());
		k_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		tmpPtr->AttachCurrent(k_current, NTBP_IONIC);
	} else
		tmpPtr->AttachCurrent(
				new NTBP_hh_sga_leak_current_o(tmpPtr->_area(), 0, 0),
				NTBP_LEAK);
	return tmpPtr;
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
		exit(EXIT_IO_ERROR);
	}
}

NTBP_membrane_compartment_sequence_o create_axon(Json::Value config_root,
		ofstream& TypePerCompartmentFile, ofstream& LengthPerCompartmentFile, vector<NTsize>& nodes_vec) {

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
			nodes_vec.push_back(compartmentCounter-2);
			NTBP_custom_cylindrical_compartment_o* compartment =
					createCompartment(config_root, simulation_parameters,
							node_parameters);
			NTBP_file_based_multi_current_o * persistant_na_current =
					new NTBP_file_based_multi_current_o(
							compartment->_area(),
							6.5 /* mum^-2 */,
							node_parameters["chNaCond"].asDouble()
									* 1e-9 /* pS */,
							config_root["vBase"].asDouble() /* mV */,
							node_parameters["chNaRevPot"].asDouble() /* mV */,
							simulation_parameters["timeStep"].asDouble(),
							config_root["temperature"].asDouble() /* C */,
							"/home/rouzbeh/thesis/channels/mcintyre_sodium_persistent.json");
			persistant_na_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
			//compartment->AttachCurrent(persistant_na_current, NTBP_IONIC);
			oModel.PushBack(compartment);
		}

		/* Create a Paranode compartment */
		if (config_root.get("paranode", false).asBool()) {
			for (NTsize lcomp = 0;
					lcomp < paranode_parameters["numComp"].asUInt(); lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 2" << endl;
				LengthPerCompartmentFile
						<< paranode_parameters["length"].asDouble() << endl;
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
				oModel.PushBack(
						createCompartment(config_root, simulation_parameters,
								paranode_parameters));
			}
		}

	}

	return oModel;
}
