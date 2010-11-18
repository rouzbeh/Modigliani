/**\file main.cpp - demo file
 * by Ahmed Aldo Faisal &copy; created ..2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation sy
stem
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


/* $Id: diffusion_signalling.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
* $Log: diffusion_signalling.cpp,v $
* Revision 1.3  2003/06/20 13:25:55  face
* *** empty log message ***
*
* Revision 1.2  2002/03/04 18:08:31  face
* added checking for NaN and Inf values in find_spikes_in_binary_file-
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


int main(int argc, char* argv[])
{
    /* Read and set parameters */
    string filename;
    NTsize useVis;
    NTreal currentAmount;
    NTreal diameter;
    NTreal length;
    NTsize numCompartments;
    NTreal timeStep;
    NTsize numIterations;

    bool waitForSpike = false;


    if (3 > argc) {
        cerr << "Invalid comand line. Use > " << argv[0]
             << "<FILENAME> <visualisation interval> <injector current amplitude in nA>  <compartment diameter in muMeter> <compartmentLength in muMeter> < number of Compartments > <time step size in mSec> <num iterations>."
             << "\n storing data in data.dat, 100 interations on update on graphics, Using 0.01 nA. 20 muMeter, 1 muMeter, 50, 0.001 mSec, 100000 iterations." << endl;
        filename = "data.dat";
        useVis = 100;
        currentAmount = 0.01;
        diameter = 20.0;
        length = 1.0;
        numCompartments = 50;
        timeStep = 0.001;
        numIterations = 100000;
    }
    else {
        filename = argv[1];
        useVis = atoi( argv[2]	);
        currentAmount = atof(	argv[3] );
        diameter = atof (argv[4] );
        length = atof (argv[5] );
        numCompartments = atoi (argv[6] );
        timeStep = atof ( argv[7] );
        numIterations = atoi ( argv[8] );
    }
    cout << "Storing data in" << filename;
    cout << "Current amplitude injected in stimulated compartment [nA] " << currentAmount << endl;
    cout << "Compartment diameter in muMeter " << diameter << endl;
    cout << "CompartmentLength in muMeter " << length << endl;
    cout << "Number of Compartments " << numCompartments << endl;
    cout << "time step size in mSec " << timeStep << endl;
    cout << "Num iterations " << numIterations << endl;
    cout << "Data format is : peakTimeInMS, inpCurrent, peakPos, total cumul leakCurr, total cumul naCurr, toal cumul injCurr 	" << endl;
    ofstream file(filename.c_str(), ios::binary);
//  file.width(12);
// file.precision(9);

    NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur !
    NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !
    NTreal rnd = 0;

    /* Model setup */
    NTBP_membrane_compartment_sequence_o oModel;
    oModel.UpdateTimeStep(timeStep /* mSec */ ) ;
    oModel.StepNTBP();

    NTreal timeInMS = 0;
    for (NTsize ll=0; ll < numCompartments; ll++) {
        NTBP_custom_cylindrical_compartment_o * tmpPtr =
            new NTBP_custom_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */, 1 /*muFarad/cm^2*/, 35.4 /* ohm cm */);
        tmpPtr->AttachCurrent( new NTBP_hh_sga_leak_current_o( tmpPtr->_area() ), NTBP_LEAK );

//* Stochastic, single channel currents */
//	tmpPtr->AttachCurrent( new NTBP_sga_sodium_current_o( tmpPtr->_area() ), NTBP_IONIC);
//		tmpPtr->AttachCurrent( new NTBP_delayed_potassium_rectifier_current_o( tmpPtr->_area(),NTBP_SGA_K_CONTI ), NTBP_IONIC);

        /* Deterministic channel currents */
