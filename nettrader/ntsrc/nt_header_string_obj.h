/* nt_header_string_obj.h - declaration of header information object
   in "string" storage classes  */
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


/* $Id: nt_header_string_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_header_string_obj.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/05/18 10:38:32  face
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
 * Revision 1.4  1999/03/27 16:12:57  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/09/28 00:15:27  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/06/02 20:07:34  face
 * Initial revision
 *
 */


#ifndef _NT_HEADER_STRING_OBJ_H_
#define _NT_HEADER_STRING_OBJ_H_

#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
#include "nt_error_obj.h"

class NT_header_string_o : NT_o
            /** PURPOSE:    The header block with reference count and available space
               REMARKS:    An instance of this structure is BELOW each string
                           that is allocated on the collectible heap
            */
{
public:
    int unique() {
        return _rc == 1;
    }
    void incref() {
        ++_rc;
    }
    int decref() {
        return --_rc;
    }

    NTsize length() const {
        return _length;
    }

    static char* alloc(const char[]);
    static char* alloc(int, const char[], int);
    static void free(char*);

    static NT_header_string_o* header(const char* p) {
        return (NT_header_string_o*)p - 1;
    }

private:
    char* start() const {
        return (char*)(this+1);
    }

    unsigned _rc; // reference count
    NTsize _length;

    friend class NT_string_o;
};

#endif /* _NT_HEADER_STRING_OBJ_H_ */
