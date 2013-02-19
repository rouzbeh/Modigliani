/**
 * @file cylindrical_compartment.h
 * \brief Cylindrical_compartment class header
 *
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

namespace modigliani_core {

/**
 * \brief Cylindrical_compartment class
 *
 */
class Cylindrical_compartment : public Membrane_compartment {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
//	Cylindrical_compartment(modigliani_base::Real length /* in muMeter */,
//			modigliani_base::Real diameter /* in muMeter */, modigliani_base::Real newTemperature =
//					6.3);
    Cylindrical_compartment(const modigliani_base::Real newLength,
                            const modigliani_base::Real newDiameter,
                            const modigliani_base::Real newCm,
                            const modigliani_base::Real newRa,
                            const modigliani_base::Real newTemperature);

    Cylindrical_compartment(const Cylindrical_compartment & original) = delete;
    Cylindrical_compartment & operator=(const Cylindrical_compartment & right) = delete;

    virtual ~Cylindrical_compartment();

    /**
     * \brief length constant in muMeter for instantenous membrane conductance
     */
    modigliani_base::Real ActiveLengthConstant() const {
      return (diameter() / (4.0 * ra() * 1.0e4 * WeightedConductance()));
    }

    /* in muMeter^2 */
    modigliani_base::Real _CrosssectionalArea() const {
      return (radius() * radius() * M_PI);
    }

    /**
     * \return diameter in muMeter
     */
    modigliani_base::Real diameter() const {
      return (diameter_);
    }
    /* in muMeter */
    modigliani_base::Real radius() const {
      return (diameter() / 2.0);
    }
    /* in muMeter */
    modigliani_base::Real length() const {
      return (length_);
    }
    /* ***  Data                 ***/
    protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    /// in muMeter
    const modigliani_base::Real diameter_;
    /// in muMeter
    const modigliani_base::Real length_;
    /// in muMeter^3
    const modigliani_base::Real volume_;
    private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
  };}
#endif /* _modigliani_core_cylindrical_compartment.h_ */