//AttachCurrent( new NTBP_hh_sga_sodium_current_o( _area() ), NTBP_IONIC );
//AttachCurrent( new NTBP_hh_sga_potassium_current_o( _area() ), NTBP_IONIC );

        /* Multi method channel currents */
        /*		NTBP_multi_sodium_current_o* tmpNaPtr = new NTBP_multi_sodium_current_o( tmpPtr->_area() );
        		tmpNaPtr->SetSimulationMode(NTBP_DETERMINISTIC);
        		tmpPtr->AttachCurrent( tmpNaPtr , NTBP_IONIC );

        		NTBP_multi_potassium_current_o* tmpKPtr = new NTBP_multi_potassium_current_o( tmpPtr->_area() ,NTBP_SGA_K_CONTI );
        		tmpKPtr->SetSimulationMode(NTBP_DETERMINISTIC);
        		tmpPtr->AttachCurrent( tmpKPtr  , NTBP_IONIC );
        */
        oModel.PushBack(tmpPtr);
    }

    oModel.Init();
    oModel.InitialStep();

    /* Graphics init */
    NT3D_glx_drv_o drvVP(800,200);
    drvVP.SetWindowTitle("Voltage-Compartment-Plot");
    NT3D_plot2d_vec_vp_o plotXY(numCompartments);
    if (NT_FAIL == plotXY.Connect(&drvVP)) exit(1);
    plotXY.AutoRange(true);
    //	plotXY.SetXRange(0,numCompartments);
    //	plotXY.SetYRange(-10,120);


    /* *********************************************************  Main loop ************************************************ */
    cerr << "MainLoop started" << endl;
    oModel.Step();

    NTreal timeToNextSpike = 25.0;
    NTreal injectionTime = 10.0;
    float leakCurr = 0;
    float injIntCurr = 0;
    float peakPos = 0;
    NTreal startCurr = 0;
    NTreal inpCurrent = 0;


    for (startCurr = currentAmount; startCurr <  currentAmount*1000000; startCurr += startCurr/10.0) {
        timeInMS = 0;
        timeToNextSpike = 25.0;
        injectionTime = 25.0;
        leakCurr = 0;
        injIntCurr = 0;
        peakPos = 0;
        inpCurrent = 0;
        float peakTime = 0;

        for (NTsize lt = 0; lt < numIterations; lt++) {
            timeInMS += oModel._timeStep();

            if (lt%useVis == 0) {
                cerr << "t=" << timeInMS << "mSec. Time to spike is " << timeToNextSpike << endl;
                plotXY.SetData(oModel._vVec());
                plotXY.Draw();
            }

            NTsize ll;
            for (ll = 0; ll < numCompartments; ll++) {
                leakCurr += oModel.AttachedCurrent(ll+1,1);
                injIntCurr += inpCurrent;
            }

            for (ll = numCompartments; ll > 0;   ll--) {
                if ((oModel.MembraneVoltage(ll) > 20) &&
                        (ll > peakPos)
                   ) {
                    peakPos = (float) ll;
                    peakTime = timeInMS;
                }
            }

            if (timeToNextSpike < 1.0) {
                float time = timeInMS;
            }

            if (timeToNextSpike <= 0.0) {
                inpCurrent = startCurr;
                injectionTime -= oModel._timeStep();
                if (injectionTime < 0) inpCurrent = 0;
            }

            timeToNextSpike -= oModel._timeStep();


            oModel.InjectCurrent(inpCurrent, 1);
            oModel.Step();

            if (numCompartments == peakPos) break;
        }
        cerr << "WRITE" << endl;
        file.write( reinterpret_cast<char*>(&peakTime), sizeof(float) );
        file.write( reinterpret_cast<char*>(&inpCurrent), sizeof(float) );
        file.write( reinterpret_cast<char*>(&peakPos), sizeof(float) );
        file.write( reinterpret_cast<char*>(&leakCurr), sizeof(float));
        file.write( reinterpret_cast<char*>(&injIntCurr), sizeof(float));

        if (numCompartments == peakPos) break;
    }
    cerr << "Simulation completed." << endl;

    return 0;
}
