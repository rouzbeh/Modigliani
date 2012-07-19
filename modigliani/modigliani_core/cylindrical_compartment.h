/**
 * @file ntbp_cylindrical_compartment.h
 * Cylindrical_compartment class header
 * @author Ahmed Aldo Faisal &copy; created 26.3.2001  
 * NetTrader - visualisation, scientific and financial analysis and simulation system
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
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

#ifndef _modigliani_core_cylindrical_compartment_h_
#define _modigliani_core_cylindrical_compartment_h_

/* Parent includes */
#include "membrane_compartment.h"
/* NT includes */

/* other includes */

namespace modigliani_core {

/**
 *  @short Cylindrical_compartment class
 *  \bug unknown
 *  \warning unknown
 */
class Cylindrical_compartment: public Membrane_compartment {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Cylindrical_compartment(modigliani_base::Real length /* in muMeter */,
			modigliani_base::Real diameter /* in muMeter */, modigliani_base::Real newTemperature =
					6.3);
	Cylindrical_compartment(const Cylindrical_compartment & original);
	Cylindrical_compartment & operator=(const Cylindrical_compartment & right);
	virtual ~Cylindrical_compartment();
	/* ***  Methods              ***/
	/** length constant in muMeter for instantenous membrane conductance */
	modigliani_base::Real ActiveLengthConstant() {
		return (_diameter() / (4.0 * ra() * 1.0e4 * total_conductance()));
	}
	/* in muMeter^2 */
	modigliani_base::Real _CrosssectionalArea() const {
		return (_radius() * _radius() * M_PI);
	}
	/* in muMeter */
	modigliani_base::Real _diameter() const {
		return (diameter);
	}
	/* in muMeter */
	modigliani_base::Real _radius() const {
		return (_diameter() / 2.0);
	}
	/* in muMeter */
	modigliani_base::Real _length() const {
		return (length);
	}
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	modigliani_base::Real diameter; // in muMeter
	modigliani_base::Real length; // in muMeter
	/* ***  Data                 ***/
};
}
#endif /* _modigliani_core_cylindrical_compartment.h_ */

/* File skeleton generated by GenNTObj version 0.7. */