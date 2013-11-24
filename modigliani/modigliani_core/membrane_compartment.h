/**
 * @file membrane_compartment.h
 *
 * @brief Membrane_compartment class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_COMPARTMENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_COMPARTMENT_H_

#include <vector>
#include <fstream>
#include <string>

#include "modigliani_core/object.h"
#include "modigliani_core/membrane_current.h"
#include "modigliani_core/voltage_gated_ion_channel_current.h"

namespace modigliani_core {

/**
 * @brief Membrane_compartment class acts as a "container + glue" for the various membrane_current
 * classes that can be attached to it.
 */
  class Membrane_compartment:public Object {
 public :

/**
 * @brief Constructor
 *
 * @param newArea Membrane surface area in [@f$\si{\micro\meter\squared}@f$]
 * @param newTemperature Temperature in [C]
 * @param newCm Membrane capacitance in [@f$\si{\micro\farad\per\centi\meter\squared}@f$]
 * @param newRa Axoplasmic resistance in [@f$\si{ohm,\centi\meter}@f$]
 */
    Membrane_compartment(const modigliani_base::Real newArea,
                         const modigliani_base::Real newTemperature,
                         const modigliani_base::Real newCm,
                         const modigliani_base::Real newRa);

    Membrane_compartment(const Membrane_compartment & original) = delete;

    Membrane_compartment & operator=(const Membrane_compartment & right) =
        delete;

    virtual ~Membrane_compartment();

    /**
     * @brief Adds a new current to the compartment
     *
     * @param currentPtr Pointer to the Membrane_current object
     * @param type Current type
     */
    virtual modigliani_base::ReturnEnum AttachCurrent(Membrane_current *
                                                      currentPtr,
                                                      CurrentType type =
                                                      IONIC);

/**
 * @brief Sets the membrane potential to the specified value and runs of step of simulation.
 *
 * The membrane compartment has one mode of operation. If the voltage
 * is specified externally at each time step, the compartment acts as
 * a container for the occuring current objects, integratios has to be
 * provided externally.
 * e.g.
 *  Membrane_compartment c; [...]
 *   FLOAT voltage;
 *   LOOP
 *     voltage = IntegrateDifferentialEquation( c.vm() );
 *     c.Step( voltage );
 *   END LOOP;
 * Additionally single compartment HodgkinHuxley simulation is
 * possible using this where the voltage is updated at each step, if
 * the old voltage is specified as input. e.g. :
 *   LOOP
 *     Membrane_compartment c; [...]
 *     c.Step( c.vm() );
 *     deltaVM = 1.0e-3 * CompartmentMembraneNetCurrent() / CompartmentMembraneCapacitance() ;
 *     vM += deltaVM * _timeStep();
 *   END LOOP;
 * @param newVM The membrane potential in [mV]
 * @return Success or failure
 */
    virtual modigliani_base::ReturnEnum Step(const modigliani_base::Real newVM);

/**
 * @brief Sets the membrane potential according to membrane current and runs of step of simulation.
 *
 * @see Step()
 * @return Success or failure
 */
    virtual modigliani_base::ReturnEnum Step();

/**
 * @brief Injects current into the compartment.
 *
 * This funcion is used to mimic the effects of a current injection in
 * to cell.
 * @param current Current to inject in [nA]
 * @return Success or failure
 */   
    modigliani_base::ReturnEnum InjectCurrent(modigliani_base::Real current);

/**
 * @brief Returns the current conductance of the attached conductance
 * source.
 *
 * You can think of this as returning @f$G*m(t)^3*h(t)@f$ for a
 * Hodgkin-Huxley sodium channel.
 * @param currentIndex The current source we are interested in. 
 * @return Conductance in [mS]
 * @warning Currents are index as in Matlab : [1..m]
 */   
    modigliani_base::
      Real AttachedConductance(modigliani_base::Size currentIndex) {
      assert((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]->conductance());
    }

/**
 * @brief Returns the reversal potential of the attached conductance
 * source.
 *
 * @param currentIndex The current source we are interested in. 
 * @return Reversal potential in [mV]
 * @warning Currents are index as in Matlab : [1..m]
 */      
    modigliani_base::
      Real AttachedReversalPotential(modigliani_base::Size currentIndex) {
      assert((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]->reversal_potential());
    }

/**
 * @brief Opens an output file and writes the header.
 *
 * This function is used to create a file in which to dump data during
 * the simulation. For performance reasons (mainly when loading output
 * files into Matlab) we use a binary file format. Data is dumped
 * sequentially into the file, and the header is a simple float32
 * stating the number of columns that should be used to reshape the
 * data.
 * @see WriteOutput()
 * @param output_file_name Output file name
 * @return Success status
 * @warning Call after having attached all currents.
 */
    virtual modigliani_base::ReturnEnum SetupOutput(std::
                                                        string
                                                        output_file_name);

/**
 * @brief Writes the current flwoing through all attached
 * Membrane_current instances to a binary file.   
 *
 * The values are written sequentially as float32. There are no
 * delimiters. They can be reshaped into a matrix using the number
 * of columns written into the header by SetupOutput().
 * @see SetupOutput()
 * @return Success or Failure
 */
    virtual modigliani_base::ReturnEnum WriteOutput() const;

/**
 * @brief Membrane time constant at instaneous membrane conductivity
 *
 * @warning Uses weighted conductance
 * @return Time constant in [ms]
 */
    modigliani_base::Real TimeConstant()const {
      return ((cm() / WeightedConductance()) * area() * 1.0e8);
    }

/**
 * @brief Returns the current object at the given index
 *
 * @return Pointer to the Membrane_current object
 * @warning Currents are index as in Matlab : [1..m]
 */  
    const Membrane_current* Current(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]);
    }

