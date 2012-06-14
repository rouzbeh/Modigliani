/**
 * @file Transition_rate_matrix.h
 * Transition_rate_matrix class declaration
 * @author Ali Neishabouri <ali@neishabouri.net>
 */

#ifndef TRANSITION_RATE_MATRIX_H_
#define TRANSITION_RATE_MATRIX_H_

#include "mbase/types.h"
// Boost's multi array class can be slow, if the data is not accessed properly.
// See http://stackoverflow.com/questions/446866/boostmulti-array-performance-question

namespace mcore {
class Transition_rate_matrix {
public:
	Transition_rate_matrix(modigliani_base::Size numNewStates, modigliani_base::Real min,
			modigliani_base::Real max, modigliani_base::Real step);
	virtual ~Transition_rate_matrix();
	void setTransitionProbability(modigliani_base::Real voltage, modigliani_base::Size start,
			modigliani_base::Size stop, modigliani_base::Real probability);
	void setTransitionProbability(modigliani_base::Size index, modigliani_base::Size start,
			modigliani_base::Size stop, modigliani_base::Real probability);
	modigliani_base::Real getTransitionProbability(modigliani_base::Real voltage,
			modigliani_base::Size start, modigliani_base::Size stop);
	modigliani_base::Real getTransitionProbability(modigliani_base::Size index,
			modigliani_base::Size start, modigliani_base::Size stop);
	modigliani_base::Size get_index(modigliani_base::Real voltage);
private:
	//typedef boost::multi_array<double, 3> matrix_array_type;
	//typedef blitz::Array<double, 3> matrix_array_type;
	typedef modigliani_base::Real* matrix_array_type;
	matrix_array_type _probMatrices;

	modigliani_base::Real min;
	modigliani_base::Real max;
	modigliani_base::Real step;
	int num_states;
};
}

#endif /* TRANSITION_RATE_MATRIX_H_ */
