/* nt_currency_obj.c - class implementation for currency object */
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


/* $Id: nt_currency_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_currency_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.4  1999/06/26 16:49:02  face
 * *** empty log message ***
 *
 * Revision 2.3  1999/06/06 23:51:34  face
 * *** empty log message ***
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
 * Revision 1.2  1999/01/07 23:53:04  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/25 21:27:13  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.4  1998/06/28 17:12:38  face
 * debugged
 *
 * Revision 1.3  1998/06/01 19:05:26  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/24 11:22:47  face
 * perliminary version (cnst & self) constructor missing
 *
 * Revision 1.1  1998/05/22 16:44:54  face
 * Initial revision
 *
 */

#include "nt_main.h"
#include "nt_types.h"
#include "nt_codes.h"
#include "nt_currency_obj.h"



//
//	Method name : nt_currency_obj
//
//	Description : standard constructor
//	Input :
//	Output :
//
NT_currency_o::NT_currency_o()
{
}

//
//	Method name : NT_currency_o
//
//	Description : elaborate constructor
//	Input :
//	Output :
//
NT_currency_o::NT_currency_o(NTcountryCode Initcountry, NTreal InitconversionFactor)
{
    set_amount(0);
    set_id(NT_CURRENCY);
    set_name(countryCode2currencyName(Initcountry)); // 2DO implement conversion routine
    set_value(0.0);
    countryCode = Initcountry;
    conversionFactor = InitconversionFactor;
}




//
//	Method name : ~NT_currency_o
//
//	Description : standard destructor
//	Input :
//	Output :
//
NT_currency_o::~NT_currency_o()
{

}

//
//	Method name : code2country(NTcoutnryCode)
//
/**	Description : convert a country code into the apropriate currency name
	Input : valid country code (see nt_codes.h)
	Output : deletable NT_string (pointer to NTchar[NT_STRING_LENGTH)
*/
string NT_currency_o::countryCode2currencyName(NTcountryCode code)
{
    string country;
    switch (code) {
    case NT_EU :
        country = NT_EURO_CURRENCY;
        break;
    case NT_USA :
        country = NT_USA_CURRENCY;
        break;
    case NT_ITALY :
        country =  NT_ITALY_CURRENCY;
        break;
    case NT_GERMANY :
        country =  NT_GERMANY_CURRENCY;
        break;
    default :
        country =  "UNKOWN CURRENCY";
        break;
    }
    return country;
}


//* NOTE : const is omitted to access the element functions of the base class  */
ostream & operator<<(ostream & os,  const   NT_currency_o & self)
{
    string osName = self._name();
    os << "NT_currency_o"  << NT_SEPARATOR << self._id() << NT_SEPARATOR
    << self._amount() << NT_SEPARATOR  <<  osName << NT_SEPARATOR
    << self._value() << NT_SEPARATOR << (NTint) self.countryCode << NT_SEPARATOR
    << self.conversionFactor << endl;

    return os;
}
