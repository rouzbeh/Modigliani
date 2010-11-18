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

int main()
{
    /*
    NTG_node_o aNode;
    NTG_node_o bNode;
    NTG_value_node_o cNode;

    NTG_graph_o G;
    G.ShowStatistics();

    NTid a = G.AddNode(aNode);
    NTid b = G.AddNode(bNode);
    NTid c = G.AddNode(cNode);
    NTid d = G.AddNode(cNode);
    NTid e = G.AddNode(cNode);
    NTid f = G.AddNode(cNode);

    G.ShowStatistics();
    G.ShowAdjacencyMatrix();

    cout << "************Connect the nodes******************" << endl;
    G.Connect(a,b);
    G.Connect(b,c);
    G.Connect(c,a);
    G.Connect(c,e);
    G.Connect(e,c);
    				G.Connect(d,c);
    G.Connect(a,c);
    G.Connect(e,b);
    G.Connect(e,d);
    				G.Connect(d,c);
    G.Connect(f,e);
    G.Connect(a,f);
    G.Connect(a,e);
    G.Connect(f,a);
    G.SymmetricConnect(b,d);

    G.ShowStatistics();
    G.ShowAdjacencyMatrix();
    cout << "G.sizeof()=" << sizeof(G) <<endl;

    cout << "A**********Disconnect a,b & symmetric d,c********************" << endl;
    cerr <<G.Disconnect(a,b);
    cerr <<G.SymmetricDisconnect(d,c);
    G.ShowStatistics();
    G.ShowAdjacencyMatrix();
    cout << "G.sizeof()=" << sizeof(G) <<endl;

    cout << "B***************Disconnect symm. b,d***************" << endl;
    cerr <<G.SymmetricDisconnect(b,d);
    G.ShowStatistics();
    G.ShowAdjacencyMatrix();
    cout << "G.sizeof()=" << sizeof(G) <<endl;

    cout << "C****************************** delete node a" << endl;
    cerr << "Deleting node";
    cerr << G.DeleteNode(a);
    cerr << "Node deleted";
    G.ShowStatistics();
    G.ShowAdjacencyMatrix();
    cout << "G.sizeof()=" << sizeof(G) <<endl;

    cout << "C****************************** add node g and connect g,b & c,g" << endl;
    NTid g = G.AddNode(cNode);
    NTid h = G.AddNode(cNode);
    G.Connect(g,b);
    G.Connect(c,h);
    G.SymmetricConnect(g,h);
    G.ShowStatistics();
    G.ShowAdjacencyMatrix();
    cout << "G.sizeof()=" << sizeof(G) <<endl;

    cout << "D****************************** delete node g" << endl;
    G.DeleteNode(g);
    G.ShowStatistics();
    G.ShowAdjacencyMatrix();



    fstream fout ("graph.bin",ios::out|ios::binary);
    cout << "G.sizeof()=" << sizeof(G) <<endl;
    */


    NTreal p;
    NTreal n;
    NTsize numGraphs;

    cout << "Init simulation" << endl;

    cout << "Number of graphs";
    cin >> numGraphs;
    cout << "Connectedness ratio:";
    cin >> p;
    cout << "NUmber of nodes in graph:";
    cin >> n;


    vector <NTG_GraphStatistics> statVec(0);
    NTG_node_o protoNode;
    NTG_edge_o protoEdge;

    cout << "Starting simulation " << sizeof(NTG_GraphStatistics) << " "<<sizeof(statVec) <<endl;
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





    cout << "Simulation completed" << endl;

    /*

    	cout <<"Number of edges="<<g.NumEdges()<< " Number of nodes="<<g.NumNodes() <<endl;
    	cout << "Number of edges / Total possible number of edges ="<<pConnect <<endl;

    	for (ll = 0; ll<g.NumNodes(); ll++){
    		cout << fanInPerNode[ll].second << "\t" ;
    	}
    	cout << endl;

    	for (ll = 0; ll<g.NumNodes(); ll++){
    		cout << fanOutPerNode[ll].second << "\t" ;
    	}
    	cout << endl;
    */





    /*
    	NTG_random_graph_o rG(5, 0.5, protoNode, protoEdge);

    	rG.ShowStatistics();
    	rG.ShowAdjacencyMatrix();

    	NTG_random_graph_o rG2(5, 0.5, protoNode, protoEdge);
    	rG2.ShowAdjacencyMatrix();
    		*/
    return 0;
}



