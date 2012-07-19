/**
 * @file aux_func.h
 * Auxiliary function headers  for heteregenous object creation
 * @author Ahmed Aldo Faisal &copy; created 22.4.2005  
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_AUX_FUNC_H_
#define MODIGLIANI_MODIGLIANI_CORE_AUX_FUNC_H_


#ifdef __MINGW32__
#include <jsoncpp/json.h>
#else
#include <json/json.h>
#endif

#include <boost/filesystem.hpp>
#include <tnt/tnt.h>

#include <string>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cmath>
#include <vector>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "modigliani_core/membrane_compartment_sequence.h"
#include "modigliani_core/file_based_stochastic_multi_current.h"
#include "modigliani_core/lua_based_stochastic_multi_current.h"
#include "modigliani_core/lua_based_deterministic_multi_current.h"
#include "modigliani_core/custom_cylindrical_compartment.h"
#include "modigliani_core/leak_current.h"

namespace modigliani_core {

const int EXIT_V_TOO_HIGH = 1;
const int EXIT_GRAPHIC_ERROR = 2;
const int EXIT_IO_ERROR = 3;

modigliani_base::Real /* mum^-2 */
corrected_channel_density(modigliani_base::Real chDensity /* mum^-2 */,
                          modigliani_base::Real compArea /* mum^-2 */);

string createOutputFolder(string outputFolder);

modigliani_core::Custom_cylindrical_compartment* create_compartment(
    Json::Value config_root, Json::Value simulation_parameters,
    Json::Value compartment_parameters);

/**
 * Opens a new file in write mode.
 * @param output Folder The folder in which to create the new file.
 * @param prefix File name prefix
 * @param outStream Will contain an ofstream pointing to the newly created file.
 */
void openOutputFile(string outputFolder, string prefix, ofstream& outStream,
                    string extension = ".txt");

ofstream* openOutputFile(string outputFolder, string prefix, int counter,
                         string extension = ".txt");

modigliani_core::Membrane_compartment_sequence* create_axon(
    Json::Value config_root, ofstream& TypePerCompartmentFile,
    ofstream& LengthPerCompartmentFile);

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 * @return A JSON structure containing the parameters
 */
Json::Value read_config(string fileName);

std::vector<modigliani_base::Size> get_electrods(Json::Value root);
}

#endif  // MODIGLIANI_MODIGLIANI_CORE_AUX_FUNC_H_