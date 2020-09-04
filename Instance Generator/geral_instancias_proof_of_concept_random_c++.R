#############################################
###Instance Generator for proof of concept###
#############################################

getwd()
library(readxl)

#!!!!DISCLAIMER!!!#
#Dado que a matriz de custos não pode ter valores negativos, é acrescentado uma constante para evitar valores negativos
#Ver linha 173
NegativeValuesConstant<-2000

#diretorio onde queremos colocar as instancias
path_instancias<-"/Users/LuisDias/Desktop/Doutoramento DEGI/A-Papers LUIS DIAS/3_paper/3 - Development/C++ development/Proof_of_concept/build/data/"

#variavel a usar na impressao dos ficheiros
iteracoes<-1 #numero de instancias a gerar por cada classe de instancia (N[X])
AssetNumber_iteration<-c(100) #numero de ativos


############################################################################################
###-----Funcoes para gerar a instancia do problema original e do problema aproximado-----###
############################################################################################

#funcao para criar os deltas de degradacao 
simular_degradacao_linear<-function(AssetNumber,Parametros_gamma_distribution){
  
  #guardar resultados da degradacao
  vetor_degradacao<-array(data=NA,dim=AssetNumber)
  
  #gamma process para cada ativo
  for(i in 1:AssetNumber){
    vetor_degradacao[i]<-round(qgamma(sample(seq(0.01,0.99,by=0.01),1),shape=Parametros_gamma_distribution$alpha[i],scale =Parametros_gamma_distribution$beta[i]), digits = 2)
  }
  
  return(vetor_degradacao)
}

#função que permite criar o ficheiro de texto
criar_instancia_original_problem<-function(AssetNumber,TimeWindow,MaintenanceTypes,InitialHealth,AssetDegradation,PeriodDegradation,CostReplacingAsset,CostFailure,CostAction,ReplacementEffect,FailureEffect,MaintenanceEffect,DefinedBudget,ReplacementCapacity,MaintenanceActionCapacity){
  
  #inicializar variavel
  string<-""
  #AssetNumber
  string<-paste(string,"Asset Number\n",AssetNumber,"\n\n",sep = "")
  #TimeWindow
  string<-paste(string,"Time Window\n",TimeWindow,"\n\n",sep = "")
  #MaintenanceTypes
  string<-paste(string,"Maintenance Types\n",MaintenanceTypes,"\n\n",sep = "")
  #InitialHealth
  string<-paste(string,"Initial Condition\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,InitialHealth[i]," ",sep = "")
    }else{
      string<-paste(string,InitialHealth[i],"\n\n",sep = "")
    }
  }
  #AssetDegradation
  string<-paste(string,"Average Degradation\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,AssetDegradation[i]," ",sep = "")
    }else{
      string<-paste(string,AssetDegradation[i],"\n\n",sep = "")
    }
  }
  #PeriodDegradation
  string<-paste(string,"Period Degradation\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,PeriodDegradation[i]," ",sep = "")
    }else{
      string<-paste(string,PeriodDegradation[i],"\n\n",sep = "")
    }
  }
  #CostReplacingAsset
  string<-paste(string,"Replacement Cost\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,CostReplacingAsset[i]," ",sep = "")
    }else{
      string<-paste(string,CostReplacingAsset[i],"\n\n",sep = "")
    }
  }
  #CostFailure
  string<-paste(string,"Failure Cost\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,CostFailure[i]," ",sep = "")
    }else{
      string<-paste(string,CostFailure[i],"\n\n",sep = "")
    }
  }
  #CostAction
  string<-paste(string,"Maintenance Action\n",sep = "") 
  for(a in 1:MaintenanceTypes){
    for (i in 1:AssetNumber) {
      if(i<AssetNumber){
        string<-paste(string,CostAction[i,a]," ",sep = "")
      }else{
        string<-paste(string,CostAction[i,a],"\n",sep = "")
      }
    }
    if(a==MaintenanceTypes){
      string<-paste(string,"\n",sep = "")
    }
  }
  #ReplacementEffect
  string<-paste(string,"Replacement Effect\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,ReplacementEffect[i]," ",sep = "")
    }else{
      string<-paste(string,ReplacementEffect[i],"\n\n",sep = "")
    }
  }
  #FailureEffect
  string<-paste(string,"Failure Effect\n",sep = "") 
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,FailureEffect[i]," ",sep = "")
    }else{
      string<-paste(string,FailureEffect[i],"\n\n",sep = "")
    }
  }
  #MaintenanceEffect
  string<-paste(string,"Maintenance Action Effect\n",sep = "") 
  for(a in 1:MaintenanceTypes){
    for (i in 1:AssetNumber) {
      if(i<AssetNumber){
        string<-paste(string,MaintenanceEffect[i,a]," ",sep = "")
      }else{
        string<-paste(string,MaintenanceEffect[i,a],"\n",sep = "")
      }
    }
    if(a==MaintenanceTypes){
      string<-paste(string,"\n",sep = "")
    }
  }
  #DefinedBudget
  string<-paste(string,"Defined Budget\n",DefinedBudget,"\n\n",sep = "")
  #ReplacementCapacity
  string<-paste(string,"Replacement Capacity\n",ReplacementCapacity,"\n\n",sep = "")
  #MaintenanceActionCapacity
  string<-paste(string,"Maintenance Action Capacity\n",sep = "") 
  for (i in 1:MaintenanceTypes){
    if(i<MaintenanceTypes){
      string<-paste(string,MaintenanceActionCapacity[i]," ",sep = "")
    }else{
      string<-paste(string,MaintenanceActionCapacity[i],sep = "")
    }
  }
  
  return(string)
}

