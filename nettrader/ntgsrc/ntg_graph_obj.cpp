/**\file ntg_graph_obj.cpp - NTG_graph_o class implementation
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

/* $Id: ntg_graph_obj.cpp,v 1.2 2002/03/04 18:09:08 face Exp $
 * $Log: ntg_graph_obj.cpp,v $
 * Revision 1.2  2002/03/04 18:09:08  face
 * added checking for NaN and Inf values in find_spikes_in_binary_file-
 * BinomialStep: redo loop if after state change the states do not sum up
 *
 * Revision 1.1  2001/06/29 13:16:58  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/11/06 10:37:37  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/11/04 10:32:18  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/10/15 19:21:38  face
 * *** empty log message ***
 *
 * Revision 1.4  2000/10/04 15:15:28  face
 * *** empty log message ***
 *
 * Revision 1.3  2000/10/03 15:50:55  face
 * *** empty log message ***
 *
 * Revision 1.2  2000/10/03 08:30:01  face
 * *** empty log message ***
 *
 * Revision 1.1  2000/10/01 09:33:22  face
 * added for the first time
 *

 */
#include "ntg_graph_obj.h"

/* *** helper classes *** */
class NTG_graph_sort_edges_o {
public:
	int operator()(const NTG_edge_o & a, const NTG_edge_o & b) const {
		return (a.Source() < b.Source());
	}
};

/* ***      CONSTRUCTORS	***/
/** Create a NTG_graph_o */
NTG_graph_o::NTG_graph_o() :
		nodeIdManager(1, NTG_MAX_NODES_IN_GRAPH) {

	cerr
			<< "NTG_graph_o - WARNING : The entire code of this class should be checked as changes where "
			<< " necessary to make it compile on gcc 3.0. especially the handling of NULL pointer on iterators should"
			<< " be removed and replaced by more STL-style algorithms. AAF, 26.2.2002. "
			<< endl;

	V.resize(0);
	E.resize(0);
	UpdateStatistics();
}

/* ***      COPY AND ASSIGNMENT	***/
NTG_graph_o::NTG_graph_o(const NTG_graph_o & original) :
		NT_o(), nodeIdManager(original.nodeIdManager) {
// add assignment code here
	E = original.E;
	V = original.V;
	UpdateStatistics();
}

const NTG_graph_o&
NTG_graph_o::operator=(const NTG_graph_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
// add assignment code here
	nodeIdManager = right.nodeIdManager;
	E = right.E;
	V = right.V;
	UpdateStatistics();
	return *this;
}

/* ***      DESTRUCTOR		***/
NTG_graph_o::~NTG_graph_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short
 @param
 @return    NTid of added node, 0 on any error;
 \warning    unknown
 \bug        unknown
 */
