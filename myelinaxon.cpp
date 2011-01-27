/**\file myelinaxonsimulator-v1.cpp - myelinated axon simultor version 1
 * based on axonsimulator-v3.cpp - axon simulator version 3
 * by Ahmed Aldo Faisal &copy; created 25.9.2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
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
#include <ctime>

#include <ntbp_membrane_compartment_sequence_obj.h>
#include <ntbp_sga_cylindrical_compartment_obj.h>
#include <ntbp_custom_cylindrical_compartment_obj.h>

#include <ntbp_auxfunc.h>

#include <ntbp_multi_sodium_current_obj.h>
#include <ntbp_colbert_axonal_sodium_multi_current_obj.h>
#include <ntbp_hranvier_sodium_multi_current_obj.h>
#include <ntbp_patlak_sodium_multi_current_obj.h>

#include <ntbp_multi_potassium_current_obj.h>
#include <ntbp_colbert_axonal_potassium_multi_current_obj.h>
#include <ntbp_hranvier_potassium_multi_current_obj.h>
#include <ntbp_potassium_multi_current_obj.h>

#include <nt_error_obj.h>
#include <nt_vector_obj.h>

#include <nt_histogram_obj.h>
#include <nt_sequential_statistics_obj.h>
#include <tnt.h>
#include <cmat.h>
#include <nt_uniform_rnd_dist_obj.h>
#include <nt_binomial_rnd_dist_obj.h>
#include <nt_gaussian_rnd_dist_obj.h>
#include <nt3d_plot2d_vec_vp_obj.h>
#include <nt3d_glx_drv_obj.h>

#include <nt_config_file_parser_obj.h>

using namespace std;
using namespace TNT;

string filename;

/* Global */
bool swComputeELeak;

typedef std::map<std::string, NTreal> parameters;
parameters nodeParameters;
parameters paranodeParameters;
parameters internodeParameters;
parameters globalParameters;

/* Simulation */
string inputFilename;
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
NTsize numAxonHillockNodeCompartments;

