/**\file nt3d_vp_obj.h - NT3D_vp_o class header 
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

/* $Id: nt3d_vp_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_vp_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/10/03 08:29:59  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/06/13 20:10:26  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.3  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/05/13 11:34:59  face
 * Initial revision
 *
 * Revision 1.2  1999/05/09 09:34:15  face
 * corrected output
 *
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
*/
#ifndef _nt3d_vp_h_
#define _nt3d_vp_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include "nt3d_obj.h"
#include "nt3d_bitmap_obj.h"

/** @short NT3D_vp_o class
Superclass that unifys a common interface for
abstract viewport implementations and their
driver dependend realisations for various window systems (viewport driver).
NT3D_drv_vp_o basically provides an implementation for
all the functionality or infrastructure defined by this class,
which is assigned to an NT3D_abstract_vp_o which uses this infrastructure.
In this context it is possible that an NT3D_abstract_vp_o can be assigned
to different drivers. On the other side the abstract viewport
should provide all drawing within the Draw() method for the viewport 
drivers, which is the only method called from the driver classes.
In the other direction the abstract classes should actually avoid
to call the Draw() method themselves.
\bug unknown
\warning unknown 
 */
 
class NT3D_vp_o : public NT3D_o {
public:
  /***   Constructors, CopyAssignment and Destructor  ***/  
  NT3D_vp_o();
  NT3D_vp_o(const NT3D_vp_o & original);
  const NT3D_vp_o & operator= (const NT3D_vp_o & right);
  virtual ~NT3D_vp_o(){Disconnect();};
  /* ***  Methods              ***/  
  virtual void Init()=0;
  virtual void MakeCurrent()=0;
  virtual void SwapBuffer(){};
  virtual NTreturn Resize(NTsize width, NTsize height)=0;
  virtual void Redraw()=0;
  NTreturn Connect(NT3D_vp_o * connectingVP);
  NTreturn Disconnect();
  virtual NT3D_bitmap_o View2Bitmap()=0;
  virtual NT3D_bitmap_o * View2BitmapPtr(NT3D_bitmap_o * bmpPtr)=0;
  virtual NTsize _width()=0;
  virtual NTsize _height()=0;
  /* ***  Data                 ***/  
  protected:
  /* ***  Methods              ***/  
  NT3D_vp_o * pVP;
  /* ***  Data                 ***/  
  private:
  /* ***  Methods              ***/  
  /* ***  Data                 ***/
  NTbool bConnected;
 
};
#endif /* _nt3d_vp_h_ */ 

/* File skeleton generated by GenNTObj version 0.4. */
