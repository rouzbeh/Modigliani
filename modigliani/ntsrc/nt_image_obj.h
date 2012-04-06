/**\file nt_image_obj.h - NT_image_o class header
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

/* $Id: nt_image_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_image_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/10/03 08:30:03  face
 * *** empty log message ***
 *
 * Revision 1.1  2000/05/18 10:38:32  face
 * *** empty log message ***
 *

 */

#ifndef _nt_image_obj_h_
#define _nt_image_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* NT includes */
#include "nt_codes.h"

/* other includes */
#include <valarray>
#include <string>

/** @short NT_image_o class
 \bug unknown
 \warning unknown
 */
class NT_image_o: public NT_o {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NT_image_o(NTsize width = 128, NTsize height = 64, NTsize colorDepth = 16,
			NTimageDataFormat dataFormat = NT_RGBA);
	NT_image_o(const NT_image_o & original);
	const NT_image_o & operator=(const NT_image_o & right);
	virtual ~NT_image_o();
	/* ***  Methods              ***/
	NTsize _width() {
		return width;
	}
	NTsize _height() {
		return height;
	}
	NTsize _colorDepth() {
		return 24;
	}
	NTimageDataFormat _dataFormat() {
		return dataFormat;
	}

	NTreturn Load(string filename, NTimageFileType fileType = NT_AUTO);
	NTreturn Save(__attribute__ ((unused)) string filename,
			__attribute__ ((unused)) NTimageFileType fileType = NT_AUTO) const {
		return NT_NOT_IMPLEMENTED;
	}
	;
	const valarray<NTsize>* GetImageArray() const {
		return &imageVec;
	}
//valarray<double>* GetRGBAImageArray() const;
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	NTreal _imageVecR(NTsize elementIndex) const {
		return imageVec[elementIndex] / 255.0;
	}
	NTsize _imageVecI(NTsize elementIndex) const {
		return imageVec[elementIndex];
	}
	/* ***  Data                 ***/
	NTsize width;
	NTsize height;
	NTsize colorDepth; /*  in bits per pixel without alpha channel {1,2,4,8,15,16,24,32}; */
	NTimageDataFormat dataFormat;
private:
	/* ***  Methods              ***/
	NTimageFileType Filename2Filetype(string filename);
	NTreturn LoadBMP(__attribute__ ((unused)) string filename) {
		cerr << "Not implemented." << endl;
		return NT_NOT_IMPLEMENTED;
	}
	NTreturn LoadPPM(__attribute__ ((unused)) string filename) {
		cerr << "Not implemented." << endl;
		return NT_NOT_IMPLEMENTED;
	}
	NTreturn LoadTGA(string filename);
	/* ***  Data                 ***/
	valarray<NTsize> imageVec;
};

#endif /* _nt_image_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */