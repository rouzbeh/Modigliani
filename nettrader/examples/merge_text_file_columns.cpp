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


/* $Id: merge_text_file_columns.cpp,v 1.1 2001/06/29 13:16:51 face Exp $
* $Log: merge_text_file_columns.cpp,v $
* Revision 1.1  2001/06/29 13:16:51  face
* *** empty log message ***
*
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <strstream>
#include <stdlib.h>
#include <vector>

#include "nt_error_obj.h"
#include "nt_vector_obj.h"

#include "nt_sequential_statistics_obj.h"

using namespace std;




int
main(int argc, char* argv[])
{
    if (argc != 3) {
        cerr <<argv[0]
             <<"  <single column text file 1>  <single column text file 2>"
             << endl;
        exit(1);
    }

    string filename1(argv[1]);
    string filename2(argv[2]);

    ifstream file1(filename1.c_str(), ios::in);
    ifstream file2(filename2.c_str(), ios::in);

    float a;
    float b;

    while ((file1.eof() == 0) || (file2.eof() == 0) ) {
        file1 >> a;
        file2 >> b;
        cout << a << "\t" << b << endl;
    }



}



















