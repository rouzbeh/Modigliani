/* nt_codes.h - defintions of codes*/
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


/* $Id: nt_codes.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_codes.h,v $
 * Revision 1.1  2001/06/11 13:18:56  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/10/03 08:30:03  face
 * *** empty log message ***
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 2.4  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 2.3  1999/06/06 23:51:28  face
 * *** empty log message ***
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
 * Revision 1.3  1998/05/24 18:05:28  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/24 11:23:29  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/05/22 16:44:49  face
 * Initial revision
 *
 */


#ifndef _NT_CODES_H_
#define _NT_CODES_H_

#include "nt_main.h"
//#include "nt_types.h"

/** CLASS CODES */
enum NTclassCode {
    /** NT class */
    NT_O = 1,
    /** NT3D classes */
    NT3D_O = 1000000
};

/** image/bitmap related  CODES */
enum NTimageFileType {NT_AUTO, NT_RAW, NT_NT=1, NT_PPM, NT_BMP, NT_TGA, NT_GIF, NT_JPG, NT_PNG, NT_UNKNOWN};
enum NTimageDataFormat {NT_SINGLE_CHANNEL = 1, NT_RGB = 3, NT_RGBA = 4,
                        NT_RED, NT_GREEN, NT_BLUE, NT_ALPHA
                       };
/* SINGLE-channel corresponds to a single "gray" channel */

/** COUNTRY CODES == intl. phone code */
enum NTcountryCode { NT_EU			=-1,
                     NT_USA			=1,
                     NT_ITALY		=39,
                     NT_UK			=44,
                     NT_GERMANY		=49
                   } ;
/*
#define NT_EU  (-1)
#define NT_USA (1) // ?
#define NT_ITALY (39)
#define NT_UK    (44)
#define NT_GERMANY (49)
*/

/* *** CURRENCY NAMES *** */
#define NT_EURO_CURRENCY "Euro"
#define NT_USA_CURRENCY  "US-Dollar"
#define NT_ITALY_CURRENCY "Lira Italiane"
#define NT_GERMANY_CURRENCY "Deutsche Mark"


enum NTstockExchangeId {  NT_STKEXC_UNDEF		=0,
                          /* USA */
                          NT_STKEXC_NYSE		=1000,
                          NT_STKEXC_NASDAQ	=2000,
                          /* EUROPE */
                          NT_STKEXC_FTSE		=10000,
                          NT_STKEXC_FSE		=20000
                       } ;

//#define NT_STKEXC_UNDEF 0

//#define NT_STKEXC_NYSE 1000
//#define NT_STKEXC_NASDAQ 2000

//#define NT_STKEXC_FTSE 10000 // London ?
//#define NT_STKEXC_FSE 20000 // Frankfurt ?

/* *** VALUE OBJ TYPE ID *** */
#define NT_UNDEF 0
#define NT_CURRENCY 100
#define NT_STOCK_VALUE 200


#endif /* _NT_CODES_H_ */


