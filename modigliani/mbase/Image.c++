/**
 * @file Image.c++
 * Image class implementation
 * @author Ahmed Aldo Faisal &copy; created 14.5.2000
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Image.h"

using namespace mbase;

/* ***      CONSTRUCTORS	***/
/** Create a Image */
Image::Image(Size_t newWidth, Size_t newHeight, Size_t newColorDepth,
		ImageDataFormat newDataFormat) :
		width(newWidth), height(newHeight), colorDepth(newColorDepth), dataFormat(
				newDataFormat) {
	imageVec.resize(width * height * (int) dataFormat);
}

/* ***      COPY AND ASSIGNMEM	***/
Image::Image(const Image & original) :
		Obj(), width(original.width), height(original.height), colorDepth(
				original.colorDepth), dataFormat(original.dataFormat) {
	this->imageVec.resize(width * height * (int) dataFormat);
}

const Image&
Image::operator=(const Image & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Image::~Image() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
Mreturn Image::Load(std::string filename, ImageFileType fileType) {
	if (AUTO == fileType)
		fileType = Filename2Filetype(filename);
	if (fileType == UNKNOWN) {
		std::cerr
				<< "Image::Load - Error : In filename="
				<< filename
				<< " a wrong filetype format or unsupported filetype was specified."
				<< std::endl;
		return (M_WRONG_FILE_TYPE);
	}

	switch (fileType) {
	case BMP:
		return (LoadBMP(filename));
		break;
	case PPM:
		return (LoadPPM(filename));
		break;
	case TGA:
		return (LoadTGA(filename));
		break;
	default:
		std::cerr << "Image::Load - Error : In filename=" << filename
				<< "Unsupported filetype "
				<< filename.substr(filename.length() - 3, 3) << " specified."
				<< std::endl;
		return (M_WRONG_FILE_TYPE);
	}

	std::cerr
			<< "Image::Load - Error : Oops, program flow should not arrive here."
			<< std::endl;
	return (M_FAIL);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/** @short     File type idendification is based on last four  characters in "filename"
 @param     filename with last four characters of kind ".???"

 @return     none
 \warning    unknown
 \bug        unknown
 */
mbase::ImageFileType Image::Filename2Filetype(std::string filename) {
	Size_t length = filename.length();

	if (filename.substr(length - 4, 1) != ".") {
		std::cerr
				<< "MimageFileType Filename2Filetype - Warning : filename="
				<< filename
				<< " does "
				<< "not match rule , that last four characters should be of format .??? (no match in '"
				<< filename.substr(length - 4, 1) << "'). Returned M_UNKOWN "
				<< "instead." << std::endl;
		return (UNKNOWN);
	}

	std::string fnSuffix = filename.substr(length - 3, 3);

	if (fnSuffix == "bmp")
		return (BMP);
	if (fnSuffix == "BMP")
		return (BMP);
	if (fnSuffix == "tga")
		return (TGA);
	if (fnSuffix == "TGA")
		return (TGA);
	if (fnSuffix == "ppm")
		return (PPM);
	if (fnSuffix == "PPM")
		return (PPM);
	if (fnSuffix == "jpg")
		return (JPG);
	if (fnSuffix == "JPG")
		return (JPG);

	return (UNKNOWN);
}

/* Reads in RGBA data for a 32bit image. */
unsigned char *getTGARGBA(FILE *s, int size) {
	unsigned char *rgba;
	unsigned char temp;
	int bread;
	int i;

	rgba = new unsigned char[size * 4];

	if (rgba == NULL)
		return (0);

	bread = fread(rgba, sizeof(unsigned char), size * 4, s);

	/* TGA is stored in BGRA, make it RGBA */
	if (bread != size * 4) {
		delete[] rgba;
		std::cerr << "Image::LoadTGA->getTGARGBA - Warning, read " << bread
				<< " bytes instead of " << size * 4
				<< " picture data is missing or incomplete." << std::endl;
		return (0);
	}

	for (i = 0; i < size * 4; i += 4) {
		temp = rgba[i];
		rgba[i] = rgba[i + 2];
		rgba[i + 2] = temp;
	}

	return (rgba);
}

/* Reads in RGB data for a 24bit image.  */
unsigned char *getTGARGB(FILE *s, int size) {
	unsigned char *rgb;
	unsigned char temp;
	int bread;
	int i;

	rgb = new unsigned char[size * 3];

	if (rgb == NULL)
		return (0);

	bread = fread(rgb, sizeof(unsigned char), size * 3, s);

	if (bread != size * 3) {
		delete[] rgb;
		std::cerr << "Image::LoadTGA->getTGARGB - Warning, read " << bread
				<< " bytes instead of " << size * 3
				<< " picture data is missing or incomplete." << std::endl;
		return (0);
	}

	/* TGA is stored in BGR, make it RGB */
	for (i = 0; i < size * 3; i += 3) {
		temp = rgb[i];
		rgb[i] = rgb[i + 2];
		rgb[i + 2] = temp;
	}

	return (rgb);
}

/* Gets the grayscale image data.  */
unsigned char *getTGAGray(FILE *s, int size) {
	unsigned char *grayData;
	int bread;

	grayData = new unsigned char[size];

	if (grayData == NULL)
		return (0);

	bread = fread(grayData, sizeof(unsigned char), size, s);

	if (bread != size) {
		delete[] grayData;
		std::cerr << "Image::LoadTGA->getTGAGray - Warning, read " << bread
				<< " bytes instead of " << size * 3
				<< " picture data is missing or incomplete." << std::endl;

		return (0);
	}

	return (grayData);
}

/** Choose correct TGA file loader */
unsigned char *getTGAData(FILE *s, int sz, int mode) {
	if (mode == 32)
		return (getTGARGBA(s, sz));
	else if (mode == 24)
		return (getTGARGB(s, sz));
	else if (mode == 8)
		return (getTGAGray(s, sz));
	else {
		std::cerr
				<< "Image::LoadTGA( filename )-getTGAData : Unsupported data mode="
				<< mode << " . NULL image pointer returned." << std::endl;
		return (NULL);
	}

}

/** @short     Load a TGA file
 @param     filename with last four characters of kind ".???"

 @return     none
 \warning    unknown
 \bug        unknown
 */

Mreturn Image::LoadTGA(std::string filename) {
	unsigned char type[4];
	unsigned char info[7];
	unsigned char *imageData = NULL;
	int size; /* linear image size */
	int mode; /* is it an RGBA, RGB, GRAY ?*/
	FILE *s;

	if (!(s = fopen(filename.c_str(), "r+bt")))
		return (M_FILE_NOT_FOUND);

	fread(&type, sizeof(char), 3, s); // read in colormap info and image type, byte 0 ignored
	fseek(s, 12, SEEK_SET); // seek past the header and useless info
	fread(&info, sizeof(char), 6, s);

	if (type[1] != 0 || (type[2] != 2 && type[2] != 3)) {
		std::cerr << "Image::LoadTGA(" << filename
				<< ") - Error : Bad image type." << std::endl;
		fclose(s);
		return (M_BAD_FILE);
	}

	width = info[0] + info[1] * 256;
	height = info[2] + info[3] * 256;
	mode = info[4];

	switch (mode) {
	case 8:
		dataFormat = SINGLE_CHANNEL;
		break;
	case 24:
		dataFormat = RGB;
		break;
	case 32:
		dataFormat = RGBA;
		break;
	default:
		std::cerr
				<< "Image::LoadTGA( std::string filename ) - Error : Unknown data mode, neither SINGLE_CHANNEL, nor RGB, nor RGBA. Is this a valid tga file ?."
				<< std::endl;
		fclose(s);
		return (M_BAD_DATA);
	}

	size = width * height;

	std::cerr << s << "\t" << size << "\t" << mode << std::endl;
	imageData = getTGAData(s, size, mode);

	/* no image data */
	if (imageData == NULL) {
		std::cerr << "Image::LoadTGA - Error : Bad data in image file "
				<< filename << "." << std::endl;
		return (M_BAD_DATA);
	}

	imageVec.resize(width * height * (int) dataFormat);

	unsigned char tmp1;
	int tmp2;
	std::cerr << width * height * (int) dataFormat << std::endl;
	for (Size_t ll = 0; ll < width * height * (int) dataFormat; ll++) {
		tmp1 = imageData[ll];
		tmp2 = (int) tmp1;
		imageVec[ll] = tmp2;

	}
	std::cerr << "delete[] imageData" << std::endl;
	delete[] imageData;
	std::cerr << "deleted imageData" << std::endl;
	fclose(s);
	return (M_SUCCESS);
}

/* File skeleton generated by GenMObj version 0.7. */
