//
//  GetNext.h
//  Optimal_Meeting
//
//  Created by WangYuli on 2/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

#ifndef GetNext_h
#define GetNext_h

#include <queue>
#include <algorithm>
#include "RoadNetwork.h"

#define DEBUG_GETNEXT 0

void RoadNetwork::modifyGNTQueue(priority_queue<ent> &Q, ent parent, Edge e) {
    int newid = e.endNode;
    vector<ent> newRecord;  // for copy back to the queue
    
    bool old = false;
    while(!Q.empty() && !old) {
        ent temp = Q.top(); Q.pop();
        
        // match an old node
        if(temp.id == newid){
            old = true;
            if(temp.spd > parent.spd + e.distance) {    // find new shortest path, update cost
                newRecord.push_back(ent(temp.id, parent.spd + e.distance));
            }
            else newRecord.push_back(temp); // is an old one, but not with better spd, then just keep the old one
        }
        else newRecord.push_back(temp); // not match an old one, then just keep the old one
    }
    // not found in old record, push a new one
    if(!old) {
        newRecord.push_back(ent(newid, parent.spd + e.distance));
    }
    // copy vector back to queue
    for(vector<ent>::iterator itr = newRecord.begin(); itr!=newRecord.end(); itr++) {
        ent temp = (*itr);
        Q.push(ent(temp.id, temp.spd));
    }
    
}

pair<int, double> RoadNetwork::getNext(int u) { // access the table property of Node: node[u].table
    
    if(node[u].table.empty())   // 0. check for end of table condition & return an exception value
        return pair<int, double>(-1, -1);
    
    // 1. Get the "next" unvisited node in the table
    bool found = false;
    ent target;
    while(!found) {
        target = node[u].table.top(); node[u].table.pop(); // get next, the info to return
        
        if(!node[u].visited[target.id] || node[u].table.empty()) {
            // if the next is unvisited or the table is exhausted, we got what we want
            found = true;
        }
    }
    
    int x = target.id;
    node[u].visited[x] = true;
    
    // 2. For the node connected to "next", only push the unvisited
    for(vector<Edge>::iterator itr = node[x].edgeList.begin(); itr!=node[x].edgeList.end(); itr++) {
        Edge e = (*itr);
        int newID = e.endNode;
        if(!node[u].visited[newID])    {// not visited, then update queue
            modifyGNTQueue(node[u].table, target, e);
        }
    }
    
    return pair<int, double>(target.id, target.spd);
}

#endif /* GetNext_h */
