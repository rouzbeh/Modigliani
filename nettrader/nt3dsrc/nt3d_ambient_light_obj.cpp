/**\file nt3d_ambient_light_obj.cpp - NT3D_ambient_light_o class implementation
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

/* $Id: nt3d_ambient_light_obj.cpp,v 1.1 2001/06/29 13:16:55 face Exp $
* $Log: nt3d_ambient_light_obj.cpp,v $
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.4  2000/05/11 12:42:08  face
* removed nt3d_rainbow_o by intergrating it into nt3d_light_o
*
* Revision 1.3  2000/05/11 11:32:44  face
* upgrade to NetTrader Version 0.5.
* Common state of all files checkin (Versioon 0.5)
* rearranged color and material classes...
* texture class added
*
* Revision 1.2  1999/07/05 00:50:37  face
* updated light color model
*
* Revision 1.1  1999/07/04 03:17:30  face
* minor mods
*

*/#include "nt3d_ambient_light_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_ambient_light_o */
NT3D_ambient_light_o::NT3D_ambient_light_o()
{
}


/* ***      COPY AND ASSIGNMENT	***/
NT3D_ambient_light_o::NT3D_ambient_light_o(const NT3D_ambient_light_o & original)
{
    cerr << "NT3D_ambient_light_o::NT3D_ambient_light_o - Talk : lightId copied too." << endl;
}

const NT3D_ambient_light_o&
NT3D_ambient_light_o::operator= (const NT3D_ambient_light_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    cerr << "NT3D_ambient_light_o::operator= - Talk : lightId copied too." << endl;
    // add assignment code here
    return *this;
}


/* ***      DESTRUCTOR		***/
NT3D_ambient_light_o::~NT3D_ambient_light_o()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
    \warning    unknown
    \bug        unknown
 */
void
NT3D_ambient_light_o::IlluminateLight()
{
    IlluminateAmbient();
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.5. */
