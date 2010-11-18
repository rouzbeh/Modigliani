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


/* $Id: multi_compartment_neuron.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
 * $Log: multi_compartment_neuron.cpp,v $
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

#include "nt_simple_markov_obj.h"

#include "ntbp_sodium_ion_channels_obj.h"
#include "ntbp_potassium_ion_channels_obj.h"

#include "nt_sequential_statistics_obj.h"

#include "tnt/tnt.h"
#include "tnt/cmat.h"

#include "ntbp_membrane_compartment_sequence_obj.h"
#include "ntbp_sga_cylindrical_compartment_obj.h"

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

    NTreal timeInMS = 0;

    NTBP_membrane_compartment_sequence_o oModel;

    for (NTsize ll=0; ll < 10; ll++) {
        NTBP_sga_cylindrical_compartment_o * tmpPtr = new NTBP_sga_cylindrical_compartment_o(100 /* muMeter */);
        oModel.PushBack(tmpPtr);
    }

    oModel.Init();
    oModel.InitialStep();

    for (NTint lt=1; lt< 1000.0/NTBP_TIMESTEP; lt++) {
        timeInMS = lt*NTBP_TIMESTEP;
        oModel.Step();
        oModel.ShowVoltage();
    }

    return 0;
}



