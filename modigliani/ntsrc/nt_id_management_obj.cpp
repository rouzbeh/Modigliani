/**\file nt_id_management_obj.cpp - NT_id_management_o class implementation
 * by Ahmed Aldo Faisal &copy; created 3.7.1999
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.4
 * Copyright (C) 1998,199 Ahmed Aldo Faisal
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

/* $Id: nt_id_management_obj.cpp,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_id_management_obj.cpp,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/10/15 19:23:48  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/10/03 08:30:03  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/04/28 16:02:43  face
 * found nasty hidden bug, by initialisation of an hideen ellipsoid making the
 * entire program crash
 * on destructor call... reason unknown
 *
 * Revision 1.2  1999/07/16 15:05:36  face
 * corrected error, switched to vector in id:mnanagement.added code::^
 *
 * Revision 1.1  1999/07/04 03:17:42  face
 * minor mods
 *

 */

#include "nt_id_management_obj.h"
#include "nt_error_obj.h"

/* ***		HELPER CLASS	*** */
class NT_id_management_compare_o {
public:
	int operator()(const NTid & a, const NTid & b) const {
		return (a < b);
	}
};

/* ***      CONSTRUCTORS	*** */
/** Create a NT_id_management_o */
NT_id_management_o::NT_id_management_o(NTid smallestId, NTid largestId) {
	NT_ASSERT_PRECOND(smallestId < largestId);
// cerr << "NT_id_management_o::NT_id_management_o(NTid smallestId, NTid largestId)" << endl;
	lowestId = smallestId;
	highestId = largestId;
	range = largestId - smallestId;
	/* initialize vector to unused compononents, (lowestId-1) */
	usedIdList.resize(range + 1);
	typedef vector<NTid>::iterator VI;
	VI it = usedIdList.begin();
	for (it = usedIdList.begin(); it != usedIdList.end(); it++)
		(*it) = (lowestId - 1);
}

/* ***      COPY AND ASSIGNMENT	***/
NT_id_management_o::NT_id_management_o(const NT_id_management_o & original) :
		NT_o() {
	lowestId = original.lowestId;
	highestId = original.highestId;
	range = original.range;
	usedIdList = original.usedIdList;
}

const NT_id_management_o&
NT_id_management_o::operator=(const NT_id_management_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	lowestId = right.lowestId;
	highestId = right.highestId;
	range = right.range;
	usedIdList = right.usedIdList;
	return (*this);
}

/* ***      DESTRUCTOR		***/
NT_id_management_o::~NT_id_management_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short      Check if an id was already taken
 @param      none
 @return
 \warning    unknown
 \bug        unknown
 */
NTbool NT_id_management_o::IsTaken(NTid id) const {
	/*set<NTid> tmp;
	 tmp.insert(usedIdList.begin(),42);
	 tmp.insert(usedIdList.begin(),43);
	 tmp.insert(tmp.begin(),id); */
	/*NTid tmp = id;
	 bool result = includes(&tmp, &tmp,usedIdList.begin(),usedIdList.end());
	 if (true == result) return NT_TRUE;
	 else return NT_FALSE; */
	if ((id < lowestId) || (id > highestId)) {
		cerr << "NT_id_management_o::IsTaken - Warning : Query id=" << id
				<< " out of id manager range [" << lowestId << "," << highestId
				<< "]. Will return 'false'." << endl;
		return NT_FALSE;
	}

	vector<NTid>::const_iterator it = usedIdList.begin();
	for (it = usedIdList.begin(); it != usedIdList.end(); it++) {
		if (id == (*it))
			return NT_TRUE;
	}
	return NT_FALSE;

}

/** @short      Largest currently issued id
 @param      none
 @return     on success NTid otherwise a value 1 smaller then lowestId
 \warning    unknown
 \bug        unknown
 */
NTid NT_id_management_o::LargestIssuedId() {
	return (*max_element(usedIdList.begin(), usedIdList.end())); //,NT_id_management_compare_o());
}

/** @short      Issue an id and mark it as taken
 @param      none
 @return     on success NTid otherwise a value 1 smaller then lowestId
 \warning    unknown
 \bug        unknown
 */
NTid NT_id_management_o::IssueId() {
	NTid ll;
	/* 2DO not very efficient as repeated run through list
	 with every IsTaken call */
	for (ll = lowestId; ll <= highestId; ll++) {
		/* search for a free id */
		if (!IsTaken(ll)) {
			vector<NTid>::iterator it = usedIdList.begin();
			/* search for a unused vector component */
			for (it = usedIdList.begin(); it != usedIdList.end(); it++) {
				/* is it a free component */
				if ((lowestId - 1) == *it) {
					*it = ll;
					return ll;
				}
			}
		}
	}
	return (lowestId - 1);
}

/** @short
 @param      none
 @return     NT_FAIL id was not issued.
 \warning    unknown
 \bug        unknown
 */
NTreturn NT_id_management_o::FreeId(NTid id) {
	vector<NTid>::iterator it;
	for (it = usedIdList.begin(); it != usedIdList.end(); ++it) {
		if (id == *it) {
			*it = lowestId - 1;
			return NT_SUCCESS;
		}
	}
	return NT_FAIL;
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.5. */
