/*
Luis Dias 3rd Paper Proof of concept
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

    //Rotina para ler as coordenadas do ficheiro dados
    string line;
    if(argc >= 1){
        int i,j; //indices da matriz
        int contador=0; //conta o numero de iteracoes a fazer dependendo de cada fase da leitura do ficheiro ||1º fase: Ler tamanho matriz|| 2ª fase: Ler valores da matriz|| 3ª fase: ler a procura|| 4ª fase: ler a capacidade||
        size_t pos = 0; //marca a posicao na string
        string token; // valor a extrair
        string delimiter = " "; //separador a utilizar no parse do texto (ex: ficheiro csv, tsv)
        ifstream file("Optimization_input.txt"); //nome do ficheiro
        if (file.is_open()) {
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
                if(contador>2 & line!=""){
                    SupplyArray[j] = stod(line);
                    cout << SupplyArray[j] << endl;
                    j++;
                }

            }
            file.close();
        }
        else{
            cout << "There was a problem opening or reading input file" << endl;
        }
    }else{
        cerr << "File not found \n";
        exit (EXIT_FAILURE);
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

