/** \file tunnel_flight_demo.cpp - flight through a tunnel.
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


/* $Id: tunnel_flight_demo.cpp,v 1.3 2003/06/20 13:25:55 face Exp $
 * $Log: tunnel_flight_demo.cpp,v $
 * Revision 1.3  2003/06/20 13:25:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2002/03/04 18:08:31  face
 * added checking for NaN and Inf values in find_spikes_in_binary_file-
 *
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/11/04 22:56:03  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/11/04 10:32:15  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/04/26 17:10:07  face
 * added planetary_demo (derived from tunnel_flight_demo)
 * tunnel_flight_demo isn actually a tunnel demo anymore
 * added planetary_demo (derived from tunnel_flight_demo)
 * tunnel_flight_demo isn actually a tunnel demo anymore
 *
 * Revision 1.6  1999/12/03 16:35:58  face
 * the error lies in the initilisation
 * sequence nt3d_glx_drv
 *
 * Revision 1.5  1999/12/03 14:53:41  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/07/20 01:14:47  face
 * added testing code for universe_camera_vp
 *
 * Revision 1.3  1999/07/17 18:21:30  face
 * ienhanced, two moons now
 *
 * Revision 1.2  1999/07/16 15:04:58  face
 * added code::^
 *
 * Revision 1.1  1999/07/04 03:17:28  face
 * minor mods
 *
 */



/*
#include "nt3d_glut_drv_obj.h"
#include "nt3d_vp_glut_drv_obj.h"
#include "nt3d_demo_glut_drv_obj.h"
*/

#include "nt3d_glx_drv_obj.h"
#include "nt3d_camera_vp_obj.h"
#include "nt3d_universe_camera_vp_obj.h"
#include "nt3d_universe_obj.h"

#include "nt3d_sphere_obj.h"
#include "nt_error_obj.h"

#include "nt_gaussian_vec_rnd_dist_obj.h"
#include "nt3d_cylinder_obj.h"
#include "nt3d_position_lamp_obj.h"

#include <cmath>
#include <iostream>
#include <string>

NT_gaussian_vec_rnd_dist_o tmpDist(0,0.05,3);
NT_vector_o tmpVec(3);
NT3D_cylinder_o tunnel(NT_OriginVec3(),100,10);

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
    {
        moon = NT3D_sphere_o(NT_OriginVec3(), 0.2);
        moon2 = NT3D_sphere_o(NT_OriginVec3(), 0.1);
        lamp.SetTranslationX(0);
        lamp.SetTranslationZ(15);
        SetTranslationX(0);
        SetTranslationZ(15);
        SetRotationY(/*0.1*t*180.0/M_PI+*/270);
        t=0.0;

        universe.AddChild(&moon);
        universe.AddChild(&moon2);
        universe.AddChild(&earth);
        universe.AddChild(&lamp);
    }
    void DrawScene()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glShadeModel (GL_SMOOTH);
        lamp.Draw();

        {

            t = t + 0.01;
            moon.SetTranslationX(5*cos(t*3));
            moon.SetTranslationZ(5*sin(t*3));
            moon2.SetTranslationX(2*cos(t*4));
            moon2.SetTranslationZ(2*sin(t*4));

            /* Fixed camera */
            /*SetTranslationX(15*cos(t));
            SetTranslationZ(15*sin(t));*/

            moon.TranslateZ(0.1);
            moon.TranslateY(0.1);
            moon.TranslateX(0.1);
        }
        /*static NTbool exec = NT_FALSE;
        if (NT_FALSE == exec) {
        	lamp.On();
        	lamp.SetTranslationZ(10);
        	lamp.SetTranslationY(10);
        	lamp.SetTranslationX(10);
        	camera.SetTranslationZ(15.0);

        	glNewList(1, GL_COMPILE);
          	glBegin(GL_POINTS);
           	for (int llx=-0; llx < 10; llx++)
            	for (int lly=-10; lly < 10; lly++)
             	for (int llz=-10; llz < 10; llz++)
             	{
           	  	tmpVec = tmpDist.RndVec();
           		glVertex3f(tmpVec[0],tmpVec[1],tmpVec[2]+50);
           		glVertex3f(tmpVec[0],tmpVec[1],tmpVec[2]+55);
           	}
        	glEnd();
        	glEndList();

            exec = NT_TRUE;
           }*/
        //tunnel.Draw();
        earth.Draw();
        moon.Draw();
        moon2.Draw();
        //glCallList(1);
    }
};




