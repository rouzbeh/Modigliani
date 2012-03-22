/* nt_stock_value_obj.h - base class for stock exchangable values */
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

/*
 * $Id: nt_stock_value_obj.h,v 1.1 2001/06/11 13:18:56 face Exp $
 * $Log: nt_stock_value_obj.h,v $
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
 * Revision 1.6  1999/01/07 23:52:56  face
 * *** empty log message ***
 *
 * Revision 1.6  1998/12/25 21:28:17  face
 *  new NetTrader version 0.2 set with this revision of this file
 *
 * Revision 1.5  1998/06/01 19:05:33  face
 * *** empty log message ***
 *
 * Revision 1.4  1998/06/01 12:37:26  face
 * *** empty log message ***
 *
 * Revision 1.3  1998/05/28 20:33:24  face
 * *** empty log message ***
 *
 * Revision 1.2  1998/05/24 18:05:28  face
 * *** empty log message ***
 *
 * Revision 1.1  1998/05/24 11:23:29  face
 * Initial revision
 *
 */

#include "nt_value_obj.h"
#include "nt_date_obj.h"

//
//	Class name : stock_balue_o
//
/**	Bbase class for values traded at a stock exchange
 */
class NT_stock_value_o : public NT_value_o {

private:
    NTnum stockExchangeId; // where the value is traded
    NTreal currentBuyingPrice;
    NTreal currentSellingPrice;
    NT_date_o emissionDate;

protected:

public:
    NT_stock_value_o();
    NT_stock_value_o(const NT_stock_value_o &);
    ~NT_stock_value_o();

    friend ostream & operator<<(ostream & os, const NT_stock_value_o & self);
};



