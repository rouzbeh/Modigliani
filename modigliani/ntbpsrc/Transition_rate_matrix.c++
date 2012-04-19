/*
 * ntbp_transition_rate_matrix_o.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#include "Transition_rate_matrix.h"

NTBP_transition_rate_matrix_o::NTBP_transition_rate_matrix_o(
		NTsize new_num_states, NTreal new_min, NTreal new_max, NTreal new_step) :
		min(new_min), max(new_max), step(new_step), num_states(new_num_states) {
	int length = (floor((max - min) / step + 0.5) + 1)	* num_states * num_states;
	_probMatrices = new NTreal[length];
	for (int i = 0; i< length; i++){
		_probMatrices[i]=0;
	}
}

NTBP_transition_rate_matrix_o::~NTBP_transition_rate_matrix_o(){
	delete[] _probMatrices;
}

void NTBP_transition_rate_matrix_o::setTransitionProbability(NTreal voltage,
		NTsize start, NTsize stop, NTreal probability) {
	NTsize index = floor((voltage - min) / step + 0.5);
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

void NTBP_transition_rate_matrix_o::setTransitionProbability(NTsize index,
		NTsize start, NTsize stop, NTreal probability) {
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

NTsize NTBP_transition_rate_matrix_o::get_index(NTreal voltage){
	return (floor((voltage - min) / step +0.5));
}

NTreal NTBP_transition_rate_matrix_o::getTransitionProbability(NTreal voltage,
		NTsize start, NTsize stop) {
	return (_probMatrices[get_index(voltage) * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}

NTreal NTBP_transition_rate_matrix_o::getTransitionProbability(NTsize index,
		NTsize start, NTsize stop) {
	return (_probMatrices[index * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}
