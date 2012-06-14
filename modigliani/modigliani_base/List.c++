/**
 * @file List.c++
 * General linked list implementation for any type--numerical, pointer or class
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

#include "List.h"
#include <iostream>

using namespace modigliani_base;

void pr(const List<int>& list) {
	Iterator_list<int> it(list);
	while (!it.at_end()) {
		std::cout << it.current() << " ";
		it.next();
	}
	std::cout << std::endl;
}

void pr2(List<int> list)
// to test copy constructor, destructor
		{
	Iterator_list<int> it(list);
	while (!it.at_end()) {
		std::cout << it.current() << " ";
		it.next();
	}
	std::cout << std::endl;
}
