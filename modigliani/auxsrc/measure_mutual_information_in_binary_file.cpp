/**\file measure_mutual_information_in_binary_file.cpp - computes the joint probability density function of two columns
 in binary (float) data files.
 * by Ahmed Aldo Faisal &copy; created 10.10.2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
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

/* $Id: measure_mutual_information_in_binary_file.cpp,v 1.3 2003/06/20 13:25:53 face Exp $
 * $Log: measure_mutual_information_in_binary_file.cpp,v $
 * Revision 1.3  2003/06/20 13:25:53  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/15 17:05:22  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/12 09:18:32  face
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
#include <vector>

#include "nt_error_obj.h"
#include "nt_vector_obj.h"
#include "nt_histogram_obj.h"
#include "nt_2dim_histogram_obj.h"

#include "nt_sequential_statistics_obj.h"
#include "tnt/tnt.h"
#include "tnt/cmat.h"

int main(int argc, char* argv[]) {
	if (argc != 12) {
		cerr
				<< "First columns is time. Invalid command line."
				<< argv[0]
				<< " <1 binary float data file> <2 number of columns in file> <3 numBins for A> "
				<< " <4 column A> <5 low A> <6 high A> <7 numBins for B> <8 column B> <9 low B> "
				<< " <10 high B> <11 numRows to analyse in data file> " << endl;

		exit(1);
	}
	string filename = argv[1];
	int columns = atoi(argv[2]);
	int numBinsA = atoi(argv[3]);
	int columnA = atoi(argv[4]);
	float lowA = atof(argv[5]);
	float highA = atof(argv[6]);
	int numBinsB = atoi(argv[7]);
	int columnB = atoi(argv[8]);
	float lowB = atof(argv[9]);
	float highB = atof(argv[10]);
	int numRows = atoi(argv[11]);

	float buffer[columns];

	ifstream file(filename.c_str(), ios::binary);
	if (!file.good()) {
		cerr << "Something is bad with reading from specified file " << filename
				<< endl;
		exit(2);
	}

	NT_histogram_o histA(lowA, highA, numBinsA);
	NT_histogram_o histB(lowB, highB, numBinsB);
	NT_2dim_histogram_o histJoint(numBinsA, lowA, highA, numBinsB, lowB, highB);

	int rows = 0;
	while (file.eof() == 0 && rows <= numRows) {
		file.read(reinterpret_cast<char*>(buffer), columns * sizeof(float));
		//    cerr <<  buffer[columnA-1] << "\t" << buffer[columnB-1] << endl;
		histA.BinValue(buffer[columnA - 1]);
		histB.BinValue(buffer[columnB - 1]);
		histJoint.BinValue(buffer[columnA - 1], buffer[columnB - 1]);
		rows++;
	}

	string dummy1 = "HistColumnA+B";
	string dummy2 = "HistColumnA";
	string dummy3 = "HistColumnB";

	//  cout << "Entropy of column A " << histA.ShannonEntropy() << endl;
	//  cout << "Entropy of column B " << histB.ShannonEntropy() << endl;
	//  cout << "Joint entropy AB" << histJoint.ShannonEntropy() << endl;

	histJoint.Show();
	cout << endl;
	histA.Show();
	cout << endl;
	histB.Show();
	cerr << "Completed and printed (float) " << rows
			<< " rows  columns from file " << filename << " with " << columns
			<< "." << endl;

	return 0;
}

