/**
 * @file Date.h
 * Date/time object
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * NetTrader - finance management, analysis and simulation system
 * @version  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mbase_date_h_
#define _mbase_date_h_

#include "main.h"
#include "types.h"
#include "Obj.h"

#include <iostream>
#include <ostream>
#include <ctime>


namespace mbase {
/**
 * 	Date and time class for NetTrader

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
class Date: public Obj {

protected:

private:

	int_t nsec; //!* nanoseconds (0-999999999) UNSUPPORTED
	int_t sec; //!* seconds (0 - 60)
	int_t min; //!* minutes (0 - 59)
	int_t hour; //!* hours (0 - 23)
	int_t mday; //!* day of month (1 - 31)
	int_t mon; //!* month of year (0 - 11)
	int_t year; //!* year (! no offset to 1900 !)
	int_t wday; //!* day of week (Sunday = 0)
	int_t yday; //!* day of year (0 - 365)
	int_t isdst; //!* is summer time in effect?
	Mchar zone[M_STRING_LENGTH]; //!* abbreviation of timezone name
	Mlong gmtoff; //!* offset from UTC in seconds

public:
	Date();
	Date(const Date &);
	~Date();
	struct tm * _tm() const;

	friend std::ostream & operator<<(std::ostream & os, const Date & self);
};
}
#endif /* _mbase_date_h_ */

