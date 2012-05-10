/* Value.c++ - class implementation for the super object value */
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Value.h"

using namespace mbase;
//
//	Method name : Value
//
//	Description : Standard constructor
//	Input :
//	Output :
//
Value::Value()
{
    amount = 0;
    id = UNDEF;
    name="UNITIALIZED";
    value = 0.0;
}



//
//	Method name : Value
//
//	Description :
//	Input :
//	Output :
//


Value::Value(Mid InittypeId, M_string Initname, Mnum Initamount, Mreal Initvalue)
{
    amount= Initamount;
    id = InittypeId;
    name = Initname;
    value = Initvalue;
}


//
//	Method name : ~Value()
//
//	Description : Destructor
//	Input :
//	Output :
//
Value::~Value()
{
}



//
//	Method name : set_amount
//
//	Description :
//	Input :
//	Output :
//
void Value::set_amount(Mnum newAmount)
{
    amount = newAmount;
}

//
//	Method name : set_id(Mid newId)
//
//	Description :
//	Input :
//	Output :
//
void Value::set_id(Mid newId)
{
    id = newId;
}

//	Method name : set_name(M_std::string newName)
//
//	Description :
//	Input :
//	Output :
//
void Value::set_name(std::string newName)
{
    name = newName;
}



//
//	Method name :  set_value(Mreal newValue)
//
//	Description :
//	Input :
//	Output :
//
void Value::set_value(Mreal newValue)
{
    value = newValue;
}


std::ostream & operator<<(std::ostream & os, const Value & self)
{
    os << "Value" << M_SEPARATOR << self._id() << M_SEPARATOR
    << self._amount() << M_SEPARATOR << self._name() << M_SEPARATOR << self._value() << std::endl;
    return (os);
}



//
//	Method name : id
//
//	Description :
//	Input :
//	Output :
//
inline Mnum Value::_id() const {
	return (id);
}

//
//	Method name : name
//
//	Description : return the name attribute
//	Input :
//	Output : deletable Mstd::string
//
inline std::string Value::_name() const {
	return (name);
}

//
//	Method name : value
//
//	Description :
//	Input :
//	Output :
//
inline Mreal Value::_value() const {
	return (value);
}

