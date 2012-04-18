/* nt_list_obj.cc - general linked list implementation for any type--numerical, pointer or class */
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

/* $Id: nt_list_obj.cpp,v 1.2 2003/06/20 13:26:00 face Exp $
 * $Log: nt_list_obj.cpp,v $
 * Revision 1.2  2003/06/20 13:26:00  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
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
 * Revision 1.2  1998/11/22 22:53:56  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/11/20 09:43:20  face
 * Initial revision
 *
 * Revision 1.1  1998/07/02 18:58:34  face
 * Initial revision
 *
 *
 */

#include "nt_list_obj.h"
#include <iostream>

void pr(const NT_list_o<int>& list) {
	NT_iterator_list_o<int> it(list);
	while (!it.at_end()) {
		cout << it.current() << " ";
		it.next();
	}
	cout << endl;
}

void pr2(NT_list_o<int> list)
// to test copy constructor, destructor
		{
	NT_iterator_list_o<int> it(list);
	while (!it.at_end()) {
		cout << it.current() << " ";
		it.next();
	}
	cout << endl;
}

/*
 int main()
 {  NT_list_o<int> a;
 for (int i = 1; i <= 10; i++) a.insert(i);
 pr2(a);
 a.reset();
 while (!a.at_end())
 {  a.remove();
 a.next();
 }
 pr(a);
 return (0);
 }
 */
