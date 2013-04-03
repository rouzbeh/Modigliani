/**
 * @file file_based_stochastic_voltage_gated_channel.h
 * @author man210
 */

#ifndef FILE_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_
#define FILE_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_

#include "voltage_gated_ion_channel_current.h"
#include "ion_channels.h"
#include "aux_func.h"
#include <boost/property_tree/ptree.hpp>
#include <fstream>
#include <iostream>

namespace modigliani_core {
class File_based_stochastic_voltage_gated_channel : public Voltage_gated_ion_channel_current {
  public:
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

    void reset() override {
      channels_ptr_->reset();
    }

    static void load_file(std::string fileName, double temperature,
                          double time_step);
    static std::map<std::string, Transition_rate_matrix*> probability_matrix_map;
    static std::map<std::string, int> number_of_states_map;
    static std::map<std::string, double> base_temperature_map;
    static std::map<std::string, std::vector<modigliani_base::Size> > open_states_map;

    virtual modigliani_base::ReturnEnum StepCurrent() override;
    virtual modigliani_base::Real open_channels() const;
    virtual modigliani_base::Real ComputeConductance() override;
    modigliani_base::Real num_channels_in_state(
        modigliani_base::Size state) const;
    modigliani_base::Real ComputeTimeConstant() const;
    void show_param() const;
    void printProb(std::string fileName) {
      for (modigliani_base::Real v = 20; v < 130; v += 0.005) {
        std::cout
            << probability_matrix_map[fileName]->getTransitionProbability(v, 2,
                                                                          1)
            << std::endl;
      }
    }

  private:
    Ion_channels* channels_ptr_;
    static bool initTableLookUp;
    static std::vector<std::string> initialised_probability_matrices;
    modigliani_base::Real baseTemp;

};
}
#endif /* FILE_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_ */
