/* file nt_obj.h - NT master object declaration
 * by Ahmed A. Faisal, 22. 5. 1998(c)
 *
 *
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


/* $Id: nt_obj.h,v 1.2 2002/02/25 22:48:54 face Exp $
 * $Log: nt_obj.h,v $
 * Revision 1.2  2002/02/25 22:48:54  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.4  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/12/25 21:28:17  face
 * n
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.3  1998/10/20 14:51:52  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/06/01 19:05:33  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/06/01 12:37:26  face
 * Initial revision
 *
 */


#ifndef _NT_OBJ_H_
#define _NT_OBJ_H_

#include "nt_main.h"
#include "nt_types.h"



/**
  @short Master class for all NT objects.
  It provides object accounting, and a unique identifier (uniqId) for
  each object.  The unique id might become ambigous when uniqId gets
  greater LONG_MAX. However, the initialisation of each NT object with
  this class might help in synchronisation of a threaded version.
  */
class NT_o {
public:
    NT_o(); // 2DO : the constructor is not thread safe !
    NT_o(const NT_o &);
    virtual ~NT_o();

    //protected:
    NTlong _runnId() const;
    NTlong _uniqId() const;



private:
    /** unique Id number of the object
      static data member,  see implementation */
    NTlong uniqId;
    /** uniqId of the last created NT_o */
    static NTlong runnId;
};

//
//	Method name : _runnId
//
//	Description :
//	Input :
//	Output :
//
inline NTlong NT_o::_runnId() const
{
    return runnId;
}


//
//	Method name : _uniqId
//
//	Description :
//	Input :
//	Output :
//
inline NTlong NT_o::_uniqId() const
{
    return uniqId;
}



#endif /* _NT_OBJ_H_ */
