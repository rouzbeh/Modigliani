/** \file nt3d_solid_obj.cpp - NT3D solid class implementation
 * by Ahmed Aldo Faisal &copy; created 15.10.1998
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:   0.2
 * Copyright (C) 1998 Ahmed Aldo Faisal
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


/* $Id: nt3d_solid_obj.cpp,v 1.1 2001/06/29 13:16:55 face Exp $
 * $Log: nt3d_solid_obj.cpp,v $
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.13  2000/07/06 08:06:10  face
 * *** empty log message ***
 *
 * Revision 1.12  2000/06/26 19:14:06  face
 * finally found the error that caused solid object to disappear:
 * the scaling vector was not constructed in the 2nd constructor
 * of the solid bject thus resulting in a zero object
 * new demo file: showing some random graphics
 *
 * Revision 1.11  2000/06/13 20:10:26  face
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
 * Revision 1.5  2000/04/27 18:20:48  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.4  2000/04/26 22:57:45  face
 * added new classes with rainbow_texture as demo class
 *
 * Revision 1.3  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.2  1999/07/04 10:57:11  face
 * modified from rainbow to color object
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.5  1999/01/19 09:23:50  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/11 02:26:40  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/07 23:53:04  face
 * Initial revision
 *
 * Revision 1.1  1998/12/25 21:27:13  face
 * Initial revision
 *
 *
 */

#include "nt3d_solid_obj.h"

#include "nt3d_rainbow_texture_obj.h"
#include "nt3d_worldmap_texture_obj.h"
#include "nt3d_file_texture_obj.h"



/* 	CONSTRUCTORS 		*/
NT3D_solid_o::NT3D_solid_o(const NT_vector3_o & oNewPosition)
        :
        NT3D_surfaced_object_o(oNewPosition),
        oScaling(1.0, 1.0, 1.0)
{
    NT_CERR(9,"NT3D_solid_o::NT3D_solid_o()");
    InitQuadric();
}




/* 	COPY AND ASSIGNMENT 	*/
/** The copy constructor does NOT actually copy the quadric object,
but instead a new quadric object is initialized.  All necessary
information should be stored inside the ellipsoid object such that the
state of that object can be reconstructed by copying the corresponding
object data.
Note: The texturePointer is not copied BUT instead a new is generated based on the original.*/
NT3D_solid_o::NT3D_solid_o(const NT3D_solid_o & original)
{
    pQuadObj = gluNewQuadric();
    drawStyle = original.drawStyle;
    slices = original.slices;
    stacks = original.stacks;
}

/** The copy constructor does NOT actually copy the quadric object,
but instead a new quadric object is initialized.  All necessary
information should be stored inside the ellipsoid object such that the
state of that object can be reconstructed by copying the corresponding
object data.
Note: The texturePointer is not copied BUT instead a new is generated based on the original. */
const NT3D_solid_o&
NT3D_solid_o::operator= (const NT3D_solid_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    //free(pQuadObj);
    pQuadObj = gluNewQuadric();
    drawStyle = right.drawStyle;
    slices = right.slices;
    stacks = right.stacks;

    return *this;
}


/* 	DESTRUCTOR 		*/
/** Everything (also the oTexturePtr) is cleaned up */
NT3D_solid_o::~NT3D_solid_o()
{
    // cerr <<"NT3D_solid_o::~NT3D_solid_o() - Warning : gluQuadric not deleted." << endl;
    gluDeleteQuadric(pQuadObj);
}


/*   	PUBLIC                	*/

void
NT3D_solid_o::Scale(  NT_vector3_o oDeltaScaling )
{
    oScaling = oScaling + oDeltaScaling;
}

void
NT3D_solid_o::ScaleX ( NTreal deltaSclX)
{
    oScaling[0] += deltaSclX;
}

void
NT3D_solid_o::ScaleY ( NTreal deltaSclY)
{
    oScaling[1] += deltaSclY;
}

void
NT3D_solid_o::ScaleZ ( NTreal deltaSclZ)
{
    oScaling[2] += deltaSclZ;
}

void
NT3D_solid_o::ScaleXYZ ( NTreal deltaSclXYZ)
{
    oScaling = oScaling * deltaSclXYZ;
}




/*   	PROTECTED     	      	*/


/* 	PRIVATE               	*/
/** @short 	common init for all constructors
    @param 	none
    @return 	none
    \warning 	unknown
    \bug 	unknown
 */
void
NT3D_solid_o::InitQuadric()
{
    slices = 16;
    stacks = 8;
    pQuadObj = gluNewQuadric();
    drawStyle = GLU_FILL;
}

/** @short 	Capsuled draw method to include color and material drawing
	Do not override in derived classes,
	Use DrawSolidObj instead!

    @param 	none
    @return 	none
    \warning 	unknown
    \bug 	unknown
 */
void
NT3D_solid_o::DrawSurfacedObj()
{
    oMaterial.Paint();

    if (true == _bUseTexture() ) {
        NT_CERR(11, "NT3D_solid_o::DrawSurfacedObj() - with use of textures called.");
        oTexturePtr->Paint();
        gluQuadricNormals(pQuadObj, GL_SMOOTH);
        gluQuadricTexture(pQuadObj, GL_TRUE);
    } else  oTexturePtr->Painted();

    DrawSolidObj();

    if (true == _bUseTexture() ) oTexturePtr->Painted();
}


void
NT3D_solid_o::ControlSurfacedObj(NT3Dcontroller cntrl , NTint value)
{
    switch	(cntrl) {
    case NT3D_DrawStyle:
        drawStyle = value;
        gluQuadricDrawStyle( pQuadObj,
                             (GLenum) drawStyle );
        break;
    case NT3D_Slices:
        slices = value;
        break;
    case NT3D_Stacks:
        stacks = value;
        break;
    default:
        break;
    }
}

