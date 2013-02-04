/**
 * @file network.cc
 * \brief This executable simulates a network of HH neurons.
 *
 * The configuration of each neurons is given in a JSON file similar, but
 * not identical, to the one used for axon_simulator.
 *
 * \version  0.1
 * \author Copyright (C) 2013, Mohammad Ali Neishabouri
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

#include <modigliani_core/aux_func.h>
#include <modigliani_core/network_synapse.h>
#include <modigliani_core/spherical_compartment.h>

#include <boost/program_options.hpp>
#include <sstream>

/**
 * \brief Runs a simulation using parameters in the given json file.
 *
 * \param vm Command line options
 * \return Status
 */
int Simulate(boost::program_options::variables_map vm) {
  using modigliani_base::Size;
  using modigliani_base::Real;
  using modigliani_core::Spherical_compartment;
  using modigliani_core::Network_synapse;
  using std::vector;

  Size num_neurons;
  Json::Value config_root = modigliani_core::read_config(
      vm["config-file"].as<string>());
  string timedOutputFolder;
  // We write each neuron's potential and currents into a single file.
  ofstream time_file, log_file;

  std::vector<ofstream*> pot_current_files;
  if (config_root["simulation_parameters"].get("sampN", 0).asUInt() > 0) {
    timedOutputFolder = modigliani_core::createOutputFolder(
        config_root["simulation_parameters"]["outputFolder"].asString());

    std::ifstream ifs(vm["config-file"].as<string>(), std::ios::binary);
    string temp_string = timedOutputFolder;
    std::ofstream ofs(temp_string.append("/conf.json").c_str(),
                      std::ios::binary);

    ofs << ifs.rdbuf();
    ofs.close();
    ifs.close();

    modigliani_core::openOutputFile(timedOutputFolder, "Time", time_file);
    modigliani_core::openOutputFile(timedOutputFolder, "log", log_file, ".log");
    time_file << "% in ms" << std::endl;

    for (int i = 0; i < num_neurons; i++) {
      pot_current_files.push_back(
          modigliani_core::openOutputFile(timedOutputFolder, "neuron", i,
                                          ".bin"));
    };
  } else {
    modigliani_core::openOutputFile("/tmp", "log", log_file, ".log");
  }

  vector<vector<float>> input_data(0);
  Json::Value inputs = config_root["input"];
  for (unsigned int index = 0; index < inputs.size(); ++index) {
    Json::Value input = inputs[index];
    input_data.push_back(vector<float>(0));
    ifstream dataFile(input["file"].asString());
    if (dataFile.fail()) {
      std::cerr
          << "Could not open input file "
          << config_root["simulation_parameters"]["inputFile"].asString().c_str()
          << std::endl;
      exit(1);
    }

    while (dataFile.good()) {
      char tmp[100];
      dataFile.getline(tmp, 100);
      std::stringstream convertor(tmp);
      float tmp_data;
      convertor >> tmp_data;
      input_data[index].push_back(tmp_data);
    }
    dataFile.close();
  }

  bool verbose = false;
  if (vm.count("verbose")) {
    verbose = true;
  }

  Size force_alg = 0;
  if (vm.count("algorithm")) {
    force_alg = vm["algorithm"].as<int>();
  }

  Size num_trials = config_root["simulation_parameters"]["numTrials"].asUInt();

  std::vector<Spherical_compartment*> network_vector(0);

  for (Size lTrials = 0; lTrials < num_trials; lTrials++) {
    vector<Real> inp_current(inputs.size());
    for (int i = 0; i < num_neurons; i++) {
      Spherical_compartment* tmp_ptr = new Spherical_compartment(
          config_root["neuron_parameters"]);
      tmp_ptr->update_timeStep(
          config_root["simulation_parameters"]["timeStep"].asDouble());
      bool randomise_densities =
          config_root["simulation_parameters"]["randomise_densities"].asBool();

      attach_current(tmp_ptr, config_root["neuron_parameters"]["currents"],
                     config_root, randomise_densities, force_alg);
      network_vector.push_back(tmp_ptr);
    }

    // Now we connect neurons
    Json::Value synapses = config_root["synapses"];
    Json::Value synapse_parameters = config_root["synapse_parameters"];
    for (unsigned int index = 0; index < synapses.size(); ++index) {
      Json::Value synapse = synapses[index];
      network_vector[synapse["destination"].asUInt()]->AttachCurrent(
          new Network_synapse(
              synapse_parameters[synapse["type"].asUInt()]["ReversalPotential"].asDouble(),
              network_vector[synapse["destination"].asUInt()],
              config_root["simulation_parameters"]["timeStep"].asDouble(),
              synapse_parameters[synapse["type"].asUInt()]["Model"].asString(),
              synapse["strength"].asDouble()));
    }

    std::cerr << "MainLoop started" << std::endl;
    modigliani_base::Real timeInMS = 0;
    int dataRead = 0;
    for (modigliani_base::Size lt = 0;
        lt < config_root["simulation_parameters"]["numIter"].asUInt(); lt++) {
      timeInMS += config_root["simulation_parameters"]["timeStep"].asDouble();
      // Write number of columns
      if (config_root["simulation_parameters"]["sampN"].asInt() > 0 && lt == 0
          && lTrials == 0) {
        modigliani_base::Size counter = 0;
        for (auto ci = network_vector.begin(); ci != network_vector.end();
            ci++) {
          modigliani_base::Size number_of_columns = (*ci)->NumberCurrents() + 1;
          pot_current_files[counter++]->write(
              reinterpret_cast<char*>(&number_of_columns),
              sizeof(modigliani_base::Size));
        }
      }

      // the "sampling ratio" used for "measurement" to disk
      if (config_root["simulation_parameters"]["sampN"].asInt() > 0
          && lt % config_root["simulation_parameters"]["sampN"].asInt() == 0) {
        modigliani_base::Size counter = 0;
        for (auto ci = network_vector.begin(); ci != network_vector.end();
            ci++) {
          float* data = (*ci)->data();
          pot_current_files[counter++]->write(reinterpret_cast<char*>(data),
                                              sizeof(data));
        }
        if (!lTrials) time_file << timeInMS << std::endl;
      }

      if (lt % config_root["simulation_parameters"]["readN"].asInt() == 0) {
        for (unsigned int index = 0; index < inputs.size(); ++index) {
          inp_current[index] = (input_data[index][dataRead]);
          dataRead++;
        }
      }

      for (unsigned int index = 0; index < inputs.size(); ++index) {
        network_vector[inputs[index]["neuron"].asUInt()]->InjectCurrent(
            inp_current[index]);
      }
      for (auto ci = network_vector.begin(); ci != network_vector.end(); ci++) {
        (*ci)->Step();
      }
    }

    network_vector.clear();
  }  // lTrials
  log_file << "Simulation completed." << std::endl;
  log_file.close();
  for (auto ci = pot_current_files.begin(); ci != pot_current_files.end();
      ++ci) {
    auto file = *ci;
    file->close();
    delete file;
  }

  return (0);
}

int main(int argc, char* argv[]) {
  using modigliani_base::Real;
  using modigliani_base::Size;
  namespace po = boost::program_options;
// Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "config-file", po::value<string>(), "which configuration file to use")(
      "algorithm,a", po::value<int>(), "set algorithm")("trials,t",
                                                        po::value<Size>(),
                                                        "set number of trials")(
      "verbose,v", "activate debug messages")("input-file,i",
                                              po::value<string>(),
                                              "set input file");
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

  return (Simulate(vm));
}