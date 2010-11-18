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


/* $Id: nt3d_glx_test.cpp,v 1.2 2003/06/20 13:25:55 face Exp $
 * $Log: nt3d_glx_test.cpp,v $
 * Revision 1.2  2003/06/20 13:25:55  face
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

int main(void)
{
    NT3D_glx_drv_o drvVP(400,400);
    NT3D_universe_o theUniverse;
    NT3D_universe_camera_vp_o abstrVP(&theUniverse);
    if (NT_FAIL == abstrVP.Connect(&drvVP)) exit(1);
    abstrVP.Init();

    NT3D_position_lamp_o lamp;
    theUniverse.AddChild(&lamp);

    NT3D_sphere_o world(NT_OriginVec3(), 10);
    theUniverse.AddChild(&world);

    lamp.On();
    world.SetRotationY(90);


    NTreal t = 0.0;
    for (NTint ll = 0; ll < 1000; ll++) {
        t = t + 0.1;
        cerr << t << "\t" << flush;
        abstrVP.Draw();
        abstrVP.SetTranslationX(-t);
    }



    cerr << "End of program code, cleaning up.";

    return 0;
}


