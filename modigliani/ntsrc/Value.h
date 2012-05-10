/* Value.h - class header for value class */
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

#ifndef _mbase_value_h_
#define _mbase_value_h_

#include "types.h"
#include "codes.h"
#include "Obj.h"

#include <ostream>
#include <cstring>
//
//	Class name : mbase::Value
//
//	Description :
//
namespace mbase {
class Value: public Obj {
protected:

	void set_amount(Mnum newAmount);
	void set_id(Mid newId);
	void set_name(std::string newName);
	void set_value(Mreal newValue);

private:
	Mnum amount;
	Mid id;
	std::string name;
	Mreal value;

public:
	Value();
	Value(Mid typeId, M_string name, Mnum amount, Mreal value);
	virtual ~Value();
	friend std::ostream & operator<<(std::ostream & os, const Value & self);
	Mnum _amount() const {
		return (amount);
	}
	Mid _id() const;
	std::string _name() const;
	Mreal _value() const;
};

}

#endif /* _mbase_value_h_ */
