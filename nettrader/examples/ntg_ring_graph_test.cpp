#include "ntg_node_obj.h"
#include "ntg_graph_obj.h"



#include "nt_main.h"
#include "nt_types.h"
#include "nt_codes.h"
#include "nt_obj.h"

#include "ntg_graph_obj.h"
//#include "ntg_random_graph_obj.h"
#include "ntg_neighbour_ring_graph_obj.h"
#include "ntg_random_ring_graph_obj.h"
#include "ntg_node_obj.h"
//#include "ntg_value_node_obj.h"

#include "nt_histogram_obj.h"

#include <ostream>
#include <fstream>

int main()
{

    NTreal p;
    NTsize n;
    NTsize r;
    NTsize numGraphs;

    cout << "Init simulation" << endl;

//	cout << "Number of graphs";
//	cin >> numGraphs;
    cout << "Number of nodes in graph (>0):";
    cin >> n;
    cout << "Radius of neighbourhood (>0):";
    cin >> r;
    cout << "Reconnection probability (0 < p < 1):";
    cin >> p;

    NTG_node_o protoNode;
    NTG_edge_o protoEdge;




    cout << "Starting simulation " << endl;
    NTG_neighbour_ring_graph_o  G(n,r,protoNode,protoEdge);
    G.ShowStatistics();
    G.ShowAdjacencyMatrix();
    cout << G.NodeDistanceMatrix( NodeDistanceMatrixConditioning( G.AdjacencyMatrix() ) );

    cout << "Starting simulation " << endl;
    NTG_random_ring_graph_o  rG(n,r,p, false, protoNode,protoEdge);
    rG.ShowStatistics();
    rG.ShowAdjacencyMatrix();
    cout << rG.NodeDistanceMatrix( NodeDistanceMatrixConditioning( rG.AdjacencyMatrix() ) );

    cout << "Starting simulation " << endl;
    NTG_random_ring_graph_o  dG(n,r,p, true, protoNode,protoEdge);
    dG.ShowStatistics();
    dG.ShowAdjacencyMatrix();
    cout << dG.NodeDistanceMatrix( NodeDistanceMatrixConditioning( dG.AdjacencyMatrix() ) );


    cout << "Simulation completed" << endl;

    return 0;
}



