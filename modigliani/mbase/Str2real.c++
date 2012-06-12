/**
 * @file Str2real.c++
 * Str2real class implementation
 * @author Ahmed Aldo Faisal &copy; created 3.7.1999  
 * @version  0.0
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
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

#include "Str2real.h"

using namespace mbase;

/* ***      CONSTRUCTORS	***/
/** Create a Str2real */
Str2real::Str2real(std::string const& value) {
	value_ = value;
}

Str2real::Str2real(const char* c) {
	value_ = c;
}

Str2real::Str2real(double d) {
	std::stringstream s;
	s << d;
	value_ = s.str();
}

Str2real::Str2real(Str2real const& other) :
		Obj() {
	value_ = other.value_;
}

/* ***      COPY AND ASSIGNMEM	***/
Str2real& Str2real::operator=(Str2real const& other) {
	value_ = other.value_;
	return (*this);
}

/* ***      DESTRUCTOR		***/
Str2real::~Str2real() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
Str2real& Str2real::operator=(double i) {
	std::stringstream s;
	s << i;
	value_ = s.str();
	return (*this);
}

Str2real& Str2real::operator=(std::string const& s) {
	value_ = s;
	return (*this);
}

Str2real::operator std::string() const {
	return (value_);
}

Str2real::operator double() const {
	return (atof(value_.c_str()));
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
