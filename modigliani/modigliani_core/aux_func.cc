/**
 * @file aux_func.cc
 * Auxiliary function implementation for heterogeneous object creation
 * @author Ahmed Ali Neishabouri &copy; created 01.4.2012
 * @version   0.5
 * Copyright (C) 1998-2005 Ahmed Aldo Faisal    
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

#include "modigliani_core/aux_func.h"

modigliani_base::Real modigliani_core::corrected_channel_density(
    modigliani_base::Real chDensity, modigliani_base::Real compArea) {
  modigliani_base::Real chPerCompartment = compArea * chDensity;
  modigliani_base::Real pChFloor = (ceil(chPerCompartment) - chPerCompartment);

  modigliani_base::Uniform_rnd_dist uniRnd;
  /* compute number of channels, such that average density is achieved */
  modigliani_base::Real indChDensity = (
      uniRnd.RndVal() > pChFloor ? ceil(chPerCompartment) / compArea :
          floor(chPerCompartment) / compArea);

  return (indChDensity);
}

/**
 * \brief Creates a new folder in the output directory
 * and puts a timestamp in its name.
 *
 * \param outputFolder The folder in which to create the new folder.
 * \return Name of the newly created folder
 */
string modigliani_core::createOutputFolder(string outputFolder) {
  /* open files */
  time_t rawtime;
  struct tm * timeinfo;
  stringstream ss(stringstream::in | stringstream::out);
  char dateString[80];
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  const char* format = "%b%d_%H%M%S";
  size_t max_size = 80;
  strftime(dateString, max_size, format, timeinfo);
  ss << outputFolder << dateString;
  string folderName;
  ss >> folderName;
  stringstream ss2(stringstream::in | stringstream::out);
  ss2 << folderName << "/compartments/";
  string temp_folder_name;
  ss2 >> temp_folder_name;

  boost::filesystem::path temp_folder_path(temp_folder_name);
  boost::filesystem::create_directories(
      temp_folder_path.make_preferred().native());

  return (folderName);
}

/**
 * Creates a cylindrical compartment using the parameters
 * supplied in the parameters structs supplied.
 * @return The constructed compartment.
 */
modigliani_core::Cylindrical_compartment* modigliani_core::create_compartment(
    boost::property_tree::ptree config_root,
    boost::property_tree::ptree simulation_parameters,
    boost::property_tree::ptree compartment_parameters,
    modigliani_base::Size force_alg) {

  modigliani_core::Cylindrical_compartment *tmpPtr =
      new modigliani_core::Cylindrical_compartment(
          compartment_parameters.get<double>("length") /* muMeter */,
          config_root.get<double>("diameter") /* muMeter */,
          compartment_parameters.get<double>("Cm")/*muFarad/cm^2 */,
          compartment_parameters.get<double>("Ra") /* ohm cm */,
          config_root.get<double>("temperature"));

  tmpPtr->update_timeStep(
      config_root.get<double>("simulation_parameters.timeStep") /* mSec */);

  bool randomise_densities = simulation_parameters.get<bool>(
      "randomise_densities");

  // Read a list of currents for each compartments
  boost::property_tree::ptree currents = compartment_parameters.get_child(
      "currents");

  attach_current(tmpPtr, currents, config_root, randomise_densities, force_alg);

  return (tmpPtr);
}

void modigliani_core::attach_current(
    modigliani_core::Membrane_compartment* compartment,
    const boost::property_tree::ptree currents,
    boost::property_tree::ptree config_root, bool randomise_densities,
    modigliani_base::Size force_alg) {
  //for (unsigned int index = 0; index < currents.size(); ++index) {

  for(boost::property_tree::ptree::value_type const &v : currents) {
    // v.first is the name of the child.
    // v.second is the child tree.
    boost::property_tree::ptree current = v.second;

    if (string("leak") == current.get<string>("type")) {
      compartment->AttachCurrent(
          new Leak_current(compartment->area(), current.get<double>("GLeak"),
                           config_root.get<double>("eLeak")),
          NTBP_LEAK);
      continue;
    }

    if (string("file") == current.get<string>("type")) {
      modigliani_base::Real indDensity = corrected_channel_density(
          current.get<double>("chDen"), compartment->area());
      File_based_stochastic_voltage_gated_channel * file_current =
          new File_based_stochastic_voltage_gated_channel(
              compartment->area(),
              (randomise_densities ? indDensity : current.get<double>("chDen")),
              /* mum^-2 */
              current.get<double>("chCond") * 1e-9 /* pS */,
              current.get<double>("chRevPot") /* mV */, compartment->timeStep(),
              config_root.get<double>("temperature") /* C */,
              current.get<string>("chModel"));
      auto alg = current.get<int>("chAlg");
      if (force_alg) alg = force_alg;
      if (4 == alg) file_current->set_simulation_mode(BINOMIALPOPULATION);
      if (2 == alg) file_current->set_simulation_mode(SINGLECHANNEL);
      compartment->AttachCurrent(file_current, NTBP_IONIC);
      continue;
    }

    if (string("lua") == current.get<string>("type")) {
      auto alg = (force_alg ? force_alg : current.get<int>("chAlg"));
      if (1 == alg) {
        Lua_based_deterministic_voltage_gated_channel * lua_current =
            new Lua_based_deterministic_voltage_gated_channel(
                compartment->area(), current.get<double>("chDen") /* mum^-2 */,
                current.get<double>("chCond") * 1e-9 /* pS */,
                current.get<double>("chRevPot") /* mV */,
                compartment->timeStep(),
                config_root.get<double>("temperature") /* C */,
                current.get<string>("chModel"));
        lua_current->set_simulation_mode(DETERMINISTIC);
        compartment->AttachCurrent(lua_current, NTBP_IONIC);
        continue;
      } else if (4 == alg || 2 == alg) {
        modigliani_base::Real indDensity = corrected_channel_density(
            current.get<double>("chDen"), compartment->area());
        Lua_based_stochastic_voltage_gated_channel * lua_current =
            new Lua_based_stochastic_voltage_gated_channel(
                compartment->area(),
                (randomise_densities ? indDensity : current.get<double>("chDen")),
                current.get<double>("chCond") * 1e-9 /* pS */,
                current.get<double>("chRevPot") /* mV */,
                compartment->timeStep(),
                config_root.get<double>("temperature") /* C */,
                current.get<string>("chModel"));
        if (4 == alg) lua_current->set_simulation_mode(BINOMIALPOPULATION);
        if (2 == alg) lua_current->set_simulation_mode(SINGLECHANNEL);
        compartment->AttachCurrent(lua_current, NTBP_IONIC);
        continue;
      }
    }
  }
}

