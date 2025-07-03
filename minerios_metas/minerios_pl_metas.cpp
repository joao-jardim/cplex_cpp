#include<iostream>
#include<stdio.h>
#include <sstream>

#include <ilcplex/ilocplex.h>


typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumVarMatrix> IloNumVar3Matrix;
typedef IloArray<IloNumVar3Matrix> IloNumVar4Matrix;

using namespace std;

int main(int argc, char *argv[])
{

    // Declarando conjuntos de entrada
    // I -> conjunto de Pilhas
    // J -> conjunto de Parametros de controle
    int I, J;

    // Dados de entrada dependentes dos conjuntos
    char** NomePilha; // por pilha
    float* QtdMax; // Qui Quantidade Maxima em toneladas existente na pilha
    //float* Custo; // Custo de uma tonelada de minerio da pilha
    float Qtd; // p Quantidade em toneladas da mistura a ser formada

    char** NomeParametro; // por parametro
    float* TeorMin; // Tlj Porcentagem Minima admissível
    float* TeorRec; // Trj Porcentagem Maxima admissível
    float* TeorMax; // Tuj Porcentagem Maxima admissível
    float** Teor; // Tij Porcentagem de cada parametro em uma tonelada de cada pilha
    float* PesoMetaNeg; // wnmjPeso para o desvio negativo de cada parâmetro
    float* PesoMetaPos; // wpmj Peso para o desvio postivio de cada parâmetro
    float* PesoNeg; // wnej Peso para o desvio negativo de cada parâmetro
    float* PesoPos; // wpej Peso para o desvio postivio de cada parâmetro


    // Leitura do Arquivo
    FILE* fp = fopen(argv[1],"r");
    if( fp == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);
    }

    // Leitura dos conjuntos
    fscanf(fp, "%d", &I);
    fscanf(fp, "%d", &J);

    // Definindo os dados de entrada de acordo com o tamanho dos conjuntos lidos
    // Por Pilha
    NomePilha = new char*[I];
    for(int f = 0; f < I; f++){
        NomePilha[f] = new char[51];
    }
    QtdMax = new float[I];
    // Por Parametro
    NomeParametro = new char*[J];
    for(int c = 0; c < J; c++){
        NomeParametro[c] = new char[51];
    }

    Teor = new float*[I];
    for (int i = 0; i < I; i++) {
        Teor[i] = new float[J];
    }

    TeorMin = new float[J];
    TeorRec = new float[J];
    TeorMax = new float[J];
    PesoMetaNeg = new float[J];
    PesoMetaPos = new float[J];
    PesoNeg = new float[J];
    PesoPos = new float[J];

    // Após a declaração dos tamanhos dos dados de entrada, realizando a leitura
    // Por Pilha
    for (int i = 0; i < I; i++) {
        fscanf(fp, "%s %f %f", NomePilha[i], &QtdMax[i]);
    }

    // Por Parâmetro
    for (int j = 0; j < J; j++) {
        fscanf(fp, "%s", NomeParametro[j]);
        for (int i = 0; i < I; i++) {
            fscanf(fp, "%f", &Teor[i][j]); // Corrigido: Teor[i][j]
        }
        fscanf(fp, "%f %f %f %f %f %f %f", &TeorMin[j], &TeorRec[j], &TeorMax[j], &PesoMetaNeg[j], &PesoMetaPos[j], &PesoNeg[j], &PesoPos[j]);
    }

    fscanf(fp, "%f", &Qtd);

    // Impressão para Verificação dos dados
    printf("I: %d\n", I);
    printf("J: %d\n", J);
    printf("Pilhas:\nNome\tQtdMax\n");
    for (int i = 0; i < I; i++) {
        printf("%s\t%.2f\n", NomePilha[i], QtdMax[i]);
    }
    printf("Parâmetros:\nNome\t");
    for (int i = 0; i < I; i++) printf("%s\t", NomePilha[i]);
    printf("TeorMin\tTeorMax\tPesoMetaNegativo\tPesoMetaPositivo\tPesoNegativo\tPesoPositivo\n");
    for (int j = 0; j < J; j++) {
        printf("%s\t", NomeParametro[j]);
        for (int i = 0; i < I; i++) {
            printf("%.2f\t", Teor[i][j]);
        }
        printf("%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", TeorMin[j],TeorRec[j], TeorMax[j], PesoMetaNeg[j],PesoMetaPos[j], PesoNeg[j], PesoPos[j]);
    }
    printf("Qtd: %.2f\n", Qtd);


    // DECLARANDO O AMBIENTE E O MODELO MATEMATICO
    IloEnv env;
	IloModel modelo(env);

    // DECLARAÇÃO DE VARIÁVEIS
    // parametroambiente, tamanho, valor min, valor max, tipo (ILOINT, ILOFLOAT, ILOBOOL)

    // Quantidade de minerio a ser retirado da pilha i ∈ Pilhas
    IloNumVarArray x(env, I, 0, IloInfinity, ILOFLOAT);
    for (int i = 0; i < I; i++) {
        stringstream var;
        var << "x[" << NomePilha[i] << "]";
        x[i].setName(var.str().c_str());
        modelo.add(x[i]);
    }

    // Desvio negativo da Meta de cada parametro
    IloNumVarArray dnm(env, J, 0, IloInfinity, ILOFLOAT);
    for (int j = 0; j < J; j++) {
        stringstream var;
        var << "dnm[" << NomeParametro[j] << "]";
        dnm[j].setName(var.str().c_str());
        modelo.add(dnm[j]);
    }

    // Desvio Positivo da Meta de cada parametro
    IloNumVarArray dpm(env, J, 0, IloInfinity, ILOFLOAT);
    for (int j = 0; j < J; j++) {
        stringstream var;
        var << "dpm[" << NomeParametro[j] << "]";
        dpm[j].setName(var.str().c_str());
        modelo.add(dpm[j]);
    }

    // Desvio negativo e positivo de cada parametro na mistura
    IloNumVarArray dn(env, J, 0, IloInfinity, ILOFLOAT);
    for (int j = 0; j < J; j++) {
        stringstream varNeg;
        //stringstream varPos;

        varNeg << "dn[" << NomeParametro[j] << "]";
        //varPos << "dp[" << NomeParametro[j] << "]";

        dn[j].setName(varNeg.str().c_str());
        //d[j].setName(varPos.str().c_str());

        modelo.add(dn[j]);
    }

    // Desvio positivo de cada parametro na mistura
    IloNumVarArray dp(env, J, 0, IloInfinity, ILOFLOAT);
    for (int j = 0; j < J; j++) {
        stringstream var;
        var << "dp[" << NomeParametro[j] << "]";
        dp[j].setName(var.str().c_str());
        modelo.add(dp[j]);
    }

    // DECLARAÇÃO DA FUNÇÃO OBJETIVO
    IloExpr fo(env);

    IloExpr soma1(env);
    IloExpr soma2(env);
    for (int j = 0; j < J; j++) { // para todo j pertencente a parametros
        soma1 += (PesoPos[j] * dp[j]) + (PesoNeg[j] * dn[j]);
        soma2 += (PesoMetaPos[j] * dpm[j]) + (PesoMetaNeg[j] * dnm[j]);

        fo += (soma1 + soma2);
    }
    modelo.add(IloMinimize(env, fo)); // Minimizar custo

    // Restrições: qt do parametro supere a qtd maxima permitida em dp
    for (int j = 0; j < J; j++) { //para todo j em parametros
        //-dn [j] <= quantP <= d[j]
        IloExpr quantP(env);
        
        for (int i = 0; i < I; i++) { //somátorio de para todo i em pilhas
            quantP += (((Teor[i][j] - TeorRec[j])* x[i]) + dnm[j] - dpm[j]);
        }     
        // IloRange parametros: ambiente, valor min, expressão, valor maximo
        IloRange restQuantP(env, 0, quantP, 0);

        stringstream rest;
        rest << "QuantidadeP[" << NomeParametro[j] << "]";
        restQuantP.setName(rest.str().c_str());

        modelo.add(restQuantP);
    }

    // Restrições: qt do parametro seja inferior a qtd minima permitida em d
    for (int j = 0; j < J; j++) { //para todo j em parametros
        //-dn [j] <= quantP <= d[j]
        IloExpr quantP(env);
        
        for (int i = 0; i < I; i++) { //somátorio de para todo i em pilhas
            quantP += (((Teor[i][j] - TeorMin[j])* x[i]) + dn[j]);
        }     
        // IloRange parametros: ambiente, valor min, expressão, valor maximo
        IloRange restQuantP(env, 0, quantP, IloInfinity);

        stringstream rest;
        rest << "QuantidadeU[" << NomeParametro[j] << "]";
        restQuantP.setName(rest.str().c_str());

        modelo.add(restQuantP);
    }
    
    // Restrição: Quantidade a ser retomada é limitada
    for (int i = 0; i < I; i++) { //para todo i em pilhas
        //IloExpr quant(env);
        IloRange restQuant(env, -IloInfinity, x[i], QtdMax[i]);
        modelo.add(restQuant);
    } 

    // Restrição: A mistura deve ter peso total p
    IloExpr peso(env);
    for (int i = 0; i < I; i++) { //para todo i em pilhas
        peso += x[i];
    }
    IloRange restPeso(env, Qtd, peso, Qtd);
    modelo.add(restPeso);

    // Restrição: Não negatividade
    for (int i = 0; i < I; i++) { //para todo i em pilhas
    //IloExpr quant(env);
        IloRange restNeg(env, 0, x[i], IloInfinity);
        modelo.add(restNeg);
    } 

    // CARREGANDO E RESOLVENDO O MODELO
    IloCplex cplex(modelo);
    cplex.exportModel("minerios_pl_metas.lp");
    cplex.solve();

    // IMPRESSÃO DOS RESULTADOS
    IloAlgorithm::Status status = cplex.getStatus();
    const char* statusString;
    switch (status) {
        case IloAlgorithm::Optimal:
            statusString = "Optimal";
            break;
        case IloAlgorithm::Infeasible:
            statusString = "Infeasible";
            break;
        case IloAlgorithm::Unbounded:
            statusString = "Unbounded";
            break;
        case IloAlgorithm::Feasible:
            statusString = "Feasible";
            break;
        default:
            statusString = "Unknown";
            break;
    }

    printf("\nStatus da solução: %s\n", statusString);
    printf("\nCusto total: %.2f\n", cplex.getObjValue());
    printf("\nSolução:\n");
    for (int i = 0; i < I; i++) {
        double valor = cplex.getValue(x[i]);
        printf("\n%s: %.2f toneladas\n", NomePilha[i], valor);
    }

    return 0;
}