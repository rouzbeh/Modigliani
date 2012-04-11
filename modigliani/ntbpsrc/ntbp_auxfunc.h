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

/* $Id: ntbp_auxfunc.h,v 1.1 2005/04/25 13:55:27 face Exp $
 * $Log: ntbp_auxfunc.h,v $
 * Revision 1.1  2005/04/25 13:55:27  face
 * *** empty log message ***
 *
 */

#ifndef _ntbp_auxfunc_h_ 
#define _ntbp_auxfunc_h_ 
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cmath>
#include <json/json.h>
#include <boost/filesystem.hpp>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "ntbp_membrane_compartment_sequence_obj.h"
#include "ntbp_file_based_stochastic_multi_current_obj.h"
#include "ntbp_lua_based_stochastic_multi_current_obj.h"
#include "ntbp_lua_based_deterministic_multi_current_obj.h"
#include "ntbp_custom_cylindrical_compartment_obj.h"
#include "ntbp_hh_sga_leak_current_obj.h"
#include "ntbp_auxfunc.h"

#include <nt_error_obj.h>
#include <nt_vector_obj.h>

#include <tnt.h>
#include <nt3d_plot2d_vec_vp_obj.h>

const int EXIT_V_TOO_HIGH = 1;
const int EXIT_GRAPHIC_ERROR = 2;
const int EXIT_IO_ERROR = 3;

NTreal /* mum^-2 */
NTBP_corrected_channel_density(NTreal chDensity /* mum^-2 */,
		NTreal compArea /* mum^-2 */);

NTBP_membrane_current_o *
NTBP_create_na_channel_ptr(NTsize sodiumModel, NTsize algorithm,
		NTreal chDensity /* mum^-2 */, NTreal chConductance /* pS */,
		NTreal q10, NTreal temperature /* C */, NTreal compArea /* mum^2 */);

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 */
void read_config(string fileName);

string createOutputFolder(string outputFolder);

/**
 * Formats and prints current parameters into the ofstream given as input.
 * @param out
 */
void printConfig(ofstream& out, Json::Value node_parameters,
		Json::Value paranode_parameters, Json::Value internode_parameters,
		Json::Value simulation_parameters, Json::Value config_root);

NTBP_custom_cylindrical_compartment_o* createCompartment(
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

NTBP_membrane_compartment_sequence_o create_axon(Json::Value config_root,
		ofstream& TypePerCompartmentFile, ofstream& LengthPerCompartmentFile);

#endif /* _ntbp_auxfunc_h_ */
