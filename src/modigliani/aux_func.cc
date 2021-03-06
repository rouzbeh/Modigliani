/**
 * @file aux_func.cc
 * Auxiliary function implementation for heterogeneous object creation
 *
 * Copyright (C) 1998-2005 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
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

#include "modigliani/aux_func.h"

namespace modigliani {
modigliani::Real CorrectedChannelDensity(modigliani::Real chDensity,
                                              modigliani::
                                              Real                  compArea) {
  modigliani::Real chPerCompartment = compArea * chDensity;
  modigliani::Real pChFloor         =
    (ceil(chPerCompartment) - chPerCompartment);

  boost::random::uniform_01<> uni = boost::random::uniform_01<>();
  boost::random::mt19937 rng      = boost::random::mt19937();

  rng.seed(time(NULL));

  /* compute number of channels, such that average density is achieved */
  modigliani::Real indChDensity =
    (uni(rng) >
     pChFloor ? ceil(chPerCompartment) / compArea : floor(chPerCompartment) /
     compArea);

  return indChDensity;
}

string CreateOutputFolder(string outputFolder) {
  time_t rawtime;
  struct tm   *timeinfo;
  stringstream ss(stringstream::in | stringstream::out);
  char dateString[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  const char *format = "%b%d_%H%M%S";
  size_t max_size    = 80;
  strftime(dateString, max_size, format, timeinfo);
  ss << outputFolder << dateString;
  string folderName;
  ss >> folderName;
  stringstream ss2(stringstream::in | stringstream::out);
  ss2 << folderName << "/compartments/";
  string temp_folder_name;
  ss2 >> temp_folder_name;

  boost::filesystem::path temp_folder_path(temp_folder_name);
  boost::filesystem::create_directories(temp_folder_path.make_preferred().
                                        native());

  return folderName;
}

Membrane_compartment* CreateCompartment(bool                  spherical,
                                        boost::property_tree::
                                        ptree                 config_root,
                                        boost::property_tree::
                                        ptree                 simulation_parameters,
                                        boost::property_tree::
                                        ptree                 compartment_parameters,
                                        modigliani::Size force_alg) {
  modigliani::Membrane_compartment *tmpPtr;

  if (spherical) {
    std::cout << "Spherical" <<std::endl;
    tmpPtr = new modigliani::Spherical_compartment(config_root.get<
                                                      double>("diameter") / 2,
                                                    compartment_parameters.get<
                                                      double>("Cm"),
                                                    config_root.get<
                                                        double>("temperature"));
  } else {
    tmpPtr =
      new modigliani::Cylindrical_compartment(compartment_parameters.get<
                                                     double>("length"),
                                                   config_root.get<
                                                     double>("diameter"),
                                                   compartment_parameters.get<
                                                     double>("Cm"),
                                                   compartment_parameters.get<
                                                     double>("Ra"),
                                                   config_root.get<
                                                     double>("temperature"));
  }

  tmpPtr->set_timestep(simulation_parameters.get<
                         double>("timeStep") /* mSec */);

  bool randomise_densities =
    simulation_parameters.get<bool>("randomise_densities");

  // Read a list of currents for each compartments
  boost::property_tree::ptree currents =
    compartment_parameters.get_child("currents");

  AttachCurrent(tmpPtr, currents, config_root, randomise_densities,
                force_alg);

  return tmpPtr;
}

void AttachCurrent(modigliani::Membrane_compartment *compartment,
                   const boost::property_tree::ptree      currents,
                   boost::property_tree::ptree            config_root,
                   bool                                   randomise_densities,
                   modigliani::Size                  force_alg) {
  for (boost::property_tree::ptree::value_type const& v : currents) {
    // v.first is the name of the child.
    // v.second is the child tree.
    boost::property_tree::ptree current = v.second;

    if (string("leak") == current.get<string>("type")) {
      compartment->
      AttachCurrent(new
                    Leak_current(compartment->area(),
                                 current.get<double>("GLeak"),
                                 config_root.get<double>("eLeak")),
                    LEAK);
      continue;
    }

    if (string("file") == current.get<string>("type")) {
      modigliani::Real indDensity =
        CorrectedChannelDensity(current.get<double>("chDen"),
                                compartment->area());
      File_based_stochastic_voltage_gated_channel *file_current =
        new File_based_stochastic_voltage_gated_channel(compartment->area(),
                                                        (randomise_densities ?
                                                         indDensity : current.
                                                         get<
                                                           double>("chDen")),

/* mum^-2 */
                                                        current.get<
                                                          double>("chCond") *
                                                        1e-9 /* pS */,
                                                        current.get<
                                                          double>("chRevPot")

                                                        /* mV */,
                                                        compartment->
                                                        timestep(),
                                                        config_root.get<
                                                          double
                                                          >("temperature")

                                                        /* C */,
                                                        current.get<string>
                                                          ("chModel"));
      auto alg = current.get<int>("chAlg");

      if (force_alg) alg = force_alg;

      if (4 == alg) file_current->set_simulation_mode(BINOMIALPOPULATION);

      if (2 == alg) file_current->set_simulation_mode(SINGLECHANNEL);
      compartment->AttachCurrent(file_current, IONIC);
      continue;
    }

    if (string("lua") == current.get<string>("type")) {
      auto alg = (force_alg ? force_alg : current.get<int>("chAlg"));

      if (1 == alg) {
        Lua_based_deterministic_voltage_gated_channel *lua_current =
          new Lua_based_deterministic_voltage_gated_channel(compartment->
                                                            area(),
                                                            current.get<
                                                              double>("chDen")

                                                            /* mum^-2 */,
                                                            current.get<
                                                              double>("chCond")
                                                            * 1e-9 /* pS */,
                                                            current.get<
                                                              double
                                                              >("chRevPot")

                                                            /* mV */,
                                                            compartment->
                                                            timestep(),
                                                            config_root.get<
                                                              double
                                                              >("temperature")

                                                            /* C */,
                                                            current.get<
                                                              string>
                                                              ("chModel"));
        lua_current->set_simulation_mode(DETERMINISTIC);
        compartment->AttachCurrent(lua_current, IONIC);
        continue;
      } else if ((4 == alg) || (2 == alg)) {
        modigliani::Real indDensity =
          CorrectedChannelDensity(current.get<double>("chDen"),
                                  compartment->area());
        Lua_based_stochastic_voltage_gated_channel *lua_current =
          new Lua_based_stochastic_voltage_gated_channel(compartment->area(),
                                                         (randomise_densities
                                                          ? indDensity :
                                                          current.get<
                                                            double>("chDen")),
                                                         current.get<
                                                           double>("chCond") *
                                                         1e-9,
                                                         current.get<
                                                           double>("chRevPot")

                                                         /* mV */,
                                                         compartment->
                                                         timestep(),
                                                         config_root.get<
                                                           double
                                                           >("temperature"),
                                                         current.get<
                                                           string>
                                                           ("chModel"));

        if (4 == alg) lua_current->set_simulation_mode(BINOMIALPOPULATION);

        if (2 == alg) lua_current->set_simulation_mode(SINGLECHANNEL);
        compartment->AttachCurrent(lua_current, IONIC);
        continue;
      }
    }
  }
}

void OpenOutputFile(string outputFolder, string prefix,
                    ofstream& outStream, string extension) {
  /* open files */
  stringstream ss(stringstream::in | stringstream::out);

  ss << outputFolder << "/" << prefix << extension;
  string temp_name;
  ss >> temp_name;

  outStream.open(temp_name.c_str(), std::ios::binary);

  if (outStream.fail()) {
    std::cerr << "Could not open output file " << prefix << std::endl;
    throw(std::runtime_error("WRITE FAILED"));
  }
}

ofstream* OpenOutputFile(string outputFolder, string prefix,
                         int counter, string extension) {
  /* open files */
  stringstream ss(stringstream::in | stringstream::out);

  ss << outputFolder << "/compartments/" << prefix << "_" << counter
     << extension;
  string temp_name;
  ss >> temp_name;

  ofstream *out_stream = new ofstream(temp_name.c_str(), std::ios::binary);

  if (out_stream->fail()) {
    std::cerr << "Could not open output file " << temp_name << std::endl;
    throw(std::runtime_error("OPEN FAILED"));
  }
  return out_stream;
}

modigliani::Membrane_compartment_sequence *
CreateAxon(boost::property_tree::ptree config_root,
           ofstream                  & TypePerCompartmentFile,
           ofstream                  & LengthPerCompartmentFile,
           modigliani::Size       force_alg) {
  string lua_script = config_root.get<string>("anatomy_lua");
  lua_State *L      = luaL_newstate();

  luaL_openlibs(L);
  luaL_dostring(L, lua_script.c_str());

  auto compartment_types = std::vector<int>();
  lua_getglobal(L, "compartments");

  /* table is in the stack at index 't' */
  lua_pushnil(L); /* first key */

  while (lua_next(L, -2) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    int found = lua_tonumber(L, -1);
    compartment_types.push_back(found);
    lua_pop(L, 1);
  }
  lua_close(L);

  auto oModel = new Membrane_compartment_sequence();
  oModel->set_timestep(config_root.get<
                         double>("simulation_parameters.timeStep")

                       /* mSec */);
  std::vector<boost::property_tree::ptree> compartments_parameters(0);

  for (boost::property_tree::ptree::value_type const& v : config_root.
       get_child("compartments_parameters")) {
    compartments_parameters.push_back(v.second);
  }

  size_t num_compartments = compartments_parameters.size();

  for (std::vector<int>::iterator it = compartment_types.begin();
       it != compartment_types.end(); ++it) {
    TypePerCompartmentFile << *it << std::endl;
    LengthPerCompartmentFile
      << compartments_parameters[*it].get<std::string>
    ("length") << std::endl;
    oModel->
    PushBack(CreateCompartment
               (num_compartments == 1, config_root,
               config_root.get_child("simulation_parameters"),
               compartments_parameters[*it], force_alg));
  }
  return oModel;
}

boost::property_tree::ptree ReadConfig(string fileName) {
  boost::property_tree::ptree config_root;

  try {
    read_json(fileName, config_root);
  }
  catch (const exception& e) {
    // report to the user the failure and their locations in the document.
    std::cerr << "Failed to parse configuration\n" << e.what();
    exit(1);
  }
  return config_root;
}

std::vector<modigliani::Size>
GetElectrods(boost::property_tree::ptree root_config) {
  auto   outvec     = std::vector<modigliani::Size>();
  string lua_script = root_config.get<string>("electrods_lua");
  lua_State *L      = luaL_newstate();

  luaL_openlibs(L);
  luaL_dostring(L, lua_script.c_str());

  lua_getglobal(L, "electrods");

  /* table is in the stack at index 't' */
  lua_pushnil(L); /* first key */

  while (lua_next(L, -2) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    int found = lua_tonumber(L, -1);
    outvec.push_back(found);
    lua_pop(L, 1);
  }
  lua_close(L);
  return outvec;
}

int SetLuaPath(lua_State *L, const string path) {
  lua_getglobal(L, "package");

  // get field "path" from table at top of stack (-1)
  lua_getfield(L, -1, "path");

  // grab path string from top of stack
  std::string cur_path = lua_tostring(L, -1);

  // do your path magic her
  cur_path.append(";");
  cur_path.append(path);
  lua_pop(L, 1);

  // get rid of the string on the stack we just pushed on line 5
  lua_pushstring(L, cur_path.c_str()); // push the new one
  // set the field "path" in table at -2 with value at top of stack
  lua_setfield(L, -2, "path");
  lua_pop(L, 1);

  // get rid of package table from top of stack
  return 0; // all done!
}
}  // namespace modigliani