/**
 * @brief Returns the membrane surface area
 *
 * @return Membrane area in [@f$\si{\micro\meter\squared}@f$]
 */   
    modigliani_base::Real area() const {
      return (area_);
    }

/**
 * @brief Returns the membrane potential
 *
 * @return Membrane potential in [@f$\si{\milli\volt}@f$]
 */ 
    modigliani_base::Real vm() const {
      return (vm_);
    }

/**
 * @brief Sets the membrane potential
 *
 * @param newVoltage Membrane potential in [@f$\si{milli\volt}@f$]
 */ 
    void set_vm(modigliani_base::Real newVoltage) {
      vm_ = newVoltage;
    }


/**
 * @brief Sets the membrane capacitance per surface area
 *
 * @return capacitance in [@f$\si{\micro\farad\per\centi\meter\squared}@f$]
 */ 
    modigliani_base::Real cm() const {
      return (cm_);
    }

/**
 * @brief Sets the axial resistance
 *
 * @return axial resistance in [@f$\si{\ohm\centi\meter}@f$]
 */
    modigliani_base::Real ra() const {
      return (ra_);
    }

/**
 * @brief  Set temperature in compartment and for all
 * currents within the compartments.
 *
 * This also affects future attached currents.
 *
 * @param newTemp New temperature in [@f$\si{\celsius}@f$]
 */
    virtual modigliani_base::ReturnEnum set_temperature(
        modigliani_base::Real newTemp) {
      temperature_ = newTemp;
      for (modigliani_base::Size i = 0; i < current_vec_.size(); i++)
        current_vec_[i]->set_temperature(newTemp);
      return (modigliani_base::ReturnEnum::SUCCESS);
    }

/**
 * @brief  Returns temperature in compartment
 *
 * @return Temperature in [@f$\si{\celsius}@f$]
 */
    modigliani_base::Real temperature()const {
      return (temperature_);
    }

/**
 * @brief Returns total compartment capacitance
 *
 * @return Capacitance in [@f$\si{\micro\farad}@f$]
 */    
    modigliani_base::Real CompartmentMembraneCapacitance() const;

/**
 * @brief Returns net membrane current
 *
 * @return Current in [@f$\si{\nano\ampere}@f$]
 */      
    modigliani_base::Real CompartmentMembraneNetCurrent()const;

/**
 * @brief Sum of escape rates from current state
 *
 * This is useful for the Gillespie algorithm.
 * @return Escape rate in @f$[\si{\kilo\hertz}@f$]
 */
    modigliani_base::Real CompartmentChannelStateTimeConstant()const;

/**
 * @brief Perform one step of the Gillespie algorithm.
 *
 * @warning This is not as well tested as the binomial algorithm.
 */
    bool GillespieStep();

/**
 * @brief Returns the number of currents attached to the compartment.
 *
 * This includes both ion channels and leak currents.
 * @return Number of currents
 */
    modigliani_base::Size NumberCurrents() const;

/**
 * @brief Returns the sum of attached conductance weighted by the
 * difference between the membrane potential and the reversal potential.
 *
 * @return Current in [@f$\si{\micro\ampere}@f$]
 */
    modigliani_base::Real WeightedConductance() const;

/**
 * @brief Returns the vector of attached currents
 *
 * @return Vector of Membrane_current pointers
 */
    const std::vector< Membrane_current * > current_vec() const {
      return current_vec_;
    }

    static bool seed_set_;

 protected :
    std::ofstream * output_file = 0;

 private :
    // injected current into compartment in nA
    modigliani_base::Real i_inj_;

    // Contains pointers to attached currents
    std::vector < Membrane_current * >current_vec_;

    // membrane capacity in muFarad/cm^2
    const modigliani_base::Real cm_;

    // axoplasmatic resistance in Ohm cm
    const modigliani_base::Real ra_;

    // in muMeter^2
    const modigliani_base::Real area_;

    // in Celsius
    modigliani_base::Real temperature_;

    // membrane voltage in mV
    modigliani_base::Real vm_;

    boost::random::mt19937 rng_;
    boost::random::binomial_distribution <> bin_;
    boost::random::uniform_01 <> uni_;
    unsigned int seed_;
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_COMPARTMENT_H_
