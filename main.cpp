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
#include <fstream>

using namespace t_simplex;
using namespace std;

//Variaveis globais <---esta parte pode ser melhorada pois estou alocar memória de uma forma estática e não dinâmica
int NumMaintenanceInterventions,NumAssetsTransportMatrix;
double CostMatrix[10][1000];
double DemandArray[1000];
double SupplyArray[10];


double Dist(int * facPtr,int * warPtr) {
    //Formula for approximating the distance between a factory and a warehouse;
    return CostMatrix[*facPtr-1][*warPtr-1];
};

int main(int argc, char** argv) {


//    string line;
//    if(argc >= 1){
//        //ifstream inputFile(argv[1]);
//        ifstream inputFile("Optimization_input.txt");
//        if (inputFile.is_open())
//        {
//            getline(inputFile, line);
//            //int pNum = stoi(line);
//            int i = 0;
//
//            while (getline(inputFile, line))
//            {
//                string x = line.substr(0, line.find(" "));
//                string y = line.substr(line.find(" "));
//
//                cout << x << "\t" << y << endl;
//
//                i++;
//            }
//            inputFile.close();
//        }
//        else{
//            cout << "There was a problem opening or reading input file" << endl;
//        }
//    }else{
//        cerr << "File not found \n";
//        exit (EXIT_FAILURE);
//    }

    //Rotina para ler as coordenadas do ficheiro dados
    int i,j; //indices da matriz
    int contador=0; //conta o numero de iteracoes a fazer dependendo de cada fase da leitura do ficheiro ||1º fase: Ler tamanho matriz|| 2ª fase: Ler valores da matriz|| 3ª fase: ler a procura|| 4ª fase: ler a capacidade||
    size_t pos = 0; //marca a posicao na string
    string token; // valor a extrair
    string delimiter = " "; //separador a utilizar no parse do texto (ex: ficheiro csv, tsv)
    ifstream file("Optimization_input.txt"); //nome do ficheiro
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {

            if(line==""){
                contador++; //atualizar fase
            }

            //1º fase: Ler tamanho matriz
            if(contador==0 & line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    NumMaintenanceInterventions = stoi(token);
                    cout << NumMaintenanceInterventions << endl; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                }
                pos = 0;
                NumAssetsTransportMatrix = stoi(line);
                cout << NumAssetsTransportMatrix << endl;
            }

            //2ª fase: Ler valores da matriz
            //int CostMatrix[NumMaintenanceInterventions][NumAssetsTransportMatrix];

            if(contador==1 & line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    CostMatrix[i][j] = stod(token);
                    cout << CostMatrix[i][j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a coluna
                }
                pos = 0;
                CostMatrix[i][j] = stod(line);
                cout << CostMatrix[i][j] << endl;
                i++; //avancar a linha
                j=0; //reset da coluna
            }

            //3ª fase: ler a procura
            //int DemandArray[NumAssetsTransportMatrix];

            if(contador==2 & line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    DemandArray[j] = stod(token);
                    cout << DemandArray[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                DemandArray[j] = stod(line);
                cout << DemandArray[j] << endl;
                j=0; //reset da linha
            }

            //4ª fase: ler a capacidade
            //int SupplyArray[NumMaintenanceInterventions];

            if(contador>2 & line!=""){
                SupplyArray[j] = stod(line);
                cout << SupplyArray[j] << endl;
                j++;
            }

        }
        file.close();
    }

    //alocar os indices da factory e warehouses <--para melhorar os nomes e o processo de referenciacao
    int factories[NumMaintenanceInterventions];
    for(int k = 0; k<NumMaintenanceInterventions; k++){
        factories[k] = k+1;
    }

    int warehouses[NumAssetsTransportMatrix];
    for(int k = 0; k<NumAssetsTransportMatrix; k++){
        warehouses[k] = k+1;
    }

    cout << "Costs:" << endl;
    for (int i = 0; i < NumMaintenanceInterventions; i++)
        for (int j = 0; j < NumAssetsTransportMatrix; j++)
            cout << factories[i] << " to " << warehouses[j] << " : " << Dist(&factories[i], &warehouses[j]) << endl;

    TsSignature<int> srcSig(NumAssetsTransportMatrix, factories, SupplyArray);
    TsSignature<int> snkSig(NumAssetsTransportMatrix, warehouses, DemandArray);

    TsFlow flow[1000];
    int flowVars = 0;

    double result = transportSimplex(&srcSig, &snkSig, Dist, flow, &flowVars);

    cout << "Total cost: " << result << endl;
    cout << "Flows:" << flowVars << endl;

    for (int i = 0; i < flowVars; i++)
        cout << factories[flow[i].from] << " to " << warehouses[flow[i].to] << " : " << flow[i].amount << endl;

}

