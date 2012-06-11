/**\file Ion_channels.h - Ion_channels class header
 * by Ahmed Aldo Faisal &copy; created 15.3.2001  
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

#ifndef _mcore_ion_channels_h_
#define _mcore_ion_channels_h_

/* Parent includes */
#include "Object.h"
/* NT includes */
#include "mbase/Uniform_rnd_dist.h"
#include "mbase/Binomial_rnd_dist.h"
#include "Transition_rate_matrix.h"
/* other includes */
#include <vector>
#include <iostream>

/** @short NTBP_ion_channels_o class
 \bug unknown
 \warning unknown
 */
namespace mcore{
class Ion_channels: public Object {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Ion_channels(mbase::Size_t numNewChannels, mbase::Size_t numNewStates, Transition_rate_matrix* probMatrix, mbase::Real newTimeStep=0.1);
	Ion_channels(const Ion_channels & original);
	const Ion_channels & operator=(const Ion_channels & right);
	virtual ~Ion_channels();
	/* ***  Methods              ***/
	mbase::Size_t _numChannels() const {
		return (numChannels);
	}
	mbase::Size_t _numStates() const {
		return (numStates);
	}
	mbase::Mreturn GillespieStep(mbase::Real voltage);
	void setAsOpenState(mbase::Size_t newOpenState);
	mbase::Mreturn BinomialStep(mbase::Real voltage);
	mbase::Mreturn DeterministicStep(mbase::Real voltage);
	mbase::Mreturn step(mbase::Real voltage);
	mbase::Mreturn ComputeGillespieStep(mbase::Size_t channelStateId, mbase::Real voltage);
	void ShowStates() const;
	mbase::Size_t NumOpen() const;
	mbase::Size_t numChannelsInState(mbase::Size_t state) const {
		return (stateCounterVec[state]);
	}
	mbase::Size_t NumClosed() const {
		return (_numChannels() - NumOpen());
	}
	virtual mbase::Mreturn SteadyStateDistribution(mbase::Real voltage);
	/** Sum of escape rates from current state [1/s] */
	virtual mbase::Real ComputeChannelStateTimeConstant(mbase::Real voltage) const;
	/**  */
	void setRatesComputed(bool newValue) {
		ratesComputed = newValue;
	}
	bool getRatesComputed() {
		return (ratesComputed);
	}
	/* ***  Data                 ***/

protected:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	// TODO remove this
	std::vector<mbase::Real> statePersistenceProbVec;
	std::vector<mbase::int_t> stateCounterVec;
	static mbase::Uniform_rnd_dist uniformRnd; // [0,1] random number generation
	static mbase::Binomial_rnd_dist binomRnd;

private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	Transition_rate_matrix* _probMatrix;
	std::vector<mbase::Size_t> openStates;
	mbase::Size_t numStates;
	mbase::Size_t numChannels;
	bool ratesComputed;
};
}
#endif /* _mcore_ion_channels.h_ */

/* File skeleton generated by GenNTObj version 0.7. */
