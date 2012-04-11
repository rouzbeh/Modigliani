/**\file nt3d_target_camera_obj.h - NT3D_target_camera_o class header 
 * by Ahmed Aldo Faisal &copy; created 26.4.2000  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.4
 * Copyright (C) 1998,1999, 2000 Ahmed Aldo Faisal    
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
  

/* $Id: nt3d_target_camera_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: nt3d_target_camera_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.4  2000/10/01 09:33:47  face
* *** empty log message ***
*
* Revision 1.3  2000/04/27 18:20:48  face
* loads of changes:
* -static init. fiasco workaounrd
* -texture objects have new structure
* ...
*
* Revision 1.2  2000/04/26 20:53:02  face
* *** empty log message ***
*
* Revision 1.1  2000/04/26 17:55:39  face
* *** empty log message ***
*

*/
#ifndef _nt3d_target_camera_h_ 
#define _nt3d_target_camera_h_ 

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"  
/* Parent includes */
#include "nt3d_camera_obj.h" 
/* NT includes */

/* other includes */
#include <GL/glu.h>

/** @short NT3D_target_camera_o class 
Provides an abstraction of a positionable camera
which looks at a fixed target specified in camera coordinates.
The camera can be translated in space whith the view always
pointing at the target coordinates.
\bug unknown
\warning Rotation of this camera has no effect 
*/
class NT3D_target_camera_o : public NT3D_camera_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NT3D_target_camera_o(const NT_vector3_o & oNewPosition = NT_OriginVec3(),
					 const NT_vector3_o & oNewTarget = NT_UnitXVec3());
NT3D_target_camera_o(const NT3D_target_camera_o & original);
const NT3D_target_camera_o & operator= (const NT3D_target_camera_o & right);
 ~NT3D_target_camera_o();
/* ***  Methods              ***/  
void Draw();
/* NOT NEEDED 
void Translate( const NT_vector3_o & oDeltaTranslation ) { NT3D_object_o::Translate(oDeltaTranslation); oTarget += oDeltaTranslation; }
void TranslateX ( const NTreal deltaTraX) { NT3D_object_o::TranslateX(deltaTraX); oTarget.x += deltaTraX; }
void TranslateY ( const NTreal deltaTraY) { NT3D_object_o::TranslateY(deltaTraY); oTarget.y += deltaTraY; }
void TranslateZ ( const NTreal deltaTraZ) { NT3D_object_o::TranslateZ(deltaTraZ); oTarget.z += deltaTraZ; }
void TranslateXYZ ( const NT_vector3_o & oDeltaTra ) { NT3D_object_o::TranslateXYZ(oDeltaTra); oTarget += oDeltaTra;}
void TranslateXYZ ( const NTreal deltaTra[3]) { NT3D_object_o::TranslateXYZ(deltaTra); for(NTsize ll=0; ll<3; ll++) oTarget[ll] += deltaTra[ll];}
void TranslateNull () { oTarget -= _oTranslation(); NT3D_object_o::TranslateNull();}
void SetTranslationX ( const NTreal traX){ NT3D_object_o::SetTranslationX(traX); oTarget.x = traX ; } //2DO
void SetTranslationY ( const NTreal traY){ NT3D_object_o::SetTranslationY(traY); oTarget.y = traY ; } //2DO
void SetTranslationZ ( const NTreal traZ){ NT3D_object_o::SetTranslationZ(traZ); oTarget.z = traZ ; } //2DO
void SetTranslationXYZ ( const NT_vector3_o & oTra ) { oTranslation = oTra; oTarget = oTra ;} //2DO
*/
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/  
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/
NT_vector3_o oldPosition;
NT_vector3_o tmpDirection;
NT_vector3_o oldDirection;
};

#endif /* _nt3d_target_camera_h_ */ 

/* File skeleton generated by GenNTObj version 0.6. */
