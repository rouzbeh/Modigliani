/**
 * \file resting_potential.cc
 * \brief Find the resting potential of an axon by running simulations
 *
 * This program reads an axon configuration from a json file.
 * The axon is initialised and simulated for a certain number of
 * iterations. The last 5000 iterations are avergaed together to
 * provide a resting potential, which compared against the target.
 * The leak reversal potential is then modified appropriately and
 * the simulations are repeated until the target is matched.
 * \version  2.0
 * \author Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
 *
 * \section LICENSE
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

#include <modigliani_core/aux_func.h>
#include <boost/program_options.hpp>

int main(int argc, char* argv[]) {

  namespace po = boost::program_options;
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "config-file", po::value<string>(), "which configuration file to use")(
      "target,t", po::value<double>(),
      "target reting potential in mV (default -65mV)")(
      "duration,d", po::value<modigliani_base::Size>(),
      "How many mS for each value")("verbose,v", "activate debug messages");

  if (argc < 2) {
    cout << desc << "\n";
    return (1);
  }

  po::positional_options_description p;
  p.add("config-file", -1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(desc).positional(p).run(),
      vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return (0);
  }

  string filename = vm["config-file"].as<string>();
  Json::Value config_root = modigliani_core::read_config(filename);

  modigliani_base::Real target = -65;
  if (vm.count("potential")) {
    target = vm["target"].as<modigliani_base::Real>();
  }

  modigliani_base::Size duration = 50.0
      / config_root["simulation_parameters"]["timeStep"].asDouble();
  if (vm.count("duration")) {
    duration = vm["duration"].as<modigliani_base::Size>() * 1.0
        / config_root["simulation_parameters"]["timeStep"].asDouble();
  }

  double min = -90;
  double max = -20;
  double current_guess;
  double current_result = 100000;

  ofstream temp;
  temp.open("/dev/null");

  while (current_result > target + 0.1 || current_result < target - 0.1) {
    current_guess = (min + max) / 2;
    std::cout << "Trying with " << current_guess << std::endl;

    config_root["eLeak"] = current_guess;
    std::vector<modigliani_base::Size> nodes_vec, nodes_paranodes_vec;

    modigliani_core::Membrane_compartment_sequence* oModel =
        modigliani_core::create_axon(config_root, temp, temp, 1);

    oModel->Init();

    double sum = 0;
    for (modigliani_base::Size lt = 0; lt < duration; lt++) {
      if (lt <= 5000) {
        modigliani_base::Real inpCurrent = (5
            * config_root["simulation_parameters"]["inpISDV"].asDouble())
            + config_root["simulation_parameters"]["inpI"].asDouble();
        oModel->InjectCurrent(inpCurrent, 1);
      }

      // The trouble is to know if this is an action potential
      if (duration - lt < 50)
        for (modigliani_base::Size ll = 0; ll < oModel->_numCompartments();
            ll++) {
          sum += oModel->compartmentVec[ll]->vm();
        }
      oModel->step();
    }
    current_result = sum / (oModel->_numCompartments() * 50);
    std::cerr << "Mean voltage = " << current_result << " found for eLeak = "
              << current_guess << " max,min" << max << ',' << min << std::endl;
    if (current_result > target) {
      max = current_guess;
    }
    if (current_result < target) {
      min = current_guess;
    }

    delete oModel;
  }
  std::cout << current_guess << std::endl;
  return (0);
}
