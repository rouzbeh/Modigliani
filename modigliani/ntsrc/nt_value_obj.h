/* nt_value_obj.h - class header for value class */
/* by Ahmed A. Faisal, 22. 5. 1998(c) */

/*
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
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


/* $Id: nt_value_obj.h,v 1.2 2003/06/20 16:15:09 face Exp $
 * $Log: nt_value_obj.h,v $
 * Revision 1.2  2003/06/20 16:15:09  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/20 01:15:29  face
 * added NT_string_o as string type
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.2  1999/05/08 23:58:55  face
 * *** empty log message ***
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.7  1998/06/28 17:12:50  face
 * debugged
 *
 * Revision 1.6  1998/06/28 13:27:02  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/06/01 19:05:33  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/06/01 12:37:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/28 20:33:24  face
 * ,
 *
 * Revision 1.2  1998/05/24 11:23:29  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/05/22 16:44:49  face
 * Initial revision
 *
 */


#ifndef _NT_VALUE_OBJ_H_
#define _NT_VALUE_OBJ_H_

#include "nt_main.h"
#include "nt_types.h"
#include "nt_codes.h"
#include "nt_obj.h"

#include <ostream>
#include <string>
//#include "nt_string_obj.h"
//
//	Class name : NT_value_o
//
//	Description :
//
class NT_value_o : public NT_o {
protected:
    NTnum _amount() const;
    NTid _id() const;
    string _name() const;
    NTreal _value() const;
    void set_amount(NTnum newAmount);
    void set_id(NTid newId);
    void set_name(string newName);
    void set_value(NTreal newValue);

private:
    NTnum amount;
    NTid id;
    string name;
    NTreal value;

public:
    NT_value_o();
    NT_value_o(NTid typeId, NT_string name, NTnum amount, NTreal value);
    virtual ~NT_value_o();

    friend ostream & operator<<(ostream & os, const NT_value_o & self);
};



//
//	Method name : amount
//
//	Description :
//	Input :
//	Output :
//
inline NTnum NT_value_o::_amount() const
{
    return amount;
}


//
//	Method name : id
//
//	Description :
//	Input :
//	Output :
//
inline NTnum NT_value_o::_id() const
{
    return id;
}


//
//	Method name : name
//
//	Description : return the name attribute
//	Input :
//	Output : deletable NTstring
//
inline string NT_value_o::_name() const
{
    return name;
}




//
//	Method name : value
//
//	Description :
//	Input :
//	Output :
//
inline NTreal NT_value_o::_value() const
{
    return value;
}





#endif /* _NT_VALUE_OBJ_H_ */
