/** \file flying_through.cpp - fly through ...
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


/* $Id: flying_through.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
 * $Log: flying_through.cpp,v $
 * Revision 1.3  2003/06/20 13:25:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2002/03/04 18:08:31  face
 * added checking for NaN and Inf values in find_spikes_in_binary_file-
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
 * */


#include "nt3d_glx_drv_obj.h"
#include "nt3d_camera_vp_obj.h"
#include "nt3d_universe_camera_vp_obj.h"
#include "nt3d_universe_obj.h"

#include "nt3d_sphere_obj.h"
#include "nt_error_obj.h"

#include "nt_gaussian_vec_rnd_dist_obj.h"
#include "nt3d_cylinder_obj.h"
#include "nt3d_open_box_obj.h"


#include "nt3d_position_lamp_obj.h"


#include "nt3d_rainbow_texture_obj.h"
#include "nt3d_worldmap_texture_obj.h"
#include "nt3d_moonmap_texture_obj.h"
#include "nt3d_file_texture_obj.h"
#include "nt_timing_obj.h"


#include <cmath>
#include <iostream>
#include <string>

#include <stdio.h>
GLboolean CheckError( const char *message )
{
    GLenum error = glGetError();
    if (error) {
        char *err = (char *) gluErrorString( error );
        fprintf( stderr, "GL Error: %s at %s\n", err, message );
        return GL_TRUE;
    }
    return GL_FALSE;
}


int main(void)
{
    NT3D_glx_drv_o drvVP(400,400,true);
    NT3D_glx_drv_o drv1VP(200,200,true);
    NT3D_glx_drv_o drv2VP(200,200,true);
    NT3D_glx_drv_o drv3VP(200,200,true);

    NT3D_universe_o theUniverse;

    NT3D_universe_camera_vp_o abstrVP(&theUniverse);
    NT3D_universe_camera_vp_o abstrView1VP(&theUniverse);
    NT3D_universe_camera_vp_o abstrView2VP(&theUniverse);
    NT3D_universe_camera_vp_o abstrView3VP(&theUniverse);

    if (NT_FAIL == abstrVP.Connect(&drvVP)) exit(1);
    abstrVP.Init();
    if (NT_FAIL == abstrView1VP.Connect(&drv1VP)) exit(2);
    abstrView1VP.Init();
    if (NT_FAIL == drv2VP.Connect(&abstrView2VP)) exit(3);
    abstrView2VP.Init();
    if (NT_FAIL == abstrView3VP.Connect(&drv3VP)) exit(4);
    abstrView3VP.Init();

    abstrView1VP.SetTranslationX(20);
    abstrView1VP.SetTranslationY(0);
    abstrView1VP.SetRotationY(180);

    abstrView2VP.SetTranslationX(0);
    abstrView2VP.SetTranslationY(0);
    abstrView2VP.SetTranslationZ(50);
    abstrView2VP.SetRotationZ(0);
    abstrView2VP.SetRotationY(-90);

    abstrView3VP.SetTranslationX(0);
    abstrView3VP.SetTranslationY(50);
    abstrView3VP.SetTranslationZ(0);
    abstrView3VP.SetRotationZ(-90);


    NT3D_position_lamp_o lamp;
    theUniverse.AddChild(&lamp);


#define numBoxes 8
    NT3D_open_box_o box[numBoxes];
    NTreal angle;
    NTint ll;
    for (ll=0; ll < numBoxes; ll++)
    {
        /*box[ll].Set_width(3);
        box[ll].Set_depth(10);
        box[ll].Set_height(10);*/
        box[ll].SetTexture(new NT3D_file_texture_o("veilmap_128_64.tga"));
        theUniverse.AddChild(&(box[ll]));
        angle = 2*M_PI/numBoxes * ll;
        box[ll].SetRotationY(angle*180/M_PI);
        box[ll].SetTranslationX(-5 * cos(angle));
        box[ll].SetTranslationZ(5 * sin(angle));
    }

    /*
    NT3D_open_box_o box1(3,10,10);
    box1.SetTexture(new NT3D_file_texture_o("veilmap_128_64.tga"));
    theUniverse.AddChild(&box1);
    box1.SetRotationY(90);
    box1.SetTranslationZ(5);

    NT3D_open_box_o box2(3,10,10);
    box2.SetTexture(new NT3D_file_texture_o("veilmap_128_64.tga"));
    theUniverse.AddChild(&box2);
    box2.SetRotationY(180);
    box2.SetTranslationX(5);

    NT3D_open_box_o box3(3,10,10);
    box3.SetTexture(new NT3D_file_texture_o("veilmap_128_64.tga"));
    theUniverse.AddChild(&box3);
    box3.SetRotationY(270);
    box3.SetTranslationZ(-5);

    */
    NTreal t = 0.0;

    for (ll = 0; ll < 1000; ll++) {
        t = t + 0.1;
        cout << t << "\t" << flush;

        abstrVP.SetTranslationX(cos(t)*10);
        abstrVP.SetTranslationZ(sin(t)*10);
        abstrVP.SetRotationY(180/M_PI *  t + 180);

        CheckError("test");

        abstrVP.Draw();
        abstrView1VP.Draw();
        abstrView2VP.Draw();
        abstrView3VP.Draw();
    }



    cerr << "End of program code, cleaning up.";

    return 0;
}


