/**
 * @file membrane_compartment_sequence.h
 * Membrane_compartment_sequence class header
 * @author Ahmed Aldo Faisal &copy; created 26.3.2001
 * @version   0.5
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

#ifndef _modigliani_core_membrane_compartment_sequence_h_
#define _modigliani_core_membrane_compartment_sequence_h_

/* Parent includes */
#include "membrane.h"
/* NT includes */
#include "cylindrical_compartment.h"
/* other includes */
#include <fstream>
#include <vector>
#include <tnt/tnt_cmat.h>

namespace modigliani_core {
/** @short Membrane_compartment_sequence class
 \bug unknown
 \warning unknown
 */
class Membrane_compartment_sequence : public Membrane {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
// see .cpp comments : Membrane_compartment_sequence();
// RESTRUCTURING GOING ON HERE
//transforming everything in  here into AddCompartment(NTBP_com..._o * compPtr) to allow
//polymorphic usage of different compartments
//
    Membrane_compartment_sequence(); Membrane_compartment_sequence(
        const Membrane_compartment_sequence & original) = delete;
    const Membrane_compartment_sequence & operator=(
        const Membrane_compartment_sequence & right) = delete;
    virtual ~Membrane_compartment_sequence();
    /* ***  Methods              ***/
    modigliani_base::ReturnEnum PushBack(Cylindrical_compartment * compartPtr);
    modigliani_base::ReturnEnum Init();
    modigliani_base::ReturnEnum InitialStep();
    modigliani_base::ReturnEnum step();
    void ShowVoltage() {
      std::cerr << "Voltage [";
      for (modigliani_base::Size ll = 0; ll < _numCompartments(); ll++) {
        std::cout << compartmentVec[ll]->vm() << "\t";
        std::cerr << compartmentVec[ll]->vm() << " ";
      }
      std::cerr << "]" << std::endl;
    }
    modigliani_base::Real MembraneVoltage(modigliani_base::Size compartmentId /* 1..numCompartments*/) {
      return (compartmentVec[compartmentId - 1]->vm());
    }
    modigliani_base::ReturnEnum InjectCurrent(modigliani_base::Real current /* in nA */, modigliani_base::Size compartmentId);
    modigliani_base::Size _numCompartments() const {
      return (numCompartments);
    }
    /**  */
    /**  */
    modigliani_base::Real CompartmentSequenceChannelStateTimeConstant() const;
    void ShowHinesMatrix();
    modigliani_base::Real AttachedCurrent(modigliani_base::Size compIndex, modigliani_base::Size currIndex) {
      M_ASSERT(compIndex > 0);
      return (compartmentVec[compIndex - 1]->Current(currIndex)->current());
    }
    std::vector<modigliani_base::Real> open_channels(modigliani_base::Size currIndex) const;
    std::vector<modigliani_base::Real> OpenChannelsRatio(modigliani_base::Size currIndex) const;
    std::vector<modigliani_base::Real> NumChannels(modigliani_base::Size currIndex) const;
    std::vector<modigliani_base::Real> _vVec() const;
    modigliani_base::ReturnEnum WriteMembranePotential(std::ostream & file) const;
    modigliani_base::ReturnEnum WriteCompartmentData(std::ostream* file, modigliani_base::Size to_print) const;
    modigliani_base::ReturnEnum WriteCurrent(std::ostream & file,
        modigliani_base::Size currentIndex /* 1..numCurrents in compartment */) const;
    modigliani_base::ReturnEnum WriteCurrent(std::ostream & file,
        modigliani_base::Size currentIndex /* 1..numCurrents in compartment */,
        std::vector<modigliani_base::Size> to_print) const;
    modigliani_base::ReturnEnum WriteMembranePotentialASCII(std::ostream & file) const;
    modigliani_base::ReturnEnum WriteCurrentAscii(std::ostream & file,
        modigliani_base::Size currentIndex /* 1..numCurrents in compartment */) const;
    Cylindrical_compartment* ReturnCompartmentVec(modigliani_base::Size index);
    /**  */
    bool GillespieStep();
    /**  */
    std::vector<modigliani_base::Real> GiveCurrent(modigliani_base::Size index);
    /* ***  Data                 ***/
    std::vector<Cylindrical_compartment*> compartmentVec;

    protected:
    /* ***  Methods              ***/
    /** CRAP not working */
    std::vector<modigliani_base::Real> ZadorPearlmutterSolveTriDiag(std::vector<modigliani_base::Real> lNewVec,
        std::vector<modigliani_base::Real> dNewVec, std::vector<modigliani_base::Real> uNewVec,
        std::vector<modigliani_base::Real> rNewVec) const;
    /** CRAP not working */
    std::vector<modigliani_base::Real> MascagniSolveTriDiag(std::vector<modigliani_base::Real> lNewVec,
        std::vector<modigliani_base::Real> dNewVec, std::vector<modigliani_base::Real> uNewVec,
        std::vector<modigliani_base::Real> rNewVec) const;
    /** WORKING ! */
    std::vector<modigliani_base::Real> NumericalRecipesSolveTriDiag(const std::vector<modigliani_base::Real> & l,
        const std::vector<modigliani_base::Real> & d, const std::vector<modigliani_base::Real> & u,
        const std::vector<modigliani_base::Real> & r) const;
    /* ***  Data                 ***/

    private:
    /* ***  Methods              ***/
    modigliani_base::Real _OpenChannelsRatio(const
        Membrane_current* in_current) const {
      return(0);
    }

    modigliani_base::Real _OpenChannelsRatio(const
        Voltage_gated_ion_channel_current* in_current) const {
      return(in_current->OpenChannelsRatio());
    }

    modigliani_base::Real _open_channels(const
        Membrane_current* in_current) const {
      return(0);
    }

    modigliani_base::Real _open_channels(const
        Voltage_gated_ion_channel_current* in_current) const {
      return(in_current->open_channels());
    }

    modigliani_base::Real _NumChannels(const
        Membrane_current* in_current) const {
      return(0);
    }

    modigliani_base::Real _NumChannels(const
        Voltage_gated_ion_channel_current* in_current) const {
      return(in_current->num_channels());
    }

    /* ***  Data                 ***/
    std::vector<modigliani_base::Real> lVec;
    std::vector<modigliani_base::Real> dVec;
    std::vector<modigliani_base::Real> uVec;

//This should be in each compartment
//std::vector <modigliani_base::Real> vVec;
    std::vector<modigliani_base::Real> rVec;
    modigliani_base::Size numCompartments;
    bool initialised;
    bool swCrankNicholson;
  };}
#endif /* _ntbp_membrane_compartment_sequence.h_ */ 

