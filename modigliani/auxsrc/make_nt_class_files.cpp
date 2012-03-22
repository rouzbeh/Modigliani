/* make_nt_class_files.cpp - GenNTObj main code
 * Utility to generate an nt .h and .cpp file
 * by Ahmed A. Faisal, 22.4.1999 (c) */
/* $Id: make_nt_class_files.cpp,v 1.4 2003/06/23 10:06:46 face Exp $
*  $Log: make_nt_class_files.cpp,v $
*  Revision 1.4  2003/06/23 10:06:46  face
*  Upgraded to gcc-3.3 compilability
*
*  Revision 1.3  2003/06/20 13:25:53  face
*  *** empty log message ***
*
*  Revision 1.2  2003/01/17 16:53:05  face
*  *** empty log message ***
*
*  Revision 1.1  2001/06/29 13:14:44  face
*  adding vraious addtions
*  mainly in ntsrc (multidim_array)
*
*  Revision 1.6  2000/11/06 10:37:33  face
*  *** empty log message ***
*
*  Revision 1.5  2000/11/04 10:32:12  face
*  *** empty log message ***
*
*  Revision 1.4  2000/05/11 11:32:26  face
*  upgrade to NetTrader Version 0.5.
*  Common state of all files checkin (Versioon 0.5)
*
*  Revision 1.3  1999/07/20 01:14:33  face
*  added support for derived classes
*
*  Revision 1.2  1999/07/04 03:17:24  face
*  minor mods
*
* Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
* Revision 1.3  1999/06/06 23:52:06  face
 * *** empty log message ***
 *
* Revision 1.2  1999/05/09 09:34:15  face
 * corrected output
 *
* Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
*/

/* compile with:
 */

#include <streambuf>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define GENNTOBJ_MAJOR_VERSION 0
#define GENNTOBJ_MINOR_VERSION 7

bool derivedClass;
string parentClassNameStem = "PARENTCLASSNAMESTEM";
int versionMajor;
int versionMinor;
string filePrefix;
string fileSuffix = "_obj";
string filetype_extension = ".h";
string classnameStem = "test_test";
string namePrefix = "NT_";
string nameSuffix = "_o";
string creationDate = "3.7.1999";
string classname;
string filenameHeader;
string filenameSource;

/* #[ writeHeaderFileMain : */

void writeHeaderFileMain(ostream& fout) {
    fout << endl  << "/"<<"* $"<<"Id:"<<"$ "<< endl
    << "* $"<<"Log:" <<"$" << endl
    << endl <<
    "*"<<"/"<<endl<<
    "#ifndef _"<<filePrefix<<classnameStem<< fileSuffix << "_h_ "<< endl <<
    "#define _"<<filePrefix<<classnameStem<< fileSuffix << "_h_ "<< endl << endl <<
    "/"<<"* NT core includes *"<<"/" << endl <<
    "#include \"nt_main.h\" " <<endl <<
    "#include \"nt_types.h\" "<<endl <<
    "#include \"nt_obj.h\"  "<< endl;

    if (true == derivedClass) {
        fout << "/"<<"* Parent includes *"<<"/" << endl;
        fout << "#include \""<<filePrefix<<parentClassNameStem<<fileSuffix<<filetype_extension <<"\" "<<endl;
    }

    fout << "/"<<"* NT includes *"<<"/" << endl << endl;
    fout << "/"<<"* other includes *"<<"/" << endl << endl;



    fout << "/"<<"** @short "<<classname<<" class "<< endl <<

    "\\bug unknown" << endl <<
    "\\warning unknown " << endl <<
    "*"<<"/"<<endl;
    if (true == derivedClass)
        fout  <<"class "<<classname<<" : public "<< namePrefix << parentClassNameStem <<nameSuffix <<" {"<< endl ;
    else
        fout  <<"class "<<classname<<" : public NT_o {"<< endl;

    fout <<"public:"<< endl <<
    "/"<<"***   Constructors, Copy/Assignment and Destructor  ***"<<"/  " <<endl <<
    classname<<"();"<<endl<<
    classname<<"(const "<<classname<<" & original);" << endl <<
    "const "<<classname<<" & operator= (const "<<classname<<" & right);" << endl <<
    "virtual ~"<<classname<<"();" << endl <<
    "/"<<"* ***  Methods              ***"<<"/  " << endl <<
    "/"<<"* ***  Data                 ***"<<"/  " << endl <<
    "protected:" << endl <<
    "/"<<"* ***  Methods              ***"<<"/  " << endl <<
    "/"<<"* ***  Data                 ***"<<"/  " << endl <<
    "private:" << endl <<
    "/"<<"* ***  Methods              ***"<<"/  " << endl <<
    "/"<<"* ***  Data                 ***"<<"/ " << endl <<
    "};"   << endl;

}

