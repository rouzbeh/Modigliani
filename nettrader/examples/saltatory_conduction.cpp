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


/* $Id: saltatory_conduction.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
* $Log: saltatory_conduction.cpp,v $
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
#include "ntbp_simple_myelinated_cylindrical_compartment_obj.h"
#include "ntbp_simple_ranvier_cylindrical_compartment_obj.h"
#include "ntbp_Na_only_sga_cylindrical_compartment_obj.h"
#include "ntbp_K_only_sga_cylindrical_compartment_obj.h"

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


#include <fstream>
#include <iostream>

using namespace std;
using namespace TNT;


int main(int argc, char* argv[])
{
    /* Read and set parameters */
    NTsize useVis;
    NTreal currentAmount;
    NTreal diameter;
    NTreal length;
    NTsize numCompartments;
    NTreal timeStep;
    NTsize numIterations;


    if (3 > argc) {
        cerr << "Invalid comand line. Use > " << argv[0]
             << "<visualisation interval> <injector current amplitude in nA>  <compartment diameter in muMeter> <compartmentLength in muMeter> < number of Compartments > <time step size in mSec> <num iterations>."
             << "\n 100 interations on update on graphics, Using 0.01 nA. 20 muMeter, 1 muMeter, 50, 0.001 mSec, 100000 iterations." << endl;
        useVis = 100;
        currentAmount = 0.01;
        diameter = 20.0;
        length = 1.0;
        numCompartments = 50;
        timeStep = 0.001;
        numIterations = 100000;
    }
    else {
        useVis = atoi( argv[1]	);
        currentAmount = atof(	argv[2] );
        diameter = atof (argv[3] );
        length = atof (argv[4] );
        numCompartments = atoi (argv[5] );
        timeStep = atof ( argv[6] );
        numIterations = atoi ( argv[7] );
    }
    cout << "Current amplitude injected in stimulated compartment [nA] " << currentAmount << endl;
    cout << "Compartment diameter in muMeter " << diameter << endl;
    cout << "CompartmentLength in muMeter " << length << endl;
    cout << "Number of Compartments " << numCompartments << endl;
    cout << "time step size in mSec " << timeStep << endl;
    cout << "Num iterations " << numIterations << endl;

    NT_uniform_rnd_dist_o testRnd; // DO NOT DELETE, otherwise linker problems occur !
    NT_gaussian_rnd_dist_o gaussianRnd; // DO NOT DELETE, otherwise linker problems occur !
    NTreal rnd = 0;


    /* Model setup */
    NTBP_membrane_compartment_sequence_o oModel;
    oModel.UpdateTimeStep(timeStep /* mSec */ ) ;
    oModel.StepNTBP();

    NTBP_membrane_compartment_sequence_o oBModel;
    oBModel.UpdateTimeStep(timeStep /* mSec */ ) ;
    oBModel.StepNTBP();

    NTreal timeInMS = 0;
    NTsize ll;
    for (ll=0; ll < (numCompartments); ll++) {
        NTBP_sga_cylindrical_compartment_o * tmpAPtr = new NTBP_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
        oBModel.PushBack(tmpAPtr);
    }






    NTBP_sga_cylindrical_compartment_o * tmpAPtr = new NTBP_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
    oModel.PushBack(tmpAPtr);

    NTBP_sga_cylindrical_compartment_o * tmpBPtr = new NTBP_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
    oModel.PushBack(tmpBPtr);

    NTBP_sga_cylindrical_compartment_o * tmpCPtr = new NTBP_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
    oModel.PushBack(tmpCPtr);

    NTBP_sga_cylindrical_compartment_o * tmpDPtr = new NTBP_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
    oModel.PushBack(tmpDPtr);


#define GAP 100
    for (ll=0; ll < ((float)numCompartments)/GAP - 1; ll++) {
        for (NTsize lk=0; lk < (GAP -1); lk++) {
            NTBP_simple_myelinated_cylindrical_compartment_o * tmpCPtr = new NTBP_simple_myelinated_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
            oModel.PushBack(tmpCPtr);
        }
        NTBP_na_only_sga_cylindrical_compartment_o * tmp4Ptr = new NTBP_na_only_sga_cylindrical_compartment_o(length /* muMeter */, diameter /* muMeter */);
        oModel.PushBack(tmp4Ptr);
    }
    oModel.Init();
    oModel.InitialStep();

    oBModel.Init();
    oBModel.InitialStep();

    /* Graphics init */
    NT3D_glx_drv_o drvVP(800,200);
    drvVP.SetWindowTitle("Voltage-Compartment-Plot");
    NT3D_plot2d_vec_vp_o plotXY(oModel._numCompartments());
    if (NT_FAIL == plotXY.Connect(&drvVP)) exit(1);
    plotXY.AutoRange(true);

    NT3D_glx_drv_o drvDVP(800,200);
    drvDVP.SetWindowTitle("Voltage-Compartment-Plot");
    NT3D_plot2d_vec_vp_o plotDXY(oBModel._numCompartments());
    if (NT_FAIL == plotDXY.Connect(&drvDVP)) exit(1);
    plotDXY.AutoRange(true);


    /* Main loop */
    cerr << "MainLoop started" << endl;
    oModel.Step();
//	oBModel.Step();

    for (NTsize lt = 0; lt < numIterations; lt++) {
        timeInMS += oModel._timeStep();

        if (lt%1000 == 0) { /* 1 kHZ white noise */
            rnd = testRnd.RndVal();
        }

        if (lt%useVis == 0) {
            cerr << "t=" << timeInMS << "mSec."<< endl;
            plotXY.SetData(oModel._vVec());
            plotXY.Draw();
            cerr << "t=" << timeInMS << "mSec."<< endl;
//			plotDXY.SetData(oBModel._vVec());
//			plotDXY.Draw();
        }

        oModel.InjectCurrent(currentAmount*rnd, 1 );
        oModel.Step();
        oBModel.InjectCurrent(currentAmount*rnd, 1 );
        oBModel.Step();
    }


    cerr << "Simulation completed." << endl;

    return 0;
}