int main(void)
{
    string a;
    cout << "nt3d_drv_test.cpp - demo of how to use the vp classes" << endl;
    cout << "NT3D_camera_vp_o ..." << endl;

    NT3D_demo_abstract_vp_o abstrVP;
    NT3D_demo_abstract_vp_o abstrView1VP;
    NT3D_demo_abstract_vp_o abstrView2VP;
    NT3D_demo_abstract_vp_o abstrView3VP;

    NT3D_glx_drv_o drvVP;
    NT3D_glx_drv_o drv1VP;
    NT3D_glx_drv_o drv2VP;
    NT3D_glx_drv_o drv3VP;


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


    cout << "Viewport Abstr<->Drv connectivity test completed." << endl;

    int ll;
    for (ll=0; ll < 100; ll++) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        abstrVP.Draw();
    }




    /*	abstrView1VP.AddUniverse(&universe);
    	abstrView1VP.Draw();
    	for (int ll=0; ll < 100; ll++) {
      		glClear(GL_COLOR_BUFFER_BIT);
      		glClear(GL_DEPTH_BUFFER_BIT);
    		abstrView1VP.Draw();
    	}


    */
    //NT3D_universe_camera_vp_o abstrView2VP;
    //NT3D_universe_camera_vp_o abstrView3VP;


    //abstrView2VP.AddUniverse(&universe);
    //abstrView3VP.AddUniverse(&universe);





    cerr << "Movement in all four windows" << endl;
    for (ll=0; ll < 100; ll++) {
        abstrView1VP.Draw();
        abstrView2VP.Draw();
        abstrView3VP.Draw();
        abstrVP.Draw();

    }
    /*
    	cerr << "Movement in first windows" << endl;
    	for (int ll=0; ll < 100; ll++) {
      		glClear(GL_COLOR_BUFFER_BIT);
      		glClear(GL_DEPTH_BUFFER_BIT);
    		abstrVP.Draw();
    	}

    	cerr << "Movement third windows" << endl;
        for (int ll=0; ll < 100; ll++) {
      		glClear(GL_COLOR_BUFFER_BIT);
      		glClear(GL_DEPTH_BUFFER_BIT);
    		abstrView3VP.Draw();
    	}

    	cerr << "Movement second windows" << endl;
    	for (int ll=0; ll < 100; ll++) {
      		glClear(GL_COLOR_BUFFER_BIT);
      		glClear(GL_DEPTH_BUFFER_BIT);
    		abstrView2VP.Draw();
    	}

    	cerr << "Movement in third windows" << endl;
    	for (int ll=0; ll < 100; ll++) {
      		glClear(GL_COLOR_BUFFER_BIT);
      		glClear(GL_DEPTH_BUFFER_BIT);
    		abstrView3VP.Draw();
    	}
    */


    cerr << "Movement in all four windows and movement of camera" << endl;
    for (ll=0; ll < 1000; ll++) {
        abstrVP.TranslateX(-0.1);
        abstrVP.Draw();
        abstrView1VP.TranslateZ(0.01);
        abstrView1VP.Draw();
        abstrView2VP.TranslateY(0.01);
        abstrView2VP.Draw();
        abstrView3VP.TranslateX(0.01);
        abstrView3VP.Draw();
    }
    cerr << "End of program code, cleaning up.";
    return 0;
}





