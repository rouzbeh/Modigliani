/*
 * network_synapse.h
 *
 *  Created on: 24 Jan 2013
 *      Author: man210
 */

#ifndef NETWORKSYNAPSE_H_
#define NETWORKSYNAPSE_H_

#include "membrane_current.h"
#include "membrane_compartment.h"

namespace modigliani_core {

class Network_synapse : public Membrane_current {
  public:
    Network_synapse(modigliani_base::Real reversal_potential,
                    Membrane_compartment *src);
    virtual ~Network_synapse();

    Network_synapse(const Network_synapse & original) = delete;
    Network_synapse & operator=(const Network_synapse & right) = delete;

    modigliani_base::ReturnEnum step_current();
    /** compute and return conductance in mSiemens */
    modigliani_base::Real compute_conductance();
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    Membrane_compartment* source;
    modigliani_base::Real v_input;
    //modigliani_base::Real avgSynBkCond; /* ns */
  };

  } /* namespace modigliani_core */
#endif /* NETWORKSYNAPSE_H_ */