void readConfig(string fileName) {
	// Remember that data file should have more lines than Num iterations.
	NT_config_file_parser_o oCfg(fileName);
	/* Global */
	globalParameters["temperature"] = oCfg.Value("global", "temperature"); //= 37; /* in Celsius */
	globalParameters["diameter"] = oCfg.Value("global", "diameter"); // = 1; /* in muMeter */
	globalParameters["swComputeELeak"] = oCfg.Value("global", "computeELeak");
	globalParameters["eLeak"] = oCfg.Value("global", "eLeak"); /* in mV */

	/* Nodes */
	nodeParameters["num"] = oCfg.Value("node", "numNd"); /* start with node at proximal end */
	nodeParameters["numComp"] = oCfg.Value("node", "numComp"); /* per Node ! */
	nodeParameters["length"] = oCfg.Value("node", "length"); /* micron */
	nodeParameters["gLeak"] = oCfg.Value("node", "GLeak");/* mSiemens/cm^2 */
	nodeParameters["ra"] = oCfg.Value("node", "Ra"); /* Ohm cm */
	nodeParameters["cm"] = oCfg.Value("node", "Cm"); /* muFarad/cm^2 */

	nodeParameters["sodiumModel"] = oCfg.Value("node", "chNaModel");
	nodeParameters["sodiumAlg"] = oCfg.Value("node", "chNaAlg");
	nodeParameters["sodiumDensity"] = oCfg.Value("node", "chNaDen"); //= 60; // per mu^2
	nodeParameters["sodiumConductance"] = oCfg.Value("node", "chNaCond");
	nodeParameters["sodiumQ10m"] = oCfg.Value("node", "chNaQ10m");
	nodeParameters["sodiumQ10h"] = oCfg.Value("node", "chNaQ10h");
	nodeParameters["sodiumReversalPotential"]
			= oCfg.Value("node", "chNaRevPot");

	nodeParameters["potassiumModel"] = oCfg.Value("node", "chKModel");
	nodeParameters["potassiumAlg"] = oCfg.Value("node", "chKAlg");
	nodeParameters["potassiumDensity"] = oCfg.Value("node", "chKDen"); //= 60; // per mu^2
	nodeParameters["potassiumConductance"] = oCfg.Value("node", "chKCond");
	nodeParameters["potassiumQ10"] = oCfg.Value("node", "chKQ10");
	nodeParameters["potassiumReversalPotential"] = oCfg.Value("node",
			"chKRevPot");

	/* Paranodes */
	paranodeParameters["numComp"] = oCfg.Value("paranode", "numComp"); /* per Node ! */
	paranodeParameters["length"] = oCfg.Value("paranode", "length"); /* micron */
	paranodeParameters["gLeak"] = oCfg.Value("paranode", "GLeak");/* mSiemens/cm^2 */
	paranodeParameters["ra"] = oCfg.Value("paranode", "Ra"); /* Ohm cm */
	paranodeParameters["cm"] = oCfg.Value("paranode", "Cm"); /* muFarad/cm^2 */

	paranodeParameters["sodiumModel"] = oCfg.Value("paranode", "chNaModel");
	paranodeParameters["sodiumAlg"] = oCfg.Value("paranode", "chNaAlg");
	paranodeParameters["sodiumDensity"] = oCfg.Value("paranode", "chNaDen"); //= 60; // per mu^2
	paranodeParameters["sodiumConductance"]
			= oCfg.Value("paranode", "chNaCond");
	paranodeParameters["sodiumQ10m"] = oCfg.Value("paranode", "chNaQ10m");
	paranodeParameters["sodiumQ10h"] = oCfg.Value("paranode", "chNaQ10h");
	paranodeParameters["sodiumReversalPotential"] = oCfg.Value("paranode",
			"chNaRevPot");

	paranodeParameters["potassiumModel"] = oCfg.Value("paranode", "chKModel");
	paranodeParameters["potassiumAlg"] = oCfg.Value("paranode", "chKAlg");
	paranodeParameters["potassiumDensity"] = oCfg.Value("paranode", "chKDen"); //= 60; // per mu^2
	paranodeParameters["potassiumConductance"] = oCfg.Value("paranode",
			"chKCond");
	paranodeParameters["potassiumQ10"] = oCfg.Value("paranode", "chKQ10");
	paranodeParameters["potassiumReversalPotential"] = oCfg.Value("paranode",
			"chKRevPot");

	/* Internodes */
	internodeParameters["numComp"] = oCfg.Value("internode", "numComp"); /* per Node ! */
	internodeParameters["length"] = oCfg.Value("internode", "length"); /* micron */
	internodeParameters["gLeak"] = oCfg.Value("internode", "GLeak");/* mSiemens/cm^2 */
	internodeParameters["ra"] = oCfg.Value("internode", "Ra"); /* Ohm cm */
	internodeParameters["cm"] = oCfg.Value("internode", "Cm"); /* muFarad/cm^2 */

	internodeParameters["sodiumModel"] = oCfg.Value("internode", "chNaModel");
	internodeParameters["sodiumAlg"] = oCfg.Value("internode", "chNaAlg");
	internodeParameters["sodiumDensity"] = oCfg.Value("internode", "chNaDen"); //= 60; // per mu^2
	internodeParameters["sodiumConductance"] = oCfg.Value("internode",
			"chNaCond");
	internodeParameters["sodiumQ10m"] = oCfg.Value("internode", "chNaQ10m");
	internodeParameters["sodiumQ10h"] = oCfg.Value("internode", "chNaQ10h");
	internodeParameters["sodiumReversalPotential"] = oCfg.Value("internode",
			"chNaRevPot");

	internodeParameters["potassiumModel"] = oCfg.Value("internode", "chKModel");
	internodeParameters["potassiumAlg"] = oCfg.Value("internode", "chKAlg");
	internodeParameters["potassiumDensity"] = oCfg.Value("internode", "chKDen"); //= 60; // per mu^2
	internodeParameters["potassiumConductance"] = oCfg.Value("internode",
			"chKCond");
	internodeParameters["potassiumQ10"] = oCfg.Value("internode", "chKQ10");
	internodeParameters["potassiumReversalPotential"] = oCfg.Value("internode",
			"chKRevPot");

	/* Simulation */
	inputFilename = (string) oCfg.Value("simulation", "inputFile");
	outputFolder = (string) oCfg.Value("simulation", "outputFolder");
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

void printConfig(ofstream& out) {
	out << ";#[Global]" << endl;

	for (parameters::iterator globalIter = globalParameters.begin(); globalIter
			!= globalParameters.end(); ++globalIter) {
		out << ";#" << globalIter->first << " = " << globalIter->second << endl;
	}

	out << endl << "#[Node]" << endl;
	for (parameters::iterator nodeIter = nodeParameters.begin(); nodeIter
			!= nodeParameters.end(); ++nodeIter) {
		out << ";#" << nodeIter->first << " = " << nodeIter->second << endl;
	}

	out << endl << "#[Paranode]" << endl;
	for (parameters::iterator paranodeIter = paranodeParameters.begin(); paranodeIter
			!= paranodeParameters.end(); ++paranodeIter) {
		out << ";#" << paranodeIter->first << " = " << paranodeIter->second
				<< endl;
	}

	out << endl << "#[Internode]" << endl;
	for (parameters::iterator internodeIter = internodeParameters.begin(); internodeIter
			!= internodeParameters.end(); ++internodeIter) {
		out << ";#" << internodeIter->first << " = " << internodeIter->second
				<< endl;
	}

	out << ";#[Simulation]" << endl;
	out << ";#Storing data in" << filename << " every " << sampN
			<< "th iteration" << endl;
	out << ";#Time step size in [mSec] " << timeStep << endl;
	out << ";#Num iterations [#] " << numIterations << endl;
	out << ";#Trial duration [ms] " << numIterations * timeStep << endl;
	out << ";#Number of repated stimulus trials [#] " << numTrials << endl;
}

void openOutputFile(string outputFolder, string prefix, ofstream& outStream) {
	/* open files */
	time_t rawtime;
	struct tm * timeinfo;
	stringstream ss(stringstream::in | stringstream::out);
	char dateString[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(dateString, 80, "%b%d_%H%M.txt", timeinfo);
	ss << outputFolder << "/" << prefix << "-" << dateString;
	string filname;
	ss >> filename;

	outStream.open(filename.c_str(), ios::binary);

	if (outStream.fail()) {
		cerr << "Could not open output file " << prefix << endl;
		exit(1);
	}
}

NTBP_custom_cylindrical_compartment_o* createCompartment(
		parameters globalParameters, parameters compartmentParameters) {

	NTBP_custom_cylindrical_compartment_o *tmpPtr =
			new NTBP_custom_cylindrical_compartment_o(
					compartmentParameters["length"] /* muMeter */,
					globalParameters["diameter"] /* muMeter */,
					compartmentParameters["cm"]/*muFarad/cm^2 */,
					compartmentParameters["ra"] /* ohm cm */);
	tmpPtr->Set_temperature(globalParameters["temperature"] /* in celsius */);

	/* Leak current is number 0 */
	tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
			compartmentParameters["gLeak"], globalParameters["eLeak"]),
			NTBP_LEAK);

	/* Channel current is number 1 */

	if (compartmentParameters["sodiumDensity"] > 0)
		tmpPtr->AttachCurrent(NTBP_create_na_channel_ptr(
				compartmentParameters["sodiumModel"],
				compartmentParameters["sodiumAlg"],
				compartmentParameters["sodiumDensity"] /* mum^-2 */,
				compartmentParameters["sodiumConductance"] /* pS */,
				compartmentParameters["sodiumQ10m"],
				compartmentParameters["sodiumQ10h"] /* q10 */,
				globalParameters["temperature"] /* C */,
				tmpPtr->_area() /* mum^2 */,
				compartmentParameters["sodiumReversalPotential"] /*mV*/),
				NTBP_IONIC);
	else
		tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
				0, 0), NTBP_LEAK);

	if (compartmentParameters["potassiumDensity"] > 0)
		tmpPtr->AttachCurrent(NTBP_create_k_channel_ptr(
				compartmentParameters["potassiumModel"],
				compartmentParameters["potassiumAlg"],
				compartmentParameters["potassiumDensity"] /* mum^-2 */,
				compartmentParameters["potassiumConductance"] /* pS */,
				compartmentParameters["potassiumQ10"] /* q10 */,
				globalParameters["temperature"] /* C */,
				tmpPtr->_area() /* mum^2 */,
				compartmentParameters["potassiumReversalPotential"] /*mV*/),
				NTBP_IONIC);
	else
		tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
				0, 0), NTBP_LEAK);
	return tmpPtr;
}

