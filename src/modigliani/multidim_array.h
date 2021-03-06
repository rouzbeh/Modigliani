/**
 * @file multidim_array.h
 * @brief Multidim_array class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_BASE_MULTIDIM_ARRAY_H_
#define MODIGLIANI_MODIGLIANI_BASE_MULTIDIM_ARRAY_H_

#include <vector>
#include <cstdarg>

#include "modigliani/types.h"
#include "modigliani/obj.h"
#include "modigliani/error.h"

namespace modigliani {
  /**
   * @brief  Multidimensional array
   *
   * Implements a regular - multidimensional, i.e.
   * a "dim"-diemsional hypercube with "num" elements per dimension.
   */
  template < class T >          // with regards to Modula-3 :)
      class Multidim_array:public Obj {
 public:
    /**
     * @brief Constructs a multidim array
     * @param d Number of dimensions
     * @param n Number of elements
     */
    Multidim_array(Size d, Size n) {
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
      for (Size ll = 1; ll < dim + 1; ll++) {
        powerSeriesCacheVec[ll] = num * powerSeriesCacheVec[ll - 1];
        std::cerr << powerSeriesCacheVec[ll] << std::endl;
      }
      // memory allocation
      try {
        dataVec.resize(powerSeriesCacheVec[dim]);
      }
      catch(std::bad_alloc & ba) {
        std::cerr
          <<
          "M_multidim_array_o::M_multidim_array_o -"
          << " Error : Memory exhausted by allocation"
          << " of multidimensional array."
          << ">" << powerSeriesCacheVec[dim] * sizeof(T)
          << " bytes of memory are necessary for this object." << std::endl;
      }
    }

    Multidim_array(const Multidim_array & original) = delete;

    const Multidim_array & operator=(const Multidim_array & right) = delete;

    /**
     * @brief Destructor
     */
    virtual ~Multidim_array() {
    }

    /**
     * @brief Access element at given coordinate position
     * @param coordinateVec Vector of coordinates
     * @return Element at the index
     * @warning No range checking done on the coordinates
     */
    T & Elem(const std::vector < Size > &coordinateVec) {
      Size hash = Hash(coordinateVec);
      M_ASSERT(dataVec.size() >= hash);
      return (dataVec[hash]);
    }

    /**
     * @brief access a element with a "meaningless" index
     *
     * The index is guaranteed to touch all elements but has no
     * assigned meaning as to the position of element in relation to
     * others. However index is guaranteed to be in @f$[0,num^dim]@f$
     * and will touch all elements once.
     * @param index The index
     * @return     The element at given index
     * @warning Arbitrary index, no information inferable from the
     * index, might change anytime (only range remains guaranteed).
     */
    const T & ElemByIndex(Size index) const {
      M_ASSERT((index < TotalNumElem()));
      return (dataVec[index]);
    }

    /**
     * @brief Sets all the elements to the given input
     * @param val Input value
     */
    void SetAll(const T & val) {
      for (Size ll = 0; ll < TotalNumElem(); ll++) {
        dataVec[ll] = val;
      }
    }

    /**
     * @brief Access element at given coordinate position
     * @param index1 Indexes
     * @return Element
     * @warning No range checking done on the coordinates
     */
    T & Elem(Size index1 ...) {
      std::vector < Size > coorVec(dim);
      va_list args;
      va_start(args, index1);
      for (Size ld = 0; ld < dim; ld++) {
        coorVec[ld] = va_arg(args, Size);
      }
      va_end(args);
      return (Elem(coorVec));
    }

    /**
     * @brief Returns total number of elements in the array
     * @return Number of elements
     */
    Size TotalNumElem()const {
      return (powerSeriesCacheVec[dim]);
    }

  private:
    /**
     * @brief Generate an address out of the coordinates of the
     * element in the multimensional array.
     *
     * The hash address is the decimal representation of the num-ary
     * dim-digit coordinate "number".
     * @warning range of coordinates values (0..num-1) is not tested
     */
     Size Hash(const std::vector < Size > &coorVec) const {
      M_ASSERT(coorVec.size() == dim);
      Size addressIndex = 0;
      for (Size ld = 0; ld < dim; ld++) {
        // TODO(Ali) WHY was ld < num+1 ?!?!?!
        addressIndex += coorVec[ld] * powerSeriesCacheVec[ld];
      } return (addressIndex);
    }
    
    std::vector < Size > dataVec;
    std::vector < Size > powerSeriesCacheVec;
    Size dim;
    Size num;  // number of elements per dimension
  };
}   // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_BASE_MULTIDIM_ARRAY_H_