/**
 * \brief Opens a new file in write mode.
 *
 * \param outputFolder The folder in which to create the new file.
 * \param prefix File name prefix
 * \param outStream Will contain an ofstream pointing to the newly
 * \param extension File extension
 * created file.
 */
void modigliani_core::openOutputFile(string outputFolder, string prefix,
                                     ofstream& outStream, string extension) {
  /* open files */
  stringstream ss(stringstream::in | stringstream::out);
  ss << outputFolder << "/" << prefix << extension;
  string temp_name;
  ss >> temp_name;

  outStream.open(temp_name.c_str(), ios::binary);

  if (outStream.fail()) {
    std::cerr << "Could not open output file " << prefix << std::endl;
    std::exit(modigliani_core::EXIT_IO_ERROR);
  }
}

/**
 * \brief Opens a new file in write mode, postfixing the name with
 * the given number
 *
 * \param outputFolder The folder in which to create the new file.
 * \param prefix File name prefix
 * \param counter Number postfix
 * \param extension File extension
 * \return outStream Will contain an ofstream pointing to
 * the newly created file.
 */
ofstream* modigliani_core::openOutputFile(string outputFolder, string prefix,
                                          int counter, string extension) {
  /* open files */
  stringstream ss(stringstream::in | stringstream::out);
  ss << outputFolder << "/compartments/" << prefix << "_" << counter
     << extension;
  string temp_name;
  ss >> temp_name;

  ofstream* out_stream = new ofstream(temp_name.c_str(), ios::binary);

  if (out_stream->fail()) {
    std::cerr << "Could not open output file " << temp_name << std::endl;
    std::exit(modigliani_core::EXIT_IO_ERROR);
  }
  return (out_stream);
}

modigliani_core::Membrane_compartment_sequence* modigliani_core::create_axon(
    boost::property_tree::ptree config_root, ofstream& TypePerCompartmentFile,
    ofstream& LengthPerCompartmentFile, modigliani_base::Size force_alg) {

  string lua_script = config_root.get<string>("anatomy_lua");
  lua_State* L = luaL_newstate();
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
  oModel->update_timeStep(
      config_root.get<double>("simulation_parameters.timeStep") /* mSec */);
  oModel->StepNTBP();

  std::vector<boost::property_tree::ptree> compartments_parameters(0);

  for(boost::property_tree::ptree::value_type const &v: config_root.get_child(
          "compartments_parameters")) {
    compartments_parameters.push_back(v.second);
  }

  for (std::vector<int>::iterator it = compartment_types.begin();
      it != compartment_types.end(); ++it) {
    TypePerCompartmentFile << *it << std::endl;
    LengthPerCompartmentFile
        << compartments_parameters[*it].get<std::string>("length") << std::endl;
    oModel->PushBack(
        create_compartment(config_root,
                           config_root.get_child("simulation_parameters"),
                           compartments_parameters[*it], force_alg));
  }
  return (oModel);
}

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 * @return A JSON structure containing the parameters
 */
boost::property_tree::ptree modigliani_core::read_config(string fileName) {
  boost::property_tree::ptree config_root;
  try {
    read_json(fileName, config_root);
  } catch (exception &e) {
    // report to the user the failure and their locations in the document.
    std::cerr << "Failed to parse configuration\n" << e.what();
    exit(1);
  }
  return (config_root);
}

std::vector<modigliani_base::Size> modigliani_core::get_electrods(
    boost::property_tree::ptree root) {
  auto outvec = std::vector<modigliani_base::Size>();
  string lua_script = root.get<string>("electrods_lua");
  lua_State* L = luaL_newstate();
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
  return (outvec);
}

int SetLuaPath(lua_State* L, const string path) {
  lua_getglobal( L, "package");
  lua_getfield(L, -1, "path");  // get field "path" from table at top of stack (-1)
  std::string cur_path = lua_tostring( L, -1 );  // grab path string from top of stack
  cur_path.append(";");  // do your path magic her
  cur_path.append(path);
  lua_pop( L, 1);
  // get rid of the string on the stack we just pushed on line 5
  lua_pushstring(L, cur_path.c_str());  // push the new one
  lua_setfield(L, -2, "path");  // set the field "path" in table at -2 with value at top of stack
  lua_pop( L, 1);
  // get rid of package table from top of stack
  return (0);  // all done!
}

