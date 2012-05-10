/**\file ntg_random_graph_obj.cpp - NTG_random_graph_o class implementation
 * by Ahmed Aldo Faisal &copy; created 10.10.2000
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

#include "ntg_random_graph_obj.h"

/* 2DO revise NT uniform dist class to do the job */
class Randint {
	unsigned long randx;
public:
	Randint(long s = 0) {
		randx = s;
	}
	void seed(long s) {
		randx = s;
	}

	int abs(int x) {
		return (x & 0x7fffffff);
	}
	static double max() {
		return (2147483648.0);
	}
	int draw() {
		return (randx = randx * 1103515245 + 12345);
	}
	double fdraw() {
		return (abs(draw()) / max());
	}
	int operator()() {
		return (abs(draw()));
	}
};

/* 2DO revise NT uniform dist class to do the job */
class Urand: public Randint {
	int n;
public:
	Urand(int nn) {
		n = nn;
	}
	int operator()() {
		int r = (int) n * fdraw();
		return ((r == n) ? n - 1 : r);
	}
};

/* ***      CONSTRUCTORS	***/

void NTG_random_graph_o::InitFullyConnectedGraph(mbase::Msize numInitialNodes,
		const NTG_node_o & nodeProto,
		const NTG_edge_o __attribute__((unused)) & edgeProto) {

	for (mbase::Msize ll = 0; ll < numInitialNodes; ll++) {
		if (AddNode(nodeProto) == 0)
			std::cerr
					<< "NTG_random_graph_o::NTG_random_graph_o - Error: An error occured while adding the nodes to the graph."
					<< std::endl;
	}

	NTG_NodeIdList idList = NodeIdList();
	NTG_NodeIdList::const_iterator listIter;
	NTG_NodeIdList::const_iterator listIterTarget;

	for (listIter = idList.begin(); listIter != idList.end(); listIter++) {
		for (listIterTarget = idList.begin(); listIterTarget != idList.end();
				listIterTarget++) {
			if ((*listIter) != (*listIterTarget)) {
				if (Connect(*listIter, *listIterTarget) != mbase::M_SUCCESS)
					std::cerr
							<< "NTG_random_graph_o::NTG_random_graph_o - Error: An error occured while connecting the graph."
							<< std::endl;
			}
		}
	}

}

/** Create a NTG_random_graph_o */
/** @short Generate fully connected graph */
NTG_random_graph_o::NTG_random_graph_o(mbase::Msize numInitialNodes,
		const NTG_node_o & nodeProto, const NTG_edge_o & edgeProto) :
		NTG_graph_o() {
	p = 1;

	if (0 == numInitialNodes)
		return;
	InitFullyConnectedGraph(numInitialNodes, nodeProto, edgeProto);
}

/** Create a NTG_random_graph_o */
NTG_random_graph_o::NTG_random_graph_o(mbase::Msize numInitialNodes,
		mbase::Mreal connectivityProbability, const NTG_node_o & nodeProto,
		const NTG_edge_o & edgeProto) :
		NTG_graph_o() {
	/* init */
	p = connectivityProbability;
	if (0 == numInitialNodes)
		return;

	InitFullyConnectedGraph(numInitialNodes, nodeProto, edgeProto);
	//std::cerr << "NumNodes="<< NumNodes() <<" NumEdges="<< NumEdges() << std::endl;

	/* dilute fully connected graph */
	mbase::Uniform_rnd_dist connectRndGen(1, NumEdges());
	mbase::Msize numDel = (mbase::Msize) (NumEdges()
			* (1 - connectivityProbability));

	NTG_EdgeContainer edgeList = EdgeList();

	random_shuffle(edgeList.begin(), edgeList.end()); /* 2DO Lets hope that the random shuffle is good enough. */
	edgeList.resize(numDel);
	random_shuffle(edgeList.begin(), edgeList.end());

	if (edgeList.size() != numDel)
		std::cerr
				<< "NTG_random_graph_o::NTG_random_graph_o - Error : Delete list size not equal 	to number of edges to be deleted."
				<< std::endl;
	NTG_EdgeContainer::const_iterator delListIter;
	for (delListIter = edgeList.begin(); delListIter != edgeList.end();
			delListIter++) {
		Disconnect((*delListIter).Source(), (*delListIter).Target());
	}

}

/* ***      COPY AND ASSIGNMENT	***/
NTG_random_graph_o::NTG_random_graph_o(
		const NTG_random_graph_o __attribute__((unused)) & original) :
		NTG_graph_o() {
// add assignment code here
}

const NTG_random_graph_o&
NTG_random_graph_o::operator=(const NTG_random_graph_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
NTG_random_graph_o::~NTG_random_graph_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
mbase::Mreturn NTG_random_graph_o::AddRandomNode(
		const NTG_node_o __attribute__((unused)) & nodeProto) {
	std::cerr << "NOT IMPLEMENTED" << std::endl;
	return (mbase::M_SUCCESS);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
mbase::Mreturn NTG_random_graph_o::AddRandomEdge(
		const NTG_edge_o __attribute__((unused)) & edgeProto) {
	std::cerr << "NOT IMPLEMENTED" << std::endl;
	return (mbase::M_SUCCESS);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
