/** \file nt3d_cylinder_obj.h - NT_cylinder_o class declaration 
 * by Ahmed Aldo Faisal &copy; created  15.10.1998  
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

/* $Id: nt3d_cylinder_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_cylinder_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/05/13 09:42:55  face
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
 * Revision 1.3  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/12 08:26:18  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/01/11 02:26:45  face
 * Initial revision
 *
 * Revision 1.1  1999/01/07 23:52:56  face
 * Initial revision
 *
 * Revision 1.1  1998/12/25 21:28:17  face
 * Initial revision
 *
 *
 */

#ifndef _nt3d_cylinder_h_
#define _nt3d_cylinder_h_

#include "mbase/nt_main.h"
#include "mbase/nt_types.h"
#include "nt3d_object_obj.h"
#include "nt3d_solid_obj.h"

#include <GL/glu.h>

/** @short NT3D_cylinder_o class
 Draws a cylinder with its height pointed towards Z-Axis
 \bug unknown
 \warning unknown
 */
class NT3D_cylinder_o: public NT3D_solid_o {
public:
	NT3D_cylinder_o();
	NT3D_cylinder_o(NT_std::vector3_o oNewPosition);
	NT3D_cylinder_o(NT_std::vector3_o oBaseCenter, mbase::Real newHeight,
			mbase::Real newRadius);
	NT3D_cylinder_o(const NT3D_cylinder_o & original);
	const NT3D_cylinder_o & operator=(const NT3D_cylinder_o & right);
	~NT3D_cylinder_o();
	/*   Methods                */
	void DrawSolidObj() const;
	/*   Data                   */
protected:
	/*   Methods                */
	virtual void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl,
			NTint __attribute__((unused)) value) {
	}
	;
	virtual void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl,
			mbase::Real __attribute__((unused)) value) {
	}
	;
	virtual void ControlObj(NT3Dcontroller __attribute__((unused)) cntrl,
			NT_std::vector_o __attribute__((unused)) value) {
	}
	;
	/*   Data                   */
private:
	/*   Methods                */
	void Init(mbase::Real radius);
	/*   Data                   */
	mbase::Real topRadius;
	mbase::Real baseRadius;
	mbase::Real height;
	mbase::Mbool bHaveTopClosed;
	mbase::Mbool bHaveBaseClosed;
};

#endif /* _nt3d_cylinder_h_ */

