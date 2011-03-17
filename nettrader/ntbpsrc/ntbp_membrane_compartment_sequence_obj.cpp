/**\file ntbp_membrane_compartment_sequence_obj.cpp - NTBP_membrane_compartment_sequence_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 26.3.2001  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
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

/* $Id: ntbp_membrane_compartment_sequence_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_membrane_compartment_sequence_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.7  2003/08/24 10:43:01  face
 * *** empty log message ***
 *
 * Revision 1.6  2002/12/12 15:31:45  face
 * *** empty log message ***
 *
 * Revision 1.5  2002/03/13 19:23:27  face
 * *** empty log message ***
 *
 * Revision 1.4  2002/03/08 15:13:26  face
 * *** empty log message ***
 *
 * Revision 1.3  2002/03/04 18:09:07  face
 * added checking for NaN and Inf values in find_spikes_in_binary_file-
 * BinomialStep: redo loop if after state change the states do not sum up
 *
 * Revision 1.2  2001/10/12 09:18:36  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "ntbp_membrane_compartment_sequence_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_membrane_compartment_sequence_o */

NTBP_membrane_compartment_sequence_o::NTBP_membrane_compartment_sequence_o() :
	NTBP_membrane_o() {
	numCompartments = 0;

	lVec.resize(1);
	uVec.resize(1);
	dVec.resize(1);
	rVec.resize(1);

	initialised = false;
	swCrankNicholson = false;
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_membrane_compartment_sequence_o::NTBP_membrane_compartment_sequence_o(
		const NTBP_membrane_compartment_sequence_o & original) {
	cerr << "DO NOT COPY" << endl;
	NT_ASSERT( 1 == 0);
	// add assignment code here
}

const NTBP_membrane_compartment_sequence_o&
NTBP_membrane_compartment_sequence_o::operator=(
		const NTBP_membrane_compartment_sequence_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	cerr << "DO NOT ASSIGN" << endl;
	NT_ASSERT( 1 == 0);
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_membrane_compartment_sequence_o::~NTBP_membrane_compartment_sequence_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    no update of SOLVER dimensionality or SOVLER INIT done
 \bug        unknown
 */
NTreturn NTBP_membrane_compartment_sequence_o::PushBack(
		NTBP_cylindrical_compartment_o * compartPtr) {
	compartPtr->setTimeStep(_timeStep());
	compartmentVec.push_back(compartPtr);
	numCompartments++;

	initialised = false;

	if (compartmentVec.size() != numCompartments)
		return NT_FAIL;
	return NT_SUCCESS;
}

/** @short Execute one time step on the compartments.      
 @param      none
 @return     none
 \warning    identical axo-geometric properties required for all compartments !
 \bug
 */
NTreturn NTBP_membrane_compartment_sequence_o::Step() {
	//	cerr << "NTBP_membrane_compartment_sequence_o::Step()" << endl;
	if (true != initialised) {
		cerr
				<< "NTBP_membrane_compartment_sequence_o::Step() - Warning : Called method without Init() beeing called after instantiation or AddCompartment. Calling Init() now"
				<< endl;
		if (Init() != NT_SUCCESS) {
			cerr
					<< "NTBP_membrane_compartment_sequence_o::Step() - Error : Call to Init failed (No compartments present ?)."
					<< endl;
			return NT_FAIL;
		}
	}

	vector<NTreal> tmpVVec;
	NTreal omega = 0.0;

	/* load voltage vector and rhs-vector */
	NTsize ll = 0;
	for (ll = 0; ll < numCompartments; ll++) {
		/* omega should have units of mV : mSec nA / muF = muV */
		omega = 1e-3 /* mV/muV */* (_timeStep()
				/ compartmentVec[ll]->_compartmentMembraneCapacitance())
				* (compartmentVec[ll]->CompartmentMembraneNetCurrent());

		rVec[ll] = compartmentVec[ll]->_vM() + omega; // compute RHS of finite difference equation
		// TODO it appears to be correct, but why is omega ADDED and not subtracted ?
	}

	vector<NTreal> vVec = NumericalRecipesSolveTriDiag(lVec, dVec, uVec, rVec);
	/* set new voltage */
	for (ll = 0; ll < numCompartments; ll++) {
		cerr << vVec[ll] << "\t";
		compartmentVec[ll]->Step(vVec[ll]); // Step also advances the voltage -> ignore by using vVec
	}
cerr << endl;
	return NT_SUCCESS;
}

/** @short       
 @param      none
 @return     none
 \warning    CONSTANT AXON diameter and axoplasmic RESISTANCE required
 \bug        unknown
 */
NTreturn NTBP_membrane_compartment_sequence_o::Init() {
	if (compartmentVec.size() <= 0) {
		cerr
				<< "NTBPCompartmentMembraneNetCurrent()_membrane_compartment_sequence_o::Init - ERROR : No compartments present."
				<< endl;
		initialised = false;
		return NT_FAIL;
	}

	lVec.resize(numCompartments);
	uVec.resize(numCompartments);
	dVec.resize(numCompartments);
	rVec.resize(numCompartments);

	NTreal sigma = 0;
	NTreal radius = compartmentVec[0]->_radius(); // should be constant with present solution method
	NTreal axoplasmicR = compartmentVec[0]->_rA(); // (specific!) should be constant with present solution method
	NTreal deltaT = _timeStep();
	NTreal deltaXX = 0;

	/* initialisation of left band l and right band u "vectors" */
	//vVec[0] = 0;
	compartmentVec[0]->Set_vM(0);
	NTsize ll = 1;
	for (ll = 1; ll < numCompartments; ll++) {
		compartmentVec[ll]->Set_vM(0);
		/* testing requirement for constant axo-geometric properties */NT_ASSERT(compartmentVec[ll]->_radius() == compartmentVec[ll-1]->_radius());
		NT_ASSERT(compartmentVec[ll]->_rA() == compartmentVec[ll-1]->_rA());
	}
	compartmentVec[numCompartments - 1]->Set_vM(0);

	/* FIRST COMPARTMENT */
	deltaXX = compartmentVec[0]->_length() * compartmentVec[0]->_length();
	/* sigma is unit-free  (scaled by 0.1) [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF) */
	sigma = (0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
			* (compartmentVec[0]->_area()
					/ compartmentVec[0]->CompartmentMembraneCapacitance());

	uVec[0] = -2.0 * sigma; // vonNeumann boundary conditions
	dVec[0] = 2.0 * sigma + 1.0;

	/* INTERMEDIATE COMPARTMENTS */
	/* requirement for constant axo-geometric properties */
	for (ll = 1; ll < numCompartments - 1; ll++) {
		deltaXX = compartmentVec[ll]->_length() * compartmentVec[ll]->_length();
		/* sigma is unit-free  (scaled by 0.1)     [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF) */
		sigma = (0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
				* (compartmentVec[ll]->_area()
						/ compartmentVec[ll]->CompartmentMembraneCapacitance());
		lVec[ll] = -sigma;
		dVec[ll] = 2.0 * sigma + 1.0;
		uVec[ll] = -sigma;
	}

	/* LAST COMPARTMENT */
	deltaXX = compartmentVec[numCompartments - 1]->_length()
			* compartmentVec[numCompartments - 1]->_length();
	/* sigma is unit-free  (scaled by 0.1)     [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF) */
	sigma
			= (0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
					* (compartmentVec[numCompartments - 1]->_area()
							/ compartmentVec[numCompartments - 1]->CompartmentMembraneCapacitance());
	dVec[numCompartments - 1] = 2.0 * sigma + 1.0;
	lVec[numCompartments - 1] = -2.0 * sigma; // vonNeumann boundary conditions


	/* completed initialisation */
	initialised = true;

	return NT_SUCCESS;
}

/** @short Setup staggering PDE integration of compartments
 Internal - voltage related - states of compartments(i.e. currents) are ahead t+.5 baseTimeStep,
 while state of compartment sequence is unchanged. imposes crank nicholson staggering.
 @param      none
 @return     none
 \warning    Calling method activates Crank-Nicholson algorithm in Step()
 \bug        !unknown!
 */
NTreturn NTBP_membrane_compartment_sequence_o::InitialStep() {

	swCrankNicholson = true;
	UpdateTimeStep(_timeStep() / 2.0);
	StepNTBP();
	for (NTsize ll = 0; ll < numCompartments; ll++) {
		compartmentVec[ll]->Step(compartmentVec[ll]->_vM()); // Step also advances the voltage -> ignore by using vVec
		// TODO why
	}

	UpdateTimeStep(_timeStep() * 2.0);
	StepNTBP();
	cerr
			<< "NTBP_membrane_compartment_sequence_o::InitialStep() - ERROR : not correctly implemented ? untested."
			<< endl;
	return NT_SUCCESS;
}

vector<NTreal> NTBP_membrane_compartment_sequence_o::OpenChannels(
		NTsize currIndex) const {
	vector<NTreal> tmp(_numCompartments());
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = compartmentVec[ll]->Current(currIndex)->OpenChannels();
	}
	return tmp;
}

vector<NTreal> NTBP_membrane_compartment_sequence_o::_vVec() const {
	vector<NTreal> out;
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		out.push_back(compartmentVec[ll]->_vM());
	}
	return out;
}

vector<NTreal> NTBP_membrane_compartment_sequence_o::NumChannels(
		NTsize currIndex) const {
	vector<NTreal> tmp(_numCompartments());
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = compartmentVec[ll]->Current(currIndex)->NumChannels();
	}
	return tmp;
}

vector<NTreal> NTBP_membrane_compartment_sequence_o::OpenChannelsRatio(
		NTsize currIndex) const {
	vector<NTreal> tmp(_numCompartments());
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = compartmentVec[ll]->Current(currIndex)->OpenChannelsRatio();
	}
	return tmp;
}

NTreturn NTBP_membrane_compartment_sequence_o::WriteMembranePotential(
		ofstream & file) {
	float data[_numCompartments()];
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		data[ll] = compartmentVec[ll]->_vM();
	}
	file.write(reinterpret_cast<char*> (data),
			_numCompartments() * sizeof(float));
	return NT_SUCCESS;
}

