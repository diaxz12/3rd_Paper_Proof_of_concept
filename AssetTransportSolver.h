/*
Luis Dias 3rd Paper Proof of concept
This header file contains the solver procedure for a given t problem of the asset transportation problem approximation coupled with the langragian relaxation.
*/


#ifndef PROOF_OF_CONCEPT_ASSETTRANSPORTSOLVER_H
#define PROOF_OF_CONCEPT_ASSETTRANSPORTSOLVER_H

#endif //PROOF_OF_CONCEPT_ASSETTRANSPORTSOLVER_H

#include "transportSimplex.h"
#include "OriginalProblemSolver.h"
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
double Budget; //orcamento definido em t
double CostMatrix[10][500]; //Custos do problema de transportes para o problema t
double MaintenanceCosts[10][500]; //Custos de manutencao definido na instancia do problema T
double DemandArray[500]; //Procura em t de cada ativo
double SupplyArray[10]; //Recursos de manutencao disponivel em t para cada tipo de acao de manutencao

//Permite extrair o valor da matriz de custo
double Dist(int * facPtr,int * warPtr) {
    return CostMatrix[*facPtr-1][*warPtr-1];
};


double AssetTransportSolver(char const * path){

    //Variable to measure time
    double ElapsedTime;

    //Initiate clock
    clock_t beginTime = clock();

    // ||---Rotina para ler as coordenadas do ficheiro input do problema de t---||
    string line;
    int i,j; //indices da matriz
    int contador=0; //conta o numero de iteracoes a fazer dependendo de cada fase da leitura do ficheiro ||1º fase: Ler tamanho matriz|| 2ª fase: Ler custos manutencao (cij)||
    // 3ª fase: Ler orcamento definido || 4ª fase: Ler custos dada uma determinada ação (kij) || 5ª fase: ler a procura|| 6ª fase: ler a capacidade||
    size_t pos = 0; //marca a posicao na string
    string token; // valor a extrair
    string delimiter = " "; //separador a utilizar no parse do texto (ex: ficheiro csv, tsv)
    ifstream file(path); //nome do ficheiro
    if (file.is_open()) {
        while (getline(file, line)) {

            if(line==""){
                contador++; //atualizar fase
                i=0; //reset dos indices
                j=0; //reset dos indices
            }

            //1º fase: Ler tamanho matriz
            if(contador==0 && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    NumMaintenanceInterventions = stoi(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                }
                pos = 0;
                NumAssetsTransportMatrix = stoi(line);
            }

            //2ª fase: Ler custos manutencao (cij)
            if(contador==1 && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    MaintenanceCosts[i][j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a coluna
                }
                pos = 0;
                MaintenanceCosts[i][j] = stod(line);
                if(i==NumMaintenanceInterventions-1){
                    i=0;
                }else{
                    i++; //avancar a linha
                }
                j=0; //reset da coluna
            }

            //3ª fase: Ler orcamento definido
            if(contador==2 && line!=""){
                Budget = stod(line);
            }

            //4ª fase: Ler custos dada uma determinada ação (kij)
            if(contador==3 && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    CostMatrix[i][j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a coluna
                }
                pos = 0;
                CostMatrix[i][j] = stod(line);
                i++; //avancar a linha
                j=0; //reset da coluna
            }

            //5ª fase: ler a procura
            if(contador==4 && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    DemandArray[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                DemandArray[j] = stod(line);
                j=0; //reset da linha
            }

            //6ª fase: ler a capacidade
            if(contador==5 && line!=""){
                SupplyArray[j] = stod(line);
                j++;
            }

        }
        file.close();
    }
    else{
        cout << "There was a problem opening or reading input file" << endl;
    }

    //||---Leitura de dados concluida---||


    //||---Problema de transportes---||
    //alocar os indices da factory e warehouses <--para melhorar os nomes e o processo de referenciacao
    int factories[NumMaintenanceInterventions];
    for(int k = 0; k<NumMaintenanceInterventions; k++){
        factories[k] = k+1;
    }

    int warehouses[NumAssetsTransportMatrix];
    for(int k = 0; k<NumAssetsTransportMatrix; k++){
        warehouses[k] = k+1;
    }

//    cout << "Costs:" << endl;
//    for (int i = 0; i < NumMaintenanceInterventions; i++)
//        for (int j = 0; j < NumAssetsTransportMatrix; j++)
//            cout << factories[i] << " to " << warehouses[j] << " : " << Dist(&factories[i], &warehouses[j]) << endl;

    TsSignature<int> srcSig(NumMaintenanceInterventions, factories, SupplyArray);
    TsSignature<int> snkSig(NumAssetsTransportMatrix, warehouses, DemandArray);

    TsFlow flow[1000];
    int flowVars = 0;

    //Start lagrange relaxation
    double BestResult = 100000; //variável que guarda o melhor valor. é preciso inicializar com um valor muito mau

    //Guardar os valores originais da matriz de custo
    double InitalCostMatrix[NumMaintenanceInterventions][NumAssetsTransportMatrix];

    for(int i=0; i<NumMaintenanceInterventions; i++){
        for(int j=0; j<NumAssetsTransportMatrix; j++){
            InitalCostMatrix[i][j]= CostMatrix[i][j];
        }
    }

    //Lagrange Relaxation subroutine
    double RealCosts,TotalMaintenanceCosts=0;

    for(double langrange=0; langrange<=2; langrange=langrange+0.25){//!!!atenção que o número de iteracoes está hardcoded.

        //modificar os valores da tabela kij com base no multipliador de langrange
        for(int i=0; i<NumMaintenanceInterventions; i++){
            for(int j=0; j<NumAssetsTransportMatrix; j++){
                CostMatrix[i][j]=InitalCostMatrix[i][j]+langrange*MaintenanceCosts[i][j];
            }
        }

        //atualizar resultados para F.O. de lagrange
        double IterationResult = transportSimplex(&srcSig, &snkSig, Dist, flow, &flowVars);//!!!A primeira iteracao não está a dar resultados certos

        //Calcular Funcao objetivo do problema original
        for (int i = 0; i < flowVars; i++){
            RealCosts = RealCosts + InitalCostMatrix[flow[i].from][flow[i].to] - 2000;//!!!atenção que o 2000 é para evitar valores negativos. é preciso alterar mais tarde esta parte.
            TotalMaintenanceCosts = TotalMaintenanceCosts + MaintenanceCosts[flow[i].from][flow[i].to];
        }

        //Contabilização do overbudget caso exista
        if(TotalMaintenanceCosts>Budget){
            RealCosts=RealCosts+TotalMaintenanceCosts-Budget;
        }

        cout << "Total cost langrange: " << IterationResult << endl; //Resultado com base na matriz de custos
        cout << "Real Total cost: " << RealCosts << endl; //Resultado com base nos custos do problema original

        //Update do lower bound
        if(RealCosts<BestResult){
            BestResult=RealCosts;
        }

        //Reset Solution Values at the end of each iteration
        RealCosts=0;
        TotalMaintenanceCosts=0;
    }

    //Output da solucao final
    for (int i = 0; i < flowVars; i++)
        cout << factories[flow[i].from] << " to " << warehouses[flow[i].to] << " : " << flow[i].amount << endl;

    //end clock
    clock_t endTime = clock();

    //Calcular tempo final
    ElapsedTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

    cout << "Solving time (s) = " << ElapsedTime << endl;

    //||---Fim problema de transportes para o periodo t---||

    // ||---Output results---||
    bool existe;
    existe = check_results_output("resultados/output_langrange.csv");

    //para imprimir a solucao e resultados
    ofstream output;

    //esta linha de codigo permite fazer append aos resultados resumidos obtidas
    output.open("resultados/output_langrange.csv", fstream::app);

    //imprimir resultados no ficheiro output_original.csv
    if (existe == true) {
        output << path << "\t" << "Langrange" << "\t" << AssetNumber << "\t" << TimeWindow << "\t" << MaintenanceTypes << "\t" << "To be defined"
               << "\t" << ElapsedTime << "\t" << BestResult << "\t" << endl;
    } else {
        output << "Instance\tFormulation type\tAssets number\tTime window Length\tMaintenance Types\tSolution Status\tSolving Time\tSolution value\n";
        output << path << "\t" << "Langrange" << "\t" << AssetNumber << "\t" << TimeWindow << "\t" << MaintenanceTypes << "\t" << "To be defined"
               << "\t" << ElapsedTime << "\t" << BestResult << "\t" << endl;
    }

    return BestResult;
}