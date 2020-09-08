/*
Luis Dias 3rd Paper Proof of concept
*/

#include <iostream>
#include <stdio.h>
#include "AssetTransportSolver.h"

int main(int argc, char** argv) {

    double TotalCost;
    double OriginalTotalCost;
    char path_original_problem[100];
    char path_aproximated_problem[100];

    // ||---Rotina para ler as coordenadas do ficheiro input do problema de t---||
    if(argc >= 1){
        for(int i=36; i<=36; i++){

            //build instances path
            sprintf(path_original_problem,"data/Instance_original_%d.txt",i);
            cout << endl << endl << "PATH Original PROBLEM = " << path_original_problem << endl << endl;
            sprintf(path_aproximated_problem,"data/Instance_aproximated_%d.txt",i);
            cout << "PATH APROXIMATED PROBLEM = " << path_aproximated_problem << endl << endl;

            //output console separator
            cout << "||--------Begin Instance solver--------||" << endl << endl;

            //Solve instance for original problem and aproximated problem respectively
            OriginalTotalCost = SolverOriginalProblem(path_original_problem);
            TotalCost = AssetTransportSolver(path_aproximated_problem);
        }
        //output console separator
        cout << endl << endl << "||--------End of Instance solver--------||" << endl << endl;
    }else{
        cerr << "Missing input File\n";
        exit (EXIT_FAILURE);
    }

    cout << endl << "Original total cost for period t =" << OriginalTotalCost << endl << endl;
    cout << endl << "Total cost for period t =" << TotalCost << endl << endl;

    return 0;
}

