/**
 * @file single_compartment_simulator.cc
 * \brief Can simulate arbitrary axons
 *
 * @version  2.0
 * @author Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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
/**
 * \brief Runs a simulation using parameters in the given json file.
 *
 * This program only simulates a single compartment. If run using
 * a configuration file made for axon_simulator, the first compartment
 * found in compartment_parameters will be used. electrods_lua and
 * anatomy_lua are not used.
 *
 * \param vm Command line options
 * \return Status
 */
int Simulate(boost::program_options::variables_map vm) {
  using modigliani_base::Size;
  Json::Value config_root = modigliani_core::read_config(
      vm["config-file"].as<string>());
  string timedOutputFolder;

  // We write each compartment's potential and currents into a single file.
  ofstream TimeFile, log_file;

  ofstream* pot_current_file;
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

    modigliani_core::openOutputFile(timedOutputFolder, "Time", TimeFile);
    modigliani_core::openOutputFile(timedOutputFolder, "log", log_file, ".log");
    TimeFile << "% in ms" << std::endl;
    pot_current_file = modigliani_core::openOutputFile(timedOutputFolder,
                                                       "compartment", 0,
                                                       ".bin");
  } else {
    modigliani_core::openOutputFile("/tmp", "log", log_file, ".log");
  }

  lua_State* L_inject_current = luaL_newstate();
  std::vector<float> inputData(1000000);
  if (vm.count("input-file")) {
    // Read input file only once. Store its content in memory.
    ifstream dataFile(vm["input-file"].as<string>());
    if (dataFile.fail()) {
      std::cerr
          << "Could not open input file "
          << config_root["simulation_parameters"]["inputFile"].asString().c_str()
          << std::endl;
      exit(1);
    }
    modigliani_base::Size index = 0;
    while (dataFile.good()) {
      if (index < inputData.size()) {
        char tmp[100];
        dataFile.getline(tmp, 100);
        std::stringstream convertor(tmp);
        convertor >> inputData[index];
        index++;
      } else {
        inputData.resize(inputData.size() + 100000);
      }
    }
    dataFile.close();
  } else {
    string lua_inject_script =
        config_root["simulation_parameters"]["inject_current_lua"].asString();
    luaL_openlibs(L_inject_current);
    luaL_dostring(L_inject_current, lua_inject_script.c_str());
  }

  bool verbose = false;
  if (vm.count("verbose")) {
    verbose = true;
  }

  Size force_alg = 0;
  if (vm.count("algorithm")) {
    force_alg = vm["algorithm"].as<int>();
  }

  // Trials loop
  for (modigliani_base::Size lTrials = 0;
      lTrials < config_root["simulation_parameters"]["numTrials"].asUInt();
      lTrials++) {
    modigliani_base::Real inp_current = 0;
    /* Model setup */
    Json::Value compartments_parameters = config_root["compartments_parameters"];
    Json::Value simulation_parameters = config_root["simulation_parameters"];
    modigliani_core::Cylindrical_compartment* oModel =
        modigliani_core::create_compartment(config_root, simulation_parameters,
                                            compartments_parameters[0u],
                                            force_alg);

    // SIMULATION ITERATION LOOP
    std::cerr << "MainLoop started" << std::endl;
    float timeVar = 0;
    modigliani_base::Real inpCurrent = 0.0;

    modigliani_base::Uniform_rnd_dist uniformRnd;

    modigliani_base::Real timeInMS = 0;
    int dataRead = 0;
    for (modigliani_base::Size lt = 0;
        lt < config_root["simulation_parameters"]["numIter"].asUInt(); lt++) {
      timeInMS += oModel->_timeStep();
      timeVar = timeInMS;
      // Write number of columns
      if (config_root["simulation_parameters"]["sampN"].asInt() > 0 && lt == 0
          && lTrials == 0) {
        modigliani_base::Size number_of_currents = oModel->NumberCurrents() + 1;
        pot_current_file->write(reinterpret_cast<char*>(&number_of_currents),
                                sizeof(modigliani_base::Size));
      }
      /* the "sampling ratio" used for "measurement" to disk */
      if (config_root["simulation_parameters"]["sampN"].asInt() > 0
          && lt % config_root["simulation_parameters"]["sampN"].asInt() == 0) {
        modigliani_base::Size number_of_currents = oModel->NumberCurrents();
        float data[1 + number_of_currents];
        data[0] = oModel->vm();
        for (modigliani_base::Size ll = 1; ll - 1 < number_of_currents; ++ll) {
          data[ll] = oModel->Current(ll)->current();
        }
        pot_current_file->write(reinterpret_cast<char*>(data),
                                (1 + number_of_currents) * sizeof(float));
      }
      if (!lTrials) TimeFile << timeVar << std::endl;

      if (lt % config_root["simulation_parameters"]["readN"].asInt() == 0) {
        inpCurrent = (inputData[dataRead]
            * config_root["simulation_parameters"]["inpISDV"].asDouble())
            + config_root["simulation_parameters"]["inpI"].asDouble();
        dataRead++;
      }
      if (vm.count("input-file")) {
        if (lt % config_root["simulation_parameters"]["readN"].asInt() == 0) {
          inp_current = (inputData[dataRead]
              * config_root["simulation_parameters"]["inpISDV"].asDouble())
              + config_root["simulation_parameters"]["inpI"].asDouble();
          dataRead++;
          cout << config_root["simulation_parameters"]["inpI"].asDouble()
               << endl;
        }
      } else {
        lua_getglobal(L_inject_current, "current");
        lua_pushnumber(L_inject_current, timeInMS);
        lua_call(L_inject_current, 1, 1);
        inp_current = lua_tonumber(L_inject_current, -1);
        lua_pop(L_inject_current, 1);
      }

      oModel->Step();
    }

    delete oModel;
  }  // lTrials
  log_file << "Simulation completed." << std::endl;
  log_file.close();
  pot_current_file->close();
  delete pot_current_file;

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
