/**
 * @file modigliani_core_voltage_gated_ion_channel_current.h
 * modigliani_core_voltage_gated_ion_channel_current class header
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001
 * @author Ali Neishabouri
 * @version   1
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _modigliani_core_voltage_gated_ion_channel_current_h_
#define _modigliani_core_voltage_gated_ion_channel_current_h_

/* NT core includes */
#include "modigliani_base/obj.h"
/* Parent includes */
#include "membrane_current.h"
/* NT includes */
#include "ion_channels.h"
/* other includes */

namespace modigliani_core {
/** @short Base class for voltage gated ion channels.
 \bug unknown
 \warning unknown
 */
class Voltage_gated_ion_channel_current : public Membrane_current {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Voltage_gated_ion_channel_current(modigliani_base::Real reversalPotential,  // in mV
        modigliani_base::Real density,  // channels per mumeter^2
        modigliani_base::Real area,  // in mumeter^2
        modigliani_base::Real conductivity  // in mSiemens per channel
        );
    Voltage_gated_ion_channel_current(const Voltage_gated_ion_channel_current & original);
    Voltage_gated_ion_channel_current & operator=(const Voltage_gated_ion_channel_current & right);
    virtual ~Voltage_gated_ion_channel_current();
    /* ***  Methods              ***/
    /* mementary total conductance */
    modigliani_base::Real density() const {
      return (density_);
    }
    modigliani_base::Real area() const {
      return (area_);
    }
    /* conductivity per channel in mSiemens */
    modigliani_base::Real conductivity() const {
      return (conductivity_);
    }
    /* conductivity if all channels open in mSiemens/cm^2, */
    modigliani_base::Real _maxConductivity() const {
      return (density_ /* num/muMeter^2 */* conductivity_ /* mSiemens */* 1.0e8 /* muMeter^2/cm^2 */);
    }
    /**  */
    void UpdateNumChannels() {
      num_channels_ = (modigliani_base::Size) ceil(density_ * area_);
    }

    /** Number of total ionic channels */
    modigliani_base::Real num_channels() const {
      return (num_channels_);
    }
    modigliani_base::Real OpenChannelsRatio() const {
      return (open_channels() / num_channels());
    }
    /** Number of open ionic channels */
    virtual modigliani_base::Real open_channels() const = 0;

    // Dangerous: since cached values are not automatically recomputed in derived classes
    //void Set_vBase(modigliani_base::Real newVBase /* mV */){ vBase = newVBase; }

    /* ***  Data                 ***/
  public:
  protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    modigliani_base::Real conductivity_;  // in mSiemens per channel
    modigliani_base::Real density_;  // channels per muMeter^2
  private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    modigliani_base::Size num_channels_;
    modigliani_base::Real area_;  // in mumeter^2
    bool ratesComputed;

};
}
#endif /* _modigliani_core_voltage_gated_ion_channel_current_h_ */
