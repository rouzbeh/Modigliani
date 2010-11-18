/** \file stochastic_ion_channels.cpp - NT demo of stochastic channel behaviour
 * by Ahmed Aldo Faisal &copy; created DATE
 */
/*
 * Copyright (C) 1999,2000 Ahmed Aldo Faisal
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


/* $Id: stochastic_ion_channels.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
 * $Log: stochastic_ion_channels.cpp,v $
 * Revision 1.2  2003/06/20 13:25:55  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
*/

#include <cmath>
#include <iostream>
#include <string>


#include "nt_error_obj.h"
#include "nt_vector_obj.h"


#include "ntbp_sodium_ion_channels_obj.h"
#include "ntbp_potassium_ion_channels_obj.h"

#include "nt_sequential_statistics_obj.h"

#include "tnt/tnt.h"
#include "tnt/cmat.h"

#include"ntbp_hh_sga_potassium_current_obj.h"
#include"ntbp_hh_sga_sodium_current_obj.h"
#include"ntbp_hh_sga_leak_current_obj.h"
#include"ntbp_delayed_potassium_rectifier_current_obj.h"
#include"ntbp_sga_sodium_current_obj.h"


#include "nt_uniform_rnd_dist_obj.h"

#include <fstream>
#include <iostream>

using namespace std;


using namespace TNT;

int main(void)
{
    NTreal vM = 0; // mV
    NTreal deltaVM = 0;
    NTreal cM = 1; // muF/cm^2
    NTreal iInj = 0; // nA
    NTreal area = 1 ;// muMeter^2

    ofstream file("time_vM_whitenoise1.0e-8_sga_1h.bin.dat", ios::binary);
    ofstream file2("interspike_intervals_at_hitenoise1.0e-8_sga_1h.dat");

    NTBP_hh_sga_leak_current_o hhL(area);

    NTBP_delayed_potassium_rectifier_current_o stK(area, NTBP_SGA_K_CONTI);
    NTBP_hh_sga_potassium_current_o hhK(area);

    NTBP_sga_sodium_current_o stNa(area);
    NTBP_hh_sga_sodium_current_o hhNa(area);

    cout << "iteration" << "\t voltage \t K current" << "\t" <<"Na current" << "\t" <<"Leak current" << endl;
    cout << vM << "\t" << stK._current()  << "\t" << stNa._current() << "\t" << hhL._current() << endl;

    file.width(12);
    file.precision(10);
    file2.width(8);

    bool spike = false;
    NTreal lastSpike = 0.0;
    NTreal timeInMS = 0.0;

    float data[3];

    NT_uniform_rnd_dist_o uniform(-1.0e-6, 1.0e-6);

    for (NTint lt=1; lt< 6000000.0/NTBP_TIMESTEP; lt++) {

        timeInMS = lt*NTBP_TIMESTEP ;

        hhK.Step(vM);
        hhNa.Step(vM);
        hhL.Step(vM);
        iInj = uniform.RndVal();
        deltaVM = (-hhK._current() - hhNa._current() - hhL._current() + iInj )/(cM*area*1.0e-8);
        vM += deltaVM * NTBP_TIMESTEP;

        data[0] = timeInMS;
        data[1] = vM;
        data[2] = iInj;
        cout << vM << endl;
        if (vM > 40.0) {
            if (spike != true) {
                spike = true;
                file2 << (float) (timeInMS - lastSpike) << endl;
                lastSpike = timeInMS;
            }
        } else {
            spike = false;
        }
        file.write( reinterpret_cast<char*>(data), 3*sizeof(float) );

    }

    return 0;
}



