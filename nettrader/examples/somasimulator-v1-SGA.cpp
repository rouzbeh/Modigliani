///**\file somasimulator-v1-SGA.cpp - soma simulator V. 1 SGA
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
///* $Id: somasimulator-v1-SGA.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
//* $Log: somasimulator-v1-SGA.cpp,v $
//* Revision 1.2  2003/06/20 13:25:55  face
//* *** empty log message ***
//*
//* Revision 1.1  2003/04/08 15:13:22  face
//* *** empty log message ***
//*
//* Revision 1.1  2003/01/30 17:13:39  face
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
//#include "ntbp_spherical_compartment_obj.h"
//#include "ntbp_multi_sodium_current_obj.h"
//#include "ntbp_multi_potassium_current_obj.h"
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
//    float inpCurrent = 0;
//    string filename;
//    NTsize useVis;
//    NTsize sampN;
//    NTreal temperature;		//= 37; /* in Celsius */
//    NTreal q10;
//    NTreal diameter;		// = 1; /* in muMeter */
//    NTreal gLeak = 0.05;	/* mSiemens/cm^2 */
//    NTreal rA = 200;		/* Ohm cm */
//
//    NTreal timeStep;		// in msec
//    NTsize numIterations;
//    NTreal sodiumDensity;	//= 60; // per mu^2
//    NTreal potassiumDensity;	// = 18; //  per mu^2
//    string dataFilename;
//    NTsize readN;
//    NTreal inpI;
//    NTreal inpISDV;
//    NTreal numTrials = 0;
//    NTreal length = 0;
//    NTsize channelType = 0;
//
//    if (17 != argc)
//    {
//        cerr << "Invalid command line. Use > " << argv[0] <<"<1 storage FILENAME>" << endl;
//        cerr << "<2 visualisation interval> <3 sample to storage file every n steps> <4 read data file every n steps> " 	      << endl;
//        cerr << "<5 data filename> <6 inpI [nA] > <7 inpSDV [nA]> " << endl;
//        cerr << "<8 diameter [mum (micrometer)]> <9 gLeak [mS/cm^2] >"	      << endl;
//        cerr << "<10 temperature [Celsius] > <11 q10 for ion channels>" <<endl;
//        cerr << "<12 Na channel density [per mum^2]> <13 K channel density [per mum^2]> "<< endl;
//        cerr << "<14 numIterations> <15 timeStep [ms]>" << endl;
//        cerr << "<16 numTrials> " << endl;
//        cerr << endl;
//        cerr << "Set inpI and inpSDV to 0 for zero input." << endl;
//        cerr << "Set inpI to 0 and inpSDV to 1 for 1:1 readout of data file float values" << endl;
//        cerr << "If data file contains normal distributed floating point random numbers..." << endl;
//        cerr << "the inpI and inpSDV set the mean and standard deviation input current." << endl;
//        exit (1);
//    }
//    else
//    {
//        filename = argv[1];
//
//        useVis = atoi (argv[2]);
//
//        sampN = atoi (argv[3]);
//        readN = atoi (argv[4]);
//
//        dataFilename = argv[5];
//        inpI = atof (argv[6]);
//        inpISDV = atof (argv[7]);
//
//        diameter = atof (argv[8]);
//        gLeak = atof (argv[9]);
//
//        temperature = atof (argv[10]);
//        q10 = atof (argv[11]);
//
//        sodiumDensity = atof (argv[12]);
//        potassiumDensity = atof (argv[13]);
//
//        numIterations = atoi (argv[14]);
//        timeStep = atof (argv[15]);
//
//        numTrials = atoi (argv[16]);
//
//    }
//
//
//
//    cerr << "Remember that data file should have more lines than Num iterations." << endl;
//    cout << "Storing data in" << filename << " every " << sampN << "th iteration" << endl;
//    cout << "Compartment Diameter [muMeter] " << diameter << endl;
//    cout << "Temperature [Celsius] " << temperature << endl;
//    cout << "Q10 for all ionic channel kinetics [#] " << q10 << endl;
//    cout << "Membrane leak conductance [mSiemens/cm^2] " << gLeak << endl;
//    cout << "Time step size in [mSec] " << timeStep << endl;
//    cout << "Num iterations [#] " << numIterations << endl;
//    cout << "Trial duration [ms] " << numIterations * timeStep << endl;
//    cout << "Na channels per mu^2" << sodiumDensity << endl;
//    cout << "K channels per mu^2" << potassiumDensity << endl;
//    cout << "Reading normal white noise from file " << dataFilename << " every " << readN << "th iteration." << endl;
//    cout << "White noise current mean [nA]" << inpI << " with SDV " << inpISDV << "." << endl;
//    cout << "Number of repated stimulus trials [#] " << numTrials << endl;
//
//    /* open files, 2DO error handling */
//    ofstream file (filename.c_str (), ios::binary);
//    ifstream dataFile (dataFilename.c_str (), ios::binary);
//
//    NT_uniform_rnd_dist_o testRnd;	// DO NOT DELETE, otherwise linker problems occur !
//    NT_gaussian_rnd_dist_o gaussianRnd;	// DO NOT DELETE, otherwise linker problems occur !
//    NTreal rnd = 0;
//
//    for (NTsize lTrials = 0; lTrials < numTrials; lTrials++) {
//        /* Model setup */
//        NTBP_spherical_compartment_o oModel(diameter/2.0	/* muMeter */,
//                                            1 /*muFarad/cm^2 */);
//        oModel.UpdateTimeStep (timeStep /* mSec */ );
//        oModel.StepNTBP ();
//
//        NTreal timeInMS = 0;
//
//        NT_uniform_rnd_dist_o uniRnd;
//        float tmpCurr = 0;
//
//        /*    Setup compartments */
//
//        oModel.Set_temperature (temperature /* in celsius */ );
//
//        /* Leak conductance */
//        oModel.AttachCurrent (new
//                              NTBP_hh_sga_leak_current_o (oModel._area (), gLeak	/* mSiemens/cm^2 */
//                                                         ), NTBP_LEAK);
//
//        /* Stochastic active ion-channel currents */
//        NTBP_multi_sodium_current_o *tmpNaPtr =
//            new NTBP_multi_sodium_current_o (oModel._area (),
//                                             sodiumDensity, 20e-9	/* mSiemens per channel */
//                                            );
////  tmpNaPtr->SetSimulationMode (NTBP_GILLESPIE);
//        tmpNaPtr->SetSimulationMode (NTBP_BINOMIALPOPULATION);
//        tmpNaPtr->Set_q10 (q10);
//        oModel.AttachCurrent (tmpNaPtr, NTBP_IONIC);
//
//        NTBP_multi_potassium_current_o *tmpKPtr =
//            new NTBP_multi_potassium_current_o (oModel._area (),
//                                                NTBP_SGA_SCHNEIDMAN,
//                                                potassiumDensity);
////	tmpKPtr->SetSimulationMode (NTBP_GILLESPIE);
//        tmpKPtr->SetSimulationMode (NTBP_BINOMIALPOPULATION);
//        tmpKPtr->Set_q10 (q10);
//        oModel.AttachCurrent (tmpKPtr, NTBP_IONIC);
//
//
//
//
//        /* Information measurement init */
//
//        /* *********************************************************  Main loop ************************************************ */
//        cerr << "MainLoop started" << endl;
//        float time = 0;
//        float vM = 0;
//        float deltaVM = 0;
//        float iL =0;
//        float iNa =0;
//        float iK =0;
//
//        oModel.Step(vM);
//
//        NT_uniform_rnd_dist_o uniformRnd;
//
//        NTreal voltVec;
//        NTreal speed;
//
//        cerr << "t [mSec]\tI_inj [nA]\tI_inj/A [nA/mum^2]\tv_maxVolt [m/s]" << endl;
//        for (NTsize lt = 0; lt < numIterations; lt++)
//        {
//            timeInMS += oModel._timeStep();
////				cerr << "******************** t=" << timeInMS << endl;
//
//            if (lt % sampN == 0)
//            {
//                /* the "sampling ratio" used for "measurement" to disk */
//                time = timeInMS;
//                vM = oModel._vM();
//                iL = oModel.AttachedCurrent(1);
//                iNa = oModel.AttachedCurrent(2);
//                iK  = oModel.AttachedCurrent(3);
//
//                file.write (reinterpret_cast < char *>(&inpCurrent),
//                            sizeof (float));
//                file.write (reinterpret_cast < char *>(&vM),
//                            sizeof (float));
//                file.write (reinterpret_cast < char *>(&iL),
//                            sizeof (float));
//                file.write (reinterpret_cast < char *>(&iNa),
//                            sizeof (float));
//                file.write (reinterpret_cast < char *>(&iK),
//                            sizeof (float));
//            }
//
//            if (lt % useVis == 0)
//            {
//                voltVec = oModel._vM();
//
//                if (NTisnan(voltVec))	{
//                    cerr << "ERROR at t=" << time <<" voltage in compartment "<<" is NaN." << endl;
//                    exit (1);
//                } else if (voltVec > 200.0 /* mV */)	{
//                    cerr << "ERROR at t=" << time <<" voltage in compartment "<<" is "<<voltVec<<"." << endl;
//                    exit (1);
//                }
//            }
//
//
//            if (lt % readN == 0)
//            {
//                dataFile.read (reinterpret_cast < char *>(&tmpCurr),sizeof (float));
//                inpCurrent = (tmpCurr * inpISDV) + inpI;
//            }
//            oModel.InjectCurrent (inpCurrent);
//
//
//            oModel.Step( vM );
//            deltaVM = 1.0e-3 /* sec /mSec */ * oModel.CompartmentMembraneNetCurrent() /* nA */ /oModel.CompartmentMembraneCapacitance() /* muF */;
//            vM += deltaVM * oModel._timeStep();
//            //oModel.GillespieStep();
//        }
//
//    } // lTrials
//    cerr << "Simulation completed." << endl;
//
    return 0;
}
