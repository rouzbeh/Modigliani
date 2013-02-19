/**
 * \file custom_cylindrical_compartment.h
 *
 * \brief This class is used to take concentrations into account
 *
 * \author Ahmed Aldo Faisal &copy; created 26.3.2001
 * \author Ali Neishabouri &copy; created 19.2.2013
 *
 * @version 1
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Ali Neishabouri
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

#include "cylindrical_compartment.h"

namespace modigliani_core {

using modigliani_base::Real;
/**
 * \brief Custom_cylindrical_compartment class
 */
class Custom_cylindrical_compartment : public Cylindrical_compartment {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Custom_cylindrical_compartment(
        const modigliani_base::Real length /* muMeter */,
        const modigliani_base::Real diameter /* muMeter */,
        const modigliani_base::Real cM /* muFarad/cm^2 */,
        const modigliani_base::Real rA /* ohm cm */,
        const modigliani_base::Real newTemperature);

    Custom_cylindrical_compartment(
        const Custom_cylindrical_compartment & original) = delete;

    Custom_cylindrical_compartment & operator=(
        const Custom_cylindrical_compartment & right) =delete;

    virtual ~Custom_cylindrical_compartment();

    /* ***  Methods              ***/
    modigliani_base::ReturnEnum AttachCurrentWithConcentrations(
        Membrane_current * currentPtr, Real concentration_inside, Real concentration_outside);
    /* ***  Data                 ***/
    protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
  };}
#endif /* _modigliani_core_custom_cylindrical_compartment.h_ */

