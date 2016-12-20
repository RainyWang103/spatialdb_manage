//
//  main.cpp
//  Optimal_Meeting
//
//  Created by WangYuli on 1/12/2016.
//  Copyright © 2016 WangYuli. All rights reserved.
//

#include <iostream>
#include "RoadNetwork.h"
#include "ShortestPath.h"
#include "GetNext.h"
#include "Topk.h"
using namespace std;

#define _TA 1
#define _NRA 2

#define _SUM 1
#define _MAX 2

int main(int argc, const char * argv[]) {
    
    RoadNetwork rn;
    rn.readRoadNetwork(argv[1], argv[2]);
    rn.output(argv[3]);
    
    vector<int> users{0, 4, 5};
    int meet = rn.topK(users, _TA, _MAX);
    
    cout<<"meet at point"<<meet<<endl;
    
    return 0;
}
