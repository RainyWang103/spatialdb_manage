//
//  RoadNetwork.h
//  Optimal_Meeting
//
//  Created by WangYuli on 1/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

#ifndef RoadNetwork_h
#define RoadNetwork_h
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <queue>
#include <map>
#include "NodeAndEdge.h"

#define DEBUG 0

using namespace std;

class RoadNetwork {
public:
    int numOfNodes_;
    std::vector<Node> node;
    
    RoadNetwork() {
        numOfNodes_ = 0;
    }
    //part 1
    void readRoadNetwork(string nodepath, string edgepath);
    void output(string outpath);
    
    //part 2
    
    //helpers
    double eudist(int src, int des); // distance based on euclidean distance
    void modifySPDQueue(priority_queue<prn>& Q, prn parent, Edge e, int des); // modify SPD queues in SPD() using A*
    void modifyGNTQueue(priority_queue<ent>& Q, ent parent, Edge e);          // modify GetNext queues in GetNext()
    int terminate_NRA_check(map<int, hte> hashTable);
    
    //spd algorithms
    double get_SPD(int src, int des);       // get spd using A*
    pair<int, double> getNext(int u);
    
    //aggregate function
    double gama(vector<double> record, int aggregate);
    
    //top-k algorithms
    int TA (vector<int> users, int aggregate);  // aggregate = MAX / SUM
    int NRA (vector<int> users, int aggregate); // aggragate = MAX / SUM
    int topK (vector<int> users, int topk_algorithm, int aggregate); // topk_algorithm = TA/NRA; aggragate = MAX / SUM
};

void RoadNetwork::readRoadNetwork(string nodepath, string edgepath) {
    int __id;
    double __longitude, __latitude;
    ifstream node_input;
    
    node_input.open(nodepath);
    if (node_input.is_open()) {
        
        if(DEBUG) cout<<"START READING NODES"<<endl;
        
        while (node_input >> __id >> __longitude >> __latitude){ // read the node
            
            if(DEBUG) cout<<"READ node: "<<__id<<endl;
            
            node.push_back(Node(__id, __longitude, __latitude));
            ++numOfNodes_;
        }
        
        if(DEBUG) cout<<"FINISHED READING NODES"<<endl;
    }
    node_input.close();
    
    int __startID, __endID;
    double __distance;
    ifstream edge_input;
    edge_input.open(edgepath);
    if (edge_input.is_open()) {
        
        if(DEBUG) cout<<"START READING EDGES"<<endl;
        
        while (edge_input >> __id >> __startID >> __endID >> __distance){ // read the edge
            if(DEBUG) cout<<"READ edge: "<<__id<<endl;
            node[__startID].edgeList.push_back(Edge(__id, __endID, __distance));
            node[__endID].edgeList.push_back(Edge(__id, __startID, __distance));
        }
        
        if(DEBUG) cout<<"FINISHED READING EDGES"<<endl;
        
    }
    edge_input.close();
    
    for(int i=0; i<node.size(); i++) {
        node[i].visited = vector<bool>(node.size(), false);
    }

}

void RoadNetwork::output(string outpath) {
    ofstream output;
    output.open(outpath);
    
    if (output.is_open()) {
        
        if(DEBUG) cout<<"START OUTPUT"<<endl;
        
        output << numOfNodes_ << endl;
        for (int i = 0; i < numOfNodes_; ++i) {
            output << node[i].id << "\t" << node[i].longitude << "\t" << node[i].latitude;
            
            if(DEBUG) cout<<"output node: "<<i<<endl;
            
            for(int j = 0; j < node[i].edgeList.size(); ++j) {
                output << "\t" << node[i].edgeList[j].id
                       << "\t" << node[i].edgeList[j].endNode
                       << "\t" << node[i].edgeList[j].distance;
            }
            output << endl;
        }
        
        if(DEBUG) cout<<"FINISHED OUTPUT"<<endl;
        
    }
    
    output.close();
}




#endif /* RoadNetwork_h */
