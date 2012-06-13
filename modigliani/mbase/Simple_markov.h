/**
 * @file Simple_markov.h
 * Simple_markov class header
 * @author Ahmed Aldo Faisal &copy; created 14.3.2001
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

/* $Id: nt_simple_markov.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_simple_markov.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *

 */
#ifndef _mbase_simple_markov_h_
#define _mbase_simple_markov_h_

/* M core includes */
#include "main.h"
#include "types.h"
#include "Obj.h"
/* M includes */
#include "nt_rnd_dist.h"
#include "nt_uniform_rnd_dist.h"

/* other includes */
#include <tnt/tnt.h>
#include <tnt/tnt_cmat.h>

using TNT::Matrix;

namespace mbase {
/** @short M_simple_markov_o class
 Basic numState-dimensional 1st order markov
 state model. The transitions-probability matrix is
 set up for sharing among multiple instances of the class
 (hence no delete management of ptrPMtr)
 \bug unknown
 \warning ptrPMtr is not deleted!
 */
class Simple_markov: public Obj {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Simple_markov(int_t numMarkovStates = 1);
	Simple_markov(const Simple_markov & original);
	const Simple_markov & operator=(const Simple_markov & right);
	virtual ~Simple_markov();
	/* ***  Methods              ***/
	Mreturn MarkovStep();
	int_t _stateIndex() const {
		return (stateIndex);
	}
	Mreturn SetProbTransMatrix(Matrix<Real> * ptrNewMtr);
	Mreturn SetState(Size_t newStateIndex);
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	Matrix<Real> * ptrTPMtr; //Transition-probability matrix
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	int_t stateIndex; // index of state 1...numStates
	Size_t numStates;
	Uniform_rnd_dist random;
	bool mtrAssigned;
};
}
#endif /* _mbase_simple_markov_h_ */

/* File skeleton generated by GenMObj version 0.7. */
