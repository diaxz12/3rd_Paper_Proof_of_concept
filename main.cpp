/*
main.cpp

An example implementation of transportSimplex.h.

Last edit April 25 2015

Copyright (C) 2006, 2015
Darren T. MacDonald
darrentmacdonald@gmail.com

This program demonstrates the use of transportSimplex.h by solving an operations
research problem. Suppose there are four factories manufacturing goods which
much be transported to five warehouses in different locations.
The factories and their current supply of goods are as follows:
Fredericton, 18
Halifax, 18
Charlottown, 8
St. John's, 16

The warehouses and their demands are as follows:
Dartmouth, 9
Nackawic, 11
Summerside, 4
Corner Brook, 10
Sydney, 6

Unit costs for transporting goods are computed by Dist. The function transportSimplex finds the
most efficient allocation of goods.
*/

#include "transportSimplex.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace t_simplex;
using namespace std;

double CostMatrix[5][5]={
        {9980,10217,10437,9994,10209},
        {9969,10058,10185,110066,10079},
        {9952,10041,10168,110044,10062},
        {9928,10024,10163,110022,10047},
        {9919,10007,10132,10215,10027}
};

double Dist(int * facPtr,int * warPtr) {
    //Formula for approximating the distance between a factory and a warehouse;
    return CostMatrix[*facPtr-1][*warPtr-1];
};

int main(int argc, char** argv) {
    int factories[5];
    factories[0] = 1;
    factories[1] = 2;
    factories[2] = 3;
    factories[3] = 4;
    factories[4] = 5;

    double supply[] = {2,0,0,0,5};

    int warehouses[5];
    warehouses[0] = 1;
    warehouses[1] = 2;
    warehouses[2] = 3;
    warehouses[3] = 4;
    warehouses[4] = 5;

    double demand[] = {1, 1, 1, 1, 1};

    cout << "Costs:" << endl;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            cout << factories[i] << " to " << warehouses[j] << " : " << Dist(&factories[i], &warehouses[j]) << endl;

    TsSignature<int> srcSig(5, factories, supply);
    TsSignature<int> snkSig(5, warehouses, demand);

    TsFlow flow[100];
    int flowVars = 0;

    double result = transportSimplex(&srcSig, &snkSig, Dist, flow, &flowVars);

    cout << "Total cost: " << result << endl;
    cout << "Flows:" << flowVars << endl;

    for (int i = 0; i < flowVars; i++)
        cout << factories[flow[i].from] << " to " << warehouses[flow[i].to] << " : " << flow[i].amount << endl;

}

