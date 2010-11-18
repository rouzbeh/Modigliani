/** \file nt3d_demo_glut_drv_obj.h - NT_demo_glut_drv_o class declaration
 * by Ahmed Aldo Faisal &copy; created  18.1.1998
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:  0.2
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


/* $Id: nt3d_demo_glut_drv_obj.h,v 1.1 2001/06/29 13:16:51 face Exp $
 * $Log: nt3d_demo_glut_drv_obj.h,v $
 * Revision 1.1  2001/06/29 13:16:51  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/11/04 22:56:03  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/11/04 10:32:15  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/07/03 15:17:20  face
 * Initial revision
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
 * Revision 1.1  1999/03/27 16:12:57  face
 * Initial revision
 *
 */

#ifndef _nt3d_demo_glut_drv_h_
#define _nt3d_demo_glut_drv_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"


#include "nt3d_vp_glut_drv_obj.h"
/** @short NT3D_demo_glut_drv_o class
  Simple demonstration of how to use the vp_glut_drv object
\bug unknown
\warning unknown
 */
class NT3D_demo_glut_drv_o : public NT3D_vp_glut_drv_o {
public:
    NT3D_demo_glut_drv_o();
    NT3D_demo_glut_drv_o(const NT3D_demo_glut_drv_o & original);
    const NT3D_demo_glut_drv_o & operator= (const NT3D_demo_glut_drv_o & right);
    virtual ~NT3D_demo_glut_drv_o();
    /*   Methods                */
    void CallBackDisplayFunc(void);
    void Init();
    /*   Data                   */
protected:
    /*   Methods                */
    /*   Data                   */
private:
    /*   Methods                */
    /*   Data                   */
};




#endif /* _nt3d_demo_glut_drv_h_ */









