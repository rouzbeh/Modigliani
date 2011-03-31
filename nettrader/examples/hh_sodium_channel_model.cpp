///**\file hh_sodium_channel_model.cpp - testing and analysis of HdgkinHuxley(1952) Na channel model
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
//
///* $Id: hh_sodium_channel_model.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
//* $Log: hh_sodium_channel_model.cpp,v $
//* Revision 1.2  2003/06/20 13:25:55  face
//* *** empty log message ***
//*
//* Revision 1.1  2003/01/17 16:56:12  face
//* *** empty log message ***
//*
//* Revision 1.3  2002/03/08 15:13:24  face
//* *** empty log message ***
//*
//* Revision 1.2  2001/10/12 09:18:34  face
//* *** empty log message ***
//*
//* Revision 1.1  2001/10/03 14:36:53  face
//* *** empty log message ***
//*
//* Revision 1.1  2001/06/29 13:16:51  face
//* *** empty log message ***
//*
//* Revision 1.1  2001/06/11 13:18:10  face
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
//#include "ntbp_membrane_compartment_sequence_obj.h"
//#include "ntbp_sga_cylindrical_compartment_obj.h"
//#include "ntbp_custom_cylindrical_compartment_obj.h"
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
//
//
//
//using namespace std;
//using namespace TNT;
//
//
int
main (int argc, char *argv[])
{
//    /* Read and set parameters */
//    NTsize numIterations = 10000;
//    NTsize numTrials = 20;
//
//
//    NTreal temperature = atof (argv[1]);
//
//
//    for (NTreal vTarget=-15;vTarget<=115;vTarget+=10.0) {
//        for (NTsize lt=0; lt<numTrials;lt++) {
//            /* *********************************************************  Main loop ************************************************ */
//            cerr << "MainLoop started" << endl;
//
//            NTreal timeInMS = 0.0;
//            NTBP_multi_sodium_current_o current(100,10);
//            current.SetSimulationMode (NTBP_BINOMIALPOPULATION);
//            current.Set_temperature (temperature /* in celsius */ );
//            current.Set_q10(3);
//
//            for (NTsize lt = 0; lt < numIterations/2; lt++)
//            {
//                timeInMS += current._timeStep();
//                current.Step(0);
//            }
//
//
//
//            for (NTsize lt = 0; lt < numIterations; lt++)
//            {
//                timeInMS += current._timeStep();
//                current.Step(vTarget);
//                cout<< vTarget <<"\t"<< lt << "\t"<<timeInMS <<"\t"<< current.OpenChannels();
//                cout<< endl;
//                //		 current.Show();
//            }
//        }
//    }
//    cerr << "Simulation completed." << endl;
//
    return 0;
}
