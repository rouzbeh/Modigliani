/**
 * @file axon_simulator.cc
 * Can simulate arbitrary axons
 * @version  2.0
 * @author Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
 * @author Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
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

#ifdef WITH_PLPLOT
#include <plplot/plplot.h>
#include <plplot/plstream.h>
#endif
#include <sstream>

#include <modigliani_core/aux_func.h>
#include <boost/program_options.hpp>

/**
 * Runs a simulation using parameters in the given json file.
 *
 * @param filename
 * @return Status
 */
int Simulate(string fileName, bool verbose, string input,
             modigliani_base::Size plot) {
  modigliani_base::Size numCompartments;
  Json::Value config_root = modigliani_core::read_config(fileName);
  string timedOutputFolder;
  // What compartments to save
  auto electrods_vec = modigliani_core::get_electrods(config_root);
  // We write each compartment's potential and currents into a single file.
  ofstream TimeFile, LengthPerCompartmentFile, TypePerCompartmentFile, log_file;

  std::vector<ofstream*> pot_current_files;
  if (config_root["simulation_parameters"].get("sampN", 0).asUInt() > 0) {
    timedOutputFolder = modigliani_core::createOutputFolder(
        config_root["simulation_parameters"]["outputFolder"].asString());

    std::ifstream ifs(fileName, std::ios::binary);
    string temp_string = timedOutputFolder;
    std::ofstream ofs(temp_string.append("/conf.json").c_str(),
                      std::ios::binary);

    ofs << ifs.rdbuf();
    ofs.close();
    ifs.close();

    modigliani_core::openOutputFile(timedOutputFolder, "Time", TimeFile);
    modigliani_core::openOutputFile(timedOutputFolder, "TypePerCompartment",
                                    TypePerCompartmentFile);
    modigliani_core::openOutputFile(timedOutputFolder, "LengthPerCompartment",
                                    LengthPerCompartmentFile);
    modigliani_core::openOutputFile(timedOutputFolder, "log", log_file, ".log");
    TimeFile << "% in ms" << std::endl;

    for_each(
        electrods_vec.begin(), electrods_vec.end(),
        [&pot_current_files, timedOutputFolder](modigliani_base::Size ll) {
          pot_current_files.push_back(
              modigliani_core::openOutputFile(timedOutputFolder, "compartment",
                  ll, ".bin"));
        });
  } else {
    modigliani_core::openOutputFile("/tmp", "log", log_file, ".log");
  }

  lua_State* L_inject_current = luaL_newstate();
  std::vector<float> inputData(1000000);
  if (input == "lua") {
    string lua_inject_script =
        config_root["simulation_parameters"]["inject_current_lua"].asString();
    luaL_openlibs(L_inject_current);
    luaL_dostring(L_inject_current, lua_inject_script.c_str());
  } else {
    // Read input file only once. Store its content in memory.
    ifstream dataFile(
        config_root["simulation_parameters"]["inputFile"].asString().c_str(),
        ios::binary);
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
  }

  /* *** Trials loop *** */
  for (modigliani_base::Size lTrials = 0;
      lTrials < config_root["simulation_parameters"]["numTrials"].asUInt();
      lTrials++) {
    /* Model setup */
    modigliani_core::Membrane_compartment_sequence* oModel =
        modigliani_core::create_axon(config_root, TypePerCompartmentFile,
                                     LengthPerCompartmentFile);

    if (!lTrials) {
      TypePerCompartmentFile.close();
      LengthPerCompartmentFile.close();
    }
    oModel->Init();

#ifdef WITH_PLPLOT
    PLFLT voltVec[oModel->_numCompartments()];
    PLFLT x[oModel->_numCompartments()];
    x[0] = 0;
#endif

    numCompartments = oModel->_numCompartments();
    log_file << "Total number of compartments(in oModel)" << numCompartments
             << std::endl;
    std::vector<modigliani_base::Real> leakCurrVec(numCompartments);
    std::vector<modigliani_base::Real> naCurrVec(numCompartments);
    std::vector<modigliani_base::Real> kCurrVec(numCompartments);

#ifdef WITH_PLPLOT
    /* Graphics init */
    plstream* pls = 0;
    if (plot > 0) {
      pls = new plstream();
      // Initialize plplot.
      pls->sdev("wxwidgets");
      pls->scolbg(255, 255, 255);
      pls->scol0(1, 0, 0, 0);
      pls->init();
    }
#endif

    /* *** SIMULATION ITERATION LOOP *** */
    std::cerr << "MainLoop started" << std::endl;

    modigliani_base::Uniform_rnd_dist uniformRnd;

    modigliani_base::Real timeInMS = 0;
    int dataRead = 0;
    for (modigliani_base::Size lt = 0;
        lt < config_root["simulation_parameters"]["numIter"].asUInt(); lt++) {
      timeInMS += oModel->_timeStep();
      //timeVar = timeInMS;
      // Write number of columns
      if (config_root["simulation_parameters"]["sampN"].asInt() > 0 && lt == 0
          && lTrials == 0) {
        modigliani_base::Size counter = 0;
        for (auto ci = electrods_vec.begin(); ci != electrods_vec.end(); ci++) {
          // For each current, we write the current and number of open channels
          modigliani_base::Size number_of_columns = 2
              * oModel->compartmentVec[*ci]->NumberCurrents() + 1;
          pot_current_files[counter++]->write(
              reinterpret_cast<char*>(&number_of_columns),
              sizeof(modigliani_base::Size));
        }
      }
      // the "sampling ratio" used for "measurement" to disk
      if (config_root["simulation_parameters"]["sampN"].asInt() > 0
          && lt % config_root["simulation_parameters"]["sampN"].asInt() == 0) {
        modigliani_base::Size counter = 0;
        for (auto ci = electrods_vec.begin(); ci != electrods_vec.end(); ci++) {
          oModel->WriteCompartmentData(pot_current_files[counter++], *ci);
        }
        if (!lTrials) TimeFile << timeInMS << std::endl;
      }
#ifdef WITH_PLPLOT
      if (plot > 0) {
        if (lt == 0) {
          for (modigliani_base::Size lc = 1; lc < numCompartments; lc++) {
            x[lc] = x[lc - 1] + oModel->compartmentVec[lc]->_length();
          }
          pls->env(0, x[numCompartments - 1], -100, 100, 0, 0);
        }
        if (lt % plot == 0) {
          pls->clear();
          pls->box("abcnt", 0, 0, "anvbct", 0, 0);
          for (modigliani_base::Size ll = 0; ll < oModel->_numCompartments();
              ll++) {
            voltVec[ll] = oModel->compartmentVec[ll]->vm();
          }
          for (auto iv : voltVec) {
            if (modigliani_base::Misnan(iv)) {
              log_file << "ERROR at t=" << timeInMS << " voltage is NaN."
                       << std::endl;
              std::exit(1);
            } else if (iv > 200.0 /* mV */) {
              log_file << "ERROR at t=" << timeInMS
                       << " voltage in compartment  is " << iv << "."
                       << std::endl;
              std::exit(1);
            }
          }
          pls->line((PLINT) oModel->_numCompartments(), x, voltVec);
          pls->flush();
        }
      }
#endif
      modigliani_base::Real inp_current = 0;
      if (input == "lua") {
        lua_getglobal(L_inject_current, "current");
        lua_pushnumber(L_inject_current, timeInMS);
        lua_call(L_inject_current, 1, 1);
        inp_current = lua_tonumber(L_inject_current, -1);
        lua_pop(L_inject_current, 1);
      } else {
        if (lt % config_root["simulation_parameters"]["readN"].asInt() == 0) {
          inp_current = (inputData[dataRead]
              * config_root["simulation_parameters"]["inpISDV"].asDouble())
              + config_root["simulation_parameters"]["inpI"].asDouble();
          dataRead++;
        }
      }
      if (verbose) cout << lt << "\t" << inp_current << endl;
      oModel->InjectCurrent(inp_current, 1);
      oModel->step();
    }
#ifdef WITH_PLPLOT
    if (pls) delete pls;
#endif
    delete oModel;
    lua_close(L_inject_current);
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
  desc.add_options()("help", "produce help message")(
      "config-file", po::value<string>(), "which configuration file to use")(
      "algorithm,a", po::value<int>(), "set algorithm")("trials,t",
                                                        po::value<Size>(),
                                                        "set number of trials")(
      "verbose,v", "activate debug messages")("output-folder,o",
                                              po::value<string>(),
                                              "set output folder")(
      "input-file,i", po::value<string>(), "set input file")(
      "plot,p", po::value<Size>(), "plot every <arg> step.");

  po::positional_options_description p;
  p.add("config-file", -1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(desc).positional(p).run(),
      vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return (1);
  }

  bool verbose = false;
  if (vm.count("verbose")) {
    verbose = true;
  }

  string input = "lua";
  if (vm.count("input-file")) {
    input = vm["input-file"].as<string>();
  }

  Size plot = 0;
  if (vm.count("plot")) {
    plot = vm["plot"].as<Size>();
  }
  return (Simulate(vm["config-file"].as<string>(), verbose, input, plot));
}