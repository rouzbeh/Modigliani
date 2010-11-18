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


/* $Id: resting_potential_noise.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
* $Log: resting_potential_noise.cpp,v $
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

NTreal AlphaN( NTreal voltage /* in mV */ ) {
    return (10.0 - voltage)/(100.0 * ( exp( 1.0 - voltage/10.0 ) - 1.0) );
}
NTreal BetaN ( NTreal voltage /* in mV */ ) {
    return 0.125 * exp(-voltage / 80.0);
}
NTreal AlphaM(NTreal vM /* in mV */) {
    return (25.0 - vM) / (10.0 * (exp((25.0 - vM)/10.0) - 1.0));
}
NTreal BetaM(NTreal vM /* in mV */) {
    return 4.0 * exp( -vM / 18.0 );
}
NTreal AlphaH(NTreal vM /* in mV */) {
    return 0.07 * exp( -vM / 20.0 );
}
NTreal BetaH(NTreal vM /* in mV */) {
    return 1.0/(exp(3.0 - vM/10.0) + 1.0);
}

int main(int argc, char* argv[])
{
    NTreal n,m,h;
    NTreal stddevN,stddevM,stddevH;

    NTreal numChan;
    NTreal v;

    cout << "v  \t" <<" numChan \t" << "n \t" << "stddevN \t" << "m \t stddevM \t" << "h \t stddevH" << endl;
    for ( numChan = 2; numChan < 5000; numChan += numChan) {
        for ( v = -20.01; v < 130.02; v += 1.0) {

            n = AlphaN(v)/(AlphaN(v)+BetaN(v));
            m = AlphaM(v)/(AlphaM(v)+BetaM(v));
            h = AlphaH(v)/(AlphaH(v)+BetaH(v));

            stddevN = sqrt(((1-n)*AlphaN(v) + n*BetaN(v))/numChan);
            stddevM = sqrt(((1-m)*AlphaM(v) + m*BetaM(v))/numChan);
            stddevH = sqrt(((1-h)*AlphaH(v) + h*BetaH(v))/numChan);

            if (numChan > 2500) {
                stddevN =0;
                stddevM =0;
                stddevH =0;
            }

            cout << v << "\t" << numChan <<"\t" << n << "\t" << stddevN << "\t" << m << "\t" << stddevM << "\t" << h << "\t" << stddevH << endl;
        }
        cout << endl;
    }

    return 0;
}

