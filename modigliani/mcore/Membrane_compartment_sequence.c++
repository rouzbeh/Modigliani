/**\file Membrane_compartment_sequence.cpp - Membrane_compartment_sequence class implementation
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

#include "Membrane_compartment_sequence.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Membrane_compartment_sequence */

Membrane_compartment_sequence::Membrane_compartment_sequence() :
		Membrane() {
	numCompartments = 0;

	lVec.resize(1);
	uVec.resize(1);
	dVec.resize(1);
	rVec.resize(1);

	initialised = false;
	swCrankNicholson = false;
}

/* ***      DESTRUCTOR		***/
Membrane_compartment_sequence::~Membrane_compartment_sequence() {
	for (auto it = compartmentVec.begin(); it!=compartmentVec.end(); it++){
		delete *it;
	}
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    no update of SOLVER dimensionality or SOVLER INIT done
 \bug        unknown
 */
mbase::Mreturn Membrane_compartment_sequence::PushBack(
		Cylindrical_compartment * compartPtr) {
	compartPtr->setTimeStep(_timeStep());
	compartmentVec.push_back(compartPtr);
	numCompartments++;

	initialised = false;

	if (compartmentVec.size() != numCompartments)
		return (mbase::M_FAIL);
	return (mbase::M_SUCCESS);
}

/** @short Execute one time step on the compartments.      
 @param      none
 @return     none
 \warning    identical axo-geometric properties required for all compartments !
 \bug
 */
mbase::Mreturn Membrane_compartment_sequence::step() {
	//	std::cerr << "Membrane_compartment_sequence::Step()" << std::endl;
	if (true != initialised) {
		std::cerr
				<< "Membrane_compartment_sequence::Step() - Warning : Called method without Init() beeing called after instantiation or AddCompartment. Calling Init() now"
				<< std::endl;
		if (Init() != mbase::M_SUCCESS) {
			std::cerr
					<< "Membrane_compartment_sequence::Step() - Error : Call to Init failed (No compartments present ?)."
					<< std::endl;
			return (mbase::M_FAIL);
		}
	}

	std::vector<mbase::Real> tmpVVec;
	mbase::Real omega = 0.0;
	mbase::Msize ll = 0;

	/* load voltage std::vector and rhs-std::vector */

	for (ll = 0; ll < numCompartments; ll++) {
		/* omega should have units of mV : mSec nA / muF = muV */
		omega = 1e-3 /* mV/muV */
		* (_timeStep() / compartmentVec[ll]->_compartmentMembraneCapacitance())
				* (compartmentVec[ll]->CompartmentMembraneNetCurrent());
		rVec[ll] = compartmentVec[ll]->_vM() + omega; // compute RHS of finite difference equation
		// TODO it appears to be correct, but why is omega ADDED and not subtracted ?
	}

	std::vector<mbase::Real> vVec = NumericalRecipesSolveTriDiag(lVec, dVec, uVec, rVec);
	/* set new voltage */

	for (ll = 0; ll < numCompartments; ll++) {
		compartmentVec[ll]->step(vVec[ll]); // Step also advances the voltage -> ignore by using vVec
	}
	return (mbase::M_SUCCESS);
}

/** @short       
 @param      none
 @return     none
 \warning    CONSTANT AXON diameter and axoplasmic RESISTANCE required
 \bug        unknown
 */
mbase::Mreturn Membrane_compartment_sequence::Init() {
	if (compartmentVec.size() <= 0) {
		std::cerr
				<< "NTBPCompartmentMembraneNetCurrent()_membrane_compartment_sequence_o::Init - ERROR : No compartments present."
				<< std::endl;
		initialised = false;
		return (mbase::M_FAIL);
	}

	lVec.resize(numCompartments);
	uVec.resize(numCompartments);
	dVec.resize(numCompartments);
	rVec.resize(numCompartments);

	mbase::Real sigma = 0;
	mbase::Real radius = compartmentVec[0]->_radius(); // should be constant with present solution method
	mbase::Real axoplasmicR = compartmentVec[0]->_rA(); // (specific!) should be constant with present solution method
	mbase::Real deltaT = _timeStep();
	mbase::Real deltaXX = 0;

	/* initialisation of left band l and right band u "std::vectors" */
	//vVec[0] = 0;
	compartmentVec[0]->Set_vM(0);
	mbase::Msize ll = 1;
	for (ll = 1; ll < numCompartments; ll++) {
		compartmentVec[ll]->Set_vM(0);
		/* testing requirement for constant axo-geometric properties */
		M_ASSERT(
				compartmentVec[ll]->_radius() == compartmentVec[ll-1]->_radius());
		M_ASSERT(compartmentVec[ll]->_rA() == compartmentVec[ll-1]->_rA());
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
	sigma =
			(0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
					* (compartmentVec[numCompartments - 1]->_area()
							/ compartmentVec[numCompartments - 1]->CompartmentMembraneCapacitance());
	dVec[numCompartments - 1] = 2.0 * sigma + 1.0;
	lVec[numCompartments - 1] = -2.0 * sigma; // vonNeumann boundary conditions

	/* completed initialisation */
	initialised = true;

	return (mbase::M_SUCCESS);
}

/** @short Setup staggering PDE integration of compartments
 Internal - voltage related - states of compartments(i.e. currents) are ahead t+.5 baseTimeStep,
 while state of compartment sequence is unchanged. imposes crank nicholson staggering.
 @param      none
 @return     none
 \warning    Calling method activates Crank-Nicholson algorithm in Step()
 \bug        !unknown!
 */
mbase::Mreturn Membrane_compartment_sequence::InitialStep() {

	swCrankNicholson = true;
	update_timeStep(_timeStep() / 2.0);
	StepNTBP();
	for (mbase::Msize ll = 0; ll < numCompartments; ll++) {
		compartmentVec[ll]->step(compartmentVec[ll]->_vM()); // Step also advances the voltage -> ignore by using vVec
		// TODO why
	}

	update_timeStep(_timeStep() * 2.0);
	StepNTBP();
	std::cerr
			<< "Membrane_compartment_sequence::InitialStep() - ERROR : not correctly implemented ? untested."
			<< std::endl;
	return (mbase::M_SUCCESS);
}

std::vector<mbase::Real> Membrane_compartment_sequence::open_channels(
		mbase::Msize currIndex) const {
	std::vector<mbase::Real> tmp(_numCompartments());
	for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = compartmentVec[ll]->Current(currIndex)->open_channels();
	}
	return (tmp);
}

std::vector<mbase::Real> Membrane_compartment_sequence::_vVec() const {
	std::vector<mbase::Real> out;
	for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
		out.push_back(compartmentVec[ll]->_vM());
	}
	return (out);
}

std::vector<mbase::Real> Membrane_compartment_sequence::NumChannels(
		mbase::Msize currIndex) const {
	std::vector<mbase::Real> tmp(_numCompartments());
	for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = compartmentVec[ll]->Current(currIndex)->NumChannels();
	}
	return (tmp);
}

