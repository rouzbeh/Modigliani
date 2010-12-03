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
 * Revision 1.1  2005/04/25 15:13:17  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2004/12/14 15:48:32  face
 * *** empty log message ***
 *
 * Revision 1.1  2003/08/24 10:44:17  face
 * *** empty log message ***
 *
 * Revision 1.1  2003/01/30 17:13:39  face
 * *** empty log message ***
 *
 * Revision 1.3  2002/03/08 15:13:24  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/12 09:18:34  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:36:53  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:10  face
 * *** empty log message ***
 *
 */

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "ntbp_membrane_compartment_sequence_obj.h"
#include "ntbp_sga_cylindrical_compartment_obj.h"
#include "ntbp_custom_cylindrical_compartment_obj.h"

#include "ntbp_auxfunc.h"

#include "ntbp_multi_sodium_current_obj.h"
#include "ntbp_colbert_axonal_sodium_multi_current_obj.h"
#include "ntbp_hranvier_sodium_multi_current_obj.h"
#include "ntbp_patlak_sodium_multi_current_obj.h"

#include "ntbp_multi_potassium_current_obj.h"
#include "ntbp_colbert_axonal_potassium_multi_current_obj.h"
#include "ntbp_hranvier_potassium_multi_current_obj.h"
#include "ntbp_potassium_multi_current_obj.h"

#include "nt_error_obj.h"
#include "nt_vector_obj.h"

#include "nt_histogram_obj.h"
#include "nt_sequential_statistics_obj.h"
#include "tnt/tnt.h"
#include "tnt/cmat.h"
#include "nt_uniform_rnd_dist_obj.h"
#include "nt_binomial_rnd_dist_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
#include "nt3d_plot2d_vec_vp_obj.h"
#include "nt3d_glx_drv_obj.h"

#include "nt_config_file_parser_obj.h"

using namespace std;
using namespace TNT;

