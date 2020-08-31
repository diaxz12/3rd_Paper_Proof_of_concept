/*
Luis Dias 3rd Paper Proof of concept
*/


#include <iostream>
#include "AssetTransportSolver.h"


int main(int argc, char** argv) {

    double TotalCost;

    // ||---Rotina para ler as coordenadas do ficheiro input do problema de t---||
    if(argc >= 1){
        char * path= "Optimization_langrange_input.txt";
        TotalCost = AssetTransportSolver(path);
    }else{
        cerr << "File not found \n";
        exit (EXIT_FAILURE);
    }

    cout << "Total cost for period t =" << TotalCost << endl;

    return 0;
}

