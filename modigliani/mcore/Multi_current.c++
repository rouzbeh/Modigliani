/**\file Multi_currentbj.cpp - Multi_current class implementation
 * by Ahmed Aldo Faisal &copy; created 16.3.2001
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

#include "Multi_current.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Multi_current */
Multi_current::Multi_current(
	mbase::Mreal newReversalPotential, //in mV
	mbase::Mreal newDensity, // channels per mumeter^2
	mbase::Mreal newArea, // in mumeter^2
	mbase::Mreal newConductivity, // in mSiemens per channel
	mbase::Mreal newVBase // mV
)
:
Membrane_current(newReversalPotential)
{
	density = newDensity;
	area = newArea;
	conductivity = newConductivity;
	numChannels = area*density;
	vBase = newVBase;
}

/* ***      COPY AND ASSIGNMENT	***/
Multi_current::Multi_current(const Multi_current & original)
:
Membrane_current(original._reversalPotential())
{
 // add assignment code here
}

const Multi_current&
Multi_current::operator= (const Multi_current & right)
{
 if (this == &right) return (*this); // Gracefully handle self assignment
 // add assignment code here
  return (*this);
}

/* ***      DESTRUCTOR		***/
Multi_current::~Multi_current()
{
}

/* ***  PUBLIC                                    ***   */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
