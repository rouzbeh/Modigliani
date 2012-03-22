/** \file nt_gui_drv_obj.h - NT_gui_drv_o class declaration
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


/* $Id: nt_gui_drv_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_gui_drv_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
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
 * Revision 1.2  1999/03/22 01:00:59  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/03/21 16:22:56  face
 * Initial revision
 *
 */

#ifndef _nt_gui_drv_h_
#define _nt_gui_drv_h_


#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

/** @short NT_gui_drv_o class
  Generic (abstract) driver for window systems
\bug unknown
\warning unknown
 */
class NT_gui_drv_o : public NT_o {
public:
    NT_gui_drv_o();
    NT_gui_drv_o(const NT_gui_drv_o & original);
    const NT_gui_drv_o& operator= (const NT_gui_drv_o & right);
    virtual ~NT_gui_drv_o();
    /*   Methods                */
    virtual void MakeCurrent() const;
    virtual void SwapBuffer() const;
    virtual void Resize(NTsize width, NTsize height);
    virtual void Redraw();
    /*   Data                   */
protected:
    /*   Methods                */
    /*   Data                   */
private:
    /*   Methods                */
    /*   Data                   */
};




#endif /* _nt_gui_drv_h_ */




