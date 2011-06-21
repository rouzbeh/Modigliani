/**\file myelinaxon.cpp - myelinated axon simultor version 2
 * based on myelinaxonsimulator-v1.cpp - axon simulator version 1
 * by Ahmed Aldo Faisal &copy; created 25.9.2001
 * by Mohammad Ali Neishabouri
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  1.1
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

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <unistd.h>
#include <boost/filesystem.hpp>

#include <ntbp_membrane_compartment_sequence_obj.h>
#include <NTBP_file_based_multi_current_obj.h>
#include <ntbp_custom_cylindrical_compartment_obj.h>
#include <ntbp_auxfunc.h>

#include <nt_error_obj.h>
#include <nt_vector_obj.h>

#include <tnt.h>
#include <nt3d_plot2d_vec_vp_obj.h>
#include <nt3d_glx_drv_obj.h>
#include <nt_config_file_parser_obj.h>

using namespace std;
using namespace TNT;

/* Global */
string filename;
bool swComputeELeak;

typedef std::map<std::string, NTreal> parameters;
typedef std::map<std::string, std::string> channel_parameters;

/* Axon parameters */
parameters hillockParameters;
parameters nodeParameters;
parameters paranodeParameters;
parameters internodeParameters;
parameters globalParameters;
channel_parameters channel_params;
/* Simulation */
string inputFilename;
NTsize output = 0;
string outputFolder;
NTsize readN;
NTreal inpI;
NTreal inpISDV;
NTsize useVis;
NTsize sampN;
NTreal timeStep; // in msec
NTsize numIterations;
NTreal numTrials;
NTsize emulateMS;
NTsize emulateMSFactor;

/**
 * This function reads a section of the config file (a section in initiated by a [tag],
 * and lasts until the next [tag]) and stores parameters common to all sections of the fibre
 * (such as channel densities)in the param object supplied.
 * @param oCfg The file parser which will be used to read parameters
 * @param params The parameters structure in which the new parameters will be saved.
 * @param section Which section of the config file to read. (ex. node)
 */
void readCommonConfig(NT_config_file_parser_o oCfg, parameters& params,
		string section) {
	params["numComp"] = oCfg.Value(section, "numComp"); /* per Node ! */
	params["length"] = oCfg.Value(section, "length"); /* micron */
	params["gLeak"] = oCfg.Value(section, "GLeak");/* mSiemens/cm^2 */
	params["ra"] = oCfg.Value(section, "Ra"); /* Ohm cm */
	params["cm"] = oCfg.Value(section, "Cm"); /* muFarad/cm^2 */

	channel_params[section + "SodiumModel"] = (string) oCfg.Value(section,
			"chNaModel");
	params["sodiumAlg"] = oCfg.Value(section, "chNaAlg");
	params["sodiumDensity"] = oCfg.Value(section, "chNaDen"); //= 60; // per mu^2
	params["sodiumConductance"] = oCfg.Value(section, "chNaCond");
	params["sodiumReversalPotential"] = oCfg.Value(section, "chNaRevPot");

	channel_params[section + "PotassiumModel"] = (string) oCfg.Value(section,
			"chKModel");
	params["potassiumAlg"] = oCfg.Value(section, "chKAlg");
	params["potassiumDensity"] = oCfg.Value(section, "chKDen"); //= 60; // per mu^2
	params["potassiumConductance"] = oCfg.Value(section, "chKCond");
	params["potassiumReversalPotential"] = oCfg.Value(section, "chKRevPot");
}

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 */
void readConfig(string fileName) {
	// Remember that data file should have more lines than Num iterations.
	NT_config_file_parser_o oCfg(fileName);
	/* Global */
	globalParameters["temperature"] = oCfg.Value("global", "temperature"); //= 37; /* in Celsius */
	globalParameters["diameter"] = oCfg.Value("global", "diameter"); // = 1; /* in muMeter */
	globalParameters["swComputeELeak"] = oCfg.Value("global", "computeELeak");
	globalParameters["eLeak"] = oCfg.Value("global", "eLeak"); /* in mV */
	globalParameters["vBase"] = oCfg.Value("global", "vBase"); /* in mV */
	globalParameters["hillock"] = oCfg.Value("global", "hillock"); /* in mV */
	globalParameters["node"] = oCfg.Value("global", "node"); /* in mV */
	globalParameters["paranode"] = oCfg.Value("global", "paranode"); /* in mV */
	globalParameters["internode"] = oCfg.Value("global", "internode"); /* in mV */

	/*Hillokc*/
	if (globalParameters["hillock"])
		readCommonConfig(oCfg, hillockParameters, "hillock");

	/* Nodes */
	if (globalParameters["node"]) {
		nodeParameters["num"] = oCfg.Value("node", "numNd"); /* start with node at proximal end */
		readCommonConfig(oCfg, nodeParameters, "node");
	}

	/* Paranodes */
	if (globalParameters["paranode"])
		readCommonConfig(oCfg, paranodeParameters, "paranode");

	/* Internodes */
	if (globalParameters["internode"])
		readCommonConfig(oCfg, internodeParameters, "internode");

	/* Simulation */
	inputFilename = (string) oCfg.Value("simulation", "inputFile");
	outputFolder = (string) oCfg.Value("simulation", "outputFolder");
	if (outputFolder.compare("none") != 0) {
		output = 1;
	}
	readN = oCfg.Value("simulation", "readN");
	inpI = oCfg.Value("simulation", "inpI");
	inpISDV = oCfg.Value("simulation", "inpISDV");
	useVis = oCfg.Value("simulation", "useVis");
	sampN = oCfg.Value("simulation", "sampN");
	timeStep = oCfg.Value("simulation", "timeStep"); // in msec
	numIterations = oCfg.Value("simulation", "numIter");
	numTrials = oCfg.Value("simulation", "numTrials");
	emulateMS = oCfg.Value("simulation", "emulateMS");
	emulateMSFactor = oCfg.Value("simulation", "emulateMSFactor");
}

