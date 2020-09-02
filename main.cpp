/*
Luis Dias 3rd Paper Proof of concept
*/

#include <iostream>
#include "AssetTransportSolver.h"

int main(int argc, char** argv) {

    double TotalCost;
    double OriginalTotalCost;

    // ||---Rotina para ler as coordenadas do ficheiro input do problema de t---||
    if(argc >= 1){
        for(int i=0; i<10; i++){
            OriginalTotalCost = SolverOriginalProblem("Optimization_original_input.txt");
            TotalCost = AssetTransportSolver("Optimization_langrange_input.txt");
        }
    }else{
        cerr << "Missing input File\n";
        exit (EXIT_FAILURE);
    }

    cout << endl << "Original total cost for period t =" << OriginalTotalCost << endl << endl;
    cout << endl << "Total cost for period t =" << TotalCost << endl << endl;

    return 0;
}

