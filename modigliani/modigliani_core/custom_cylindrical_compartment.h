/**
 * @file custom_cylindrical_compartment.h
 * Custom_cylindrical_compartment class header
 * @author Ahmed Aldo Faisal &copy; created 26.3.2001
 *
 * @version 0.5
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

#ifndef _modigliani_core_custom_cylindrical_compartment_h_
#define _modigliani_core_custom_cylindrical_compartment_h_

/* Parent includes */
#include "cylindrical_compartment.h"
/* other includes */

namespace modigliani_core {

/**
 *  @short Custom_cylindrical_compartment class
 *  \bug unknown
 *  \warning unknown
 */
class Custom_cylindrical_compartment: public Cylindrical_compartment {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Custom_cylindrical_compartment(modigliani_base::Real length /* muMeter */,
			modigliani_base::Real diameter /* muMeter */, modigliani_base::Real cM /* muFarad/cm^2 */,
			modigliani_base::Real rA /* ohm cm */, modigliani_base::Real newTemperature = 6.3);
	Custom_cylindrical_compartment(
			const Custom_cylindrical_compartment & original);
	Custom_cylindrical_compartment & operator=(
			const Custom_cylindrical_compartment & right);
	modigliani_base::Real getVoltage();
	void setVoltage(modigliani_base::Real newVoltage);
	virtual ~Custom_cylindrical_compartment();
	/* ***  Methods              ***/
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	modigliani_base::Real voltage;
};
}
#endif /* _ntbp_custom_cylindrical_compartment.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */