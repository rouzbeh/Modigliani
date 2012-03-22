/* nt_main.h - main header file for NetTrader sources */
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


/* $Id: nt_main.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_main.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/10/15 19:22:58  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/06/13 20:10:28  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/07/03 16:26:21  face
 * edited and added new nt_error_o class:
 * nt_dimension_error_o
 * to report dimension mismatches between (e.g.NT_VECTOR_O)
 * bjects
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.3  1999/06/06 23:51:28  face
 * *** empty log message ***
 *
 * Revision 2.2  1999/04/24 15:02:58  face
 * 0.3
 *
 * Revision 2.1  1999/04/20 13:49:24  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/04/20 13:46:50  face
 * Initial revision
 *
 * Revision 1.8  1998/10/21 23:55:11  face
 * *** empty log message ***
 *
 * Revision 1.7  1998/06/02 20:07:34  face
 * Version 0.2
 *
 * Revision 1.6  1998/06/01 19:05:33  face
 * *** empty log message ***
 *
 * Revision 1.5  1998/06/01 12:37:26  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/05/24 11:23:29  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/22 16:44:49  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/22 12:07:04  face
 * skeleton laid
 *
 */

/* MAIN INCLUDES NOTHING !!! */

/* 2DO for some bizarre reason lrand48 is not included/implemented
 * and random_shuffle (STL) needs it or will have to use rand()
 */
#define __STL_NO_DRAND48
/* which is switched on with the above statement */

#ifndef _NT_MAIN_H_
#define _NT_MAIN_H_

#define NT_MAJOR_VERSION 0
#define NT_MINOR_VERSION 5

#define NT_STRING_LENGTH 255
#define NT_SEPARATOR " ;; "

#define NT_EXPORT extern
#define NT_TALKLEVEL 9


#define NT_GL_SUPPORT NT_TRUE
#define NT_GLX_SUPPORT NT_TRUE

#define NT_DEBUG true

/*
   NT naming conventions:
   NT objects :
   NT_<indiviual name>_[<base class name>_]_o
   NT_stock_value_o

   NT object methods :
   returning value of a non-public variable:
   _<variable name>
   _amount()

   setting value of a non-public variable:
   set_<variable name>
   set_amount( int a )

   NT object methods :
   <UP-lo case>
   Sum

   "loneley" (non-object) NT functions :
   NT_<UP-lo case>
   NTsum

   NT variables :
   compond name variables, as runnId
   <lower case><Up-lo case>

   single name variables, as ll
   <lower case>



   */

#endif /* _NT_MAIN_H_ */

