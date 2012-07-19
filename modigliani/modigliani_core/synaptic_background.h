/**
 * @file synaptic_background.h
 * Synaptic_background class header
 * @author Ahmed Aldo Faisal &copy; created 20.8.2003  
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
 * License alhttp://npg.nature.com/npg/servlet/Content?data=xml/05_home.xml&style=xml/05_home.xslong with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _modigliani_core_synaptic_background_h_
#define _modigliani_core_synaptic_background_h_

/* Parent includes */
#include "membrane_current.h"
/* NT includes */
#include "modigliani_base/gaussian_rnd_dist.h"

/* other includes */

namespace modigliani_core {

/** @short Synaptic_background_o class
 \bug unknown
 \warning unknown
 */
class Synaptic_background: public Membrane_current {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Synaptic_background(modigliani_base::Real avgSynBkCond, /* in nS */
	modigliani_base::Real eSynBk, /* Synaptic background reversal potentialin mV (with vRest = vBase */
	modigliani_base::Real synSigma, /* Synaptic noise "diffusion" nS */
	modigliani_base::Real synTau, /* Synaptic time constant */
	modigliani_base::Real vBase = -65.0 /* in mV */
	);
	Synaptic_background(const Synaptic_background & original);
	Synaptic_background & operator=(const Synaptic_background & right);

	modigliani_base::ReturnEnum step_current()
	;
	/** compute and return conductance in mSiemens */
	modigliani_base::Real compute_conductance() ;
	virtual ~Synaptic_background();
	/* ***  Methods              ***/
	/* ***  Data                 ***/
protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	modigliani_base::Real avgSynBkCond; /* ns */
	modigliani_base::Real synSigma; /* ns */
	modigliani_base::Real synVar; /* ns^2 */
	modigliani_base::Real synTau; /* ms */
	modigliani_base::Real vBase; /* mV */
	modigliani_base::Real gT; /* nS */
	modigliani_base::Real amplitude;/* nS */
	modigliani_base::Gaussian_rnd_dist normRnd;
};}
#endif /* _modigliani_core_synaptic_background.h_ */

/* File skeleton generated by GenNTObj version 0.7. */