NTreturn NTBP_membrane_compartment_sequence_o::WriteMembranePotentialASCII(
		ofstream & file, NTreal timeVar) {
	file << timeVar << " ";
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		for (NTsize j = 0; j < compartmentVec[ll]->_length(); j++)
			file << compartmentVec[ll]->_vM() << " ";
	}
	file << endl;
	return NT_SUCCESS;
}

/** @short  Write compartment currents into a binary file
 @param      reference to a file object and a current index (i.e. position of requested current in the currentVec of
 the membrane compartment). If current index == 0, the compartment net current is written.
 @return     none
 \warning    unknown
 \bug        unknown  */
NTreturn NTBP_membrane_compartment_sequence_o::WriteCurrent(ofstream & file,
		NTsize index) {
	float data[_numCompartments()];
	if (0 == index) {
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->CompartmentMembraneNetCurrent();
		}
	} else {
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->AttachedCurrent(index);
		}
	}
	file.write(reinterpret_cast<char*> (data),
			_numCompartments() * sizeof(float));
	return NT_SUCCESS;
}

NTreturn NTBP_membrane_compartment_sequence_o::WriteCurrentAscii(
		ofstream & file, NTsize index) {
	float data[_numCompartments()];
	if (0 == index) {
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			//data[ll] = compartmentVec[ll]->CompartmentMembraneNetCurrent();
			file << compartmentVec[ll]->CompartmentMembraneNetCurrent() << " ";
		}
	} else {
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			//data[ll] = compartmentVec[ll]->AttachedCurrent(index);
			file << compartmentVec[ll]->AttachedCurrent(index) << " ";
		}
	}
	file << endl;

	return NT_SUCCESS;
}