std::vector<mbase::Real> Membrane_compartment_sequence::NumChannelsInState(
		mbase::Msize currIndex, mbase::Msize state) const {
	std::vector<mbase::Real> tmp(_numCompartments());
	for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = compartmentVec[ll]->Current(currIndex)->num_channels_in_state(
				state);
	}
	return (tmp);
}

std::vector<mbase::Real> Membrane_compartment_sequence::OpenChannelsRatio(
		mbase::Msize currIndex) const {
	std::vector<mbase::Real> tmp(_numCompartments());
	for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
		if (currIndex - 1 < compartmentVec[ll]->currentVec.size())
			tmp[ll] =
					compartmentVec[ll]->Current(currIndex)->OpenChannelsRatio();
		else
			tmp[ll] = 0;
	}
	return (tmp);
}

mbase::Mreturn Membrane_compartment_sequence::WriteMembranePotentialASCII(
		std::ostream & file) const {
	for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
		//for (mbase::Msize i =0; i<compartmentVec[ll]->_length(); i++)
		file << compartmentVec[ll]->_vM() << " ";
	}
	file << std::endl;
	return (mbase::M_SUCCESS);
}

/** @short  Write compartment currents into a ascii file
 @param      reference to a file object and a current index (i.e. position of requested current in the currentVec of
 the membrane compartment). If current index == 0, the compartment net current is written.
 @return     none
 \warning    unknown
 \bug        unknown  */
