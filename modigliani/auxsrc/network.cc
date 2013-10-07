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

  boost::property_tree::ptree config_root;
  try {
    read_json(vm["config-file"].as<string>(), config_root);
  } catch(const exception &e) {
    // report to the user the failure and their locations in the document.
    std::cerr << "Failed to parse configuration\n" << e.what();
    exit(1);
  }

  Size num_neurons = config_root.get<Size>("num_neurons");
  string timedOutputFolder;
  // We write each neuron's potential and currents into a single file.
  ofstream time_file, log_file;

  std::vector<ofstream*> pot_current_files;
  if (config_root.get<Size>("simulation_parameters.sampN") > 0) {
    timedOutputFolder = modigliani_core::CreateOutputFolder(
        config_root.get<string>("simulation_parameters.outputFolder"));

    std::ifstream ifs(vm["config-file"].as<string>(), std::ios::binary);
    string temp_string = timedOutputFolder;
    std::ofstream ofs(temp_string.append("/conf.json").c_str(),
                      std::ios::binary);

    ofs << ifs.rdbuf();
    ofs.close();
    ifs.close();

    modigliani_core::OpenOutputFile(timedOutputFolder, "Time", time_file);
    modigliani_core::OpenOutputFile(timedOutputFolder, "log", log_file, ".log");
    time_file << "% in ms" << std::endl;

    for (int i = 0; i < num_neurons; i++) {
      pot_current_files.push_back(
          modigliani_core::OpenOutputFile(timedOutputFolder, "neuron", i,
                                          ".bin"));
    };
  } else {
    modigliani_core::OpenOutputFile("/tmp", "log", log_file, ".log");
  }

  vector<vector<float>> input_data(0);
  std::vector<boost::property_tree::ptree> inputs(0);
  for (boost::property_tree::ptree::value_type const &v : config_root.get_child(
          "input")) {
    inputs.push_back(v.second);
  }
  for (unsigned int index = 0; index < inputs.size(); ++index) {
    boost::property_tree::ptree input = inputs[index];
    input_data.push_back(vector<float>(0));
    ifstream dataFile(input.get<string>("file"));
    if (dataFile.fail()) {
      std::cerr
          << "Could not open input file "
          << config_root.get<string>("simulation_parameters.inputFile").c_str()
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

  Size num_trials = config_root.get<Size>("simulation_parameters.numTrials");

  std::vector<Spherical_compartment*> network_vector(0);

  for (Size lTrials = 0; lTrials < num_trials; lTrials++) {
    vector<Real> inp_current(inputs.size());
    for (int i = 0; i < num_neurons; i++) {
      Spherical_compartment* tmp_ptr = new Spherical_compartment(
          config_root.get_child("neuron_parameters"));
      tmp_ptr->update_timeStep(
          config_root.get<double>("simulation_parameters.timeStep"));
      bool randomise_densities = config_root.get<bool>(
          "simulation_parameters.randomise_densities");

      AttachCurrent(tmp_ptr,
                     config_root.get_child("neuron_parameters.currents"),
                     config_root, randomise_densities, force_alg);
      network_vector.push_back(tmp_ptr);
    }

    // Now we connect neurons

    std::vector<boost::property_tree::ptree> synapses(0);
    for (boost::property_tree::ptree::value_type
             const &v: config_root.get_child(
            "synapses")) {
      synapses.push_back(v.second);
    }

    std::vector<boost::property_tree::ptree> synapse_parameters(0);
    for (boost::property_tree::ptree::value_type
             const &v: config_root.get_child(
            "synapse_parameters")) {
      synapse_parameters.push_back(v.second);
    }
    for (unsigned int index = 0; index < synapses.size(); ++index) {
      boost::property_tree::ptree synapse = synapses[index];
      network_vector[synapse.get<Size>("destination")]->AttachCurrent(
          new Network_synapse(
              synapse_parameters[synapse.get<Size>("type")].get<double>(
                  "ReversalPotential"),
              network_vector[synapse.get<Size>("destination")],
              config_root.get<double>("simulation_parameters.timeStep"),
              synapse_parameters[synapse.get<Size>("type")].get<string>(
                  "Model"),
              synapse.get<double>("strength")));
    }

    std::cerr << "MainLoop started" << std::endl;
    modigliani_base::Real timeInMS = 0;
    int dataRead = 0;
    for (modigliani_base::Size lt = 0;
        lt < config_root.get<Size>("simulation_parameters.numIter"); lt++) {
      timeInMS += config_root.get<double>("simulation_parameters.timeStep");
      // Write number of columns
      if (config_root.get<int>("simulation_parameters.sampN") > 0 && lt == 0
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
      if (config_root.get<int>("simulation_parameters.sampN") > 0
          && lt % config_root.get<int>("simulation_parameters.sampN") == 0) {
        modigliani_base::Size counter = 0;
        for (auto ci = network_vector.begin(); ci != network_vector.end();
            ci++) {

          float * data = new float (1 + (*ci)->NumberCurrents());
          data[0] = (*ci)->vm();
          for (unsigned int i = 1; i < 1 + (*ci)->NumberCurrents(); i++) {
            data[i] = (*ci)->Current(i)->current();
          }
          pot_current_files[counter++]->write(reinterpret_cast<char*>(data),
                                              sizeof(data));
        }
        if (!lTrials) time_file << timeInMS << std::endl;
      }

      if (lt % config_root.get<int>("simulation_parameters.readN") == 0) {
        for (unsigned int index = 0; index < inputs.size(); ++index) {
          inp_current[index] = (input_data[index][dataRead]);
          dataRead++;
        }
      }

      for (unsigned int index = 0; index < inputs.size(); ++index) {
        network_vector[inputs[index].get<Size>("neuron")]->InjectCurrent(
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
