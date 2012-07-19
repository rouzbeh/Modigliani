/**
 * @file id_management.cc
 * Id_management class implementation
 * @author Ahmed Aldo Faisal &copy; created 3.7.1999
 * @version  0.4
 * Copyright (C) 1998,199 Ahmed Aldo Faisal
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

#include "id_management.h"

using namespace modigliani_base;

/* ***        HELPER CLASS    *** */
class M_id_management_compare_o {
public:
    int operator()(const Mid & a, const Mid & b) const {
        return (a < b);
    }
};

/* ***      CONSTRUCTORS    *** */
/** Create a Id_management */
Id_management::Id_management(Mid smallestId, Mid largestId) {
    M_ASSERT_PRECOND(smallestId < largestId);
// cerr << "Id_management::Id_management(Mid smallestId, Mid largestId)" << endl;
    lowestId = smallestId;
    highestId = largestId;
    range = largestId - smallestId;
    /* initialize vector to unused compononents, (lowestId-1) */
    usedIdList.resize(range + 1);
    typedef std::vector<Mid>::iterator VI;
    VI it = usedIdList.begin();
    for (it = usedIdList.begin(); it != usedIdList.end(); it++)
        (*it) = (lowestId - 1);
}

/* ***      COPY AND ASSIGNMEM    ***/
Id_management::Id_management(const Id_management & original) :
        Obj() {
    lowestId = original.lowestId;
    highestId = original.highestId;
    range = original.range;
    usedIdList = original.usedIdList;
}

const Id_management&
Id_management::operator=(const Id_management & right) {
    if (this == &right)
        return (*this); // Gracefully handle self assignment
    // add assignment code here
    lowestId = right.lowestId;
    highestId = right.highestId;
    range = right.range;
    usedIdList = right.usedIdList;
    return (*this);
}

/* ***      DESTRUCTOR        ***/
Id_management::~Id_management() {
}

/* ***  PUBLIC                                    ***   */
/** @short      Check if an id was already taken
 @param      none
 @return
 \warning    unknown
 \bug        unknown
 */
bool Id_management::IsTaken(Mid id) const {
    /*set<Mid> tmp;
     tmp.insert(usedIdList.begin(),42);
     tmp.insert(usedIdList.begin(),43);
     tmp.insert(tmp.begin(),id); */
    /*Mid tmp = id;
     bool result = includes(&tmp, &tmp,usedIdList.begin(),usedIdList.end());
     if (true == result) return true;
     else return false; */
    if ((id < lowestId) || (id > highestId)) {
        std::cerr << "Id_management::IsTaken - Warning : Query id=" << id
                << " out of id manager range [" << lowestId << "," << highestId
                << "]. Will return 'false'." << std::endl;
        return (false);
    }

    std::vector<Mid>::const_iterator it = usedIdList.begin();
    for (it = usedIdList.begin(); it != usedIdList.end(); it++) {
        if (id == (*it))
            return (true);
    }
    return (false);

}

/** @short      Largest currently issued id
 @param      none
 @return     on success Mid otherwise a value 1 smaller then lowestId
 \warning    unknown
 \bug        unknown
 */
Mid Id_management::LargestIssuedId() {
    return (*max_element(usedIdList.begin(), usedIdList.end())); //,M_id_management_compare_o());
}

/** @short      Issue an id and mark it as taken
 @param      none
 @return     on success Mid otherwise a value 1 smaller then lowestId
 \warning    unknown
 \bug        unknown
 */
Mid Id_management::IssueId() {
    Mid ll;
    /* 2DO not very efficient as repeated run through list
     with every IsTaken call */
    for (ll = lowestId; ll <= highestId; ll++) {
        /* search for a free id */
        if (!IsTaken(ll)) {
            std::vector<Mid>::iterator it = usedIdList.begin();
            /* search for a unused vector component */
            for (it = usedIdList.begin(); it != usedIdList.end(); it++) {
                /* is it a free component */
                if ((lowestId - 1) == *it) {
                    *it = ll;
                    return (ll);
                }
            }
        }
    }
    return (lowestId - 1);
}

/** @short
 @param      none
 @return     M_FAIL id was not issued.
 \warning    unknown
 \bug        unknown
 */
ReturnEnum Id_management::FreeId(Mid id) {
    std::vector<Mid>::iterator it;
    for (it = usedIdList.begin(); it != usedIdList.end(); ++it) {
        if (id == *it) {
            *it = lowestId - 1;
            return (ReturnEnum::SUCCESS);
        }
    }
    return (ReturnEnum::FAIL);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenMObj version 0.5. */