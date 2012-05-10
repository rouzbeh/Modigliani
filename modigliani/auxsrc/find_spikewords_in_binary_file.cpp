/* find_spikewords_in_binary_file.cpp - output spike analysis  for binary data files. First column contains a time index.*/
/* */
/* by Ahmed Aldo Faisal, 1.6.2001 (c) */
/* compile with: g++ -o find_spikes_in_binary_file find_spikes_in_binary_file.cpp
 */

#include "mbase/Obj.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "mbase/Ring_buffer.h"
#define NUM_NON_COLUMN_PARAM 5 // including argv[0] ==  executable


mbase::Mreal compute_entropy_element(mbase::Mreal prob)
{

    if (0.0 == prob) return (0);
    else if (prob < 0.0) {
        std::cerr << "ComputeEntropyElement - Error : Negative probability encountered." << std::endl;
        exit(1);
    } else {
        return (prob*log(prob)/0.69314718055994530942 /* log(2) */);
    }
}

mbase::Msize binary_Word2HashIndex(std::vector <bool> word)
{
    mbase::Msize tmpVal = 0;
    mbase::Msize base = 1;
    mbase::Msize wordLength = word.size();

    for (mbase::Msize ll=0;ll < wordLength; ll++) {
        tmpVal += word[wordLength-ll-1]*base;
        base *= 2;
    }

    return (tmpVal);
}


int check_for_spike(float v, float upThreshold, float downThreshold, bool* spiking)
{
    if (v > upThreshold) {
        if (*spiking == true) {
            *spiking = true;
            return (0);
        } else {
            *spiking = true;
            return (1);
        }
    } else if ( v < downThreshold ) {
        if (*spiking == true) {
            *spiking = false;
            return (-1);
        } else {
            *spiking = false;
            return (0);
        }

    }
    return (0);
}



int
main(int argc, char* argv[])
{
    mbase::Mreal resolutionOfDataFile = 0.1; // in mSec
    mbase::Msize analysisInterval = 20; // number of iterations to pool together, i.e 20*0.1mSec = 1mSec resolution of spike data

    if (argc < NUM_NON_COLUMN_PARAM) {
        std::cerr <<"First columns is time. Invalid command line." << argv[0] <<"  <binary float data file> <number of columns>  <upstroke threshold> <down stroke threshold>  <wordLength> <column index to analyse>" << std::endl;
        exit(1);
    }

    mbase::Msize maxLines = 1/resolutionOfDataFile * 1000 * 10;
    std::cout << "Analysing the first "<< maxLines <<" lines only." << std::endl;
    std::string filename = argv[1];
    int columns = atoi ( argv[2]);
    float upThreshold = atof ( argv[3]);
    float downThreshold = atof ( argv[4]);
    mbase::Msize wordLength = atoi ( argv[5] );
    mbase::Msize select = atoi ( argv[6] );

    float buffer[columns];

    bool spike = false;
    bool spiking = false;

    mbase::Ring_buffer <bool> currentSpikeWord(wordLength);

    std::vector <mbase::Msize> wordHistogram;
    wordHistogram.resize( ceil(pow(2.0,static_cast<int>(wordLength))) );

    std::vector <mbase::Mreal> wordProbHistogram;
    wordProbHistogram.resize( wordHistogram.size() );


    std::ifstream file (filename.c_str(), std::ios::binary);
    if (!file.good()) {
        std::cerr <<"Something is bad with reading from file "<<filename<<std::endl;
        exit(2);
    }

    mbase::Msize counter = 0;
    mbase::Msize lines = 0;
    while (file.eof() == 0) {

        file.read(reinterpret_cast<char*>(buffer), columns*sizeof(float));
        counter++;
        lines++;
        if (analysisInterval == counter) {
            currentSpikeWord.Push(spike);
//		currentSpikeWord.Show();
            wordHistogram[binary_Word2HashIndex(currentSpikeWord.Buffer())] += 1;
            counter = 0;
            spike = false;
        }
        if (1 == check_for_spike(buffer[select], upThreshold, downThreshold, &spiking)) {
            spike = true;
        }

        if (lines > maxLines) break;
    }

    mbase::Msize ll;
    for (ll = 0; ll < wordHistogram.size(); ll++) {
        wordProbHistogram[ll] = wordHistogram[ll]*analysisInterval/(mbase::Mreal(lines));
    }

    mbase::Mreal entropy = 0;
    mbase::Mreal p = 0;
    for (ll = 0; ll < wordHistogram.size(); ll++) {
//	std::cout << ll << "\t" << wordProbHistogram[ll] << std::endl;
        p += wordProbHistogram[ll];
        entropy -= compute_entropy_element(wordProbHistogram[ll]);
    }
    std::cout << std::endl;
    std::cout << "Input text file had resolution of " << 1000/resolutionOfDataFile << " Hz." << std::endl;
    std::cout << "Which was sampled for binary analysis in " <<resolutionOfDataFile*analysisInterval << " mSec long bins." << std::endl;
    std::cout << "Using spike up- and downstroke threshold of "<< upThreshold << " mV and "<< downThreshold <<" respectively mV."<< std::endl;
    std::cout << "For a word length of " << wordLength << " we find the following values (p summed to "<< p<<"):" << std::endl;
    std::cout << "Entropy in bits:" << entropy << std::endl;
    std::cout << "Entropy rate (bits/s):" << 1000 * /* mSec/Sec */ entropy/wordLength*resolutionOfDataFile*analysisInterval << std::endl;

    return (0);
}
















