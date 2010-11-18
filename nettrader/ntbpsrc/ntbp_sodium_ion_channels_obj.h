/**\file ntbp_sodium_ion_channels_obj.h - NTBP_sodium_ion_channels_o class header
 * by Ahmed Aldo Faisal &copy; created 15.3.2001
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
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


/* $Id: ntbp_sodium_ion_channels_obj.h,v 1.4 2003/01/17 16:53:13 face Exp $
* $Log: ntbp_sodium_ion_channels_obj.h,v $
* Revision 1.4  2003/01/17 16:53:13  face
* *** empty log message ***
*
* Revision 1.3  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.2  2001/10/12 09:18:36  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_sodium_ion_channels_obj_h_
#define _ntbp_sodium_ion_channels_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* Parent includes */
#include "ntbp_ion_channels_obj.h"
/* NT includes */
/* other includes */
#include <iostream>

/** @short NTBP_sodium_ion_channels_o class
8 state Hodgkin-Huxley based model of Sodium conducting ion channels.
The open state has stateIndex 4.
\bug unknown
\warning 1. \are has to be take on stateCounterVec (as it is 9 dimensional (loop index!))
         2. alpha and beta RATE CONSTANTS name TRANSITION probability variables !
*/
class NTBP_sodium_ion_channels_o : public NTBP_ion_channels_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NTBP_sodium_ion_channels_o(NTsize numNewChannels = 1);
    NTBP_sodium_ion_channels_o(const NTBP_sodium_ion_channels_o & original);
    const NTBP_sodium_ion_channels_o & operator= (const NTBP_sodium_ion_channels_o & right);
    virtual ~NTBP_sodium_ion_channels_o();
    /* ***  Methods              ***/
    NTreturn Step();
    void ShowStates() const;
    NTsize NumOpen() const {
        return stateCounterVec[4];    // because 0 is num total channels
    }
    NTsize NumClosed() const {
        return _numChannels() - NumOpen();
    }
    /**  */
    NTreal ComputeChannelStateTimeConstant( ) const;
    /**  */
    NTreturn UpdateStateProb( vector < NTreal > rateConstVec);
    /**  */
    bool ComputeGillespieStep(NTsize stateId);
    /** set the steady state distribution of channel states for current rate constants, DIRTY HACK*/
    NTreturn SteadyStateDistribution();
    /** No descriptions */
    NTreturn BinomialStep();
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    NTreal alphaM;
    NTreal betaM;
    NTreal alphaH;
    NTreal betaH;
    NTreal p12;
    NTreal p23;
    NTreal p34;
    NTreal p56;
    NTreal p67;
    NTreal p78;

    NTreal p87;
    NTreal p76;
    NTreal p65;
    NTreal p43;
    NTreal p32;
    NTreal p21;

    NTreal p15;
    NTreal p26;
    NTreal p37;
    NTreal p48;

    NTreal p51;
    NTreal p62;
    NTreal p73;
    NTreal p84;

    NTreal t15;
    NTreal t26;
    NTreal t37;
    NTreal t48;

    NTreal t51;
    NTreal t62;
    NTreal t73;
    NTreal t84;

    NTreal t21;
    NTreal t32;
    NTreal t65;
    NTreal t76;
};

#endif /* _ntbp_sodium_ion_channels_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