/* #]  : */

/* #[ writeSourceFileMain : */

void writeSourceFileMain(ostream& fout) {

    fout << endl  << "/"<<"* $"<<"Id:"<<"$ "<< endl
    << "* $"<<"Log:" <<"$" << endl
    << endl <<
    "*"<<"/"<< endl <<
    "#" <<   "include \""<<filenameHeader<<"\" "<<endl<<endl
    <<"/"<<"* ***      CONSTRUCTORS	***"<<"/"<<endl
    <<"/"<<"** Create a "<<classname<<" *"<<"/"<<endl
    <<classname<<"::"<<classname<<"()";
    if (true == derivedClass) fout << "\n:\n"<<namePrefix<<parentClassNameStem<<nameSuffix<<"()"<< endl;
    fout << "{" << endl << "}" << endl <<

    "/"<<"* ***      COPY AND ASSIGNMENT	***"<<"/ "<< endl <<
    ""<<classname<<"::"<<classname<<"(const "<<classname<<" & original)"<< endl <<
    "{"<< endl <<
    " // add assignment code here"<< endl <<
    "}"<< endl <<
    endl <<
    "const "<<classname<<"&  "<< endl <<
    ""<<classname<<"::operator= (const "<<classname<<" & right)"<< endl <<
    "{"<< endl <<
    " if (this == &right) return *this; // Gracefully handle self assignment"<< endl <<
    " // add assignment code here"<< endl <<
    "  return *this;"<< endl <<
    "}"<< endl <<
    endl <<

    "/"<<"* ***      DESTRUCTOR		***"<<"/"<< endl <<
    ""<<classname<<"::~"<<classname<<"()"<< endl <<
    "{"<< endl <<
    "}"<< endl <<
    endl <<
    "/"<<"* ***  PUBLIC                                    ***   */  "<< endl <<
    "/"<<"** @short       "<< endl <<
    "    @param      none"<< endl <<
    "    @return     none"<< endl <<
    "   \\warning    unknown"<< endl <<
    "   \\bug        unknown"<< endl <<
    " */"<< endl <<
    "void"<< endl <<
    ""<<classname<<"::() const"<< endl <<
    "{"<< endl <<
    "}"<< endl <<
    endl <<
    "/"<<"* ***  PROTECTED                         ***   */"<< endl <<
    "/"<<"* ***  PRIVATE                           ***   */"<< endl <<
    "" << endl;
}

/* #]  : */

/* #[ writePreamble : */

void
writePreamble(ostream& fout, string nameOfPreambleFile, string filetypeName)
{
    fout << "/"<<"**"<<"\\"<<"file "<<nameOfPreambleFile<<" - "<<classname<<" class "<<filetypeName<<" \n * by Ahmed Aldo Faisal &copy; created "<<creationDate<<"  \n *"<<"/"<< endl;

    fout <<"/"<<"* NetTrader - visualisation, scientific and financial analysis and simulation system" << endl;
    fout <<" * Version:  "<<versionMajor<<"."<<versionMinor<<endl;
    fout <<" * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    "<< endl <<
    " *"<< endl <<
    " * This library is free software; you can redistribute it and/or"<< endl <<
    " * modify it under the terms of the GNU Library General Public"<< endl <<
    " * License as published by the Free Software Foundation; either"<< endl <<
    " * version 2 of the License, or (at your option) any later version."<< endl <<
    " *"<< endl <<
    " * This library is distributed in the hope that it will be useful,"<< endl <<
    " * but WITHOUT ANY WARRANTY; without even the implied warranty of"<< endl <<
    " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU"<< endl <<
    " * Library General Public License for more details."<< endl <<
    " *"<< endl <<
    " * You should have received a copy of the GNU Library General Public"<< endl <<
    " * License along with this library; if not, write to the Free"<< endl <<
    " * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."<< endl <<
    " */ " << endl <<
    "  "<< endl;
}
/* #]  : */

