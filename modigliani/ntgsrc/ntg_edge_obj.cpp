/**\file ntg_edge_obj.cpp - NTG_edge_o class implementation
 * by Ahmed Aldo Faisal &copy; created 30.9.2000
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


/* $Id: ntg_edge_obj.cpp,v 1.1 2001/06/29 13:16:58 face Exp $
* $Log: ntg_edge_obj.cpp,v $
* Revision 1.1  2001/06/29 13:16:58  face
* *** empty log message ***
*
* Revision 1.2  2000/10/03 08:30:01  face
* *** empty log message ***
*
* Revision 1.1  2000/10/01 09:33:22  face
* added for the first time
*

*/
#include "ntg_edge_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTG_edge_o */
NTG_edge_o::NTG_edge_o(NTid newSource, NTid newTarget)
{
    source = newSource;
    target = newTarget;
}

/* ***      COPY AND ASSIGNMENT	***/
NTG_edge_o::NTG_edge_o(const NTG_edge_o & original)
{
    source = original.source;
    target = original.target;
}

const NTG_edge_o&
NTG_edge_o::operator= (const NTG_edge_o & right)
{
    if (this == &right) return *this; // Gracefully handle self assignment
    source = right.source;
    target = right.target;
    return *this;
}

/* ***      DESTRUCTOR		***/
NTG_edge_o::~NTG_edge_o()
{
}

/* ***  PUBLIC                                    ***   */
/** @short
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
*/
NTreturn
NTG_edge_o::SetConnection(NTid newSource, NTid newTarget)
{
    //cerr << "NTG_edge_o::SetConnection - Talk :"<< newSource << "\t" << newTarget << endl;
    source = newSource;
    target = newTarget;
    return NT_SUCCESS;
}



/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */