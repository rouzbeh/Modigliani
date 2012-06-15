/**
 * @file vector.h
 * Real vector class
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * NetTrader - finance management, analysis and simulation system
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_VECTOR_H_
#define MODIGLIANI_MODIGLIANI_BASE_VECTOR_H_

#include "vector3.h"

#include <cmath>

#include "main.h"
#include "obj.h"

namespace modigliani_base {

/**    A n-dimensional real vector class with visualization capabilities
 */
class Vector : public Obj {
  public:
    Vector();
    explicit Vector(Size dim);
    Vector(Size dim, Real * elemVec);
    Vector(Size dim, float * elemVec);
    // if Real != double uncomment the below line
    // Vector(Size dim, double * elemVec);
    explicit Vector(Vector3 newVector3);
    Vector(Real a, Real b);
    Vector(Real a, Real b, Real c);
    Vector(Real a, Real b, Real c, Real d);
    Vector(const Vector &);
    Vector& operator=(Vector const &);
    ~Vector();
    /*        Methods            */
    /*            Operators        */
    /// addition C = A+B
    Vector operator+(const Vector &) const;
    /// subtraction C = A-B
    Vector operator-(const Vector &) const;
    /// scalar product r = A*B
    Real operator*(const Vector &) const;
    /// scalar (dot) product r = A*B
    Real Dot(const Vector right) const {
      return ((*this) * (right));
    }
    /// cosine of the angle between two Vector
    Real CosAng(const Vector right) {
      return ((*this) * (right) / (Length() * right.Length()));
    }
    /// Scaling, factor multiplication (from the RIGHT) C = A*alpha
    Vector operator*(Real) const;
    /// Access vector components 1..dimension slower and safer
    Real &operator()(int component) const;
    /// Access vector components 0..(dimension-1) fast and no range checking
    Real &operator[](int component) const;
    Real operator^(Vector b);
    operator Vector3() const;
    friend std::ostream & operator<<(std::ostream & os, const Vector & self);
    /*        Functions        */
    Vector BaseVectorAngle() const;
    inline Real Length() const {
      return (sqrt(LenSqr()));
    }
    ;
    Real LenSqr() const;
    Size _dim() const {
      return (dimension);
    }
    /*         Element Functions    */
    Size _dimension() const {
      return (dimension);
    }
    ;
    const Real* _elem() const;
    void set_elem(const Real * elemVec);
    /*        Data        */
  private:
    /*        Methods        */
    void Init(Size newDim);  /// < call only on first Init, or free elem before
    void set_dimension(Size newDim);
    /*        Data        */
    Real* elem;
    Size dimension;
};
}

#endif  // MODIGLIANI_MODIGLIANI_BASE_VECTOR_H_