void writeHeaderFooter(ostream& fout)
{
    fout << "#endif /* _"<<filePrefix<<classnameStem << fileSuffix <<"_h_ */ "<< endl;

    fout << endl <<"/"<<"* File skeleton generated by GenNTObj version "<<GENNTOBJ_MAJOR_VERSION<<"."<<GENNTOBJ_MINOR_VERSION<<". */" <<endl;
}

void writeFooter(ostream& fout)
{
    fout << endl <<"/"<<"* File skeleton generated by GenNTObj version "<<GENNTOBJ_MAJOR_VERSION<<"."<<GENNTOBJ_MINOR_VERSION<<". */" <<endl;
}

fstream hfout;
fstream sfout;

/* #[ main : */

int
main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    cout << " GenNTObj - Utility to generate a .h and .cpp file for NetTrader" << endl;
    cout << " by Ahmed A. Faisal, 22.4.1999 (c)" << endl;
    cout << " Version "<<GENNTOBJ_MAJOR_VERSION<<"."<<GENNTOBJ_MINOR_VERSION<<" ."<<endl;
    cout << endl << "Please enter the class name prefix (e.g. NT or NT3D)" << endl;
    cin >> namePrefix;
    namePrefix += "_";
    {
        string answer = "n";
        do {
            cout << "Is this a new class (n,N) or a derived class (d,D) ?" ;
            cin >> answer;
        } while ((answer != "n")&&(answer != "N")&&(answer != "d")&&(answer != "D"));
        if ((answer == "d")||(answer == "D")) {
            derivedClass = true;
            cout << "Please enter the parent class name stem (e.g. parent NT3D_matrix_o -> matrix)" <<endl;
            cin >> parentClassNameStem;
        }
        else derivedClass = false;
    }
    cout << endl << "Please enter the stem of the full class name"<<endl
         <<"(e.g. flexible_matrix for a class with name NT3D_flexible_matrix_o" << endl;
    cin >> classnameStem;
    cout << endl << "Please enter the current major version of NetTrader when this class is added"<<endl;
    cin >> versionMajor;
    cout << endl << "Please enter the current minor version of NetTrader when this class is added"<<endl;
    cin >> versionMinor;
    cout << endl << "Please enter the creation date (e.g. 3.7.1999) "<<endl;
    cin >> creationDate;

    /* generate the file prefix */
    string::const_iterator nP = namePrefix.begin();
    while ( nP != namePrefix.end() ) {
        filePrefix += tolower (*nP);
        ++nP;
    }

    fileSuffix = "_obj";
    nameSuffix = "_o";
    classname = namePrefix + classnameStem + nameSuffix;
    filenameHeader = filePrefix + classnameStem + fileSuffix + ".h";
    filenameSource = filePrefix + classnameStem + fileSuffix + ".cpp";

    string filetype = "header";
    hfout.open(filenameHeader.c_str(), ios::out);

    writePreamble(hfout,filenameHeader, filetype);
    writeHeaderFileMain(hfout);
    writeHeaderFooter(hfout);
    hfout.close();

    filetype = "implementation";
    sfout.open(filenameSource.c_str(), ios::out);
    writePreamble(sfout,filenameSource, filetype);
    writeSourceFileMain(sfout);
    writeFooter(sfout);
    sfout.close();
    return 0;
}

/* #]  : */


