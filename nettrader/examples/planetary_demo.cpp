/** \file planetary_demo.cpp - visualise a solar system
 * by Ahmed Aldo Faisal &copy; created 25.4.2000
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


/* $Id: planetary_demo.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
 * $Log: planetary_demo.cpp,v $
 * Revision 1.3  2003/06/20 13:25:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2003/01/30 17:13:39  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
 * Revision 1.14  2000/11/04 22:56:03  face
 * *** empty log message ***
 *
 * Revision 1.13  2000/11/04 10:32:15  face
 * *** empty log message ***
 *
 * Revision 1.12  2000/07/06 08:06:20  face
 * *** empty log message ***
 *
 * Revision 1.11  2000/06/13 20:10:23  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/05/18 10:37:17  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/05/13 09:42:53  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/05/11 12:42:05  face
 * removed nt3d_rainbow_o by intergrating it into nt3d_light_o
 *
 * Revision 1.7  2000/05/11 11:32:29  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 *
 * Revision 1.6  2000/04/28 16:02:11  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/04/27 18:20:16  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/04/26 22:57:15  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/04/26 20:52:59  face
 * *** empty log message ***
 *
 * Revision 1.1  2000/04/26 17:10:07  face
 * added planetary_demo (derived from tunnel_flight_demo)
 * tunnel_flight_demo isn actually a tunnel demo anymore
 * added planetary_demo (derived from tunnel_flight_demo)
 * tunnel_flight_demo isn actually a tunnel demo anymore
 * */




#include "nt3d_glx_drv_obj.h"
#include "nt3d_camera_vp_obj.h"
#include "nt3d_universe_camera_vp_obj.h"
#include "nt3d_universe_obj.h"

#include "nt3d_sphere_obj.h"
#include "nt_error_obj.h"

#include "nt_gaussian_vec_rnd_dist_obj.h"
#include "nt3d_cylinder_obj.h"
#include "nt3d_position_lamp_obj.h"
#include "nt3d_rainbow_texture_obj.h"
#include "nt3d_worldmap_texture_obj.h"
#include "nt3d_moonmap_texture_obj.h"
#include "nt3d_file_texture_obj.h"
#include "nt_timing_obj.h"

#include <cmath>
#include <iostream>
#include <string>


/* NT3D_rainbow_texture_o a; */

/*
NT_gaussian_vec_rnd_dist_o tmpDist(0,0.05,3);
NT_vector_o tmpVec(3);
NT3D_cylinder_o tunnel(NT_OriginVec3(),100,10);
*/
class NT3D_demo_abstract_vp_o : public NT3D_camera_vp_o
{
public :
    NT3D_position_lamp_o lamp;
    NT3D_sphere_o earth;
    NT3D_sphere_o moon;
    NT3D_sphere_o moon2;
    NT3D_universe_o universe;
    NTfloat t;

    NT3D_demo_abstract_vp_o()
            :
            earth(NT_OriginVec3(), 1.2),
            moon(NT_OriginVec3(), 0.8),
            moon2(NT_OriginVec3(), 0.4)
    {
        lamp.On();
        lamp.SetTranslationX(15);

        SetTranslationX(15);
        SetRotationY(/*0.1*t*180.0/M_PI+*/180);
        t = 0.0;

        earth.SetRotationX(90);
        moon.SetRotationX(90);
        moon2.SetRotationX(90);

        /*
        moon2.SetTexture(new NT3D_file_texture_o("worldmap_128_64.tga"));
        earth.SetTexture( new NT3D_file_texture_o("coralmap_128_64.tga"));//new NT3D_moonmap_texture_o );
        moon.SetTexture( new NT3D_file_texture_o("moonmap_128_64.tga"));//new NT3D_moonmap_texture_o );
        */
        //earth.SetTexture( new NT3D_worldmap_texture_o );

        earth.SetTexture( new NT3D_worldmap_texture_o);
        moon.SetTexture( new NT3D_file_texture_o("moonmap_128_64.tga"));
        moon2.SetTexture( new NT3D_file_texture_o("ganymedemap_128_64.tga"));

        universe.AddChild(&earth);
        universe.AddChild(&moon);
        universe.AddChild(&moon2);
        universe.AddChild(&lamp);
    }


    void
    DrawScene()
    {
        glEnable(GL_DEPTH_TEST);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glShadeModel (GL_SMOOTH);
        lamp.Draw();
        {
            t = t + 0.01;
            // The motion speed of the moons follows Keplers 2nd(?) law */
            earth.SetRotationZ(100*t);
            moon.SetTranslationX(8*cos(t));
            moon.SetTranslationY(0.3*cos(t));
            moon.SetTranslationZ(8*sin(t));
            moon.SetRotationZ(200*t);
            moon2.SetTranslationX(4*cos(t*5/3));
            moon2.SetTranslationZ(4*sin(t*5/3));
            moon2.SetRotationZ(400*t);
            lamp.SetTranslationX(50*cos(t*0.1));
            lamp.SetTranslationZ(50*sin(t*0.1));
        }
        earth.Draw();
        moon.Draw();
        moon2.Draw();
    }
};




int main(void)
{
    string a;

    NT3D_glx_drv_o drvVP;
    NT3D_glx_drv_o drv1VP;
    NT3D_glx_drv_o drv2VP;
    NT3D_glx_drv_o drv3VP;

    NT3D_demo_abstract_vp_o abstrVP;
    NT3D_demo_abstract_vp_o abstrView1VP;
    NT3D_demo_abstract_vp_o abstrView2VP;
    NT3D_demo_abstract_vp_o abstrView3VP;

    if (NT_FAIL == abstrVP.Connect(&drvVP)) exit(1);
    abstrVP.Disconnect();
    if (NT_FAIL == drvVP.Connect(&abstrVP)) exit(2);
    abstrVP.Init();
    if (NT_FAIL == abstrView1VP.Connect(&drv1VP)) exit(3);
    abstrView1VP.Init();
    if (NT_FAIL == drv2VP.Connect(&abstrView2VP)) exit(4);
    abstrView2VP.Init();
    if (NT_FAIL == abstrView3VP.Connect(&drv3VP)) exit(5);
    abstrView3VP.Init();

    abstrView1VP.SetTranslationX(0);
    abstrView1VP.SetTranslationY(0);
    abstrView1VP.SetTranslationZ(-15);
    abstrView1VP.SetRotationY(-270);

    abstrView2VP.SetTranslationX(0);
    abstrView2VP.SetTranslationY(0);
    abstrView2VP.SetTranslationZ(15);
    abstrView2VP.SetRotationZ(0);
    abstrView2VP.SetRotationY(-90);

    abstrView3VP.SetTranslationX(0);
    abstrView3VP.SetTranslationY(22);
    abstrView3VP.SetTranslationZ(0);
    abstrView3VP.SetRotationZ(-90);

    NT_timing_o stopwatch;
    stopwatch.start();
    for (int ll=0; ll < 1000; ll++) {
        abstrVP.Draw();
        abstrView1VP.Draw();
        abstrView2VP.Draw();
        abstrView3VP.Draw();
        //if (100 == ll) abstrView3VP.Resize(300,300);
    }
    stopwatch.stop();
    cerr << "4 window movement completed in t=" << stopwatch.seconds() << " seconds ." << endl;

    cerr << "End of program code, cleaning up.";
    return 0;
}



