/* nt_value_obj.c - class implementation for the super object value */
/* by Ahmed A. Faisal, 22. 5. 1998(c) */

/*
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


/* $Id: nt_value_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_value_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.2  1999/05/08 23:59:01  face
 * *** empty log message ***
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.5  1998/06/28 17:12:38  face
 * debugged
 *
 * Revision 1.4  1998/06/28 13:26:59  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/06/01 19:05:26  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/24 11:22:47  face
 * adding fruther test routines
 *
 * Revision 1.1  1998/05/22 16:44:54  face
 * Initial revision
 *
 */

#include "nt_main.h"
#include "nt_types.h"
#include "nt_value_obj.h"

#include "string.h"
//
//	Method name : NT_value_o
//
//	Description : Standard constructor
//	Input :
//	Output :
//
NT_value_o::NT_value_o()
{
    amount = 0;
    id = NT_UNDEF;
    name="UNITIALIZED";
    value = 0.0;
}



//
//	Method name : NT_value_o
//
//	Description :
//	Input :
//	Output :
//


NT_value_o::NT_value_o(NTid InittypeId, NT_string Initname, NTnum Initamount, NTreal Initvalue)
{
    amount= Initamount;
    id = InittypeId;
    name = Initname;
    value = Initvalue;
}


//
//	Method name : ~NT_value_o()
//
//	Description : Destructor
//	Input :
//	Output :
//
NT_value_o::~NT_value_o()
{
}



//
//	Method name : set_amount
//
//	Description :
//	Input :
//	Output :
//
void NT_value_o::set_amount(NTnum newAmount)
{
    amount = newAmount;
}

//
//	Method name : set_id(NTid newId)
//
//	Description :
//	Input :
//	Output :
//
void NT_value_o::set_id(NTid newId)
{
    id = newId;
}

//	Method name : set_name(NT_string newName)
//
//	Description :
//	Input :
//	Output :
//
void NT_value_o::set_name(string newName)
{
    name = newName;
}



//
//	Method name :  set_value(NTreal newValue)
//
//	Description :
//	Input :
//	Output :
//
void NT_value_o:: set_value(NTreal newValue)
{
    value = newValue;
}


ostream & operator<<(ostream & os, const NT_value_o & self)
{
    os << "NT_value_o" << NT_SEPARATOR << self.id << NT_SEPARATOR
    << self.amount << NT_SEPARATOR << self.name << NT_SEPARATOR << self.value << endl;
    return os;
}

