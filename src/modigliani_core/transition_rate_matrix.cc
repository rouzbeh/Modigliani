/**
 * @file transition_rate_matrix.cc
 * @brief Transition_rate_matrix class implementation
 *
 * Copyright 2011 Ali Neishabouri
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modigliani_core/transition_rate_matrix.h"
#include <algorithm>

namespace modigliani_core {
  Transition_rate_matrix::Transition_rate_matrix(modigliani_base::
                                                 Size new_num_states,
                                                 modigliani_base::Real new_min,
                                                 modigliani_base::Real new_max,
                                                 modigliani_base::Real new_step)
  :min_(new_min), max_(new_max), step_(new_step), num_states_(new_num_states) {
    max_index_ = GetIndex(new_max);
    int length =
      (floor((max_ - min_) / step_ + 0.5) + 1) * num_states_ * num_states_;
      prob_matrices_ = new modigliani_base::Real[length];
    for (int i = 0; i < length; i++) {
      prob_matrices_[i] = 0;
    }
  }

  Transition_rate_matrix::~Transition_rate_matrix() {
    delete[] prob_matrices_;
  }

  void Transition_rate_matrix::SetTransitionProbability(modigliani_base::
                                                        Real voltage,
                                                        modigliani_base::
                                                        Size start,
                                                        modigliani_base::
                                                        Size stop,
                                                        modigliani_base::
                                                        Real probability) {
    modigliani_base::Size index = floor((voltage - min_) / step_ + 0.5);
    prob_matrices_[index * num_states_ * num_states_ + (start - 1) * num_states_
                  + (stop - 1)] = probability;
  }

  void Transition_rate_matrix::SetTransitionProbability(modigliani_base::
                                                        Size index,
                                                        modigliani_base::
                                                        Size start,
                                                        modigliani_base::
                                                        Size stop,
                                                        modigliani_base::
                                                        Real probability) {
    prob_matrices_[index * num_states_ * num_states_ + (start - 1) * num_states_ +
                  (stop - 1)] = probability;
  }

modigliani_base::Size Transition_rate_matrix::GetIndex(modigliani_base::
                                                          Real voltage) {
    return (floor((voltage - min_) / step_ + 0.5));
  }

  modigliani_base::Real Transition_rate_matrix::
    GetTransitionProbability(modigliani_base::Real voltage,
                             modigliani_base::Size start,
                             modigliani_base::Size stop) {
    if (voltage > max_) {
      throw std::out_of_range("Voltage greater than maximum authorized value.");
    }
    return (prob_matrices_[GetIndex(voltage) * num_states_ * num_states_
                          + (start - 1) * num_states_ + (stop - 1)]);
  }

  modigliani_base::Real Transition_rate_matrix::
    GetTransitionProbability(modigliani_base::Size index,
                             modigliani_base::Size start,
                             modigliani_base::Size stop) {
    if (index > max_index_) {
      throw std::out_of_range("Voltage greater than maximum authorized value.");
    }
    return (prob_matrices_[index * num_states_ * num_states_
                          + (start - 1) * num_states_ + (stop - 1)]);
  }
}  // namespace modigliani_core