/**
 * Formats and prints current parameters into the ofstream given as input.
 * @param out
 */
void printConfig(ofstream& out) {
	for (parameters::iterator globalIter = globalParameters.begin(); globalIter
			!= globalParameters.end(); ++globalIter) {
		out << "global_" << globalIter->first << " = " << globalIter->second
				<< endl;
	}

	for (parameters::iterator nodeIter = nodeParameters.begin(); nodeIter
			!= nodeParameters.end(); ++nodeIter) {
		out << "node_" << nodeIter->first << " = " << nodeIter->second << endl;
	}

	for (parameters::iterator paranodeIter = paranodeParameters.begin(); paranodeIter
			!= paranodeParameters.end(); ++paranodeIter) {
		out << "paranode_" << paranodeIter->first << " = "
				<< paranodeIter->second << endl;
	}

	for (parameters::iterator internodeIter = internodeParameters.begin(); internodeIter
			!= internodeParameters.end(); ++internodeIter) {
		out << "internode_" << internodeIter->first << " = "
				<< internodeIter->second << endl;
	}

	out << "simulation_samplerate = " << sampN << endl;
	out << "simulation_timestep_inms = " << timeStep << endl;
	out << "simulation_number_of_iterations = " << numIterations << endl;
	out << "simulation_duration = " << numIterations * timeStep << endl;
	out << "simulation_trials = " << numTrials << endl;
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
	strftime(dateString, 80, "%b%d_%H%M%S", timeinfo);
	ss << outputFolder << "/" << dateString;
	string folderName;
	ss >> folderName;

	boost::filesystem::create_directories(folderName);

	return folderName;
}

/**
 * Opens a new file in write mode.
 * @param output Folder The folder in which to create the new file.
 * @param prefix File name prefix
 * @param outStream Will contain an ofstream pointing to the newly created file.
 */
void openOutputFile(string outputFolder, string prefix, ofstream& outStream,
		string extension = ".txt") {
	/* open files */
	stringstream ss(stringstream::in | stringstream::out);
	ss << outputFolder << "/" << prefix << extension;
	string filname;
	ss >> filename;

	outStream.open(filename.c_str(), ios::binary);

	if (outStream.fail()) {
		cerr << "Could not open output file " << prefix << endl;
		exit(1);
	}
}

/**
 * Creates a compartment using the parameters supplied in the parameters structs supplied.
 * @param globalParameters Used for the temperature, the diameter and leakage.
 * @param compartmentParameters Used for channel parameters, as well as capacity and leakage conductance.
 * @return The constructed compartment.
 */
