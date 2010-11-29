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


/* $Id: analyse_binary_data_for_isi_with_histogram.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
* $Log: analyse_binary_data_for_isi_with_histogram.cpp,v $
* Revision 1.2  2003/06/20 13:25:55  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:14:44  face
* adding vraious addtions
* mainly in ntsrc (multidim_array)
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

#include "nt_error_obj.h"
#include "nt_vector_obj.h"
#include "nt_histogram_obj.h"

#include "nt_sequential_statistics_obj.h"
#include "tnt/tnt.h"
#include "tnt/cmat.h"

using namespace std;
using namespace TNT;


#define NUM_NON_COLUMN_PARAM 8 // including argv[0] ==  executable

int CheckForSpike(float v, float upThreshold, float downThreshold, bool* spiking)
{
    if (v > upThreshold) {
        if (*spiking == true) {
            *spiking = true;
            return 0;
        } else {
            *spiking = true;
            return 1;
        }
    } else if ( v < downThreshold ) {
        if (*spiking == true) {
            *spiking = false;
            return -1;
        } else {
            *spiking = false;
            return 0;
        }

    }
    return 0;
}



int
main(int argc, char* argv[])
{
    if (argc != 1+NUM_NON_COLUMN_PARAM) {
        cerr <<"First columns is time. Invalid command line." << argv[0]
             <<"  <binary float data file>  <number of columns> <upstroke threshold> <down stroke threshold> <numBins> <lower Range> <upper Range> <column index>"
             << endl;
        exit(1);
    }
    string filename = argv[1];
    int columns = atoi ( argv[2]);
    float upThreshold = atof ( argv[3]);
    float downThreshold = atof ( argv[4]);
    int numBins = atoi ( argv[5] );
    float low = atof ( argv[6] );
    float high = atof (argv[7] );
    int plotCol = atoi ( argv[8] );

    float buffer[columns];
    float spikeTime = 0;
    bool spiking = false;

    for (unsigned int ll = 0; ll < columns; ll++) {
        buffer[ll] = 0;
    }

    int rows = 0;

    NT_histogram_o hist(low, high, numBins);

    NTreal isi = 0;

    ifstream file (filename.c_str(), ios::binary);

    stringstream histSuffix;
    histSuffix << ".Column" << plotCol << ".histogram";
    string hist_filename = filename + histSuffix.str();
    cerr << "Writing histogram to " << hist_filename << "." << endl;
    ofstream histFile(hist_filename.c_str());

    stringstream isiSuffix;
    isiSuffix << ".Column" << plotCol << ".isi";
    string isi_filename = filename + isiSuffix.str();
    cerr << "Writing interspikeintervals to " << hist_filename << "." << endl;
    ofstream isiFile(isi_filename.c_str());


    if (!file.good()) {
        cerr <<"Something is bad with reading from file "<<filename<<endl;
        exit(2);
    }

    while (file.eof() == 0) {
        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));
        if (1 ==  CheckForSpike(buffer[plotCol], upThreshold, downThreshold, &spiking) ) {
            isi = buffer[0] - spikeTime;
            hist.BinValue(isi);
            isiFile << isi << endl;
            cerr << "Working on ISI number" << rows << endl;
            spikeTime = buffer[0];
            rows++;
        }
    }

    histFile << hist << endl;


    cerr << "Completed and printed (float) "<<rows<<" rows  columns from file "<<filename<<" with "<<columns<<"."<<endl;
}


