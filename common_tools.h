/*
 * commons_tools.h
 *
 *  Created on: 8 Sep 2011
 *      Author: man210
 */

#ifndef COMMONS_TOOLS_H_
#define COMMONS_TOOLS_H_

#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <cmath>
#include <json/json.h>
#include <boost/filesystem.hpp>

#include <ntbp_membrane_compartment_sequence_obj.h>
#include <ntbp_file_based_stochastic_multi_current_obj.h>
#include <ntbp_lua_based_stochastic_multi_current_obj.h>
#include <ntbp_lua_based_deterministic_multi_current_obj.h>
#include <ntbp_custom_cylindrical_compartment_obj.h>
#include <ntbp_hh_sga_leak_current_obj.h>
#include <ntbp_auxfunc.h>

#include <nt_error_obj.h>
#include <nt_vector_obj.h>

#include <tnt.h>
#include <nt3d_plot2d_vec_vp_obj.h>
#include <nt3d_glx_drv_obj.h>
#include <nt_config_file_parser_obj.h>

using namespace std;
using namespace TNT;

const int EXIT_V_TOO_HIGH = 1;
const int EXIT_GRAPHIC_ERROR = 2;
const int EXIT_IO_ERROR = 3;

/* Global */
extern Json::Value hillock_parameters;
extern Json::Value node_parameters;
extern Json::Value paranode_parameters;
extern Json::Value internode_parameters;
extern Json::Value simulation_parameters;
extern Json::Value config_root; // will contains the root value after parsing.

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
void printConfig(ofstream& out, Json::Value hillock_parameters,
		Json::Value node_parameters, Json::Value paranode_parameters,
		Json::Value internode_parameters, Json::Value simulation_parameters,
		Json::Value config_root);

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
		ofstream& TypePerCompartmentFile, ofstream& LengthPerCompartmentFile,
		vector<NTsize>& nodes_vec, vector<NTsize>& nodes_paranodes_vec);

#endif /* COMMONS_TOOLS_H_ */
