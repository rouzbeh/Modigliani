/**\file nt_image_obj.cpp - NT_image_o class implementation
 * by Ahmed Aldo Faisal &copy; created 14.5.2000
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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


/* $Id: nt_image_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
* $Log: nt_image_obj.cpp,v $
* Revision 1.1  2001/06/11 13:18:56  face
* *** empty log message ***
*
* Revision 1.1  2000/05/18 10:38:32  face
* *** empty log message ***
*

*/
#include "nt_image_obj.h"

#include <cstdio>
#include <string>

/* ***      CONSTRUCTORS	***/
/** Create a NT_image_o */
NT_image_o::NT_image_o(NTsize newWidth,
                       NTsize newHeight,
                       NTsize newColorDepth,
                       NTimageDataFormat newDataFormat)
        :
        width(newWidth),
        height(newHeight),
        colorDepth(newColorDepth),
        dataFormat(newDataFormat)
{
    imageVec.resize(width*height*(int)dataFormat);
}

/* ***      COPY AND ASSIGNMENT	***/
NT_image_o::NT_image_o(const NT_image_o & original)
{
// add assignment code here
}

const NT_image_o&
NT_image_o::operator= (const NT_image_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
// add assignment code here
    return *this;
}

/* ***      DESTRUCTOR		***/
NT_image_o::~NT_image_o()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
NTreturn
NT_image_o::Load(string filename,  NTimageFileType fileType)
{
    if (NT_AUTO == fileType) fileType = Filename2Filetype (filename);
    if (fileType == NT_UNKNOWN) {
        cerr << "NT_image_o::Load - Error : In filename="<<filename
             <<" a wrong filetype format or unsupported filetype was specified." << endl;
        return NT_WRONG_FILE_TYPE;
    }

    switch  (fileType) {
    case NT_BMP:
        return LoadBMP(filename);
        break;
    case NT_PPM:
        return LoadPPM(filename);
        break;
    case NT_TGA:
        return LoadTGA(filename);
        break;
    default:
        cerr << "NT_image_o::Load - Error : In filename="<<filename
             <<"Unsupported filetype " << filename.substr(filename.length()-3, 3) << " specified." << endl;
        return NT_WRONG_FILE_TYPE;
    }

    cerr << "NT_image_o::Load - Error : Oops, program flow should not arrive here." <<endl;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/** @short     File type idendification is based on last four  characters in "filename"
    @param     filename with last four characters of kind ".???"

    @return     none
   \warning    unknown
   \bug        unknown
 */
NTimageFileType
NT_image_o::Filename2Filetype( string filename )
{
    NTsize length = filename.length();

    if (filename.substr(length-4, 1) != ".") {
        cerr << "NTimageFileType Filename2Filetype - Warning : filename="<<filename<<" does "<<
             "not match rule , that last four characters should be of format .??? (no match in '"
             <<filename.substr(length-4, 1)<<"'). Returned NT_UNKOWN "<<"instead." <<endl;
        return NT_UNKNOWN;
    }

    string fnSuffix = filename.substr(length-3, 3);

    if (fnSuffix == "bmp") return NT_BMP;
    if (fnSuffix == "BMP") return NT_BMP;
    if (fnSuffix == "tga") return NT_TGA;
    if (fnSuffix == "TGA") return NT_TGA;
    if (fnSuffix == "ppm") return NT_PPM;
    if (fnSuffix == "PPM") return NT_PPM;
    if (fnSuffix == "jpg") return NT_JPG;
    if (fnSuffix == "JPG") return NT_JPG;

    return NT_UNKNOWN;
}



/* Reads in RGBA data for a 32bit image. */
unsigned char *getTGARGBA (FILE *s, int size)
{
    unsigned char *rgba;
    unsigned char temp;
    int bread;
    int i;

    rgba = new unsigned char[size * 4];

    if (rgba == NULL)
        return 0;

    bread = fread (rgba, sizeof (unsigned char), size * 4, s);

    /* TGA is stored in BGRA, make it RGBA */
    if (bread != size * 4)
    {
        free (rgba);
        cerr << "NT_image_o::LoadTGA->getTGARGBA - Warning, read "<<bread<<" bytes instead of "<<size*4<<" picture data is missing or incomplete."<<endl;
        return 0;
    }

    for (i = 0; i < size * 4; i += 4 )
    {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }

    return rgba;
}

/* Reads in RGB data for a 24bit image.  */
unsigned char *getTGARGB (FILE *s, int size)
{
    unsigned char *rgb;
    unsigned char temp;
    int bread;
    int i;



    rgb = new unsigned char[size * 3];

    if (rgb == NULL)
        return 0;

    bread = fread (rgb, sizeof (unsigned char), size * 3, s);

    if (bread != size * 3)
    {
        free (rgb);
        cerr << "NT_image_o::LoadTGA->getTGARGB - Warning, read "<<bread<<" bytes instead of "<<size*3<<" picture data is missing or incomplete."<<endl;
        return 0;
    }

    /* TGA is stored in BGR, make it RGB */
    for (i = 0; i < size * 3; i += 3)
    {
        temp = rgb[i];
        rgb[i] = rgb[i + 2];
        rgb[i + 2] = temp;
    }



    return rgb;
}


/* Gets the grayscale image data.  */
unsigned char *getTGAGray (FILE *s, int size)
{
    unsigned char *grayData;
    int bread;

    grayData = new unsigned char [size];

    if (grayData == NULL)
        return 0;

    bread = fread (grayData, sizeof (unsigned char), size, s);

    if (bread != size)
    {
        free (grayData);
        cerr << "NT_image_o::LoadTGA->getTGAGray - Warning, read "<<bread<<" bytes instead of "<<size*3<<" picture data is missing or incomplete."<<endl;

        return 0;
    }

    return grayData;
}

/** Choose correct TGA file loader */
unsigned char *getTGAData (FILE *s, int sz, int mode)
{
    if (mode == 32)
        return getTGARGBA (s, sz);
    else if (mode == 24)
        return getTGARGB (s, sz);
    else if (mode == 8)
        return getTGAGray (s, sz);
    else {
        cerr << "NT_image_o::LoadTGA( filename )-getTGAData : Unsupported data mode="<<mode
             <<" . NULL image pointer returned."<<endl;
        return NULL;
    }

}

/** @short     Load a TGA file
    @param     filename with last four characters of kind ".???"

    @return     none
   \warning    unknown
   \bug        unknown
 */

NTreturn
NT_image_o::LoadTGA( string filename )
{
    unsigned char type[4];
    unsigned char info[7];
    unsigned char *imageData = NULL;
    int size; /* linear image size */
    int mode; /* is it an RGBA, RGB, GRAY ?*/
    FILE *s;

    if (!(s = fopen (filename.c_str(), "r+bt")))
        return NT_FILE_NOT_FOUND;

    fread (&type, sizeof (char), 3, s); // read in colormap info and image type, byte 0 ignored
    fseek (s, 12, SEEK_SET);			// seek past the header and useless info
    fread (&info, sizeof (char), 6, s);

    if (type[1] != 0 || (type[2] != 2 && type[2] != 3)) {
        cerr << "NT_image_o::LoadTGA("<<filename<<") - Error : Bad image type." << endl;
        return NT_BAD_FILE;
    }

    width = info[0] + info[1] * 256;
    height = info[2] + info[3] * 256;
    mode = info[4];



    switch (mode) {
    case 8 :
        dataFormat = NT_SINGLE_CHANNEL;
        break;
    case 24 :
        dataFormat = NT_RGB;
        break;
    case 32 :
        dataFormat = NT_RGBA;
        break;
    default:
        cerr <<"NT_image_o::LoadTGA( string filename ) - Error : Unknown data mode, neither SINGLE_CHANNEL, nor RGB, nor RGBA. Is this a valid tga file ?."<<endl;
        return NT_BAD_DATA;
    }

    size = width * height;

    cerr << s << "\t" << size << "\t" << mode << endl;
    imageData = getTGAData (s, size, mode);

    /* no image data */
    if (imageData == NULL) {
        cerr << "NT_image_o::LoadTGA - Error : Bad data in image file "<<filename<<"."<< endl;
        return NT_BAD_DATA;
    }

    imageVec.resize(width*height*(int)dataFormat);


    unsigned char tmp1;
    int tmp2;
    cerr << width*height*(int)dataFormat <<endl;
    for (NTsize ll = 0; ll < width*height*(int)dataFormat; ll++)
    {
        tmp1 = imageData[ll];
        tmp2 = (int) tmp1;
        imageVec[ll] = tmp2;

    }
    cerr <<"delete[] imageData"<<endl;
    delete[] imageData;
    cerr <<"deleted imageData"<<endl;

    return NT_SUCCESS;
}




/* File skeleton generated by GenNTObj version 0.7. */
