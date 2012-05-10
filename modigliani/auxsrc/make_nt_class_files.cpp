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
    fout << std::endl  << "/"<<"* $"<<"Id:"<<"$ "<< std::endl
    << "* $"<<"Log:" <<"$" << std::endl
    << std::endl <<
    "*"<<"/"<<std::endl<<
    "#ifndef _"<<filePrefix<<classnameStem<< fileSuffix << "_h_ "<< std::endl <<
    "#define _"<<filePrefix<<classnameStem<< fileSuffix << "_h_ "<< std::endl << std::endl <<
    "/"<<"* NT core includes *"<<"/" << std::endl <<
    "#include \"ntsrc/nt_main.h\" " <<std::endl <<
    "#include \"ntsrc/nt_types.h\" "<<std::endl <<
    "#include \"ntsrc/nt_obj.h\"  "<< std::endl;

    if (true == derivedClass) {
        fout << "/"<<"* Parent includes *"<<"/" << std::endl;
        fout << "#include \""<<filePrefix<<parentClassNameStem<<fileSuffix<<filetype_extension <<"\" "<<std::endl;
    }

    fout << "/"<<"* NT includes *"<<"/" << std::endl << std::endl;
    fout << "/"<<"* other includes *"<<"/" << std::endl << std::endl;



    fout << "/"<<"** @short "<<classname<<" class "<< std::endl <<

    "\\bug unknown" << std::endl <<
    "\\warning unknown " << std::endl <<
    "*"<<"/"<<std::endl;
    if (true == derivedClass)
        fout  <<"class "<<classname<<" : public "<< namePrefix << parentClassNameStem <<nameSuffix <<" {"<< std::endl ;
    else
        fout  <<"class "<<classname<<" : public NT_o {"<< std::endl;

    fout <<"public:"<< std::endl <<
    "/"<<"***   Constructors, Copy/Assignment and Destructor  ***"<<"/  " <<std::endl <<
    classname<<"();"<<std::endl<<
    classname<<"(const "<<classname<<" & original);" << std::endl <<
    "const "<<classname<<" & operator= (const "<<classname<<" & right);" << std::endl <<
    "virtual ~"<<classname<<"();" << std::endl <<
    "/"<<"* ***  Methods              ***"<<"/  " << std::endl <<
    "/"<<"* ***  Data                 ***"<<"/  " << std::endl <<
    "protected:" << std::endl <<
    "/"<<"* ***  Methods              ***"<<"/  " << std::endl <<
    "/"<<"* ***  Data                 ***"<<"/  " << std::endl <<
    "private:" << std::endl <<
    "/"<<"* ***  Methods              ***"<<"/  " << std::endl <<
    "/"<<"* ***  Data                 ***"<<"/ " << std::endl <<
    "};"   << std::endl;

}

/* #]  : */

/* #[ writeSourceFileMain : */

void writeSourceFileMain(ostream& fout) {

    fout << std::endl  << "/"<<"* $"<<"Id:"<<"$ "<< std::endl
    << "* $"<<"Log:" <<"$" << std::endl
    << std::endl <<
    "*"<<"/"<< std::endl <<
    "#" <<   "include \""<<filenameHeader<<"\" "<<std::endl<<std::endl
    <<"/"<<"* ***      CONSTRUCTORS	***"<<"/"<<std::endl
    <<"/"<<"** Create a "<<classname<<" *"<<"/"<<std::endl
    <<classname<<"::"<<classname<<"()";
    if (true == derivedClass) fout << "\n:\n"<<namePrefix<<parentClassNameStem<<nameSuffix<<"()"<< std::endl;
    fout << "{" << std::endl << "}" << std::endl <<

    "/"<<"* ***      COPY AND ASSIGNMENT	***"<<"/ "<< std::endl <<
    ""<<classname<<"::"<<classname<<"(const "<<classname<<" & original)"<< std::endl <<
    "{"<< std::endl <<
    " // add assignment code here"<< std::endl <<
    "}"<< std::endl <<
    std::endl <<
    "const "<<classname<<"&  "<< std::endl <<
    ""<<classname<<"::operator= (const "<<classname<<" & right)"<< std::endl <<
    "{"<< std::endl <<
    " if (this == &right) return *this; // Gracefully handle self assignment"<< std::endl <<
    " // add assignment code here"<< std::endl <<
    "  return *this;"<< std::endl <<
    "}"<< std::endl <<
    std::endl <<

    "/"<<"* ***      DESTRUCTOR		***"<<"/"<< std::endl <<
    ""<<classname<<"::~"<<classname<<"()"<< std::endl <<
    "{"<< std::endl <<
    "}"<< std::endl <<
    std::endl <<
    "/"<<"* ***  PUBLIC                                    ***   */  "<< std::endl <<
    "/"<<"** @short       "<< std::endl <<
    "    @param      none"<< std::endl <<
    "    @return     none"<< std::endl <<
    "   \\warning    unknown"<< std::endl <<
    "   \\bug        unknown"<< std::endl <<
    " */"<< std::endl <<
    "void"<< std::endl <<
    ""<<classname<<"::() const"<< std::endl <<
    "{"<< std::endl <<
    "}"<< std::endl <<
    std::endl <<
    "/"<<"* ***  PROTECTED                         ***   */"<< std::endl <<
    "/"<<"* ***  PRIVATE                           ***   */"<< std::endl <<
    "" << std::endl;
}

