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

/* $Id: myelinaxonsimulator-v1.cpp,v 1.1 2005/04/25 15:13:17 face Exp $
 * $Log: myelinaxonsimulator-v1.cpp,v $
 *
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

#define CHANNEL_TYPE_SODIUM 0
#define CHANNEL_TYPE_POTASSIUM 1

using namespace std;
using namespace TNT;

string filename;

/* Global */
NTreal temperature; /* in Celsius */
NTreal diameter; /* in muMeter */
NTreal eLeak;
bool swComputeELeak;

/* Nodes */
NTsize numNd; /* start with node at proximal end */
NTsize numNdComp; /* per Node ! */
NTreal lengthNd; /* micron */
NTreal ndGLeak;/* mSiemens/cm^2 */
NTreal ndRa; /* Ohm cm */
NTreal ndCm; /* muFarad/cm^2 */
NTsize ndSodiumModel;
NTsize ndSodiumAlg;
NTreal ndSodiumDensity; // per mu^2
NTreal ndSodiumConductance;
NTreal ndSodiumQ10m;
NTreal ndSodiumQ10h;
NTreal noSodiumReversalPotential;

/* Paranodes */
NTsize numPndComp;
NTreal lengthPnd; /* micron */
NTreal pndGLeak; /* mSiemens/cm^2 */
NTreal pndRa; /* Ohm cm */
NTreal pndCm; /* muFarad/cm^2 */
NTsize pndPotassiumModel;
NTsize pndPotassiumAlg;
NTreal pndPotassiumDensity; //  per mu^2
NTreal pndPotassiumConductance;
NTreal pndPotassiumQ10;

/* Internodes */
NTsize numIntComp;
NTreal lengthIntNd; /* micron */
NTreal intGLeak; /* mSiemens/cm^2 */
NTreal intRa; /* Ohm cm */
NTreal intCm; /* muFarad/cm^2 */
NTsize intPotassiumModel;
NTsize intPotassiumAlg;
NTreal intPotassiumDensity; //  per mu^2
NTreal intPotassiumConductance;

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
	NT_config_file_parser_o oCfg(fileName);
	/* Global */
	temperature = oCfg.Value("global", "temperature"); //= 37; /* in Celsius */
	diameter = oCfg.Value("global", "diameter"); // = 1; /* in muMeter */

	if (0 == oCfg.Value("global", "computeELeak")) {
		swComputeELeak = false;
		eLeak = oCfg.Value("global", "eLeak"); /* in mV */
	} else {
		swComputeELeak = true;
	}
	/* Nodes */
	numNd = oCfg.Value("node", "numNd"); /* start with node at proximal end */
	numNdComp = oCfg.Value("node", "numComp"); /* per Node ! */
	lengthNd = oCfg.Value("node", "length"); /* micron */
	ndGLeak = oCfg.Value("node", "GLeak");/* mSiemens/cm^2 */
	ndRa = oCfg.Value("node", "Ra"); /* Ohm cm */
	ndCm = oCfg.Value("node", "Cm"); /* muFarad/cm^2 */
	ndSodiumModel = oCfg.Value("node", "chNaModel");
	ndSodiumAlg = oCfg.Value("node", "chNaAlg");
	ndSodiumDensity = oCfg.Value("node", "chNaDen"); //= 60; // per mu^2
	ndSodiumConductance = oCfg.Value("node", "chNaCond");
	ndSodiumQ10m = oCfg.Value("node", "chNaQ10m");
	ndSodiumQ10h = oCfg.Value("node", "chNaQ10h");
	noSodiumReversalPotential = oCfg.Value("node", "chNaRevPot");

	/* Paranodes */
	numPndComp = oCfg.Value("paranode", "numComp");
	lengthPnd = oCfg.Value("paranode", "length"); /* micron */
	pndGLeak = oCfg.Value("paranode", "GLeak"); /* mSiemens/cm^2 */
	pndRa = oCfg.Value("paranode", "Ra"); /* Ohm cm */
	pndCm = oCfg.Value("paranode", "Cm"); /* muFarad/cm^2 */
	pndPotassiumModel = oCfg.Value("paranode", "chKModel");
	pndPotassiumAlg = oCfg.Value("paranode", "chKAlg");
	pndPotassiumDensity = oCfg.Value("paranode", "chKDen"); //  per mu^2
	pndPotassiumConductance = oCfg.Value("paranode", "chKCond");
	pndPotassiumQ10 = oCfg.Value("paranode", "chKQ10");

	/* Internodes */
	numIntComp = oCfg.Value("internode", "numComp");
	lengthIntNd = oCfg.Value("internode", "length"); /* micron */
	intGLeak = oCfg.Value("internode", "GLeak"); /* mSiemens/cm^2 */
	intRa = oCfg.Value("internode", "Ra"); /* Ohm cm */
	intCm = oCfg.Value("internode", "Cm"); /* muFarad/cm^2 */
	intPotassiumModel = oCfg.Value("internode", "chKModel");
	intPotassiumAlg = oCfg.Value("internode", "chKAlg");
	intPotassiumDensity = oCfg.Value("internode", "chKDen"); //  per mu^2
	intPotassiumConductance = oCfg.Value("paranode", "chKCond");

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
	numAxonHillockNodeCompartments = 10;

	cerr
			<< "Remember that data file should have more lines than Num iterations."
			<< endl;
}