NTreturn NTBP_membrane_compartment_sequence_o::WriteOpenChannelsRatio(
		ofstream & file, NTsize index) {
	float data[_numCompartments()];
	vector<NTreal> tmp = OpenChannelsRatio(index);
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		data[ll] = tmp[ll];
	}
	file.write(reinterpret_cast<char*> (data),
			_numCompartments() * sizeof(float));
	return NT_SUCCESS;
}

/** @short  Give compartment currents
 @param      reference to a file object and a current index (i.e. position of requested current in the currentVec of
 the membrane compartment). If current index == 0, the compartment net current is written.
 @return     none
 \warning    unknown
 \bug        unknown  */
vector<NTreal> NTBP_membrane_compartment_sequence_o::GiveCurrent(NTsize index) {
	vector<NTreal> data(_numCompartments());
	if (0 == index) {
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->CompartmentMembraneNetCurrent();
		}
	} else {
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->AttachedCurrent(index);
		}
	}

	return data;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/** @short
 @param      compartmentId refers to intuitive enumeriation, i.e. [1..m]
 @return     none
 \warning    unknown
 \bug        unknown  */
NTreturn NTBP_membrane_compartment_sequence_o::InjectCurrent(
		NTreal current /* in nA */, NTsize compartmentId) {
	if ((compartmentId < 1) || (compartmentId > _numCompartments()))
		return NT_PARAM_OUT_OF_RANGE;
	return compartmentVec[compartmentId - 1]->InjectCurrent(current);
}

/**
 @param      compartmentId refers to intuitive enumeriation, i.e. [1..m]
 @return     none
 \warning    unknown
 \bug        unknown
 */
