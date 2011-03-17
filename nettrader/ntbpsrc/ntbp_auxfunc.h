/**\file ntbp_auxfunc.h - NTBP auxiliary function headers  for heteregenous object creation 
 * by Ahmed Aldo Faisal &copy; created 22.4.2005  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998-2005 Ahmed Aldo Faisal    
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

/* $Id: ntbp_auxfunc.h,v 1.1 2005/04/25 13:55:27 face Exp $
 * $Log: ntbp_auxfunc.h,v $
 * Revision 1.1  2005/04/25 13:55:27  face
 * *** empty log message ***
 *
 */

#ifndef _ntbp_auxfunc_h_ 
#define _ntbp_auxfunc_h_ 

#include "ntbp_sga_cylindrical_compartment_obj.h"
#include "ntbp_custom_cylindrical_compartment_obj.h"

#include "ntbp_multi_sodium_current_obj.h"
#include "ntbp_colbert_axonal_sodium_multi_current_obj.h"
#include "ntbp_hranvier_sodium_multi_current_obj.h"
#include "ntbp_patlak_sodium_multi_current_obj.h"

#include "ntbp_multi_potassium_current_obj.h"
#include "ntbp_colbert_axonal_potassium_multi_current_obj.h"
#include "ntbp_hranvier_potassium_multi_current_obj.h"
#include "ntbp_potassium_multi_current_obj.h"

NTreal /* mum^-2 */
		NTBP_corrected_channel_density(NTreal chDensity /* mum^-2 */,
				NTreal compArea /* mum^-2 */);

NTBP_membrane_current_o *
NTBP_create_na_channel_ptr(NTsize sodiumModel, NTsize algorithm,
		NTreal chDensity /* mum^-2 */, NTreal chConductance /* pS */,
		NTreal q10, NTreal temperature /* C */, NTreal compArea /* mum^2 */);

NTBP_membrane_current_o *
NTBP_create_na_channel_ptr(NTsize sodiumModel, NTsize algorithm,
		NTreal chDensity /* mum^-2 */, NTreal chConductance /* pS */,
		NTreal q10m, NTreal q10h, NTreal temperature /* C */,
		NTreal compArea /* mum^2 */, NTreal timeStep,
		NTreal reversalPotential = 115 /*mV*/,
		NTreal vBase = -84);
NTBP_membrane_current_o *
NTBP_create_k_channel_ptr(NTsize potassiumModel, NTsize algorithm,
		NTreal chDensity /* mum^-2*/, NTreal chConductance /* pS */,
		NTreal q10, NTreal temperature /* C */, NTreal compArea /* mum^2 */,
		NTreal reversalPotential = 0 /*mV*/, NTreal vBase = -84);

#endif /* _ntbp_auxfunc_h_ */ 
