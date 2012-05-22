/** \file nt_vector3.h - M_solid_o class declaration
 * by Ahmed Aldo Faisal &copy; created  15.10.1998
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:  0.2
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

#ifndef _mbase_vector3_h_
#define _mbase_vector3_h_

#include "types.h"
#include "Obj.h"
#include "Error.h"

#include "aux_math_func.h"

#include <iostream>
/** @short Vector3 class
 Implementation of a three-dimensional
 real-valued vector. Useable it as a 2D vector, too; the third (Z)
 dimension defaults to zero.
 \warning All input angles (roationas) in degree!.
 \bug unknown
 */

namespace mbase {

class Vector3;

extern Vector3& M_UnitXVec3();
extern Vector3& M_UnitYVec3();
extern Vector3& M_UnitZVec3();
extern Vector3& M_UnitMinusZVec3();
extern Vector3& M_UnitXYZVec3();
extern Vector3& M_OriginVec3();

class Vector3: public Obj {
public:
	// Vector3 components
	Real x, y, z;

	/* ***	  CONSTRUCTORS          ***/
	Vector3() {
		x = y = z = 0;
	}

	Vector3(Real pX, Real pY, Real pZ) :
	x(pX), y(pY), z(pZ) {
	}

	/* ***      COPY AND ASSIGNMEM	***/
	Vector3(const Vector3 & original) :
	Obj(), x(original.x), y(original.y), z(original.z) {
	}

	const Vector3&
	operator=(const Vector3 & right) {
		if (this == &right)
			return (*this); // Gracefully handle self assignment
		// add assignment code here
		x = right.x;
		y = right.y;
		z = right.z;
		return (*this);
	}

	/* ***      DESTRUCTOR		***/
	~Vector3() {
	}

	// Vector3 arithmatic

	Vector3 operator+(const Vector3 & pV) const {
		Vector3 out(x + pV.x, y + pV.y, z + pV.z);
		return (out);
	}

	Vector3 operator-(const Vector3 & pV) const {
		Vector3 out(x - pV.x, y - pV.y, z - pV.z);
		return (out);
	}

	Vector3& operator+=(const Vector3 & pV) {
		x += pV.x;
		y += pV.y;
		z += pV.z;
		return (*this);
	}

	Vector3& operator-=(const Vector3 & pV) {
		x -= pV.x;
		y -= pV.y;
		z -= pV.z;
		return (*this);
	}

	/** cross product with another vector (returns a scalar) */
	Vector3 X(const Vector3 & pV) const {
		Vector3 out(y * pV.z - z * pV.y, z * pV.x - x * pV.z,
				x * pV.y - y * pV.x);
		return (out);
	}

	/** Access vector components 1..dimension slower and safer */
	Real operator()(Mint component) const {
		M_ASSERT_BOUNDS(component, 1, 3);
		switch (component) {
		case 1:
			return (x);
			break;
		case 2:
			return (y);
			break;
		case 3:
			return (z);
			break;
		default:
			return (0.0);
			break;
		}
		return (0.0);
	}

	/** Access vector components read/write */
	Real &
	operator[](Mint component) {
		switch (component) {
		case 0:
			return (x);
			break;
		case 1:
			return (y);
			break;
		case 2:
			return (z);
			break;
		default:
			return (x);
			break;
		}
		return (x);
	}

	/** Access vector components read-only */
	Real operator[](Mint component) const {
		switch (component) {
		case 0:
			return (x);
			break;
		case 1:
			return (y);
			break;
		case 2:
			return (z);
			break;
		default:
			return (x);
			break;
		}
		return (0);
	}

	Vector3 Cross(const Vector3 & pV) const {
		return ((*this).X(pV));
	}

	friend Vector3 Cross(const Vector3 & p1,
			const Vector3 & p2) {
		return (p1.X(p2));
	}

	/** dot product with another vector (returns a vector)  */
	Real operator*(const Vector3 & pV) const {
		return (x * pV.x + y * pV.y + z * pV.z);
	}

	Real Dot(const Vector3 & pV) const {
		return ((*this) * (pV));
	}

