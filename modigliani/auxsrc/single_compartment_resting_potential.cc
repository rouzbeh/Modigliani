/**
 * \file single_compartment_resting_potential.cc
 * \brief Find the resting potential of a compartment
 * by running simulations
 *
 * This program reads an axon configuration from a json file.
 * \version  2.0
 * \author Mohammad Ali Neishabouri
 * Copyright (C) 2011, 2012, 2013 Mohammad Ali Neishabouri
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

  boost::property_tree::ptree config_root;
  try {
    read_json(vm["config-file"].as<string>(), config_root);
  } catch(const exception &e) {
    // report to the user the failure and their locations in the document.
    std::cerr << "Failed to parse configuration\n" << e.what();
    exit(1);
  }

  modigliani_base::Size compartment_type = 0;
  if (vm.count("compartment-type")) {
    compartment_type = vm["compartment-type"].as<modigliani_base::Size>();
  }
  // We need to remove possible attached leak currents from this
  std::vector<boost::property_tree::ptree> compartments_parameters(0);

  BOOST_FOREACH(boost::property_tree::ptree::value_type
                const &v, config_root.get_child(
          "compartments_parameters")) {
    compartments_parameters.push_back(v.second);
  }
  modigliani_base::Real gleak;
  boost::property_tree::ptree compartment_parameters =
      compartments_parameters[0];
  boost::property_tree::ptree currents = compartment_parameters.get_child(
      "currents");
  BOOST_FOREACH(boost::property_tree::ptree::value_type const &v, currents) {
    boost::property_tree::ptree current = v.second;
    if ("leak" == current.get<string>("type")) {
      gleak = current.get<double>("GLeak");
      current.put("GLeak", 0);
      continue;
    }
  }

  // Has to be big enough so that we definitely have
  // channels in this compartment.
  // TODO(Ali) Calculate it !
  compartment_parameters.put("length", 500);

  config_root.put("simulation_parameters.randomise_densities", "false");

  modigliani_base::Real target = -65;
  if (vm.count("target")) {
    target = vm["target"].as<modigliani_base::Real>();
  }
  modigliani_base::Size duration = 50.0
      / config_root.get<double>("simulation_parameters.timeStep");
  if (vm.count("duration")) {
    duration = vm["duration"].as<modigliani_base::Size>() * 1.0
        / config_root.get<double>("simulation_parameters.timeStep");
  }

  modigliani_core::Cylindrical_compartment* oModel =
      modigliani_core::CreateCompartment(
          config_root, config_root.get_child("simulation_parameters"),
          compartment_parameters, force_alg);

  cout << target << endl;
  for (modigliani_base::Size lt = 0; lt < duration; lt++) {
    if (lt <= 5000) {
      oModel->Step(0);
    } else {
      oModel->Step(target);
    }
  }

  modigliani_base::Real sum_currents = 0;
  for (modigliani_base::Size ll = 2; ll - 1 < oModel->NumberCurrents(); ++ll) {
    cout << oModel->Current(ll)->current() / oModel->area() << endl;
    sum_currents += oModel->Current(ll)->current();
  }
  cout << "Final currents sum " << sum_currents << endl;
  modigliani_base::Real calculated_eLeak = sum_currents * 100000
      / (gleak * oModel->area()) + target;

  cout << sum_currents * 100000 / (gleak * oModel->area()) << endl;
  cout << "Eleak computed as " << calculated_eLeak << " mV" << endl;

  delete oModel;
  return (0);
}
