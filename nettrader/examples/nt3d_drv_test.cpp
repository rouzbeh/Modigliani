/** \file nt3d_drv_test.cpp - demo of how to use the vp classes
 * by Ahmed Aldo Faisal &copy; created 10.6.1999
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:   0.3
 * Copyright (C) 1999 Ahmed Aldo Faisal
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


/* $Id: nt3d_drv_test.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
 * $Log: nt3d_drv_test.cpp,v $
 * Revision 1.2  2003/06/20 13:25:55  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/11/04 22:56:03  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/11/04 10:32:15  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/03 16:24:51  face
 * minor mods
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.2  1999/06/26 16:49:02  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/06/19 16:15:49  face
 * Initial revision
 *
 * Revision 1.3  1999/05/13 11:34:53  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 1.1  1999/04/20 13:47:34  face
 * Initial revision
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.1  1999/03/27 16:12:50  face
 * Initial revision
 *
 *
 */


#include <iostream>
#include <string>

#include "nt3d_camera_vp_obj.h"
#include "nt3d_glx_drv_obj.h"
#include "nt3d_ellipsoid_obj.h"
#include "nt_error_obj.h"

#include "nt_gaussian_vec_rnd_dist_obj.h"

NT_gaussian_vec_rnd_dist_o tmpDist(0,0.05,3);
NT_vector_o tmpVec(3);

class NT3D_demo_abstract_vp_o : public NT3D_camera_vp_o
{
public :
    void DrawScene ()
    {
        cameraPtr->RotateY(-0.01);
        cameraPtr->TranslateX(-0.001);
        MakeCurrent();
        glColor3f(0.8,0.8,0.8);
        static NTbool exec = NT_FALSE;
        if (NT_FALSE == exec) {
            glNewList(1, GL_COMPILE);
            glBegin(GL_POINTS);
            for (int llx=-0; llx < 10; llx++)
                for (int lly=-10; lly < 10; lly++)
                    for (int llz=-10; llz < 10; llz++)
                    {
                        tmpVec = tmpDist.RndVec();
                        glVertex3f(tmpVec[0],tmpVec[1],tmpVec[2]);
                    }
            glEnd();
            glEndList();
            exec = NT_TRUE;
        }
        glCallList(1);
    }
};




int main(void)
{
    string a;
    cout << "nt3d_drv_test.cpp - demo of how to use the vp classes" << endl;
    NT3D_demo_abstract_vp_o abstrVP;
    NT3D_glx_drv_o drvVP;
    if (NT_FAIL == abstrVP.Connect(&drvVP)) exit(1);
    abstrVP.Disconnect();
    if (NT_FAIL == drvVP.Connect(&abstrVP)) exit(2);
    abstrVP.Init();
    for (int ll=0; ll < 1000; ll++) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        abstrVP.Draw();
    }
    cin >> a;
    cout << "test completed." << endl;
    return 0;
}





