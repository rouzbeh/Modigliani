///**\file ntbp_na_only_sga_cylindrical_compartment_obj.cpp - NTBP_na_only_sga_cylindrical_compartment_o class implementation
// * by Ahmed Aldo Faisal &copy; created 26.3.2001
// */
///* NetTrader - visualisation, scientific and financial analysis and simulation system
// * Version:  0.5
// * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
// *
// * This library is free software; you can redistribute it and/or
// * modify it under the terms of the GNU Library General Public
// * License as published by the Free Software Foundation; either
// * version 2 of the License, or (at your option) any later version.
// *
// * This library is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// * Library General Public License for more details.
// *
// * You should have received a copy of the GNU Library General Public
// * License along with this library; if not, write to the Free
// * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// */
//
//
///* $Id: ntbp_Na_only_sga_cylindrical_compartment_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
// * $Log: ntbp_Na_only_sga_cylindrical_compartment_obj.cpp,v $
// * Revision 1.1.1.1  2004/12/16 01:38:36  face
// * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
// *
// * Revision 1.2  2003/01/17 16:53:13  face
// * *** empty log message ***
// *
// * Revision 1.1  2001/10/03 14:37:52  face
// * *** empty log message ***
// *
// */
//
//#include "ntbp_Na_only_sga_cylindrical_compartment_obj.h"
//
///* ***      CONSTRUCTORS	***/
///** Create a NTBP_na_only_sga_cylindrical_compartment_o */
//NTBP_na_only_sga_cylindrical_compartment_o::NTBP_na_only_sga_cylindrical_compartment_o(NTreal newLength, NTreal newDiameter)
//:
//NTBP_cylindrical_compartment_o(newLength, newDiameter)
//{
//
//	Set_cM(1);   // muFarad/cm^2 (Koch, 1999)
//	Set_rA(35.4); // ohm cm  (Koch, 1999)
//	AttachCurrent( new NTBP_hh_sga_leak_current_o( _area() ), NTBP_LEAK );
//
//	/* Stochastic, single channel currents */
//// AttachCurrent( new NTBP_sga_sodium_current_o( _area() ), NTBP_IONIC);
//// AttachCurrent( new NTBP_delayed_potassium_rectifier_current_o( _area(),NTBP_NA_ONLY_SGA_K_CONTI ), NTBP_IONIC);
///* Deterministic channel currents */
////	AttachCurrent( new NTBP_hh_sga_sodium_current_o( _area() ), NTBP_IONIC );
////	AttachCurrent( new NTBP_hh_sga_potassium_current_o( _area() ), NTBP_IONIC );
///* Multi method channel currents */
//
//	NTBP_multi_sodium_current_o* tmpNaPtr = new NTBP_multi_sodium_current_o( _area() , 2000);
//	tmpNaPtr->SetSimulationMode(NTBP_DETERMINISTIC);
//	AttachCurrent( tmpNaPtr , NTBP_IONIC );
//
//
//}
//
///* ***      COPY AND ASSIGNMENT	***/
//NTBP_na_only_sga_cylindrical_compartment_o::NTBP_na_only_sga_cylindrical_compartment_o(const NTBP_na_only_sga_cylindrical_compartment_o & original)
//:
//NTBP_cylindrical_compartment_o(original._length(), original._diameter())
//{
// // add assignment code here
//}
//
//const NTBP_na_only_sga_cylindrical_compartment_o&
//NTBP_na_only_sga_cylindrical_compartment_o::operator= (const NTBP_na_only_sga_cylindrical_compartment_o & right)
//{
// if (this == &right) return *this; // Gracefully handle self assignment
// // add assignment code here
//  return *this;
//}
//
///* ***      DESTRUCTOR		***/
//NTBP_na_only_sga_cylindrical_compartment_o::~NTBP_na_only_sga_cylindrical_compartment_o()
//{
//}
//
///* ***  PUBLIC                                    ***   */
///** @short
//    @param      none
//    @return     none
//   \warning    unknown
//   \bug        unknown
//
//void
//NTBP_na_only_sga_cylindrical_compartment_o::() const
//{
//}
// */
//
///* ***  PROTECTED                         ***   */
///* ***  PRIVATE                           ***   */
//
//
///* File skeleton generated by GenNTObj version 0.7. */
