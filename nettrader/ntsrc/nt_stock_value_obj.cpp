/* nt_stock_value_obj.c - base class implementation for stock exchangable values */
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

/*
 * $Id: nt_stock_value_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_stock_value_obj.cpp,v $
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
 * Revision 1.4  1998/06/01 19:05:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/06/01 12:37:32  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/28 20:33:17  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/05/24 18:05:24  face
 * Initial revision
 *
 *
 */


#include "nt_stock_value_obj.h"
#include "nt_date_obj.h"

#include <string>

//
//	Method name : stock_value_o
//
//	Description : standard constructor
//	Input :
//	Output :
//
NT_stock_value_o::NT_stock_value_o()
{
    stockExchangeId = NT_STKEXC_UNDEF;
    currentBuyingPrice = 0.0;
    currentSellingPrice = 0.0;
}


//
//	Method name : NT_stock_value_o(const NT_stock_value_o &)
//
//	Description :
//	Input :
//	Output :
//
NT_stock_value_o::NT_stock_value_o(const NT_stock_value_o &)
{

}




//
//	Method name : ~NT_stock_value_o
//
//	Description : destructor
//	Input :
//	Output :
//
NT_stock_value_o::~NT_stock_value_o()
{

}


ostream & operator<<(ostream & os, const NT_stock_value_o & self)
{
    string tmpName = self._name();
    os << "NT_stock_value_o"  << NT_SEPARATOR << self._id() << NT_SEPARATOR
    << self._amount() << NT_SEPARATOR  <<  tmpName << NT_SEPARATOR
    << self._value() << NT_SEPARATOR << self.currentBuyingPrice << NT_SEPARATOR
    << self.currentSellingPrice << NT_SEPARATOR << self.emissionDate
    << endl;

    return os;
}

