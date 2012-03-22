/* nt_date_obj.h - date/time object */
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

/* $Id: nt_date_obj.h,v 1.3 2003/06/20 16:15:09 face Exp $
 * $Log: nt_date_obj.h,v $
 * Revision 1.3  2003/06/20 16:15:09  face
 * *** empty log message ***
 *
 * Revision 1.2  2003/06/20 13:26:00  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/03 16:26:17  face
 * edited and added new nt_error_o class:
 * nt_dimension_error_o
 * to report dimension mismatches between (e.g.NT_VECTOR_O)
 * bjects
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
 * Revision 1.6  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.6  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.5  1998/06/01 19:05:33  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/06/01 12:37:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/28 20:33:24  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/24 18:05:28  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/05/24 11:23:29  face
 * Initial revision
 *
 */


#ifndef _NT_DATE_OBJ_H_
#define _NT_DATE_OBJ_H_

#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

#include <iostream>
#include <ostream>
#include <ctime>

//
//	Class name : NT_date_o
//
/**	Date and time class for NetTrader

       nsec The number of nanoseconds after the second,
             range between 0-999999999
       sec The number of seconds after the minute, normally in
              the range 0 to 59, but can be up to 61 to allow for
              leap seconds.
       min The number of minutes after the hour, in the  range
              0 to 59.
       hour The  number  of hours past midnight, in the range 0
              to 23.
       mday The day of the month, in the range 1 to 31.
       mon The number of months since January, in the range  0
              to 11.
       year The number of years since year "0"
	      (NOTE: Jesus of Nazareth
	      was born in year 1 !, the first year of the second millenium
	      is the year 2001, not the year 2000
	      This notatation allows us to use the year figures as
	      usual).
	      \warning COMPATIBILITY: note that the Unix struct tm
	      \warning uses years since 1900 and Unix time_t
	      \warning resents the number of seconds elapsed since 00:00:00 on
	      \warning  January 1, 1970, Coordinated Universal Time (UTC).
       wday  The  number of days since Sunday, in the range 0 to
              6.
       yday   The number of days since January 1, in the range  0
              to 365.
       isdst  A  flag that indicates whether daylight saving time
              is in effect at the time described.  The  value  is
              positive if daylight saving time is in effect, zero
              if it is not, and negative if  the  information  is
              not available.

*/
class NT_date_o : public NT_o {

protected:

private:

    NTint nsec;  //!* nanoseconds (0-999999999) UNSUPPORTED
    NTint sec;      //!* seconds (0 - 60)
    NTint min;      //!* minutes (0 - 59)
    NTint hour;     //!* hours (0 - 23)
    NTint mday;     //!* day of month (1 - 31)
    NTint mon;      //!* month of year (0 - 11)
    NTint year;     //!* year (! no offset to 1900 !)
    NTint wday;     //!* day of week (Sunday = 0)
    NTint yday;     //!* day of year (0 - 365)
    NTint isdst;    //!* is summer time in effect?
    NTchar zone[NT_STRING_LENGTH];   //!* abbreviation of timezone name
    NTlong gmtoff;  //!* offset from UTC in seconds

public:
    NT_date_o();
    NT_date_o(const NT_date_o &);
    ~NT_date_o();
    struct tm * _tm() const;

    friend ostream & operator<<(ostream & os, const NT_date_o & self);
};

#endif /* _NT_DATE_OBJ_H_ */

