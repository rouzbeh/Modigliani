/**\file nt3d_bitmap_vp_obj.h - NT3D_bitmap_vp_o class header
 * by Ahmed Aldo Faisal &copy; created 7.7.2000
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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


/* $Id: nt3d_bitmap_vp_obj.h,v 1.1 2001/06/29 13:16:55 face Exp $
* $Log: nt3d_bitmap_vp_obj.h,v $
* Revision 1.1  2001/06/29 13:16:55  face
* *** empty log message ***
*
* Revision 1.2  2000/11/04 10:32:16  face
* *** empty log message ***
*

*/
#ifndef _nt3d_bitmap_vp_obj_h_
#define _nt3d_bitmap_vp_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* Parent includes */
#include "nt3d_abstract_vp_obj.h"
/* NT includes */
#include "nt3d_bitmap_obj.h"
/* other includes */
/** @short NT3D_bitmap_vp_o class
\bug unknown
\warning unknown
*/
class NT3D_bitmap_vp_o : public NT3D_abstract_vp_o
{
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NT3D_bitmap_vp_o(NT3D_bitmap_o * newBmpPtr = NULL);
    NT3D_bitmap_vp_o(const NT3D_bitmap_vp_o & original);
    const NT3D_bitmap_vp_o & operator= (const NT3D_bitmap_vp_o & right);
    virtual ~NT3D_bitmap_vp_o();
    /* ***  Methods              ***/
    void Draw();
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    void RedrawChild() {}
    NTreturn ResizeChild(NTsize newWidth, NTsize newHeight) {
        return NT_SUCCESS;
    }
    void InitChild() {}
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    NT3D_bitmap_o * bmpPtr;
    NTsize width; // bitmap , not screen size
    NTsize height; // bitmap , not screen size
};
#endif /* _nt3d_bitmap_vp_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
