/*
 * ntbp_transition_rate_matrix_o.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#include "Transition_rate_matrix.h"

NTBP_transition_rate_matrix_o::NTBP_transition_rate_matrix_o(
		mbase::Msize new_num_states, mbase::Real new_min, mbase::Real new_max, mbase::Real new_step) :
		min(new_min), max(new_max), step(new_step), num_states(new_num_states) {
	int length = (floor((max - min) / step + 0.5) + 1)	* num_states * num_states;
	_probMatrices = new mbase::Real[length];
	for (int i = 0; i< length; i++){
		_probMatrices[i]=0;
	}
}

NTBP_transition_rate_matrix_o::~NTBP_transition_rate_matrix_o(){
	delete[] _probMatrices;
}

void NTBP_transition_rate_matrix_o::setTransitionProbability(mbase::Real voltage,
		mbase::Msize start, mbase::Msize stop, mbase::Real probability) {
	mbase::Msize index = floor((voltage - min) / step + 0.5);
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

void NTBP_transition_rate_matrix_o::setTransitionProbability(mbase::Msize index,
		mbase::Msize start, mbase::Msize stop, mbase::Real probability) {
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

mbase::Msize NTBP_transition_rate_matrix_o::get_index(mbase::Real voltage){
	return (floor((voltage - min) / step +0.5));
}

mbase::Real NTBP_transition_rate_matrix_o::getTransitionProbability(mbase::Real voltage,
		mbase::Msize start, mbase::Msize stop) {
	return (_probMatrices[get_index(voltage) * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}

mbase::Real NTBP_transition_rate_matrix_o::getTransitionProbability(mbase::Msize index,
		mbase::Msize start, mbase::Msize stop) {
	return (_probMatrices[index * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}
