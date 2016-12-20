//
//  NodeAndEdge.h
//  Optimal_Meeting
//
//  Created by WangYuli on 1/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

//
//  node_and_edge.h
//  Optimal_Meeting
//
//  Created by WangYuli on 1/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

#ifndef NodeAndEdge
#define NodeAndEdge
#include <vector>
#include <iostream>
using namespace std;

struct prn {    // for A* spd finding
    int id; // node id
    double dist; // euclidean distance to destination
    double spd;  // shortest path distance to source
    double cost; // total cost for prioritizing
    
    prn(const int& _id, const double& _dist, const double& _spd) {
        id = _id; dist = _dist; spd = _spd;
        cost = _dist + _spd;
    }
    
    bool operator<(const prn& other) const {     // Min Priority Queue
        return cost > other.cost;
    }
};

ostream & operator<<(ostream & cout, const prn & x) {
    cout<<"node_"<<x.id<<" ## "<<"dist_"<<x.dist<<", spd_"<<x.spd<<", cost_"<<x.cost<<endl;
    return cout;
}



struct ent {    // table entris for the get_next table
    int id;     // node id for the entry
    double spd; // spd stored
    ent(){}
    ent(const int& _id, const double& _spd): id(_id), spd(_spd){}
    
    bool operator<(const ent& other) const {    // Min Priority Queue
        return spd > other.spd;
    }
};

ostream & operator<<(ostream& cout, const ent& x) {
    cout<<"node_"<<x.id<<" ## "<<", spd_"<<x.spd<<endl;
    return cout;
}

struct hte { // hash table entries to store spds, upper bound, lower bound
    double lb;           // lower bound
    double ub;           // upper bound
    vector<double> spds; // spds
    
    hte(){}
    
    hte(double _lb, double _ub, int _spd_size): lb(_lb), ub(_ub) {
        spds = vector<double> (_spd_size, -1);
    }
};


class Edge
{
    
public:
    int id;
    int endNode;
    double distance;
    
    Edge(int _id, int _end, double _distance)           // constructor
    : id(_id), endNode(_end), distance(_distance) {}
    
};

class Node
{
public:
    int id;
    
    double longitude;
    double latitude;
    
    std::vector<Edge> edgeList;
    priority_queue<ent> table; // priority queue to run the Dijkstra's Algorithm, the visited() should be placed in TA()/NRA(), shared by all tables
    vector<bool> visited; // visted vector to run the Dijkstra's Algorithm inside the table
    
    Node(int _id, double _longitude, double _latitude)  // constructor
    : id(_id), longitude(_longitude), latitude(_latitude) {
        table.push(ent(id, 0)); // initiate the queue
    }
    
};

#endif /* NodeAndEdge_h */
