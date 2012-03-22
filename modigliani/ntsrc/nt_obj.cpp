/* nt_obj.cc - NT master object implementation */
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

/* $Id: nt_obj.cpp,v 1.4 2003/06/20 16:15:09 face Exp $
 * $Log: nt_obj.cpp,v $
 * Revision 1.4  2003/06/20 16:15:09  face
 * *** empty log message ***
 *
 * Revision 1.3  2002/02/25 22:48:54  face
 * *** empty log message ***
 *
 * Revision 1.2  2002/02/24 20:56:06  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/05/11 11:32:46  face
 * upgrade to NetTrader Version 0.5.
 * Common state of all files checkin (Versioon 0.5)
 * rearranged color and material classes...
 * texture class added
 *
 * Revision 1.2  2000/04/28 16:02:43  face
 * found nasty hidden bug, by initialisation of an hideen ellipsoid making the
 * entire program crash
 * on destructor call... reason unknown
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
 * Revision 1.3  1999/01/18 01:02:26  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/01/07 23:53:04  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/25 21:27:13  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.2  1998/06/01 19:05:26  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/06/01 12:37:32  face
 * Initial revision
 *
 */

#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"

/* static data member declaration in C++ part 2, see declaration */
NTlong NT_o::runnId = 0;

//
//	Method name : NT_o
//
//	Description : standard constructor
//	Input :
//	Output :
//
NT_o::NT_o() {
#ifdef NT_DEBUG_OBJECT_ID
	runnId++;
	uniqId = runnId;
#endif
}

NT_o::NT_o(NT_o const __attribute__((unused)) & right) {
#ifdef NT_DEBUG_OBJECT_ID
	runnId++;
	uniqId = runnId;
#endif
}

NT_o::~NT_o() {
}

