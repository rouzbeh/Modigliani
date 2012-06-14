/** @file header_string.h
 * Declaration of header information object in "string" storage classes
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

#ifndef _mbase_header_string_h_
#define _mbase_header_string_h_

#include "main.h"
#include "types.h"
#include "Obj.h"
#include "nt_error.h"

namespace mbase {
class Header_string: Obj
/** PURPOSE:    The header block with reference count and available space
 REMARKS:    An instance of this structure is BELOW each string
 that is allocated on the collectible heap
 */
{
public:
	int unique() {
		return (_rc == 1);
	}
	void incref() {
		++_rc;
	}
	int decref() {
		return (--_rc);
	}

	Size_t length() const {
		return (_length);
	}

	static char* alloc(const char[]);
	static char* alloc(int, const char[], int);
	static void free(char*);

	static Header_string* header(const char* p) {
		return ((Header_string*) p - 1);
	}

private:
	char* start() const {
		return ((char*) (this + 1));
	}

	unsigned _rc; // reference count
	Size_t _length;

	friend class M_string_o;
};
}
#endif /* _mbase_header_string_h_ */
