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
int AssetNumber, TimeWindow, ReplacementCapacity, DefinedBudget, MaintenanceTypes;
double InitialCondition[1000];
double AverageDegradation[100];
double PeriodDegradation[100];
double ReplacementCost[1000];
double FailureCost[1000];
double MaintenanceActionCapacity[5];
double MaintenanceActionCost[5][1000];
double ReplacementEffect[1000];
double FailureEffect[1000];
double MaintenanceActionEffect[5][1000];

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
                i=0; //reset dos indices
                j=0; //reset dos indices
            }

            //1º fase: Ler Numero de ativos
            if(contador==0 && line!="Asset Number" && line!=""){
                AssetNumber = stoi(line);
            }

            //2º fase: Ler time window length
            if(contador==1 && line!="Time Window" && line!=""){
                TimeWindow = stoi(line);
            }

            //3ª fase: Ler Initial Condition
            if(contador==2 && line!="Maintenance Types" && line!=""){
                MaintenanceTypes = stoi(line);
            }

            //4ª fase: Ler Initial Condition
            if(contador==3 && line!="Initial Condition" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    InitialCondition[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                InitialCondition[j] = stod(line);
                j=0; //reset da linha
            }

            //5ª fase: Ler Average Degradation
            if(contador==4 && line!="Average Degradation" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    AverageDegradation[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                AverageDegradation[j] = stod(line);
                j=0; //reset da linha
            }

            //6ª fase: Ler Period Degradation
            if(contador==5 && line!="Period Degradation" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    PeriodDegradation[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                PeriodDegradation[j] = stod(line);
                j=0; //reset da linha
            }

            //7ª fase: Ler Replacement Cost
            if(contador==6 && line!="Replacement Cost" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    ReplacementCost[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                ReplacementCost[j] = stod(line);
                j=0; //reset da linha
            }

            //8ª fase: Ler Failure Cost
            if(contador==7 && line!="Failure Cost" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    FailureCost[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                FailureCost[j] = stod(line);
                j=0; //reset da linha
            }

            //9ª fase: Ler Maintenance Action Cost
            if(contador==8 && line!="Maintenance Action" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    MaintenanceActionCost[i][j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a coluna
                }
                pos = 0;
                MaintenanceActionCost[i][j] = stod(line);
                i++; //avancar a linha
                j=0; //reset da coluna
            }

            //10ª fase: Ler Replacement Effect
            if(contador==9 && line!="Replacement Effect" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    ReplacementEffect[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                ReplacementEffect[j] = stod(line);
                j=0; //reset da linha
            }

            //11ª fase: Ler Failure Effect
            if(contador==10 && line!="Failure Effect" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    FailureEffect[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                FailureEffect[j] = stod(line);
                j=0; //reset da linha
            }

            //12ª fase: Ler Maintenance Action Effect
            if(contador==11 && line!="Maintenance Action Effect" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    MaintenanceActionEffect[i][j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a coluna
                }
                pos = 0;
                MaintenanceActionEffect[i][j] = stod(line);
                i++; //avancar a linha
                j=0; //reset da coluna
            }

            //13ª fase: Ler Defined Budget
            if(contador==12 && line!="Defined Budget" && line!=""){
                DefinedBudget = stod(line);
            }

            //14ª fase: Ler Replacement Capacity
            if(contador==13 && line!="Replacement Capacity" && line!=""){
                ReplacementCapacity = stod(line);
            }

            //15ª fase: Ler Maintenance Action Capacity
            if(contador==14 && line!="Maintenance Action Capacity" && line!=""){
                while ((pos = line.find(delimiter)) != string::npos) {
                    token = line.substr(0, pos); //extrair valor
                    MaintenanceActionCapacity[j] = stod(token);
                    line.erase(0, pos + delimiter.length()); //apagar valor lido da linha que foi lida
                    j++; //avancar a linha
                }
                pos = 0;
                MaintenanceActionCapacity[j] = stod(line);
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

//funcao para verificar existencia do ficheiro dos resultados resumidos
bool check_results_output(char const * path) {

    bool existe;

    //try to open file
    fstream results_summed_aux(path);

    //check of file exist
    if (!results_summed_aux.good()) {
        existe = false;
    } else {
        existe = true;
    }

    results_summed_aux.close();

    return existe;
}

//funcao para resolver problema original dada uma instancia
double SolverOriginalProblem(char const * path){

    //Variable to measure time
    double ElapsedTime;

    //Initiate clock
    clock_t beginTime = clock();

    // ||---Rotina para input do problema original---||
    ReadOriginalInput(path);

    // ||---Rotina para o modelo do CPLEX---||
    IloEnv env;
    IloModel mod(env);

    //DECISION VARIABLES
    IloNumVarArray ReplaceAsset(env, AssetNumber, 0.0, IloInfinity, ILOFLOAT);

    IloNumVarArray2 MaintenanceAction(env);
    for (int a = 0; a < MaintenanceTypes; a++) {
        MaintenanceAction.add(IloNumVarArray(env, AssetNumber, 0.0, 1.0, ILOBOOL));
    }

    //AUXILIARY VARIABLES
    IloNumVar UnderBudget(env, 0.0, IloInfinity, ILOFLOAT);
    IloNumVar OverBudget(env, 0.0, IloInfinity, ILOFLOAT);
    IloNumVar EffectiveBudget(env, 0.0, IloInfinity, ILOFLOAT);
    IloNumVarArray AssetRUL(env, AssetNumber, 0.0, IloInfinity, ILOFLOAT);
    IloNumVarArray AssetFailed(env, AssetNumber, 0.0, 1.0, ILOBOOL);


    // OBJECTIVE FUNCTION
    IloExpr RulDispersion(env), TotalCost(env), ObjectiveFunction(env);
    for (int i = 0; i < AssetNumber; i++) RulDispersion += AssetRUL[i]*ReplacementCost[i];
    for (int i = 0; i < AssetNumber; i++) TotalCost += ReplacementCost[i];

    ObjectiveFunction=OverBudget+IloSum(AssetRUL)*TotalCost/AssetNumber-RulDispersion;

    mod.add(IloMinimize(env, ObjectiveFunction));
    RulDispersion.end();
    TotalCost.end();
    ObjectiveFunction.end();


    //Restricao que permite calcular o overbudget
    IloExpr TotalFailureCosts(env), TotalMaintenanceActionCosts(env), TotalReplacementCosts(env);

    for(int i = 0; i < AssetNumber; i++)TotalFailureCosts+=FailureCost[i]*AssetFailed[i]; //Calcular os custos associados à falha

    for(int a = 0; a < MaintenanceTypes; a++){
        for (int i = 0; i < AssetNumber; i++)TotalMaintenanceActionCosts+=MaintenanceActionCost[a][i]*MaintenanceAction[a][i]; //Calcular os custos associados à manutenção
    }

    for(int i = 0; i < AssetNumber; i++)TotalReplacementCosts+=ReplacementCost[i]*ReplaceAsset[i]; //Calcular os custos associados às substituições

    //adicionar restricao dos custos
    mod.add(DefinedBudget-TotalFailureCosts-TotalMaintenanceActionCosts-TotalReplacementCosts-UnderBudget+OverBudget==0);
    TotalFailureCosts.end();
    TotalMaintenanceActionCosts.end();
    TotalReplacementCosts.end();

    //Restricao de capacidade das subsituicoes
    mod.add(IloSum(ReplaceAsset)<=ReplacementCapacity);

    //Restricao da capacidade de cada tipo de manutencao
    for(int a = 0; a < MaintenanceTypes; a++)mod.add(IloSum(MaintenanceAction[a])<=MaintenanceActionCapacity[a]);

    //Restricao que modela a condicao de cada ativo atraves do RUL
    for(int i = 0; i < AssetNumber; i++){
        IloExpr TotalMaintenanceEffect(env);
        for(int a = 0; a < MaintenanceTypes; a++)TotalMaintenanceEffect+=MaintenanceActionEffect[a][i]*MaintenanceAction[a][i];
        mod.add(AverageDegradation[i]*AssetRUL[i]-FailureEffect[i]*AssetFailed[i]==InitialCondition[i]-PeriodDegradation[i]*(1-ReplaceAsset[i])+TotalMaintenanceEffect+ReplacementEffect[i]*ReplaceAsset[i]);
        TotalMaintenanceEffect.end();
    }

    //Restricao que so permite uma acao de manutencao por periodo para cada ativo
    for(int i = 0; i < AssetNumber; i++){
        IloExpr TotalMaintenanceAction(env);
        for(int a = 0; a < MaintenanceTypes; a++)TotalMaintenanceAction+=MaintenanceAction[a][i];
        mod.add(TotalMaintenanceAction+AssetFailed[i]+ReplaceAsset[i]<=1);
        TotalMaintenanceAction.end();
    }

    IloCplex cplex(mod);

    //Definir parametros do CPLEX
//    cplex.setParam(IloCplex::TiLim, Time_Limit);
//    cplex.setParam(IloCplex::EpGap, Gap);
//    cplex.setParam(IloCplex::TreLim, memory);
//    cplex.setParam(IloCplex::Threads, threads);


    //resolver o modelo gerado
    cplex.solve();

    //end clock
    clock_t endTime = clock();

    //Calcular tempo final
    ElapsedTime = double(endTime - beginTime) / CLOCKS_PER_SEC;

    // ||---Fim da rotina para o modelo do CPLEX---||

    // ||---Print Results---||
    cout << endl << "Asset Replacement" << endl;
    for(int i = 0; i < AssetNumber; i++)cout << cplex.getValue(ReplaceAsset[i]) << "\t";
    cout << endl;

    cout << endl << "Asset Maintenance" << endl;
    for(int a = 0; a < MaintenanceTypes; a++){
        for(int i = 0; i < AssetNumber; i++)cout << cplex.getValue(MaintenanceAction[a][i]) << "\t";
        cout << endl;
    }

    cout << endl << "Asset Failure" << endl;
    for(int i = 0; i < AssetNumber; i++)cout << cplex.getValue(AssetFailed[i]) << "\t";
    cout << endl;

    cout << endl << "Asset RUL" << endl;
    for(int i = 0; i < AssetNumber; i++)cout << cplex.getValue(AssetRUL[i]) << "\t";
    cout << endl;

    // ||---Output results---||
    bool existe;
    existe = check_results_output("resultados/output_original.csv");

    //para imprimir a solucao e resultados
    ofstream output;

    //esta linha de codigo permite fazer append aos resultados resumidos obtidas
    output.open("resultados/output_original.csv", fstream::app);

    //imprimir resultados no ficheiro output_original.csv
    if (existe == true) {
        output << path << "\t" << "Original" << "\t" << AssetNumber << "\t" << TimeWindow << "\t" << MaintenanceTypes << "\t" << cplex.getStatus()
               << "\t" << ElapsedTime << "\t" << cplex.getObjValue() << "\t" << endl;
    } else {
        output << "Instance\tFormulation type\tAssets number\tTime window Length\tMaintenance Types\tSolution Status\tSolving Time\tSolution value\n";
        output << path << "\t" << "Original" << "\t" << AssetNumber << "\t" << TimeWindow << "\t" << MaintenanceTypes << "\t" << cplex.getStatus()
        << "\t" << ElapsedTime << "\t" << cplex.getObjValue() << "\t" << endl;
    }


    return cplex.getObjValue();
}

