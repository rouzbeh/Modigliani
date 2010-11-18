#include "ntg_node_obj.h"
#include "ntg_graph_obj.h"



#include "nt_main.h"
#include "nt_types.h"
#include "nt_codes.h"
#include "nt_obj.h"

#include "ntg_graph_obj.h"
#include "ntg_neighbour_ring_graph_obj.h"
#include "ntg_random_ring_graph_obj.h"
#include "ntg_node_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
#include "nt_sequential_statistics_obj.h"
#include <ostream>
#include <fstream>

int main()
{
    NTreal p;
    NTsize n;
    NTsize r;
    NTsize numGraphs;


    cout << "Init simulation" << endl;


    NT_sequential_statistics_o stat;
    NT_gaussian_rnd_dist_o rnd(2, 2);

//	for (NTsize ll=0; ll<100000; ll++) {stat.Add(rnd.RndVal());}
//	cout << stat<< endl;

    cerr << "Number of graphs";
    cin >> numGraphs;

    cerr << "Number of nodes in graph (>0):";
    cin>> n;
    cerr << "Radius of neighbourhood (>0):";
    cin >> r;
    cerr << "Reconnection probability (0 < p < 1):";
    cin >> p;

    NTG_node_o protoNode;
    NTG_edge_o protoEdge;

    cerr << "Starting simulation " << endl;

    NTsize ll;
    for ( ll = 0 ; ll < numGraphs ; ll++ ) {

        NTG_random_ring_graph_o  rG(n,r,p, false, protoNode,protoEdge);
        //rG.ShowStatistics();
        //rG.ShowAdjacencyMatrix();
//	cout << rG.NodeDistanceMatrix( NodeDistanceMatrixConditioning( rG.AdjacencyMatrix() ) );
        cout << rG.AdjacencyDistanceStatistics() << endl;
    }

    return 0;
}