void NTBP_membrane_compartment_sequence_o::ShowHinesMatrix() {
	using namespace TNT;
	NTsize n = _numCompartments();

	Matrix<NTreal> hinesMtr(n, n + 2);

	/* reconstruct tridiagonal matrix from band vectors */
	hinesMtr[0][0] = dVec[0];
	hinesMtr[0][1] = uVec[0];

	NTsize ll = 0;
	for (ll = 1; ll < _numCompartments() - 1; ll++) {
		hinesMtr[ll][ll - 1] = lVec[ll];
		hinesMtr[ll][ll] = dVec[ll];
		hinesMtr[ll][ll + 1] = uVec[ll];
	}

	hinesMtr[n - 1][n - 2] = lVec[n - 1];
	hinesMtr[n - 1][n - 1] = dVec[n - 1];

	for (ll = 0; ll < _numCompartments() - 1; ll++) {
		hinesMtr[ll][n] = compartmentVec[ll]->_vM();
		hinesMtr[ll][n + 1] = rVec[ll];
	}

	cerr << hinesMtr;

}

vector<NTreal> NTBP_membrane_compartment_sequence_o::ZadorPearlmutterSolveTriDiag(
		vector<NTreal> lNewVec, vector<NTreal> dNewVec, vector<NTreal> uNewVec,
		vector<NTreal> rNewVec) const {
	const NTsize n = lNewVec.size();
	vector<NTreal> vNewVec(n);
	NT_ASSERT( (lNewVec.size() == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()) );

	vector<NTreal> Kl(n);
	vector<NTreal> Kd(n);
	vector<NTreal> Ku(n);

	vector<NTreal> BJd(n);

	NTsize ll = 0;
	for (ll = 0; ll < n; ll++) {
		BJd[ll] = 1;
	}
	for (ll = 0; ll < n - 1; ll++) {
		BJd[ll + 1] -= uNewVec[ll] * lNewVec[ll] / BJd[ll];
	}

	Kd[n - 1] = 1 / BJd[n - 1];

	NTint j = 0;
	for (j = n - 2; j > 0; j--) {
		Kd[j] = 1 / BJd[j] + (Kd[j + 1] * lNewVec[j] * uNewVec[j]) / (BJd[j]
				* BJd[j]);
		Ku[j] = -Kd[j + 1] * uNewVec[j] / BJd[j];
		Kl[j] = -Kd[j + 1] * lNewVec[j] / BJd[j];
	}

	for (ll = 1; ll < n - 1; ll++) {
		vNewVec[ll] = Kl[ll] * vNewVec[ll - 1] + Kd[ll] * rNewVec[ll] + Ku[ll]
				* vNewVec[ll + 1];
	}

	return vNewVec;
}

vector<NTreal> NTBP_membrane_compartment_sequence_o::MascagniSolveTriDiag(
		vector<NTreal> lNewVec, vector<NTreal> dNewVec, vector<NTreal> uNewVec,
		vector<NTreal> rNewVec) const {
	NTsize m = lNewVec.size();
	vector<NTreal> vNewVec(m);
	NT_ASSERT( (lNewVec.size() == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()) );

	/*	Solution of tridiagonal system */
	/* forward elimination */
	lNewVec[0] = 0;
	dNewVec[0] = dNewVec[0];
	uNewVec[0] = uNewVec[0] / dNewVec[0];
	rNewVec[0] = rNewVec[0] / dNewVec[0];
	NTsize i = 1;
	for (i = 1; i < m - 1; i++) {
		dNewVec[i] = dNewVec[i] - lNewVec[i] * vNewVec[i - 1];
		rNewVec[i] = (rNewVec[i] - lNewVec[i] * vNewVec[i - 1]) / dNewVec[i];
		uNewVec[i] = uNewVec[i] / dNewVec[i];
		cout << i << " " << lNewVec[i] << " " << dNewVec[i] << " "
				<< uNewVec[i] << " " << vNewVec[i] << " " << rNewVec[i] << endl;
	}
	dNewVec[m - 1] = dNewVec[m - 1] - lNewVec[m - 1] * vNewVec[m - 1 - 1];
	rNewVec[m - 1] = (rNewVec[m - 1] - lNewVec[m - 1] * vNewVec[m - 1 - 1])
			/ dNewVec[m - 1];
	uNewVec[m - 1] = 0;

	/* backward substitution */
	vNewVec[m - 1] = rNewVec[m - 1];
	NTint ll = 0; // INT as NTsize cannot be compared to a negative number
	for (ll = m - 2; ll > -1; ll--) {
		vNewVec[ll] = rNewVec[ll] - uNewVec[ll] * vNewVec[ll + 1];
		cout << ll << " " << lNewVec[ll] << " " << dNewVec[ll] << " "
				<< uNewVec[ll] << " " << vNewVec[ll] << " " << rNewVec[ll]
				<< endl;
	}
	return vNewVec;
}

