/*
 * ntbp_transition_rate_matrix_o.cpp
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#include "ntbp_transition_rate_matrix_obj.h"

NTBP_transition_rate_matrix_o::NTBP_transition_rate_matrix_o(
		NTsize numNewStates, NTreal min, NTreal max, NTreal step) :
			_probMatrices(
					boost::extents[floor((max - min) / step + 0.5)+1][numNewStates][numNewStates]) {
	this->min = min;
	this->max = max;
	this->step = step;
}

NTBP_transition_rate_matrix_o::~NTBP_transition_rate_matrix_o() {
	// TODO Auto-generated destructor stub
}

void NTBP_transition_rate_matrix_o::setTransitionProbability(NTreal voltage,
		NTsize start, NTsize stop, NTreal probability) {
	NTsize index = floor((voltage - min) / step + 0.5);
	_probMatrices[index][start - 1][stop - 1] = probability;
}

NTreal NTBP_transition_rate_matrix_o::getTransitionProbability(NTreal voltage,
		NTsize start, NTsize stop) {
	//NTsize index = floor(voltage/step) - floor(min / step);
	NTsize putative_index = floor((voltage - min) / step + 0.5);
	//while(abs(voltage-_voltageIndex[putative_index])>step) putative_index++;
	return _probMatrices[putative_index][start - 1][stop - 1];
}

NTreal NTBP_transition_rate_matrix_o::getTransitionProbability(NTsize index,
		NTsize start, NTsize stop) {
	return _probMatrices[index][start - 1][stop - 1];
}
