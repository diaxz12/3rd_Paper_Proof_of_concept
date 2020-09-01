/*
Luis Dias 3rd Paper Proof of concept
This header file contains the solver for the original problem using CPLEX
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <random>
#include <ilcplex/ilocplex.h>

#ifndef PROOF_OF_CONCEPT_ORIGINALPROBLEMSOLVER_H
#define PROOF_OF_CONCEPT_ORIGINALPROBLEMSOLVER_H

#endif //PROOF_OF_CONCEPT_ORIGINALPROBLEMSOLVER_H

using namespace std;

ILOSTLBEGIN
//definir vetores do cplex
typedef IloArray<IloNumVarArray> IloNumVarArray2;

//Variaveis globais
int AssetNumber, TimeWindow, ReplacementCapacity, DefinedBudget, MaintenanceTypes=3;//!!!Atenção que o maintenance number também é pera pôr no ficheiro de input
double InitialCondition[1000];
double AverageDegradation[100];
double PeriodDegradation[100];
double ReplacementCost[1000];
double FailureCost[1000];
double MaintenanceActionCapacity[5];
double MaintenanceAction[5][1000];
double ReplacementEffect[1000];
double FailureEffect[1000];

//Funcao para ler o input do problema original e carregar dados nas variaveis globais
void ReadOriginalInput(char const * path) {


// ||---Rotina para ler as coordenadas do ficheiro input do problema original---||
    string line;
    int i = 0,j =0; //indices da matriz
    int contador=0; //conta o numero de iteracoes a fazer dependendo de cada fase da leitura do ficheiro
    size_t pos = 0; //marca a posicao na string
    string token; // valor a extrair
    string delimiter = " "; //separador a utilizar no parse do texto (ex: ficheiro csv, tsv)
    ifstream file(path); //nome do ficheiro
    if (file.is_open()) {
        while (getline(file, line)) {

            if(line==""){
                contador++; //atualizar fase
            }

            //1º fase: Ler Numero de ativos
            if(contador==0 && line!="Asset Number" && line!=""){
                AssetNumber = stoi(line);
                cout << AssetNumber << endl;
            }

            //2º fase: Ler time window length
            if(contador==1 && line!="Time Window" && line!=""){
                TimeWindow = stoi(line);
                cout << TimeWindow << endl;
            }

            //3ª fase: Ler Initial Condition
            if(contador==2 && line!="Initial Condition" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    InitialCondition[j] = stod(token);
                    cout << InitialCondition[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                InitialCondition[j] = stod(line);
                cout << InitialCondition[j] << endl;
                j=0; //reset da linha
            }

            //4ª fase: Ler Average Degradation
            if(contador==3 && line!="Average Degradation" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    AverageDegradation[j] = stod(token);
                    cout << AverageDegradation[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                AverageDegradation[j] = stod(line);
                cout << AverageDegradation[j] << endl;
                j=0; //reset da linha
            }

            //5ª fase: Ler Period Degradation
            if(contador==4 && line!="Period Degradation" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    PeriodDegradation[j] = stod(token);
                    cout << PeriodDegradation[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                PeriodDegradation[j] = stod(line);
                cout << PeriodDegradation[j] << endl;
                j=0; //reset da linha
            }

            //6ª fase: Ler Replacement Cost
            if(contador==5 && line!="Replacement Cost" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    ReplacementCost[j] = stod(token);
                    cout << ReplacementCost[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                ReplacementCost[j] = stod(line);
                cout << ReplacementCost[j] << endl;
                j=0; //reset da linha
            }

            //7ª fase: Ler Replacement Cost
            if(contador==6 && line!="Failure Cost" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    FailureCost[j] = stod(token);
                    cout << FailureCost[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                FailureCost[j] = stod(line);
                cout << FailureCost[j] << endl;
                j=0; //reset da linha
            }

            //8ª fase: Ler Maintenance Action
            if(contador==7 && line!="Maintenance Action" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    MaintenanceAction[i][j] = stod(token);
                    cout << MaintenanceAction[i][j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a coluna
                }
                pos = 0;
                MaintenanceAction[i][j] = stod(line);
                cout << MaintenanceAction[i][j] << endl;
                i++; //avancar a linha
                j=0; //reset da coluna
            }

            //9ª fase: Ler Replacement Effect
            if(contador==8 && line!="Replacement Effect" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    ReplacementEffect[j] = stod(token);
                    cout << ReplacementEffect[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                ReplacementEffect[j] = stod(line);
                cout << ReplacementEffect[j] << endl;
                j=0; //reset da linha
            }

            //10ª fase: Ler Failure Effect
            if(contador==9 && line!="Failure Effect" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    FailureEffect[j] = stod(token);
                    cout << FailureEffect[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                FailureEffect[j] = stod(line);
                cout << FailureEffect[j] << endl;
                j=0; //reset da linha
            }

            //11ª fase: Ler Defined Budget
            if(contador==10 && line!="Defined Budget" && line!=""){
                DefinedBudget = stod(line);
                cout << DefinedBudget << endl;
            }

            //12ª fase: Ler Replacement Capacity
            if(contador==11 && line!="Replacement Capacity" && line!=""){
                ReplacementCapacity = stod(line);
                cout << ReplacementCapacity << endl;
            }

            //13ª fase: Ler Maintenance Action Capacity
            if(contador==12 && line!="Maintenance Action Capacity" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    MaintenanceActionCapacity[j] = stod(token);
                    cout << MaintenanceActionCapacity[j] << "\t"; // converter valor para a matriz
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                MaintenanceActionCapacity[j] = stod(line);
                cout << MaintenanceActionCapacity[j] << endl;
                j=0; //reset da linha
            }

        }
        file.close();
    }
    else{
        cout << "There was a problem opening or reading input file" << endl;
    }

    //||---Leitura de dados concluida---||

}

double SolverOriginalProblem(char const * path){

    // ||---Rotina para input do problema original---||
    ReadOriginalInput(path);

    //MODELO CPLEX////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    IloEnv env;

    //Declarar variaveis output resultado
    double objective_function = 0;
    double TotalTime = 0;
    double GAP = 0;

    //#####################//
    //#####Model definition//
    //#####################//
    IloModel mod(env);

    //DECISION VARIABLES
    IloNumVarArray ReplaceAsset(env, AssetNumber, 0.0, IloInfinity, ILOFLOAT);

    IloNumVarArray2 MaintenanceAction(env);
    for (int i = 0; i < AssetNumber; i++) {
        MaintenanceAction.add(IloNumVarArray(env, MaintenanceTypes, 0.0, 1.0, ILOBOOL));
    }

    //AUXILIARY VARIABLES
    IloNumVar UnderBudget(env, 0.0, IloInfinity, ILOFLOAT);
    IloNumVar OverBudget(env, 0.0, IloInfinity, ILOFLOAT);
    IloNumVar EffectiveBudget(env, 0.0, IloInfinity, ILOFLOAT);
    IloNumVarArray AssetRUL(env), AssetFailed(env);

    for (int i = 0; i < AssetNumber; i++) {
        AssetRUL.add(IloNumVarArray(env, AssetNumber, 0.0, IloInfinity, ILOFLOAT));
        AssetFailed.add(IloNumVarArray(env, AssetNumber, 0.0, 1.0, ILOBOOL));
    }

    // OBJECTIVE FUNCTION
    IloExpr RulDispersion(env), ObjectiveFunction(env);
    for (int i = 0; i < AssetNumber; i++) RulDispersion += AssetRUL[i]*ReplacementCost[i];

    ObjectiveFunction=IloSum(AssetRUL)*IloSum(ReplacementCost)/AssetNumber-RulDispersion;

    mod.add(IloMinimize(env, ObjectiveFunction));
    RulDispersion.end();
    ObjectiveFunction.end();


    //atualizacao da condicao_
    for (i = 0; i < numAsset; i++) {
        for (t = 0; t <= numLengthTW; t++) {
            IloExpr Maintenance(env);
            if (t == 0) {
                mod.add(Health[i][0] == InitialHealth[i]);
            } else {
                for (a = 0; a < numAction; a++)Maintenance += BenefitAction[i][a] * MaintenanceAction[a][i][t];
                mod.add(Health[i][t] - HealthFailure[i][t] == Maintenance + (Health[i][t - 1] - AssetDegradation[i]) + HealthReset[i][t]);
                Maintenance.end();
            }
        }
    }

    IloCplex cplex(mod);

    //Definir parametros do CPLEX
//    cplex.setParam(IloCplex::TiLim, Time_Limit);
//    cplex.setParam(IloCplex::EpGap, Gap);
//    cplex.setParam(IloCplex::TreLim, memory);
//    cplex.setParam(IloCplex::Threads, threads);


//resolver o modelo gerado
    cplex.solve();
    env.out() << "Solution status: " << cplex.getStatus() << endl;


}

