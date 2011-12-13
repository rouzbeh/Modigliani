/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  2.0
 * Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
 * Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
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

#include "common_tools.h"
#include "bz2stream.hpp"

/* Global */
Json::Value hillock_parameters;
Json::Value node_parameters;
Json::Value paranode_parameters;
Json::Value internode_parameters;
Json::Value simulation_parameters;
Json::Value config_root; // will contains the root value after parsing.

/**
 * Reads the parameters in the file given as argument.
 * @param fileName Input file.
 */
void read_config(string fileName) {
	// Remember that data file should have more lines than Num iterations.
	//NT_config_file_parser_o oCfg(fileName);

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

	/*Hillokc*/
	if (config_root.get("hillock", false).asBool())
		hillock_parameters = config_root["hillock_parameters"];

	/* Nodes */
	if (config_root.get("node", false).asBool()) {
		node_parameters = config_root["node_parameters"];
	}

	/* Paranodes */
	if (config_root.get("paranode", false).asBool())
		paranode_parameters = config_root["paranode_parameters"];

	/* Internodes */
	if (config_root.get("internode", false).asBool())
		internode_parameters = config_root["internode_parameters"];

	simulation_parameters = config_root["simulation_parameters"];

}

/**
 * Let's go!
 *
 * @param argc
 * @param argv
 * @return
 */
