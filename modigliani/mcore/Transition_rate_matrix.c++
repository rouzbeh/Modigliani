/*
 * transition_rate_matrix.c++
 *
 *  Created on: 30 Mar 2011
 *      Author: man210
 */

#include "Transition_rate_matrix.h"

using namespace mcore;

Transition_rate_matrix::Transition_rate_matrix(
		mbase::Size_t new_num_states, mbase::Real new_min, mbase::Real new_max, mbase::Real new_step) :
		min(new_min), max(new_max), step(new_step), num_states(new_num_states) {
	int length = (floor((max - min) / step + 0.5) + 1)	* num_states * num_states;
	_probMatrices = new mbase::Real[length];
	for (int i = 0; i< length; i++){
		_probMatrices[i]=0;
	}
}

Transition_rate_matrix::~Transition_rate_matrix(){
	delete[] _probMatrices;
}

void Transition_rate_matrix::setTransitionProbability(mbase::Real voltage,
		mbase::Size_t start, mbase::Size_t stop, mbase::Real probability) {
	mbase::Size_t index = floor((voltage - min) / step + 0.5);
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

void Transition_rate_matrix::setTransitionProbability(mbase::Size_t index,
		mbase::Size_t start, mbase::Size_t stop, mbase::Real probability) {
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

mbase::Size_t Transition_rate_matrix::get_index(mbase::Real voltage){
	return (floor((voltage - min) / step +0.5));
}

mbase::Real Transition_rate_matrix::getTransitionProbability(mbase::Real voltage,
		mbase::Size_t start, mbase::Size_t stop) {
	return (_probMatrices[get_index(voltage) * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}

mbase::Real Transition_rate_matrix::getTransitionProbability(mbase::Size_t index,
		mbase::Size_t start, mbase::Size_t stop) {
	return (_probMatrices[index * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}
