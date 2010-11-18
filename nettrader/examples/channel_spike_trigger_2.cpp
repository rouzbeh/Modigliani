/**\file channel_spike_trigger_2.cpp - based on channel spike trigger
 * by Ahmed Aldo Faisal &copy; created 25.9.2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000,2001 Ahmed Aldo Faisal
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


/* $Id: channel_spike_trigger_2.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
* $Log: channel_spike_trigger_2.cpp,v $
* Revision 1.2  2003/06/20 13:25:55  face
* *** empty log message ***
*
* Revision 1.1  2003/01/30 17:13:39  face
* *** empty log message ***
*
* Revision 1.3  2002/03/08 15:13:24  face
* *** empty log message ***
*
* Revision 1.2  2001/10/12 09:18:34  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:36:53  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:16:51  face
* *** empty log message ***
*
* Revision 1.1  2001/06/11 13:18:10  face
* *** empty log message ***
*
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

#include "ntbp_membrane_compartment_sequence_obj.h"
#include "ntbp_sga_cylindrical_compartment_obj.h"
#include "ntbp_custom_cylindrical_compartment_obj.h"

#include "nt_error_obj.h"
#include "nt_vector_obj.h"

#include "nt_histogram_obj.h"
#include "nt_sequential_statistics_obj.h"
#include "tnt/tnt.h"
#include "tnt/cmat.h"
#include "nt_uniform_rnd_dist_obj.h"
#include "nt_binomial_rnd_dist_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
#include "nt3d_plot2d_vec_vp_obj.h"
#include "nt3d_glx_drv_obj.h"




using namespace std;
using namespace TNT;


int
main (int argc, char *argv[])
{
    /* Read and set parameters */
    float inpCurrent = 0;
    string filename;
    NTsize useVis;
    NTsize sampN;
    NTreal temperature;		//= 37; /* in Celsius */
    NTreal q10;
    NTreal diameter;		// = 1; /* in muMeter */
    NTreal gLeak = 0.05;	/* mSiemens/cm^2 */
    NTreal rA = 70;		/* Ohm cm */

    NTsize numCompartments;
    NTreal timeStep;		// in msec
    NTsize numIterations;
    NTreal sodiumDensity;	//= 60; // per mu^2
    NTreal potassiumDensity;	// = 18; //  per mu^2
    string dataFilename;
    NTsize readN;
    NTreal inpI;
    NTreal inpISDV;
    NTreal numTrials = 0;
    NTreal length = 0;
    NTsize numActiveChannels = 1;

    if (21 != argc)
    {
        cerr << "Invalid command line. Use > " << argv[0] <<"<1 storage FILENAME>" << endl;
        cerr << "<2 visualisation interval> <3 sample to storage file every n steps> <4 read data file every n steps> " 	      << endl;
        cerr << "<5 data filename> <6 inpI [nA] > <7 inpSDV [nA]> " << endl;
        cerr << "<8 diameter [mum (micrometer)]> <9 gLeak [mS/cm^2] > <10 R_axoplasmic [Ohm cm]> "	      << endl;
        cerr << "<11 temperature [Celsius] > <12 q10 for ion channels>" <<endl;
        cerr << "<13 Na channel density [per mum^2]> <14 K channel density [per mum^2]> "<< endl;
        cerr << "<15 numIterations> <16 timeStep [ms]>" << endl;
        cerr << "<17 numCompartments > <18 length of compartment [mum]" <<endl;
        cerr << "<19 numTrials> <20 number of addionally Na open channels>" << endl;
        cerr << endl;
        cerr << "Set inpI and inpSDV to 0 for zero input." << endl;
        cerr << "Set inpI to 0 and inpSDV to 1 for 1:1 readout of data file float values" << endl;
        cerr << "If data file contains normal distributed floating point random numbers..." << endl;
        cerr << "the inpI and inpSDV set the mean and standard deviation input current." << endl;
        exit (1);
    }
    else
    {
        filename = argv[1];

        useVis = atoi (argv[2]);

        sampN = atoi (argv[3]);
        readN = atoi (argv[4]);

        dataFilename = argv[5];
        inpI = atof (argv[6]);
        inpISDV = atof (argv[7]);

        diameter = atof (argv[8]);
        gLeak = atof (argv[9]);
        rA = atof (argv[10]);

        temperature = atof (argv[11]);
        q10 = atof (argv[12]);

        sodiumDensity = atof (argv[13]);
        potassiumDensity = atof (argv[14]);

        numIterations = atoi (argv[15]);
        timeStep = atof (argv[16]);

        numCompartments = atoi (argv[17]);
        length = atof (argv[18]);

        numTrials = atoi (argv[19]);


        numActiveChannels = atoi(argv[20]);
    }


    int midCompartment = numCompartments/2;

    cerr << "Remember that data file should have more lines than Num iterations." << endl;
    cout << "Storing data in" << filename << " every " << sampN << "th iteration" << endl;
    cout << "Compartment Diameter [muMeter] " << diameter << endl;
    cout << "Compartment Length [muMeter] " << length << endl;
    cout << "Temperature [Celsius] " << temperature << endl;
    cout << "Q10 for all ionic channel kinetics [#] " << q10 << endl;
    cout << "Membrane leak conductance [mSiemens/cm^2] " << gLeak << endl;
    cout << "Axoplasmic resistivity [Ohm cm] " << rA << endl;
    cout << "Time step size in [mSec] " << timeStep << endl;
    cout << "Num iterations [#] " << numIterations << endl;
    cout << "Trial duration [ms] " << numIterations * timeStep << endl;
    cout << "Na channels per mu^2" << sodiumDensity << endl;
    cout << "K channels per mu^2" << potassiumDensity << endl;
    cout << "Reading normal white noise from file " << dataFilename << " every " << readN << "th iteration." << endl;
    cout << "White noise current mean [nA]" << inpI << " with SDV " << inpISDV << "." << endl;
    cout << "Number of repated stimulus trials [#] " << numTrials << endl;
    cout << "Length of compartment [muMeter] " << length << endl;
    cout << "Number of Compartments " << numCompartments << endl;
    cout << "Passive length const (gL based) of cable [muMeter] " << NTBP_LengthConstantPassiveCable (diameter, rA,gLeak) << " muMeter." << endl;
    cout << "Compartment membrane area " << M_PI * diameter * length << " [muMeter^2]." << endl;



    /* open files, 2DO error handling */
    ofstream file (filename.c_str (), ios::binary);
    ifstream dataFile (dataFilename.c_str (), ios::binary);

    NT_uniform_rnd_dist_o testRnd;	// DO NOT DELETE, otherwise linker problems occur !
    NT_gaussian_rnd_dist_o gaussianRnd;	// DO NOT DELETE, otherwise linker problems occur !
    NTreal rnd = 0;



    for (NTsize lTrials = 0; lTrials < numTrials; lTrials++) {
        /* Model setup */
        NTBP_membrane_compartment_sequence_o oModel;
        oModel.UpdateTimeStep (timeStep /* mSec */ );
        oModel.StepNTBP ();

        NTreal timeInMS = 0;

        /* Take into account non-integer numbers of channels,
           given a*floor(naPerCompartment) + b*ceil(naPerCompartment) = naDensity
           pick for each compartment with prob a/b whether ceil or floor numbers of channel to be used
         */
        NTreal naPerCompartment = (diameter * M_PI * length) * sodiumDensity;
        cout << "Numeric density of Na-channels per compartment :" <<	naPerCompartment << endl;
        NTreal kPerCompartment = (diameter * M_PI * length) * potassiumDensity;
        cout << "Numeric density of K-channels per compartment :" << kPerCompartment << endl;

        NTreal aMixNa =	numCompartments * (ceil (naPerCompartment) - naPerCompartment);
        NTreal pNaFloor = aMixNa / NTreal (numCompartments);

        NTreal aMixK =	numCompartments * (ceil (kPerCompartment) - kPerCompartment);
        NTreal pKFloor = aMixK / NTreal (numCompartments);

        cout << "pNaFloor=" << pNaFloor << " " << "pKFloor=" << pKFloor << endl;

        NT_uniform_rnd_dist_o uniRnd;
        NTreal totalNaChannels = 0;
        NTreal totalKChannels = 0;
        NTreal areaPerCompartment;	// in $\mu m^2$
        float tmpCurr = 0;

        /*    Setup compartments */
        for (NTsize ll = 0; ll < numCompartments; ll++)
        {
            /* Create a cylindrical membrane compartment */
            NTBP_custom_cylindrical_compartment_o *tmpPtr =
                new NTBP_custom_cylindrical_compartment_o (length	/* muMeter */,
                        diameter	/* muMeter */,
                        1 /*muFarad/cm^2 */ ,
                        rA /* ohm cm */ );
            areaPerCompartment = tmpPtr->_area ();

            tmpPtr->Set_temperature (temperature /* in celsius */ );

            /* compute number of channels, such that average density is achieved */
            NTreal indSodiumDensity =
                (uniRnd.RndVal () >
                 pNaFloor ? ceil (naPerCompartment) /
                 tmpPtr->_area () : floor (naPerCompartment) /
                 tmpPtr->_area ());
            NTreal indPotassiumDensity =
                (uniRnd.RndVal () >
                 pKFloor ? ceil (kPerCompartment) /
                 tmpPtr->_area () : floor (kPerCompartment) / tmpPtr->_area ());
            cout << indSodiumDensity *
                 tmpPtr->_area () << "\t" << indPotassiumDensity *
                 tmpPtr->_area () << endl;

            totalNaChannels += indSodiumDensity * (tmpPtr->_area ());
            totalKChannels += indPotassiumDensity * (tmpPtr->_area ());

            /* Leak conductance */
            tmpPtr->AttachCurrent (new
                                   NTBP_hh_sga_leak_current_o (tmpPtr->_area (), gLeak	/* mSiemens/cm^2 */
                                                              ), NTBP_LEAK);

            /* Stochastic active ion-channel currents */
            NTBP_multi_sodium_current_o *tmpNaPtr =
                new NTBP_multi_sodium_current_o (tmpPtr->_area (),
                                                 indSodiumDensity, 20e-9	/* mSiemens per channel */
                                                );
            tmpNaPtr->SetSimulationMode (NTBP_BINOMIALPOPULATION);
            tmpNaPtr->Set_q10 (q10);
            tmpPtr->AttachCurrent (tmpNaPtr, NTBP_IONIC);

            NTBP_multi_potassium_current_o *tmpKPtr =
                new NTBP_multi_potassium_current_o (tmpPtr->_area (),
                                                    NTBP_SGA_SCHNEIDMAN,
                                                    indPotassiumDensity);
            tmpKPtr->SetSimulationMode (NTBP_BINOMIALPOPULATION);
            tmpKPtr->Set_q10 (q10);
            tmpPtr->AttachCurrent (tmpKPtr, NTBP_IONIC);

            oModel.PushBack (tmpPtr);
        }
        cout << "Average density of Na channels per compartment =" <<
             totalNaChannels / NTreal (numCompartments) << endl;
        cout << "Average density of K channels per compartment =" <<
             totalKChannels / NTreal (numCompartments) << endl;

        oModel.Init ();


        /* Information measurement init */
        vector < NTreal > leakCurrVec (numCompartments);
        vector < NTreal > naCurrVec (numCompartments);
        vector < NTreal > kCurrVec (numCompartments);


        /* *********************************************************  Main loop ************************************************ */
        cerr << "MainLoop started" << endl;
        oModel.Step ();

        NT_uniform_rnd_dist_o uniformRnd;

        vector < NTreal > voltVec;
        vector < NTreal >::iterator maxVoltPos;
        vector < NTreal >::iterator maxVoltOldPos;
        NTreal speed;

        NTreal maxVoltage;

        cerr << "t [mSec]\tI_inj [nA]\tI_inj/A [nA/mum^2]\tv_maxVolt [m/s]" << endl;
        for (NTsize lt = 0; lt < numIterations; lt++)
        {
            timeInMS += oModel._timeStep();
//				cerr << "******************** t=" << timeInMS << endl;

            if (lt % sampN == 0)
            {			/* the "sampling ratio" used for "measurement" to disk */
                float time = timeInMS;
                file.write (reinterpret_cast < char *>(&time),
                            sizeof (float));
                file.write (reinterpret_cast < char *>(&inpCurrent),
                            sizeof (float));
                oModel.WriteMembranePotential (file);
                oModel.WriteCurrent(file,1); // Leak
                oModel.WriteCurrent(file,2); // Na
                oModel.WriteCurrent(file,3); // K
            }





            voltVec = oModel._vVec ();
            for (NTsize lc = 0; lc++; lc<numCompartments) {
                if (NTisnan(voltVec[lc]))	{
                    cerr << "ERROR at t=" << time <<" voltage in compartment "<<lc<<" is NaN." << endl;
                    exit (1);
                } else if (voltVec[lc] > 200.0 /* mV */)	{
                    cerr << "ERROR at t=" << time <<" voltage in compartment "<<lc<<" is "<<voltVec[lc]<<"." << endl;
                    exit (1);
                }
            }




            if ((lt % readN == 0) && (timeInMS > 20))
            {
                inpCurrent /* nA */ = (115 /* mV */ - voltVec[midCompartment]) * 20 /* pS */ * 0.000001 * numActiveChannels;
                oModel.InjectCurrent (inpCurrent, midCompartment);
            }


            oModel.Step();
            //oModel.GillespieStep();
        }

    } // lTrials
    cerr << "Simulation completed." << endl;

    return 0;
}
