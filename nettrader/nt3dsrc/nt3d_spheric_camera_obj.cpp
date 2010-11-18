/**\file nt3d_spheric_camera_obj.cpp - NT3D_spheric_camera_o class implementation
 * by Ahmed Aldo Faisal &copy; created \date 8.7.2000
 */
/* \version Version:  0.5
* \author Ahmed Aldo Faisal
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


/* $Id: nt3d_spheric_camera_obj.cpp,v 1.2 2002/02/25 22:48:51 face Exp $
* $Log: nt3d_spheric_camera_obj.cpp,v $
* Revision 1.2  2002/02/25 22:48:51  face
* *** empty log message ***
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.2  2000/11/04 10:32:16  face
* *** empty log message ***
*

*/
#include "nt3d_spheric_camera_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_spheric_camera_o */
NT3D_spheric_camera_o::NT3D_spheric_camera_o(NTsize azimutalSteps, NTsize altitudalSteps)
        :
        NT3D_universe_vp_o()
{
    aziSteps = azimutalSteps;
    altSteps = altitudalSteps;
}


/* ***      COPY AND ASSIGNMENT	***/
NT3D_spheric_camera_o::NT3D_spheric_camera_o(const NT3D_spheric_camera_o & original)
{
    NT_CERR(1,"NT3D_spheric_camera_o::NT3D_spheric_camera_o(...) - Error : Const contructor not implemented."); // add assignment code here
}

const NT3D_spheric_camera_o&
NT3D_spheric_camera_o::operator= (const NT3D_spheric_camera_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    NT_CERR(1,"NT3D_spheric_camera_o::operator= - Error : Not implemented."); // add assignment code here
    return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_spheric_camera_o::~NT3D_spheric_camera_o()
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
NT3D_spheric_camera_o::DrawChild()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    NTsize width = pVP->_width();
    NTsize height = pVP->_height();

    NTsize degX = 360/aziSteps;
    NTsize degY = 180/altSteps;

    NTsize deltaX = width/aziSteps;
    NTsize deltaY = height/altSteps;

    NT_vector3_o pos = eye._oTranslation();

    for (NTsize llx=0; llx < aziSteps; llx++) {
        for (NTsize lly=0; lly < altSteps; lly++) {
            glViewport(llx*deltaX,lly*deltaY, deltaX, deltaY);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(degY,degX/degY, 0.1, 100);
            // 2DO here we omit the orientation of the camera
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef( -pos.x, -pos.y, -pos.z);
            // THIS CLASS IS A MESS ; AAF,25.02.2002
            // why can't is the camera not a camera object ?
            // why do we also have universe_camera vp ?
            gluLookAt(0, 0, 0,
                      0, (-(llx-aziSteps/2) + 0.5)*degX,(-(llx-aziSteps/2) + 0.5)*degX,
                      0,1,0);
            glTranslatef( pos.x,pos.y,pos.z);
//    	glRotatef((-(llx-aziSteps/2) + 0.5)*degX, 0.0, 1.0, 0.0);
            //		glRotatef((lly-altSteps/2 + 0.5)*degY, 0.0, 0.0, 1.0);

            DrawUniverse();
        }
    }


}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.8. */
