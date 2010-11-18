/** \file flying_demo.cpp - a demo ...
 * by Ahmed Aldo Faisal &copy; created 18.5.2000
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:   0.4
 * Copyright (C) 1999,2000 Ahmed Aldo Faisal
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


/* $Id: spheric_flying_demo.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
 * $Log: spheric_flying_demo.cpp,v $
 * Revision 1.3  2003/06/20 13:25:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2002/02/25 22:48:49  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/11/04 22:56:03  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/11/04 10:32:15  face
 * *** empty log message ***
 *
 * Revision 1.1  2000/10/17 10:39:52  face
 * *** empty log message ***
 *
 * Revision 1.1  2000/06/26 19:13:25  face
 * new demo file: showing some random graphics
 *
 * */


#include "nt3d_glx_drv_obj.h"
#include "nt3d_camera_vp_obj.h"
#include "nt3d_bitmap_vp_obj.h"
#include "nt3d_universe_camera_vp_obj.h"
#include "nt3d_camera_obj.h"
#include "nt3d_target_camera_obj.h"
#include "nt3d_universe_obj.h"
#include "nt3d_spheric_camera_obj.h"

#include "nt3d_sphere_obj.h"
#include "nt_error_obj.h"

#include "nt_gaussian_vec_rnd_dist_obj.h"
#include "nt3d_cylinder_obj.h"
#include "nt3d_open_box_obj.h"
#include "nt3d_rectangle_obj.h"

#include "nt3d_position_lamp_obj.h"

#include "nt3d_rainbow_texture_obj.h"
#include "nt3d_worldmap_texture_obj.h"
#include "nt3d_moonmap_texture_obj.h"
#include "nt3d_file_texture_obj.h"
#include "nt_timing_obj.h"

#include <cmath>
#include <iostream>
#include <string>

int main(void)
{
    NT3D_glx_drv_o drvVP(200,200,true);
    NT3D_glx_drv_o bmp1drvVP(200,200,true);
    NT3D_universe_o theUniverse;
    NT3D_target_camera_o oTCam(NT_vector3_o(-15,0,0));
    NT3D_camera_o oCam;

    NT3D_universe_camera_vp_o abstrVP(&oTCam);
    abstrVP.AddUniverse(&theUniverse);
    if (NT_FAIL == abstrVP.Connect(&drvVP)) exit(1);
    abstrVP.Init();

    NT3D_bitmap_o MainViewBMP;
    NT3D_bitmap_vp_o bmp1VP(&MainViewBMP);
    if (NT_FAIL == bmp1VP.Connect(&bmp1drvVP)) exit(1);
    bmp1VP.Init();

    NT3D_position_lamp_o lamp;
    theUniverse.AddChild(&lamp);

    NT3D_cylinder_o tunnel(NT_OriginVec3(), 10, 2);
    tunnel.Control(NT3D_Texture, false);
    //theUniverse.AddChild(&tunnel);

    NT3D_rectangle_o rect(5,5,5);
    rect.Control(NT3D_Texture, false);
    //rect.SetTexture( new NT3D_moonmap_texture_o);
    //theUniverse.AddChild(&rect);

    NT3D_sphere_o sphere(NT_OriginVec3(), 30);
    sphere.Control(NT3D_Texture, true);
    sphere.SetTexture( new NT3D_worldmap_texture_o);
    theUniverse.AddChild(&sphere);

    NT3D_open_box_o box(2,2,2);
    box.Control(NT3D_Texture, false);
    //theUniverse.AddChild(&box);


    NT3D_glx_drv_o sphereDrvVP(200,200,true);
    NT3D_spheric_camera_o  sphereVP;
    sphereVP.SetUniverse(&theUniverse);
    if (NT_FAIL == sphereVP.Connect(&sphereDrvVP)) exit(1);
    sphereVP.Init();

    box.SetTranslationY(0);
    box.SetTranslationX(-10);

    sphere.SetTranslationZ(0);
    sphere.SetTranslationY(0);
    sphere.SetTranslationX(0);

    tunnel.SetTranslationY(0);
    tunnel.SetTranslationX(0);
    tunnel.SetRotationY(270);

    NTreal t = 0.0;
    for (NTint ll = 0; ll < 1000; ll++) {

        cerr << t << "\t" << flush;

        //  sphere.SetRotationY(t*100);
        //  box.SetRotationY(90);

        //	abstrVP.TranslateX(t*0.1);
        //	abstrVP.TranslateZ(sin(t)*0.01);


        abstrVP.Draw();
        abstrVP.View2BitmapPtr(&MainViewBMP);
        bmp1VP.Draw();

        sphereVP.Draw();

        GLenum error;
        while  ((error=glGetError()) != GL_NO_ERROR) cerr << "GLerror :" << gluErrorString(error)<<endl;

        sphereVP.eye.SetTranslationX(t);
        /*
        sphereVP.eye.SetTranslationY(t);
        sphereVP.eye.SetTranslationZ(t);
        */

        t = t + 0.1;
    }


    cerr << "End of program code, cleaning up.";

    return 0;
}