mbase::Mreturn Membrane_compartment_sequence::WriteCurrentAscii(std::ostream & file,
		mbase::Msize index) const {
	if (0 == index) {
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			file << compartmentVec[ll]->CompartmentMembraneNetCurrent() << " ";
		}
	} else {
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			if (index - 1 < compartmentVec[ll]->currentVec.size())
				file << compartmentVec[ll]->AttachedCurrent(index) << " ";
		}
	}
	file << std::endl;

	return (mbase::M_SUCCESS);
}

mbase::Mreturn Membrane_compartment_sequence::WriteMembranePotential(
		std::ostream & file) const {
	float data[numCompartments];
	for (mbase::Msize ll = 0; ll < numCompartments; ll++) {
		data[ll] = compartmentVec[ll]->_vM();
	}
	file.write(reinterpret_cast<char*>(data), numCompartments * sizeof(float));
	return (mbase::M_SUCCESS);
}

mbase::Mreturn Membrane_compartment_sequence::WriteCompartmentData(
		std::ostream* file, mbase::Msize to_print) const {
	mbase::Msize number_of_currents = compartmentVec[to_print]->currentVec.size();
	float data[1 + number_of_currents];
	data[0] = compartmentVec[to_print]->_vM();
	for (mbase::Msize ll = 1; ll - 1 < number_of_currents; ++ll) {
		data[ll] = compartmentVec[to_print]->AttachedCurrent(ll);
	}
	file->write(reinterpret_cast<char*>(data),
			(1 + number_of_currents) * sizeof(float));
	return (mbase::M_SUCCESS);
}

/** @short  Write compartment currents into a binary file
 @param      reference to a file object and a current index (i.e. position of requested current in the currentVec of
 the membrane compartment). If current index == 0, the compartment net current is written.
 @return     none
 \warning    unknown
 \bug        unknown  */
mbase::Mreturn Membrane_compartment_sequence::WriteCurrent(std::ostream & file,
		mbase::Msize index) const {
	if (0 == index) {
		float data[numCompartments];
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->CompartmentMembraneNetCurrent();
		}
		file.write(reinterpret_cast<char*>(data),
				_numCompartments() * sizeof(float));
	} else {
		float data[numCompartments];
		for (mbase::Msize ll = 0; ll < numCompartments; ll++) {
			if (index - 1 < compartmentVec[ll]->currentVec.size())
				data[ll] = compartmentVec[ll]->AttachedCurrent(index);
		}
		file.write(reinterpret_cast<char*>(data),
				_numCompartments() * sizeof(float));
	}
	return (mbase::M_SUCCESS);
}

/** @short  Give compartment currents
 @param      reference to a file object and a current index (i.e. position of requested current in the currentVec of
 the membrane compartment). If current index == 0, the compartment net current is written.
 @return     none
 \warning    unknown
 \bug        unknown  */
std::vector<mbase::Real> Membrane_compartment_sequence::GiveCurrent(mbase::Msize index) {
	std::vector<mbase::Real> data(_numCompartments());
	if (0 == index) {
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->CompartmentMembraneNetCurrent();
		}
	} else {
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			data[ll] = compartmentVec[ll]->AttachedCurrent(index);
		}
	}

	return (data);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/** @short
 @param      compartmentId refers to intuitive enumeriation, i.e. [1..m]
 @return     none
 \warning    unknown
 \bug        unknown  */
mbase::Mreturn Membrane_compartment_sequence::InjectCurrent(
		mbase::Real current /* in nA */, mbase::Msize compartmentId) {
	if ((compartmentId < 1) || (compartmentId > _numCompartments()))
		return (mbase::M_PARAM_OUT_OF_RANGE);
	return (compartmentVec[compartmentId - 1]->InjectCurrent(current));
}

/**
 @param      compartmentId refers to intuitive enumeriation, i.e. [1..m]
 @return     none
 \warning    unknown
 \bug        unknown
 */
void Membrane_compartment_sequence::ShowHinesMatrix() {
	using namespace TNT;
	mbase::Msize n = _numCompartments();

	Matrix<mbase::Real> hinesMtr(n, n + 2);

	/* reconstruct tridiagonal matrix from band std::vectors */
	hinesMtr[0][0] = dVec[0];
	hinesMtr[0][1] = uVec[0];

	mbase::Msize ll = 0;
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

	std::cerr << hinesMtr;

}

