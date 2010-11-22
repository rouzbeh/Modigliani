/**\file nt_str2real_obj.cpp - NT_str2real_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 3.7.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.0
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
  

/* $Id: nt_str2real_obj.cpp,v 1.1 2005/04/25 13:55:32 face Exp $ 
* $Log: nt_str2real_obj.cpp,v $
* Revision 1.1  2005/04/25 13:55:32  face
* *** empty log message ***
*

*/
#include <cstdlib>

#include "nt_str2real_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT_str2real_o */
NT_str2real_o::NT_str2real_o(std::string const& value) {
  value_=value;
}


NT_str2real_o::NT_str2real_o(const char* c) {
  value_=c;
}

NT_str2real_o::NT_str2real_o(double d) {
  std::stringstream s;
  s<<d;
  value_=s.str();
}

NT_str2real_o::NT_str2real_o(NT_str2real_o const& other) {
  value_=other.value_;
}

/* ***      COPY AND ASSIGNMENT	***/ 
NT_str2real_o& NT_str2real_o::operator=(NT_str2real_o const& other) {
  value_=other.value_;
  return *this;
}

/* ***      DESTRUCTOR		***/
NT_str2real_o::~NT_str2real_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
NT_str2real_o& NT_str2real_o::operator=(double i) {
  std::stringstream s;
  s << i;
  value_ = s.str();
  return *this;
}

NT_str2real_o& NT_str2real_o::operator=(std::string const& s) {
  value_=s;
  return *this;
}

NT_str2real_o::operator std::string() const {
  return value_;
}

NT_str2real_o::operator double() const {
  return atof(value_.c_str());
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