int main(int argc, char *argv[]) {

	/* Read and set parameters */
	readConfig(argv[1]);
	ofstream ATPFile;
	openOutputFile(outputFolder, "ATP", ATPFile);
	ofstream PotentialFile;
	openOutputFile(outputFolder, "Potential", PotentialFile);

	printConfig(ATPFile);
	printConfig(PotentialFile);

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
				globalParameters, temp);
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
				naCurrent = compartment->AttachedReversalPotential(1)
						* (compartment->AttachedConductance(1));
				kCurrent = compartment->AttachedReversalPotential(2)
						* (compartment->AttachedConductance(2));

				tmpEleak = -(naCurrent + kCurrent) / tmpLeakPtr->_conductance();
				cerr << "I_Na=" << naCurrent << " I_K=" << kCurrent
						<< " E_Leak=" << tmpEleak << endl;
			}
		}

		naCurrent = compartment->AttachedReversalPotential(1)
				* (compartment->AttachedConductance(1));
		kCurrent = compartment->AttachedReversalPotential(2)
				* (compartment->AttachedConductance(2));
		globalParameters["eLeak"] = -(naCurrent + kCurrent)
				/ tmpLeakPtr->_conductance();
		swComputeELeak = false;
		cout << "Eleak computed as " << globalParameters["eLeak"] << " mV"
				<< endl;
		cerr << "Eleak computed as " << globalParameters["eLeak"] << " mV"
				<< endl;
		if ((globalParameters["eLeak"]
				> compartment->AttachedReversalPotential(1))
				|| (globalParameters["eLeak"]
						< compartment->AttachedReversalPotential(2))) {
			cout << "Eleak might be out of biological plausible range." << endl;
			cout << "Typically E_Na="
					<< compartment->AttachedReversalPotential(1) << " < Eleak="
					<< globalParameters["eLeak"] << " < E_K="
					<< compartment->AttachedReversalPotential(2) << endl;
			cout
					<< "Does a stable resting potential exist at all? Check by increasing iterations of current balance equation."
					<< endl;
			cerr << "Eleak might be out of biological plausible range." << endl;
			cerr << "Typically E_Na="
					<< compartment->AttachedReversalPotential(1) << " < Eleak="
					<< globalParameters["eLeak"] << " < E_K="
					<< compartment->AttachedReversalPotential(2) << endl;
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

		for (NTsize lcomp = 0; lcomp < numAxonHillockNodeCompartments; lcomp++) {

			cout << compartmentCounter++ << "NODE (Axon Hillock)" << endl;
			cerr << "Node compartment " << endl;
			parameters hillockParameters = nodeParameters;
			hillockParameters["numComp"] = 10;
			oModel.PushBack(createCompartment(globalParameters,
					hillockParameters));
		}

		/* Create a Node, followed by Paranode, Internode, Paranode */
		for (NTsize lnd = 0; lnd < nodeParameters["num"]; lnd++) {
			/* Create a Node compartment */
			for (NTsize lcomp = 0; lcomp < nodeParameters["numComp"]; lcomp++) {
				cout << compartmentCounter++ << "NODE" << endl;
				cerr << "Node compartment " << endl;
				oModel.PushBack(createCompartment(globalParameters,
						nodeParameters));
			}

			/* Create a Paranode compartment */
			for (NTsize lcomp = 0; lcomp < paranodeParameters["numComp"]; lcomp++) {
				cout << compartmentCounter++ << "PARA" << endl;
				cerr << "Paranode compartment " << endl;
				oModel.PushBack(createCompartment(globalParameters,
						paranodeParameters));
			}

			if ((nodeParameters["num"] - 1) == lnd) {
				break;
			}

			/* Create an Internode compartment */
			for (NTsize lcomp = 0; lcomp < internodeParameters["numComp"]; lcomp++) {
				cout << compartmentCounter++ << "INTER" << endl;
				cerr << "Internode compartment " << endl;
				oModel.PushBack(createCompartment(globalParameters,
						internodeParameters));
			}

			/* Create a Paranode compartment */
			for (NTsize lcomp = 0; lcomp < paranodeParameters["numComp"]; lcomp++) {
				cout << compartmentCounter++ << "PARA" << endl;
				cerr << "Paranode compartment " << endl;

				oModel.PushBack(createCompartment(globalParameters,
						paranodeParameters));
			}

		}

		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = numAxonHillockNodeCompartments
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
		if (useVis > 0) {
			NT3D_glx_drv_o* drvVP = new NT3D_glx_drv_o(500, 200);
			drvVP->SetWindowTitle("Voltage-Compartment-Plot");
			if (NT_FAIL == plotXY.Connect(drvVP))
				exit(1);
			plotXY.AutoRange(false);
			plotXY.SetXRange(0, numCompartments);
			plotXY.SetYRange(-10, 120);
		}

		NT3D_plot2d_vec_vp_o plotChanNa(numCompartments);
		if (useVis > 0) {
			NT3D_glx_drv_o* drv2VP = new NT3D_glx_drv_o(500, 200);
			drv2VP->SetWindowTitle("NaOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanNa.Connect(drv2VP))
				exit(1);
			plotChanNa.SetXRange(0, numCompartments);
			plotChanNa.SetYRange(0, 6000);
		}

		NT3D_plot2d_vec_vp_o plotChanK(numCompartments);
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

			if (lt % sampN == 0) {
				/* the "sampling ratio" used for "measurement" to disk */
				//file.write(reinterpret_cast<char *> (&timeVar), sizeof(float));
				//file.write(reinterpret_cast<char *> (&inpCurrent), sizeof(float));
				oModel.WriteMembranePotentialASCII(PotentialFile, timeVar);
				//oModel.WriteCurrent(file, 2); // Na
				//oModel.WriteCurrent(file, 3); // K

				// Number of ATPs consumed is (SI units)
				// NaCurrent * dt * 6.24151e18 / 3
				oModel.WriteATP(ATPFile);
			}

			if (useVis > 0) {
				if (lt % useVis == 0) {
					voltVec = oModel._vVec();

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

					plotXY.SetData(oModel._vVec());
					plotXY.Draw();
					plotChanNa.SetData(oModel.OpenChannels(2));
					plotChanNa.Draw();
					plotChanK.SetData(oModel.OpenChannels(3));
					plotChanK.Draw();
				}
			}

			if (lt % readN == 0) {
				inpCurrent = (inputData[dataRead] * inpISDV) + inpI;
				dataRead++;
			}
			oModel.InjectCurrent(inpCurrent, 1);

			oModel.Step();
		}
	} // lTrials


	ATPFile.close();
	PotentialFile.close();
	cerr << "Simulation completed." << endl;
	return 0;
}

