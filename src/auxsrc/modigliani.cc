/**
 * @file modigliani.cc
 * @brief Can simulate arbitrary axons using configuration given in a
 * json file.
 *
 * Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
 *
 * Copyright (C) 2016 Mohammad Ali Neishabouri
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <modigliani/aux_func.h>
#include <modigliani/aux_math_func.h>

#include <boost/program_options.hpp>
#include <boost/progress.hpp>
#include <boost/timer.hpp>
#include <boost/property_tree/exceptions.hpp>

#ifdef WITH_PLPLOT
# include <plplot/plplot.h>
# include <plplot/plstream.h>
#endif // ifdef WITH_PLPLOT

#include <vector>
#include <sstream>

/**
 * \brief Runs a simulation using parameters in the given json file.
 *
 * \param vm Command line options
 * \return Status
 */
int Simulate(boost::program_options::variables_map vm) {
  using modigliani::Size;
  Size numCompartments;
  boost::property_tree::ptree config_root;
  try {
    read_json(vm["config-file"].as<string>(), config_root);
  } catch (const exception& e) {
    // report to the user the failure and their locations in the document.
    std::cerr << "Failed to parse configuration\n" << e.what();
    exit(1);
  }
  string timedOutputFolder;

  // What compartments to save
  auto electrods_vec = modigliani::GetElectrods(config_root);

  // We write each compartment's potential and currents into a single file.
  ofstream TimeFile, LengthPerCompartmentFile, TypePerCompartmentFile, log_file;

  if (config_root.get<Size>("simulation_parameters.sampN") > 0) {
    timedOutputFolder = modigliani::CreateOutputFolder(
      config_root.get<string>("simulation_parameters.outputFolder"));

    std::ifstream ifs(vm["config-file"].as<string>(), std::ios::binary);
    string temp_string = timedOutputFolder;
    std::ofstream ofs(temp_string.append("/conf.json").c_str(),
                      std::ios::binary);

    ofs << ifs.rdbuf();
    ofs.close();
    ifs.close();

    modigliani::OpenOutputFile(timedOutputFolder, "Time", TimeFile);
    modigliani::OpenOutputFile(timedOutputFolder, "TypePerCompartment",
                                    TypePerCompartmentFile);
    modigliani::OpenOutputFile(timedOutputFolder, "LengthPerCompartment",
                                    LengthPerCompartmentFile);
    modigliani::OpenOutputFile(timedOutputFolder, "log", log_file, ".log");
    TimeFile << "% in ms" << std::endl;
  } else {
    modigliani::OpenOutputFile("/tmp", "log", log_file, ".log");
  }

  lua_State *L_inject_current = luaL_newstate();
  std::vector<float> inputData(1000000);

  // We can inject currents in two ways. Either execute a lua program,
  // or read from a file. The choice is made depending on the presence of
  // input-file in the command line interface.
  if (vm.count("input-file")) {
    // Read input file only once. Store its content in memory.
    ifstream dataFile(vm["input-file"].as<string>());

    if (dataFile.fail()) {
      std::cerr
        << "Could not open input file "
        << config_root.get<string>("simulation_parameters.inputFile").c_str()
        << std::endl;
      exit(1);
    }

    modigliani::Size index = 0;

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
    string lua_inject_script = config_root.get<string>(
      "simulation_parameters.inject_current_lua");
    luaL_openlibs(L_inject_current);
    luaL_dostring(L_inject_current, lua_inject_script.c_str());
  }

  bool verbose = false;

  if (vm.count("verbose")) {
    verbose = true;
  }

  bool show_bar = false;

  if (vm.count("progressbar")) {
    show_bar = true;
  }

#ifdef WITH_PLPLOT
  Size plot = 0;

  if (vm.count("plot")) {
    plot = vm["plot"].as<Size>();
  }
#endif // ifdef WITH_PLPLOT

  Size force_alg = 0;

  if (vm.count("algorithm")) {
    force_alg = vm["algorithm"].as<int>();
  }

  Size num_trials = 1;
  try {
    num_trials = config_root.get<Size>("simulation_parameters.numTrials");
  } catch (const boost::property_tree::ptree_bad_path& e) {
    std::cout << "Warning : numTrials not found in simulation parameters."
              << std::endl;
  }
  // Do we want to simulate an electrod injecting current somewhere?
  lua_State *L_change_potential = luaL_newstate();
  bool change_potentials        = true;
  try {
    string lua_change_potential_script = config_root.get<string>(
      "simulation_parameters.change_potential_lua");
    luaL_openlibs(L_change_potential);
    luaL_dostring(L_change_potential, lua_change_potential_script.c_str());
    change_potentials = true;
  } catch (const boost::property_tree::ptree_bad_path& e) {
    // No need to change potentials
    change_potentials = false;
    lua_close(L_change_potential);
  }
  auto output_files                      = vector<string>(0);
  boost::progress_display *show_progress = 0;

  /* *** Trials loop *** */
  for (modigliani::Size lTrials = 0; lTrials < num_trials; lTrials++) {
    modigliani::Real inp_current = 0;

    /* Model setup */
    modigliani::Membrane_compartment_sequence *oModel =
      modigliani::CreateAxon(config_root, TypePerCompartmentFile,
                                  LengthPerCompartmentFile, force_alg);

    if (!lTrials) {
      TypePerCompartmentFile.close();
      LengthPerCompartmentFile.close();
    }
    oModel->Init();

#ifdef WITH_PLPLOT
    PLFLT voltVec[oModel->num_compartments()];
    PLFLT x[oModel->num_compartments()];
    x[0] = 0;
#endif // ifdef WITH_PLPLOT

    numCompartments = oModel->num_compartments();
    log_file << "Total number of compartments(in oModel)" << numCompartments
             << std::endl;

    // std::vector<modigliani::Real> leakCurrVec(numCompartments);
    // std::vector<modigliani::Real> naCurrVec(numCompartments);
    // std::vector<modigliani::Real> kCurrVec(numCompartments);

#ifdef WITH_PLPLOT
    /* Graphics init */
    plstream *pls = 0;

    if (plot > 0) {
      pls = new plstream();

      // Initialize plplot.
      // pls->sdev("qt");
      pls->scolbg(255, 255, 255);
      pls->scol0(1, 0, 0, 0);
      pls->init();
    }
#endif // ifdef WITH_PLPLOT

    std::cerr << "MainLoop started" << std::endl;

    if (show_bar && (show_progress == 0))
      show_progress = new boost::progress_display(
        config_root.get<Size>("simulation_parameters.numIter") * num_trials
        / 100);

    modigliani::Real timeInMS = 0;
    int dataRead                   = 0;

    for (modigliani::Size lt = 0;
         lt < config_root.get<Size>("simulation_parameters.numIter"); lt++) {
      timeInMS += oModel->timestep();

      // Write number of columns
      if ((config_root.get<int>("simulation_parameters.sampN") > 0) && (lt == 0)
          && (lTrials == 0)) {
        modigliani::Size counter = 0;

        for (auto ci = electrods_vec.begin(); ci != electrods_vec.end(); ci++) {
          if (*ci >= oModel->compartment_vec_.size())
            std::cerr
              << "Warning : Electrod requested in non existing compartment "
              << *ci << " ignored." << std::endl;

          stringstream ss(stringstream::in | stringstream::out);
          ss << timedOutputFolder << "/compartments/" << "compartment" << "_"
             << counter << ".bin";
          string temp_name;
          ss >> temp_name;
          output_files.push_back(temp_name);
          oModel->compartment_vec_[*ci]->SetupOutput(temp_name);
          counter++;
        }
      }

      if ((config_root.get<int>("simulation_parameters.sampN") > 0) && (lt == 0)
          && (lTrials != 0)) {
        modigliani::Size counter = 0;

        for (auto ci : electrods_vec) {
          oModel->compartment_vec_[ci]->SetupOutput(output_files[counter++]);
        }
      }

      // the "sampling ratio" used for "measurement" to disk
      if ((config_root.get<int>("simulation_parameters.sampN") > 0)
          && ((lt + 1) % config_root.get<int>("simulation_parameters.sampN") ==
              0)) {
        for (auto ci = electrods_vec.begin(); ci != electrods_vec.end(); ci++) {
          oModel->compartment_vec_[*ci]->WriteOutput();
        }

        if (!lTrials) TimeFile << timeInMS << std::endl;
      }

#ifdef WITH_PLPLOT

      if (plot > 0) {
        if (lt == 0) {
          for (modigliani::Size lc = 1; lc < numCompartments; lc++) {
            x[lc] = x[lc - 1] + oModel->compartment_vec_[lc]->length();
          }
          pls->env(0, x[numCompartments - 1], -100, 100, 0, 0);
        }

        if (lt % plot == 0) {
          pls->clear();
          pls->box("abcnt", 0, 0, "anvbct", 0, 0);

          for (modigliani::Size ll = 0; ll < oModel->num_compartments();
               ll++) {
            voltVec[ll] = oModel->compartment_vec_[ll]->vm();
          }

          for (auto iv : voltVec) {
            if (modigliani::IsNAN(iv)) {
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
          pls->line((PLINT)oModel->num_compartments(), x, voltVec);
          pls->flush();
        }
      }
#endif // ifdef WITH_PLPLOT

      if (vm.count("input-file")) {
        if (lt % config_root.get<int>("simulation_parameters.readN") == 0) {
          inp_current = (inputData[dataRead]
                         * config_root.get<double>("simulation_parameters.inpISDV"))
                        + config_root.get<double>("simulation_parameters.inpI");
          dataRead++;
          cout << inp_current << endl;
        }
      } else {
        lua_getglobal(L_inject_current, "current");
        lua_pushnumber(L_inject_current, timeInMS);
        lua_call(L_inject_current, 1, 1);
        inp_current = lua_tonumber(L_inject_current, -1);
        lua_pop(L_inject_current, 1);
      }

      if (verbose) cout << lt << "\t" << inp_current << endl;
      oModel->InjectCurrent(inp_current, 1);

      if (change_potentials) {
        for (modigliani::Size lc = 0; lc < numCompartments; lc++) {
          // Get value from lua
          lua_getglobal(L_change_potential, "change_in_potential");
          lua_pushnumber(L_change_potential, timeInMS);
          lua_pushnumber(L_change_potential, lc);
          lua_call(L_change_potential, 2, 1);
          auto change_in_potential = lua_tonumber(L_change_potential, -1);
          lua_pop(L_change_potential, 1);
          oModel->compartment_vec_[lc]->set_vm(
            oModel->compartment_vec_[lc]->vm() + change_in_potential);
        }
      }

      oModel->Step();

      if (show_bar && (lt % 100 == 0)) show_progress->operator++();
    }
#ifdef WITH_PLPLOT

    if (pls) {
      delete pls;
    }
#endif // ifdef WITH_PLPLOT
    delete oModel;
  } // lTrials
  lua_close(L_inject_current);
  log_file << "Simulation completed." << std::endl;
  log_file.close();

  return 0;
}

int main(int argc, char *argv[]) {
  using modigliani::Real;
  using modigliani::Size;
  namespace po = boost::program_options;

  // Declare supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
    "config-file", po::value<string>(), "which configuration file to use")(
    "algorithm,a", po::value<int>(), "set algorithm")("trials,t",
                                                      po::value<Size>(),
                                                      "set number of trials")(
    "verbose,v", "activate debug messages")("input-file,i",
                                            po::          value<string>(),
                                            "set input file")(
    "progressbar,b", "Show a progress bar")
#ifdef WITH_PLPLOT
  ("plot,p", po::value<Size>(), "plot every <arg> step.")
#endif // ifdef WITH_PLPLOT
  ;

  if (argc < 2) {
    cout << desc << "\n";
    return 1;
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
    return 0;
  }

  return Simulate(vm);
}
