//
//  ShortestPath.h
//  Optimal_Meeting
//
//  Created by WangYuli on 1/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

#ifndef ShortestPath_h
#define ShortestPath_h
#include <math.h>
#include <queue>
#include <algorithm>
#include "RoadNetwork.h"

#define DEBUG_SPD 0

//helpers
double RoadNetwork::eudist(int src, int des) {   // distance based on euclidean distance
    
    if (src == des) return 0.0;
    
    // if connected, return edge distance
    for (vector<Edge>::iterator itr = node[src].edgeList.begin(); itr!=node[src].edgeList.end(); itr++) {
        if((*itr).endNode == des)
            return (*itr).distance;
    }
    // if not connected, return euclidean distance
    double sum = pow(node[src].latitude - node[des].latitude, 2.0) + pow(node[src].longitude - node[des].longitude, 2.0);
    return sqrt(sum);
}

void RoadNetwork::modifySPDQueue(priority_queue<prn>& Q, prn parent, Edge e, int des) {
    int newid = e.endNode;
    vector<prn> newRecord;  // for copy back to the queue
    
    bool old = false;   // old: node already in queue
    while(!Q.empty() && !old) {
        prn temp = Q.top(); Q.pop();
        
        // match an old node
        if(temp.id == newid){
            old = true;
            if(temp.spd > parent.spd + e.distance) {    // find new shortest path, update cost
                newRecord.push_back(prn(temp.id, temp.dist, parent.spd + e.distance));
            }
            else newRecord.push_back(temp); // is an old one, but not with better spd, then just keep the old one
        }
        else newRecord.push_back(temp); // not match an old one, then just keep the old one
    }
    // not found in old record, push a new one
    if(!old) {
        newRecord.push_back(prn(newid, eudist(newid, des), parent.spd + e.distance));
    }
    // copy vector back to queue
    for(vector<prn>::iterator itr = newRecord.begin(); itr!=newRecord.end(); itr++) {
        prn temp = (*itr);
        Q.push(prn(temp.id, temp.dist, temp.spd));
    }
}


//spd algorithm
double RoadNetwork::get_SPD(int src, int des) { // get spd using A*
    priority_queue<prn> Q;
    vector<bool> visited(node.size(), false);
    
    Q.push(prn(src, 0, 0)); // push src node to queue
    //visited[src] = true;
    while(!Q.empty()) {
        prn mark = Q.top(); Q.pop(); // dequeue to get the best one
        int m = mark.id;
        visited[m]=true;
        
        if(m == des)   return mark.spd; // found destination, return shortest path distance
        
        // neighbours: add new node / update old node in queue
        for(vector<Edge>::iterator itr = node[m].edgeList.begin(); itr!=node[m].edgeList.end(); itr++){
            Edge e = (*itr);
            int newID = e.endNode;
            
            if(visited[newID])  // already visited, then do nothing
                continue;
            
//            double new_dist = eudist(newID, des);
//            double new_spd = mark.spd + e.distance;
//            prn temp(newID, new_dist, new_spd);
            
            modifySPDQueue(Q, mark, (*itr), des);
        }
    }
    return -1;
}


#endif /* ShortestPath_h */