void printConfig(ofstream& out) {
	out << "#[Global]" << endl;
	out << "#Temperature [Celsius] " << temperature << endl;
	out << "#Membrane leak reversal potential [mV]" << eLeak << endl;

	out << "#[Node]" << endl;
	out << "#Number of axon hillock compartments (node-like) "
			<< numAxonHillockNodeCompartments << endl;
	out << "#Number of nodes " << numNd << endl;
	out << "#Number of node compartments per node " << numNdComp << endl;
	out << "#Node Compartment Diameter [muMeter] " << diameter << endl;
	out << "#Node Compartment Length [muMeter] " << lengthNd << endl;
	out << "#Membrane leak conductance [mSiemens/cm^2] " << ndGLeak << endl;
	out << "#Axoplasmic resistivity [Ohm cm] " << ndRa << endl;
	out << "#Sodium Q10m " << ndSodiumQ10m << endl;
	out << "#Sodium Q10h " << ndSodiumQ10h << endl;
	out << "#Sodium Channel reversal potential " << noSodiumReversalPotential
			<< endl;

	out << "#[Paranode]" << endl;
	out << "#Paranode Compartment Diameter [muMeter] " << diameter << endl;
	out << "#Membrane leak conductance [mSiemens/cm^2] " << pndGLeak << endl;
	out << "#Axoplasmic resistivity [Ohm cm] " << pndRa << endl;

	out << "#[Internode]" << endl;
	out << "#InterNode Compartment Diameter [muMeter] " << diameter << endl;
	out << "#Membrane leak conductance [mSiemens/cm^2] " << intGLeak << endl;
	out << "#Axoplasmic resistivity [Ohm cm] " << intRa << endl;

	out << "#[Simulation]" << endl;
	out << "#Storing data in" << filename << " every " << sampN
			<< "th iteration" << endl;
	out << "#Time step size in [mSec] " << timeStep << endl;
	out << "#Num iterations [#] " << numIterations << endl;
	out << "#Trial duration [ms] " << numIterations * timeStep << endl;
	out << "#Number of repated stimulus trials [#] " << numTrials << endl;
}

