/**
 * @file transition_rate_matrix.cc
 * @author Ali Neishabouri
 */

#include "transition_rate_matrix.h"

using namespace modigliani_core;

Transition_rate_matrix::Transition_rate_matrix(
		modigliani_base::Size new_num_states, modigliani_base::Real new_min, modigliani_base::Real new_max, modigliani_base::Real new_step) :
		min(new_min), max(new_max), step(new_step), num_states(new_num_states) {
  max_index = get_index(new_max);
	int length = (floor((max - min) / step + 0.5) + 1)	* num_states * num_states;
	_probMatrices = new modigliani_base::Real[length];
	for (int i = 0; i< length; i++){
		_probMatrices[i]=0;
	}
}

Transition_rate_matrix::~Transition_rate_matrix(){
	delete[] _probMatrices;
}

void Transition_rate_matrix::setTransitionProbability(modigliani_base::Real voltage,
		modigliani_base::Size start, modigliani_base::Size stop, modigliani_base::Real probability) {
	modigliani_base::Size index = floor((voltage - min) / step + 0.5);
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

void Transition_rate_matrix::setTransitionProbability(modigliani_base::Size index,
		modigliani_base::Size start, modigliani_base::Size stop, modigliani_base::Real probability) {
	_probMatrices[index * num_states * num_states + (start - 1) * num_states
			+ (stop - 1)] = probability;
}

modigliani_base::Size Transition_rate_matrix::get_index(modigliani_base::Real voltage){
	return (floor((voltage - min) / step +0.5));
}

modigliani_base::Real Transition_rate_matrix::getTransitionProbability(modigliani_base::Real voltage,
		modigliani_base::Size start, modigliani_base::Size stop) {
  if (voltage > max){
    std::cerr << "Voltage greater than maximum authorized value: " << voltage << std::endl;
    std::exit(1);
  }
	return (_probMatrices[get_index(voltage) * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}

modigliani_base::Real Transition_rate_matrix::getTransitionProbability(modigliani_base::Size index,
		modigliani_base::Size start, modigliani_base::Size stop) {
  if (index > max_index){
      std::cerr << "Index greater than maximum authorized value: " << index << std::endl;
      std::exit(1);
    }
	return (_probMatrices[index * num_states * num_states
			+ (start - 1) * num_states + (stop - 1)]);
}