NTBP_custom_cylindrical_compartment_o* createCompartment(
		parameters globalParameters, parameters compartmentParameters,
		string sodiumModel, string potassiumModel) {

	NTBP_custom_cylindrical_compartment_o *tmpPtr =
			new NTBP_custom_cylindrical_compartment_o(
					compartmentParameters["length"] /* muMeter */,
					globalParameters["diameter"] /* muMeter */,
					compartmentParameters["cm"]/*muFarad/cm^2 */,
					compartmentParameters["ra"] /* ohm cm */,
					globalParameters["temperature"]);

	/* Leak current is number 0 */
	tmpPtr->AttachCurrent(
			new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
					compartmentParameters["gLeak"], globalParameters["eLeak"]),
			NTBP_LEAK);

	/* Channel current is number 1 */
	if (compartmentParameters["sodiumDensity"] > 0) {
		NTreal indSodiumDensity = NTBP_corrected_channel_density(
				compartmentParameters["sodiumDensity"], tmpPtr->_area());
		NTBP_file_based_multi_current_o
				* na_current =
						new NTBP_file_based_multi_current_o(
								tmpPtr->_area(),
								indSodiumDensity /* mum^-2 */,
								compartmentParameters["sodiumConductance"]
										* 1e-9 /* pS */,
								globalParameters["vBase"] /* mV */,
								compartmentParameters["sodiumReversalPotential"] /* mV */,
								timeStep,
								globalParameters["temperature"] /* C */,
								sodiumModel);
		na_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		tmpPtr->AttachCurrent(na_current, NTBP_IONIC);

	} else
		tmpPtr->AttachCurrent(
				new NTBP_hh_sga_leak_current_o(tmpPtr->_area(), 0, 0),
				NTBP_LEAK);

	if (compartmentParameters["potassiumDensity"] > 0) {
		NTreal indPotassiumDensity = NTBP_corrected_channel_density(
				compartmentParameters["potassiumDensity"], tmpPtr->_area());
		NTBP_file_based_multi_current_o
				* k_current =
						new NTBP_file_based_multi_current_o(
								tmpPtr->_area(),
								indPotassiumDensity /* mum^-2 */,
								compartmentParameters["potassiumConductance"]
										* 1e-9 /* pS */,
								globalParameters["vBase"] /* mV */,
								compartmentParameters["potassiumReversalPotential"] /* mV */,
								timeStep,
								globalParameters["temperature"] /* C */,
								potassiumModel);
		k_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		tmpPtr->AttachCurrent(k_current, NTBP_IONIC);
	} else
		tmpPtr->AttachCurrent(
				new NTBP_hh_sga_leak_current_o(tmpPtr->_area(), 0, 0),
				NTBP_LEAK);
	return tmpPtr;
}