ofstream openOutputFile(string outputFolder, string prefix) {
	/* open files */
	time_t rawtime;
	struct tm * timeinfo;
	stringstream ss(stringstream::in | stringstream::out);
	char dateString[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(dateString, 80, "%b%d_%H%M.txt", timeinfo);
	ss << outputFolder << "/" << prefix << "-" << dateString;

	ofstream outStream(ss.str().c_str(), ios::binary);

	if (outStream.fail()) {
		cerr << "Could not open output file " << prefix << endl;
		exit(1);
	}
	return outStream;
}

NTBP_custom_cylindrical_compartment_o* createSection(NTsize numberCompartments, NTsize length, NTsize sectionDiameter,
		NTreal C, NTsize R, NTsize temperature, NTreal eLeak, NTreal gLeak, NTsize channelType,
		NTsize channelModel, NTsize channelAlg, NTsize channelDensity /* mum^-2 */,
		NTreal channelConductance /* pS */, NTreal q10m,
		NTreal q10h /* q10 */, NTreal channelReversalPotential) {
	for (NTsize lcomp = 0; lcomp < numberCompartments; lcomp++) {
		/*cout << compartmentCounter++ << "NODE (Axon Hillock)"
		 << endl;
		 cerr << "Node compartment " << endl;*/
		NTBP_custom_cylindrical_compartment_o
				*tmpPtr = new NTBP_custom_cylindrical_compartment_o(
						length /* muMeter */, sectionDiameter /* muMeter */,
						C/*muFarad/cm^2 */, R /* ohm cm */);
		tmpPtr->Set_temperature(temperature /* in celsius */);
		/* Leak current is number 0 */
		tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
				gLeak, eLeak), NTBP_LEAK);
		/* Channel current is number 1 */
		if(channelType == CHANNEL_TYPE_SODIUM)
		tmpPtr->AttachCurrent(NTBP_create_na_channel_ptr(channelModel,
				channelAlg, channelDensity /* mum^-2 */,
				channelConductance /* pS */, q10m,
				q10h /* q10 */, temperature /* C */,
				tmpPtr->_area() /* mum^2 */, channelReversalPotential /*mV*/),
				NTBP_IONIC);
		if(channelType == CHANNEL_TYPE_POTASSIUM)
				tmpPtr->AttachCurrent(NTBP_create_k_channel_ptr(channelModel, channelAlg,
						channelDensity /* mum^-2 */,
						channelConductance /* pS */, 3 /* q10 */,
						temperature /* C */, tmpPtr->_area() /* mum^2 */),
						NTBP_IONIC);
		/* Dummy zero leak current is number 2 */
		tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
				0, eLeak), NTBP_LEAK);
		/*oModel.PushBack(tmpPtr);*/
		return tmpPtr;
	}
}

