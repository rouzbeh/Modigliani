/**
 * @file Membrane_patch.h
 * Membrane_patch_o class header
 * @author Ahmed Aldo Faisal &copy; created 21.1.2002  
 * @version   0.5
 * Copyright (C) 1998,1999,2000,2001,2002 Ahmed Aldo Faisal
 *
 * @section LICENSE
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

#ifndef _mcore_membrane_patch_h_
#define _mcore_membrane_patch_h_

/* NT includes */
#include "Membrane_compartment.h"
#include "Object.h"
/* other includes */

namespace mcore {

/** @short Membrane_patch_o class
 \bug unknown
 \warning unknown
 */
class Membrane_patch: public Membrane_compartment {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Membrane_patch(modigliani_base::Real area /* [muM^2] */, modigliani_base::Real newCM /* muF/cm^2 */);
	Membrane_patch(const Membrane_patch & original) = delete;
	Membrane_patch & operator=(const Membrane_patch & right) = delete;
	virtual ~Membrane_patch();
	/* ***  Methods              ***/
	modigliani_base::Real MembranePotential() const {
		return (vm());
	}
	/** No descriptions */
	modigliani_base::ReturnEnum Step();
	/** No descriptions */
	modigliani_base::ReturnEnum InitialStep();
	/** Current [nA]  */
	modigliani_base::Real MembraneCurrent(modigliani_base::Size currentIndex) const {
		M_ASSERT((currentIndex > 0) && (currentIndex-1 < NumberCurrents()));
		return (GetCurrent(currentIndex)->current());
	}
	/* in muMeter^2 */

	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
};
}
#endif /* _mcore_membrane_patch.h_ */

/* File skeleton generated by GenNTObj version 0.7. */
