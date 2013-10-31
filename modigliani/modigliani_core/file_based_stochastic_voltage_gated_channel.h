/**
 * @file file_based_stochastic_voltage_gated_channel.h
 * @brief File_based_stochastic_voltage_gated_channel class header
 *
 * Copyright (C) 2013 Mohammad Ali Neishabouri
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_FILE_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_
#define MODIGLIANI_MODIGLIANI_CORE_FILE_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_

#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "modigliani_core/voltage_gated_ion_channel_current.h"
#include "modigliani_core/ion_channels.h"
#include "modigliani_core/aux_func.h"

namespace modigliani_core {
/**
 * @class File_based_stochastic_voltage_gated_channel
 * @brief Voltage gated ion channel with transition probabilities stored in a file
 */
class File_based_stochastic_voltage_gated_channel
:public Voltage_gated_ion_channel_current {
  public:
/**
 * @brief Instantiates a new ion channel by reading data from a JSON
 * file
 * @param newArea Compartment surface area in [@f$\si{\micro\meter\squared}@f$]
 * @param newDensity Channel density in
 * [@f$\si{\per\micro\meter\squared}@f$]
 * @param newConductivity Single channel conductivity in
 * [@f$\si{\milli\siemens}@f$]
 * @param reversalPotential Ionic reversal potential in
 * [@f$\si{\milli\volt}@f$]
 * @param newTimeStep Simulation timestep in
 * [@f$\si{\milli\second}@f$]
 * @param newTemperature Temperature in [@f$\si{\celsius}@f$]
 * @param fileName JSON file path
 */
    File_based_stochastic_voltage_gated_channel(
        modigliani_base::Real newArea, modigliani_base::Real newDensity,
        modigliani_base::Real newConductivity,
        modigliani_base::Real reversalPotential,
        modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
        std::string fileName);

    File_based_stochastic_voltage_gated_channel(
        const File_based_stochastic_voltage_gated_channel & original) = delete;

    const File_based_stochastic_voltage_gated_channel& operator=(
        const File_based_stochastic_voltage_gated_channel & right) = delete;

    virtual ~File_based_stochastic_voltage_gated_channel();

/**
 * @brief Opens, reads and parses the JSON file. Uses data to populate
 *  the transition matrix.
 *
 * The JSON file is expected to contain the following information
 *   - base_temperature The temperature at which recordings were
 * done, in [@f$\si{\celsius}@f$]
 *   - number_of_states Number of states
 *   - open_states List of states in which the channel is conducting
 * ([1..m])
 *   - maxV Maximum voltage in [@f$\si{\milli\volt}@f$]
 *   - minV Minimum voltage in [@f$\si{\milli\volt}@f$]
 *   - step Voltage step in [@f$\si{\milli\volt}@f$]
 *   - transitions List of structure containing
 *     - voltage Membrane potential in [@f$\si{\milli\volt}@f$]
 *     - probability Transition probability
 *     - start Start state [1..]
 *     - stop End state [1..m]
 *     - q10 Dependency on temperature
 *
 * For an example, see \ref sodium_channel.json
 * @param fileName JSON file path
 * @param temperature Temperature for simulations in
 * [@f$\si{\celsius}@f$]
 * @param time_step Timestep in [@f$\si{\milli\second}@f$]
 */
    static void LoadFile(std::string fileName, double temperature,
                          double time_step);

/**
 * @short This function simulates one timestep.
 * @return Success or Failure
 */
    virtual modigliani_base::ReturnEnum StepCurrent() override;

/**
 * @brief Returns number of open ionic channels
 * @return Number of open channels
 */
    virtual modigliani_base::Real OpenChannels() const override;

/**
 * @brief Sets the conductance of the membrane current to the value
 * given by the number of open channels.
 * @return Conductance in [@f$\si{\milli\siemens}@f$]
 */
    virtual modigliani_base::Real ComputeConductance() override;

/**
 * @brief Returns channels time constant
 * @return Time constant in [@f$\si{\per\second}@f$]
 */
    modigliani_base::Real ComputeTimeConstant() const;

    static std::map<std::string,
      Transition_rate_matrix*> probability_matrix_map;
    static std::map<std::string, int> number_of_states_map;
    static std::map<std::string, double> base_temperature_map;
    static std::map<std::string,
      std::vector<modigliani_base::Size> > open_states_map;

  private:
    Ion_channels* channels_ptr_;
    static bool initTableLookUp;
    static std::vector<std::string> initialised_probability_matrices;
    modigliani_base::Real baseTemp;
};
/**
 * @example sodium_channel.json
 * This is an example of a JSON file for a sodium channel.
 *
 * Note that the voltage step here is deliberately too big (@f$\SI{10}{\milli\volt}@f$)
 * to make the example file smaller.
 */ 
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_FILE_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_
