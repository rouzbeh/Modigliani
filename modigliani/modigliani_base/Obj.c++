/**
 * @file Obj.c++
 * Master object implementation
 *
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

#include "Obj.h"

using namespace modigliani_base;

/* static data member declaration in C++ part 2, see declaration */
Long Obj::runnId = 0;

//
//	Method name : Obj
//
//	Description : standard constructor
//	Input :
//	Output :
//
Obj::Obj() {
#ifdef M_DEBUG_OBJECT_ID
	runnId++;
	uniqId = runnId;
#endif
}

Obj::Obj(Obj const __attribute__((unused)) & right) {
#ifdef M_DEBUG_OBJECT_ID
	runnId++;
	uniqId = runnId;
#endif
}

Obj::~Obj() {
}


//
//	Method name : _runnId
//
//	Description :
//	Input :
//	Output :
//
inline Long Obj::_runnId() const
{
    return (runnId);
}


//
//	Method name : _uniqId
//
//	Description :
//	Input :
//	Output :
//
inline Long Obj::_uniqId() const
{
    return (uniqId);
}