int main(int argc, char *argv[]) {

	/* Read and set parameters */
	string filename;
	NT_config_file_parser_o oCfg("mylienated.config.txt");
	/* Global */
	NTreal temperature = oCfg.Value("global", "temperature"); //= 37; /* in Celsius */
	NTreal diameter = oCfg.Value("global", "diameter"); // = 1; /* in muMeter */
	NTreal eLeak;
	bool swComputeELeak;
	if (0 == oCfg.Value("global", "computeELeak")) {
		swComputeELeak = false;
		eLeak = oCfg.Value("global", "eLeak"); /* in mV */
	} else {
		swComputeELeak = true;
	}
	/* Nodes */
	NTsize numNd = oCfg.Value("node", "numNd"); /* start with node at proximal end */
	NTsize numNdComp = oCfg.Value("node", "numComp"); /* per Node ! */
	NTreal lengthNd = oCfg.Value("node", "length"); /* micron */
	NTreal ndGLeak = oCfg.Value("node", "GLeak");/* mSiemens/cm^2 */
	NTreal ndRa = oCfg.Value("node", "Ra"); /* Ohm cm */
	NTreal ndCm = oCfg.Value("node", "Cm"); /* muFarad/cm^2 */
	NTsize ndSodiumModel = oCfg.Value("node", "chNaModel");
	NTsize ndSodiumAlg = oCfg.Value("node", "chNaAlg");
	NTreal ndSodiumDensity = oCfg.Value("node", "chNaDen"); //= 60; // per mu^2
	NTreal ndSodiumConductance = oCfg.Value("node", "chNaCond");

	/* Paranodes */
	NTsize numPndComp = oCfg.Value("paranode", "numComp");
	NTreal lengthPnd = oCfg.Value("paranode", "length"); /* micron */
	NTreal pndGLeak = oCfg.Value("paranode", "GLeak"); /* mSiemens/cm^2 */
	NTreal pndRa = oCfg.Value("paranode", "Ra"); /* Ohm cm */
	NTreal pndCm = oCfg.Value("paranode", "Cm"); /* muFarad/cm^2 */
	NTsize pndPotassiumModel = oCfg.Value("paranode", "chKModel");
	NTsize pndPotassiumAlg = oCfg.Value("paranode", "chKAlg");
	NTreal pndPotassiumDensity = oCfg.Value("paranode", "chKDen"); //  per mu^2
	NTreal pndPotassiumConductance = oCfg.Value("paranode", "chKCond");

	/* Internodes */
	NTsize numIntComp = oCfg.Value("internode", "numComp");
	NTreal lengthIntNd = oCfg.Value("internode", "length"); /* micron */
	NTreal intGLeak = oCfg.Value("internode", "GLeak"); /* mSiemens/cm^2 */
	NTreal intRa = oCfg.Value("internode", "Ra"); /* Ohm cm */
	NTreal intCm = oCfg.Value("internode", "Cm"); /* muFarad/cm^2 */
	NTsize intPotassiumModel = oCfg.Value("internode", "chKModel");
	NTsize intPotassiumAlg = oCfg.Value("internode", "chKAlg");
	NTreal intPotassiumDensity = oCfg.Value("internode", "chKDen"); //  per mu^2
	NTreal intPotassiumConductance = oCfg.Value("paranode", "chKCond");

	/* Simulation */
	string inputFilename = oCfg.Value("simulation", "inputFile");
	string ATPOutputFilename = oCfg.Value("simulation", "ATPOutputFile");
	string potentialOutputFilename = oCfg.Value("simulation", "potentialOutputFile");
	NTsize readN = oCfg.Value("simulation", "readN");
	NTreal inpI = oCfg.Value("simulation", "inpI");
	NTreal inpISDV = oCfg.Value("simulation", "inpISDV");
	NTsize useVis = oCfg.Value("simulation", "useVis");
	NTsize sampN = oCfg.Value("simulation", "sampN");
	NTreal timeStep = oCfg.Value("simulation", "timeStep"); // in msec
	NTsize numIterations = oCfg.Value("simulation", "numIter");
	NTreal numTrials = oCfg.Value("simulation", "numTrials");


	int numAxonHillockNodeCompartments = 10;

	cerr
			<< "Remember that data file should have more lines than Num iterations."
			<< endl;

	cout << "[Global]" << endl;
	cout << "Temperature [Celsius] " << temperature << endl;
	cout << "Membrane leak reversal potential [mV]" << eLeak << endl;

	cout << "[Node]" << endl;
	cout << "Number of axon hillock compartments (node-like) " << numAxonHillockNodeCompartments << endl;
	cout << "Number of nodes " << numNd << endl;
	cout << "Number of node compartments per node " << numNdComp << endl;
	cout << "Node Compartment Diameter [muMeter] " << diameter << endl;
	cout << "Node Compartment Length [muMeter] " << lengthNd << endl;
	cout << "Membrane leak conductance [mSiemens/cm^2] " << ndGLeak << endl;
	cout << "Axoplasmic resistivity [Ohm cm] " << ndRa << endl;

	cout << "[Paranode]" << endl;
	cout << "Paranode Compartment Diameter [muMeter] " << diameter << endl;
	cout << "Membrane leak conductance [mSiemens/cm^2] " << pndGLeak << endl;
	cout << "Axoplasmic resistivity [Ohm cm] " << pndRa << endl;

	cout << "[Internode]" << endl;
	cout << "InterNode Compartment Diameter [muMeter] " << diameter << endl;
	cout << "Membrane leak conductance [mSiemens/cm^2] " << intGLeak << endl;
	cout << "Axoplasmic resistivity [Ohm cm] " << intRa << endl;

	cout << "[Simulation]" << endl;
	cout << "Storing data in" << filename << " every " << sampN
			<< "th iteration" << endl;
	cout << "Time step size in [mSec] " << timeStep << endl;
	cout << "Num iterations [#] " << numIterations << endl;
	cout << "Trial duration [ms] " << numIterations * timeStep << endl;
	cout << "Number of repated stimulus trials [#] " << numTrials << endl;


	if (false == swComputeELeak) {
		cout << "Eleak set to " << eLeak << " mV." << endl;
	} else {
		cout << "Eleak not set. Going to compute eLeak." << endl;
		/* COMPUTE Eleak by simulating and solving for the current balance equation after 50ms of simulated time at the NODE including the PARANODAL K channels*/

		/* Create a cylindrical membrane compartment */
		NTBP_custom_cylindrical_compartment_o compartment(10 /* muMeter */,
				10 /* muMeter */, 1 /*muFarad/cm^2 */, 100 /* ohm cm */);
		NTreal areaPerCompartment = compartment._area();
		compartment.Set_temperature(temperature /* in celsius */);

		NTBP_membrane_current_o* tmpLeakPtr = new NTBP_hh_sga_leak_current_o(
				compartment._area(), ndGLeak, eLeak);

		/* Na current is number 1 */
		NTBP_membrane_current_o* tmpNaPtr = NTBP_create_na_channel_ptr(
				ndSodiumModel, ndSodiumAlg, ndSodiumDensity /* mum^-2 */,
				ndSodiumConductance /* pS */, 3 /* q10 */, temperature /* C */,
				compartment._area() /* mum^2 */);
		compartment.AttachCurrent(tmpNaPtr, NTBP_IONIC);
		/* K current is number 2 */
		NTBP_membrane_current_o* tmpKPtr = NTBP_create_k_channel_ptr(
				pndPotassiumModel, pndPotassiumAlg,
				pndPotassiumDensity /* mum^-2 */,
				pndPotassiumConductance /* pS */, 3 /* q10 */,
				temperature /* C */, compartment._area() /* mum^2 */);
		compartment.AttachCurrent(tmpKPtr, NTBP_IONIC);

		float naCurrent = 0;
		float kCurrent = 0;

		/* ***  Determine leak reversal potential by simulating 50 ms *** */
		NTsize lt = 0;
		for (lt = 0; lt < 50.0 / timeStep; lt++) {
			compartment.Step(0);
			if (lt % 50 == 0) {
				naCurrent = compartment.AttachedReversalPotential(1)
						* (compartment.AttachedConductance(1));
				kCurrent = compartment.AttachedReversalPotential(2)
						* (compartment.AttachedConductance(2));
				cerr << "I_Na=" << naCurrent << " I_K=" << kCurrent
						<< " E_Leak=" << -(naCurrent + kCurrent)
						/ tmpLeakPtr->_conductance() << endl;
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
					<< "Does a stable resting potential exist at all? Check by increasing iteratiosn of current balance equation."
					<< endl;
			cerr << "Eleak might be out of biological plausible range." << endl;
			cerr << "Typically E_Na=" << compartment.AttachedReversalPotential(
					1) << " < Eleak=" << eLeak << " < E_K="
					<< compartment.AttachedReversalPotential(2) << endl;
			cerr
					<< "Does a stable resting potential exist at all? Check by increasing iteratiosn of current balance equation."
					<< endl;
		}
	}

	/* open files */
	ofstream ATPFile(ATPOutputFilename.c_str(), ios::binary);
	if(ATPFile.fail()){
		cerr << "Could not open output file " << ATPOutputFilename.c_str() << endl;
		return 1;
	}
	ofstream PotentialFile(potentialOutputFilename.c_str(), ios::binary);
	if(PotentialFile.fail()){
		cerr << "Could not open output file " << potentialOutputFilename.c_str() << endl;
		return 1;
	}

	NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur!
	NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !
	NTreal rnd = 0;

	// Read input file only once. Store its content in memory.
	ifstream dataFile(inputFilename.c_str(), ios::binary);
	if(dataFile.fail()){
		cerr << "Could not open input file " << inputFilename.c_str() << endl;
		exit(1);
	}
	int count=1000000;
	vector<float> inputData (count);
	int index=0;

	while(dataFile.good()){
		if (index<count) {
			char tmp[100];
			dataFile.getline(tmp,100);
			sscanf(tmp, "%f", &inputData[index]);
			index++;
		}
		else {
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


			if (0 == lnd){ // Generate an axon hillock

				for (NTsize lcomp = 0; lcomp < numAxonHillockNodeCompartments; lcomp++) {
					cout << compartmentCounter++ << "NODE (Axon Hillock)" << endl;
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
							tmpPtr->AttachCurrent(NTBP_create_na_channel_ptr(ndSodiumModel,
									ndSodiumAlg, ndSodiumDensity /* mum^-2 */,
									ndSodiumConductance /* pS */, 3 /* q10 */,
									temperature /* C */, tmpPtr->_area() /* mum^2 */),
									NTBP_IONIC);
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
				tmpPtr->AttachCurrent(NTBP_create_na_channel_ptr(ndSodiumModel,
						ndSodiumAlg, ndSodiumDensity /* mum^-2 */,
						ndSodiumConductance /* pS */, 3 /* q10 */,
						temperature /* C */, tmpPtr->_area() /* mum^2 */),
						NTBP_IONIC);
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
			for (NTsize lcomp = 0; lcomp < numIntComp; lcomp++) {
				cout << compartmentCounter++ << "INT" << endl;
				cerr << "Internode compartment " << endl;
				NTBP_custom_cylindrical_compartment_o *tmpPtr =
						new NTBP_custom_cylindrical_compartment_o(
								lengthIntNd /* muMeter */,
								diameter /* muMeter */, intCm/*muFarad/cm^2 */,
								intRa /* ohm cm */);
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
		NTsize numCompartments = numAxonHillockNodeCompartments + (numNd - 1) * (numNdComp + numPndComp + numIntComp + numPndComp) + numNdComp + numPndComp;
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
			plotXY.SetYRange(-30, 120);
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
		float tmpCurr = 0.0;
		NTreal inpCurrent = 0.0;

		/* prerun system for 20 ms
		 oModel.InjectCurrent(inpCurrent, 0);
		 NTsize ll=0;
		 for(ll=0;ll<20.0/oModel._timeStep();ll++ )
		 {
		 oModel.Step();
		 }
		 */

		NT_uniform_rnd_dist_o uniformRnd;

		vector<NTreal> voltVec;
		vector<NTreal>::iterator maxVoltPos;
		vector<NTreal>::iterator maxVoltOldPos;
		NTreal speed;

		NTreal maxVoltage;

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

					for (NTsize lc = 0; lc++; lc < numCompartments) {
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

					cerr << "t=" << timeInMS << " mSec :" << voltVec[1] << "\t"
							<< voltVec[10] << endl;

					plotXY.SetData(oModel._vVec());
					plotXY.Draw();
					cout << "Na" << endl;
					plotChanNa.SetData(oModel.OpenChannels(2));
					plotChanNa.Draw();
					cout << "K" << endl;
					plotChanK.SetData(oModel.OpenChannels(3));
					plotChanK.Draw();
					cout << "plotted" << endl;
				}
			}

			if (lt % readN == 0) {
				inpCurrent = (inputData[dataRead] * inpISDV) + inpI;
				dataRead++;
			}
			oModel.InjectCurrent(inpCurrent, 1);

			oModel.Step();
			//oModel.GillespieStep();
		}
	} // lTrials


	ATPFile.close();
	PotentialFile.close();
	cerr << "Simulation completed." << endl;
	return 0;
}
