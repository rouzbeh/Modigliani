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
 * Creates a new folder in the output directory
 * and puts a timestamp in its name.
 * @param output Folder The folder in which to create the new folder.
 * @return Name of the newly created folder
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

  boost::filesystem::create_directories(temp_folder_name);

  return (folderName);
}

/**
 * Creates a compartment using the parameters supplied in the parameters structs supplied.
 * @return The constructed compartment.
 */
modigliani_core::Custom_cylindrical_compartment*
modigliani_core::create_compartment(Json::Value config_root,
                                    Json::Value simulation_parameters,
                                    Json::Value compartment_parameters) {
  Custom_cylindrical_compartment *tmpPtr = new Custom_cylindrical_compartment(
      compartment_parameters["length"].asDouble() /* muMeter */,
      config_root["diameter"].asDouble() /* muMeter */,
      compartment_parameters["Cm"].asDouble()/*muFarad/cm^2 */,
      compartment_parameters["Ra"].asDouble() /* ohm cm */,
      config_root["temperature"].asDouble());

  tmpPtr->update_timeStep(
      config_root["simulation_parameters"]["timeStep"].asDouble() /* mSec */);

  bool randomise_densities =
      simulation_parameters["randomise_densities"].asBool();

  // Read a list of currents for each compartments
  Json::Value currents = compartment_parameters["currents"];
  for (unsigned int index = 0; index < currents.size(); ++index) {
    Json::Value current = currents[index];

    if ("leak" == current["type"].asString()) {
      tmpPtr->AttachCurrent(
          new Leak_current(tmpPtr->area(), current["GLeak"].asDouble(),
                           config_root["eLeak"].asDouble()),
          NTBP_LEAK);
      continue;
    }

    if ("file" == current["type"].asString()) {
      modigliani_base::Real indDensity = corrected_channel_density(
          current["chDen"].asDouble(), tmpPtr->area());
      File_based_stochastic_multi_current * file_current =
          new File_based_stochastic_multi_current(
              tmpPtr->area(),
              (randomise_densities ? indDensity : current["chDen"].asDouble()) /* mum^-2 */,
              current["chCond"].asDouble() * 1e-9 /* pS */,
              current["chRevPot"].asDouble() /* mV */,
              simulation_parameters["timeStep"].asDouble(),
              config_root["temperature"].asDouble() /* C */,
              current["chModel"].asString());
      if (4 == current["chAlg"].asInt())
        file_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
      if (2 == current["chAlg"].asInt())
        file_current->SetSimulationMode(NTBP_SINGLECHANNEL);
      tmpPtr->AttachCurrent(file_current, NTBP_IONIC);
      continue;
    }

    if ("lua" == current["type"].asString()) {
      if (1 == current["chAlg"].asInt()) {
        Lua_based_deterministic_multi_current * lua_current =
            new Lua_based_deterministic_multi_current(
                tmpPtr->area(), current["chDen"].asDouble() /* mum^-2 */,
                current["chCond"].asDouble() * 1e-9 /* pS */,
                current["chRevPot"].asDouble() /* mV */,
                simulation_parameters["timeStep"].asDouble(),
                config_root["temperature"].asDouble() /* C */,
                current["chModel"].asString());
        lua_current->SetSimulationMode(NTBP_DETERMINISTIC);
        tmpPtr->AttachCurrent(lua_current, NTBP_IONIC);
        continue;
      } else if (4 == current["chAlg"].asInt()
          || 2 == current["chAlg"].asInt()) {
        modigliani_base::Real indDensity = corrected_channel_density(
            current["chDen"].asDouble(), tmpPtr->area());
        Lua_based_stochastic_multi_current * lua_current =
            new Lua_based_stochastic_multi_current(
                tmpPtr->area(),
                (randomise_densities ? indDensity : current["chDen"].asDouble()) /* mum^-2 */,
                current["chCond"].asDouble() * 1e-9 /* pS */,
                current["chRevPot"].asDouble() /* mV */,
                simulation_parameters["timeStep"].asDouble(),
                config_root["temperature"].asDouble() /* C */,
                current["chModel"].asString());
        if (4 == current["chAlg"].asInt())
          lua_current->SetSimulationMode(NTBP_BINOMIALPOPULATION);
        if (2 == current["chAlg"].asInt())
          lua_current->SetSimulationMode(NTBP_SINGLECHANNEL);
        tmpPtr->AttachCurrent(lua_current, NTBP_IONIC);
        continue;
      }
    }
  }
  return (tmpPtr);
}

/**
 * Opens a new file in write mode.
 * @param output Folder The folder in which to create the new file.
 * @param prefix File name prefix
 * @param outStream Will contain an ofstream pointing to the newly
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
 * @short Opens a new file in write mode, postfixing the name with
 * the given number
 * @param output Folder The folder in which to create the new file.
 * @param prefix File name prefix
 * @param counter Number postfix
 * @param extenstion File extension
 * @return outStream Will contain an ofstream pointing to
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
    Json::Value config_root, ofstream& TypePerCompartmentFile,
    ofstream& LengthPerCompartmentFile) {

  string lua_script = config_root["anatomy_lua"].asString();
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
      config_root["simulation_parameters"]["timeStep"].asDouble() /* mSec */);
  oModel->StepNTBP();

  Json::Value compartments_parameters = config_root["compartments_parameters"];

  for (std::vector<int>::iterator it = compartment_types.begin();
      it != compartment_types.end(); ++it) {
    TypePerCompartmentFile << *it << std::endl;
    LengthPerCompartmentFile
        << compartments_parameters[*it]["length"].asDouble() << std::endl;
    oModel->PushBack(
        create_compartment(config_root, config_root["simulation_parameters"],
                           compartments_parameters[*it]));
  }
  return (oModel);
}

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 * @return A JSON structure containing the parameters
 */
Json::Value modigliani_core::read_config(string fileName) {
  Json::Value config_root;
  // Remember that data file should have more lines than Num iterations.
  Json::Reader config_reader;
  ifstream config_doc;
  config_doc.open(fileName.c_str(), ifstream::in);
  bool parsingSuccessful = config_reader.parse(config_doc, config_root);
  if (!parsingSuccessful) {
    // report to the user the failure and their locations in the document.
    std::cerr << "Failed to parse configuration\n"
              << config_reader.getFormatedErrorMessages();
    exit(1);
  }
  return (config_root);
}

std::vector<modigliani_base::Size> modigliani_core::get_electrods(
    Json::Value root) {
  auto outvec = std::vector<modigliani_base::Size>(100);
  string lua_script = root["electrods_lua"].asString();
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
