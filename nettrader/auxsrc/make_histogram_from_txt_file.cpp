/* make_histogram_from_text_file.cpp - compute the histogram */
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o print_interspike_intervals_in_binary_file print_interspike_intervals_in_binary_file.cpp
 */

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

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
main(int argc, char* argv[])
{
    bool selectColumns = false;

    if (argc < NUM_NON_COLUMN_PARAM) {
        cerr <<"First columns is time. Invalid command line." << argv[0] <<"  <number of columns> <binary float data file> <upstroke threshold> <down stroke threshold> [<column index to print 2...>] [<column index to print 3...>] [...] "<< endl;
        exit(1);
    }
    string filename = argv[1];
    int columns = atoi ( argv[2]);
    float upThreshold = atof ( argv[3]);
    float downThreshold = atof ( argv[4]);

    if (argc > NUM_NON_COLUMN_PARAM) selectColumns = true;
    vector <unsigned int> selectVec(argc-NUM_NON_COLUMN_PARAM);

    vector < vector < float > > isiListVec;

    vector < vector < float >  >::iterator itVec;
    vector < float >::iterator itList;
    unsigned	int ll;
    for (ll =0 ;ll < argc-NUM_NON_COLUMN_PARAM; ll++) {
        if (atoi(argv[ll+NUM_NON_COLUMN_PARAM]) == 1) {
            cerr << "You selected the first column to be checked for spikes, it should contain the time." << endl;
        }
        selectVec[ll] = atoi(argv[ll+NUM_NON_COLUMN_PARAM])-1;
    }

    for ( itVec =  isiListVec.begin(); itVec !=  isiListVec.end(); itVec++) {
        for ( itList =  itVec->begin(); itList !=  itVec->end(); itList++) {
            itVec->resize(0);
        }
    }

    float buffer[columns];
    float spikeTime[columns];
    bool spiking[columns];

    for (ll = 0; ll < columns; ll++) {
        buffer[ll] = 0;
        spikeTime[ll] = 0;
        spiking[ll] = false;
    }

    int rows = 0;

    vector < int > indexVec;

    if (true == selectColumns) {
        isiListVec.resize(argc-NUM_NON_COLUMN_PARAM);
    } else {
        isiListVec.resize(columns);
    }



    ifstream file (filename.c_str(), ios::binary);
    if (!file.good()) {
        cerr <<"Something is bad with reading from file "<<filename<<endl;
        exit(2);
    }

    while (file.eof() == 0) {
        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));
        if (true == selectColumns) {
            for (unsigned int ll =0 ;ll < argc-NUM_NON_COLUMN_PARAM; ll++) {
                if (1 ==  CheckForSpike(buffer[selectVec[ll]], upThreshold, downThreshold, &(spiking[selectVec[ll]]))) {
                    (isiListVec[ll]).push_back(buffer[0] - spikeTime[selectVec[ll]]);
                    spikeTime[selectVec[ll]] = buffer[0];
                }
            }
        } else {
            for (unsigned int ll = 1; ll < columns; ll++) {
                if (1 ==  CheckForSpike(buffer[ll], upThreshold, downThreshold, &(spiking[ll]))) {
                    (isiListVec[ll]).push_back(buffer[0] - spikeTime[ll]);
                    spikeTime[ll] = buffer[0];
                }
            }
        }
        cerr << "Analysing row " << rows << endl;
        rows++;
    }



    cerr << "Data output" << endl;


    for ( itVec =  isiListVec.begin(); itVec !=  isiListVec.end(); itVec++) {
        for ( itList =  itVec->begin(); itList !=  itVec->end(); itList++) {
            cout << *itList << endl;
        }
        cout << endl << endl;
    }




    cerr << "Completed and printed (float) "<<rows<<" rows  columns from file "<<filename<<" with "<<columns<<"."<<endl;

}



















