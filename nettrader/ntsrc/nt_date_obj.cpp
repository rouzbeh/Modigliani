/* nt_date_obj.cc - date/time class implementations */
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


/* $Id: nt_date_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_date_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.2  1999/02/17 17:46:18  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.5  1998/06/01 19:05:26  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/05/28 20:33:17  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/24 18:05:24  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/24 12:59:06  face
 * added nsec
 *
 * Revision 1.1  1998/05/24 11:22:47  face
 * Initial revision
 *
 */

#include "nt_date_obj.h"

#include <string.h>


//
//	Method name : NT_date_obj()
//
//	Description : Standard constructor
//	Input :
//	Output :
//
NT_date_o::NT_date_o()
{
    nsec = 0;
    sec = 0;
    min = 0;
    hour = 0;
    mday = 0;
    mon = 0;
    year = 1; /* year (! no offset to 1900 !) */
    wday = 0;
    yday = 0;
    isdst = 0;
    strcpy(zone,"UTC");
    gmtoff = 0;
}


//
//	Method name : NT_date_obj(const NT_date_obj &)
//
//	Description :
//	Input :
//	Output :
//
NT_date_o::NT_date_o(const NT_date_o &)
        :
        NT_o()
{

}


//
//	Method name : ~NT_date_o()
//
//	Description : destructor
//	Input :
//	Output :
//
NT_date_o::~NT_date_o()
{

}



//
//	Method name : _tm
//
//	Description : convert date to unix tm_t structure
//	Input : deletable tm_t pointer to structure (delete zone first!)
//	Output :
//
struct tm * NT_date_o::_tm() const
    {
        struct tm * tmPtr;
        tmPtr = new (struct tm);

        tmPtr->tm_sec=sec;         /* seconds */
        tmPtr->tm_min=min;         /* minutes */
        tmPtr->tm_hour=hour;        /* hours */
        tmPtr->tm_mday=mday;        /* day of the month */
        tmPtr->tm_mon=mon;         /* month */
        tmPtr->tm_year=year-1900;        /* year */
        tmPtr->tm_wday=wday;        /* day of the week */
        tmPtr->tm_yday=yday;        /* day in the year */
        tmPtr->tm_isdst=isdst;       /* daylight saving time */
        /** apparently timezone features unsupported by glibc2  */
//  tmPtr->__tm_zone__ = new NTchar[NT_STRING_LENGTH];
// strcpy((char *)(tmPtr->__tm_zone__),zone);   /* abbreviation of timezone name */
        //  tmPtr->__tm_gmtoff__=gmtoff;  /* offset from UTC in seconds */
        return tmPtr;
    }






ostream & operator<<(ostream & os, const  NT_date_o & self)
{
    struct tm * tmPtr;
    tmPtr = self._tm();
    os << asctime(tmPtr);

    /** apparently timezone features unsupported by glibc2  */
    //     delete(tmPtr->__tm_zone__);
    delete(tmPtr);
    return os;
}

