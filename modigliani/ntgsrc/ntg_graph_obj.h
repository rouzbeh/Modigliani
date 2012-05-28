/**\file ntg_graph_obj.h - NTG_vertex_o class header
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
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _ntg_graph_obj_h_
#define _ntg_graph_obj_h_

/* M core includes */
#include "mbase/main.h"
#include "mbase/types.h"
#include "mbase/Obj.h"
/* M includes */
#include "mbase/Id_management.h"
#include "ntg_node_obj.h"
#include "ntg_edge_obj.h"
#include "mbase/Uniform_rnd_dist.h"
#include "mbase/Sequential_statistics.h"
/* other includes */
#include <vector>
#include <algorithm>
#include "tnt/tnt.h"
#include "tnt/tnt_cmat.h"
#include <cstring>

using namespace TNT;

/** @short NTG_vertex_o class
 ** Concepts: the NTG_vertex_o mimics the mathematical definition
 ** of a graph it is composed by a set E (here a <vector>) of NTG_edge_o
 ** objects and of vertices (here a <vector>) V. A vertex is a pair (<pair>)
 ** composed of a vertex id and a NTG_node_o which can be derived by the user
 ** according to needs. Similar edges can be derived using an NTG_edge_o object.
 **
 \bug unknown
 \warning For digraphs: Adjacency and Connectivity matrices use A[i][j] to denote the connection
 from node i to node j. This does not correspond to standard dynamical
 system notation.
 */

#define NTG_MAX_NODES_IN_GRAPH 100000

class NTG_vertex_o {
public:

	NTG_vertex_o(mbase::Mid id = 0, NTG_node_o node = NTG_node_o(), std::string name =
			"") {
		first = id;
		second = node;
		if (name == "")
			third = first;
		else
			third = name;
	}
	mbase::Mid first;
	NTG_node_o second;
	std::string third;
};

//typedef pair<mbase::Mid,NTG_node_o> NTG_vertex_o;
typedef std::vector<NTG_vertex_o> NTG_NodeContainer;
typedef std::vector<NTG_edge_o> NTG_EdgeContainer;
typedef std::vector<mbase::Mid> NTG_NodeIdList;
typedef Matrix<mbase::Real> NTG_PositiveCostMatrix;
typedef Matrix<bool> NTG_AdjacencyMatrix;
typedef Matrix<mbase::Real> NTG_NodeDistanceMatrix;

template<class T>
NTG_NodeDistanceMatrix NodeDistanceMatrixConditioning(const Matrix<T> & aMtr) {
	NTG_NodeDistanceMatrix dMtr(aMtr.num_rows(), aMtr.num_cols());
	M_ASSERT( aMtr.size() == dMtr.size());
	for (mbase::Size_t lr = 0; lr < (mbase::Size_t) aMtr.num_rows(); lr++) {
		for (mbase::Size_t lc = 0; lc < (mbase::Size_t) aMtr.num_cols(); lc++) {
			if (lr == lc)
				dMtr[lr][lc] = 0;
			else {
				T tmpVal = aMtr[lr][lc];
				if (0 == tmpVal)
					dMtr[lr][lc] = 1 / 0.0; /* set value to infinity (NAN), this is correct code ! */
				else if ((mbase::Real) tmpVal < 0) {
					std::cerr
							<< "NodeDistanceMatrixPrecondition - Warning : Negative values in matrix encountered. Using absolute values."
							<< std::endl;
					dMtr[lr][lc] = -tmpVal;
				} else
					dMtr[lr][lc] = tmpVal;
			}
		}
	}

	return (dMtr);
}

class NTG_graph_o: public mbase::Obj {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	NTG_graph_o();
	NTG_graph_o(const NTG_graph_o & original);
	const NTG_graph_o & operator=(const NTG_graph_o & right);
	virtual ~NTG_graph_o();
	/* ***  Methods              ***/
// queries
	mbase::Mbool InGraph(mbase::Mid selNode) const;
	mbase::Mbool IsConnected(mbase::Mid source, mbase::Mid target) const;
	mbase::Mbool IsSymmetricConnected(mbase::Mid a, mbase::Mid b) const;
	mbase::Mbool GraphConsistency() {
		return (mbase::M_TRUE);
	}
	mbase::Size_t NumNodes() const;
	mbase::Size_t NumEdges() const;
	mbase::Size_t NumConvergentConnections(mbase::Mid id) const;
	NTG_NodeIdList ShortestPath() const;
	mbase::Size_t Distance(mbase::Mid a, mbase::Mid b) const;
	NTG_EdgeContainer ConvergentConnections(mbase::Mid id) const;
	NTG_EdgeContainer DivergentConnections(mbase::Mid id) const;
	NTG_NodeIdList NodeIdList() const;
	NTG_EdgeContainer EdgeList() const;
	void ShowStatistics();
	void ShowAdjacencyMatrix();
	NTG_AdjacencyMatrix AdjacencyMatrix();

	NTG_NodeDistanceMatrix NodeDistanceMatrix(
			const NTG_NodeDistanceMatrix & aMtr) const;
	mbase::Sequential_statistics AdjacencyDistanceStatistics();

// node manipulation
	mbase::Mid AddNode(const NTG_node_o & newNode);
	mbase::Mreturn DeleteNode(mbase::Mid delNode);
	mbase::Mid RandomNodeId() const;
// edge manupulation
	virtual mbase::Mreturn Connect(mbase::Mid source, mbase::Mid target);
	virtual mbase::Mreturn Connect(mbase::Mid source, mbase::Mid target,
			NTG_edge_o newEdge);
	virtual mbase::Mreturn SymmetricConnect(mbase::Mid source,
			mbase::Mid target);
	virtual mbase::Mreturn SymmetricConnect(mbase::Mid source,
			mbase::Mid target, const NTG_edge_o & newEdge);
	virtual mbase::Mreturn Disconnect(mbase::Mid source, mbase::Mid target);
	virtual mbase::Mreturn SymmetricDisconnect(mbase::Mid source,
			mbase::Mid target);
	/* ***  Data                 ***/

protected:
	/* ***  Methods              ***/
// queries
// edge manupulation
	virtual mbase::Mreturn DisconnectNode(mbase::Mid node);
	virtual mbase::Mreturn DisconnectDivergence(mbase::Mid source);
	virtual mbase::Mreturn DisconnectConvergence(mbase::Mid target);
	/* ***  Data                 ***/

private:
	/* ***  Methods              ***/
// queries
	NTG_NodeContainer::iterator VertexIterator(mbase::Mid id);
	NTG_EdgeContainer::iterator EdgeIterator(mbase::Mid a, mbase::Mid b);
	void UpdateStatistics();
// edge manipulation
	mbase::Mreturn AddEdge(const NTG_edge_o & newEdge);
	/* ***  Data                 ***/
	mbase::Size_t numNodes;
	mbase::Size_t numEdges;
	mbase::Id_management nodeIdManager;
	mbase::Uniform_rnd_dist rndNodeGen;
	NTG_NodeContainer V;
	NTG_EdgeContainer E;
};

#endif /* _ntg_graph_obj_h_ */

