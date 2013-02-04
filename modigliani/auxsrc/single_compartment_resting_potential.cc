/**
 * \file single_compartment_resting_potential.cc
 * \brief Find the resting potential of a compartment
 * by running simulations
 *
 * This program reads an axon configuration from a json file.
 * \version  2.0
 * \author Copyright (C) 2011, 2012, 2013 Mohammad Ali Neishabouri
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
  desc.add_options()("help,h", "produce help message")("algorithm,a",
                                                       po::value<int>(),
                                                       "set algorithm")(
      "config-file", po::value<string>(), "which configuration file to use")(
      "target,t", po::value<double>(),
      "target resting potential in mV (default -65mV)")(
      "compartment-type,c", po::value<modigliani_base::Size>(),
      "Which compartment type to simulate (default 0)")(
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

  modigliani_base::Size force_alg = 0;
  if (vm.count("algorithm")) {
    force_alg = vm["algorithm"].as<int>();
  }

  string filename = vm["config-file"].as<string>();
  Json::Value config_root = modigliani_core::read_config(filename);

  modigliani_base::Size compartment_type = 0;
  if (vm.count("compartment-type")) {
    compartment_type = vm["compartment-type"].as<modigliani_base::Size>();
  }
  // We need to remove possible attached leak currents from this
  Json::Value compartment_parameters =
      config_root["compartments_parameters"][compartment_type];
  modigliani_base::Real gleak;
  Json::Value currents = compartment_parameters["currents"];
  for (unsigned int index = 0; index < currents.size(); ++index) {
    Json::Value current = currents[index];
    if ("leak" == current["type"].asString()) {
      gleak = compartment_parameters["currents"][index]["GLeak"].asDouble();
      compartment_parameters["currents"][index]["GLeak"] = 0;
      continue;
    }
  }

  // Has to be big enough so that we definitely have channels in this compartment.
  // TODO : Calculate it !
  compartment_parameters["length"] = 500;

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

  modigliani_core::Cylindrical_compartment* oModel =
      modigliani_core::create_compartment(config_root,
                                          config_root["simulation_parameters"],
                                          compartment_parameters, force_alg);

  for (modigliani_base::Size lt = 0; lt < duration; lt++) {
    if (lt <= 5000) {
      oModel->Step(0);
    } else {
      oModel->Step(target);
    }
  }

  modigliani_base::Real sum_currents = 0;
  for (modigliani_base::Size ll = 2; ll - 1 < oModel->NumberCurrents(); ++ll) {
    cout << oModel->Current(ll)->current() << endl;
    sum_currents += oModel->Current(ll)->current();
  }
  cout << "Final currents sum " << sum_currents << endl;
  modigliani_base::Real calculated_eLeak = sum_currents / gleak + target;

  cout << "Eleak computed as " << calculated_eLeak << " mV" << endl;

  delete oModel;
  return (0);
}
