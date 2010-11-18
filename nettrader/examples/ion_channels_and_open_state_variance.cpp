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


/* $Id: ion_channels_and_open_state_variance.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
 * $Log: ion_channels_and_open_state_variance.cpp,v $
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



using namespace TNT;

int main(void)
{
    /*
    NTreal timeStep = 1; // in msec
    NTint n = 100; // number of channels


    Matrix<NTreal> tpMtr(4,4);
    for (NTint ll=0; ll<4; ll++){
    	tpMtr[ll][ll] = 0.5;
    	tpMtr[0][1] = 0.5;
    	tpMtr[1][2] = 0.5;
    	tpMtr[2][3] = 0.5;
    	tpMtr[3][0] = 0.5;
    }

    NT_simple_markov_o channelModel(4);
    channelModel.SetProbTransMatrix(&tpMtr);

    for (NTint ll=0; ll<100; ll++){
    	channelModel.MarkovStep();
    	cout << channelModel._stateIndex() << endl;
    }
    */

    /* DEPRECATED NOTATION AND METHODS USED
    	for (NTint lx=1; lx< 100; lx++){

    		NTBP_sodium_ion_channels_o  sodiumChannels(lx * 100);
    		NTBP_potassium_ion_channels_o  potassiumChannels(lx * 100);
    		NT_sequential_statistics_o statsNa;
    		NT_sequential_statistics_o statsK;

    //new notation		sodiumChannels.SetTransitionProbabilities(0.1,0.1,0.05,0.05);
    //new notation		potassiumChannels.SetTransitionProbabilities( 0.1,0.1);



    		for (NTint ll=0; ll<10000; ll++){
    			sodiumChannels.Step();
    			potassiumChannels.Step();
    			statsNa.Add( sodiumChannels.NumOpen() );
    			statsK.Add( potassiumChannels.NumOpen() );
    		}

    		cout << lx * 100 << "\t";
    		cout << statsNa._variance() << "\t";
    		cout << statsK._variance() << endl;

    	}

    */
    return 0;
}



