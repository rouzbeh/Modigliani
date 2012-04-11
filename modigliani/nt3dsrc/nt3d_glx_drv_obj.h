/**\file nt3d_glx_drv_obj.h - NT3D_glx_drv_o class header 
 * by Ahmed Aldo Faisal &copy; created 9.5.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.3
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

/* $Id: nt3d_glx_drv_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_glx_drv_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/10/03 08:29:59  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/07/06 08:06:10  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/05/18 10:38:25  face
 * an amazing error sits in nt3d_file_texture
 * if the constructor constructs a texture(1,1,1)
 * the system does not display the texture
 * if (64,64,2) is specified it does.
 * this is strange because the load routinge
 * should adapt the object correctly...investigate here 2DO
 *
 * Revision 1.5  2000/04/27 18:20:48  face
 * loads of changes:
 * -static init. fiasco workaounrd
 * -texture objects have new structure
 * ...
 *
 * Revision 1.4  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.3  1999/12/03 16:36:09  face
 * the error lies in the initilisation
 * sequence nt3d_glx_drv
 *
 * Revision 1.2  1999/07/20 01:15:02  face
 * general debugging and clean up
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.2  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/05/13 11:34:59  face
 * Initial revision
 *
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
 */

#ifndef _nt3d_glx_drv_h_
#define _nt3d_glx_drv_h_


#include "ntsrc/nt_main.h"
#include "ntsrc/nt_types.h"
#include "ntsrc/nt_obj.h"

#include "nt3d_drv_vp_obj.h"
#include "nt3d_bitmap_obj.h"

#include <cstring>

#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>




/** @short NT3D_glx_drv_o class
    Driver class to provide drawing window for OpenGL
    under X11 through the GLX extensions
    (No GLX support required by X server if Mesa is used!)
\bug unknown
\warning  When both common and individual contexts are to be used on, make
		  sure that the first VP to open is a common context. Otherwise
		  later opened common VP might miss their propoer drawing object init.
 */

class NT3D_glx_drv_o : public NT3D_drv_vp_o {
public:
  /***   Constructors, CopyAssignment and Destructor  ***/  
  NT3D_glx_drv_o(NTsize windowWidth = 200 , NTsize windowHeight = 200, bool useCommonContext = false);
  NT3D_glx_drv_o(const NT3D_glx_drv_o & original);
  const NT3D_glx_drv_o & operator= (const NT3D_glx_drv_o & right);
  virtual ~NT3D_glx_drv_o();
  /* ***  Methods              ***/  
  NTreturn Open(NT3D_vp_spec_o newWinSpec);
  NTreturn Close();
  /** makes the window associated with this object the current rendering context */
  void MakeCurrent() { /*cerr << "NT3D_glx_drv_o : "<<_uniqId() <<"MC"<<endl;*/
  					   if (NT_FALSE == glXMakeCurrent (dpy, win, ctx )) NT_CERR(1,"NT3D_glx_drv_o::MakeCurrent - error : unable to comply. Current rendering context not changed.");
  					   //else cerr << dpy << " "<< win <<" " << ctx << " "/*"NT3D_glx_drv_o::MakeCurrent - talk : Made windowNumber "*/<< windowNumber <<" current rendering context." << endl;
  					   }
  /** call instead of glXSwapBuffers */
  void SwapBuffer() { /*cerr << "NT3D_glx_drv_o : "<<_uniqId() <<"SB"<<endl;*/
                      glXSwapBuffers (dpy, win);}
  NTreturn Resize(NTsize width, NTsize height);
  NTreturn SetWindowTitle(string newTitle);

  /** Make sure that the method is called directly after a Draw() to the VP,
   ** as neither a MakeCurrent() nor a Draw()  is executed by this method */
  NT3D_bitmap_o   View2Bitmap();
  NT3D_bitmap_o * View2BitmapPtr(NT3D_bitmap_o * bmpPtr);

  NTsize _width(){return width;}
  NTsize _height(){return height;}

  protected:
  private:
  /* Create a window of given size and return its unique window number */
  NTint CreateWindow(NTint windowWidth, NTint windowHeight);
  /** Low level  and GLX call to open a RGBA double buffered window */
  NTreturn MakeRGBDBWindow(NTint windowWidth, NTint windowHeight);

  static NTint initCount;
  static NTint windowCount;
  static NTint commonCtxCount;
  unsigned int width;
  unsigned int height;
  NTint windowNumber;
  string windowTitle;
  static Display * dpy;
  Window win;
  GLXContext ctx;      
  bool useUniqueCommonCtx;
  static GLXContext uniqueCommonCtx;
};

#endif /* _nt3d_glx_drv_h_ */ 

/* File skeleton generated by GenNTObj version 0.3. */
