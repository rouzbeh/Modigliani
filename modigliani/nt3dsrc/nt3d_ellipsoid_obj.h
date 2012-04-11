/* nt3d_ellipsoid_obj.h - NT3D declaration of ellipsoid 
 * by Ahmed A. Faisal, 15. 10. 1998(c) 
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

/* $Id: nt3d_ellipsoid_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_ellipsoid_obj.h,v $
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
 * Revision 1.6  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/01/11 02:26:45  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/01/07 23:52:56  face
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
 *
 */

#ifndef _nt3d_ellipsoid_h_
#define _nt3d_ellipsoid_h_

#include "ntsrc/nt_main.h"
#include "ntsrc/nt_types.h"

#include "nt3d_solid_obj.h"

//
//      Class name : ellipsoid
//
//      Description : NT 3D ellipsoid class
//

/**
 Provides a scaled GLU sphere wrapped as an object
 */
class NT3D_ellipsoid_o: public NT3D_solid_o {
public:
	NT3D_ellipsoid_o();
	NT3D_ellipsoid_o(NT_vector3_o oNewPosition) :
			NT3D_solid_o(oNewPosition), oHalfAxis(1.0, 1.0, 1.0) {
	}
	;
	NT3D_ellipsoid_o(NT_vector3_o oNewPosition, NT_vector3_o oNewHalfAxis);
	NT3D_ellipsoid_o(const NT3D_ellipsoid_o & original);
	~NT3D_ellipsoid_o();
	/*   Methods                */
	void DrawSolidObj() const;
protected:
	/*   Data                   */
private:
	/*   Methods                */
	void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl,
			NTint __attribute__((unused)) value) {
	}
	;
	void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl,
			NTreal __attribute__((unused)) value) {
	}
	;
	void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl,
			NT_vector_o __attribute__((unused)) value) {
	}
	;
	/*   Data                   */
	NT_vector3_o oHalfAxis;

};

#endif /* _nt3d_ellipsoid_h_ */