criar_instancia_approximated_problem<-function(MaxHealth,AssetNumber,TimeWindow,MaintenanceTypes,InitialHealth,AssetDegradation,PeriodDegradation,CostReplacingAsset,CostFailure,CostAction,ReplacementEffect,FailureEffect,MaintenanceEffect,DefinedBudget,ReplacementCapacity,MaintenanceActionCapacity){
  
  #RUL Matrix dependendo de cada intervencao
  RULMatrix<-matrix(data = NA,nrow = (MaintenanceTypes+2),ncol = AssetNumber)##O valor 2 inclui a intervenção de substituição e o não fazer nada
  RULMatrix[1,]<-MaxHealth/AssetDegradation##RUL com base na degradacao media apos substituicao
  for(i in 1:MaintenanceTypes){
    RULMatrix[i+1,]<-(InitialHealth+MaintenanceEffect[,i]-PeriodDegradation)/AssetDegradation##RUL com base na degradacao media apos uma intervencao de manutencao de um dado tipo
  }
  RULMatrix[(MaintenanceTypes+2),]<-(InitialHealth-PeriodDegradation)/AssetDegradation#RUL com base na degradacao media apos não fazer nada
  
  #Inicializar matriz de custos
  CostMatrix<-matrix(data = NA,nrow = (MaintenanceTypes+2),ncol = AssetNumber)##O valor 2 inclui a intervenção de substituição e o não fazer nada
  #calcular matriz de custos para o problema aproximado (Cij)
  for(i in 1:(MaintenanceTypes+2)){
    for(j in 1:AssetNumber){
      if(RULMatrix[i,j]<0 & i<(MaintenanceTypes+2)){
        CostMatrix[i,j]<-10000#Big M para os casos em que existir falha com uma intervencao de manutencao colocar custos proibitivos
      }else if(RULMatrix[i,j]<0 & i==(MaintenanceTypes+2)){
        CostMatrix[i,j]<-round(RULMatrix[1,j]*sum(CostReplacingAsset)/AssetNumber-RULMatrix[1,j]*CostReplacingAsset[j])#assumimos que o ativo quando falha é substituido no caso de não fazer nada
      }else{
        CostMatrix[i,j]<-round(RULMatrix[i,j]*sum(CostReplacingAsset)/AssetNumber-RULMatrix[i,j]*CostReplacingAsset[j])
      }
    }
  }
  #!!!!DISCLAIMER!!!#
  #Dado que a matriz de custos não pode ter valores negativos, é acrescentado uma constante de +500 para evitar valores negativos
  CostMatrix<-CostMatrix+NegativeValuesConstant
  
  #####Output data
  
  #inicializar variavel
  string<-""
  #Number of possible interventions and AssetNumber
  string<-paste(string,(MaintenanceTypes+2)," ",AssetNumber,"\n\n",sep = "")
  #Maintenance costs para cada acao de manutencao possivel (substituicao, manutencao ou não fazer nada) no problema original
  #substituicao
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,CostReplacingAsset[i]," ",sep = "")
    }else{
      string<-paste(string,CostReplacingAsset[i],"\n",sep = "")
    }
  }
  #manutencao
  for(a in 1:MaintenanceTypes){
    for (i in 1:AssetNumber) {
      if(i<AssetNumber){
        string<-paste(string,CostAction[i,a]," ",sep = "")
      }else{
        string<-paste(string,CostAction[i,a],"\n",sep = "")
      }
    }
  }
  #não fazer nada
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      if(RULMatrix[MaintenanceTypes+2,i]<0){
        string<-paste(string,CostFailure[i]," ",sep = "")
      }else{
        string<-paste(string,0," ",sep = "")
      }
    }else{
      if(RULMatrix[MaintenanceTypes+2,i]<0){
        string<-paste(string,CostFailure[i],"\n\n",sep = "")
      }else{
        string<-paste(string,0,"\n\n",sep = "")
      }
    }
  }
  
  #DefinedBudget
  string<-paste(string,DefinedBudget,"\n\n",sep = "")
  
  #CostMatrix
  for(a in 1:(MaintenanceTypes+2)){
    for (i in 1:AssetNumber) {
      if(i<AssetNumber){
        string<-paste(string,CostMatrix[a,i]," ",sep = "")
      }else{
        string<-paste(string,CostMatrix[a,i],"\n",sep = "")
      }
    }
  }
  string<-paste(string,"\n",sep = "")
  
  #Transportation problem Demand
  for (i in 1:AssetNumber) {
    if(i<AssetNumber){
      string<-paste(string,1," ",sep = "")
    }else{
      string<-paste(string,1,"\n\n",sep = "")
    }
  }
  #Transportation problem capacities
  string<-paste(string,ReplacementCapacity,"\n",sep = "")
  for (i in 1:MaintenanceTypes) {
    string<-paste(string,MaintenanceActionCapacity[i],"\n",sep = "")
  }
  string<-paste(string,AssetNumber,sep = "")
  
  return(string)
}

