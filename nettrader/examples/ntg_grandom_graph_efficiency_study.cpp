#include "ntg_node_obj.h"
#include "ntg_graph_obj.h"



#include "nt_main.h"
#include "nt_types.h"
#include "nt_codes.h"
#include "nt_obj.h"

#include "ntg_graph_obj.h"
#include "ntg_random_graph_obj.h"
#include "ntg_node_obj.h"
#include "ntg_value_node_obj.h"

#include "nt_histogram_obj.h"

#include <ostream>
#include <fstream>

struct NTG_GraphStatistics {
    NTreal compareValue;
    NTsize numNodes;
    NTsize numEdges;
    NTreal connectivityRatio;
    NTreal avgFanIn;
    NTreal avgFanOut;
    NTreal fanOutInRatio;
    vector < pair<NTid, NTreal> > fanInPerNode;
    vector < pair<NTid, NTreal> > ratioFanOutInPerNode;
    vector < pair<NTid, NTreal> > fanOutPerNode;
    Matrix <bool> adjacencyMatrix;
};

NT_histogram_o ComputeFanInHistogram(NTG_GraphStatistics & stat, NTsize numBins=10)
{
    vector < pair<NTid, NTreal> >::iterator vecIter;
    NT_histogram_o histogram(0,stat.numNodes, numBins);

    for (vecIter = stat.fanInPerNode.begin(); vecIter != stat.fanInPerNode.end(); vecIter++) {
        histogram.BinValue((*vecIter).second );
    }

    return histogram;
}


NT_histogram_o ComputeFanOutHistogram(NTG_GraphStatistics & stat, NTsize numBins=10)
{
    vector < pair<NTid, NTreal> >::iterator vecIter;
    NT_histogram_o histogram(0,stat.numNodes, numBins);

    for (vecIter = stat.fanOutPerNode.begin(); vecIter != stat.fanOutPerNode.end(); vecIter++) {
        histogram.BinValue((*vecIter).second );
    }

    return histogram;
}


NT_histogram_o ComputeFanHistogram(NTG_GraphStatistics & stat, NTsize numBins = 10)
{
    vector < pair<NTid, NTreal> >::iterator vecIter;
    vector < pair<NTid, NTreal> >::iterator vecIter2;
    NT_histogram_o histogram(0,stat.numNodes*2, numBins);

    if (stat.fanInPerNode.size() != stat.fanOutPerNode.size()) cerr << "ComputeFanHistogram - Error : stat.fanInPerNode and stat.fanOutPerNode do not have the same length."<<endl;

    vecIter = stat.fanInPerNode.begin();
    vecIter2 = stat.fanOutPerNode.begin();

    NTreal sum = 0;
    do {
        if ( (*vecIter).first != (*vecIter2).first) cerr << "ComputeFanHistogram - Error : stat.fanInPerNode and stat.fanOutPerNode do not have the same sequence of node ids."<<endl;
        sum = ((*vecIter).second  + (*vecIter2).second) ;
        histogram.BinValue(sum);
        vecIter++;
        vecIter2++;
    } while (vecIter != stat.fanInPerNode.end()) ;


    return histogram;
}







/** \warning the Fan Out In ratio is computed with Out/In = Out iff In = 0.
 **
 ** */
