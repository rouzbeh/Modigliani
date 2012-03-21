/* find_spikewords_in_binary_file.cpp - output spike analysis  for binary data files. First column contains a time index.*/
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o find_spikes_in_binary_file find_spikes_in_binary_file.cpp
 */

#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "nt_ring_buffer_obj.h"
#define NUM_NON_COLUMN_PARAM 5 // including argv[0] ==  executable


NTreal ComputeEntropyElement(NTreal prob)
{

    if (0.0 == prob) return 0;
    else if (prob < 0.0) {
        cerr << "ComputeEntropyElement - Error : Negative probability encountered." << endl;
        exit(1);
    } else {
        return prob*log(prob)/0.69314718055994530942 /* log(2) */;
    }
}

NTsize BinaryWord2HashIndex(vector <bool> word)
{
    NTsize tmpVal = 0;
    NTsize base = 1;
    NTsize wordLength = word.size();

    for (NTsize ll=0;ll < wordLength; ll++) {
        tmpVal += word[wordLength-ll-1]*base;
        base *= 2;
    }

    return tmpVal;
}


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
    NTreal resolutionOfDataFile = 0.1; // in mSec
    NTsize analysisInterval = 20; // number of iterations to pool together, i.e 20*0.1mSec = 1mSec resolution of spike data

    if (argc < NUM_NON_COLUMN_PARAM) {
        cerr <<"First columns is time. Invalid command line." << argv[0] <<"  <binary float data file> <number of columns>  <upstroke threshold> <down stroke threshold>  <wordLength> <column index to analyse>" << endl;
        exit(1);
    }

    NTsize maxLines = 1/resolutionOfDataFile * 1000 * 10;
    cout << "Analysing the first "<< maxLines <<" lines only." << endl;
    string filename = argv[1];
    int columns = atoi ( argv[2]);
    float upThreshold = atof ( argv[3]);
    float downThreshold = atof ( argv[4]);
    NTsize wordLength = atoi ( argv[5] );
    NTsize select = atoi ( argv[6] );

    float buffer[columns];

    bool spike = false;
    bool spiking = false;

    NT_ring_buffer_o <bool> currentSpikeWord(wordLength);

    vector <NTsize> wordHistogram;
    wordHistogram.resize( ceil(pow(2.0,static_cast<int>(wordLength))) );

    vector <NTreal> wordProbHistogram;
    wordProbHistogram.resize( wordHistogram.size() );


    ifstream file (filename.c_str(), ios::binary);
    if (!file.good()) {
        cerr <<"Something is bad with reading from file "<<filename<<endl;
        exit(2);
    }

    NTsize counter = 0;
    NTsize lines = 0;
    while (file.eof() == 0) {

        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));
        counter++;
        lines++;
        if (analysisInterval == counter) {
            currentSpikeWord.Push(spike);
//		currentSpikeWord.Show();
            wordHistogram[BinaryWord2HashIndex(currentSpikeWord.Buffer())] += 1;
            counter = 0;
            spike = false;
        }
        if (1 == CheckForSpike(buffer[select], upThreshold, downThreshold, &spiking)) {
            spike = true;
        }

        if (lines > maxLines) break;
    }

    NTsize ll;
    for (ll = 0; ll < wordHistogram.size(); ll++) {
        wordProbHistogram[ll] = wordHistogram[ll]*analysisInterval/(NTreal(lines));
    }

    NTreal entropy = 0;
    NTreal p = 0;
    for (ll = 0; ll < wordHistogram.size(); ll++) {
//	cout << ll << "\t" << wordProbHistogram[ll] << endl;
        p += wordProbHistogram[ll];
        entropy -= ComputeEntropyElement(wordProbHistogram[ll]);
    }
    cout << endl;
    cout << "Input text file had resolution of " << 1000/resolutionOfDataFile << " Hz." << endl;
    cout << "Which was sampled for binary analysis in " <<resolutionOfDataFile*analysisInterval << " mSec long bins." << endl;
    cout << "Using spike up- and downstroke threshold of "<< upThreshold << " mV and "<< downThreshold <<" respectively mV."<< endl;
    cout << "For a word length of " << wordLength << " we find the following values (p summed to "<< p<<"):" << endl;
    cout << "Entropy in bits:" << entropy << endl;
    cout << "Entropy rate (bits/s):" << 1000 * /* mSec/Sec */ entropy/wordLength*resolutionOfDataFile*analysisInterval << endl;

    return 0;
}
















