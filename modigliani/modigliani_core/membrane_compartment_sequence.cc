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

namespace modigliani_core{

bool Membrane_compartment_sequence::seed_set_ = false;

Membrane_compartment_sequence::Membrane_compartment_sequence(bool use_gillespie)
    : Membrane(),
      initialised_(false),
      sw_crank_nicholson_(false),
      use_gillespie_(use_gillespie) {
  num_compartments_ = 0;

  l_vec_.resize(1);
  u_vec_.resize(1);
  d_vec_.resize(1);
  r_vec_.resize(1);

  if (Membrane_compartment_sequence::seed_set_ == false) {
    seed_ = time(NULL);
    std::cout << "Initial seed = " <<seed_ << std::endl;
    Membrane_compartment_sequence::seed_set_ = true;
  }
  rng_ = boost::random::mt19937();
  rng_.seed(seed_++);
  uni_ = boost::random::uniform_01<>();
}

Membrane_compartment_sequence::~Membrane_compartment_sequence() {
  for (auto it = compartment_vec_.begin(); it != compartment_vec_.end(); it++) {
    delete *it;
  }
}

modigliani_base::ReturnEnum Membrane_compartment_sequence::PushBack(
    Cylindrical_compartment * compartPtr) {
  compartPtr->set_timestep(timestep());
  compartment_vec_.push_back(compartPtr);
  num_compartments_++;

  initialised_ = false;

  if (compartment_vec_.size() != num_compartments_)
    return (modigliani_base::ReturnEnum::FAIL);
  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Membrane_compartment_sequence::InjectCurrent(
    modigliani_base::Real current,
    modigliani_base::Size compartmentId) {
  if ((compartmentId < 1) || (compartmentId > num_compartments()))
    return (modigliani_base::ReturnEnum::PARAM_OUT_OF_RANGE);
  return (compartment_vec_[compartmentId - 1]->InjectCurrent(current));
}


modigliani_base::ReturnEnum Membrane_compartment_sequence::Step() {
  if (!initialised_) {
    std::cerr
        << "Membrane_compartment_sequence::Step() "
        << "- Warning : Called method without Init()"
        << " beeing called after instantiation or "
        << "AddCompartment. Calling Init() now"
        << std::endl;
    if (Init() != modigliani_base::ReturnEnum::SUCCESS) {
      std::cerr
          << "Membrane_compartment_sequence::Step() "
          << "- Error : Call to Init failed (No compartments present ?)."
          << std::endl;
      return (modigliani_base::ReturnEnum::FAIL);
    }
  }

  // Necessary work for Gillespie algorithm. I have not tested this.
  if (use_gillespie_){
    std::cerr << "Membrane_compartment_sequence::GillespieStep()" << std::endl;
    std::vector<modigliani_base::Real> compartmentTau_vec_(num_compartments());
    modigliani_base::Real val;
    modigliani_base::Real sum;
    modigliani_base::Real sequenceTau;
    bool integrateStep = false;
    modigliani_base::Real newDeltaT;
    modigliani_base::Real maxDeltaT;
    
    modigliani_base::Real tStar = 0.0;
    
    do {
      // BLOCK 2
      std::cerr << "GILLESPIE STEP" << std::endl;
      sequenceTau = CompartmentSequenceChannelStateTimeConstant();
      sum = 0.0;
      val = uni_(rng_);
      for (modigliani_base::Size ll = 0; ll < num_compartments(); ll++) {
        sum += compartment_vec_[ll]->CompartmentChannelStateTimeConstant();
        if (val < sum / sequenceTau) {
          std::cerr << "STEPING COMPARTMENT " << ll << std::endl;
          integrateStep = compartment_vec_[ll]->GillespieStep();
          break;
        }
      }
      // BLOCK 1
      // this is a sum of rate constants !
      newDeltaT = log(1 / uni_(rng_)) / sequenceTau;  //sequenceTau in [kHz] while newDeltaT in [ms]
      std::cerr << "NEW DELTA T=" << newDeltaT << std::endl;
      maxDeltaT = 1;  // maximumTimeStep ought to be 1 ms
      if (newDeltaT > maxDeltaT) {
        newDeltaT = maxDeltaT;
      }
      set_timestep(newDeltaT);
      tStar += newDeltaT;
    } while (integrateStep == false);
    
    std::cerr << "INTEGRATOR STEP WITH T_STAR=" << tStar << std::endl;
    set_timestep(tStar);
  }
  
  std::vector<modigliani_base::Real> tmpVVec;
  modigliani_base::Real omega = 0.0;
  modigliani_base::Size ll = 0;

  /* load voltage std::vector and rhs-std::vector */

  for (ll = 0; ll < num_compartments_; ll++) {
    /* omega should have units of mV : mSec nA / muF = muV */
    omega = 1e-3 /* mV/muV */
    * (timestep() / compartment_vec_[ll]->CompartmentMembraneCapacitance())
        * (compartment_vec_[ll]->CompartmentMembraneNetCurrent());
    r_vec_[ll] = compartment_vec_[ll]->vm() + omega;  // compute RHS of finite difference equation
    // OMEGA is substracted because the currents are of the opposite sign to Aldo's PhD thesis
  }

  std::vector<modigliani_base::Real> vVec = NumericalRecipesSolveTriDiag(l_vec_,
                                                                         d_vec_,
                                                                         u_vec_,
                                                                         r_vec_);
  /* set new voltage */
  for (ll = 0; ll < num_compartments_; ll++) {
    compartment_vec_[ll]->Step(vVec[ll]);  // Step also advances the voltage -> ignore by using vVec
  }
  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Membrane_compartment_sequence::Init() {
  if (compartment_vec_.size() <= 0) {
    std::cerr
        << "membrane_compartment_sequence_o::Init -"
        << " ERROR : No compartments present."
        << std::endl;
    initialised_ = false;
    return (modigliani_base::ReturnEnum::FAIL);
  }

  l_vec_.resize(num_compartments_);
  u_vec_.resize(num_compartments_);
  d_vec_.resize(num_compartments_);
  r_vec_.resize(num_compartments_);

  compartment_vec_[0]->set_vm(-65);
  modigliani_base::Size ll = 1;
  for (ll = 1; ll < num_compartments_; ll++) {
    compartment_vec_[ll]->set_vm(-65);
    /* testing requirement for constant axo-geometric properties */
    assert(compartment_vec_[ll]->radius() == compartment_vec_[ll - 1]->radius());
    if(compartment_vec_[ll]->ra() != compartment_vec_[ll - 1]->ra()) {
      std::cerr << "WARNING : Non-uniform axoplasmic resistance "
                << "is not thoroughly tested."
                << std::endl;
    }
  }
  compartment_vec_[num_compartments_ - 1]->set_vm(-65);

  /* initialisation of left band l and right band u "std::vectors" */
  /* FIRST COMPARTMENT */
  auto sigma_forward = _sigma(compartment_vec_[1], compartment_vec_[0]);

  u_vec_[0] = -2.0 * sigma_forward;  // vonNeumann boundary conditions
  d_vec_[0] = 2.0 * sigma_forward + 1.0;

  /* INTERMEDIATE COMPARTMENTS */
  for (ll = 1; ll < num_compartments_ - 1; ll++) {
    sigma_forward = _sigma(compartment_vec_[ll + 1], compartment_vec_[ll]);
    auto sigma_backward = _sigma(compartment_vec_[ll - 1], compartment_vec_[ll]);
    l_vec_[ll] = -sigma_backward;
    d_vec_[ll] = sigma_forward + sigma_backward + 1.0;
    u_vec_[ll] = -sigma_forward;
  }

  /* LAST COMPARTMENT */
  auto sigma_backward = _sigma(compartment_vec_[num_compartments_ - 2],
                               compartment_vec_[num_compartments_ - 1]);
  d_vec_[num_compartments_ - 1] = 2.0 * sigma_backward + 1.0;
  l_vec_[num_compartments_ - 1] = -2.0 * sigma_backward;  // vonNeumann boundary conditions

  /* completed initialisation */
  initialised_ = true;

  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::Real Membrane_compartment_sequence::_sigma(
    const Cylindrical_compartment* from,
    const Cylindrical_compartment* to) const {
  modigliani_base::Real deltaT = timestep();
  // sigma is unit-free  (scaled by 0.1) [sigma] = mSec muM / (muM^2 Ohm cm) * (muM^2 / muF)
  modigliani_base::Real output = 0.1
      * (deltaT / to->CompartmentMembraneCapacitance())
      / (to->ra()
         * (2 * to->length() / (4 * 3.1415 * pow(to->radius(), 2)))
         + from->ra()
         * (2 * from->length() / (4 * 3.1415 * pow(from->radius(), 2))));
  return (output);
}

modigliani_base::ReturnEnum Membrane_compartment_sequence::InitialStep() {

  sw_crank_nicholson_ = true;
  set_timestep(timestep() / 2.0);
  for (modigliani_base::Size ll = 0; ll < num_compartments(); ll++) {
    compartment_vec_[ll]->Step(compartment_vec_[ll]->vm());
  }

  set_timestep(timestep() * 2.0);
  std::cerr
      << "Membrane_compartment_sequence::InitialStep() - ERROR : not correctly implemented ? untested."
      << std::endl;
  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::Real Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant() const {
  std::cerr
      << "Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant()"
      << std::endl;
  modigliani_base::Real sum = 0.0;
  for (modigliani_base::Size ll = 0; ll < num_compartments(); ll++) {
    //		cout <<"Membrane_compartment_sequence::CompartmentSequenceChannelStateTimeConstant  SEQ" << std::endl;
    sum += compartment_vec_[ll]->CompartmentChannelStateTimeConstant();
    //		cout << "SEQ " << std::endl;
  }
  return (sum);
}

std::vector<modigliani_base::Real> Membrane_compartment_sequence::NumericalRecipesSolveTriDiag(
    const std::vector<modigliani_base::Real> & lNewVec,
    const std::vector<modigliani_base::Real> & dNewVec,
    const std::vector<modigliani_base::Real> & uNewVec,
    const std::vector<modigliani_base::Real> & rNewVec) const {
  modigliani_base::Size n = lNewVec.size();
  std::vector<modigliani_base::Real> vNewVec(n);
  assert(
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
}  // namespace modigliani_core
