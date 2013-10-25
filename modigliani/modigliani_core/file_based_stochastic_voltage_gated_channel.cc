/**
 * @file file_based_stochastic_voltage_gated_channel.cc
 * @brief File_based_stochastic_voltage_gated_channel implementation
 *
 * Copyright 2013 Mohammad Ali Neishabouri
 *
 * @section LICENCE
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

#include "modigliani_core/file_based_stochastic_voltage_gated_channel.h"

namespace modigliani_core {
  bool File_based_stochastic_voltage_gated_channel::initTableLookUp = false;
  std::map <
    std::string,
    Transition_rate_matrix *
    >File_based_stochastic_voltage_gated_channel::probability_matrix_map;
  std::map
  < std::string,
    int >File_based_stochastic_voltage_gated_channel::number_of_states_map;
  std::map
  < std::string,
    double >File_based_stochastic_voltage_gated_channel::base_temperature_map;
  std::map
  < std::string,
    std::vector < modigliani_base::Size >
    >File_based_stochastic_voltage_gated_channel::open_states_map;

  using boost::property_tree::ptree;
  using modigliani_base::Size;
  using modigliani_base::Real;

  File_based_stochastic_voltage_gated_channel::
    File_based_stochastic_voltage_gated_channel(modigliani_base::Real newArea,
                                                modigliani_base::
                                                Real newDensity,
                                                modigliani_base::
                                                Real newConductivity,
                                                modigliani_base::
                                                Real newReversalPotential,
                                                modigliani_base::
                                                Real newTimeStep,
                                                modigliani_base::
                                                Real newTemperature,
                                                std::string fileName)
  :Voltage_gated_ion_channel_current(newReversalPotential, newDensity, newArea,
                                     newConductivity) {
    UpdateNumChannels();  // TODO(Ali)

    set_timestep(newTimeStep);
    set_temperature(newTemperature);
    if (number_of_states_map[fileName] == 0) {
      LoadFile(fileName, newTemperature, newTimeStep);
    }
    baseTemp = base_temperature_map[fileName];

    assert(number_of_states_map[fileName] > 0);

    if (false == initTableLookUp) {
      initTableLookUp = true;
    }
    channels_ptr_ = new Ion_channels(num_channels(),
                                     number_of_states_map[fileName],
                                     probability_matrix_map[fileName],
                                     newTimeStep);

    for (unsigned int i = 0; i < open_states_map[fileName].size(); i++) {
      channels_ptr_->SetAsOpenState(open_states_map[fileName][i]);
    }
  }

  File_based_stochastic_voltage_gated_channel::
  ~File_based_stochastic_voltage_gated_channel() {
    delete channels_ptr_;
  }

  void File_based_stochastic_voltage_gated_channel::
  LoadFile(std::string fileName,
            double temperature,
            double time_step) {
    std::cout << "Loading probabilities from " << fileName << std::endl;
    boost::property_tree::ptree root;
    try {
      read_json(fileName, root);
    } catch(std::exception & e) {
      // report to the user the failure and their locations in the document.
      std::cerr << "Failed to parse configuration\n" << e.what();
      exit(1);
    }

    base_temperature_map[fileName] =
      root.get < double >("base_temperature", 20);
    number_of_states_map[fileName] = root.get < Size > ("number_of_states", 0);

    open_states_map[fileName] = std::vector < modigliani_base::Size > ();
    BOOST_FOREACH(boost::property_tree::ptree::value_type const &v,
                  root.get_child("open_states")) {
      open_states_map[fileName].push_back(v.second.get < modigliani_base::Size >
                                          (""));
    }

    Real minV = root.get < double >("minV", 0);
    Real maxV = root.get < double >("maxV", 0);
    Real step = root.get < double >("step", 0);

    std::vector < boost::property_tree::ptree > transitions(0);
    for (boost::property_tree::ptree::value_type const
             &v:root.get_child("transitions")) {
      transitions.push_back(v.second);
    }

    probability_matrix_map[fileName] =
      new Transition_rate_matrix(number_of_states_map[fileName], minV, maxV,
                                 step);

    for (unsigned int index = 0; index < transitions.size(); ++index) {
      double q10 = transitions[index].get < double >("q10", 1);
      double base_probability =
        transitions[index].get < double >("probability", 0);
      Real probability = modigliani_core::
          TemperatureRateRelation(temperature,
                                  base_temperature_map
                                  [fileName],
                                  q10)
        * base_probability * time_step;

      // assert(probability>0 && probability<=1);
      // Converted voltage is real_voltage
      Real converted_voltage = transitions[index].get < double >("voltage", 0);
      probability_matrix_map[fileName]->SetTransitionProbability
        (converted_voltage, transitions[index].get < Size > ("start"),
         transitions[index].get < Size > ("stop"), probability);
    }
  }

  inline modigliani_base::
    ReturnEnum File_based_stochastic_voltage_gated_channel::StepCurrent() {
    switch (simulation_mode()) {
      case BINOMIALPOPULATION: {
          return (channels_ptr_->BinomialStep(voltage_));
        }
        break;
      case SINGLECHANNEL: {
          return (channels_ptr_->SingleChannelStep(voltage_));
        }
        break;
      case GILLESPIE: {
          return (channels_ptr_->GillespieStep(voltage_));
        }
        break;
      case DETERMINISTIC: {
          return (channels_ptr_->DeterministicStep(voltage_));
        }

        break;
      default:
        std::cerr
          << "File_based_stochastic_voltage_gated_channel::StepCurrent"
          << " - ERROR : Unsupported simulation mode."
          << std::endl;
        return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
        break;
    }
    return (modigliani_base::ReturnEnum::FAIL);
  }

  inline modigliani_base::
    Real File_based_stochastic_voltage_gated_channel::OpenChannels() const {
    return (channels_ptr_->NumOpen());
  }

  inline modigliani_base::
    Real File_based_stochastic_voltage_gated_channel::ComputeConductance() {
    return (set_conductance(channels_ptr_->NumOpen() * conductivity()));
  }

  inline modigliani_base::
    Real File_based_stochastic_voltage_gated_channel::ComputeTimeConstant()
    const {
    return (channels_ptr_->ComputeChannelStateTimeConstant(voltage_));
  }
}  // namespace modigliani_core
