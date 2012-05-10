/** \file nt3d_composite_obj.h - NT3D_composite_o class declaration 
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


/* $Id: nt3d_composite_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_composite_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.1  1999/04/20 13:47:34  face
 * Initial revision
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.1  1999/02/17 17:46:22  face
 * Initial revision
 *
 */

#ifndef _nt3d_composite_h_
#define _nt3d_composite_h_


#include "ntsrc/nt_main.h"
#include "ntsrc/nt_types.h"
#include "ntsrc/nt_obj.h"

#include "nt3d_hierarchic_object_obj.h"

/** @short NT3D_composite_o class
  template2 
\bug unknown
\warning unknown 
 */
class NT3D_composite_o : public NT3D_hierarchic_object_o {
public:
  NT3D_composite_o();
  NT3D_composite_o(const NT3D_composite_o & original);
  const NT3D_composite_o & operator= (const NT3D_composite_o & right);
  virtual ~NT3D_composite_o();
  /*   Methods                */  
  void DrawObj(){}
  /*   Data                   */  
protected:
  /*   Methods                */  
 void ControlObj(NT3Dcontroller cntrl , NTint value) {};
  void ControlObj(NT3Dcontroller cntrl , mbase::Mreal value) {}; 
  void ControlObj(NT3Dcontroller cntrl , NT_std::vector_o value){};
   /*   Data                   */  
    private:
  /*   Methods                */  
  /*   Data                   */
};




#endif /* _nt3d_composite_h_ */









