/**
 * @file voltage_gated_ion_channel_current.h
 * @brief Voltage_gated_ion_channel_current class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * Copyright (C) 2013 Mohammad Ali Neishabouri
 *
 * @section LICENSE
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_VOLTAGE_GATED_ION_CHANNEL_CURRENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_VOLTAGE_GATED_ION_CHANNEL_CURRENT_H_

#include "modigliani/obj.h"
#include "modigliani/membrane_current.h"
#include "modigliani/ion_channels.h"

namespace modigliani {
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
    Voltage_gated_ion_channel_current(modigliani::Real reversalPotential,
        modigliani::Real density,
        modigliani::Real area,
        modigliani::Real conductivity);

    Voltage_gated_ion_channel_current(
        const Voltage_gated_ion_channel_current & original) = delete;
    Voltage_gated_ion_channel_current & operator=(
        const Voltage_gated_ion_channel_current & right) = delete;

    virtual ~Voltage_gated_ion_channel_current();

/**
 * @brief Returns channel density
 * @return Channel density in [@f$\si{\per\micro\meter\squared}@f$]
 */
    modigliani::Real density() const {
      return (density_);
    }

/**
 * @brief Returns membrane surface area
 * @return Area in [@f$\si{\micro\meter\squared}@f$]
 */    
    modigliani::Real area() const {
      return (area_);
    }

/**
 * @brief Returns conductivity per channel
 * @return conductivity per channel [@f$\si{\milli\siemens}@f$]
 */      
    modigliani::Real conductivity() const {
      return (conductivity_);
    }

/**    
 * @brief Conductivity if all channels open
 * @return Conductivity in [@f$\si{\milli\siemens\per\centi\meter\squared}@f$]
 */
    modigliani::Real MaxConductivity() const {
      return (density_ * conductivity_ * 1.0e8);
    }

/**
 * @brief Sets the number of channels to agree with the membrane area and the
 * density of channels
 */
    void UpdateNumChannels() {
      num_channels_ = (modigliani::Size) ceil(density_ * area_);
    }

/**
 * @brief Returns number of ionic channels
 * @return Total number of channels
 */
    modigliani::Real num_channels() const {
      return (num_channels_);
    }

/**
 * @brief Returns ratio of open channels over total number of channels
 * @return Ratio of open channels
 */
    modigliani::Real OpenChannelsRatio() const {
      return (OpenChannels() / num_channels());
    }

/**
 * @brief Returns number of open ionic channels
 * @return Number of open channels
 */
    virtual modigliani::Real OpenChannels() const = 0;

  protected:
    // in mSiemens per channel
    modigliani::Real conductivity_;
    // channels per muMeter^2
    modigliani::Real density_;

  private:
    modigliani::Size num_channels_;
    // in mumeter^2
    modigliani::Real area_;
    bool ratesComputed;
};
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_CORE_VOLTAGE_GATED_ION_CHANNEL_CURRENT_H_
