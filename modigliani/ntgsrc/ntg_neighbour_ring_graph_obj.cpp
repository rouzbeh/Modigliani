/**\file ntg_neighbour_ring_graph_obj.cpp - NTG_neighbour_ring_graph_o class implementation
 * by Ahmed Aldo Faisal &copy; created 13.10.2000
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

/* $Id: ntg_neighbour_ring_graph_obj.cpp,v 1.1 2001/06/29 13:16:58 face Exp $
 * $Log: ntg_neighbour_ring_graph_obj.cpp,v $
 * Revision 1.1  2001/06/29 13:16:58  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/11/06 10:37:37  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/11/04 10:32:18  face
 * *** empty log message ***
 *
 * Revision 1.1  2000/10/15 19:21:38  face
 * *** empty log message ***
 *

 */
#include "ntg_neighbour_ring_graph_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTG_neighbour_ring_graph_o */
NTG_neighbour_ring_graph_o::NTG_neighbour_ring_graph_o(NTsize nodes,
		NTsize neighbourhoodRange, const NTG_node_o & nodeProto,
		const NTG_edge_o __attribute__((unused)) & edgeProto) :
		NTG_undirected_graph_o() {
	NT_ASSERT(neighbourhoodRange > 0);
	radius = neighbourhoodRange;
	if (radius * 2 > nodes)
		cerr
				<< "NTG_neighbour_ring_graph_o::NTG_neighbour_ring_graph_o - Error : not enough nodes to allow that radius length."
				<< endl;

	NT_ASSERT(nodes > 0);
	for (NTsize ll = 0; ll < nodes; ll++) {
		if (0 == AddNode(nodeProto))
			cerr
					<< "NTG_neighbour_ring_graph_o::NTG_neighbour_ring_graph_o - Error : Node adding generated an error."
					<< endl;
	}

	NTG_NodeIdList nodeList = NodeIdList();
	NTG_NodeIdList::iterator nodeIter;

	vector<NTG_NodeIdList::iterator> iterVec(radius);

	for (nodeIter = nodeList.begin(); nodeIter != nodeList.end(); nodeIter++) {

		for (NTsize lli = 1; lli <= radius; lli++) {
			iterVec[lli - 1] = nodeIter;

			/* "modulo" operation on a node list */
			for (NTsize llr = 1; llr <= lli; llr++) {
				(iterVec[lli - 1])++;
				if (nodeList.end() == iterVec[lli - 1])
					iterVec[lli - 1] = nodeList.begin();
			}
			if (NT_SUCCESS != SymmetricConnect(*nodeIter, *(iterVec[lli - 1])))
				cerr
						<< "NTG_neighbour_ring_graph_o::NTG_neighbour_ring_graph_o(NTsize nodes, NTsize neighbourhoodRange,NTG_node_o & nodeProto, NTG_edge_o & edgeProto ) : NTG_graph_o() - Error : Error creating symmetric  neighbour ring connection at radius="
						<< lli << "." << endl;
			// cerr <<"NTG_neighbour_ring_graph_o::NTG_neighbour_ring_graph_o - Talk : Connected ("<<*nodeIter<<","<<*(iterVec[lli-1])<<")." << endl;
		}

	}

}

/* ***      COPY AND ASSIGNMENT	***/
NTG_neighbour_ring_graph_o::NTG_neighbour_ring_graph_o(
		const NTG_neighbour_ring_graph_o __attribute__((unused)) & original) :
		NTG_undirected_graph_o() {
// add assignment code here
}

const NTG_neighbour_ring_graph_o&
NTG_neighbour_ring_graph_o::operator=(
		const NTG_neighbour_ring_graph_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
// add assignment code here
	return *this;
}

/* ***      DESTRUCTOR		***/
NTG_neighbour_ring_graph_o::~NTG_neighbour_ring_graph_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown

 void
 NTG_neighbour_ring_graph_o::() const
 {
 }
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */