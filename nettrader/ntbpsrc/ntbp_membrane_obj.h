/**\file ntbp_membrane_obj.h - NTBP_membrane_o class header
 * by Ahmed Aldo Faisal &copy; created 16.3.2001
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


/* $Id: ntbp_membrane_obj.h,v 1.1 2001/10/03 14:37:52 face Exp $
* $Log: ntbp_membrane_obj.h,v $
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_membrane_obj_h_
#define _ntbp_membrane_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* Parent includes */
#include "ntbp_object_obj.h"
/* NT includes */
#include "ntbp_membrane_compartment_obj.h"
/* other includes */

/** @short NTBP_membrane_o class
\bug unknown
\warning unknown
*/
class NTBP_membrane_o : public NTBP_object_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NTBP_membrane_o();
    NTBP_membrane_o(const NTBP_membrane_o & original);
    const NTBP_membrane_o & operator= (const NTBP_membrane_o & right);
    virtual ~NTBP_membrane_o();
    /* ***  Methods              ***/
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    vector< NTBP_membrane_compartment_o > membCompVec;
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
};
#endif /* _ntbp_membrane_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
