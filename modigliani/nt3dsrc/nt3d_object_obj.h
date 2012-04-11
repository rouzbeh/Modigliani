/* nt3d_object_obj.h - NT 3D supcerclass declaration
 * by Ahmed A. Faisal, 5. 12. 1998(c) 
 *
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
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


/* $Id: nt3d_object_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_object_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2003/06/20 13:25:57  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/10/01 09:33:47  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/06/26 19:14:06  face
 * finally found the error that caused solid object to disappear:
 * the scaling vector was not constructed in the 2nd constructor
 * of the solid bject thus resulting in a zero object
 * new demo file: showing some random graphics
 *
 * Revision 1.8  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/05/13 09:42:55  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/05/11 11:32:44  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
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
 * Revision 1.3  1999/07/20 01:15:06  face
 * general debugging and clean up
 *
 * Revision 1.2  1999/07/16 15:05:13  face
 * added code::^
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.3  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 2.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.7  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.6  1999/01/19 09:23:50  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/11 02:26:45  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/01/07 12:56:23  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/12/27 11:53:34  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.2  1998/12/16 14:57:14  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/06 15:03:06  face
 * Initial revision
 *
 */

#ifndef _nt3d_object_h_
#define _nt3d_object_h_

#include "ntsrc/nt_main.h"
#include "ntsrc/nt_types.h"

#include "ntsrc/nt_vector_obj.h"
#include "ntsrc/nt_vector3_obj.h"
#include <deque>
//#include "ntsrc/nt_data_obj.h"

#include "nt3d_obj.h"

enum NT3Dcontroller { NT3D_DrawStyle = 1000, 
				NT3D_Slices, 
				NT3D_Stacks,
				NT3D_Texture
			} ;

 
//
//	Class name : NT3D_object_o
//
/**	NT3D object class. Definition of a drawable entity with positional
    and rotational characteristics in 3D space.

	The "main axis" of an object in space,  
	should be constructed along the X-Axis, as methods
	will assume this.
 */
class NT3D_object_o : public NT3D_o {
public:
  NT3D_object_o();
  NT3D_object_o(const NT_vector3_o & oNewPosition);
  NT3D_object_o(const NT3D_object_o & original);
   
  virtual ~NT3D_object_o();
  /*                         */  
  void Rotate( const NT_vector3_o & oDeltaRotation );
  void RotateX ( NTreal deltaRotX);
  void RotateY ( NTreal deltaRotY);
  void RotateZ ( NTreal deltaRotZ);
  void RotateXYZ ( NT_vector3_o oDeltaRot ) { oRotation += oDeltaRot; }
  void RotateXYZ ( const NTreal deltaRot[3] );
  void RotateNull () { oRotation = NT_OriginVec3();};
  void SetRotationX ( const NTreal rotX){ oRotation.x= rotX; }
  void SetRotationY ( const NTreal rotY){ oRotation.y= rotY; }
  void SetRotationZ ( const NTreal rotZ){ oRotation.z= rotZ; }
  void SetRotationXYZ ( const NT_vector3_o & oRot ) { oTranslation = oRot; }
  
  virtual void Translate( const NT_vector3_o & oDeltaTranslation ); 
  virtual void TranslateX ( const NTreal deltaTraX);
  virtual void TranslateY ( const NTreal deltaTraY);
  virtual void TranslateZ ( const NTreal deltaTraZ);
  virtual void TranslateXYZ ( const NT_vector3_o & oDeltaTra ) { oTranslation += oDeltaTra; }
  virtual void TranslateXYZ ( const NTreal deltaTra[3]);
  virtual void TranslateNull () { oTranslation = NT_OriginVec3();};
  virtual void SetTranslationX ( const NTreal traX){ oTranslation.x= traX; }
  virtual void SetTranslationY ( const NTreal traY){ oTranslation.y= traY; }
  virtual void SetTranslationZ ( const NTreal traZ){ oTranslation.z= traZ; }
  virtual void SetTranslationXYZ ( const NT_vector3_o & oTra ) { oTranslation = oTra; }

  /** rotate the object such that its (oRotation vector would rotate 
      a vector on the) x-axis in the direction of oRelativeVector */ 
  void Collinearize (const NT_vector3_o & oRelativeVector);


  NT_vector3_o _oTranslation() const { return oTranslation; }
  NT_vector3_o _oRotation() const { return oRotation; }


 /** The Draw() method is the invocation method to use for users.
    It handles the drawing of the object */
  virtual void Draw() = 0;
  /** Set any control/state variables in the object and in the child tree */
  void Control(NT3Dcontroller cntrl , NTint value){ControlImpl(cntrl , value);}
  void Control(NT3Dcontroller cntrl , NTreal value){ControlImpl(cntrl , value);}
  void Control(NT3Dcontroller cntrl , NT_vector_o value){ControlImpl(cntrl , value);}
 
  protected:
  NT_vector3_o oTranslation;
  NT_vector3_o oRotation;

 /** Set the control/state variable in *this object.
    ControlImpl() should be overridden by derived classes ONLY if control/state
    are to be applied to *this object !*/
  virtual void ControlImpl(NT3Dcontroller cntrl , NTint value) = 0;
  virtual void ControlImpl(NT3Dcontroller cntrl , NTreal value) = 0; 
  virtual void ControlImpl(NT3Dcontroller cntrl , NT_vector_o value) = 0;

  private:
  double padding;

};


#endif /* _nt3d_object_h_ */


