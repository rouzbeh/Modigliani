/* Vector.h - real vector class */
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mbase_vector_h_
#define _mbase_vector_h_

#include "main.h"
#include "Obj.h"

#include "Vector3.h"

#include <iostream>
//#include <math.h>
#include <cmath>
//
//	Class name : Vector
//
/**	A n-dimensional real vector class with visualization capabilities
 */
namespace mbase{
class Vector : public Obj {
public:
    Vector();
    Vector(Muint dim);
    Vector(Muint dim, Mreal * elemVec);
    Vector(Muint dim, float * elemVec);
    // if Mreal != double uncomment the below line
    // Vector(Muint dim, double * elemVec);
    Vector(Vector3 newVector3);
    Vector(Mreal a, Mreal b);
    Vector(Mreal a, Mreal b, Mreal c);
    Vector(Mreal a, Mreal b, Mreal c, Mreal d);
    Vector(const Vector &);
    Vector& operator=(Vector const &);
    ~Vector();
    /*		Methods			*/
    /*            Operators		*/
    //! addition C = A+B
    Vector operator+(const Vector &) const;
    //! subtraction C = A-B
    Vector operator-(const Vector &) const;
    //! scalar product r = A*B
    Mreal operator*(const Vector &) const;
    //! scalar (dot) product r = A*B
    Mreal Dot(const Vector right) const
    {
        return ((*this)*(right));
    }
    /** cosine of the angle between two Vector */
    Mreal CosAng( const Vector right)
    {
        return ((*this)*(right) / (Length() * right.Length()));
    }
    //! Scaling, factor multiplication (from the RIGHT) C = A*alpha
    Vector operator*(Mreal) const;
    //! Access vector components 1..dimension slower and safer
    Mreal &operator()(Mint component) const;
    //! Access vector components 0..(dimension-1) fast and no range checking
    Mreal &operator[](Mint component) const;
    Mreal operator^(Vector b);
    operator Vector3() const;
    friend std::ostream & operator<<(std::ostream & os, const Vector & self);
    /*		Functions		*/
    Vector BaseVectorAngle() const;
    inline Mreal Length() const {
        return (sqrt(LenSqr()));
    };
    Mreal LenSqr() const;
    Muint _dim() const {
        return (dimension);
    }
    /* 		Element Functions	*/
    Muint _dimension() const {
        return (dimension);
    };
    const Mreal* _elem() const;
    void set_elem(const Mreal * elemVec);
    /*		Data		*/
private:
    /*		Methods		*/
    void Init(Muint newDim);//!< call only on first Init, or free elem before
    void set_dimension(Muint newDim);
    /*		Data		*/
    Mreal* elem;
    Muint dimension;
};
}




#endif /* _mbase_vector_h_ */