#########################################################
###-----Rotina para gerar cada uma das instancias-----###
########################################################

#escolher o horizonte para gerar na classe de instancias
for(AssetNumber in AssetNumber_iteration){
  for(contador in 1:iteracoes){
    #verificacao do diretorio e iteracao atual do ciclo
    print(path_instancias)
    print(paste("iteteracao",contador,sep = " "))
    
    #parametros do modelo
    TimeWindow<-5
    MaintenanceTypes<-3
    MaxHealth<-100
    PenaltyCost<-400
    
    #Condicao inicial dos ativos
    InitialHealth<-array(data=NA,dim = AssetNumber)
    for (i in 1:AssetNumber) {
      InitialHealth[i]<-round(sample(seq(MaxHealth*0.05,MaxHealth),1))
    }
    
    #Gerar degradacoes média e do cenario
    Parametros_gamma_distribution<-data.frame("alpha"=array(data = NA,dim = AssetNumber),"beta"=array(data = NA,dim = AssetNumber))
    #Degradacao media do ativo
    AssetDegradation<-array(dim = length(AssetNumber))
    for(i in 1:AssetNumber){
      #shape parameter da gaussian distribution
      Parametros_gamma_distribution$alpha[i]<-sample(seq(4,7,by=0.5),1)
      #scale parameter da gaussian distribution
      Parametros_gamma_distribution$beta[i]<-sample(seq(1.5,2.5,by=0.25),1)
      
      #utiliza-se a distribuicao BirnbaumSaunders para calcular as probabilidades do RUL
      C_beta<-InitialHealth[i]/Parametros_gamma_distribution$beta[i]
      alpha<-1/sqrt(C_beta)
      
      #calculo da degradacao media
      AssetDegradation[i]<-round(InitialHealth[i]/(C_beta/Parametros_gamma_distribution$alpha[i]))
    }
    
    #Degradacao do periodo
    PeriodDegradation<-round(simular_degradacao_linear(AssetNumber,Parametros_gamma_distribution))
    
    #utilizar este parâmetro para definir os custos de substituição normal
    Penalty_multiplier<-0.5
    
    #permite gerar os dados das instâncias
    #gerar os custos de falha
    CostFailure<-rep(1,AssetNumber)
    for(i in 1:AssetNumber){
      CostFailure[i]<-round(rnorm(1,mean=PenaltyCost,sd=50))
    }
    
    #gerar os custos para as ações de substituição devido a falha
    CostReplacingAsset<-array(data=0,dim = AssetNumber)
    for(i in 1:AssetNumber){
      CostReplacingAsset[i]<-Penalty_multiplier*CostFailure[i]
    }
    
    #gerar os beneficios das ações de manutenção
    #atencao que o beneficio das manutencoes sao calculados atraves do valor esperado da degradacao dos ativos
    MaintenanceEffect<-matrix(data=NA,nrow = AssetNumber, ncol = MaintenanceTypes)
    ratio<-c(0.8,0.6,0.3)
    for(i in 1:AssetNumber){
      for(j in 1:MaintenanceTypes){
        MaintenanceEffect[i,j]<-round(AssetDegradation[i]*ratio[j],digits = 2)
      }
    }
    
    #gerar os custos para as ações de manutenção
    CostAction<-matrix(data=0,nrow = AssetNumber, ncol = MaintenanceTypes)
    multiplier<-0
    for(i in 1:AssetNumber){
      for(j in 1:MaintenanceTypes){
        #e preciso calcular o ratio de custo por unidade de forma a evitar que seja mais benefica a manutencao do que a substituicao de forma sistematica
        multiplier<-sample(seq(CostReplacingAsset[i]/MaxHealth,CostReplacingAsset[i]/MaxHealth*1/Penalty_multiplier,by = 0.1),1)
        CostAction[i,j]<-round(MaintenanceEffect[i,j]*multiplier,digits = 0)
      }
    }
    
    #Gerar o replacement effect
    ReplacementEffect<-rep(0,AssetNumber)
    for(i in 1:AssetNumber){
      ReplacementEffect[i]<-MaxHealth-InitialHealth[i]
    }
    
    #Gerar Failure effect
    FailureEffect<-rep(0,AssetNumber)
    for(i in 1:AssetNumber){
      if(InitialHealth[i]-PeriodDegradation[i]<0){
        FailureEffect[i]<-abs(InitialHealth[i]-PeriodDegradation[i])
      }else{
        FailureEffect[i]<-0
      }
    }
    
    #Total de manutencoes (Substituicoes e manutencoes)
    TotalMaintenanceInterventions<-round(sample(seq(AssetNumber*0.25,AssetNumber,by=1),1)) #!!!pelo menos 1/4 dos ativos pode ter manutencao
    
    #Gerar Replacement capacity
    ReplacementCapacity<-sample(seq(0,TotalMaintenanceInterventions,by=1),1)
    
    #Gerar Maintenance Action Capacity
    MaintenanceActionCapacity<-rep(0,MaintenanceTypes)
    for(i in 1:MaintenanceTypes){
      if(i==1){
        MaintenanceActionCapacity[i]<-sample(seq(0,TotalMaintenanceInterventions-ReplacementCapacity,by=1),1) 
      }else{
        MaintenanceActionCapacity[i]<-sample(seq(0,TotalMaintenanceInterventions-ReplacementCapacity-sum(MaintenanceActionCapacity[1:(i-1)]),by=1),1)
      }
    }
    
    #Gerar Defined Budget 
    #Maintenance Action Costs
    MaintenanceActionBudget<-rep(0,MaintenanceTypes)
    for(i in 1:MaintenanceTypes){
      MaintenanceActionBudget[i]<-MaintenanceActionCapacity[i]*max(CostAction[,i])*sample(seq(0,1,by=0.01),1)
    }

    #Replacement Costs
    ReplacementBudget<-ReplacementCapacity*max(CostReplacingAsset)*sample(seq(0,1,by=0.01),1)
    
    #Get Defined Budget
    DefinedBudget<-round(ReplacementBudget+sum(MaintenanceActionBudget))
    
    #criar ficheiro de texto
    string<-criar_instancia_original_problem(AssetNumber,TimeWindow,MaintenanceTypes,InitialHealth,AssetDegradation,PeriodDegradation,CostReplacingAsset,CostFailure,
                                             CostAction,ReplacementEffect,FailureEffect,MaintenanceEffect,DefinedBudget,ReplacementCapacity,MaintenanceActionCapacity)
    
    #Create instance para o problema original
    fileConn<-file(paste(path_instancias,"teste_original",".txt",sep=""))
    writeLines(string, fileConn)
    close(fileConn)
    
    #Create instance para o problema aproximado
    string<-criar_instancia_approximated_problem(MaxHealth,AssetNumber,TimeWindow,MaintenanceTypes,InitialHealth,AssetDegradation,PeriodDegradation,CostReplacingAsset,CostFailure,
                              CostAction,ReplacementEffect,FailureEffect,MaintenanceEffect,DefinedBudget,ReplacementCapacity,MaintenanceActionCapacity)
    
    #exemplo de como imprimir os dados requeridos
    #fileConn<-file(paste("output_final_AssetNumbers_",AssetNumber,"_TW_",numLengthTW,"_numscenarios_",numScenarios,"_instancia_",contador,".txt"))
    fileConn<-file(paste(path_instancias,"teste_aproximado",".txt",sep=""))
    writeLines(string, fileConn)
    close(fileConn)
    
  }
}



