/**\file ntbp_auxfunc.h - NTBP auxiliary function headers  for heteregenous object creation 
 * by Ahmed Aldo Faisal &copy; created 22.4.2005  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998-2005 Ahmed Aldo Faisal    
 *
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

#ifndef _mcore_auxfunc_h_
#define _mcore_auxfunc_h_
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cmath>
#include <ctime>
#ifdef __MINGW32__
#include <jsoncpp/json.h>
#else
#include <json/json.h>
#endif
#include <boost/filesystem.hpp>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "Membrane_compartment_sequence.h"
#include "File_based_stochastic_multi_current.h"
#include "Lua_based_stochastic_multi_current.h"
#include "Lua_based_deterministic_multi_current.h"
#include "Custom_cylindrical_compartment.h"
#include "HH_sga_leak_current.h"

#include <tnt/tnt.h>

namespace mcore {

const int EXIT_V_TOO_HIGH = 1;
const int EXIT_GRAPHIC_ERROR = 2;
const int EXIT_IO_ERROR = 3;

mbase::Real /* mum^-2 */
NTBP_corrected_channel_density(mbase::Real chDensity /* mum^-2 */,
		mbase::Real compArea /* mum^-2 */);

mcore::Membrane_current *
NTBP_create_na_channel_ptr(mbase::Size_t sodiumModel, mbase::Size_t algorithm,
		mbase::Real chDensity /* mum^-2 */,
		mbase::Real chConductance /* pS */, mbase::Real q10,
		mbase::Real temperature /* C */, mbase::Real compArea /* mum^2 */);

string createOutputFolder(string outputFolder);

/**
 * Formats and prints current parameters into the ofstream given as input.
 * @param out
 */
void printConfig(ofstream& out, Json::Value node_parameters,
		Json::Value paranode_parameters, Json::Value internode_parameters,
		Json::Value simulation_parameters, Json::Value config_root);

mcore::Custom_cylindrical_compartment* createCompartment(
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

mcore::Membrane_compartment_sequence* create_axon(Json::Value config_root,
		ofstream& TypePerCompartmentFile, ofstream& LengthPerCompartmentFile);
}
#endif /* _ntbp_auxfunc_h_ */
