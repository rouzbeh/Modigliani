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

NTreal NTBP_corrected_channel_density(NTreal chDensity, NTreal compArea) {
	NTreal chPerCompartment = compArea * chDensity;
	NTreal pChFloor = (ceil(chPerCompartment) - chPerCompartment);

	NT_uniform_rnd_dist_o uniRnd;
	/* compute number of channels, such that average density is achieved */
	NTreal indChDensity = (uniRnd.RndVal() > pChFloor ? ceil(chPerCompartment)
			/ compArea : floor(chPerCompartment) / compArea);

	return indChDensity;
}

NTBP_membrane_current_o *
NTBP_create_na_channel_ptr(NTsize sodiumModel, NTsize sodiumAlg,
		NTreal sodiumDensity, NTreal sodiumConductance, NTreal q10m,
		NTreal q10h, NTreal temperature, NTreal area, NTreal timeStep, NTreal reversalPotential,
		NTreal vBase) {

	NTreal indSodiumDensity = NTBP_corrected_channel_density(sodiumDensity,
			area);

	/* Na ion channel currents */
	NTBP_membrane_current_o *tmpNaPtr;
	switch (sodiumModel) {
	case 1:
		/* SGA */
		tmpNaPtr
				= new NTBP_multi_sodium_current_o(area, indSodiumDensity, pow(
						3, (temperature - 6.3) / 10.0) * sodiumConductance
						* 1e-9 /* mSiemens per channel */, vBase,
						reversalPotential);
		cout << "Using SGA Na model." << endl;
		break;
	case 2:
		/* hRanvier */
		tmpNaPtr = new NTBP_hranvier_sodium_multi_current_o(area,
				indSodiumDensity, sodiumConductance * 1e-9, /* mSiemens per channel */
				vBase, q10m, q10h, reversalPotential, timeStep, temperature);
		cout << "Using human Ranvier Na model." << endl;
		break;
	case 3:
		/* COLBERTAXONAL */
		tmpNaPtr = new NTBP_colbert_axonal_sodium_multi_current_o(area,
				indSodiumDensity, pow(1.4, (temperature - 24.0) / 10.0)
						* sodiumConductance * 1e-9 /* mSiemens per channel */
				, vBase, q10m, q10h, reversalPotential);
		cout << "Using Colbert-Axonal Na model." << endl;
		break;
	case 4:
		/* PATLAK */
		tmpNaPtr = new NTBP_patlak_sodium_multi_current_o(area,
				indSodiumDensity, sodiumConductance * 1e-9 /* mSiemens per channel */
		);
		cout << "Using Patlak Na model." << endl;
		break;
	default:
		cerr
				<< "Axonsimulator-v2 - Error  : Invalid Na channel model specified. Exiting.";
		exit(NT_PARAM_OUT_OF_RANGE);
		break;
	}
	switch (sodiumAlg) {
	case 1:
		tmpNaPtr->SetSimulationMode(NTBP_DETERMINISTIC);
		cout << "Using HH Deterministic Na algorithm." << endl;
		break;
	case 2:
		tmpNaPtr->SetSimulationMode(NTBP_LANGEVIN);
		cout << "Using HH Langevin Na algorithm." << endl;
		break;
	case 3:
		tmpNaPtr->SetSimulationMode(NTBP_SINGLECHANNEL);
		cout << "Using HH Markovian Na algorithm (Singlechannel)." << endl;
		break;
	case 4:
		tmpNaPtr->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		cout << "Using HH Markovian Na algorithm (Binomialpopulation)." << endl;
		break;
	case 5:
		tmpNaPtr->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		cout << "Using HH Gillespie Na algorithm ." << endl;
		break;
	default:
		cerr
				<< "Axonsimulator-v2 - Error  : Invalid Na channel algorithm  specified. Exiting.";
		exit(NT_PARAM_OUT_OF_RANGE);
		break;
	}

	return tmpNaPtr;
}

NTBP_membrane_current_o *
NTBP_create_k_channel_ptr(NTsize potassiumModel, NTsize potassiumAlg,
		NTreal potassiumDensity, NTreal potassiumConductance, NTreal q10,
		NTreal temperature, NTreal area, NTreal potassiumReversalPotential,
		NTreal vBase) {

	NTreal indPotassiumDensity = NTBP_corrected_channel_density(
			potassiumDensity, area);
	/* K ion channel currents */
	NTBP_membrane_current_o *tmpKPtr;
	switch (potassiumModel) {
	case 1:
		/* SGA */
		tmpKPtr = new NTBP_multi_potassium_current_o(area, NTBP_SGA_SCHNEIDMAN,
				indPotassiumDensity, vBase, potassiumReversalPotential);
		cout << "Using SGA K model." << endl;
		break;
	case 2:
		/* hRanvier */
		tmpKPtr = new NTBP_hranvier_potassium_multi_current_o(area,
				indPotassiumDensity,
				potassiumConductance * 1e-9/* mSiemens per channel */, vBase,
				q10, potassiumReversalPotential);
		cout << "Using human Ranvier K model." << endl;
		break;
	case 3:
		/* COLBERT AXONAL */
		tmpKPtr = new NTBP_colbert_axonal_potassium_multi_current_o(area,
				indPotassiumDensity,
				potassiumConductance * 1e-9 /* mSiemens per channel */,
				vBase/*-64.5*/, q10, potassiumReversalPotential);
		cout << "Using Colbert-Axonal K model." << endl;
		break;
	case 4:
		/* K-MULTI */
		tmpKPtr = new NTBP_potassium_multi_current_o(area, indPotassiumDensity,
				potassiumConductance * 1e-9 /* mSiemens per channel*/,
				-63 /* mV */, 24 /* C */, -15 /* mV */
		);//CHANGE VALUES ALSO ABOVE !
		cout << "Using K-multi model." << endl;
		break;
	default:
		cerr << "Axonsimulator-v2 - Error : Invalid K channel model "
				<< potassiumModel << " specified. Exiting.";
		exit(NT_PARAM_OUT_OF_RANGE);
		break;
	}
	switch (potassiumAlg) {
	case 1:
		tmpKPtr->SetSimulationMode(NTBP_DETERMINISTIC);
		cout << "Using HH Deterministic K algorithm." << endl;
		break;
	case 2:
		tmpKPtr->SetSimulationMode(NTBP_LANGEVIN);
		cout << "Using HH Langevin K algorithm." << endl;
		break;
	case 3:
		tmpKPtr->SetSimulationMode(NTBP_SINGLECHANNEL);
		cout << "Using HH Markovian K algorithm (Singlechannel)." << endl;
		break;
	case 4:
		tmpKPtr->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		cout << "Using HH Markovian K algorithm (Binomialpopulation)." << endl;
		break;
	case 5:
		tmpKPtr->SetSimulationMode(NTBP_BINOMIALPOPULATION);
		cout << "Using HH Gillespie K algorithm ." << endl;
		break;
	default:
		cerr
				<< "Axonsimulator-v2 - Error  : Invalid K channel algorithm  specified. Exiting.";
		exit(NT_PARAM_OUT_OF_RANGE);
		break;
	}
	tmpKPtr->Set_q10(q10);

	return tmpKPtr;
}

