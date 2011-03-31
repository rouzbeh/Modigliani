///**\file hranvier_sodium_channel_model.cpp - testing and analysis of hranvier(1991) Na channel model
// * by Ahmed Aldo Faisal &copy; created 25.9.2001
// */
///* NetTrader - visualisation, scientific and financial analysis and simulation system
// * Version:  0.5
// * Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
// *
// * This library is free software; you can redistribute it and/or
// * modify it under the terms of the GNU Library General Public
// * License as published by the Free Software Foundation; either
// * version 2 of the License, or (at your option) any later version.
// *
// * This library is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// * Library General Public License for more details.
// *
// * You should have received a copy of the GNU Library General Public
// * License along with this library; if not, write to the Free
// * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// */
//
///* $Id: performance_current_classes_comparison.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
//* $Log: performance_current_classes_comparison.cpp,v $
//* Revision 1.2  2003/06/20 13:25:55  face
//* *** empty log message ***
//*
//* Revision 1.1  2003/01/30 17:13:39  face
//* *** empty log message ***
//*
//*/
//
//#ifdef HAVE_CONFIG_H
//#include <config.h>
//#endif
//
//#include <cmath>
//#include <string>
//#include <fstream>
//#include <iostream>
//#include <stdlib.h>
//#include <algorithm>
//
//#include <sys/times.h>
//
//#include "ntbp_object_obj.h"
//#include "ntbp_membrane_compartment_sequence_obj.h"
//#include "ntbp_sga_cylindrical_compartment_obj.h"
//#include "ntbp_custom_cylindrical_compartment_obj.h"
//#include "ntbp_hranvier_sodium_multi_current_obj.h"
//#include "ntbp_patlak_sodium_multi_current_obj.h"
//#include "ntbp_colbert_axonal_sodium_multi_current_obj.h"
//#include "ntbp_multi_sodium_current_obj.h"
//#include "ntbp_sodium_ion_channels_obj.h"
//
//#include "nt_error_obj.h"
//#include "nt_vector_obj.h"
//
//#include "nt_histogram_obj.h"
//#include "nt_sequential_statistics_obj.h"
//#include "tnt/tnt.h"
//#include "tnt/cmat.h"
//#include "nt_uniform_rnd_dist_obj.h"
//#include "nt_binomial_rnd_dist_obj.h"
//#include "nt_gaussian_rnd_dist_obj.h"
//#include "nt3d_plot2d_vec_vp_obj.h"
//#include "nt3d_glx_drv_obj.h"
//
//using namespace std;
//using namespace TNT;
//
int
main (int argc, char *argv[])
{
//    /* Read and set parameters */
//    NTsize numIterations = 100000;
//    NTreal timeStep = 0.001;
//
//    if (argc != 5) cerr << "Useage: "<<argv[0]<<" <temperature[C]> <simulationMode> <numChannels> <typeChannel>"<< endl;
//    NTreal temperature = atof (argv[1]);
//    NTsize simulationMode = atoi(argv[2]);
//    NTsize numChannels = atoi(argv[3]);
//    NTsize typeChannel = atoi(argv[4]);
//
//    enum NTBPstochasticType simAlg;
//    switch (simulationMode) {
//    case 0:
//        simAlg = NTBP_DETERMINISTIC;
//        break;
//    case 1:
//        simAlg = NTBP_SINGLECHANNEL;
//        break;
//    case 2:
//        simAlg = NTBP_BINOMIALPOPULATION;
//        break;
//    default:
//        cerr << "Unknown simulation mode specified \n 1=NTBP_DETERMINISTIC \n 2=NTBP_SINGLECHANNELN \n 3=TBP_BINOMIALPOPULATION."<<endl;
//        exit(NT_PARAM_UNSUPPORTED);
//    }
//
//    struct tms bufStart;
//    times(&bufStart);
//
//
////		for (NTreal vTarget=-20;vTarget<=80;vTarget+=10.0){
//    NTreal vTarget = 0;
//    /* *********************************************************  Main loop ************************************************ */
//    cerr << "MainLoop started" << endl;
//
//    NTreal timeInMS = 0.0;
//
//    NTBP_multi_current_o * currentPtr;
//    switch (typeChannel) {
//    case 1:
//        currentPtr = new NTBP_multi_sodium_current_o(1,numChannels);
//        break;
//    case 2:
//        currentPtr = new NTBP_patlak_sodium_multi_current_o(1,numChannels);
//        break;
//    case 3:
//        currentPtr = new NTBP_hranvier_sodium_multi_current_o(1,numChannels);
//        break;
//    case 4:
//        currentPtr = new NTBP_colbert_axonal_sodium_multi_current_o(1,numChannels);
//        break;
//    default:
//        cerr << "Unkown channel type specified.\n 1=SGA\n 2=Patlak\n 3=hRanvier\n 4=ColbertAxonal"<<endl;
//        exit(NT_PARAM_UNSUPPORTED);
//    }
//    currentPtr->SetSimulationMode(simAlg);
//    currentPtr->Set_temperature(temperature /* in celsius */ );
//    currentPtr->UpdateTimeStep(timeStep /* mSec */ ) ;
//
//    for (NTsize lt = 0; lt < numIterations; lt++)
//    {
//        timeInMS += currentPtr->_timeStep();
//        currentPtr->Step(vTarget);
//    }
//// }
//
//
//
//    struct tms bufStop;
//    times(&bufStop);
//
//    cout << temperature << "\t"<< simulationMode << "\t"<< numChannels << "\t"<< typeChannel << "\t"
//         << bufStop.tms_utime-bufStart.tms_utime   << "\t"<< bufStop.tms_stime-bufStart.tms_stime   << "\t" << numIterations << endl;
//    cerr << "Simulation completed." << endl;
    return 0;
}
