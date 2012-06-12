/**
 * @file types.h - type declarations for NetTrader
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

#ifndef _mbase_types_h_
#define _mbase_types_h_

#include <cstring>
#include <cmath>
#include <iostream>

namespace mbase {
typedef float Mfloat;
typedef double Mdouble;
typedef Mdouble Real;

typedef int int_t;
typedef unsigned int Muint;
typedef unsigned int Size_t;
typedef long int Mlong;
typedef Mlong Mnum;

typedef long int Mid;

typedef char Mchar;
typedef char* M_string;
typedef std::string M_string_o;

typedef bool Mbool; /* FALSE TRUE , used for state variables */
const Mbool M_FALSE = (Mbool) false;
const Mbool M_TRUE = (Mbool) true;

enum Mreturn {
	M_SUCCESS,
	M_FAIL,
	M_NOT_IMPLEMENTED,
	M_NOT_DERIVED, /* SUCCESS FAIL ... , used for return "error codes" of funcs */
	M_OUT_OF_MEMORY, /* memory operations */
	M_FILE_NOT_FOUND,
	M_WRONG_FILE_TYPE,
	M_BAD_FILE,
	M_EMPTY_FILE,
	M_BAD_DATA,
	M_READ_FAIL,
	M_NO_SPACE_LEFT,
	M_WRITE_FAIL, /* file op returns */
	M_NOT_EXISTING_NODE,
	M_NOT_EXISTING_EDGE, /* MG graph operations */
	M_PARAM_OUT_OF_RANGE,
	M_PARAM_UNSUPPORTED
};
/* parameter errors */

//typedef double M_AlignType; /* as used by nt_data_o */
/*                on termination        on error      on warning      on (coding related warning)
 enum MtalkLevel {M_TERMINATION_ERROR = 0, M_ERROR = 1, M_WARNING = 3, M_SMALL_WARNING = 5,
 on status (low freq)  on (code related talk, as _uniqId() reports) and high freq
 M_TALK = 7,          M_SMALL_TALK = 9};
 */

inline void M_cerr(Size_t currentTalkLevel, Size_t talkLevel, std::string message) {
	if (currentTalkLevel >= talkLevel)
		std::cerr << message << std::endl;
}

#define M_CERR( L , M )  (M_cerr( M_TALKLEVEL, L , M ))

#ifndef M_PI /* pi */
#define M_PI           3.14159265358979323846  /* pi */
#endif /* pi */
#ifndef M_E
#define M_E            2.7182818284590452354   /* e */
#endif /* e */
#ifndef M_SQRT2
#define M_SQRT2        1.41421356237309504880  /* sqrt(2) */
#endif /* sqrt(2) */

#define M_SELF (*this)

template<class T> inline T M_Max(T a, T b) {
	return (a > b ? a : b);
}
template<class T> inline T M_Min(T a, T b) {
	return (a < b ? a : b);
}
template<class T> inline T M_Abs(T x) {
	return ((x) >= 0 ? (x) : -(x));
}
template<class T> inline void M_Swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

}

#endif /* _M_TYPES_H_ */

