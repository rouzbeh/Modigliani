/**
 * \file membrane_compartment.cc
 * \brief Membrane_compartment class implementation
 *
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001
 * @version   1.0
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2016 Ali Neishabouri
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

#include "membrane_compartment.h"

namespace modigliani {
bool Membrane_compartment::seed_set_ = false;

Membrane_compartment::Membrane_compartment(
  const modigliani::Real newArea /* in muMeter^2 */,
  const modigliani::Real newTemperature,
  const modigliani::Real newCm, const modigliani::Real newRa)
  : cm_(newCm), ra_(newRa), area_(newArea) {
  vm_    = 0;
  i_inj_ = 0;
  set_temperature(newTemperature);
  current_vec_ = std::vector<Membrane_current *>(0);
  output_file  = 0;

  if (Membrane_compartment::seed_set_ == false) {
    Membrane_compartment::seed_ = time(NULL);
    std::cout << "Initial seed = " << Membrane_compartment::seed_ << std::endl;
    Membrane_compartment::seed_set_ = true;
  }
  rng_ = boost::random::mt19937();
  rng_.seed(Membrane_compartment::seed_++);
  uni_ = boost::random::uniform_01<>();
}

Membrane_compartment::~Membrane_compartment() {
  for (auto it = current_vec_.begin(); it != current_vec_.end(); ++it) {
    delete *it;
  }

  if (output_file) output_file->close();
}

modigliani::ReturnEnum Membrane_compartment::SetupOutput(
  std::string output_file_name) {
  std::ifstream filestr;

  filestr.open(output_file_name.c_str(), std::ios::binary); // open your file
  filestr.seekg(0, std::ios::end);                          // put the "cursor"
                                                            // at the end of the
                                                            // file
  int file_length = filestr.tellg();                        // find the position
                                                            // of the cursor
  filestr.close();                                          // close your file

  if (file_length > 0) {
    output_file = new std::ofstream(output_file_name.c_str(),
                                    std::ios::binary | std::ios::app);

    if (output_file->fail()) {
      std::cerr << "Could not open output file " << output_file_name
                << std::endl;
      return modigliani::ReturnEnum::FAIL;
    }
  } else {
    output_file = new std::ofstream(output_file_name.c_str(), std::ios::binary);

    if (output_file->fail()) {
      std::cerr << "Could not open output file " << output_file_name
                << std::endl;
      return modigliani::ReturnEnum::FAIL;
    }
    modigliani::Size number_of_columns = NumberCurrents() + 1;
    output_file->write(reinterpret_cast<char *>(&number_of_columns),
                       sizeof(modigliani::Size));
  }
  return modigliani::ReturnEnum::SUCCESS;
}

modigliani::ReturnEnum Membrane_compartment::WriteOutput() const {
  float out_data[1 + NumberCurrents()];

  out_data[0] = vm();

  for (modigliani::Size ll = 1; ll - 1 < NumberCurrents(); ++ll) {
    out_data[ll] = Current(ll)->current();
  }
  output_file->write(reinterpret_cast<char *>(out_data),
                     (1 + NumberCurrents()) * sizeof(float));
  return modigliani::ReturnEnum::SUCCESS;
}

modigliani::ReturnEnum Membrane_compartment::Step(
  const modigliani::Real newVM) {
  vm_ = newVM;

  // for every current
  for (modigliani::Size it = 0; it < current_vec_.size(); it++) {
    (current_vec_[it])->Step(vm_);
  }

  return modigliani::ReturnEnum::SUCCESS;
}

modigliani::ReturnEnum Membrane_compartment::Step() {
  return Step(vm_ +
              1.0e-3 * CompartmentMembraneNetCurrent()
              / CompartmentMembraneCapacitance() * timestep());
}

modigliani::Real Membrane_compartment::WeightedConductance() const {
  modigliani::Real result = 0.0;

  std::vector<Membrane_current *>::const_iterator it = current_vec_.begin();

  for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
    result += ((*it)->conductance()) * ((*it)->reversal_potential());
  }
  return result;
}

modigliani::ReturnEnum Membrane_compartment::AttachCurrent(
  Membrane_current *currentPtr, CurrentType type) {
  currentPtr->set_voltage(vm_);
  currentPtr->set_timestep(timestep());
  currentPtr->set_temperature(temperature_);

  switch (type) {
  case LEAK:
    current_vec_.push_back(currentPtr);
    break;

  case IONIC:
    current_vec_.push_back(currentPtr);
    break;

  default:
    std::cerr
      << "Membrane_compartment::AttachCurrent - Error : Unsupported current type "
      << type << "specified." << std::endl;
    return modigliani::ReturnEnum::PARAM_UNSUPPORTED;
  }

  return modigliani::ReturnEnum::SUCCESS;
}

modigliani::ReturnEnum Membrane_compartment::InjectCurrent(
  modigliani::Real current) {
  i_inj_ = current;
  return modigliani::ReturnEnum::SUCCESS;
}

modigliani::Real Membrane_compartment::CompartmentMembraneCapacitance() const
{
  return cm_ /* muF/cm^2 */ * area_ /* muMeter^2 */ * 1.0e-8 /* cm^2/muMeter^2
                                                              */
  ;
}

modigliani::Real Membrane_compartment::CompartmentMembraneNetCurrent() const
{
  modigliani::Real sumDeltaI = 0.0;

  for (modigliani::Size it = 0; it < current_vec_.size(); it++) {
    sumDeltaI -= (current_vec_[it])->current();

    // i.e. ionic current is subtracted (modern  current convention)
  }
  sumDeltaI += i_inj_;
  return sumDeltaI;
}

modigliani::Real Membrane_compartment::CompartmentChannelStateTimeConstant()
const {
  std::cerr << "Membrane_compartment::CompartmentChannelStateTimeConstant"
            << std::endl;
  modigliani::Real sum                          = 0;
  std::vector<Membrane_current *>::const_iterator it = current_vec_.begin();

  for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
    // (*it)->ComputeRateConstants(_vM());
    sum += (*it)->ComputeTimeConstant();
    std::cerr << "Compartment Time constant sum=" << sum << std::endl;
  }

  return sum;
}

bool Membrane_compartment::GillespieStep() {
  std::cerr << "Membrane_compartment::GillespieStep()" << std::endl;
  modigliani::Real val = uni_(rng_);
  modigliani::Real sum = 0.0;

  // 2DO this might be actually called more then once in one total iteration
  // time step
  modigliani::Real compartmentTau = CompartmentChannelStateTimeConstant();

  std::cerr << "COMPARTMENT -> compartmentTau=" << compartmentTau << std::endl;
  std::vector<Membrane_current *>::iterator it = current_vec_.begin();

  for (it = current_vec_.begin(); it != current_vec_.end(); it++) {
    sum += (*it)->ComputeTimeConstant();
    std::cerr << "COMPARTMENT -> SUM=" << sum << " VAL=" << val << std::endl;

    if (val < sum / compartmentTau) {
      return (*it)->StepCurrent();

      break;
    }
  }
  std::cerr
    <<
    "Membrane_compartment::GillespieStep - Error : Control flow should not reach here."
    << std::endl;
  return modigliani::ReturnEnum::FAIL;
}

modigliani::Size Membrane_compartment::NumberCurrents() const {
  return current_vec_.size();
}
} // namespace modigliani