int simulate(string fileName) {
	read_config(fileName);
	string timedOutputFolder;

	ofstream TimeFile, PotentialPerCompartmentFile, LengthPerCompartmentFile,
			PotassiumFile, SodiumFile, PersistentSodiumFile,
			TypePerCompartmentFile, ConfigUsedFile, LeakFile;
	/*bz2ostream *bz2PotentialPerCompartmentFile = 0, *bz2SodiumFile = 0,
	 *bz2PotassiumFile = 0, *bz2LeakFile = 0, *bz2PersistentFile = 0;*/

	if (simulation_parameters.get("sampN", 0).asUInt() > 0) {
		timedOutputFolder = createOutputFolder(
				simulation_parameters["outputFolder"].asString());
		openOutputFile(timedOutputFolder, "Time", TimeFile);
		openOutputFile(timedOutputFolder, "PotentialPerCompartment",
				PotentialPerCompartmentFile, ".bin");
		openOutputFile(timedOutputFolder, "Potassium", PotassiumFile, ".bin");
		openOutputFile(timedOutputFolder, "Sodium", SodiumFile, ".bin");
		openOutputFile(timedOutputFolder, "TypePerCompartment",
				TypePerCompartmentFile);
		openOutputFile(timedOutputFolder, "LengthPerCompartment",
				LengthPerCompartmentFile);
		openOutputFile(timedOutputFolder, "LeakPerCompartment", LeakFile,
				".bin");
		openOutputFile(timedOutputFolder, "PersistentSodium",
				PersistentSodiumFile, ".bin");
		openOutputFile(timedOutputFolder, "ConfigUsed", ConfigUsedFile, ".m");
		printConfig(ConfigUsedFile, hillock_parameters, node_parameters,
				paranode_parameters, internode_parameters,
				simulation_parameters, config_root);
		TimeFile << "% in ms" << endl;

		/*bz2PotentialPerCompartmentFile = new bz2ostream(
		 PotentialPerCompartmentFile.rdbuf());
		 bz2SodiumFile = new bz2ostream(SodiumFile.rdbuf());
		 bz2PotassiumFile = new bz2ostream(PotassiumFile.rdbuf());
		 bz2LeakFile = new bz2ostream(LeakFile.rdbuf());
		 bz2PersistentFile = new bz2ostream(PersistentSodiumFile.rdbuf());*/
	}

	cout << "Eleak set to " << config_root.get("eLeak", 0).asDouble() << " mV."
			<< endl;

	//NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur!
	//NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !

	// Read input file only once. Store its content in memory.
	ifstream dataFile(simulation_parameters["inputFile"].asString().c_str(),
			ios::binary);
	if (dataFile.fail()) {
		cerr << "Could not open input file "
				<< simulation_parameters["inputFile"].asString().c_str()
				<< endl;
		exit(EXIT_IO_ERROR);
	}
	int count = 1000000;
	vector<float> inputData(count);
	int index = 0;

	while (dataFile.good()) {
		if (index < count) {
			char tmp[100];
			dataFile.getline(tmp, 100);
			sscanf(tmp, "%f", &inputData[index]);
			index++;
		} else {
			count += 1000000;
			inputData.resize(count);
		}
	}
	dataFile.close();
	//NTsize compartmentCounter;
	cout << "Assembling neuron..." << endl;
	NTsize numCompartments;
	vector<NTsize> nodes_vec(0);
	vector<NTsize> nodes_paranodes_vec(0);
	/* *** Trials loop *** */
	for (NTsize lTrials = 0;
			lTrials < simulation_parameters["numTrials"].asUInt(); lTrials++) {
		/* Model setup */
		nodes_vec.clear();
		nodes_paranodes_vec.clear();
		NTBP_membrane_compartment_sequence_o oModel = create_axon(config_root,
				TypePerCompartmentFile, LengthPerCompartmentFile, nodes_vec,
				nodes_paranodes_vec);

		if (!lTrials) {
			TypePerCompartmentFile.close();
			LengthPerCompartmentFile.close();
		}
		oModel.Init();

		/* Information measurement init */
		numCompartments = hillock_parameters["numComp"].asUInt()
				+ (node_parameters["numNd"].asUInt() - 1)
						* (node_parameters["numComp"].asUInt()
								+ 2 * paranode_parameters["numComp"].asUInt()
								+ internode_parameters["numComp"].asUInt())
				+ node_parameters["numComp"].asUInt()
				+ paranode_parameters["numComp"].asUInt();
		cerr << "Total number of compartments(computed)" << numCompartments
				<< endl;
		//cerr << "Total number of compartments(assembled)" << compartmentCounter
		//	<< endl;
		cerr << "Total number of compartments(in oModel)"
				<< oModel._numCompartments() << endl;
		vector<NTreal> leakCurrVec(numCompartments);
		vector<NTreal> naCurrVec(numCompartments);
		vector<NTreal> kCurrVec(numCompartments);

		/* Graphics init */
		NT3D_plot2d_vec_vp_o plotXY(numCompartments);
		NT3D_plot2d_vec_vp_o plotChanNa(numCompartments);
		NT3D_plot2d_vec_vp_o plotChanK(numCompartments);

		if (simulation_parameters["useVis"].asInt() > 0) {
			NT3D_glx_drv_o* drvVP = new NT3D_glx_drv_o(1000, 120);
			drvVP->SetWindowTitle("Voltage-Compartment-Plot");
			if (NT_FAIL == plotXY.Connect(drvVP))
				return (EXIT_GRAPHIC_ERROR);
			plotXY.AutoRange(false);
			plotXY.SetXRange(0, numCompartments);
			plotXY.SetYRange(-100, 200);

			NT3D_glx_drv_o* drv2VP = new NT3D_glx_drv_o(1000, 100);
			drv2VP->SetWindowTitle("NaOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanNa.Connect(drv2VP))
				return (EXIT_GRAPHIC_ERROR);
			plotChanNa.SetXRange(0, numCompartments);
			plotChanNa.SetYRange(0, 100);

			NT3D_glx_drv_o* drv3VP = new NT3D_glx_drv_o(1000, 100);
			drv3VP->SetWindowTitle("KOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanK.Connect(drv3VP))
				return (EXIT_GRAPHIC_ERROR);
			plotChanK.SetXRange(0, numCompartments);
			plotChanK.SetYRange(0, 100);
		}

		/* *** SIMULATION ITERATION LOOP *** */
		/* ***********************  Main loop **************************** */
		cerr << "MainLoop started" << endl;
		float timeVar = 0;
		NTreal inpCurrent = 0.0;

		NT_uniform_rnd_dist_o uniformRnd;

		vector<NTreal> voltVec;
		//vector<NTreal>::iterator maxVoltPos;
		//vector<NTreal>::iterator maxVoltOldPos;
		NTreal timeInMS = 0;
		int dataRead = 0;
		for (NTsize lt = 0; lt < simulation_parameters["numIter"].asUInt();
				lt++) {
			timeInMS += oModel._timeStep();
			timeVar = timeInMS;
			/* the "sampling ratio" used for "measurement" to disk */
			if (simulation_parameters["sampN"].asInt() > 0
					&& lt % simulation_parameters["sampN"].asInt() == 0) {
				oModel.WriteMembranePotential(PotentialPerCompartmentFile,
						nodes_vec);
				oModel.WriteCurrent(LeakFile, 1); //Leak
				oModel.WriteCurrent(SodiumFile, 2, nodes_vec); //Na
				oModel.WriteCurrent(PotassiumFile, 3, nodes_paranodes_vec); // K
				oModel.WriteCurrent(PersistentSodiumFile, 4, nodes_vec); // Persistent Na
				TimeFile << timeVar << endl;
			}
			if (simulation_parameters["useVis"].asInt() > 0) {
				if (lt % simulation_parameters["useVis"].asInt() == 0) {
					voltVec.clear();
					for (NTsize ll = 0; ll < oModel._numCompartments(); ll++) {
						voltVec.push_back(oModel.compartmentVec[ll]->_vM());
					}
					for (NTsize lc = 0; lc < numCompartments; lc++) {
						if (NTisnan(voltVec[lc])) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is NaN." << endl;
							return (EXIT_V_TOO_HIGH);
						} else if (voltVec[lc] > 200.0 /* mV */) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is " << voltVec[lc] << "." << endl;
							return (EXIT_V_TOO_HIGH);
						}
					}

					plotXY.SetData(voltVec);
					plotXY.Draw();
					plotChanNa.SetData(oModel.OpenChannelsRatio(2));
					//plotChanNa.SetData(oModel.NumChannelsInState(2,5));
					plotChanNa.Draw();
					plotChanK.SetData(oModel.OpenChannelsRatio(3));
					//plotChanK.SetData(oModel.NumChannelsInState(3,5));
					plotChanK.Draw();
				}
			}
			if (lt % simulation_parameters["readN"].asInt() == 0) {
				inpCurrent = (inputData[dataRead]
						* simulation_parameters["inpISDV"].asDouble())
						+ simulation_parameters["inpI"].asDouble();
				dataRead++;
				cout << lt << "\t" << inpCurrent << endl;
			}
			oModel.InjectCurrent(inpCurrent, 1);

			oModel.Step();
		}
	} // lTrials
	cerr << "Simulation completed." << endl;
	cerr << "Number of compartments : " << numCompartments << endl;
	ConfigUsedFile << "Number of compartments : " << numCompartments << endl;
	return 0;
}