/**  Compute sum of escape rates over current state in [kHz] */
NTreal NTBP_membrane_compartment_sequence_o::CompartmentSequenceChannelStateTimeConstant() const {
	cerr
			<< "NTBP_membrane_compartment_sequence_o::CompartmentSequenceChannelStateTimeConstant()"
			<< endl;
	NTreal sum = 0.0;
	for (NTsize ll = 0; ll < numCompartments; ll++) {
		//		cout <<"NTBP_membrane_compartment_sequence_o::CompartmentSequenceChannelStateTimeConstant  SEQ" << endl;
		sum += compartmentVec[ll]->CompartmentChannelStateTimeConstant();
		//		cout << "SEQ " << endl;
	}
	return sum;
}

/**  */
vector<NTreal> NTBP_membrane_compartment_sequence_o::NumericalRecipesSolveTriDiag(
		const vector<NTreal> & lNewVec, const vector<NTreal> & dNewVec,
		const vector<NTreal> & uNewVec, const vector<NTreal> & rNewVec) const {
	NTsize n = lNewVec.size();
	vector<NTreal> vNewVec(n);
	NT_ASSERT( (n == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()) );
	NTreal bet;
	vector<NTreal> gam(n);

	// a is l	
	// b is d
	// c is u
	// u is v
	// r is r

	vNewVec[0] = rNewVec[0] / (bet = dNewVec[0]);
	NTsize j = 0;
	for (j = 1; j < n; j++) {
		gam[j] = uNewVec[j - 1] / bet;
		bet = dNewVec[j] - lNewVec[j] * gam[j];
		if (0 == bet)
			cerr
					<< "NTBP_membrane_compartment_sequence_o::NumericalRecipesSolveTriDiag - solver failed, zero pivot element ? is the matrix diagonally domininat ?)."
					<< endl;
		vNewVec[j] = (rNewVec[j] - lNewVec[j] * vNewVec[j - 1]) / bet;
	}
	NTint i = 0;
	for (i = (n - 2); i > -1; i--) {
		vNewVec[i] -= gam[i + 1] * vNewVec[i + 1];
	}

	return vNewVec;
}

/**  */
bool NTBP_membrane_compartment_sequence_o::GillespieStep() {
	cerr << "NTBP_membrane_compartment_sequence_o::GillespieStep()" << endl;
	vector<NTreal> compartmentTauVec(_numCompartments());
	NT_uniform_rnd_dist_o rnd;
	NTreal val;
	NTreal sum;
	NTreal sequenceTau;
	bool integrateStep = false;
	NTreal newDeltaT;
	NTreal maxDeltaT;

	NTreal tStar = 0.0;

	do {
		/** BLOCK 2 */
		cerr << "GILLESPIE STEP" << endl;
		sequenceTau = CompartmentSequenceChannelStateTimeConstant();
		sum = 0.0;
		val = rnd.RndVal();
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			sum += compartmentVec[ll]->CompartmentChannelStateTimeConstant();
			if (val < sum / sequenceTau) {
				cerr << "STEPING COMPARTMENT " << ll << endl;
				integrateStep = compartmentVec[ll]->GillespieStep();
				break;
			}
		}
		/** BLOCK 2 */
		/** BLOCK 1 */
		// this is a sum of rate constants !
		newDeltaT = log(1 / rnd.RndVal()) / sequenceTau; //sequenceTau in [kHz] while newDeltaT in [ms]
		cerr << "NEW DELTA T=" << newDeltaT << endl;
		maxDeltaT = 1; // maximumTimeStep ought to be 1 ms
		if (newDeltaT > maxDeltaT) {
			newDeltaT = maxDeltaT;
		}
		UpdateTimeStep(newDeltaT);
		StepNTBP();
		tStar += newDeltaT;
		/** BLOCK 1 */
	} while (integrateStep == false);

	cerr << "INTEGRATOR STEP WITH T_STAR=" << tStar << endl;
	UpdateTimeStep(tStar);
	StepNTBP();
	Step();
	ShowVoltage();

	return NT_SUCCESS;
}

/** @short  Write number of ATPs consumed into a file
 @param      reference to a file object.
 @return     none
 \warning    unknown
 \bug        unknown  */
NTreturn NTBP_membrane_compartment_sequence_o::WriteATP(ofstream & file) {
	//float data[_numCompartments()];
	for (NTsize ll = 0; ll < _numCompartments(); ll++) {
		file << -compartmentVec[ll]->AttachedCurrent(2)
		/** _timeStep()
		 * 6241510000 / 3 */<< " ";
	}
	file << endl;
	return NT_SUCCESS;
}
