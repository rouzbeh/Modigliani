/**\file Multidim_array.h - Multidim_array class header
 * by Ahmed Aldo Faisal &copy; created 28.6.2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
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

#ifndef _mbase_multidim_array_h_
#define _mbase_multidim_array_h_

#include "types.h"
#include "Obj.h"
#include "Error.h"

/* other includes */
#include <vector>
#include <cstdarg>

namespace mbase {
/** @short Multidim_array class
 Implements a regular - multidimensional, i.e.
 a "dim"-diemsional hypercube with "num" elements per dimension.
 \bug unknown
 \warning unknown
 */
template<class T> // with regards to Modula-3 :)
class Multidim_array: public Obj {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Multidim_array(Size_t d, Size_t n) {
		M_ASSERT(d > 0);
		dim = d;
		M_ASSERT(n > 0);
		num = n;
		// implementation range checking
		/* 2DO not working as numeric_limits not implemented in gcc 2.95
		 Real numElem = pow(n,d);
		 Real maxIndexRange = numeric_limits<Size_t>::max();
		 if (numElem >= maxIndexRange) {
		 std::cerr <<
		 "M_multidim_array_o::M_multidim_array_o - Error : array will contain to many elements"
		 << numElem << " for supported implementation range "
		 << maxIndexRange << " ("<< numeric_limits<Size_t>.max()
		 << "). Undefined behaviour may result." << std::endl;
		 }
		 */
		// cache setup
		powerSeriesCacheVec.resize(dim + 1);
		// = {1,num,num^2,...,num^dim}
		powerSeriesCacheVec[0] = 1;
		for (Size_t ll = 1; ll < dim + 1; ll++) {
			powerSeriesCacheVec[ll] = num * powerSeriesCacheVec[ll - 1];
			std::cerr << powerSeriesCacheVec[ll] << std::endl;
		}
		// memory allocation
		try {
			dataVec.resize(powerSeriesCacheVec[dim]);
		} catch (std::bad_alloc& ba) {
			std::cerr
					<< "M_multidim_array_o::M_multidim_array_o - Error : Memory exhausted by allocation of multidimensional array."
					<< ">" << powerSeriesCacheVec[dim] * sizeof(T)
					<< " bytes of memory are necessary for this object."
					<< std::endl;
		}
	}

	/* ***      COPY AND ASSIGNMEM	***/
	/** copy constructor currently not to be implemented. */
	Multidim_array(const Multidim_array & original) {
		std::cerr
				<< "Multidim_array::(const Multidim_array) - Error : Not implemented. Not to be supported ?"
				<< std::endl;
	}

	/** assignment operator currently not to be implemented. */
	const Multidim_array&
	operator=(const Multidim_array & right) {
		if (this == &right)
			return (*this); // Gracefully handle self assignment
		std::cerr
				<< "Multidim_array::operator= - Error : Not implemented. Not to be supported ?"
				<< std::endl;
		return (*this);
	}

	/* ***      DESTRUCTOR		***/
	virtual ~Multidim_array() {
	}

	/* ***  PUBLIC                                    ***   */
	/** @short	access element at given coordinate position
	 @param      none
	 @return     none
	 \warning    no range checking done on the coordinates
	 \bug        unknown
	 */
	T &
	Elem(const std::vector<Size_t> & coordinateVec) {
		Size_t hash = Hash(coordinateVec);
		M_ASSERT(dataVec.size() >= hash);
		return (dataVec[hash]);
	}

	/** @short	access a element with a "meaningless" index
	 @param     An index that is guaranteed to touch all elements
	 but has no assigned meaning as to the position of
	 element in relation to others. However
	 index $\in [0,num^dim]$ is guaranteed and will touch
	 all elements ones.
	 @return     none
	 \warning    Arbitrary index, no information inferable from the index,
	 might change anytime (only range remains guaranteed).
	 \bug        unknown
	 */
	const T &
	ElemByIndex(Size_t index) const {
		M_ASSERT((index < TotalNumElem()));
		return (dataVec[index]);
	}

	void SetAll(const T & val) {
		for (Size_t ll = 0; ll < TotalNumElem(); ll++) {
			dataVec[ll] = val;
		}
	}

	/** @short	access element at given coordinate position
	 @param      none
	 @return     none
	 \warning    no range checking done on the coordinates
	 \bug        unknown
	 */
	T &
	Elem(Size_t index1...)
	{
		std::vector <Size_t> coorVec(dim);
		va_list args;
		va_start(args,index1);
		for (Size_t ld=0; ld < dim; ld++) {
			coorVec[ld] = va_arg(args, Size_t);
		}
		va_end(args);
		return (Elem(coorVec));
	}

	Size_t
	TotalNumElem() const
	{
		return (powerSeriesCacheVec[dim]);
	}

	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/** Generate an address out of the coordinates of the element
	 in the multimensional array. The hash address is the
	 decimal representation of the num-ary dim-digit coordinate "number".
	 \warning range of coordinates values (0..num-1) is not tested
	 */
	Size_t Hash(const std::vector <Size_t> & coorVec) const
	{
		M_ASSERT( coorVec.size() == dim );
		Size_t addressIndex = 0;
		for (Size_t ld = 0; ld < dim; ld++) { //2DO  WHY was ld < num+1 ?!?!?!
				addressIndex += coorVec[ld] * powerSeriesCacheVec[ld];
			}
			return (addressIndex);
		}
		/* ***  Data                 ***/
		std::vector <Size_t> dataVec;
		std::vector <Size_t> powerSeriesCacheVec;
		Size_t dim;
		Size_t num; // number of elements per dimension

	};

	}

#endif /* _Multidim_array_h_ */
