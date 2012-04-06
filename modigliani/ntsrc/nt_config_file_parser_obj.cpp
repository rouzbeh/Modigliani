/**\file nt_config_file_parser_obj.cpp - NT_config_file_parser_o class implementation 
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
  

/* $Id: nt_config_file_parser_obj.cpp,v 1.1 2005/04/25 13:55:32 face Exp $ 
* $Log: nt_config_file_parser_obj.cpp,v $
* Revision 1.1  2005/04/25 13:55:32  face
* *** empty log message ***
*

*/
#include "nt_config_file_parser_obj.h" 

std::string NT_trimstr(std::string const& source, char const* delims = " \t\r\n") {
  std::string result(source);
  std::string::size_type index = result.find_last_not_of(delims);
  if(index != std::string::npos)
    result.erase(++index);

  index = result.find_first_not_of(delims);
  if(index != std::string::npos)
    result.erase(0, index);
  else
    result.erase();
  return result;
}

/* ***      CONSTRUCTORS	***/
/** Create a NT_config_file_parser_o */
/* ***      COPY AND ASSIGNMENT	***/ 
NT_config_file_parser_o::NT_config_file_parser_o(std::string const& configFile) {
  std::ifstream file(configFile.c_str());

  std::string line;
  std::string name;
  std::string value;
  std::string inSection;
  int posEqual;
  while (std::getline(file,line)) {

    if (! line.length()) continue;

    if (line[0] == '#') continue;
    if (line[0] == ';') continue;

    if (line[0] == '[') {
      inSection=NT_trimstr(line.substr(1,line.find(']')-1));
      continue;
    }

    posEqual=line.find('=');
    name  = NT_trimstr(line.substr(0,posEqual));
    value = NT_trimstr(line.substr(posEqual+1));

    content_[inSection+'/'+name]=NT_str2real_o(value);
  }
}

/* ***      DESTRUCTOR		***/
NT_config_file_parser_o::~NT_config_file_parser_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short      Value
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
NT_str2real_o const& NT_config_file_parser_o::Value(std::string const& section, std::string const& entry) const {

  std::map<std::string,NT_str2real_o>::const_iterator ci = content_.find(section + '/' + entry);

  if (ci == content_.end()) throw "does not exist";

  return ci->second;
}

NT_str2real_o const& NT_config_file_parser_o::Value(std::string const& section, std::string const& entry, double value) {
  try {
    return Value(section, entry);
  } catch(const char *) {
    return content_.insert(std::make_pair(section+'/'+entry, NT_str2real_o(value))).first->second;
  }
}

NT_str2real_o const& NT_config_file_parser_o::Value(std::string const& section, std::string const& entry, std::string const& value) {
  try {
    return Value(section, entry);
  } catch(const char *) {
    return content_.insert(std::make_pair(section+'/'+entry, NT_str2real_o(value))).first->second;
  }
}

/* File skeleton generated by GenNTObj version 0.7. */