/* #]  : */

/* #[ writePreamble : */

void
writePreamble(ostream& fout, string nameOfPreambleFile, string filetypeName)
{
    fout << "/"<<"**"<<"\\"<<"file "<<nameOfPreambleFile<<" - "<<classname<<" class "<<filetypeName<<" \n * by Ahmed Aldo Faisal &copy; created "<<creationDate<<"  \n *"<<"/"<< std::endl;

    fout <<"/"<<"* NetTrader - visualisation, scientific and financial analysis and simulation system" << std::endl;
    fout <<" * Version:  "<<versionMajor<<"."<<versionMinor<<std::endl;
    fout <<" * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    "<< std::endl <<
    " *"<< std::endl <<
    " * This library is free software; you can redistribute it and/or"<< std::endl <<
    " * modify it under the terms of the GNU Library General Public"<< std::endl <<
    " * License as published by the Free Software Foundation; either"<< std::endl <<
    " * version 2 of the License, or (at your option) any later version."<< std::endl <<
    " *"<< std::endl <<
    " * This library is distributed in the hope that it will be useful,"<< std::endl <<
    " * but WITHOUT ANY WARRANTY; without even the implied warranty of"<< std::endl <<
    " * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU"<< std::endl <<
    " * Library General Public License for more details."<< std::endl <<
    " *"<< std::endl <<
    " * You should have received a copy of the GNU Library General Public"<< std::endl <<
    " * License along with this library; if not, write to the Free"<< std::endl <<
    " * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."<< std::endl <<
    " */ " << std::endl <<
    "  "<< std::endl;
}
/* #]  : */

void writeHeaderFooter(ostream& fout)
{
    fout << "#endif /* _"<<filePrefix<<classnameStem << fileSuffix <<"_h_ */ "<< std::endl;

    fout << std::endl <<"/"<<"* File skeleton generated by GenNTObj version "<<GENNTOBJ_MAJOR_VERSION<<"."<<GENNTOBJ_MINOR_VERSION<<". */" <<std::endl;
}

void writeFooter(ostream& fout)
{
    fout << std::endl <<"/"<<"* File skeleton generated by GenNTObj version "<<GENNTOBJ_MAJOR_VERSION<<"."<<GENNTOBJ_MINOR_VERSION<<". */" <<std::endl;
}

fstream hfout;
fstream sfout;

/* #[ main : */

int
main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    std::cout << " GenNTObj - Utility to generate a .h and .cpp file for NetTrader" << std::endl;
    std::cout << " by Ahmed A. Faisal, 22.4.1999 (c)" << std::endl;
    std::cout << " Version "<<GENNTOBJ_MAJOR_VERSION<<"."<<GENNTOBJ_MINOR_VERSION<<" ."<<std::endl;
    std::cout << std::endl << "Please enter the class name prefix (e.g. NT or NT3D)" << std::endl;
    cin >> namePrefix;
    namePrefix += "_";
    {
        string answer = "n";
        do {
            std::cout << "Is this a new class (n,N) or a derived class (d,D) ?" ;
            cin >> answer;
        } while ((answer != "n")&&(answer != "N")&&(answer != "d")&&(answer != "D"));
        if ((answer == "d")||(answer == "D")) {
            derivedClass = true;
            std::cout << "Please enter the parent class name stem (e.g. parent NT3D_matrix_o -> matrix)" <<std::endl;
            cin >> parentClassNameStem;
        }
        else derivedClass = false;
    }
    std::cout << std::endl << "Please enter the stem of the full class name"<<std::endl
         <<"(e.g. flexible_matrix for a class with name NT3D_flexible_matrix_o" << std::endl;
    cin >> classnameStem;
    std::cout << std::endl << "Please enter the current major version of NetTrader when this class is added"<<std::endl;
    cin >> versionMajor;
    std::cout << std::endl << "Please enter the current minor version of NetTrader when this class is added"<<std::endl;
    cin >> versionMinor;
    std::cout << std::endl << "Please enter the creation date (e.g. 3.7.1999) "<<std::endl;
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
    return (0);
}

/* #]  : */