	friend Real Dot(const Vector3 & p1, const Vector3 & p2) {
		return (p1 * p2);
	}

	/**  scalar muliplication from the right */
	Vector3 operator*(const Real pR) const {
		Vector3 out(x * pR, y * pR, z * pR);
		return (out);
	}

	/**  scalar muliplication from the left */
	friend Vector3 operator*(const Real pR, const Vector3 pV) {
		Vector3 out(pV.x * pR, pV.y * pR, pV.z * pR);
		return (out);
	}

	/**  scalar muliplication from the right applied to *this */
	Vector3& operator*=(const Real pR) {
		x *= pR;
		y *= pR;
		z *= pR;
		return (*this);
	}

	/** Return a vector rotated around X, Y and then Z axis.
	 The corresponding rotation angles  phi, theta and psi are
	 given in degree. */
	Vector3 XYZrotation(const Vector3 & rotV) const;

	/** squared euclidean length of vector */
	Real LenSqr(void) const {
		return (x * x + y * y + z * z);
	}

	/** euclidean length of vector */
	Real Length(void) const {
		return (sqrt(LenSqr()));
	}

	/** Normalize vector
	 Normlises vector, or unmodified if length is 0
	 */
	void Norm() {
		Real tmpLen = Length();
		if (tmpLen > 0) {
			x /= tmpLen;
			y /= tmpLen;
			z /= tmpLen;
		}
	}

	/** Return A normalized version of vector */
	Vector3 Norm() const {
		Real tmpLen = Length();
		if (tmpLen > 0)
			return (Vector3(x / tmpLen, y / tmpLen, z / tmpLen));
		else
			return (M_OriginVec3());
	}

	/** projection (componeM of *this parallel to pV)
	 Note: componeM perpendicular to pV is:  *this - Proj(pV) */
	Vector3 Proj(const Vector3 & pV) {
		Vector3 out(pV * (this->Dot(pV) / pV.LenSqr()));
		return (out);
	}

	/** cosine of the angle between two Vector3s */
	Real CosAng(const Vector3 & pV) {
		return (this->Dot(pV) / (Length() * pV.Length()));
	}

	Real operator^(const Vector3 & right) {
		return (M_SELF.CosAng(right));
	}

	/** Angle between the 3D-unit bases and the vector
	 @return
	 Vector3 each component contains the angle between the
	 vector and the corresponding base vector */
	Vector3 BaseVectorAngle() const {
		Vector3 oDirection;
		for (Muint ll = 0; ll < 3; ll++) {
			Vector3 oUnit;
			oUnit[ll] = 1.0;
			oDirection[ll] = oUnit ^ (*this);
		}
		return (oDirection);
	}

	void set_elem(const Real * elemVec) {
		x = elemVec[0];
		y = elemVec[1];
		z = elemVec[2];
	}

	/** return the dimension, which is 3 */
	Muint _dimension() const {
		return (3);
	}
	/** return a pointer to a DELETABLE Real[3] */
	Real* _elem() const {
		Real * tmpPtr = new Real[3];
		tmpPtr[0] = x;
		tmpPtr[1] = y;
		tmpPtr[2] = z;
		return (tmpPtr);
	}

	/** comparison */
	Mbool operator==(const Vector3 & pV) const {
		return ((x == pV.x && y == pV.y && z == pV.z) ? M_TRUE : M_FALSE);
	}

	Mbool operator!=(const Vector3 & pV) const {
		return ((x != pV.x || y != pV.y || z != pV.z) ? M_TRUE : M_FALSE);
	}

	/** pretty print components */
	friend std::ostream& operator<<(std::ostream& pStr, const Vector3& pV) {
		return (pStr << '(' << pV.x << ',' << pV.y << ',' << pV.z << ')');
	}

	/** read components from stream pStr */
	friend std::istream& operator>>(std::istream pStr, Vector3& pV) {
		return (pStr >> pV.x >> pV.y >> pV.z);
	}
};

}
#endif /* _mbase_vector3_h_ */

