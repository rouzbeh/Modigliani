/**
 * @file membrane_compartment_sequence.cc
 * Membrane_compartment_sequence class implementation
 * @author Ahmed Aldo Faisal &copy; created 26.3.2001  
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * @section LICENSE
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

#include "membrane_compartment_sequence.h"

using namespace modigliani_core;

/* ***      CONSTRUCTORS	***/
Membrane_compartment_sequence::Membrane_compartment_sequence() :
		Membrane(), initialised(false),swCrankNicholson(false) {
	numCompartments = 0;

	lVec.resize(1);
	uVec.resize(1);
	dVec.resize(1);
	rVec.resize(1);

}

/* ***      DESTRUCTOR		***/
Membrane_compartment_sequence::~Membrane_compartment_sequence() {
	for (auto it = compartmentVec.begin(); it != compartmentVec.end(); it++) {
		delete *it;
	}
}

/**
 * \brief Adds a compartment to the axon.
 *
 * \param      compartPtr Pointer to the compartment to add
 * \return     none
 * \warning    no update of SOLVER dimensionality or SOVLER INIT done
 */
modigliani_base::ReturnEnum Membrane_compartment_sequence::PushBack(
		Cylindrical_compartment * compartPtr) {
	compartPtr->setTimeStep(timeStep());
	compartmentVec.push_back(compartPtr);
	numCompartments++;

	initialised = false;

	if (compartmentVec.size() != numCompartments)
		return (modigliani_base::ReturnEnum::FAIL);
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/** \brief Execute one time step on the compartments.
 *  \warning    identical axo-geometric properties required for all compartments !
 */
modigliani_base::ReturnEnum Membrane_compartment_sequence::step() {
	//	std::cerr << "Membrane_compartment_sequence::Step()" << std::endl;
	if (true != initialised) {
		std::cerr
				<< "Membrane_compartment_sequence::Step() - Warning : Called method without Init() beeing called after instantiation or AddCompartment. Calling Init() now"
				<< std::endl;
		if (Init() != modigliani_base::ReturnEnum::SUCCESS) {
			std::cerr
					<< "Membrane_compartment_sequence::Step() - Error : Call to Init failed (No compartments present ?)."
					<< std::endl;
			return (modigliani_base::ReturnEnum::FAIL);
		}
	}

	std::vector<modigliani_base::Real> tmpVVec;
	modigliani_base::Real omega = 0.0;
	modigliani_base::Size ll = 0;

	/* load voltage std::vector and rhs-std::vector */

	for (ll = 0; ll < numCompartments; ll++) {
		/* omega should have units of mV : mSec nA / muF = muV */
		omega = 1e-3 /* mV/muV */
		* (timeStep() / compartmentVec[ll]->CompartmentMembraneCapacitance())
				* (compartmentVec[ll]->CompartmentMembraneNetCurrent());
		rVec[ll] = compartmentVec[ll]->vm() + omega; // compute RHS of finite difference equation
		// TODO it appears to be correct, but why is omega ADDED and not subtracted ?
	}

	std::vector<modigliani_base::Real> vVec = NumericalRecipesSolveTriDiag(lVec, dVec,
			uVec, rVec);
	/* set new voltage */

	for (ll = 0; ll < numCompartments; ll++) {
		compartmentVec[ll]->Step(vVec[ll]); // Step also advances the voltage -> ignore by using vVec
	}
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/** @short       
 \warning    CONSTANT AXON diameter and axoplasmic RESISTANCE required
 */
modigliani_base::ReturnEnum Membrane_compartment_sequence::Init() {
	if (compartmentVec.size() <= 0) {
		std::cerr
				<< "NTBPCompartmentMembraneNetCurrent()_membrane_compartment_sequence_o::Init - ERROR : No compartments present."
				<< std::endl;
		initialised = false;
		return (modigliani_base::ReturnEnum::FAIL);
	}

	lVec.resize(numCompartments);
	uVec.resize(numCompartments);
	dVec.resize(numCompartments);
	rVec.resize(numCompartments);

	modigliani_base::Real sigma = 0;
	modigliani_base::Real radius = compartmentVec[0]->radius(); // should be constant with present solution method
	modigliani_base::Real axoplasmicR = compartmentVec[0]->ra(); // (specific!) should be constant with present solution method
	modigliani_base::Real deltaT = timeStep();
	modigliani_base::Real deltaXX = 0;

	/* initialisation of left band l and right band u "std::vectors" */
	//vVec[0] = 0;
	compartmentVec[0]->set_vm(0);
	modigliani_base::Size ll = 1;
	for (ll = 1; ll < numCompartments; ll++) {
		compartmentVec[ll]->set_vm(0);
		/* testing requirement for constant axo-geometric properties */
		M_ASSERT(
				compartmentVec[ll]->radius() == compartmentVec[ll-1]->radius());
		M_ASSERT(compartmentVec[ll]->ra() == compartmentVec[ll-1]->ra());
	}
	compartmentVec[numCompartments - 1]->set_vm(0);

	/* FIRST COMPARTMENT */
	deltaXX = compartmentVec[0]->length() * compartmentVec[0]->length();
	/* sigma is unit-free  (scaled by 0.1) [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF) */
	sigma = (0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
			* (compartmentVec[0]->area()
					/ compartmentVec[0]->CompartmentMembraneCapacitance());

	uVec[0] = -2.0 * sigma; // vonNeumann boundary conditions
	dVec[0] = 2.0 * sigma + 1.0;

	/* INTERMEDIATE COMPARTMENTS */
	/* requirement for constant axo-geometric properties */
	for (ll = 1; ll < numCompartments - 1; ll++) {
		deltaXX = compartmentVec[ll]->length() * compartmentVec[ll]->length();
		/* sigma is unit-free  (scaled by 0.1)     [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF) */
		sigma = (0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
				* (compartmentVec[ll]->area()
						/ compartmentVec[ll]->CompartmentMembraneCapacitance());
		lVec[ll] = -sigma;
		dVec[ll] = 2.0 * sigma + 1.0;
		uVec[ll] = -sigma;
	}

	/* LAST COMPARTMENT */
	deltaXX = compartmentVec[numCompartments - 1]->length()
			* compartmentVec[numCompartments - 1]->length();
	/* sigma is unit-free  (scaled by 0.1)     [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF) */
	sigma =
			(0.1 / 2.0) * (deltaT / deltaXX) * (radius / axoplasmicR)
					* (compartmentVec[numCompartments - 1]->area()
							/ compartmentVec[numCompartments - 1]->CompartmentMembraneCapacitance());
	dVec[numCompartments - 1] = 2.0 * sigma + 1.0;
	lVec[numCompartments - 1] = -2.0 * sigma; // vonNeumann boundary conditions

	/* completed initialisation */
	initialised = true;

	return (modigliani_base::ReturnEnum::SUCCESS);
}

/**
 * \brief Setup staggering PDE integration of compartments
 * Internal - voltage related - states of compartments(i.e. currents)
 * are ahead t+.5 baseTimeStep, while state of compartment sequence
 * is unchanged. imposes crank nicholson staggering.
 *
 * \warning    Calling method activates Crank-Nicholson algorithm in Step()
 */
modigliani_base::ReturnEnum Membrane_compartment_sequence::InitialStep() {

	swCrankNicholson = true;
	update_timeStep(timeStep() / 2.0);
	StepNTBP();
	for (modigliani_base::Size ll = 0; ll < numCompartments; ll++) {
		compartmentVec[ll]->Step(compartmentVec[ll]->vm());
	}

	update_timeStep(timeStep() * 2.0);
	StepNTBP();
	std::cerr
			<< "Membrane_compartment_sequence::InitialStep() - ERROR : not correctly implemented ? untested."
			<< std::endl;
	return (modigliani_base::ReturnEnum::SUCCESS);
}

std::vector<modigliani_base::Real> Membrane_compartment_sequence::open_channels(
		modigliani_base::Size currIndex) const {
	std::vector<modigliani_base::Real> tmp(_numCompartments());
	for (modigliani_base::Size ll = 0; ll < _numCompartments(); ll++) {
		tmp[ll] = _open_channels(compartmentVec[ll]->Current(currIndex));
	}
	return (tmp);
}

std::vector<modigliani_base::Real> Membrane_compartment_sequence::_vVec() const {
	std::vector<modigliani_base::Real> out;
	for (modigliani_base::Size ll = 0; ll < _numCompartments(); ll++) {
		out.push_back(compartmentVec[ll]->vm());
	}
	return (out);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/**
 * \param current
 * \param compartmentId refers to intuitive enumeriation, i.e. [1..m]
 */
modigliani_base::ReturnEnum Membrane_compartment_sequence::InjectCurrent(
		modigliani_base::Real current /* in nA */, modigliani_base::Size compartmentId) {
	if ((compartmentId < 1) || (compartmentId > _numCompartments()))
		return (modigliani_base::ReturnEnum::PARAM_OUT_OF_RANGE);
	return (compartmentVec[compartmentId - 1]->InjectCurrent(current));
}

/**
 *
 */
void Membrane_compartment_sequence::ShowHinesMatrix() {
	using namespace TNT;
	modigliani_base::Size n = _numCompartments();

	Matrix<modigliani_base::Real> hinesMtr(n, n + 2);

	/* reconstruct tridiagonal matrix from band std::vectors */
	hinesMtr[0][0] = dVec[0];
	hinesMtr[0][1] = uVec[0];

	modigliani_base::Size ll = 0;
	for (ll = 1; ll < _numCompartments() - 1; ll++) {
		hinesMtr[ll][ll - 1] = lVec[ll];
		hinesMtr[ll][ll] = dVec[ll];
		hinesMtr[ll][ll + 1] = uVec[ll];
	}

	hinesMtr[n - 1][n - 2] = lVec[n - 1];
	hinesMtr[n - 1][n - 1] = dVec[n - 1];

	for (ll = 0; ll < _numCompartments() - 1; ll++) {
		hinesMtr[ll][n] = compartmentVec[ll]->vm();
		hinesMtr[ll][n + 1] = rVec[ll];
	}

	std::cerr << hinesMtr;

}

// NOT WORKING
std::vector<modigliani_base::Real> Membrane_compartment_sequence::ZadorPearlmutterSolveTriDiag(
		std::vector<modigliani_base::Real> lNewVec, std::vector<modigliani_base::Real> dNewVec,
		std::vector<modigliani_base::Real> uNewVec,
		std::vector<modigliani_base::Real> rNewVec) const {
	const modigliani_base::Size n = lNewVec.size();
	std::vector<modigliani_base::Real> vNewVec(n);
	M_ASSERT(
			(lNewVec.size() == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()));

	std::vector<modigliani_base::Real> Kl(n);
	std::vector<modigliani_base::Real> Kd(n);
	std::vector<modigliani_base::Real> Ku(n);

	std::vector<modigliani_base::Real> BJd(n);

	modigliani_base::Size ll = 0;
	for (ll = 0; ll < n; ll++) {
		BJd[ll] = 1;
	}
	for (ll = 0; ll < n - 1; ll++) {
		BJd[ll + 1] -= uNewVec[ll] * lNewVec[ll] / BJd[ll];
	}

	Kd[n - 1] = 1 / BJd[n - 1];

	int j = 0;
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
std::vector<modigliani_base::Real> Membrane_compartment_sequence::MascagniSolveTriDiag(
		std::vector<modigliani_base::Real> lNewVec, std::vector<modigliani_base::Real> dNewVec,
		std::vector<modigliani_base::Real> uNewVec,
		std::vector<modigliani_base::Real> rNewVec) const {
	modigliani_base::Size m = lNewVec.size();
	std::vector<modigliani_base::Real> vNewVec(m);
	M_ASSERT(
			(lNewVec.size() == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()));

	/*	Solution of tridiagonal system */
	/* forward elimination */
	lNewVec[0] = 0;
	//dNewVec[0] = dNewVec[0];
	uNewVec[0] = uNewVec[0] / dNewVec[0];
	rNewVec[0] = rNewVec[0] / dNewVec[0];
	modigliani_base::Size i = 1;
	for (i = 1; i < m - 1; i++) {
		dNewVec[i] = dNewVec[i] - lNewVec[i] * vNewVec[i - 1];
		rNewVec[i] = (rNewVec[i] - lNewVec[i] * vNewVec[i - 1]) / dNewVec[i];
		uNewVec[i] = uNewVec[i] / dNewVec[i];
		std::cout << i << " " << lNewVec[i] << " " << dNewVec[i] << " "
				<< uNewVec[i] << " " << vNewVec[i] << " " << rNewVec[i]
				<< std::endl;
	}
	dNewVec[m - 1] = dNewVec[m - 1] - lNewVec[m - 1] * vNewVec[m - 1 - 1];
	rNewVec[m - 1] = (rNewVec[m - 1] - lNewVec[m - 1] * vNewVec[m - 1 - 1])
			/ dNewVec[m - 1];
	uNewVec[m - 1] = 0;

	/* backward substitution */
	vNewVec[m - 1] = rNewVec[m - 1];
	int ll = 0; // INT as modigliani_base::Size_t cannot be compared to a negative number
	for (ll = m - 2; ll > -1; ll--) {
		vNewVec[ll] = rNewVec[ll] - uNewVec[ll] * vNewVec[ll + 1];
		std::cout << ll << " " << lNewVec[ll] << " " << dNewVec[ll] << " "
				<< uNewVec[ll] << " " << vNewVec[ll] << " " << rNewVec[ll]
				<< std::endl;
	}
	return (vNewVec);
}

/**  Compute sum of escape rates over current state in [kHz] */
modigliani_base::Real Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant() const {
	std::cerr
			<< "Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant()"
			<< std::endl;
	modigliani_base::Real sum = 0.0;
	for (modigliani_base::Size ll = 0; ll < numCompartments; ll++) {
		//		cout <<"Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant  SEQ" << std::endl;
		sum += compartmentVec[ll]->CompartmentChannelStateTimeConstant();
		//		cout << "SEQ " << std::endl;
	}
	return (sum);
}

/**  */
std::vector<modigliani_base::Real> Membrane_compartment_sequence::NumericalRecipesSolveTriDiag(
		const std::vector<modigliani_base::Real> & lNewVec,
		const std::vector<modigliani_base::Real> & dNewVec,
		const std::vector<modigliani_base::Real> & uNewVec,
		const std::vector<modigliani_base::Real> & rNewVec) const {
	modigliani_base::Size n = lNewVec.size();
	std::vector<modigliani_base::Real> vNewVec(n);
	M_ASSERT(
			(n == dNewVec.size()) && (dNewVec.size() == uNewVec.size()) && (uNewVec.size() == vNewVec.size()) && (vNewVec.size() == rNewVec.size()));
	modigliani_base::Real bet;
	std::vector<modigliani_base::Real> gam(n);

	// a is l
	// b is d
	// c is u
	// u is v
	// r is r

	vNewVec[0] = rNewVec[0] / (bet = dNewVec[0]);
	modigliani_base::Size j = 0;
	for (j = 1; j < n; j++) {
		gam[j] = uNewVec[j - 1] / bet;
		bet = dNewVec[j] - lNewVec[j] * gam[j];
		if (0 == bet)
			std::cerr
					<< "Membrane_compartment_sequence::NumericalRecipesSolveTriDiag - solver failed, zero pivot element ? is the matrix diagonally domininat ?)."
					<< std::endl;
		vNewVec[j] = (rNewVec[j] - lNewVec[j] * vNewVec[j - 1]) / bet;
	}
	int i = 0;
	for (i = (n - 2); i > -1; i--) {
		vNewVec[i] -= gam[i + 1] * vNewVec[i + 1];
	}

	return (vNewVec);
}

/**  */
bool Membrane_compartment_sequence::GillespieStep() {
	std::cerr << "Membrane_compartment_sequence::GillespieStep()" << std::endl;
	std::vector<modigliani_base::Real> compartmentTauVec(_numCompartments());
	modigliani_base::Uniform_rnd_dist rnd;
	modigliani_base::Real val;
	modigliani_base::Real sum;
	modigliani_base::Real sequenceTau;
	bool integrateStep = false;
	modigliani_base::Real newDeltaT;
	modigliani_base::Real maxDeltaT;

	modigliani_base::Real tStar = 0.0;

	do {
		/** BLOCK 2 */
		std::cerr << "GILLESPIE STEP" << std::endl;
		sequenceTau = CompartmentSequenceChannelStateTimeConstant();
		sum = 0.0;
		val = rnd.RndVal();
		for (modigliani_base::Size ll = 0; ll < _numCompartments(); ll++) {
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

	return (modigliani_base::ReturnEnum::SUCCESS);
}

//returns copy of compartment std::vector
Cylindrical_compartment* Membrane_compartment_sequence::ReturnCompartmentVec(
		modigliani_base::Size index) //TODO: added
		{
	Cylindrical_compartment* compVec = compartmentVec[index];
	return (compVec);
}