// NOT WORKING
std::vector<mbase::Real> Membrane_compartment_sequence::ZadorPearlmutterSolveTriDiag(
		std::vector<mbase::Real> lNewVec, std::vector<mbase::Real> dNewVec, std::vector<mbase::Real> uNewVec,
		std::vector<mbase::Real> rNewVec) const {
	const mbase::Msize n = lNewVec.size();
	std::vector<mbase::Real> vNewVec(n);
	M_ASSERT(
			(lNewVec.size() == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()));

	std::vector<mbase::Real> Kl(n);
	std::vector<mbase::Real> Kd(n);
	std::vector<mbase::Real> Ku(n);

	std::vector<mbase::Real> BJd(n);

	mbase::Msize ll = 0;
	for (ll = 0; ll < n; ll++) {
		BJd[ll] = 1;
	}
	for (ll = 0; ll < n - 1; ll++) {
		BJd[ll + 1] -= uNewVec[ll] * lNewVec[ll] / BJd[ll];
	}

	Kd[n - 1] = 1 / BJd[n - 1];

	mbase::Mint j = 0;
	for (j = n - 2; j > 0; j--) {
		Kd[j] = 1 / BJd[j]
				+ (Kd[j + 1] * lNewVec[j] * uNewVec[j]) / (BJd[j] * BJd[j]);
		Ku[j] = -Kd[j + 1] * uNewVec[j] / BJd[j];
		Kl[j] = -Kd[j + 1] * lNewVec[j] / BJd[j];
	}

	for (ll = 1; ll < n - 1; ll++) {
		vNewVec[ll] = Kl[ll] * vNewVec[ll - 1] + Kd[ll] * rNewVec[ll]
				+ Ku[ll] * vNewVec[ll + 1];
	}

	return (vNewVec);
}

// NOT WORKING
std::vector<mbase::Real> Membrane_compartment_sequence::MascagniSolveTriDiag(
		std::vector<mbase::Real> lNewVec, std::vector<mbase::Real> dNewVec, std::vector<mbase::Real> uNewVec,
		std::vector<mbase::Real> rNewVec) const {
	mbase::Msize m = lNewVec.size();
	std::vector<mbase::Real> vNewVec(m);
	M_ASSERT(
			(lNewVec.size() == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()));

	/*	Solution of tridiagonal system */
	/* forward elimination */
	lNewVec[0] = 0;
	//dNewVec[0] = dNewVec[0];
	uNewVec[0] = uNewVec[0] / dNewVec[0];
	rNewVec[0] = rNewVec[0] / dNewVec[0];
	mbase::Msize i = 1;
	for (i = 1; i < m - 1; i++) {
		dNewVec[i] = dNewVec[i] - lNewVec[i] * vNewVec[i - 1];
		rNewVec[i] = (rNewVec[i] - lNewVec[i] * vNewVec[i - 1]) / dNewVec[i];
		uNewVec[i] = uNewVec[i] / dNewVec[i];
		std::cout << i << " " << lNewVec[i] << " " << dNewVec[i] << " " << uNewVec[i]
				<< " " << vNewVec[i] << " " << rNewVec[i] << std::endl;
	}
	dNewVec[m - 1] = dNewVec[m - 1] - lNewVec[m - 1] * vNewVec[m - 1 - 1];
	rNewVec[m - 1] = (rNewVec[m - 1] - lNewVec[m - 1] * vNewVec[m - 1 - 1])
			/ dNewVec[m - 1];
	uNewVec[m - 1] = 0;

	/* backward substitution */
	vNewVec[m - 1] = rNewVec[m - 1];
	mbase::Mint ll = 0; // INT as mbase::Msize cannot be compared to a negative number
	for (ll = m - 2; ll > -1; ll--) {
		vNewVec[ll] = rNewVec[ll] - uNewVec[ll] * vNewVec[ll + 1];
		std::cout << ll << " " << lNewVec[ll] << " " << dNewVec[ll] << " "
				<< uNewVec[ll] << " " << vNewVec[ll] << " " << rNewVec[ll]
				<< std::endl;
	}
	return (vNewVec);
}

