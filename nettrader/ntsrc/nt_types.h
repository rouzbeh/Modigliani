/* nt_types.h - type declarations for NetTrader */
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


/* $Id: nt_types.h,v 1.5 2002/03/13 19:23:30 face Exp $
 * $Log: nt_types.h,v $
 * Revision 1.5  2002/03/13 19:23:30  face
 * *** empty log message ***
 *
 * Revision 1.4  2002/03/04 18:09:10  face
 * added checking for NaN and Inf values in find_spikes_in_binary_file-
 * BinomialStep: redo loop if after state change the states do not sum up
 *
 * Revision 1.3  2002/02/25 22:48:54  face
 * *** empty log message ***
 *
 * Revision 1.2  2001/10/03 14:37:13  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/10/03 08:30:03  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/10/01 09:33:48  face
 * *** empty log message ***
 *
 * Revision 1.6  2000/06/13 20:10:28  face
 * *** empty log message ***
 *
 * Revision 1.5  2000/05/18 10:38:33  face
 * *** empty log message ***
 *
 * Revision 1.4  1999/07/20 01:15:28  face
 * added NT_string_o as string type
 *
 * Revision 1.3  1999/07/16 15:05:39  face
 * corrected error, switched to vector in id:mnanagement.added code::^
 *
 * Revision 1.2  1999/07/04 03:17:42  face
 * minor mods
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.4  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 2.3  1999/05/08 23:58:55  face
 * *** empty log message ***
 *
 * Revision 2.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.11  1999/02/17 17:46:22  face
 * *** empty log message ***
 *
 * Revision 1.10  1998/12/06 15:03:06  face
 * *** empty log message ***
 *
 * Revision 1.9  1998/10/21 23:55:11  face
 * *** empty log message ***
 *
 * Revision 1.8  1998/10/19 14:09:03  face
 * *** empty log message ***
 *
 * Revision 1.7  1998/07/07 12:10:15  face
 * *** empty log message ***
 *
 * Revision 1.6  1998/06/02 20:07:34  face
 * Version 0.2
 *
 * Revision 1.5  1998/06/01 19:05:33  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/06/01 12:37:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/22 16:44:49  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/22 12:06:48  face
 * skeleton laid
 *
 * Revision 1.1  1998/05/22 11:56:32  face
 * Initial revision
 *
 */

#ifndef _NT_TYPES_H_
#define _NT_TYPES_H_

#include "nt_main.h"
#include <string>
#include <cmath>
#include <iostream>

using namespace std;

typedef float    NTfloat;
typedef double   NTdouble;
typedef NTdouble NTreal;

typedef int      	NTint;
typedef unsigned int    NTuint;
typedef unsigned int 	NTsize;
typedef long int   	NTlong;
typedef NTlong   	NTnum;

typedef long int NTid;

typedef char      NTchar;
typedef char* NT_string;
typedef string NT_string_o;

typedef bool NTbool; /* FALSE TRUE , used for state variables */
const NTbool NT_FALSE = (NTbool) false;
const NTbool NT_TRUE = (NTbool) true;

enum NTreturn {NT_SUCCESS, NT_FAIL, NT_NOT_IMPLEMENTED, NT_NOT_DERIVED, /* SUCCESS FAIL ... , used for return "error codes" of funcs */
               NT_OUT_OF_MEMORY, /* memory operations */
               NT_FILE_NOT_FOUND, NT_WRONG_FILE_TYPE, NT_BAD_FILE, NT_EMPTY_FILE, NT_BAD_DATA, NT_READ_FAIL, NT_NO_SPACE_LEFT, NT_WRITE_FAIL, /* file op returns */
               NT_NOT_EXISTING_NODE, NT_NOT_EXISTING_EDGE, /* NTG graph operations */
               NT_PARAM_OUT_OF_RANGE, NT_PARAM_UNSUPPORTED
              }; /* parameter errors */

//typedef double NT_AlignType; /* as used by nt_data_o */

/*                on termination        on error      on warning      on (coding related warning)
enum NTtalkLevel {NT_TERMINATION_ERROR = 0, NT_ERROR = 1, NT_WARNING = 3, NT_SMALL_WARNING = 5,
				  on status (low freq)  on (code related talk, as _uniqId() reports) and high freq
				  NT_TALK = 7,          NT_SMALL_TALK = 9};
*/

inline void NT_cerr(NTsize currentTalkLevel, NTsize talkLevel, string message)
{
    if (currentTalkLevel >= talkLevel) cerr << message << endl;
}

#define NT_CERR( L , M )  (NT_cerr( NT_TALKLEVEL, L , M ))

#ifndef M_PI /* pi */
#define M_PI           3.14159265358979323846  /* pi */
#endif /* pi */
#ifndef M_E
#define M_E            2.7182818284590452354   /* e */
#endif /* e */
#ifndef M_SQRT2
#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#endif /* sqrt(2) */

#define NT_SELF (*this)

template<class T> inline T NT_Max(T a, T b) {
    return a > b ? a : b;
}
template<class T> inline T NT_Min(T a, T b) {
    return a < b ? a : b;
}
template<class T> inline T NT_Abs(T x) {
    return (x) >= 0 ? (x) : -(x);
}
template<class T> inline void NT_Swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}



#endif /* _NT_TYPES_H_ */



