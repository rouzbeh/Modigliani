///** \file stochastic_ion_channels.cpp - NT demo of stochastic channel behaviour
// * by Ahmed Aldo Faisal &copy; created DATE
// */
///*
// * Copyright (C) 1999,2000 Ahmed Aldo Faisal
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
///* $Id: test_membrane_currents.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
// * $Log: test_membrane_currents.cpp,v $
// * Revision 1.2  2003/06/20 13:25:55  face
// * *** empty log message ***
// *
// * Revision 1.1  2001/06/29 13:16:51  face
// * *** empty log message ***
// *
//*/
//
//#include <cmath>
//#include <iostream>
//#include <string>
//
//
//#include "nt_error_obj.h"
//#include "nt_vector_obj.h"
//
//
//#include "ntbp_sodium_ion_channels_obj.h"
//#include "ntbp_potassium_ion_channels_obj.h"
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
//
//
//using namespace TNT;
//
int main(void)
{
//    NTreal vM = 0; // mV
//    NTreal deltaVM = 0;
//    NTreal cM = 1; // muF/cm^2
//    NTreal iInj = 0; // nA
//    NTreal area = 100 ;// muMeter^2
//
//    NTBP_hh_sga_leak_current_o hhL(area);
//    NTBP_hh_sga_potassium_current_o hhK(area);
//    NTBP_sga_sodium_current_o stNa(area);
//    NTBP_hh_sga_sodium_current_o hhNa(area);
//
//    cout << "vM" << "\t" <<"K current" << "\t" <<"Na current" << "\t" <<"Leak current" << endl;
//    cout << vM << "\t" << hhK._current()  << "\t" << stNa._current() << "\t" << hhL._current() << endl;
//
//    /*
//    	for (NTint lx=1; lx< 1400; lx++){
//    		vM = + 100/1000.0 *  lx - 40;
//    		for (NTint ll=1; ll < 10000; ll++){
//    			hhK.Step(vM);
//    			hhNa.Step(vM);
//    			hhL.Step(vM);
//    		}
//    		cout << vM << "\t";
//    */
//    for (NTint lx=1; lx< 600000; lx++) {
//        if (lx > 1000) iInj = 0.0003;
//        if (lx > 100000) iInj = 0;
//        if (lx > 200000) iInj = 0.0003;
//        if (lx > 500000) iInj = 0;
//        hhK.Step(vM);
//        stNa.Step(vM);
//        hhL.Step(vM);
//        deltaVM = (-hhK._current() - stNa._current() - hhL._current() + iInj )/(cM*area*1.0e-8);
//        vM += deltaVM * NTBP_TIMESTEP;
//
//        cout << lx*NTBP_TIMESTEP << "\t";
//        cout << vM << "\t I_K= " << hhK._current() << " ";
//        hhK.Show();
//        cout << "\t I_Na= " << stNa._current() << " ";
//        stNa.Show();
//        cout <<"\t I_L= " << hhL._current() << " ";
//        hhL.Show();
//        cout << "\t ";
//        stNa.ShowChannelStatePopulation();
//        cout << endl;
//    }
//
    return 0;
}
//
//

