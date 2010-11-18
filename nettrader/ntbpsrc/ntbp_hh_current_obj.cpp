/**\file ntbp_hh_current_obj.cpp - NTBP_hh_current_o class implementation
 * by Ahmed Aldo Faisal &copy; created 17.3.2001
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


/* $Id: ntbp_hh_current_obj.cpp,v 1.1 2001/10/03 14:37:52 face Exp $
* $Log: ntbp_hh_current_obj.cpp,v $
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#include "ntbp_hh_current_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hh_current_o */
NTBP_hh_current_o::NTBP_hh_current_o(NTreal newReversalPotential, NTreal newMaxConductivity, NTreal newArea)
        :
        NTBP_membrane_current_o(newReversalPotential)
{
    NT_ASSERT(newMaxConductivity >= 0);
    NT_ASSERT(newArea >= 0);
    SetSimulationMode(NTBP_DETERMINISTIC);
    maxConductivity = newMaxConductivity;
    area = newArea;
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_hh_current_o::NTBP_hh_current_o(const NTBP_hh_current_o & original)
        :
        NTBP_membrane_current_o(original._reversalPotential())
{
// add assignment code here
}

const NTBP_hh_current_o&
NTBP_hh_current_o::operator= (const NTBP_hh_current_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
// add assignment code here
    return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_hh_current_o::~NTBP_hh_current_o()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown

void
NTBP_hh_current_o::() const
{
}
*/
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
