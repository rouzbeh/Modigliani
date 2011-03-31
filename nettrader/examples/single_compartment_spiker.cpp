///**\file main.cpp - demo file
// * by Ahmed Aldo Faisal &copy; created ..2001
// */
///* NetTrader - visualisation, scientific and financial analysis and simulation sy
//stem
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
///* $Id: single_compartment_spiker.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
//* $Log: single_compartment_spiker.cpp,v $
//* Revision 1.2  2003/06/20 13:25:55  face
//* *** empty log message ***
//*
//* Revision 1.1  2001/06/29 13:16:51  face
//* *** empty log message ***
//*
//*/
//
//#ifdef HAVE_CONFIG_H
//#include <config.h>
//#endif
//
//#include <iostream>
//#include <stdlib.h>
//
//#include <cmath>
//#include <iostream>
//#include <string>
//
//
//#include "nt_error_obj.h"
//#include "nt_vector_obj.h"
//
//#include "nt_simple_markov_obj.h"
//
//#include "ntbp_sodium_ion_channels_obj.h"
//#include "ntbp_potassium_ion_channels_obj.h"
//
//#include "nt_sequential_statistics_obj.h"
//
//#include "tnt/tnt.h"
//#include "tnt/cmat.h"
//
//#include"ntbp_hh_sga_potassium_current_obj.h"
//#include"ntbp_hh_sga_sodium_current_obj.h"
//#include"ntbp_hh_sga_leak_current_obj.h"
//#include"ntbp_delayed_potassium_rectifier_current_obj.h"
//#include"ntbp_sga_sodium_current_obj.h"
//#include"ntbp_membrane_compartment_sequence_obj.h"
//#include"ntbp_sga_cylindrical_compartment_obj.h"
//
//
//
//#include "nt_uniform_rnd_dist_obj.h"
//
//#include <fstream>
//#include <iostream>
//
//using namespace std;
//using namespace TNT;
//
//
int main(int argc, char* argv[])
{
//    NT_ASSERT(2==argc);
//    float currentAmount = atof(	argv[1] );
//    cout << currentAmount << endl;
//
//    NTsize numCompartments = 5;
//    NTreal timeInMS = 0;
//
//    NTBP_membrane_compartment_sequence_o oModel;
//
//    for (NTsize ll=0; ll < numCompartments; ll++) {
//        NTBP_sga_cylindrical_compartment_o * tmpPtr = new NTBP_sga_cylindrical_compartment_o(10 /* muMeter */);
//        oModel.PushBack(tmpPtr);
//    }
//
//    NTBP_sga_cylindrical_compartment_o	oCompartment(10 /* muMeter */);
//
//    cout << "Area [muMeter^2] " << oCompartment._area() << "\tSpec. Membrane Capacitance [muF/cm^2] " << oCompartment._cM()
//         << "\t Axoplasmic resistivity in [Ohm cm] " <<oCompartment._rA()  << endl;
//    for (NTint lt=1; lt< 1000.0/NTBP_TIMESTEP; lt++) {
//        oCompartment.Step( oCompartment._vM() );
//        oCompartment.InjectCurrent(currentAmount);
//        timeInMS = lt*NTBP_TIMESTEP;
//        cout <<	timeInMS <<	"\t" << oCompartment._vM() << endl;
//    }
//
//    /*
//    oModel.Init();
//    cerr << "InitialStep" << endl;
//    oModel.InitialStep();
//
//    cerr << "MainLoop" << endl;
//    for (NTint lt=1; lt< 1000.0/NTBP_TIMESTEP; lt++){
//    	timeInMS = lt*NTBP_TIMESTEP;
//    	cout << timeInMS <<"\t";
//    	oModel.Step();
//    	oModel.InjectCurrent(0.0, 1);
//    //	oModel.InjectCurrent(0.0001, 2);
//    //	oModel.InjectCurrent(0.001, 3);
//    //	oModel.InjectCurrent(0.01, 4);
//    //	oModel.InjectCurrent(0.1, 5);
//    	cout << endl;
//    }
//    */
//
    return 0;
}
