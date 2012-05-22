/**\file ntg_random_ring_graph_obj.cpp - NTG_random_ring_graph_o class implementation
 * by Ahmed Aldo Faisal &copy; created 14.10.2000
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

#include "ntg_random_ring_graph_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTG_random_ring_graph_o */
NTG_random_ring_graph_o::NTG_random_ring_graph_o(mbase::Msize nodes,
		mbase::Msize neighbourhoodRange, mbase::Real reconnectProbability,
		bool deterministic, const NTG_node_o & nodeProto,
		const NTG_edge_o & edgeProto) :
		NTG_neighbour_ring_graph_o(nodes, neighbourhoodRange, nodeProto,
				edgeProto) {
	mbase::Uniform_rnd_dist rndGen(0, 1);

	reconnectionP = reconnectProbability;
	NTG_EdgeContainer edgeList = EdgeList();
	mbase::Msize numDelEdges = 0;

	determDeletion = deterministic;

	if (true == determDeletion) {
		/* This code defines a deterministic rewiring of randomly selected edges,
		 * thus p is the fraction of nodes which are rewired (which are choson randomly) */
		numDelEdges = (mbase::Msize) mbase::Mround(NumEdges() * reconnectionP / 2.0);
		random_shuffle(edgeList.begin(), edgeList.end());
		NTG_EdgeContainer::const_iterator edgeIter = edgeList.begin();
		for (mbase::Msize ll = 0; ll < numDelEdges; ll++) {
			//			cerr << (*edgeIter).Source() <<  " "<< (*edgeIter).Target() << std::endl;
			if (true
					== IsConnected((*edgeIter).Source(),
							(*edgeIter).Target())) {
				SymmetricDisconnect((*edgeIter).Source(), (*edgeIter).Target());
			} else {
				ll--;
			}
			edgeIter++;
		}
	} else {
		/* This code determines for each edge if it should be deleted according to a probability
		 * law */
		numDelEdges = 0;
		NTG_EdgeContainer::const_iterator edgeIter;
		for (edgeIter = edgeList.begin(); edgeIter != edgeList.end();
				edgeIter++) {
			if (rndGen.RndVal() < reconnectionP / 2.0) {
				if (true
						== IsConnected((*edgeIter).Source(),
								(*edgeIter).Target())) {
					SymmetricDisconnect((*edgeIter).Source(),
							(*edgeIter).Target());
					numDelEdges++;
				}
			}
		}
	}

	mbase::Msize counter = 0;
	mbase::Mid a, b;

	while (counter < numDelEdges) {
		do {
			do {
				a = RandomNodeId();
				b = RandomNodeId();
				// cerr << " " << a << " " << b << std::endl;
			} while (a == b);
		} while (true == IsConnected(a, b));

		if (mbase::M_SUCCESS != SymmetricConnect(a, b))
			std::cerr
					<< "NTG_random_ring_graph_o::NTG_random_ring_graph_o - Error : Random reconnection failed."
					<< std::endl;
		counter++;
	}
}

/* ***      COPY AND ASSIGNMENT	***/
NTG_random_ring_graph_o::NTG_random_ring_graph_o(
		const NTG_random_ring_graph_o & original) :
		NTG_neighbour_ring_graph_o(original) {
// add assignment code here
}

const NTG_random_ring_graph_o&
NTG_random_ring_graph_o::operator=(const NTG_random_ring_graph_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
NTG_random_ring_graph_o::~NTG_random_ring_graph_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown

 void
 NTG_random_ring_graph_o::() const
 {
 }
 */

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
