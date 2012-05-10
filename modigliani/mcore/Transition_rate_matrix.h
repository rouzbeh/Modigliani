/*
 * ntbp_transition_rate_matrix.h
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_TRANSITION_RATE_MATRIX_OBJ_H_
#define NTBP_TRANSITION_RATE_MATRIX_OBJ_H_

#include "ntsrc/types.h"
// Boost's multi array class can be slow, if the data is not accessed properly.
// See http://stackoverflow.com/questions/446866/boostmulti-array-performance-question

class NTBP_transition_rate_matrix_o {
public:
	NTBP_transition_rate_matrix_o(mbase::Msize numNewStates, mbase::Mreal min, mbase::Mreal max,
			mbase::Mreal step);
	virtual ~NTBP_transition_rate_matrix_o();
	void setTransitionProbability(mbase::Mreal voltage, mbase::Msize start, mbase::Msize stop,
			mbase::Mreal probability);
	void setTransitionProbability(mbase::Msize index, mbase::Msize start, mbase::Msize stop,
				mbase::Mreal probability);
	mbase::Mreal getTransitionProbability(mbase::Mreal voltage, mbase::Msize start, mbase::Msize stop);
	mbase::Mreal getTransitionProbability(mbase::Msize index, mbase::Msize start, mbase::Msize stop);
	mbase::Msize get_index(mbase::Mreal voltage);
private:
	//typedef boost::multi_array<double, 3> matrix_array_type;
	//typedef blitz::Array<double, 3> matrix_array_type;
	typedef mbase::Mreal* matrix_array_type;
	matrix_array_type _probMatrices;

	mbase::Mreal min;
	mbase::Mreal max;
	mbase::Mreal step;
	int num_states;
};

#endif /* NTBP_TRANSITION_RATE_MATRIX_OBJ_H_ */