/**
 * Let's go!
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
	/* Read and set parameters */
	string fileName = argv[1];
	//return colbert(argc, argv);
	readConfig(fileName);
	string timedOutputFolder;

	ofstream TimeFile, PotentialPerCompartmentFile, PotentialPerUnitLengthFile,
			PotassiumFile, SodiumFile, TypePerCompartmentFile,
			TypePerUnitLengthFile, ConfigUsedFile;

	if (output) {
		timedOutputFolder = createOutputFolder(outputFolder);
		openOutputFile(timedOutputFolder, "Time", TimeFile);
		openOutputFile(timedOutputFolder, "PotentialPerCompartment",
				PotentialPerCompartmentFile);
		openOutputFile(timedOutputFolder, "PotentialPerUnitLength",
				PotentialPerUnitLengthFile);
		openOutputFile(timedOutputFolder, "Potassium", PotassiumFile);
		openOutputFile(timedOutputFolder, "Sodium", SodiumFile);
		openOutputFile(timedOutputFolder, "TypePerCompartment",
				TypePerCompartmentFile);
		openOutputFile(timedOutputFolder, "TypePerUnitLength",
				TypePerUnitLengthFile);
		openOutputFile(timedOutputFolder, "ConfigUsed", ConfigUsedFile, ".m");
		printConfig(ConfigUsedFile);
		TimeFile << "% in ms" << endl;
	}

	if (0 == globalParameters["swComputeELeak"]) {
		cout << "Eleak set to " << globalParameters["eLeak"] << " mV." << endl;
	} else {
		cout << "Eleak not set. Going to compute eLeak." << endl;
		/* COMPUTE Eleak by simulating and solving for the current balance equation after
		 * 50ms of simulated time at the NODE including the PARANODAL K channels*/
		parameters temp = nodeParameters;
		temp["potassiumModel"] = paranodeParameters["potassiumModel"];
		temp["potassiumAlg"] = paranodeParameters["potassiumAlg"];
		temp["potassiumDensity"] = paranodeParameters["potassiumDensity"];
		temp["potassiumConductance"]
				= paranodeParameters["potassiumConductance"];
		temp["potassiumQ10"] = paranodeParameters["potassiumQ10"];
		temp["potassiumReversalPotential"]
				= paranodeParameters["potassiumReversalPotential"];

		temp["gLeak"] = 0;

		/* Create a cylindrical membrane compartment */
		NTBP_custom_cylindrical_compartment_o* compartment = createCompartment(
				globalParameters, temp, channel_params["nodeSodiumModel"],
				channel_params["nodePotassiumModel"]);
		NTreal areaPerCompartment = compartment->_area();

		NTBP_membrane_current_o* tmpLeakPtr = new NTBP_hh_sga_leak_current_o(
				areaPerCompartment, nodeParameters["gLeak"],
				globalParameters["eLeak"]);

		float naCurrent = 0;
		float kCurrent = 0;

		/* ***  Determine leak reversal potential by simulating 50 ms *** */
		NTsize lt = 0;
		NTreal tmpEleak = 0;
		for (lt = 0; lt < 100.0 / timeStep; lt++) {
			compartment->Step(0);
			if (lt % 100 == 0) {
				naCurrent = compartment->AttachedReversalPotential(2)
						* (compartment->AttachedConductance(2));
				kCurrent = compartment->AttachedReversalPotential(3)
						* (compartment->AttachedConductance(3));

				tmpEleak = -(naCurrent + kCurrent) / tmpLeakPtr->_conductance();
				cerr << "I_Na=" << naCurrent << " I_K=" << kCurrent
						<< " E_Leak=" << tmpEleak << endl;
			}
		}

		naCurrent = compartment->AttachedReversalPotential(2)
				* (compartment->AttachedConductance(2));
		kCurrent = compartment->AttachedReversalPotential(3)
				* (compartment->AttachedConductance(3));
		globalParameters["eLeak"] = -(naCurrent + kCurrent)
				/ tmpLeakPtr->_conductance();
		swComputeELeak = false;
		cout << "Eleak computed as " << globalParameters["eLeak"] << " mV"
				<< endl;
		cerr << "Eleak computed as " << globalParameters["eLeak"] << " mV"
				<< endl;
		if ((globalParameters["eLeak"]
				> compartment->AttachedReversalPotential(2))
				|| (globalParameters["eLeak"]
						< compartment->AttachedReversalPotential(3))) {
			cout << "Eleak might be out of biological plausible range." << endl;
			cout << "Typically E_Na="
					<< compartment->AttachedReversalPotential(2) << " < Eleak="
					<< globalParameters["eLeak"] << " < E_K="
					<< compartment->AttachedReversalPotential(3) << endl;
			cout
					<< "Does a stable resting potential exist at all? Check by increasing iterations of current balance equation."
					<< endl;
			cerr << "Eleak might be out of biological plausible range." << endl;
			cerr << "Typically E_Na="
					<< compartment->AttachedReversalPotential(2) << " < Eleak="
					<< globalParameters["eLeak"] << " < E_K="
					<< compartment->AttachedReversalPotential(3) << endl;
			cerr
					<< "Does a stable resting potential exist at all? Check by increasing iterations of current balance equation."
					<< endl;
		}
	}

	NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur!
	NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !

	// Read input file only once. Store its content in memory.
	ifstream dataFile(inputFilename.c_str(), ios::binary);
	if (dataFile.fail()) {
		cerr << "Could not open input file " << inputFilename.c_str() << endl;
		exit(1);
	}
	int count = 1000000;
	vector<float> inputData(count);
	int index = 0;

	while (dataFile.good()) {
		if (index < count) {
			char tmp[100];
			dataFile.getline(tmp, 100);
			sscanf(tmp, "%f", &inputData[index]);
			index++;
		} else {
			count += 1000000;
			inputData.resize(count);
		}
	}
	dataFile.close();
	NTsize compartmentCounter;
	cout << "Assembling neuron..." << endl;
	/* *** Trials loop *** */
	for (NTsize lTrials = 0; lTrials < numTrials; lTrials++) {

		/* Model setup */
		NTBP_membrane_compartment_sequence_o oModel;
		oModel.UpdateTimeStep(timeStep /* mSec */);
		oModel.StepNTBP();
		NTreal timeInMS = 0;

		compartmentCounter = 1;
		/* *** MODEL CREATION LOOP *** */

		// Generate an axon hillock
		if (globalParameters["hillock"]) {
			for (NTsize lcomp = 0; lcomp < hillockParameters["numComp"]; lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 0" << endl;
				for (int i = 0; i < hillockParameters["length"]; i++)
					TypePerUnitLengthFile << "0" << endl;
				cerr << "Node compartment " << endl;
				oModel.PushBack(
						createCompartment(globalParameters, hillockParameters,
								channel_params["hillockSodiumModel"],
								channel_params["hillockPotassiumModel"]));
			}
		}

		/* Create a Node, followed by Paranode, Internode, Paranode */
		for (NTsize lnd = 0; lnd < nodeParameters["num"]; lnd++) {
			/* Create a Node compartment */
			for (NTsize lcomp = 0; lcomp < nodeParameters["numComp"]; lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 1" << endl;
				for (int i = 0; i < nodeParameters["length"]; i++)
					TypePerUnitLengthFile << "1" << endl;
				cerr << "Node compartment " << endl;
				oModel.PushBack(
						createCompartment(globalParameters, nodeParameters,
								channel_params["nodeSodiumModel"],
								channel_params["nodePotassiumModel"]));
			}

			/* Create a Paranode compartment */
			if (globalParameters["paranode"]) {
				for (NTsize lcomp = 0; lcomp < paranodeParameters["numComp"]; lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 2"
							<< endl;
					for (int i = 0; i < paranodeParameters["length"]; i++)
						TypePerUnitLengthFile << "2" << endl;
					cerr << "Paranode compartment " << endl;
					oModel.PushBack(
							createCompartment(globalParameters,
									paranodeParameters,
									channel_params["paranodeSodiumModel"],
									channel_params["paranodePotassiumModel"]));
				}
			}

			if ((nodeParameters["num"] - 1) == lnd) {
				break;
			}

			/* Create an Internode compartment */
			if (globalParameters["internode"]) {
				for (NTsize lcomp = 0; lcomp < internodeParameters["numComp"]; lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 3"
							<< endl;
					for (int i = 0; i < internodeParameters["length"]; i++)
						TypePerUnitLengthFile << "3" << endl;
					cerr << "Internode compartment " << endl;
					oModel.PushBack(
							createCompartment(globalParameters,
									internodeParameters,
									channel_params["internodeSodiumModel"],
									channel_params["internodePotassiumModel"]));
				}
			}

			/* Create a Paranode compartment */
			if (globalParameters["paranode"]) {
				for (NTsize lcomp = 0; lcomp < paranodeParameters["numComp"]; lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 2"
							<< endl;
					for (int i = 0; i < paranodeParameters["length"]; i++)
						TypePerUnitLengthFile << "2" << endl;
					cerr << "Paranode compartment " << endl;
					oModel.PushBack(
							createCompartment(globalParameters,
									paranodeParameters,
									channel_params["paranodeSodiumModel"],
									channel_params["paranodePotassiumModel"]));
				}
			}

		}

		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = hillockParameters["numComp"]
				+ (nodeParameters["num"] - 1) * (nodeParameters["numComp"]
						+ paranodeParameters["numComp"]
						+ internodeParameters["numComp"]
						+ paranodeParameters["numComp"])
				+ nodeParameters["numComp"] + paranodeParameters["numComp"];
		cerr << "Total number of compartments(computed)" << numCompartments
				<< endl;
		cerr << "Total number of compartments(assembled)" << compartmentCounter
				<< endl;
		cerr << "Total number of compartments(in oModel)"
				<< oModel._numCompartments() << endl;
		vector<NTreal> leakCurrVec(numCompartments);
		vector<NTreal> naCurrVec(numCompartments);
		vector<NTreal> kCurrVec(numCompartments);

		/* Graphics init */
		NT3D_plot2d_vec_vp_o plotXY(numCompartments);
		NT3D_plot2d_vec_vp_o plotChanNa(numCompartments);
		NT3D_plot2d_vec_vp_o plotChanK(numCompartments);

		if (useVis > 0) {
			NT3D_glx_drv_o* drvVP = new NT3D_glx_drv_o(500, 200);
			drvVP->SetWindowTitle("Voltage-Compartment-Plot");
			if (NT_FAIL == plotXY.Connect(drvVP))
				exit(1);
			plotXY.AutoRange(false);
			plotXY.SetXRange(0, numCompartments);
			plotXY.SetYRange(-100, 200);
		}

		if (useVis > 0) {
			NT3D_glx_drv_o* drv2VP = new NT3D_glx_drv_o(500, 200);
			drv2VP->SetWindowTitle("NaOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanNa.Connect(drv2VP))
				exit(1);
			plotChanNa.SetXRange(0, numCompartments);
			plotChanNa.SetYRange(0, 100);
		}

		if (useVis > 0) {
			NT3D_glx_drv_o* drv3VP = new NT3D_glx_drv_o(500, 200);
			drv3VP->SetWindowTitle("KOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanK.Connect(drv3VP))
				exit(1);
			plotChanK.SetXRange(0, numCompartments);
			plotChanK.SetYRange(0, 100);
		}

		/* *** SIMULATION ITERATION LOOP *** */

		/* ***********************  Main loop **************************** */
		cerr << "MainLoop started" << endl;
		float timeVar = 0;
		NTreal inpCurrent = 0.0;

		NT_uniform_rnd_dist_o uniformRnd;

		vector<NTreal> voltVec;
		vector<NTreal>::iterator maxVoltPos;
		vector<NTreal>::iterator maxVoltOldPos;
		timeInMS = 0;
		int dataRead = 0;
		for (NTsize lt = 0; lt < numIterations; lt++) {
			timeInMS += oModel._timeStep();
			timeVar = timeInMS;
			/* the "sampling ratio" used for "measurement" to disk */
			if (output && lt % sampN == 0) {
				oModel.WriteMembranePotentialASCII(PotentialPerUnitLengthFile,
						true);
				oModel.WriteMembranePotentialASCII(PotentialPerCompartmentFile,
						false);
				oModel.WriteCurrentAscii(SodiumFile, 2); //Na
				oModel.WriteCurrentAscii(PotassiumFile, 3); // K
				TimeFile << timeVar << endl;
			}

			if (useVis > 0) {
				if (lt % useVis == 0) {
					voltVec.clear();
					for (NTsize ll = 0; ll < oModel._numCompartments(); ll++) {
						voltVec.push_back(oModel.compartmentVec[ll]->_vM());
					}
					for (NTsize lc = 0; lc < numCompartments; lc++) {
						if (NTisnan(voltVec[lc])) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is NaN." << endl;
							exit(1);
						} else if (voltVec[lc] > 200.0 /* mV */) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is " << voltVec[lc] << "." << endl;
							exit(1);
						}
					}
					//cerr << "t=" << timeInMS << " mSec :" << voltVec[1] << "\t"
					//		<< voltVec[10] << endl;

					plotXY.SetData(voltVec);
					plotXY.Draw();
					plotChanNa.SetData(oModel.OpenChannelsRatio(2));
					//plotChanNa.SetData(oModel.NumChannelsInState(2,1));
					plotChanNa.Draw();
					plotChanK.SetData(oModel.OpenChannelsRatio(3));
					//plotChanK.SetData(oModel.NumChannelsInState(3,1));
					plotChanK.Draw();
				}
			}

			if (lt % readN == 0) {
				inpCurrent = (inputData[dataRead] * inpISDV) + inpI;
				dataRead++;
			}
			oModel.InjectCurrent(inpCurrent, 1);
			//oModel.InjectCurrent(, 1);

			oModel.Step();
		}
	} // lTrials
	cerr << "Simulation completed." << endl;
	return 0;
}

