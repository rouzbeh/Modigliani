/**
 * @file vector.cc
 * Real vector class
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

#include "vector.h"
#include "memory.h"

using namespace modigliani_base;

//    Method name : Init
//
//    Description : set dimension and allocate memory
//    Input :
//    Output :
//      NOTE : use only on fresh objects or if elem was already freed
//
void Vector::Init(Size newDim) {
    dimension = newDim;
    elem = (Real*) new Real[dimension];
    for (Size ll = 0; ll < dimension; ll++)
        elem[ll] = 0.0;
}

//
//    Method name : Vector
//
//    Description : CONSTRUCTORS
//    Input :
//    Output :
//
Vector::Vector() {
    Init(1);
}

Vector::Vector(Size newDim) {
    Init(newDim);
}

Vector::Vector(Size dim, Real * elemVec) {
    Init(dim);
    for (Size i = 0; i < dimension; i++)
        elem[i] = elemVec[i];
}

Vector::Vector(Size dim, float * elemVec) {
    Init(dim);
    for (Size i = 0; i < dimension; i++)
        elem[i] = elemVec[i];
}

/* if Real != double uncomment the below lines
 Vector::Vector(Size dim, double * elemVec){
 Init(dim);
 for (Size i = 0; i < dimension; i++) elem[i] = elemVec[i];
 }
 */

Vector::Vector(Vector3 newVec3) {
    Init(3);
    elem[0] = newVec3.x;
    elem[1] = newVec3.y;
    elem[2] = newVec3.z;
}

Vector::Vector(Real a, Real b) {
    Init(2);
    elem[0] = a;
    elem[1] = b;
}

Vector::Vector(Real a, Real b, Real c) {
    Init(3);
    elem[0] = a;
    elem[1] = b;
    elem[2] = c;
}

Vector::Vector(Real a, Real b, Real c, Real d) {
    Init(4);
    elem[0] = a;
    elem[1] = b;
    elem[2] = c;
    elem[3] = d;
}

Vector::Vector(const Vector &right) :
        Obj() {
    dimension = right.dimension;
    elem = (Real*) new Real[dimension];
    for (Size i = 0; i < dimension; i++)
        elem[i] = right.elem[i];
}

//    Method name : ~Vector
//
//    Description : DESTRUCTOR
//    Input :
//    Output :
//  \Bug Always a source of annoying crashes with delete([]) and gcc 2.95 (linux)
//       commenting it out often helps.
Vector::~Vector() {

    //cerr << "[" << dimension << flush;
    if (dimension < 2)
        delete elem;
    else
        delete[] elem;
    //cerr << "]" << std::endl;

}

Vector::operator Vector3() const {
    Vector3 tmp;
    if (dimension == 3) {
        tmp.x = elem[0];
        tmp.y = elem[1];
        tmp.z = elem[2];
    } else if (dimension == 2) {
        std::cerr
                << "Vector3(const Vector M_SELF) - warning : call with M_SELF of dimension="
                << _dimension()
                << " < 3. Copied components, and set missing ones to 0.0."
                << std::endl;
        tmp.x = elem[0];
        tmp.y = elem[1];
        tmp.z = 0.0;
    } else if (dimension == 1) {
        std::cerr
                << "Vector3(const Vector M_SELF) - warning : call with M_SELF of dimension="
                << _dimension()
                << " < 3. Copied components, and set missing ones to 0.0."
                << std::endl;
        tmp.x = elem[0];
        tmp.y = 0.0;
        tmp.z = 0.0;
    } else {
        std::cerr
                << "Vector3(const Vector M_SELF) - warning : call with M_SELF of dimension="
                << _dimension() << " > 3. Copied first three compnonents."
                << std::endl;
        tmp.x = elem[0];
        tmp.y = elem[1];
        tmp.z = elem[2];
    }
    return (tmp);
}

// Access vector components 1..dimension slower and safer
Real &
Vector::operator()(int component) const {
    M_ASSERT_BOUNDS(component, 1, dimension);
    return (elem[component - 1]);
}

// Access vector components 0..(dimension-1)
Real &
Vector::operator[](int component) const {
    return (elem[component]);
}

// Arithmetics
Vector Vector::operator+(const Vector &right) const {
    Vector left(*this);
    for (Size i = 0; i < dimension; i++)
        left.elem[i] = left.elem[i] + right.elem[i];
    return (left);
}

Vector Vector::operator-(const Vector &right) const {
    Vector left(*this);
    for (Size i = 0; i < dimension; i++) {
        left.elem[i] = left.elem[i] - right.elem[i];
    }
    return (left);
}

Real Vector::operator*(const Vector &right) const {
    Real result = 0.0;
    for (Size i = 0; i < dimension; i++) {
        result += elem[i] * right[i];
    }
    return (result);
}

// scalar multiplication from the RIGHT side
Vector Vector::operator*(Real alpha) const {
    Vector left(*this);
    for (Size i = 0; i < dimension; i++) {
        left[i] = alpha * elem[i];
    }
    return (left);
}

Vector &
Vector::operator=(Vector const &right) {
    M_ASSERT_DIM(dimension, right.dimension);
    if (this == &right)
        return (*this);
    else {
        if (dimension == 1)
            delete elem;
        else
            delete[] elem;
        dimension = right.dimension;
        elem = new Real[dimension];

        memcpy(elem, right.elem, dimension * sizeof(Real));

        return (*this);
    }
}

/** Angle between the (dimension)-unit bases and the vector
 @return
 Vector[dimension] each component contains the angle between the
 vector and the corresponding base vector */
Vector Vector::BaseVectorAngle() const {
    Vector oDirection(dimension);
    for (Size ll = 0; ll < dimension; ll++) {
        Vector oUnit(dimension);
        oUnit[ll] = 1.0;
        oDirection[ll] = oUnit ^ (*this);
    }
    return (oDirection);
}

Real Vector::LenSqr() const {
    Real len = 0.0;
    for (Size ll = 0; ll < dimension; ll++)
        len += elem[ll] * elem[ll];
    return(len);
}

/** return pointer to element vector.
 \warning The constant pointer to Real[dimension] should
 not be modified by any reason.
 */
const Real*
Vector::_elem() const {
    return ((const Real*) elem);
}

void Vector::set_elem(const Real * elemVec) {
    for (Size i = 0; i < dimension; i++)
        elem[i] = elemVec[i];
}

void Vector::set_dimension(Size newDim) {
    // save old entries and create new vector components
    Vector tmpVec = *this;
    delete elem;
    elem = new Real[newDim];

    // copy old entries into vector forall i < newDim
    for (Size i = 0; i < newDim; i++)
        elem[i] = tmpVec[i];
    // fill up empy components i=oldDim..newDim with 0.0
    if (dimension < newDim) {
        for (Size ll = newDim; ll < dimension; ll++)
            elem[ll] = 0.0;
    }
    // set new dimension of vector
    dimension = newDim;

}

std::ostream & operator<<(std::ostream & os, const Vector & self) {
    os << "Vector" << M_SEPARATOR << self._dimension();
    for (Size ll = 0; ll < self._dimension(); ll++) {
        os << M_SEPARATOR << self[ll];
    }
    os << std::endl;
    return (os);
}

Real Vector::operator^(Vector right) {
    return (CosAng(right));
}

