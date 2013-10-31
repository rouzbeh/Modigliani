/**
 * @file custom_cylindrical_compartment.cc
 *
 * @brief Custom_cylindrical_compartment class implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Ali Neishabouri
 *
 * @section LICENSE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modigliani_core/custom_cylindrical_compartment.h"

using modigliani_base::Real;

namespace modigliani_core {
  Custom_cylindrical_compartment::
  Custom_cylindrical_compartment(const modigliani_base::Real
                                 newLength,
                                 const
                                 modigliani_base::Real
                                 newDiameter,
                                 const
                                 modigliani_base::Real
                                 newCm, const
                                 modigliani_base::Real
                                 newRa, const
                                 modigliani_base::Real
                                 newTemperature,
                                 const
                                 modigliani_base::Real
                                 voutvinratio)
  :Cylindrical_compartment(newLength, newDiameter, newCm, newRa,
                           newTemperature), volumeratio(voutvinratio) {
    custom_current_vec_ = std::vector < custom_current > (0);
    set_temperature(newTemperature);
  }

  Custom_cylindrical_compartment::~Custom_cylindrical_compartment() {
  }

  modigliani_base::ReturnEnum
    Custom_cylindrical_compartment::AttachCurrentWithConcentrations
    (Membrane_current * currentPtr, Real concentration_inside,
     Real concentration_outside) {
    currentPtr->set_voltage(vm());
    currentPtr->set_timestep(timestep());
    currentPtr->set_temperature(temperature());

    custom_current new_current;
    new_current.current_ptr = currentPtr;
    new_current.inside_concentration = concentration_inside;
    new_current.outside_concentration = concentration_outside;
    new_current.reversal_potential = currentPtr->reversal_potential();
    new_current.track = true;
    custom_current_vec_.push_back(new_current);
    Membrane_compartment::AttachCurrent(currentPtr);
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  modigliani_base::ReturnEnum Custom_cylindrical_compartment::
    AttachCurrent(Membrane_current * currentPtr, CurrentType type) {
    currentPtr->set_voltage(vm());
    currentPtr->set_timestep(timestep());
    currentPtr->set_temperature(temperature());

    custom_current new_current;
    new_current.current_ptr = currentPtr;
    new_current.inside_concentration = 0;
    new_current.outside_concentration = 0;
    new_current.reversal_potential = currentPtr->reversal_potential();
    new_current.track = false;
    custom_current_vec_.push_back(new_current);
    Membrane_compartment::AttachCurrent(currentPtr);
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  modigliani_base::ReturnEnum Custom_cylindrical_compartment::
    Step(const modigliani_base::Real newVM) {
    set_vm(newVM);
    // for every current
    for (modigliani_base::Size it = 0; it < NumberCurrents(); it++) {
      Membrane_current *current = custom_current_vec_[it].current_ptr;
      current->Step(newVM);
      if ((custom_current_vec_[it]).track) {
        // Count how many ions got in, in picomoles
        modigliani_base::Real ions_picomoles = current->current() * timestep()
          / 96485.3415;
        custom_current_vec_[it].inside_concentration -= ions_picomoles * 1000000
          / volume();
        custom_current_vec_[it].outside_concentration +=
          ions_picomoles * 1000000 / (volume() * volumeratio);
        // TODO(Ali): What to do with outside concentration?
        custom_current_vec_[it].reversal_potential = nernst_multiplier
          * log(custom_current_vec_[it].outside_concentration
                / custom_current_vec_[it].inside_concentration);
        current->set_reversal_potential(custom_current_vec_[it].
                                        reversal_potential);
      }
      // TODO(Ali): pumps!
    }
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  modigliani_base::ReturnEnum
  Custom_cylindrical_compartment::SetupOutput(std::string output_file_name) {
    std::ifstream filestr;
    filestr.open(output_file_name.c_str(), std::ios::binary);
     // put the "cursor" at the end of the file
    filestr.seekg(0, std::ios::end);
    // find the position of the cursor
    int file_length = filestr.tellg();
    // close your file
    filestr.close();
    if (file_length > 0) {
      output_file = new std::ofstream(output_file_name.c_str(),
                                      std::ios::binary | std::ios::app);

      if (output_file->fail()) {
        std::cerr << "Could not open output file " << output_file_name
          << std::endl;
        return (modigliani_base::ReturnEnum::FAIL);
      }
    } else {
      output_file =
        new std::ofstream(output_file_name.c_str(), std::ios::binary);

      if (output_file->fail()) {
        std::cerr << "Could not open output file " << output_file_name
          << std::endl;
        return (modigliani_base::ReturnEnum::FAIL);
      }
      modigliani_base::Size number_of_columns = 4 * NumberCurrents() + 1;
      output_file->write(reinterpret_cast < char *>(&number_of_columns),
                         sizeof(number_of_columns));
    }
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  modigliani_base::ReturnEnum
  Custom_cylindrical_compartment::WriteOutput()const {
    float out_data[1 + 4 * NumberCurrents()];
    out_data[0] = vm();
    int counter = 1;
    for (modigliani_base::Size ll = 1; ll - 1 < NumberCurrents(); ++ll) {
      out_data[counter++] = Current(ll)->current();
      out_data[counter++] = custom_current_vec_[ll - 1].inside_concentration;
      out_data[counter++] = custom_current_vec_[ll - 1].outside_concentration;
      out_data[counter++] = custom_current_vec_[ll - 1].reversal_potential;
    } output_file->write(reinterpret_cast < char *>(out_data),
                         (1 + 4 * NumberCurrents()) * sizeof(out_data[0]));
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  void Custom_cylindrical_compartment::
  SetInsideConcentration(modigliani_base::Size currentIndex,
                         Real new_concentration) {
    assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
    custom_current_vec_[currentIndex - 1].inside_concentration =
      new_concentration;
    custom_current_vec_[currentIndex - 1].reversal_potential = nernst_multiplier
      * log(custom_current_vec_[currentIndex - 1].outside_concentration
            / custom_current_vec_[currentIndex - 1].inside_concentration);
    custom_current_vec_[currentIndex -
                        1].current_ptr->
      set_reversal_potential(custom_current_vec_[currentIndex - 1].
                             reversal_potential);
  }

  void Custom_cylindrical_compartment::
      SetOutsideConcentration(modigliani_base::Size currentIndex,
                              Real new_concentration) {
    assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
    custom_current_vec_[currentIndex - 1].outside_concentration =
      new_concentration;
    custom_current_vec_[currentIndex - 1].reversal_potential = nernst_multiplier
      * log(custom_current_vec_[currentIndex - 1].outside_concentration
            / custom_current_vec_[currentIndex - 1].inside_concentration);
    custom_current_vec_[currentIndex -
                        1].current_ptr->
      set_reversal_potential(custom_current_vec_[currentIndex - 1].
                             reversal_potential);
  }
}  // namespace modigliani_core
