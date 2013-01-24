/*
 * network_synapse.cc
 *
 *  Created on: 24 Jan 2013
 *      Author: man210
 */

#include "network_synapse.h"

namespace modigliani_core {

Network_synapse::Network_synapse(modigliani_base::Real new_reversal_potential,
                                 Membrane_compartment *src)
    : Membrane_current(new_reversal_potential), source(src) {
  // TODO Auto-generated constructor stub

}

Network_synapse::~Network_synapse() {
  // TODO Auto-generated destructor stub
}

modigliani_base::ReturnEnum Network_synapse::step_current() {
  v_input = source->vm();
  return(modigliani_base::SUCCESS);
}

modigliani_base::Real Network_synapse::compute_conductance(){
  // TODO do something with v input I guess.
  return(0);
}
}
