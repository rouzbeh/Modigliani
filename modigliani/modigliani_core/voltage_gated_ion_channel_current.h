/**
 * @file voltage_gated_ion_channel_current.h
 * modigliani_core_voltage_gated_ion_channel_current class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_VOLTAGE_GATED_ION_CHANNEL_CURRENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_VOLTAGE_GATED_ION_CHANNEL_CURRENT_H_

#include "modigliani_base/obj.h"
#include "modigliani_core/membrane_current.h"
#include "modigliani_core/ion_channels.h"

namespace modigliani_core {
/**
 * @brief Base class for voltage gated ion channels.
 */
class Voltage_gated_ion_channel_current : public Membrane_current {
  public:
/**
 * @brief Constructor
 * @param reversalPotential Reversal potential in [@f$\si{\milli\volt}@f$]
 * @param density Channel density in [@f$\si{\per\micro\meter\squared}@f$]
 * @param area Membrane area in [@f$\si{\micro\meter\squared}@f$]
 * @param conductivity Single channel conductivity in [@f$\si{\milli\siemens}@f$]
 */
    Voltage_gated_ion_channel_current(modigliani_base::Real reversalPotential,
        modigliani_base::Real density,
        modigliani_base::Real area,
        modigliani_base::Real conductivity);

    Voltage_gated_ion_channel_current(
        const Voltage_gated_ion_channel_current & original) = delete;
    Voltage_gated_ion_channel_current & operator=(
        const Voltage_gated_ion_channel_current & right) = delete;

    virtual ~Voltage_gated_ion_channel_current();

/**
 * @brief Returns channel density
 * @return Channel density in [@f$\si{\per\micro\meter\squared}@f$]
 */
    modigliani_base::Real density() const {
      return (density_);
    }

/**
 * @brief Returns membrane surface area
 * @return Area in [@f$\si{\micro\meter\squared}@f$]
 */    
    modigliani_base::Real area() const {
      return (area_);
    }

/**
 * @brief Returns conductivity per channel
 * @return conductivity per channel [@f$\si{\milli\siemens}@f$]
 */      
    modigliani_base::Real conductivity() const {
      return (conductivity_);
    }

/**    
 * @brief Conductivity if all channels open
 * @return Conductivity in [@f$\si{\milli\siemens\per\centi\meter\squared}@f$]
 */
    modigliani_base::Real MaxConductivity() const {
      return (density_ * conductivity_ * 1.0e8);
    }

/**
 * @brief Sets the number of channels to agree with the membrane area and the
 * density of channels
 */
    void UpdateNumChannels() {
      num_channels_ = (modigliani_base::Size) ceil(density_ * area_);
    }

/**
 * @brief Returns number of ionic channels
 * @return Total number of channels
 */
    modigliani_base::Real num_channels() const {
      return (num_channels_);
    }

/**
 * @brief Returns ratio of open channels over total number of channels
 * @return Ratio of open channels
 */
    modigliani_base::Real OpenChannelsRatio() const {
      return (OpenChannels() / num_channels());
    }

/**
 * @brief Returns number of open ionic channels
 * @return Number of open channels
 */
    virtual modigliani_base::Real OpenChannels() const = 0;

  protected:
    // in mSiemens per channel
    modigliani_base::Real conductivity_;
    // channels per muMeter^2
    modigliani_base::Real density_;

  private:
    modigliani_base::Size num_channels_;
    // in mumeter^2
    modigliani_base::Real area_;
    bool ratesComputed;
};
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_VOLTAGE_GATED_ION_CHANNEL_CURRENT_H_
