/**
 * @file Stock_value.c++
 * Base class implementation for stock exchangable values
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

#include "Stock_value.h"

using namespace modigliani_base;
//
//	Method name : stock_value_o
//
//	Description : standard constructor
//	Input :
//	Output :
//
Stock_value::Stock_value() {
	stockExchangeId = STKEXC_UNDEF;
	currentBuyingPrice = 0.0;
	currentSellingPrice = 0.0;
}

//
//	Method name : Stock_value(const Stock_value &)
//
//	Description :
//	Input :
//	Output :
//
Stock_value::Stock_value(const Stock_value &) :
		Value() {

}

//
//	Method name : ~Stock_value
//
//	Description : destructor
//	Input :
//	Output :
//
Stock_value::~Stock_value() {

}

std::ostream & modigliani_base::operator<<(std::ostream & os, const Stock_value & self) {
	std::string tmpName = self._name();
	os << "Stock_value" << M_SEPARATOR << self._id() << M_SEPARATOR
	<< self._amount() << M_SEPARATOR << tmpName << M_SEPARATOR
	<< self._value() << M_SEPARATOR << self.currentBuyingPrice << M_SEPARATOR
	<< self.currentSellingPrice << M_SEPARATOR << self.emissionDate << std::endl;

	return (os);
}