int main(int argc, char *argv[]) {

	/* Read and set parameters */
	readConfig(argv[1]);
	ofstream ATPFile = openOutputFile(outputFolder, "ATP");
	ofstream PotentialFile = openOutputFile(outputFolder, "Potential");

	printConfig(ATPFile);
	printConfig(PotentialFile);
	if (false == swComputeELeak) {
		cout << "Eleak set to " << eLeak << " mV." << endl;
	} else {
		cout << "Eleak not set. Going to compute eLeak." << endl;
		/* COMPUTE Eleak by simulating and solving for the current balance equation after
		 * 50ms of simulated time at the NODE including the PARANODAL K channels*/

		/* Create a cylindrical membrane compartment */
		NTBP_custom_cylindrical_compartment_o
				compartment(1 /* muMeter */, diameter /* muMeter */,
						ndCm /*muFarad/cm^2 */, ndRa /* ohm cm */);
		NTreal areaPerCompartment = compartment._area();
		compartment.Set_temperature(temperature /* in celsius */);

		NTBP_membrane_current_o* tmpLeakPtr = new NTBP_hh_sga_leak_current_o(
				areaPerCompartment, ndGLeak, eLeak);

		/* Na current is number 2 */
		NTBP_membrane_current_o* tmpNaPtr =
				NTBP_create_na_channel_ptr(
						ndSodiumModel,
						1//ndSodiumAlg
						, ndSodiumDensity /* mum^-2 */,
						ndSodiumConductance /* pS */, ndSodiumQ10m,
						ndSodiumQ10h, temperature /* C */,
						areaPerCompartment /* mum^2 */,
						noSodiumReversalPotential /*mV*/);
		compartment.AttachCurrent(tmpNaPtr, NTBP_IONIC);
		/* K current is number 3 */
		NTBP_membrane_current_o* tmpKPtr = NTBP_create_k_channel_ptr(
				pndPotassiumModel,
				1,//pndPotassiumAlg,
				pndPotassiumDensity /* mum^-2 */,
				pndPotassiumConductance /* pS */, pndPotassiumQ10 /* q10 */,
				temperature /* C */, areaPerCompartment /* mum^2 */);
		compartment.AttachCurrent(tmpKPtr, NTBP_IONIC);
		//compartment.AttachCurrent(tmpLeakPtr, NTBP_LEAK);//not sure
		float naCurrent = 0;
		float kCurrent = 0;

		/* ***  Determine leak reversal potential by simulating 50 ms *** */
		NTsize lt = 0;
		NTreal tmpEleak = 0;
		for (lt = 0; lt < 100.0 / timeStep; lt++) {
			compartment.Step(0);
			if (lt % 100 == 0) {
				naCurrent = compartment.AttachedReversalPotential(1)
						* (compartment.AttachedConductance(1));
				kCurrent = compartment.AttachedReversalPotential(2)
						* (compartment.AttachedConductance(2));

				tmpEleak = -(naCurrent + kCurrent) / tmpLeakPtr->_conductance();
				cerr << "I_Na=" << naCurrent << " I_K=" << kCurrent
						<< " E_Leak=" << tmpEleak << endl;
			}
		}

		naCurrent = compartment.AttachedReversalPotential(1)
				* (compartment.AttachedConductance(1));
		kCurrent = compartment.AttachedReversalPotential(2)
				* (compartment.AttachedConductance(2));
		eLeak = -(naCurrent + kCurrent) / tmpLeakPtr->_conductance();
		swComputeELeak = false;
		cout << "Eleak computed as " << eLeak << " mV" << endl;
		cerr << "Eleak computed as " << eLeak << " mV" << endl;
		if ((eLeak > compartment.AttachedReversalPotential(1)) || (eLeak
				< compartment.AttachedReversalPotential(2))) {
			cout << "Eleak might be out of biological plausible range." << endl;
			cout << "Typically E_Na=" << compartment.AttachedReversalPotential(
					1) << " < Eleak=" << eLeak << " < E_K="
					<< compartment.AttachedReversalPotential(2) << endl;
			cout
					<< "Does a stable resting potential exist at all? Check by increasing iterations of current balance equation."
					<< endl;
			cerr << "Eleak might be out of biological plausible range." << endl;
			cerr << "Typically E_Na=" << compartment.AttachedReversalPotential(
					1) << " < Eleak=" << eLeak << " < E_K="
					<< compartment.AttachedReversalPotential(2) << endl;
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

	cout << "Assembling neuron..." << endl;
	/* *** Trials loop *** */
	for (NTsize lTrials = 0; lTrials < numTrials; lTrials++) {

		/* Model setup */
		NTBP_membrane_compartment_sequence_o oModel;
		oModel.UpdateTimeStep(timeStep /* mSec */);
		oModel.StepNTBP();
		NTreal timeInMS = 0;

		NTsize compartmentCounter = 1;
		/* *** MODEL CREATION LOOP *** */
		/* Create a Node, followed by Paranode, Internode, Paranode */
		for (NTsize lnd = 0; lnd < numNd; lnd++) {

			if (0 == lnd) { // Generate an axon hillock

				for (NTsize lcomp = 0; lcomp < numAxonHillockNodeCompartments; lcomp++) {
					cout << compartmentCounter++ << "NODE (Axon Hillock)"
							<< endl;
					cerr << "Node compartment " << endl;
					NTBP_custom_cylindrical_compartment_o *tmpPtr =
							new NTBP_custom_cylindrical_compartment_o(
									lengthNd /* muMeter */,
									diameter /* muMeter */,
									ndCm/*muFarad/cm^2 */, ndRa /* ohm cm */);
					tmpPtr->Set_temperature(temperature /* in celsius */);
					/* Leak current is number 0 */
					tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
							tmpPtr->_area(), ndGLeak, eLeak), NTBP_LEAK);
					/* Na current is number 1 */
					tmpPtr->AttachCurrent(NTBP_create_na_channel_ptr(
							ndSodiumModel, ndSodiumAlg,
							ndSodiumDensity /* mum^-2 */,
							ndSodiumConductance /* pS */, ndSodiumQ10m,
							ndSodiumQ10h /* q10 */, temperature /* C */,
							tmpPtr->_area() /* mum^2 */,
							noSodiumReversalPotential /*mV*/), NTBP_IONIC);
					/* Dummy zero leak current is number 2 */
					tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
							tmpPtr->_area(), 0, eLeak), NTBP_LEAK);
					oModel.PushBack(tmpPtr);
				}

			}

			/* Create a Node compartment */
			for (NTsize lcomp = 0; lcomp < numNdComp; lcomp++) {
				cout << compartmentCounter++ << "NODE" << endl;
				cerr << "Node compartment " << endl;
				NTBP_custom_cylindrical_compartment_o *tmpPtr =
						new NTBP_custom_cylindrical_compartment_o(
								lengthNd /* muMeter */, diameter /* muMeter */,
								ndCm/*muFarad/cm^2 */, ndRa /* ohm cm */);
				tmpPtr->Set_temperature(temperature /* in celsius */);
				/* Leak current is number 0 */
				tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
						tmpPtr->_area(), ndGLeak, eLeak), NTBP_LEAK);
				/* Na current is number 1 */
				tmpPtr->AttachCurrent(
						NTBP_create_na_channel_ptr(ndSodiumModel, ndSodiumAlg,
								ndSodiumDensity /* mum^-2 */,
								ndSodiumConductance /* pS */, ndSodiumQ10m,
								ndSodiumQ10h /* q10 */, temperature /* C */,
								tmpPtr->_area() /* mum^2 */,
								noSodiumReversalPotential /*mV*/), NTBP_IONIC);
				/* Dummy zero leak current is number 2 */
				tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
						tmpPtr->_area(), 0, eLeak), NTBP_LEAK);
				oModel.PushBack(tmpPtr);
			}

			/* Create a Paranode compartment */
			for (NTsize lcomp = 0; lcomp < numPndComp; lcomp++) {
				cout << compartmentCounter++ << "PARA" << endl;
				cerr << "Paranode compartment " << endl;
				NTBP_custom_cylindrical_compartment_o *tmpPtr =
						new NTBP_custom_cylindrical_compartment_o(
								lengthPnd /* muMeter */,
								diameter /* muMeter */, pndCm/*muFarad/cm^2 */,
								pndRa /* ohm cm */);
				tmpPtr->Set_temperature(temperature /* in celsius */);
				/* Leak current is number 0 */
				tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
						tmpPtr->_area(), pndGLeak, eLeak), NTBP_LEAK);
				/* Dummy zero leak current is number 1 */
				tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
						tmpPtr->_area(), 0.001, eLeak), NTBP_LEAK);
				/* K current is number 2 */
				tmpPtr->AttachCurrent(NTBP_create_k_channel_ptr(
						pndPotassiumModel, pndPotassiumAlg,
						pndPotassiumDensity /* mum^-2 */,
						pndPotassiumConductance /* pS */, 3 /* q10 */,
						temperature /* C */, tmpPtr->_area() /* mum^2 */),
						NTBP_IONIC);
				oModel.PushBack(tmpPtr);
			}

			if ((numNd - 1) == lnd) {
				break;
			}

			/* Create an Internode compartment */
			if (emulateMS && (lnd == emulateMS || lnd == emulateMS + 1)) {
				for (NTsize lcomp = 0; lcomp < numIntComp; lcomp++) {
					cout << compartmentCounter++ << "INT" << endl;
					cerr << "Internode compartment " << endl;
					NTBP_custom_cylindrical_compartment_o *tmpPtr =
							new NTBP_custom_cylindrical_compartment_o(
									lengthIntNd /* muMeter */,
									diameter /* muMeter */,
									intCm/*muFarad/cm^2 */, intRa /* ohm cm */);
					tmpPtr->Set_temperature(temperature /* in celsius */);
					/* Leak current is number 0 */
					tmpPtr->AttachCurrent(
							new NTBP_hh_sga_leak_current_o(tmpPtr->_area(),
									intGLeak * emulateMSFactor, eLeak),
							NTBP_LEAK);
					/* Dummy zero leak current is number 1 */
					tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
							tmpPtr->_area(), 0.001, eLeak), NTBP_LEAK);
					/* K current is number 2 */
					tmpPtr->AttachCurrent(NTBP_create_k_channel_ptr(
							intPotassiumModel, 4,
							intPotassiumDensity /* mum^-2 */,
							intPotassiumConductance /* pS */, 3 /* q10 */,
							temperature /* C */, tmpPtr->_area() /* mum^2 */),
							NTBP_IONIC);
					oModel.PushBack(tmpPtr);
				}
			} else {
				for (NTsize lcomp = 0; lcomp < numIntComp; lcomp++) {
					cout << compartmentCounter++ << "INT" << endl;
					cerr << "Internode compartment " << endl;
					NTBP_custom_cylindrical_compartment_o *tmpPtr =
							new NTBP_custom_cylindrical_compartment_o(
									lengthIntNd /* muMeter */,
									diameter /* muMeter */,
									intCm/*muFarad/cm^2 */, intRa /* ohm cm */);
					tmpPtr->Set_temperature(temperature /* in celsius */);
					/* Leak current is number 0 */
					tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
							tmpPtr->_area(), intGLeak, eLeak), NTBP_LEAK);
					/* Dummy zero leak current is number 1 */
					tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
							tmpPtr->_area(), 0.001, eLeak), NTBP_LEAK);
					/* K current is number 2 */
					tmpPtr->AttachCurrent(NTBP_create_k_channel_ptr(
							intPotassiumModel, intPotassiumAlg,
							intPotassiumDensity /* mum^-2 */,
							intPotassiumConductance /* pS */, 3 /* q10 */,
							temperature /* C */, tmpPtr->_area() /* mum^2 */),
							NTBP_IONIC);
					oModel.PushBack(tmpPtr);
				}
			}

			/* Create a Paranode compartment */
			for (NTsize lcomp = 0; lcomp < numPndComp; lcomp++) {
				cout << compartmentCounter++ << "PARA" << endl;
				cerr << "Paranode compartment " << endl;
				NTBP_custom_cylindrical_compartment_o *tmpPtr =
						new NTBP_custom_cylindrical_compartment_o(
								lengthPnd /* muMeter */,
								diameter /* muMeter */, pndCm/*muFarad/cm^2 */,
								pndRa /* ohm cm */);
				tmpPtr->Set_temperature(temperature /* in celsius */);
				/* Leak current is number 0 */
				tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
						tmpPtr->_area(), pndGLeak, eLeak), NTBP_LEAK);
				/* Dummy zero leak current is number 1 */
				tmpPtr->AttachCurrent(new NTBP_hh_sga_leak_current_o(
						tmpPtr->_area(), 0.001, eLeak), NTBP_LEAK);
				/* K current is number 2 */
				tmpPtr->AttachCurrent(NTBP_create_k_channel_ptr(
						pndPotassiumModel, pndPotassiumAlg,
						pndPotassiumDensity /* mum^-2 */,
						pndPotassiumConductance /* pS */, 3 /* q10 */,
						temperature /* C */, tmpPtr->_area() /* mum^2 */),
						NTBP_IONIC);
				oModel.PushBack(tmpPtr);
			}

		}

		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = numAxonHillockNodeCompartments + (numNd - 1)
				* (numNdComp + numPndComp + numIntComp + numPndComp)
				+ numNdComp + numPndComp;
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
			plotChanNa.SetYRange(0, 100);
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
		//float tmpCurr = 0.0;
		NTreal inpCurrent = 0.0;

		NT_uniform_rnd_dist_o uniformRnd;

		vector<NTreal> voltVec;
		vector<NTreal>::iterator maxVoltPos;
		vector<NTreal>::iterator maxVoltOldPos;
		//NTreal speed;

		//NTreal maxVoltage;

		timeInMS = 0;

		int dataRead = 0;
		for (NTsize lt = 0; lt < numIterations; lt++) {
			timeInMS += oModel._timeStep();
			timeVar = timeInMS;

			if (lt % sampN == 0) {
				/* the "sampling ratio" used for "measurement" to disk */
				//file.write(reinterpret_cast<char *> (&timeVar), sizeof(float));
				//file.write(reinterpret_cast<char *> (&inpCurrent), sizeof(float));
				oModel.WriteMembranePotentialASCII(PotentialFile);
				//oModel.WriteCurrent(file, 2); // Na
				//oModel.WriteCurrent(file, 3); // K

				// Added by Ali
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
					//cout << "Na" << endl;
					plotChanNa.SetData(oModel.OpenChannels(2));
					plotChanNa.Draw();
					//cout << "K" << endl;
					plotChanK.SetData(oModel.OpenChannels(3));
					plotChanK.Draw();
					//cout << "plotted" << endl;
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