NTid NTG_graph_o::AddNode(const NTG_node_o & newNode) {
	NTid tmp = nodeIdManager.IssueId();
	if (tmp < 1) {
		cerr
				<< "NTG_graph_o::AddNode - Error : nodeIdManager returned error state."
				<< endl;
		return 0;
	}
	V.push_back(NTG_vertex_o(tmp, newNode));
	UpdateStatistics();
	/* 2DO check if issued id is not already present. however, do not use InGraph, as that relys on nodeIdManager. */
	return tmp;
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::Connect(NTid newSource, NTid newTarget) {
	NTG_edge_o tmp;
	return Connect(newSource, newTarget, tmp);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::Connect(NTid newSource, NTid newTarget,
		NTG_edge_o newEdge) {
	NTbool error = false;
	if (false == InGraph(newSource)) {
		cerr << "NTG_graph_o::Connect - Error : Not existing source node id="
				<< newSource << " specified." << endl;
		error = true;
	}
	if (false == InGraph(newTarget)) {
		cerr << "NTG_graph_o::Connect - Error : Not existing target node id="
				<< newTarget << " specified." << endl;
		error = true;
	}
	if (true == error)
		return NT_NOT_EXISTING_NODE;
	newEdge.SetConnection(newSource, newTarget);
	return AddEdge(newEdge);
}

NTG_EdgeContainer NTG_graph_o::EdgeList() const {
	return E;
}

NTG_NodeIdList NTG_graph_o::NodeIdList() const {
	NTG_NodeContainer::const_iterator nodeIter;

	vector<NTid> tmpVec;
	tmpVec.resize(0);

	for (nodeIter = V.begin(); nodeIter != V.end(); nodeIter++) {
		tmpVec.push_back((*nodeIter).first);
	}

	return tmpVec;
}

/** @short      Make symmetric connections
 @param      none
 @return     NT_SUCCESS if successful, otherwise error code of connect (a,b) first or connect (b,a) second,
 note that both can be unsuccessfull but error code of (a,b) is only returned in ths case.
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::SymmetricConnect(NTid a, NTid b) {
	NTG_edge_o tmp;
	return SymmetricConnect(a, b, tmp);

}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::SymmetricConnect(NTid a, NTid b,
		const NTG_edge_o & newEdge) {
	NTreturn tmp1 = Connect(a, b, newEdge);
	NTreturn tmp2 = Connect(b, a, newEdge);

	if (NT_SUCCESS != tmp1)
		return tmp1;
	else if (NT_SUCCESS != tmp2)
		return tmp2;
	return NT_SUCCESS;
}

NTsize NTG_graph_o::NumNodes() const {
	return numNodes;
}
NTsize NTG_graph_o::NumEdges() const {
	return numEdges;
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
void NTG_graph_o::ShowStatistics() {
	UpdateStatistics();
	cout << "The graph has " << NumNodes() << " nodes and " << NumEdges()
			<< " edge entries.";

	NTG_EdgeContainer::iterator itE;
	for (itE = E.begin(); itE != E.end(); itE++) {
		cout << "(" << (*itE).Source() << "," << (*itE).Target() << ")";
	}
	cout << endl;
	/*	for (itV = V.begin(); itV != V.end(); itV++){

	 cout << "Node with id=" << (*itV).first << endl ;

	 cout << "   ... is neighbour to the following nodes ";
	 for (itE = E.begin(); itE != E.end(); itE++){
	 if ( (*itE).Source() == (*itV).first ) cout << (*itE).Target();
	 }
	 cout << endl;

	 cout << "   ... is neighboured by the following nodes ";
	 for (itE = E.begin(); itE != E.end(); itE++){
	 if ( (*itE).Target() == (*itV).first ) cout << (*itE).Source();
	 }
	 cout << endl;
	 }
	 */
}

/** @short     Remove directed link (possibly multiples too)
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::Disconnect(NTid newSource, NTid newTarget) {

	NTbool conn = IsConnected(newSource, newTarget);
	if (false == conn) {
		cerr
				<< "NTG_graph_o::Disconnect - Error : Attempts to disconnect non-existing edge ("
				<< newSource << "," << newTarget << ").";
		return NT_NOT_EXISTING_NODE;
	}

//	cerr << "NTG_graph_o::Disconnect - Talk: currently "<<E.size() <<" in list. Deleting..." << endl;
	NTG_EdgeContainer::iterator itE;
	itE = EdgeIterator(newSource, newTarget);

	cerr << "commented out line for compiling (gcc 3.0.1) does the code work"
			<< endl;
//	if (  NULL == itE ) {
//	cerr << "Failed edge iterator, was looking for ("<<newSource<<","<<newTarget<<")." << endl;
//	} else {
	E.erase(itE);
//	cerr << "Edge  ("<<newSource<<","<<newTarget<<") deleted." << endl;
//	}

//	cerr << "NTG_graph_o::Disconnect - Talk: currently "<< E.size() <<" in list." << endl;
	UpdateStatistics();
	return NT_SUCCESS;
}

/** @short     Remove links in both directions (multiples and directed ones too)
 @param     target and source node
 @return    NT_SUCCESS, if in Disconnect(a,b) or (b,a) an error occurred that error code
 is returned, if in both Disconnects an error occurred that of (a,b)
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::SymmetricDisconnect(NTid a, NTid b) {
	NTreturn returnVal1, returnVal2;
	returnVal1 = Disconnect(a, b);
	returnVal2 = Disconnect(b, a);

	if (returnVal1 != returnVal2) {
		if (NT_SUCCESS == returnVal1)
			return returnVal2;
		else if (NT_SUCCESS == returnVal2)
			return returnVal1;
		else {
			cerr
					<< "NTG_graph_o::SymmetricDisconnect - Local invalid state 0 occurred"
					<< endl;
			return NT_FAIL;
		}
	} else if (returnVal1 == returnVal2) {
		if (NT_SUCCESS == returnVal1)
			return NT_SUCCESS;
		else
			return returnVal1;
	} else {
		cerr
				<< "NTG_graph_o::SymmetricDisconnect - Local invalid state 1 occurred"
				<< endl;
		return NT_FAIL;
	}
}

/** @short
 @param
 @return    true if NTid is present in Graph
 \warning    unknown
 \bug        unknown
 */
bool NTG_graph_o::InGraph(NTid selNode) const {
	bool idManagerResponse = nodeIdManager.IsTaken(selNode);
	bool isPresent = false;

	/* 2DO (optional): this is a slow but safe consistency check, possibly remove */
	NTG_NodeContainer::const_iterator itV;
	for (itV = V.begin(); itV != V.end(); itV++) {
		if ((*itV).first == selNode) {
			if (true == isPresent) {
				cerr
						<< "NTbool NTG_graph_o::InGraph - Error : Node id present multiple times in Vertex list."
						<< endl;
				return false;
			} else {
				isPresent = true;
			}
		}
	}

	if (idManagerResponse != isPresent) {
		cerr
				<< "NTbool NTG_graph_o::InGraph - Error : Mismatch, node id manager says "
				<< idManagerResponse << " while node list search results in "
				<< isPresent << ".\n";
		return false;
	} else {
		return idManagerResponse;
	}
}

/** @short
 @param
 @return
 \warning   Assumes simplex graph 2DO differentiate simplex and non-simplex graphs
 \bug
 */
NTG_NodeDistanceMatrix NTG_graph_o::NodeDistanceMatrix(
		const NTG_NodeDistanceMatrix & aMtr) const {
	NT_ASSERT(
			( (NTsize) aMtr.num_rows() == NumNodes() ) && ( (NTsize)aMtr.num_cols() ==NumNodes()));
	//cerr << "NTG_graph_o::NodeDistanceMatrix  - Talk : Entered method."<< endl;
	NTG_NodeDistanceMatrix dMtr(aMtr);

	Matrix<bool> foundMtr(NumNodes(), NumNodes(), false);
	//cerr << "NTG_graph_o::NodeDistanceMatrix  - Talk : Init done. NumNodes ="<< NumNodes()<< endl;

	NTreal minVal;
	NTsize minNode = 0;

	//cout << aMtr;
	//cout << dMtr;
	//cout << foundMtr;

	NTsize lc, lr, ll;
	for (lr = 0; lr < NumNodes(); lr++) {

		foundMtr[lr][lr] = true;
		dMtr[lr][lr] = 0;

		for (ll = 0; ll < NumNodes(); ll++) {
			minVal = 1 / 0.0; /* set value to infinity (NAN) , this is correct code ! */

			for (lc = 0; lc < NumNodes(); lc++) {
				if (false == foundMtr[lr][lc]) {
					if (dMtr[lr][lc] < minVal) {
						//cout << "Shorter path possible: " << lr << "\t" << lc << endl;
						minNode = lc;
						minVal = dMtr[lr][lc];
					}
				}
			}
			foundMtr[lr][minNode] = true;
			//cout << "Found minimum node: " << minNode << "\t" << minVal << endl;
			for (lc = 0; lc < NumNodes(); lc++) {
				if (false == foundMtr[lr][lc]) {
					if ((minVal + aMtr[minNode][lc]) < (dMtr[lr][lc])) {
						//cout << "Shorter path found between:" <<  minNode << "\t" << lc << endl;
						dMtr[lr][lc] = minVal + aMtr[minNode][lc];
					}
				}
			}
		}
	}
	return dMtr;
}

/** @short   Statistics of connectivity based on adjacency distance
 @param   none
 @return  statiscs of node-to-node distances based on adjacency, i.e. two neighbouring nodes
 have a distance of 1, infinity otherwise
 \warning   Assumes simplex graph
 \bug        2DO differentiate simplex and non-simplex graphs
 */
NT_sequential_statistics_o NTG_graph_o::AdjacencyDistanceStatistics() {
	NTG_NodeDistanceMatrix dMtr = NodeDistanceMatrix(
			NodeDistanceMatrixConditioning(AdjacencyMatrix()));
	NT_sequential_statistics_o oStats;

	NT_ASSERT(NTsize(dMtr.size()) == NTsize(NumNodes()*NumNodes()));
	/* simple test, 2DO improve */
	NTsize lr, lc;
	for (lr = 0; lr < NumNodes(); lr++) {
		for (lc = 0; lc < NumNodes(); lc++) {
			oStats.Add(dMtr[lr][lc]);
		}
	}

	return oStats;
}

NTbool NTG_graph_o::IsConnected(NTid source, NTid target) const {
	if (InGraph(source) && InGraph(target)) {
		NTG_EdgeContainer::const_iterator itE;
		for (itE = E.begin(); itE != E.end(); itE++) {
			if (((*itE).Source() == source) && ((*itE).Target() == target))
				return true;
		}
		return false;
	} else {
		cerr
				<< "NTG_graph_o::IsConnected - Warning : Source or Target node id do not exist in graph. Returning 'false'.";
		return false;
	}
}

/** @short     Check if two nodes in the graph are connected bidirectionally
 @param     nodes
 @return    true, else false (also if error occurred, but with error message)
 \warning    It does not check whether two nodes are connected at all in either direction.
 \bug        unknown
 */
NTbool NTG_graph_o::IsSymmetricConnected(NTid a, NTid b) const {
	NTbool oneway = false;
	NTbool otherway = false;
	NTG_EdgeContainer::const_iterator itE;
	if (InGraph(a) && InGraph(b)) {
		for (itE = E.begin(); itE != E.end(); itE++) {
			if (((*itE).Source() == a) && ((*itE).Target() == b))
				oneway = true;
			else if (((*itE).Source() == b) && ((*itE).Target() == a))
				otherway = true;
		}
		return (oneway && otherway);
	} else {
		cerr
				<< "NTG_graph_o::Connected - Warning : Source or Target node in graph. Returning 'false'.";
		return false;
	}
}

/** @short     Remove links in both directions (multiples and directed ones too)
 @param     target and source node
 @return    NT_SUCCESS, if in Disconnect(a,b) or (b,a) an error occurred that error code
 is returned, if in both Disconnects an error occurred that of (a,b)
 \warning    unknown
 \bug        unknown
 */
NTreturn NTG_graph_o::DeleteNode(NTid delNode) {
	if (InGraph(delNode) != true)
		return NT_NOT_EXISTING_NODE;
	DisconnectNode(delNode);

	NTG_NodeContainer::iterator itV = VertexIterator(delNode);
//	if (NULL == itV) cerr << "NTG_graph_o::DeleteNode - Error : No iterator returned." << endl;
	V.erase(itV); /* get the new iterator as the RHS one is invalidated */
	nodeIdManager.FreeId(delNode); /* VERY important */

	if (InGraph(delNode) == false) {
		UpdateStatistics();
		return NT_SUCCESS;
	} else {
		UpdateStatistics();
		cerr
				<< "NTG_graph_o::DeleteNode - Error : InGraph() returned true, but node was not found in vertex list."
				<< endl;
		return NT_NOT_EXISTING_NODE;
	}

}

/** @short      print the adjacency matrix
 node that row, column position correspond to target, source edges
 the position values correpsond to the node id. I.e. non-existing nodes
 taking up intermediate values of node id are thus plotted in the adjacency matrix,
 however not really present.
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
void NTG_graph_o::ShowAdjacencyMatrix() {
	Matrix<int> aMtr(nodeIdManager.LargestIssuedId() + 1,
			nodeIdManager.LargestIssuedId() + 1, 0);
	sort(E.begin(), E.end(), NTG_graph_sort_edges_o()); //2DO is this really needed ?

	for (int ll = 1; ll <= nodeIdManager.LargestIssuedId(); ll++) {
		if (InGraph(ll) == true) {
			aMtr[ll][0] = ll;
			aMtr[0][ll] = ll;
		} else {
			aMtr[ll][0] = -1;
			aMtr[0][ll] = -1;
			for (NTid ls = 0; ls <= nodeIdManager.LargestIssuedId(); ls++) {
				aMtr[ll][ls] = -1;
				aMtr[(int) ls][ll] = -1;
			}
		}
	}

	NTG_EdgeContainer::iterator itE;
	for (itE = E.begin(); itE != E.end(); itE++) {
		(aMtr[(int) (*itE).Source()][(int) (*itE).Target()])++;
	}
	cout
			<< "Showing adjacency matrix (first row and column indicate node id, negative values indicate unused node ids)."
			<< endl;
	cout << aMtr;
}

NTG_AdjacencyMatrix NTG_graph_o::AdjacencyMatrix() {
	NTG_AdjacencyMatrix aMtr(nodeIdManager.LargestIssuedId(),
			nodeIdManager.LargestIssuedId(), false);
	sort(E.begin(), E.end(), NTG_graph_sort_edges_o()); // 2DO really needed ?

	NTG_EdgeContainer::iterator itE;
	for (itE = E.begin(); itE != E.end(); itE++) {
		//	cerr << "Edge  ("<<(*itE).Source()<<","<<(*itE).Target()<<") touched." << endl;
		(aMtr[(int) (*itE).Source() - 1][(int) (*itE).Target() - 1]) = true;
	}

	return aMtr;
}

NTsize NTG_graph_o::NumConvergentConnections(NTid id) const {
	NTG_EdgeContainer::const_iterator itE;

	NTsize counter = 0;
	for (itE = E.begin(); itE != E.end(); itE++) {
		if ((*itE).Target() == id)
			counter++;
	}

	return counter;
}

NTG_EdgeContainer NTG_graph_o::ConvergentConnections(NTid id) const {
	NTG_EdgeContainer::const_iterator itE;

	NTG_EdgeContainer tmpE;
	tmpE.resize(0);

	for (itE = E.begin(); itE != E.end(); itE++) {
		if ((*itE).Target() == id)
			tmpE.push_back(*itE);
	}

	return tmpE;
}

NTG_EdgeContainer NTG_graph_o::DivergentConnections(NTid id) const {
	NTG_EdgeContainer::const_iterator itE;

	NTG_EdgeContainer tmpE;
	tmpE.resize(0);

	for (itE = E.begin(); itE != E.end(); itE++) {
		if ((*itE).Source() == id)
			tmpE.push_back(*itE);
	}

	return tmpE;

}

NTid NTG_graph_o::RandomNodeId() const {
	NTid selNode;
	if (NumNodes() == 0)
		return 0;
	if (NumNodes() == 1)
		return (*(V.begin())).first;
	else {
		do {
			selNode = (NTid) NTround(rndNodeGen.RndVal());
		} while (false == InGraph(selNode));
		return selNode;
	}
}

/* ***  PROTECTED                         ***   */
NTreturn NTG_graph_o::DisconnectNode(NTid node) {
	NTreturn di, co;
	di = DisconnectDivergence(node);
	co = DisconnectConvergence(node);

	if (di != NT_SUCCESS)
		return di;
	else if (co != NT_SUCCESS)
		return co;

	UpdateStatistics();
	return NT_SUCCESS;
}

NTreturn NTG_graph_o::DisconnectDivergence(NTid source) {
	NTG_EdgeContainer tmpE = DivergentConnections(source);
	NTG_EdgeContainer::iterator tmpEit;

	NTid tmpId;

	for (tmpEit = tmpE.begin(); tmpEit != tmpE.end(); tmpEit++) {
		tmpId = (*tmpEit).Target();
		ShowStatistics();
		Disconnect(source, tmpId);
	}

	UpdateStatistics();

	return NT_SUCCESS; /* 2DO implement check */

}

NTreturn NTG_graph_o::DisconnectConvergence(NTid target) {
	NTG_EdgeContainer tmpE = ConvergentConnections(target);
	NTG_EdgeContainer::iterator tmpEit;

	NTid tmpId;

	for (tmpEit = tmpE.begin(); tmpEit != tmpE.end(); tmpEit++) {
		tmpId = (*tmpEit).Source();
		ShowStatistics();
		Disconnect(tmpId, target);
	}

	UpdateStatistics();

	return NT_SUCCESS; /* 2DO implement check */
}

/* ***  PRIVATE                           ***   */

/** @short     Return iterator to corresponding node id.
 @param     chosen node
 @return    iterator on node, else NULL if node not in graph or error (with text comment).
 \warning    unknown
 \bug        unknown
 */
NTG_NodeContainer::iterator NTG_graph_o::VertexIterator(NTid id) {
//	if (false == InGraph(id)) return NULL;

	NTG_NodeContainer::iterator itV;
	for (itV = V.begin(); itV != V.end(); itV++) {
		if ((*itV).first == id)
			return itV;
	}
	cerr
			<< "NTG_graph_o::NodeIterator - Error : Control flow should not have reached here."
			<< endl;
	return itV;
}

/** @short     Return iterator to corresponding node id.
 @param     chosen node
 @return    iterator on node, else NULL if node not in graph or error (with text comment).
 \warning    unknown
 \bug        unknown
 */
NTG_EdgeContainer::iterator NTG_graph_o::EdgeIterator(NTid source,
		NTid target) {
//	if (IsConnected(source,target) == false) return NULL;
	NTG_EdgeContainer::iterator itE;
	for (itE = E.begin(); itE != E.end(); itE++) {
		if (((*itE).Source() == source) && ((*itE).Target() == target))
			return itE;
	}
	cerr
			<< "NTG_graph_o::EdgeIterator - Error : Control flow should not have reached here."
			<< endl;
	return itE;
}

NTreturn NTG_graph_o::AddEdge(const NTG_edge_o & newEdge) {
	E.push_back(newEdge);
	UpdateStatistics();
	return NT_SUCCESS;
}

void NTG_graph_o::UpdateStatistics() {
	numNodes = V.size();
	numEdges = E.size();
	if (numNodes > 1)
		rndNodeGen = NT_uniform_rnd_dist_o(1, nodeIdManager.LargestIssuedId());
}

/* File skeleton generated by GenNTObj version 0.7. */
