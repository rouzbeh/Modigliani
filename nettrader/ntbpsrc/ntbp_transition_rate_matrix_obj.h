/*
 * ntbp_transition_rate_matrix_obj.h
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_TRANSITION_RATE_MATRIX_OBJ_H_
#define NTBP_TRANSITION_RATE_MATRIX_OBJ_H_

#include "nt_types.h"
// Boost's multi array class can be slow, if the data is not accessed properly.
// See http://stackoverflow.com/questions/446866/boostmulti-array-performance-question

class NTBP_transition_rate_matrix_o {
public:
	NTBP_transition_rate_matrix_o(NTsize numNewStates, NTreal min, NTreal max,
			NTreal step);
	virtual ~NTBP_transition_rate_matrix_o();
	void setTransitionProbability(NTreal voltage, NTsize start, NTsize stop,
			NTreal probability);
	NTreal getTransitionProbability(NTreal voltage, NTsize start, NTsize stop);
	NTreal getTransitionProbability(NTsize index, NTsize start, NTsize stop);
	NTsize get_index(NTreal voltage);
private:
	//typedef boost::multi_array<double, 3> matrix_array_type;
	//typedef blitz::Array<double, 3> matrix_array_type;
	typedef NTreal* matrix_array_type;
	matrix_array_type _probMatrices;

	NTreal min;
	NTreal max;
	NTreal step;
	int num_states;
};

#endif /* NTBP_TRANSITION_RATE_MATRIX_OBJ_H_ */
