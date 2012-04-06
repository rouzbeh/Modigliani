/**\file nt3d_universal_vp_obj.h - NT3D_universal_vp_o class header 
 * by Ahmed Aldo Faisal &copy; created 13.5.1999  
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

/* $Id: nt3d_universal_vp_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_universal_vp_obj.h,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/11/04 10:32:16  face
 * *** empty log message ***
 *
 */

#ifndef _nt3d_universal_vp_h_
#define _nt3d_universal_vp_h_

#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include "nt3d_abstract_vp_obj.h"

/** @short NT3D_universal_vp_o class

 \bug unknown
 \warning unknown
 */
class NT3D_universal_vp_o: public NT3D_abstract_vp_o {
public:
	/***   Constructors, CopyAssignment and Destructor  ***/
	NT3D_universal_vp_o();
	NT3D_universal_vp_o(const NT3D_universal_vp_o & original);
	const NT3D_universal_vp_o & operator=(const NT3D_universal_vp_o & right);
	virtual ~NT3D_universal_vp_o();
	/* ***  Methods              ***/
	virtual void Draw();
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	void RedrawChild() {
	}
	NTreturn ResizeChild(NTsize __attribute__((unused)) width,
			NTsize __attribute__((unused)) height) {
		return NT_SUCCESS;
	}
	void InitChild() {
	}
	/* ***  Data                 ***/
};
#endif /* _nt3d_universal_vp_h_ */ 

/* File skeleton generated by GenNTObj version 0.4. */