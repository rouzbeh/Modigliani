/*
 * ntbp_transition_rate_matrix.h
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_TRANSITION_RATE_MATRIX_OBJ_H_
#define NTBP_TRANSITION_RATE_MATRIX_OBJ_H_

#include "mbase/types.h"
// Boost's multi array class can be slow, if the data is not accessed properly.
// See http://stackoverflow.com/questions/446866/boostmulti-array-performance-question

class NTBP_transition_rate_matrix_o {
public:
	NTBP_transition_rate_matrix_o(mbase::Size_t numNewStates, mbase::Real min, mbase::Real max,
			mbase::Real step);
	virtual ~NTBP_transition_rate_matrix_o();
	void setTransitionProbability(mbase::Real voltage, mbase::Size_t start, mbase::Size_t stop,
			mbase::Real probability);
	void setTransitionProbability(mbase::Size_t index, mbase::Size_t start, mbase::Size_t stop,
				mbase::Real probability);
	mbase::Real getTransitionProbability(mbase::Real voltage, mbase::Size_t start, mbase::Size_t stop);
	mbase::Real getTransitionProbability(mbase::Size_t index, mbase::Size_t start, mbase::Size_t stop);
	mbase::Size_t get_index(mbase::Real voltage);
private:
	//typedef boost::multi_array<double, 3> matrix_array_type;
	//typedef blitz::Array<double, 3> matrix_array_type;
	typedef mbase::Real* matrix_array_type;
	matrix_array_type _probMatrices;

	mbase::Real min;
	mbase::Real max;
	mbase::Real step;
	int num_states;
};

#endif /* NTBP_TRANSITION_RATE_MATRIX_OBJ_H_ */
