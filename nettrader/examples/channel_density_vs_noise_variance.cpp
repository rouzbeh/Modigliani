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
///* $Id: channel_density_vs_noise_variance.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
// * $Log: channel_density_vs_noise_variance.cpp,v $
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
//
//
//#include "nt_uniform_rnd_dist_obj.h"
//
//#include <fstream>
//#include <iostream>
//
//using namespace std;
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
//    NTreal area = 1 ;// muMeter^2
//
//    ofstream file("voltage_variance_vs_channel_density_sga_1min.dat");
//    ofstream file2("interspike_intervals_at_various_channel_densities_sga_1min.dat");
//
//
//
//    file2.width(8);
//
//    bool spike = false;
//    NTreal lastSpike = 0.0;
//    NTreal timeInMS = 0.0;
//
//    NTsize densities[7] = {10, 50,
//                           100, 330,
//                           1000, 5000,
//                           10000
//                          };
//
//    file << "Na channel density \t K channel density \t voltage mean \t variance \t min \t max \t spikes" << endl;
//    file2 << "Na channel density \t K channel density \t interspike interval" << endl;
//    for (NTint lld = 0; lld < 7; lld++) {
//
//        NTBP_hh_sga_leak_current_o hhL(area);
//
//        NTBP_delayed_potassium_rectifier_current_o stK(area, NTBP_SGA_K_CONTI);
//        stK.SetDensity( ceil(densities[lld]/11.0) );
//
//        NTBP_sga_sodium_current_o stNa(area);
//        stK.SetDensity(densities[lld]);
//
//        NT_sequential_statistics_o stats;
//
//        NTsize spikeCount = 0;
//
//        for (NTint lt=1; lt< 60000/NTBP_TIMESTEP; lt++) {
//            timeInMS = lt*NTBP_TIMESTEP ;
//            stK.Step(vM);
//            stNa.Step(vM);
//            hhL.Step(vM);
//            deltaVM = (-stK._current() - stNa._current() - hhL._current() + iInj )/(cM*area*1.0e-8);
//            vM += deltaVM * NTBP_TIMESTEP;
//
//            stats.Add(vM);
//
//            if (vM > 40.0) {
//                if (spike != true) {
//                    spike = true;
//                    file2 << densities[lld] << "\t" << ceil(densities[lld]/11.0) << "\t" << (float) (timeInMS - lastSpike) << endl;
//                    lastSpike = timeInMS;
//                    spikeCount++;
//                }
//            } else {
//                spike = false;
//            }
//
//        }
//        file << densities[lld] << "\t" << densities[lld]/11.0
//        << "\t" << stats._average() << "\t" << stats._variance()
//        << "\t" << stats._min() << "\t" << stats._max()
//        << "\t" << spikeCount
//        << endl;
//
//    }
//
    return 0;
}
//
//

