/**\file ntg_matrix_graph_obj.h - NTG_matrix_graph_o class header
 * by Ahmed Aldo Faisal &copy; created 30.9.2000
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
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


/* $Id: ntg_matrix_graph_obj.h,v 1.1 2001/06/29 13:16:58 face Exp $
* $Log: ntg_matrix_graph_obj.h,v $
* Revision 1.1  2001/06/29 13:16:58  face
* *** empty log message ***
*
* Revision 1.2  2000/10/03 08:30:01  face
* *** empty log message ***
*
* Revision 1.1  2000/10/01 09:33:22  face
* added for the first time
*

*/
#ifndef _ntg_matrix_graph_obj_h_
#define _ntg_matrix_graph_obj_h_

/* NT core includes */
/*#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"  */
/* NT includes */
#include "ntg_node_obj.h"
#include "ntg_edge_obj.h"

/* other includes */
#include <vector>
#include "tnt/tnt.h"
#include "tnt/vec.h"
#include "tnt/cmat.h"

using namespace TNT;
using namespace std;

typedef Matrix<int> IntMatrix;

/** @short NTG_matrix_graph_o class
\bug unknown
\warning unknown
*/
class NTG_matrix_graph_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NTG_matrix_graph_o(unsigned int newNumNodes=1);
    NTG_matrix_graph_o(const NTG_matrix_graph_o & original);
    const NTG_matrix_graph_o & operator= (const NTG_matrix_graph_o & right);
    virtual ~NTG_matrix_graph_o();
    /* ***  Methods              ***/
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
};

#endif /* _ntg_matrix_graph_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */