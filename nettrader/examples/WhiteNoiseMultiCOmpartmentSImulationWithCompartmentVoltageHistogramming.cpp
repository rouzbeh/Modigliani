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


/* $Id: WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
* $Log: WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming.cpp,v $
* Revision 1.3  2003/06/20 13:25:55  face
* *** empty log message ***
*
* Revision 1.2  2002/03/04 18:08:31  face
* added checking for NaN and Inf values in find_spikes_in_binary_file-
*
* Revision 1.1  2001/06/29 13:16:51  face
* *** empty log message ***
*
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>

#include <cmath>
#include <iostream>
#include <string>


#include "ntbp_membrane_compartment_sequence_obj.h"
#include "ntbp_sga_cylindrical_compartment_obj.h"

#include "nt_error_obj.h"
#include "nt_vector_obj.h"

#include "nt_histogram_obj.h"
#include "nt_sequential_statistics_obj.h"
#include "tnt/tnt.h"
#include "tnt/cmat.h"
#include "nt_uniform_rnd_dist_obj.h"
#include "nt3d_plot2d_vec_vp_obj.h"
#include "nt3d_glx_drv_obj.h"

#include <fstream>
#include <iostream>

using namespace std;
using namespace TNT;


int main(int argc, char* argv[])
{
    /* Read and set parameters */
    NTreal currentAmount;
    NTreal diameter;
    NTreal length;
    NTsize numCompartments;
    NTreal timeStep;
    NTsize numIterations;

    if (3 > argc) {
        cerr << "Invalid comand line. Use > " << argv[0]
             << "<injector current amplitude in nA>  <compartment diameter in muMeter> <compartmentLength in muMeter> < number of Compartments > <time step size in mSec> <num iterations>."
             << "\n Using 0.01 nA. 20 muMeter, 1 muMeter, 50, 0.001 mSec, 100000 iterations." << endl;
        currentAmount = 0.01;
        diameter = 20.0;
        length = 1.0;
        numCompartments = 50;
        timeStep = 0.001;
        numIterations = 100000;
    }
    else {
        currentAmount = atof(	argv[1] );
        diameter = atof (argv[2] );
        length = atof (argv[3] );
        numCompartments = atoi (argv[4] );
        timeStep = atof ( argv[5] );
        numIterations = atoi ( argv[6] );
    }
    cout << "Current amplitude injected in stimulated compartment [nA] " << currentAmount << endl;
    cout << "Compartment diameter in muMeter " << diameter << endl;
    cout << "CompartmentLength in muMeter " << length << endl;
    cout << "Number of Compartments " << numCompartments << endl;
    cout << "time step size in mSec " << timeStep << endl;
    cout << "Num iterations " << numIterations << endl;

    NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur !
    NTreal rnd = 0;


    /* Model setup */
    NTBP_membrane_compartment_sequence_o oModel;
    oModel.UpdateTimeStep(timeStep /* mSec */ ) ;
    oModel.StepNTBP();

    NTreal timeInMS = 0;
    NTsize ll;
    for (ll=0; ll < numCompartments; ll++) {
        NTBP_sga_cylindrical_compartment_o * tmpPtr = new NTBP_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
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
    NT_histogram_o inpCurHist(0, currentAmount, 100);
    NT_histogram_o compHist(-20, 130, 150);
    vector < NT_histogram_o > histVec;
    histVec.resize(numCompartments);

    for (ll=0; ll < numCompartments; ll++) {
        histVec[ll] =	 compHist;
    }

    /* Main loop */
    cerr << "MainLoop started" << endl;
    oModel.Step();

    for (NTint lt = 0; lt < numIterations; lt++) {
        timeInMS += oModel._timeStep();

        if (lt%100 == 0) {
            cerr << "t=" << timeInMS << "mSec."<< endl;
            rnd = testRnd.RndVal();
            plotXY.SetData(oModel._vVec());
            plotXY.Draw();
            inpCurHist.BinValue(rnd*currentAmount);
        }

        for (ll=0; ll < numCompartments; ll++) {
            histVec[ll].BinValue( oModel.MembraneVoltage(ll+1) );
        }

        //oModel.ShowHinesMatrix();
        oModel.InjectCurrent(currentAmount*rnd, 1 );
        oModel.Step();
    }

    cout << "Input current entropy=" << inpCurHist.ShannonEntropy() << endl;
    inpCurHist.Show(true);
    for (ll=0; ll < numCompartments; ll++) {
        cout << endl;
        cout << "Compartment "<< ll + 1 << "voltage entropy=" << histVec[ll].ShannonEntropy() << endl;
        histVec[ll].Show(true);
    }


    cerr << "Simulation completed." << endl;

    return 0;
}
