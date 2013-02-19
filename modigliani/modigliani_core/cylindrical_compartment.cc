/**
 * @file cylindrical_compartment.cc
 * Cylindrical_compartment class implementation
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

#include "cylindrical_compartment.h"

using namespace modigliani_core;

/* ***      CONSTRUCTORS	***/
/** Create a Cylindrical_compartment */
//Cylindrical_compartment::Cylindrical_compartment(
//    modigliani_base::Real newLength /* in muMeter */,
//    modigliani_base::Real newDiameter /* in muMeter */,
//    modigliani_base::Real newTemperature)
//    : Membrane_compartment(M_PI * newDiameter * newLength /* in muMeter^2 */,
//                           newTemperature) {
//
//}
Cylindrical_compartment::Cylindrical_compartment(
    const modigliani_base::Real newLength,
    const modigliani_base::Real newDiameter, const modigliani_base::Real newCm,
    const modigliani_base::Real newRa,
    const modigliani_base::Real newTemperature)
    : Membrane_compartment(M_PI * newDiameter * newLength /* in muMeter^2 */,
                           newTemperature, newCm, newRa), length_(newLength), diameter_(
        newDiameter), volume_(M_PI * M_PI * (newDiameter / 2) * newLength) {

}

/* ***      DESTRUCTOR		***/
Cylindrical_compartment::~Cylindrical_compartment() {
}
