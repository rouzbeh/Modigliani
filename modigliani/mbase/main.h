/**
 * @file main.h - main header file for Modigliani sources
 * @version:  2
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


/**
 * @namespace mbase
 * Base types and classes for Modigliani
 *
 */
/* 2DO for some bizarre reason lrand48 is not included/implemented
 * and random_shuffle (STL) needs it or will have to use rand()
 */
#define __STL_NO_DRAND48
/* which is switched on with the above statement */

#ifndef _M_MAIN_H_
#define _M_MAIN_H_

#define M_MAJOR_VERSION 0
#define M_MINOR_VERSION 5

#define M_STRING_LENGTH 255
#define M_SEPARATOR " ;; "

#define M_EXPORT extern
#define M_TALKLEVEL 9


#define M_GL_SUPPORT M_TRUE
#define M_GLX_SUPPORT M_TRUE

#define M_DEBUG true

/*
   M naming conventions:
   M objects :
   M_<indiviual name>_[<base class name>_]_o
   M_stock_value_o

   M object methods :
   returning value of a non-public variable:
   _<variable name>
   _amount()

   setting value of a non-public variable:
   set_<variable name>
   set_amount( int a )

   M object methods :
   <UP-lo case>
   Sum

   "loneley" (non-object) M functions :
   M_<UP-lo case>
   Msum

   M variables :
   compond name variables, as runnId
   <lower case><Up-lo case>

   single name variables, as ll
   <lower case>



   */

#endif /* _M_MAIN_H_ */