int simulateMS(string fileName) {
	read_config(fileName);
	string timedOutputFolder;

	ofstream TimeFile, PotentialPerCompartmentFile, LengthPerCompartmentFile,
			PotassiumFile, SodiumFile, PersistentSodiumFile,
			TypePerCompartmentFile, ConfigUsedFile, LeakFile;
//	bz2ostream *bz2PotentialPerCompartmentFile = 0, *bz2SodiumFile = 0,
//			*bz2PotassiumFile = 0, *bz2LeakFile = 0, *bz2PersistentFile = 0;

	if (simulation_parameters.get("sampN", 0).asUInt() > 0) {
		timedOutputFolder = createOutputFolder(
				simulation_parameters["outputFolder"].asString());
		openOutputFile(timedOutputFolder, "Time", TimeFile);
		openOutputFile(timedOutputFolder, "PotentialPerCompartment",
				PotentialPerCompartmentFile, ".bin");
		openOutputFile(timedOutputFolder, "Potassium", PotassiumFile, ".bz2");
		openOutputFile(timedOutputFolder, "Sodium", SodiumFile, ".bz2");
		openOutputFile(timedOutputFolder, "TypePerCompartment",
				TypePerCompartmentFile);
		openOutputFile(timedOutputFolder, "LengthPerCompartment",
				LengthPerCompartmentFile);
		openOutputFile(timedOutputFolder, "LeakPerCompartment", LeakFile,
				".bz2");
		openOutputFile(timedOutputFolder, "PersistentSodium",
				PersistentSodiumFile, ".bz2");
		openOutputFile(timedOutputFolder, "ConfigUsed", ConfigUsedFile, ".m");
		printConfig(ConfigUsedFile, hillock_parameters, node_parameters,
				paranode_parameters, internode_parameters,
				simulation_parameters, config_root);
		TimeFile << "% in ms" << endl;

//		bz2PotentialPerCompartmentFile = new bz2ostream(
//				PotentialPerCompartmentFile.rdbuf());
//		bz2SodiumFile = new bz2ostream(SodiumFile.rdbuf());
//		bz2PotassiumFile = new bz2ostream(PotassiumFile.rdbuf());
//		bz2LeakFile = new bz2ostream(LeakFile.rdbuf());
//		bz2PersistentFile = new bz2ostream(PersistentSodiumFile.rdbuf());
	}

	cout << "Eleak set to " << config_root.get("eLeak", 0).asDouble() << " mV."
			<< endl;

	//NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur!
	//NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !

	// Read input file only once. Store its content in memory.
	ifstream dataFile(simulation_parameters["inputFile"].asString().c_str(),
			ios::binary);
	if (dataFile.fail()) {
		cerr << "Could not open input file "
				<< simulation_parameters["inputFile"].asString().c_str()
				<< endl;
		exit(EXIT_IO_ERROR);
	}
	int count = 1000000;
	vector<float> inputData(count);
	int index = 0;

	while (dataFile.good()) {
		if (index < count) {
			char tmp[100];
			dataFile.getline(tmp, 100);
			sscanf(tmp, "%f", &inputData[index]);
			index++;
		} else {
			count += 1000000;
			inputData.resize(count);
		}
	}

	dataFile.close();
	//NTsize compartmentCounter;
	cout << "Assembling neuron..." << endl;
	/* *** Trials loop *** */
	for (NTsize lTrials = 0;
			lTrials < simulation_parameters["numTrials"].asUInt(); lTrials++) {

		vector<NTsize> nodes_vec;
		if (lTrials > 9 && lTrials < 20) {
			config_root["internode_parameters"]["GLeak"] =
					config_root["node_parameters"]["GLeak"];
			config_root["internode_parameters"]["Cm"] =
					config_root["node_parameters"]["Cm"];
			config_root["internode_parameters"]["chKDen"] = 0.36;
		}
		if (lTrials > 19 && lTrials < 30) {
			config_root["internode_parameters"]["GLeak"] =
					config_root["node_parameters"]["GLeak"];
			config_root["internode_parameters"]["Cm"] =
					config_root["node_parameters"]["Cm"];
			config_root["internode_parameters"]["chKDen"] = 0.36;
		}
		/* Model setup */
		/*Hillokc*/
		if (config_root.get("hillock", false).asBool())
			hillock_parameters = config_root["hillock_parameters"];

		/* Nodes */
		if (config_root.get("node", false).asBool()) {
			node_parameters = config_root["node_parameters"];
		}

		/* Paranodes */
		if (config_root.get("paranode", false).asBool())
			paranode_parameters = config_root["paranode_parameters"];

		/* Internodes */
		if (config_root.get("internode", false).asBool())
			internode_parameters = config_root["internode_parameters"];

		simulation_parameters = config_root["simulation_parameters"];
		NTBP_membrane_compartment_sequence_o oModel;
		oModel.UpdateTimeStep(
				simulation_parameters["timeStep"].asDouble() /* mSec */);
		oModel.StepNTBP();

		NTsize compartmentCounter = 1;
		/* *** MODEL CREATION LOOP *** */

		// Generate an axon hillock
		if (config_root.get("hillock", false).asBool()) {
			for (NTsize lcomp = 0;
					lcomp < hillock_parameters["numComp"].asUInt(); lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 0" << endl;
				LengthPerCompartmentFile
						<< hillock_parameters["length"].asDouble() << endl;
				oModel.PushBack(
						createCompartment(config_root, simulation_parameters,
								hillock_parameters));
			}
		}

		/* Create a Node, followed by Paranode, Internode, Paranode */
		for (NTsize lnd = 0; lnd < node_parameters["numNd"].asUInt(); lnd++) {
			/* Create a Node compartment */

			Json::Value ms_node_parameters = node_parameters;
			if (lTrials > 1 && lTrials < 3) {
				ms_node_parameters["chNaDen"] = 10;
			}
			for (NTsize lcomp = 0; lcomp < node_parameters["numComp"].asUInt();
					lcomp++) {
				TypePerCompartmentFile << compartmentCounter++ << " 1" << endl;
				LengthPerCompartmentFile << node_parameters["length"].asDouble()
						<< endl;
				nodes_vec.push_back(compartmentCounter - 2);
				NTBP_custom_cylindrical_compartment_o* compartment;
				if (lnd == 6) {
					compartment = createCompartment(config_root,
							simulation_parameters, ms_node_parameters);
				} else {
					compartment = createCompartment(config_root,
							simulation_parameters, node_parameters);
				}
				oModel.PushBack(compartment);
			}

			Json::Value ms_paranode_parameters = paranode_parameters;
			if (lTrials > 0 && lTrials < 2) {
				ms_paranode_parameters["GLeak"] =
						config_root["node_parameters"]["GLeak"].asDouble() / 4;
				ms_paranode_parameters["Cm"] =
						config_root["node_parameters"]["Cm"].asDouble() / 4;
				ms_paranode_parameters["chKDen"] = 0;
			}
			if (lTrials > 1 && lTrials < 3) {
				ms_paranode_parameters["GLeak"] =
						config_root["node_parameters"]["GLeak"].asDouble() / 4;
				ms_paranode_parameters["Cm"] =
						config_root["node_parameters"]["Cm"].asDouble() / 4;
				ms_paranode_parameters["chKDen"] = 0;
				ms_paranode_parameters["chNaDen"] = 10;
			}
			/* Create a Paranode compartment */
			if (config_root.get("paranode", false).asBool()) {
				for (NTsize lcomp = 0;
						lcomp < paranode_parameters["numComp"].asUInt();
						lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 2"
							<< endl;
					LengthPerCompartmentFile
							<< paranode_parameters["length"].asDouble() << endl;
					if (lnd == 5) {
						oModel.PushBack(
								createCompartment(config_root,
										simulation_parameters,
										ms_paranode_parameters));
					} else
						oModel.PushBack(
								createCompartment(config_root,
										simulation_parameters,
										paranode_parameters));
				}
			}

			if ((node_parameters["numNd"].asUInt() - 1) == lnd) {
				break;
			}

			Json::Value ms_internode_parameters = internode_parameters;
			if (lTrials > 0 && lTrials < 2) {
				ms_internode_parameters["GLeak"] =
						config_root["node_parameters"]["GLeak"].asDouble() / 4;
				ms_internode_parameters["Cm"] =
						config_root["node_parameters"]["Cm"].asDouble() / 4;
				ms_internode_parameters["chKDen"] = 0;
			}
			if (lTrials > 1 && lTrials < 3) {
				ms_internode_parameters["GLeak"] =
						config_root["node_parameters"]["GLeak"].asDouble() / 4;
				ms_internode_parameters["Cm"] =
						config_root["node_parameters"]["Cm"].asDouble() / 4;
				ms_internode_parameters["chKDen"] = 0;
				ms_internode_parameters["chNaDen"] = 10;
			}
			/* Create an Internode compartment */
			if (config_root.get("internode", false).asBool()) {
				for (NTsize lcomp = 0;
						lcomp < internode_parameters["numComp"].asUInt();
						lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 3"
							<< endl;
					LengthPerCompartmentFile
							<< internode_parameters["length"].asDouble()
							<< endl;
					if (lnd == 5)
						oModel.PushBack(
								createCompartment(config_root,
										simulation_parameters,
										ms_internode_parameters));
					else
						oModel.PushBack(
								createCompartment(config_root,
										simulation_parameters,
										internode_parameters));
				}
			}

			if (config_root.get("paranode", false).asBool()) {
				for (NTsize lcomp = 0;
						lcomp < paranode_parameters["numComp"].asUInt();
						lcomp++) {
					TypePerCompartmentFile << compartmentCounter++ << " 2"
							<< endl;
					LengthPerCompartmentFile
							<< paranode_parameters["length"].asDouble() << endl;
					if (lnd == 5) {
						oModel.PushBack(
								createCompartment(config_root,
										simulation_parameters,
										ms_paranode_parameters));
					} else
						oModel.PushBack(
								createCompartment(config_root,
										simulation_parameters,
										paranode_parameters));
				}
			}

		}

		if (!lTrials) {
			TypePerCompartmentFile.close();
			LengthPerCompartmentFile.close();
		}
		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = hillock_parameters["numComp"].asUInt()
				+ (node_parameters["numNd"].asUInt() - 1)
						* (node_parameters["numComp"].asUInt()
								+ 2 * paranode_parameters["numComp"].asUInt()
								+ internode_parameters["numComp"].asUInt())
				+ node_parameters["numComp"].asUInt()
				+ paranode_parameters["numComp"].asUInt();
		cerr << "Total number of compartments(computed)" << numCompartments
				<< endl;
		//cerr << "Total number of compartments(assembled)" << compartmentCounter
		//	<< endl;
		cerr << "Total number of compartments(in oModel)"
				<< oModel._numCompartments() << endl;
		vector<NTreal> leakCurrVec(numCompartments);
		vector<NTreal> naCurrVec(numCompartments);
		vector<NTreal> kCurrVec(numCompartments);

		/* Graphics init */
		NT3D_plot2d_vec_vp_o plotXY(numCompartments);
		NT3D_plot2d_vec_vp_o plotChanNa(numCompartments);
		NT3D_plot2d_vec_vp_o plotChanK(numCompartments);

		if (simulation_parameters["useVis"].asInt() > 0) {
			NT3D_glx_drv_o* drvVP = new NT3D_glx_drv_o(1000, 120);
			drvVP->SetWindowTitle("Voltage-Compartment-Plot");
			if (NT_FAIL == plotXY.Connect(drvVP))
				return (EXIT_GRAPHIC_ERROR);
			plotXY.AutoRange(false);
			plotXY.SetXRange(0, numCompartments);
			plotXY.SetYRange(-100, 200);

			NT3D_glx_drv_o* drv2VP = new NT3D_glx_drv_o(1000, 100);
			drv2VP->SetWindowTitle("NaOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanNa.Connect(drv2VP))
				return (EXIT_GRAPHIC_ERROR);
			plotChanNa.SetXRange(0, numCompartments);
			plotChanNa.SetYRange(0, 100);

			NT3D_glx_drv_o* drv3VP = new NT3D_glx_drv_o(1000, 100);
			drv3VP->SetWindowTitle("KOpenChannelRatio-Compartment-Plot");
			if (NT_FAIL == plotChanK.Connect(drv3VP))
				return (EXIT_GRAPHIC_ERROR);
			plotChanK.SetXRange(0, numCompartments);
			plotChanK.SetYRange(0, 100);
		}

		/* *** SIMULATION ITERATION LOOP *** */

		/* ***********************  Main loop **************************** */
		cerr << "MainLoop started" << endl;
		float timeVar = 0;
		NTreal inpCurrent = 0.0;

		NT_uniform_rnd_dist_o uniformRnd;

		vector<NTreal> voltVec;
		vector<NTreal>::iterator maxVoltPos;
		vector<NTreal>::iterator maxVoltOldPos;
		NTreal timeInMS = 0;
		int dataRead = 0;
		for (NTsize lt = 0; lt < simulation_parameters["numIter"].asUInt();
				lt++) {
			timeInMS += oModel._timeStep();
			timeVar = timeInMS;
			/* the "sampling ratio" used for "measurement" to disk */
			if (simulation_parameters["sampN"].asInt() > 0
					&& lt % simulation_parameters["sampN"].asInt() == 0) {
				oModel.WriteMembranePotentialASCII(PotentialPerCompartmentFile);
				oModel.WriteCurrentAscii(LeakFile, 1); //Leak
				oModel.WriteCurrentAscii(SodiumFile, 2); //Na
				oModel.WriteCurrentAscii(PotassiumFile, 3); // K
				oModel.WriteCurrentAscii(PersistentSodiumFile, 4); // Persistent Na
				TimeFile << timeVar << endl;
			}

			if (simulation_parameters["useVis"].asInt() > 0) {
				if (lt % simulation_parameters["useVis"].asInt() == 0) {
					voltVec.clear();
					for (NTsize ll = 0; ll < oModel._numCompartments(); ll++) {
						voltVec.push_back(oModel.compartmentVec[ll]->_vM());
					}
					for (NTsize lc = 0; lc < numCompartments; lc++) {
						if (NTisnan(voltVec[lc])) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is NaN." << endl;
							return (EXIT_V_TOO_HIGH);
						} else if (voltVec[lc] > 200.0 /* mV */) {
							cerr << "ERROR at t=" << timeVar
									<< " voltage in compartment " << lc
									<< " is " << voltVec[lc] << "." << endl;
							return (EXIT_V_TOO_HIGH);
						}
					}

					plotXY.SetData(voltVec);
					plotXY.Draw();
					plotChanNa.SetData(oModel.OpenChannelsRatio(2));
					//plotChanNa.SetData(oModel.NumChannelsInState(2,5));
					plotChanNa.Draw();
					plotChanK.SetData(oModel.OpenChannelsRatio(3));
					//plotChanK.SetData(oModel.NumChannelsInState(3,5));
					plotChanK.Draw();
				}
			}

			if (lt % simulation_parameters["readN"].asInt() == 0) {
				inpCurrent = (inputData[dataRead]
						* simulation_parameters["inpISDV"].asDouble())
						+ simulation_parameters["inpI"].asDouble();
				dataRead++;
				cout << lt << "\t" << inpCurrent << endl;
			}
			oModel.InjectCurrent(inpCurrent, 1);

			oModel.Step();
		}
	} // lTrials
	cerr << "Simulation completed." << endl;
//	bz2PotentialPerCompartmentFile->flush();
//	bz2SodiumFile->flush();
//	bz2PotassiumFile->flush();
//	bz2LeakFile->flush();
//	bz2PersistentFile->flush();
//	delete bz2PotentialPerCompartmentFile;
//	delete bz2SodiumFile;
//	delete bz2PotassiumFile;
//	delete bz2LeakFile;
//	delete bz2PersistentFile;
	return 0;
}

