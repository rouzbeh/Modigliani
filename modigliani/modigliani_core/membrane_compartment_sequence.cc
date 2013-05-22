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
Membrane_compartment_sequence::Membrane_compartment_sequence()
    : Membrane(), initialised(false), swCrankNicholson(false) {
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
    rVec[ll] = compartmentVec[ll]->vm() + omega;  // compute RHS of finite difference equation
    // OMEGA is substracted because the currents are of the opposite sign to Aldo's PhD thesis
  }

  std::vector<modigliani_base::Real> vVec = NumericalRecipesSolveTriDiag(lVec,
                                                                         dVec,
                                                                         uVec,
                                                                         rVec);
  /* set new voltage */

  for (ll = 0; ll < numCompartments; ll++) {
    compartmentVec[ll]->Step(vVec[ll]);  // Step also advances the voltage -> ignore by using vVec
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

  //modigliani_base::Real sigma = 0;
  //modigliani_base::Real radius = compartmentVec[0]->radius();  // should be constant with present solution method
  //modigliani_base::Real axoplasmicR = compartmentVec[0]->ra();  // (specific!) should be constant with present solution method
  //modigliani_base::Real deltaT = timeStep();
  //modigliani_base::Real deltaXX = 0;

  compartmentVec[0]->set_vm(-65);
  modigliani_base::Size ll = 1;
  for (ll = 1; ll < numCompartments; ll++) {
    compartmentVec[ll]->set_vm(-65);
    /* testing requirement for constant axo-geometric properties */
    M_ASSERT(compartmentVec[ll]->radius() == compartmentVec[ll - 1]->radius());
    M_ASSERT(compartmentVec[ll]->ra() == compartmentVec[ll - 1]->ra());
  }
  compartmentVec[numCompartments - 1]->set_vm(-65);

  /* initialisation of left band l and right band u "std::vectors" */
  /* FIRST COMPARTMENT */
  auto sigma_forward = _sigma(compartmentVec[0], compartmentVec[1]);

  uVec[0] = -2.0 * sigma_forward;  // vonNeumann boundary conditions
  dVec[0] = 2.0 * sigma_forward + 1.0;

  /* INTERMEDIATE COMPARTMENTS */
  for (ll = 1; ll < numCompartments - 1; ll++) {
    sigma_forward = _sigma(compartmentVec[ll], compartmentVec[ll + 1]);
    auto sigma_backward = _sigma(compartmentVec[ll], compartmentVec[ll-1]);
    auto sigma = sigma_forward + sigma_backward;
    lVec[ll] = -sigma_backward;
    dVec[ll] = sigma + 1.0;
    uVec[ll] = -sigma_forward;
  }

  /* LAST COMPARTMENT */
  auto sigma_backward = _sigma(compartmentVec[numCompartments - 1],
                               compartmentVec[numCompartments - 2]);
  dVec[numCompartments - 1] = 2.0 * sigma_backward + 1.0;
  lVec[numCompartments - 1] = -2.0 * sigma_backward;  // vonNeumann boundary conditions

  /* completed initialisation */
  initialised = true;

  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::Real Membrane_compartment_sequence::_sigma(
    const Cylindrical_compartment* from,
    const Cylindrical_compartment* to) const {
  modigliani_base::Real deltaT = timeStep();
  modigliani_base::Real deltaXX = from->length() * to->length();
  modigliani_base::Real output = (0.1 / 2.0) * (deltaT / deltaXX)
      * (from->radius() / from->ra())
      * (from->area() / from->CompartmentMembraneCapacitance());

  return (output);
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
    modigliani_base::Real current /* in nA */,
    modigliani_base::Size compartmentId) {
  if ((compartmentId < 1) || (compartmentId > _numCompartments()))
    return (modigliani_base::ReturnEnum::PARAM_OUT_OF_RANGE);
  return (compartmentVec[compartmentId - 1]->InjectCurrent(current));
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
      (n == dNewVec.size()) && (dNewVec.size() == uNewVec.size())
          && (uNewVec.size() == vNewVec.size())
          && (vNewVec.size() == rNewVec.size()));
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
    newDeltaT = log(1 / rnd.RndVal()) / sequenceTau;  //sequenceTau in [kHz] while newDeltaT in [ms]
    std::cerr << "NEW DELTA T=" << newDeltaT << std::endl;
    maxDeltaT = 1;  // maximumTimeStep ought to be 1 ms
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
