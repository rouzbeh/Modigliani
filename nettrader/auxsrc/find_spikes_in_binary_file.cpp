/* find_spikes_in_binary_file.cpp - output spike analysis  for binary data files. First column contains a time index.*/
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o find_spikes_in_binary_file find_spikes_in_binary_file.cpp
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "nt_aux_math_func.h"

#define NUM_NON_COLUMN_PARAM 5 // including argv[0] ==  executable

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
NTAUXisnan (double value) {
    int status = (value != value);
    return status;
}


int
NTAUXisinf (const double x)
{
    double y = x - x;
    int s = (y != y);

    if (s && x > 0)
        return +1;
    else if (s && x < 0)
        return -1;
    else
        return 0;
}


int
main(int argc, char* argv[])
{
    bool selectColumns = false;

    if (argc < NUM_NON_COLUMN_PARAM) {
        cerr <<"Invalid command line." << argv[0] <<"  <binary float data file> <number of columns in data file> <upstroke threshold> <down stroke threshold> [<compartment index 1 to print ...>] [<compartment index 2 to print ...>] [...] "<< endl;
        cerr <<"NOTE: Usually the first two columns of every data file are time and current data." << endl;
        cerr <<"      and then the index ought to beg reater or equal 3, whith 3 designating the first voltage column" << endl;
        exit(1);
    }
    string filename = argv[1];
    unsigned int columns = atoi ( argv[2]);
    float upThreshold = atof ( argv[3]);
    float downThreshold = atof ( argv[4]);

    cerr <<"Processing file "<<filename<<" ..."<< endl;

    if (argc > NUM_NON_COLUMN_PARAM) selectColumns = true;
    vector <unsigned int> selectVec(argc-NUM_NON_COLUMN_PARAM);

    unsigned int ll;
    for (ll =0 ;ll < argc-NUM_NON_COLUMN_PARAM; ll++) {
        if (atoi(argv[ll+NUM_NON_COLUMN_PARAM]) == 1) cerr << "You selected the first column to be checked for spikes, it should contain the time and no voltage trace." << endl;
        if (atoi(argv[ll+NUM_NON_COLUMN_PARAM]) == 2) cerr << "You selected the second column to be checked for spikes, it may contain current data and no voltage trace." << endl;
        selectVec[ll] = atoi(argv[ll+NUM_NON_COLUMN_PARAM])-1;
    }

    float buffer[columns];
    unsigned int rows = 0;

    bool spiking[columns];

    for (ll = 0; ll < columns; ll++) {
        spiking[ll] = false;
    }

    ifstream file (filename.c_str(), ios::binary);
    if (!file.good()) {
        cerr <<"Something is bad with reading from file "<<filename<<endl;
        exit(2);
    }

    float v = 0;
    while (file.eof() == 0) {
        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));
        if (true == selectColumns) {
            // cout << buffer[0] << "\t";
            for (ll =0 ;ll < argc-NUM_NON_COLUMN_PARAM; ll++) {
                v = buffer[selectVec[ll]];
                if (NTAUXisnan(v)) cerr << "Warning - Column "<<selectVec[ll]<<" is NaN." << endl;
                else if (NTAUXisinf(v)) cerr <<"Warning - Column "<<selectVec[ll]<<" is inf." << endl;
                else cout << CheckForSpike(v, upThreshold, downThreshold, &(spiking[selectVec[ll]])) << "\t";
            }
        } else {
            // cout << buffer[0] << "\t";
            for (ll = 1; ll < columns; ll++) {
                v=buffer[ll];
                if (NTAUXisnan(v)) cerr << "Warning - Column "<<ll<<" is NaN." << endl;
                else if (NTAUXisinf(v)) cerr <<"Warning - Column "<<ll<<" is +/- inf." << endl;
                else cout << CheckForSpike(v, upThreshold, downThreshold,  &(spiking[ll])) << "\t";
            }
        }
        cout << endl;
        rows++;
    }



    cerr << "Completed and printed (float) "<<rows<<" rows with "<<columns<<" columns from file "<<filename<<"."<<endl;

}

















