/**\file myelinaxon.cpp - myelinated axon simultor version 2
 * based on myelinaxonsimulator-v1.cpp - axon simulator version 1
 * by Ahmed Aldo Faisal &copy; created 25.9.2001
 * by Mohammad Ali Neishabouri
 */
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

#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <boost/filesystem.hpp>

#include <ntbp_membrane_compartment_sequence_obj.h>
#include <ntbp_file_based_multi_current_obj.h>
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

const int EXIT_V_TOO_HIGH = 1;
const int EXIT_GRAPHIC_ERROR = 2;
const int EXIT_IO_ERROR = 3;

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
	for (parameters::iterator globalIter = globalParameters.begin();
			globalIter != globalParameters.end(); ++globalIter) {
		out << "global_" << globalIter->first << " = " << globalIter->second
				<< ";" << endl;
	}

	for (parameters::iterator nodeIter = nodeParameters.begin();
			nodeIter != nodeParameters.end(); ++nodeIter) {
		out << "node_" << nodeIter->first << " = " << nodeIter->second << ";"
				<< endl;
	}

	for (parameters::iterator paranodeIter = paranodeParameters.begin();
			paranodeIter != paranodeParameters.end(); ++paranodeIter) {
		out << "paranode_" << paranodeIter->first << " = "
				<< paranodeIter->second << ";" << endl;
	}

	for (parameters::iterator internodeIter = internodeParameters.begin();
			internodeIter != internodeParameters.end(); ++internodeIter) {
		out << "internode_" << internodeIter->first << " = "
				<< internodeIter->second << ";" << endl;
	}

	out << "simulation_samplerate = " << sampN << ";" << endl;
	out << "simulation_timestep_inms = " << timeStep << ";" << endl;
	out << "simulation_number_of_iterations = " << numIterations << ";" << endl;
	out << "simulation_duration = " << numIterations * timeStep << ";" << endl;
	out << "simulation_trials = " << numTrials << ";" << endl;
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
		exit(EXIT_IO_ERROR);
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
		NTBP_file_based_multi_current_o * na_current =
				new NTBP_file_based_multi_current_o(
						tmpPtr->_area(),
						indSodiumDensity /* mum^-2 */,
						compartmentParameters["sodiumConductance"]
								* 1e-9 /* pS */,
						globalParameters["vBase"] /* mV */,
						compartmentParameters["sodiumReversalPotential"] /* mV */,
						timeStep, globalParameters["temperature"] /* C */,
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
		NTBP_file_based_multi_current_o * k_current =
				new NTBP_file_based_multi_current_o(
						tmpPtr->_area(),
						indPotassiumDensity /* mum^-2 */,
						compartmentParameters["potassiumConductance"]
								* 1e-9 /* pS */,
						globalParameters["vBase"] /* mV */,
						compartmentParameters["potassiumReversalPotential"] /* mV */,
						timeStep, globalParameters["temperature"] /* C */,
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

	ofstream TimeFile, PotentialPerCompartmentFile, LengthPerCompartmentFile,
			PotassiumFile, SodiumFile, TypePerCompartmentFile, ConfigUsedFile,
			LeakFile;

	if (output) {
		timedOutputFolder = createOutputFolder(outputFolder);
		openOutputFile(timedOutputFolder, "Time", TimeFile);
		openOutputFile(timedOutputFolder, "PotentialPerCompartment",
				PotentialPerCompartmentFile);
		openOutputFile(timedOutputFolder, "Potassium", PotassiumFile);
		openOutputFile(timedOutputFolder, "Sodium", SodiumFile);
		openOutputFile(timedOutputFolder, "TypePerCompartment",
				TypePerCompartmentFile);
		openOutputFile(timedOutputFolder, "LengthPerCompartment",
				LengthPerCompartmentFile);
		openOutputFile(timedOutputFolder, "LeakPerCompartment", LeakFile,
				".txt");
		openOutputFile(timedOutputFolder, "ConfigUsed", ConfigUsedFile, ".m");
		printConfig(ConfigUsedFile);
		TimeFile << "% in ms" << endl;
	}

	cout << "Eleak set to " << globalParameters["eLeak"] << " mV." << endl;

	NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur!
	NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !

	// Read input file only once. Store its content in memory.
	ifstream dataFile(inputFilename.c_str(), ios::binary);
	if (dataFile.fail()) {
		cerr << "Could not open input file " << inputFilename.c_str() << endl;
		return (EXIT_IO_ERROR);
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
			for (NTsize lcomp = 0; lcomp < hillockParameters["numComp"];
					lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 0" << endl;
				LengthPerCompartmentFile << hillockParameters["length"] << endl;
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
				LengthPerCompartmentFile << nodeParameters["length"] << endl;
				oModel.PushBack(
						createCompartment(globalParameters, nodeParameters,
								channel_params["nodeSodiumModel"],
								channel_params["nodePotassiumModel"]));
			}

			/* Create a Paranode compartment */
			if (globalParameters["paranode"]) {
				for (NTsize lcomp = 0; lcomp < paranodeParameters["numComp"];
						lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 2"
							<< endl;
					LengthPerCompartmentFile << paranodeParameters["length"]
							<< endl;
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
				for (NTsize lcomp = 0; lcomp < internodeParameters["numComp"];
						lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 3"
							<< endl;
					LengthPerCompartmentFile << internodeParameters["length"]
							<< endl;
					oModel.PushBack(
							createCompartment(globalParameters,
									internodeParameters,
									channel_params["internodeSodiumModel"],
									channel_params["internodePotassiumModel"]));
				}
			}

			/* Create a Paranode compartment */
			if (globalParameters["paranode"]) {
				for (NTsize lcomp = 0; lcomp < paranodeParameters["numComp"];
						lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 2"
							<< endl;
					LengthPerCompartmentFile << paranodeParameters["length"]
							<< endl;
					oModel.PushBack(
							createCompartment(globalParameters,
									paranodeParameters,
									channel_params["paranodeSodiumModel"],
									channel_params["paranodePotassiumModel"]));
				}
			}

		}

		TypePerCompartmentFile.close();
		LengthPerCompartmentFile.close();

		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = hillockParameters["numComp"]
				+ (nodeParameters["num"] - 1)
						* (nodeParameters["numComp"]
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
			NT3D_glx_drv_o* drvVP = new NT3D_glx_drv_o(1000, 120);
			drvVP->SetWindowTitle("Voltage-Compartment-Plot");
			if (NT_FAIL == plotXY.Connect(drvVP))
				return (EXIT_GRAPHIC_ERROR);
			plotXY.AutoRange(false);
			plotXY.SetXRange(0, numCompartments);
			plotXY.SetYRange(-100, 200);

			NT3D_glx_drv_o* drv2VP = new NT3D_glx_drv_o(1000, 100);
			drv2VP->SetWindowTitle("NaOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanNa.Connect(drv2VP))
				return (EXIT_GRAPHIC_ERROR);
			plotChanNa.SetXRange(0, numCompartments);
			plotChanNa.SetYRange(0, 100);

			NT3D_glx_drv_o* drv3VP = new NT3D_glx_drv_o(1000, 100);
			drv3VP->SetWindowTitle("KOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanK.Connect(drv3VP))
				return (EXIT_GRAPHIC_ERROR);
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
				oModel.WriteMembranePotentialASCII(PotentialPerCompartmentFile,
						false);
				oModel.WriteCurrentAscii(LeakFile, 1); //Leak
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
							return (EXIT_V_TOO_HIGH);
						} else if (voltVec[lc] > 200.0 /* mV */) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is " << voltVec[lc] << "." << endl;
							return (EXIT_V_TOO_HIGH);
						}
					}

					plotXY.SetData(voltVec);
					plotXY.Draw();
					plotChanNa.SetData(oModel.OpenChannelsRatio(2));
					//plotChanNa.SetData(oModel.NumChannelsInState(2,5));
					plotChanNa.Draw();
					plotChanK.SetData(oModel.OpenChannelsRatio(3));
					//plotChanK.SetData(oModel.NumChannelsInState(3,5));
					plotChanK.Draw();
				}
			}

			if (lt % readN == 0) {
				inpCurrent = (inputData[dataRead] * inpISDV) + inpI;
				dataRead++;
				cout << lt << "\t" << inpCurrent << endl;
			}
			oModel.InjectCurrent(inpCurrent, 1);

			oModel.Step();
		}
	} // lTrials
	cerr << "Simulation completed." << endl;
	return 0;
}
