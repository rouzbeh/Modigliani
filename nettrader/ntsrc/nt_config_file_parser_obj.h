/**\file nt_config_file_parser_obj.h - NT_config_file_parser_o class header 
 * by Ahmed Aldo Faisal &copy; created 3.7.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.0
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
  

/* $Id: nt_config_file_parser_obj.h,v 1.1 2005/04/25 13:55:32 face Exp $ 
* $Log: nt_config_file_parser_obj.h,v $
* Revision 1.1  2005/04/25 13:55:32  face
* *** empty log message ***
*

*/
#ifndef _nt_config_file_parser_obj_h_ 
#define _nt_config_file_parser_obj_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* NT includes */
#include "nt_str2real_obj.h"
/* other includes */
#include <string>
#include <map>
#include <fstream>

/** @short NT_config_file_parser_o class 
\bug unknown
\warning unknown 
*/
class NT_config_file_parser_o : public NT_o {

   std::map<std::string,NT_str2real_o> content_;

public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
  NT_config_file_parser_o(std::string const& configFile);
   NT_config_file_parser_o();
   const NT_config_file_parser_o & operator= (const NT_config_file_parser_o & original);
virtual ~NT_config_file_parser_o();

/* ***  Methods              ***/  
  NT_str2real_o const& Value(std::string const& section, std::string const& entry) const;

  NT_str2real_o const& Value(std::string const& section, std::string const& entry, double value);
  NT_str2real_o const& Value(std::string const& section, std::string const& entry, std::string const& value);
};

#endif /* _nt_config_file_parser_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