NTG_GraphStatistics ConnectivityStatistics(NTG_graph_o & g)
{
    NTG_GraphStatistics stat;
    //g.UpdateStatistics();
    stat.numNodes = g.NumNodes();
    stat.numEdges = g.NumEdges();
    stat.connectivityRatio = (NTreal) g.NumEdges()/(g.NumNodes()*(g.NumNodes()-1.0));

    stat.fanInPerNode.resize(g.NumNodes());
    stat.fanOutPerNode.resize(g.NumNodes());
    stat.ratioFanOutInPerNode.resize(g.NumNodes());

    NTG_NodeIdList nodeList = g.NodeIdList();
    NTG_NodeIdList::const_iterator nodeListIter;

    NTsize tmpCont;
    NTsize tmpCont2;

    NTsize ll=0;
    for ( nodeListIter = nodeList.begin(); nodeListIter != nodeList.end(); nodeListIter++) {
        NTid a = (*nodeListIter);
        tmpCont = (g.ConvergentConnections( a )).size();
        tmpCont2 = (g.DivergentConnections( a )).size();

        stat.fanInPerNode[ll].first = a;
        stat.fanInPerNode[ll].second = tmpCont;
        stat.fanOutPerNode[ll].first = a;
        stat.fanOutPerNode[ll].second = tmpCont2;
        stat.ratioFanOutInPerNode[ll].first = a;
        if (tmpCont == 0) stat.ratioFanOutInPerNode[ll].second = tmpCont2;
        else stat.ratioFanOutInPerNode[ll].second = tmpCont2/(NTreal)tmpCont ;

        ll++;
    }


    stat.avgFanIn = 0;
    stat.avgFanOut = 0;
    stat.fanOutInRatio = 0.0;

    for (ll=0; ll< g.NumNodes(); ll++) {
        stat.avgFanIn += stat.fanInPerNode[ll].second;
        stat.avgFanOut += stat.fanOutPerNode[ll].second;
        stat.fanOutInRatio += stat.ratioFanOutInPerNode[ll].second;
    }


    stat.avgFanIn /= g.NumNodes();
    stat.avgFanOut /= g.NumNodes();
    stat.fanOutInRatio /= g.NumNodes();

    stat.adjacencyMatrix = g.AdjacencyMatrix();

    return stat;
}



class NTG_GraphStatistics_CompareGreater {
public:
    int operator() (const NTG_GraphStatistics & a, const NTG_GraphStatistics & b) const
    {
        return ( a.compareValue > b.compareValue );
    }
};



int main(int argc, char* argv[])
{

    if (argc != 3) {
        cerr << "Incorrect number of arguments" << endl;
        exit(1);
    }

    NTsize n = atoi(argv[0]);
    NTreal p = atof(argv[1]);
    NTsize numGraphs = atoi(argv[2]);

    cout << "Init simulation" << endl;

    cout << "Number of graphs" << numGraphs;
    cout << "Connectedness ratio" << p;
    cout << "Number of nodes in graph" << n;

    vector <NTG_GraphStatistics> statVec(0);
    NTG_node_o protoNode;
    NTG_edge_o protoEdge;

    cerr << "Starting simulation " << sizeof(NTG_GraphStatistics) << " "<<sizeof(statVec) <<endl;
    {
        for (NTsize ll=0; ll<numGraphs; ll++) {
            NTG_random_graph_o fG(n, p, protoNode, protoEdge);
            //cout << ll <<endl;
            statVec.push_back( ConnectivityStatistics(fG) );
        }
    }


    {
        for (NTsize ll=0; ll<numGraphs; ll++) {
            //		cout << ll << "\t"<< statVec[ll].numEdges << "\t" <<statVec[ll].connectivityRatio << "\t" <<statVec[ll].avgFanIn << "\t" <<statVec[ll].avgFanOut << "\t" <<statVec[ll].fanOutInRatio << endl;
            statVec[ll].compareValue = statVec[ll].fanOutInRatio;
            // cout << statVec[ll].adjacencyMatrix;
        }
    }


    sort(statVec.begin(), statVec.end(), NTG_GraphStatistics_CompareGreater());



    NTsize topten = (numGraphs >= 100) ? 100 : numGraphs;

    for (NTsize ll=0; ll< numGraphs; ll++) {
//			cout << ll << "\t"<< statVec[ll].numEdges << "\t" <<statVec[ll].connectivityRatio << "\t" <<statVec[ll].avgFanIn << "\t" <<statVec[ll].avgFanOut << "\t" <<statVec[ll].fanOutInRatio << endl;
        //(ComputeFanOutHistogram(statVec[ll],statVec[ll].numNodes)).Show();
        //(ComputeFanInHistogram(statVec[ll],statVec[ll].numNodes)).Show();
        cout << (ComputeFanHistogram(statVec[ll],statVec[ll].numNodes*2)) << statVec[ll].fanOutInRatio << endl;
    }





    cerr << "Simulation completed" << endl;
    return 0;
}


