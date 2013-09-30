/**
 * @file date.cc
 * Date/time class implementations
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
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

#include "modigliani_base/date.h"

using modigliani_base::Date;

//
//    Method name : Datebj()
//
//    Description : Standard constructor
//    Input :
//    Output :
//
Date::Date() {
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
  snprintf(zone, sizeof("UTC"), "UTC");
  gmtoff = 0;
}

//
//    Method name : Datebj(const Datebj &)
//
//    Description :
//    Input :
//    Output :
//
Date::Date(const Date & original) {
  nsec = original.nsec;
  sec = original.sec;
  min = original.min;
  hour = original.hour;
  mday = original.mday;
  mon = original.mon;
  year = original.year;
  wday = original.wday;
  yday = original.yday;
  isdst = original.isdst;
  snprintf(zone, sizeof("UTC"), "UTC");
  gmtoff = original.gmtoff;
}

//
//    Method name : ~Date()
//
//    Description : destructor
//    Input :
//    Output :
//
Date::~Date() {
}

//
//    Method name : _tm
//
//    Description : convert date to unix tm_t structure
//    Input : deletable tm_t pointer to structure (delete zone first!)
//    Output :
//
struct tm * Date::_tm() const {
  struct tm * tmPtr;
  tmPtr = new struct tm;

  tmPtr->tm_sec = sec; /* seconds */
  tmPtr->tm_min = min; /* minutes */
  tmPtr->tm_hour = hour; /* hours */
  tmPtr->tm_mday = mday; /* day of the month */
  tmPtr->tm_mon = mon; /* month */
  tmPtr->tm_year = year - 1900; /* year */
  tmPtr->tm_wday = wday; /* day of the week */
  tmPtr->tm_yday = yday; /* day in the year */
  tmPtr->tm_isdst = isdst; /* daylight saving time */
  return (tmPtr);
}

std::ostream & operator<<(std::ostream & os, const Date & self) {
  struct tm * tmPtr;
  tmPtr = self._tm();
  os << asctime(tmPtr);

  delete (tmPtr);
  return (os);
}

