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


/* $Id: gaussian_isi_spikes_on_langevin_axon.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
* $Log: gaussian_isi_spikes_on_langevin_axon.cpp,v $
* Revision 1.2  2003/06/20 13:25:55  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:16:51  face
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
#include "nt_gaussian_rnd_dist_obj.h"
#include "nt3d_plot2d_vec_vp_obj.h"
#include "nt3d_glx_drv_obj.h"




using namespace std;
using namespace TNT;


int main(int argc, char* argv[])
{
    /* Read and set parameters */
    float inpCurrent = 0;
    string filename;
    NTsize useVis;
    NTreal currentAmount;
    NTreal diameter;
    NTreal length;
    NTsize numCompartments;
    NTreal timeStep;
    NTsize numIterations;

    bool waitForSpike = false;
    NTreal timeToNextSpike = 0.0;



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

        /* Stochastic, single channel currents */
        // AttachCurrent( new NTBP_sga_sodium_current_o( _area() ), NTBP_IONIC);
        //	 AttachCurrent( new NTBP_delayed_potassium_rectifier_current_o( _area(),NTBP_SGA_K_CONTI ), NTBP_IONIC);

        /* Deterministic channel currents */
        //	AttachCurrent( new NTBP_hh_sga_sodium_current_o( _area() ), NTBP_IONIC );
        //	AttachCurrent( new NTBP_hh_sga_potassium_current_o( _area() ), NTBP_IONIC );

        /* Multi method channel currents */
        NTBP_multi_sodium_current_o* tmpNaPtr = new NTBP_multi_sodium_current_o( tmpPtr->_area() );
        tmpNaPtr->SetSimulationMode(NTBP_LANGEVIN);
        tmpPtr->AttachCurrent( tmpNaPtr , NTBP_IONIC );

        NTBP_multi_potassium_current_o* tmpKPtr = new NTBP_multi_potassium_current_o( tmpPtr->_area() ,NTBP_SGA_K_CONTI );
        tmpKPtr->SetSimulationMode(NTBP_LANGEVIN);
        tmpPtr->AttachCurrent( tmpKPtr  , NTBP_IONIC );

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

    /* Information measurement init */

    vector <NTreal>  leakCurrVec(numCompartments);
    vector <NTreal>  naCurrVec(numCompartments);
    vector <NTreal>  kCurrVec(numCompartments);

    float leakCurrPtr[numCompartments];
    float naCurrPtr[numCompartments];
    float kCurrPtr[numCompartments];

    /* *********************************************************  Main loop ************************************************ */
    cerr << "MainLoop started" << endl;
    oModel.Step();

    for (NTsize lt = 0; lt < numIterations; lt++) {
        timeInMS += oModel._timeStep();

        if (lt%1000 == 0) { /* 1000 kHZ measurement */
            float time = timeInMS;
            file.write( reinterpret_cast<char*>(&time), sizeof(float) );
            file.write( reinterpret_cast<char*>(&inpCurrent), sizeof(float) );
            oModel.WriteMembranePotential(file);
        }


        if (lt%useVis == 0) {
            cerr << "t=" << timeInMS << "mSec. Time to spike is " << timeToNextSpike << endl;
            plotXY.SetData(oModel._vVec());
            plotXY.Draw();
        }

        //oModel.ShowHinesMatrix();
        if (timeToNextSpike <= 0.0) {
            inpCurrent = currentAmount;
            oModel.InjectCurrent(currentAmount, 1 );
            while (timeToNextSpike <= 0.0) {
                timeToNextSpike = gaussianRnd.RndVal()*10.0 + 25.0; /* pick interspike interval, in ms */
            }
        }	else {
            inpCurrent = 0;
            oModel.InjectCurrent(inpCurrent, 1);
            timeToNextSpike -= oModel._timeStep();
        }



        oModel.Step();
    }

    cerr << "Simulation completed." << endl;

    return 0;
}
