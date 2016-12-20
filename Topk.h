//
//  Topk.h
//  Optimal_Meeting
//
//  Created by WangYuli on 1/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

#ifndef Topk_h
#define Topk_h
#include <map>
#include "ShortestPath.h"
#include "GetNext.h"

#define _TA 1
#define _NRA 2

#define _SUM 1
#define _MAX 2

#define DEBUG_TA 0
#define DEBUG_NRA 0
#define PERFORMANCE_TEST 1

double RoadNetwork::gama(vector<double> record, int aggregate) {
    
    if(aggregate == _MAX) {
        double max = -1;
        for(int i=0; i<record.size(); i++) {
            if(record[i]>max)
                max = record[i];
        }
        return max;
    }
    
    else {
        double sum = 0;
        for(int i=0; i<record.size(); i++) {
            sum+=record[i];
        }
        return sum;
    }
}

int RoadNetwork::terminate_NRA_check(map<int, hte> hashTable) {
    // find maximum upper bound,
    // if min upper bound <= all lower bound (with not the maximum upper bound)
    // then we terminate, otherwise, return <-1,-1>
    
    int min_up_id = -1;
    double min_up = -1;
    // find minumum upper bound
    for(map<int, hte>::iterator itr = hashTable.begin(); itr!=hashTable.end(); itr++) {
        if((*itr).second.ub == -1) continue;
        if((*itr).second.ub < min_up || min_up == -1) {
            min_up = (*itr).second.ub;
            min_up_id = (*itr).first;
        }
    }
    // compare it with all lower bound
    int corrects = 0;
    for(map<int, hte>::iterator itr = hashTable.begin(); itr!=hashTable.end(); itr++) {
        if((*itr).first != min_up_id && min_up <= (*itr).second.lb && min_up != -1) {
            corrects++;
        }
    }
    return (corrects ==  hashTable.size() - 1 )? min_up_id: -1;
}

int RoadNetwork::TA(vector<int> users, int aggregate) {     // aggragate = MAX / SUM
   
    double min_gama_v = -1;
    int min_meetPoint = -1;
    double threshold = -1;
    
    int round = 0;
    int num_get_spd = 0;
    while(min_gama_v>=threshold) {
        round++;
        vector<double> latest_spds; // to compute gama score of latest spds
        
        for(int i=0; i<users.size(); i++){
            // get_next() to get the next nearest: v for ui
            pair<int, double> latest = getNext(users[i]);
            latest_spds.push_back(latest.second);
            int v = latest.first;
            
            vector<double> v_spds;
            v_spds.push_back(latest.second);
            
            // get_spd(): (spd(u1, v), spd(u2, v), spd(u3, v)....)
            for(int j=0; j<users.size(); j++){
                if(i!=j){
                    double temp_spd = get_SPD(users[j], v);
                    num_get_spd++;
                    v_spds.push_back(temp_spd);
                }
            }
            
            double gama_v = gama(v_spds, aggregate);
            
            if(min_gama_v == -1 || gama_v < min_gama_v){
                min_gama_v = gama_v;
                min_meetPoint = v;
            }
        }
        threshold = gama(latest_spds, aggregate);
    }
    
    if(PERFORMANCE_TEST) {
        cout<<"-------- TA Algorithm Performance view ---------"<<endl<<endl;
        cout<<"aggregation function: "<<aggregate<<endl;
        cout<<"user locations: ";
        for(int i=0; i<users.size(); i++) cout<<users[i]<<", ";
        cout<<endl;
        cout<<"# of user locations: "<<users.size()<<endl;
        cout<<"# of rounds (# of rows of get_next() call): "<<round<<endl;
        cout<<"# of get_spd() calls: "<<num_get_spd<<endl<<endl;
        cout<<"-------- Final result --------"<<endl;
    }
    
    return min_meetPoint;
}

int RoadNetwork::NRA(vector<int> users, int aggregate) {    // aggragate = MAX / SUM
    map<int, hte> hashTable; // hash table to keep track
    
    bool finished = false;
    int round = 0;
    int num_of_ub_update = 0;
    int num_of_lb_update = 0;
    while(!finished) {
        round++;
        vector<Edge> latest_spds;
        
        // 1. get_next(): for a new row, stores in latest_spds; update spds hashTable
        for(int i=0; i<users.size(); i++) {
            
            // node (v) for user i in node (users[i])
            pair<int, double> latest = getNext(users[i]);
            int v = latest.first; double v_spd = latest.second;
            latest_spds.push_back(Edge(users[i], v, v_spd));
            
            // update spds in hash table
            if(hashTable.find(v) == hashTable.end()){
                int spd_size = int(users.size());
                hashTable[v] = hte(0, -1, spd_size);
            }
            hashTable[v].spds[i] = v_spd;
        }
        
        // 2. update hash table for upper bound and lower bound
        for(map<int, hte>::iterator itr = hashTable.begin(); itr!=hashTable.end(); itr++){
            int s = (*itr).first; // target node id for updating
            
            // 2.1 upper bound
            if(find(hashTable[s].spds.begin(), hashTable[s].spds.end(), -1) == hashTable[s].spds.end()) {
                hashTable[s].ub = gama(hashTable[s].spds, aggregate);
                
            }
            num_of_ub_update ++;
            num_of_lb_update ++;
            
            // 2.2 lower bound
            double lowerBound = 0;
            for(int j=0; j<users.size(); j++) {
                if(hashTable[s].spds[j] == -1) {
                    lowerBound = gama(vector<double>{lowerBound,latest_spds[j].distance}, aggregate);
                }
                else {
                    lowerBound = gama(vector<double>{lowerBound,hashTable[s].spds[j]}, aggregate);
                }
            }
            hashTable[s].lb = lowerBound;
        }

        // 3. check ending condition
        int end = terminate_NRA_check(hashTable);
        if(end != -1) {
            
            if(PERFORMANCE_TEST) {
                cout<<"-------- NRA Algorithm Performance view ---------"<<endl<<endl;
                cout<<"aggregation function: "<<aggregate<<endl;
                cout<<"user locations: ";
                for(int i=0; i<users.size(); i++) cout<<users[i]<<", ";
                cout<<endl;
                cout<<"# of user locations: "<<users.size()<<endl;
                cout<<"# of rounds (# of rows of get_next() call): "<<round<<endl;
                cout<<"# of update upper bounds: "<<num_of_ub_update<<endl;
                cout<<"# of update lower bounds: "<<num_of_lb_update<<endl<<endl;
                cout<<"-------- Final result --------"<<endl;
            }
            
            finished = true;
            return end;
        }
    }

    return -1;
}


int RoadNetwork::topK(vector<int> users,int topk_algorithm, int aggregate) {  // topk_algorithm = TA/NRA; aggragate = MAX / SUM
    
    return topk_algorithm == _TA ? TA(users, aggregate) : NRA(users, aggregate);
}


#endif /* Topk_h */
