/*
 * ntbp_transition_rate_matrix_obj.h
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_TRANSITION_RATE_MATRIX_OBJ_H_
#define NTBP_TRANSITION_RATE_MATRIX_OBJ_H_

#include "nt_types.h"
#include "boost/multi_array.hpp"

class NTBP_transition_rate_matrix_o {
public:
	NTBP_transition_rate_matrix_o(NTsize numNewStates, NTreal min, NTreal max,
			NTreal step);
	virtual ~NTBP_transition_rate_matrix_o();
	void setTransitionProbability(NTreal voltage, NTsize start, NTsize stop,
			NTreal probability);
	NTreal getTransitionProbability(NTreal voltage, NTsize start, NTsize stop);
private:
	typedef boost::multi_array<double, 3> matrix_array_type;
	matrix_array_type _probMatrices;
	NTreal getTransitionProbability(NTsize index, NTsize start, NTsize stop);

	NTreal min;
	NTreal max;
	NTreal step;
};

#endif /* NTBP_TRANSITION_RATE_MATRIX_OBJ_H_ */
