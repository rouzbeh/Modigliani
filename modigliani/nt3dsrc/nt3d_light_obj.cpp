/**\file nt3d_light_obj.cpp - NT3D_light_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 3.7.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.4
 * Copyright (C) 1998,199 Ahmed Aldo Faisal                            
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

/* $Id: nt3d_light_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: nt3d_light_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2003/06/23 10:06:51  face
 * Upgraded to gcc-3.3 compilability
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/05/18 10:38:25  face
 * an amazing error sits in nt3d_file_texture
 * if the constructor constructs a texture(1,1,1)
 * the system does not display the texture
 * if (64,64,2) is specified it does.
 * this is strange because the load routinge
 * should adapt the object correctly...investigate here 2DO
 *
 * Revision 1.9  2000/05/11 12:42:08  face
 * removed nt3d_rainbow_o by intergrating it into nt3d_light_o
 *
 * Revision 1.8  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.7  2000/04/28 16:02:40  face
 * found nasty hidden bug, by initialisation of an hideen ellipsoid making the
 * entire program crash
 * on destructor call... reason unknown
 *
 * Revision 1.6  2000/04/27 23:52:27  face
 * further corrections
 *
 * Revision 1.5  2000/04/26 20:53:02  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/07/17 18:22:01  face
 * imodified
 *
 * Revision 1.3  1999/07/16 15:05:10  face
 * added code::^
 *
 * Revision 1.2  1999/07/05 00:50:42  face
 * updated light color model
 *
 * Revision 1.1  1999/07/04 03:17:31  face
 * minor mods
 *

 */

#include "nt3d_light_obj.h" 

NT_id_management_o NT3D_light_o::idManager = NT_id_management_o(0,
		NT3D_MAX_LIGHTS - 1);

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_light_o */
NT3D_light_o::NT3D_light_o() {
	cerr << "NT3D_light_o::NT3D_light_o()" << std::endl;
	InitLights();
	lightOn = NT_TRUE;
	lightId = CreateLight();
}

/* ***      COPY AND ASSIGNMENT	***/
NT3D_light_o::NT3D_light_o(const NT3D_light_o & original) :
		NT_o() {
	lightOn = original.lightOn;
	lightId = CreateLight();
}

const NT3D_light_o&
NT3D_light_o::operator=(const NT3D_light_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	lightOn = right.lightOn;
	CreateLight();
	return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_light_o::~NT3D_light_o() {
	DeleteLight();
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
void NT3D_light_o::Illuminate() {
	if (NT_TRUE == lightOn) {
		glEnable(GL_LIGHTING);
		glEnable((GLenum) lightId);
		IlluminateLight();
	} else {
		glDisable((GLenum) lightId);
	}
}

void NT3D_light_o::On() {
	lightOn = NT_TRUE;
}

void NT3D_light_o::Off() {
	lightOn = NT_FALSE;
}

/* ***  PROTECTED                         ***   */

void NT3D_light_o::InitLights() {
	colorAmbient = NT3D_white() * 0.5 + NT3D_alpha();
	colorDiffuse = NT3D_white() * 0.5 + NT3D_alpha();
	colorSpecular = NT3D_white() * 0.5 + NT3D_alpha();
}

/* ***  PRIVATE                           ***   */
/** @short set up light
 @param      none
 @return     none
 \warning    only the first 8 lights of the standard  are implemented
 \bug        unknown
 */
NTid NT3D_light_o::CreateLight() {
	NTid number = idManager.IssueId();
	if (-1 == number)
		cerr
				<< "NT3D_light_o::NT3D_light_o() - warning : no new lightID issued. Possibly number of maximally "
				<< (int) NT3D_MAX_LIGHTS<<" lights exceeded." << std::endl;

	switch (number) {
	case 0:
		lightId = GL_LIGHT0;
		break;
	case 1:
		lightId = GL_LIGHT1;
		break;
	case 2:
		lightId = GL_LIGHT2;
		break;
	case 3:
		lightId = GL_LIGHT3;
		break;
	case 4:
		lightId = GL_LIGHT4;
		break;
	case 5:
		lightId = GL_LIGHT5;
		break;
	case 6:
		lightId = GL_LIGHT6;
		break;
	case 7:
		lightId = GL_LIGHT7;
		break;
	default:
		cerr
				<< "NT3D_light_o::CreateLight - error : issued light id out of range of implemented OpenGL lights \n";
		lightId = (NTid) -1;
		break;
	}
	return lightId;
}

/** @short  remove light
 sets lightId to -1
 @param      none
 @return     none
 \warning    only the first 8 lights of the standard  are implemented
 \bug        unknown
 */
void NT3D_light_o::DeleteLight() {
	switch (lightId) {
	case GL_LIGHT0:
		idManager.FreeId(0);
		break;
	case GL_LIGHT1:
		idManager.FreeId(1);
		break;
	case GL_LIGHT2:
		idManager.FreeId(2);
		break;
	case GL_LIGHT3:
		idManager.FreeId(3);
		break;
	case GL_LIGHT4:
		idManager.FreeId(4);
		break;
	case GL_LIGHT5:
		idManager.FreeId(5);
		break;
	case GL_LIGHT6:
		idManager.FreeId(6);
		break;
	case GL_LIGHT7:
		idManager.FreeId(7);
		break;
	}
	lightId = (NTid) -1;
}
/* File skeleton generated by GenNTObj version 0.5. */
