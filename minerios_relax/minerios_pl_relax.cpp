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
    float* QtdMax; // Quantidade Maxima em toneladas existente na pilha
    //float* Custo; // Custo de uma tonelada de minerio da pilha
    float Qtd; // Quantidade em toneladas da mistura a ser formada

    char** NomeParametro; // por parametro
    float* TeorMin; // Porcentagem Minima admissível
    float* TeorMax; // Porcentagem Maxima admissível
    float** Teor; // Porcentagem de cada parametro em uma tonelada de cada pilha
    float* PesoNeg; // Peso para o desvio negativo de cada parâmetro
    float* PesoPos; // Peso para o desvio postivio de cada parâmetro


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

    Teor = new float*[I]; // Corrigido: Teor[i][j] para pilha i, parâmetro j
    for (int i = 0; i < I; i++) {
        Teor[i] = new float[J];
    }
    TeorMin = new float[J];
    TeorMax = new float[J];
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
        fscanf(fp, "%f %f %f %f", &TeorMin[j], &TeorMax[j], &PesoNeg[j], &PesoPos[j]);
    }

    fscanf(fp, "%f", &Qtd);

// Impressão para Verificação dos dados
    printf("I: %d\n", I);
    printf("J: %d\n", J);
    printf("Pilhas:\nNome\tQtdMax\n");
    for (int i = 0; i < I; i++) {
        printf("%s\t%.2f\t%.2f\n", NomePilha[i], QtdMax[i]);
    }
    printf("Parâmetros:\nNome\t");
    for (int i = 0; i < I; i++) printf("%s\t", NomePilha[i]);
    printf("TeorMin\tTeorMax\tPesoNegativo\tPesoPositivo\n");
    for (int j = 0; j < J; j++) {
        printf("%s\t", NomeParametro[j]);
        for (int i = 0; i < I; i++) {
            printf("%.2f\t", Teor[i][j]);
        }
        printf("%.2f\t%.2f\t%.2f\t%.2f\n", TeorMin[j], TeorMax[j], PesoNeg[j], PesoPos[j]);
    }
    printf("Qtd: %.2f\n", Qtd);


    // DECLARANDO O AMBIENTE E O MODELO MATEMATICO
    IloEnv env;
	IloModel modelo(env);

    // DECLARAÇÃO DE VARIÁVEIS
    // Quantidade de minerio a ser retirado da pilha i ∈ Pilhas
    //Aqui eu só adiciono uma variavel, mas eu nao especifico o que ela faz?
    IloNumVarArray x(env, I, 0, IloInfinity, ILOFLOAT);
    for (int i = 0; i < I; i++) {
        stringstream var;
        var << "x[" << NomePilha[i] << "]";
        x[i].setName(var.str().c_str());
        modelo.add(x[i]);
    }

    // Desvio negativo e positivo de cada parametro na mistura
    IloNumVarArray d(env, J, 0, IloInfinity, ILOFLOAT);
    for (int j = 0; j < J; j++) {
        stringstream varNeg;
        //stringstream varPos;

        varNeg << "dn[" << NomeParametro[j] << "]";
        //varPos << "dp[" << NomeParametro[j] << "]";

        d[j].setName(varNeg.str().c_str());
        //d[j].setName(varPos.str().c_str());

        modelo.add(d[j]);
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
        soma1 += PesoPos[j] * dp[j];
        soma2 += PesoNeg[j] * d[j];

        fo += (soma1 + soma2);
    }
    modelo.add(IloMinimize(env, fo)); // Minimizar custo

    // Restrições: qt do parametro supere a qtd maxima permitida em dp
    for (int j = 0; j < J; j++) { //para todo j em parametros
        //-dn [j] <= quantP <= d[j]
        IloExpr quantP(env);
        
        for (int i = 0; i < I; i++) { //somátorio de para todo i em pilhas
            quantP += (((Teor[i][j] - TeorMax[j])* x[i]) - dp[j]);
        }     
        // IloRange parametros: ambiente, valor min, expressão, valor maximo
        IloRange restQuantP(env, -IloInfinity, quantP, 0);

        stringstream rest;
        rest << "QuantidadeL[" << NomeParametro[j] << "]";
        restQuantP.setName(rest.str().c_str());

        modelo.add(restQuantP);
    }

    // Restrições: qt do parametro seja inferior a qtd minima permitida em d
    for (int j = 0; j < J; j++) { //para todo j em parametros
        //-dn [j] <= quantP <= d[j]
        IloExpr quantP(env);
        
        for (int i = 0; i < I; i++) { //somátorio de para todo i em pilhas
            quantP += (((Teor[i][j] - TeorMin[j])* x[i]) + d[j]);
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
    cplex.exportModel("minerios_pl_relax.lp");
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