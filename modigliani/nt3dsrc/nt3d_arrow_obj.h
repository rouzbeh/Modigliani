/** \file nt3d_arrow_obj.h - NT3D_arrow_o class declaration 
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


/* $Id: nt3d_arrow_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_arrow_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
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

#ifndef _nt3d_arrow_h_
#define _nt3d_arrow_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
#include "nt3d_obj.h"

#include "nt3d_solid_obj.h"
#include "nt3d_cylinder_obj.h"

/** @short NT3D_arrow_o class
  Draw an arrow which, is constructed of a cylinder as shaft and a conus as arrow head.
  It can either be rotated into a desired direction as all NT3D_objects_o OR it can be created
  and manipulated specifying a head and tail position for it in space. 
  \bug unknown
  \warning In case the arrow is ment to point into a certain direction designated by a base-tip constructor
  is used be sure to not rotate the object using any of the rotate calls!
 */
class NT3D_arrow_o : public NT3D_solid_o {
public:
  NT3D_arrow_o();
  NT3D_arrow_o(const NT3D_arrow_o & original);
  NT3D_arrow_o(const NT_vector3_o & newBase, const NT_vector3_o & newTip);
  const NT3D_arrow_o & operator= (const NT3D_arrow_o & right);
  virtual ~NT3D_arrow_o();
  /*   Methods                */  
  void Collinearize(const NT_vector3_o &);
  void DrawSolidObject() const{};
  /*   Data                   */  
	protected:
  /*   Methods                */  
  /*   Data                   */  
    private:
  /*   Methods                */  
  /*   Data                   */
  NT3D_cylinder_o head;
};




#endif /* _nt3d_arrow_h_ */









