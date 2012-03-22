/**\file ntg_graph_obj.h - NTG_graph_o class header
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


/* $Id: ntg_graph_obj.h,v 1.1 2001/06/29 13:16:58 face Exp $
* $Log: ntg_graph_obj.h,v $
* Revision 1.1  2001/06/29 13:16:58  face
* *** empty log message ***
*
* Revision 1.9  2000/11/06 10:37:37  face
* *** empty log message ***
*
* Revision 1.8  2000/11/04 10:32:18  face
* *** empty log message ***
*
* Revision 1.6  2000/10/15 19:21:38  face
* *** empty log message ***
*
* Revision 1.3  2000/10/04 15:15:28  face
* *** empty log message ***
*
* Revision 1.2  2000/10/03 08:30:01  face
* *** empty log message ***
*
* Revision 1.1  2000/10/01 09:33:22  face
* added for the first time
*

*/
#ifndef _ntg_graph_obj_h_
#define _ntg_graph_obj_h_

/* NT core includes */
#include "nt_main.h"
#include "nt_types.h"
#include "nt_obj.h"
/* NT includes */
#include "nt_id_management_obj.h"
#include "ntg_node_obj.h"
#include "ntg_edge_obj.h"
#include "nt_uniform_rnd_dist_obj.h"
#include "nt_sequential_statistics_obj.h"
/* other includes */
#include <vector>
#include <algorithm>
#include "tnt/tnt.h"
#include "tnt/cmat.h"
#include <cstring>

using namespace TNT;

/** @short NTG_graph_o class
 ** Concepts: the NTG_graph_o mimics the mathematical definition
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



class NTG_vertex_o
{
public:

    NTG_vertex_o( NTid id = 0, NTG_node_o node = NTG_node_o(), string name="")	{
        first = id;
        second = node;
        if (name == "") third = first;
        else third = name;
    }
    NTid first;
    NTG_node_o second;
    string third;
};


//typedef pair<NTid,NTG_node_o> NTG_vertex_o;
typedef vector< NTG_vertex_o > NTG_NodeContainer;
typedef vector< NTG_edge_o > NTG_EdgeContainer;
typedef vector< NTid >	NTG_NodeIdList;
typedef Matrix< NTreal > NTG_PositiveCostMatrix;
typedef Matrix< bool > NTG_AdjacencyMatrix;
typedef Matrix< NTreal > NTG_NodeDistanceMatrix;

template <class T>
NTG_NodeDistanceMatrix NodeDistanceMatrixConditioning(const Matrix<T> & aMtr)
{
    NTG_NodeDistanceMatrix dMtr(aMtr.num_rows(),aMtr.num_cols());
    NT_ASSERT( aMtr.size() == dMtr.size() );
    for (NTsize lr=0; lr < (NTsize)aMtr.num_rows(); lr++) {
        for (NTsize lc=0; lc < (NTsize)aMtr.num_cols(); lc++) {
            if (lr == lc) dMtr[lr][lc] = 0;
            else {
                T tmpVal = aMtr[lr][lc];
                if ( 0 ==  tmpVal) dMtr[lr][lc] = 1/0.0; /* set value to infinity (NAN), this is correct code ! */
                else
                    if ( (NTreal)tmpVal < 0) {
                        cerr << "NodeDistanceMatrixPrecondition - Warning : Negative values in matrix encountered. Using absolute values." << endl;
                        dMtr[lr][lc] = -tmpVal;
                    } else dMtr[lr][lc] = tmpVal;
            }
        }
    }

    return dMtr;
}



class NTG_graph_o : public NT_o {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    NTG_graph_o();
    NTG_graph_o(const NTG_graph_o & original);
    const NTG_graph_o & operator= (const NTG_graph_o & right);
    virtual ~NTG_graph_o();
    /* ***  Methods              ***/
// queries
    NTbool InGraph(NTid selNode) const;
    NTbool IsConnected(NTid source, NTid target) const;
    NTbool IsSymmetricConnected(NTid a, NTid b) const;
    NTbool GraphConsistency() {
        return NT_TRUE;
    }
    NTsize NumNodes() const;
    NTsize NumEdges() const;
    NTsize NumConvergentConnections(NTid id) const;
    NTG_NodeIdList ShortestPath() const;
    NTsize Distance(NTid a, NTid b) const;
    NTG_EdgeContainer ConvergentConnections(NTid id) const;
    NTG_EdgeContainer DivergentConnections(NTid id) const;
    NTG_NodeIdList NodeIdList() const;
    NTG_EdgeContainer EdgeList() const;
    void ShowStatistics();
    void ShowAdjacencyMatrix();
    NTG_AdjacencyMatrix AdjacencyMatrix();

    NTG_NodeDistanceMatrix NodeDistanceMatrix(const NTG_NodeDistanceMatrix & aMtr) const;
    NT_sequential_statistics_o AdjacencyDistanceStatistics();


// node manipulation
    NTid AddNode(const NTG_node_o & newNode);
    NTreturn DeleteNode(NTid delNode);
    NTid RandomNodeId() const;
// edge manupulation
    virtual NTreturn Connect(NTid source, NTid target);
    virtual NTreturn Connect(NTid source, NTid target, NTG_edge_o newEdge);
    virtual NTreturn SymmetricConnect(NTid source, NTid target);
    virtual NTreturn SymmetricConnect(NTid source, NTid target, const NTG_edge_o & newEdge);
    virtual NTreturn Disconnect(NTid source, NTid target);
    virtual NTreturn SymmetricDisconnect(NTid source, NTid target);
    /* ***  Data                 ***/

protected:
    /* ***  Methods              ***/
// queries
// edge manupulation
    virtual NTreturn DisconnectNode(NTid node);
    virtual NTreturn DisconnectDivergence(NTid source);
    virtual NTreturn DisconnectConvergence(NTid target);
    /* ***  Data                 ***/

private:
    /* ***  Methods              ***/
// queries
    NTG_NodeContainer::iterator VertexIterator(NTid id);
    NTG_EdgeContainer::iterator EdgeIterator(NTid a, NTid b);
    void UpdateStatistics();
// edge manipulation
    NTreturn AddEdge(const NTG_edge_o & newEdge);
    /* ***  Data                 ***/
    NTsize numNodes;
    NTsize numEdges;
    NT_id_management_o nodeIdManager;
    NT_uniform_rnd_dist_o rndNodeGen;
    NTG_NodeContainer V;
    NTG_EdgeContainer E;
};

#endif /* _ntg_graph_obj_h_ */

/* File skeleton generated by GenNTObj version 0.7. */