int get_resting_potential(string fileName) {
	read_config(fileName);
	double min = -90;
	double max = -40;
	double current_guess;
	double current_result = 100000;

	ofstream temp;
	temp.open("/dev/null");

	while (current_result > -79.9 || current_result < -80.1) {
		/* Model setup */
		current_guess = (min + max) / 2;
		cout << "Trying with " << current_guess << endl;
		config_root["eLeak"] = current_guess;
		config_root["node_parameters"]["numNd"] = 5;
		vector<NTsize> nodes_vec, nodes_paranodes_vec;
		/* Model setup */
		NTBP_membrane_compartment_sequence_o oModel = create_axon(config_root,
				temp, temp, nodes_vec, nodes_paranodes_vec);

		oModel.Init();

		/* Information measurement init */
		NTsize numCompartments = hillock_parameters["numComp"].asUInt()
				+ (node_parameters["numNd"].asUInt() - 1)
						* (node_parameters["numComp"].asUInt()
								+ 2 * paranode_parameters["numComp"].asUInt()
								+ internode_parameters["numComp"].asUInt())
				+ node_parameters["numComp"].asUInt()
				+ paranode_parameters["numComp"].asUInt();
		cerr << "Total number of compartments(computed)" << numCompartments
				<< endl;
		//cerr << "Total number of compartments(assembled)" << compartmentCounter
		//	<< endl;
		cerr << "Total number of compartments(in oModel)"
				<< oModel._numCompartments() << endl;

		/* ***********************  Main loop **************************** */
		cerr << "MainLoop started" << endl;

		double sum = 0;
		for (NTsize lt = 0; lt < simulation_parameters["numIter"].asUInt();
				lt++) {

			/* the "sampling ratio" used for "measurement" to disk */
			if (lt % 10000 == 0) {
				sum = 0;
				for (NTsize ll = 0; ll < oModel._numCompartments(); ll++) {
					sum += oModel.compartmentVec[ll]->_vM();
				}
				cout << "Mean voltage = " << sum / oModel._numCompartments()
						<< endl;
			}

			if (lt == 10000) {
				NTreal inpCurrent = (5
						* simulation_parameters["inpISDV"].asDouble())
						+ simulation_parameters["inpI"].asDouble();
				oModel.InjectCurrent(inpCurrent, 1);
			} else {
				oModel.InjectCurrent(0, 1);
			}

			oModel.Step();
		}
		current_result = sum / oModel._numCompartments();
		if (current_result > -80) {
			max = current_guess;
		}
		if (current_result < -80) {
			min = current_guess;
		}
	}
	cerr << "Simulation completed. Found " << current_guess << endl;
	return 0;
}

int main(int argc, char* argv[]) {
	if (strcmp(argv[1], "resting") == 0) {
		return get_resting_potential(argv[2]);
	}
	if (strcmp(argv[1], "simulate") == 0) {
		return simulate(argv[2]);
	}
	if (strcmp(argv[1], "MS") == 0) {
		return simulateMS(argv[2]);
	}
	return 1;
}