/**  Compute sum of escape rates over current state in [kHz] */
mbase::Real Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant() const {
	std::cerr
			<< "Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant()"
			<< std::endl;
	mbase::Real sum = 0.0;
	for (mbase::Msize ll = 0; ll < numCompartments; ll++) {
		//		cout <<"Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant  SEQ" << std::endl;
		sum += compartmentVec[ll]->CompartmentChannelStateTimeConstant();
		//		cout << "SEQ " << std::endl;
	}
	return (sum);
}

/**  */
std::vector<mbase::Real> Membrane_compartment_sequence::NumericalRecipesSolveTriDiag(
		const std::vector<mbase::Real> & lNewVec, const std::vector<mbase::Real> & dNewVec,
		const std::vector<mbase::Real> & uNewVec, const std::vector<mbase::Real> & rNewVec) const {
	mbase::Msize n = lNewVec.size();
	std::vector<mbase::Real> vNewVec(n);
	M_ASSERT(
			(n == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()));
	mbase::Real bet;
	std::vector<mbase::Real> gam(n);

	// a is l
	// b is d
	// c is u
	// u is v
	// r is r

	vNewVec[0] = rNewVec[0] / (bet = dNewVec[0]);
	mbase::Msize j = 0;
	for (j = 1; j < n; j++) {
		gam[j] = uNewVec[j - 1] / bet;
		bet = dNewVec[j] - lNewVec[j] * gam[j];
		if (0 == bet)
			std::cerr
					<< "Membrane_compartment_sequence::NumericalRecipesSolveTriDiag - solver failed, zero pivot element ? is the matrix diagonally domininat ?)."
					<< std::endl;
		vNewVec[j] = (rNewVec[j] - lNewVec[j] * vNewVec[j - 1]) / bet;
	}
	mbase::Mint i = 0;
	for (i = (n - 2); i > -1; i--) {
		vNewVec[i] -= gam[i + 1] * vNewVec[i + 1];
	}

	return (vNewVec);
}

/**  */
bool Membrane_compartment_sequence::GillespieStep() {
	std::cerr << "Membrane_compartment_sequence::GillespieStep()" << std::endl;
	std::vector<mbase::Real> compartmentTauVec(_numCompartments());
	mbase::Uniform_rnd_dist rnd;
	mbase::Real val;
	mbase::Real sum;
	mbase::Real sequenceTau;
	bool integrateStep = false;
	mbase::Real newDeltaT;
	mbase::Real maxDeltaT;

	mbase::Real tStar = 0.0;

	do {
		/** BLOCK 2 */
		std::cerr << "GILLESPIE STEP" << std::endl;
		sequenceTau = CompartmentSequenceChannelStateTimeConstant();
		sum = 0.0;
		val = rnd.RndVal();
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			sum += compartmentVec[ll]->CompartmentChannelStateTimeConstant();
			if (val < sum / sequenceTau) {
				std::cerr << "STEPING COMPARTMENT " << ll << std::endl;
				integrateStep = compartmentVec[ll]->GillespieStep();
				break;
			}
		}
		/** BLOCK 2 */
		/** BLOCK 1 */
		// this is a sum of rate constants !
		newDeltaT = log(1 / rnd.RndVal()) / sequenceTau; //sequenceTau in [kHz] while newDeltaT in [ms]
		std::cerr << "NEW DELTA T=" << newDeltaT << std::endl;
		maxDeltaT = 1; // maximumTimeStep ought to be 1 ms
		if (newDeltaT > maxDeltaT) {
			newDeltaT = maxDeltaT;
		}
		update_timeStep(newDeltaT);
		StepNTBP();
		tStar += newDeltaT;
		/** BLOCK 1 */
	} while (integrateStep == false);

	std::cerr << "INTEGRATOR STEP WITH T_STAR=" << tStar << std::endl;
	update_timeStep(tStar);
	StepNTBP();
	step();
	ShowVoltage();

	return (mbase::M_SUCCESS);
}

//returns copy of compartment std::vector
Cylindrical_compartment* Membrane_compartment_sequence::ReturnCompartmentVec(
		mbase::Msize index) //TODO: added
		{
	Cylindrical_compartment* compVec = compartmentVec[index];
	return (compVec);
}

