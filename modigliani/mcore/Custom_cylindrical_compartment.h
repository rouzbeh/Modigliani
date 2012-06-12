/**\file Custom_cylindrical_compartment.h - Custom_cylindrical_compartment class header
 * by Ahmed Aldo Faisal &copy; created 26.3.2001  
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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mcore_custom_cylindrical_compartment_h_
#define _mcore_custom_cylindrical_compartment_h_

/* Parent includes */
#include "Cylindrical_compartment.h"
/* other includes */

/** @short Custom_cylindrical_compartment class
 \bug unknown
 \warning unknown
 */
namespace mcore {
class Custom_cylindrical_compartment: public Cylindrical_compartment {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Custom_cylindrical_compartment(mbase::Real length /* muMeter */,
			mbase::Real diameter /* muMeter */, mbase::Real cM /* muFarad/cm^2 */,
			mbase::Real rA /* ohm cm */, mbase::Real newTemperature = 6.3);
	Custom_cylindrical_compartment(
			const Custom_cylindrical_compartment & original);
	Custom_cylindrical_compartment & operator=(
			const Custom_cylindrical_compartment & right);
	mbase::Real getVoltage();
	void setVoltage(mbase::Real newVoltage);
	virtual ~Custom_cylindrical_compartment();
	/* ***  Methods              ***/
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	mbase::Real voltage;
};
}
#endif /* _ntbp_custom_cylindrical_compartment.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
