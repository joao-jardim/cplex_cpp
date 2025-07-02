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
    float* Custo; // Custo de uma tonelada de minerio da pilha
    float Qtd; // Quantidade em toneladas da mistura a ser formada

    char** NomeParametro; // por parametro
    float* TeorMin; // Porcentagem Minima admissível
    float* TeorMax; // Porcentagem Maxima admissível
    float** Teor; // Porcentagem de cada parametro em cada pilha



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
    Custo = new float[I];

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


    // Após a declaração dos tamanhos dos dados de entrada, realizando a leitura
    // Por Pilha
    for (int i = 0; i < I; i++) {
        fscanf(fp, "%s %f %f", NomePilha[i], &QtdMax[i], &Custo[i]);
    }

    // Por Parâmetro
    for (int j = 0; j < J; j++) {
        fscanf(fp, "%s", NomeParametro[j]);
        for (int i = 0; i < I; i++) {
            fscanf(fp, "%f", &Teor[i][j]); // Corrigido: Teor[i][j]
        }
        fscanf(fp, "%f %f", &TeorMin[j], &TeorMax[j]);
    }

    fscanf(fp, "%f", &Qtd);

// Impressão para Verificação dos dados
    printf("I: %d\n", I);
    printf("J: %d\n", J);
    printf("Pilhas:\nNome\tQtdMax\tCusto\n");
    for (int i = 0; i < I; i++) {
        printf("%s\t%.2f\t%.2f\n", NomePilha[i], QtdMax[i], Custo[i]);
    }
    printf("Parâmetros:\nNome\t");
    for (int i = 0; i < I; i++) printf("%s\t", NomePilha[i]);
    printf("TeorMin\tTeorMax\n");
    for (int j = 0; j < J; j++) {
        printf("%s\t", NomeParametro[j]);
        for (int i = 0; i < I; i++) {
            printf("%.2f\t", Teor[i][j]);
        }
        printf("%.2f\t%.2f\n", TeorMin[j], TeorMax[j]);
    }
    printf("Qtd: %.2f\n", Qtd);


    // DECLARANDO O AMBIENTE E O MODELO MATEMATICO
    IloEnv env;
	IloModel modelo(env);

    // DECLARAÇÃO DE VARIÁVEIS
    // Quantidade de minerio a ser retirado da pilha i ∈ Pilhas
    IloNumVarArray x(env, I, 0, IloInfinity, ILOFLOAT);
    for (int i = 0; i < I; i++) {
        // x[i].setUB(QtdMax[i]); // Limite superior: x_i <= QtdMax_i
        stringstream var;
        var << "x[" << NomePilha[i] << "]";
        x[i].setName(var.str().c_str());
        modelo.add(x[i]);
    }

    // DECLARAÇÃO DA FUNÇÃO OBJETIVO
    IloExpr fo(env);
    for (int i = 0; i < I; i++) { // para todo i pertencente a pilhas
        fo += Custo[i] * x[i];
    }
    modelo.add(IloMinimize(env, fo)); // Minimizar custo

    // Restrições: teores mínimo e máximo
    for (int j = 0; j < J; j++) { //para todo j em parametros
        IloExpr somaTeor(env);
        
        for (int i = 0; i < I; i++) { //somátorio de para todo i em pilhas
            somaTeor += Teor[i][j] * x[i];
        }

        IloRange restTeor(env, TeorMin[j] * Qtd, somaTeor, IloInfinity);
        IloRange restTeor2(env, -IloInfinity, somaTeor, TeorMax[j] * Qtd);
        stringstream rest;
        rest << "Teor[" << NomeParametro[j] << "]";
        restTeor.setName(rest.str().c_str());

        rest << "Teor[" << NomeParametro[j] << "]";
        restTeor2.setName(rest.str().c_str());
        modelo.add(restTeor);
        modelo.add(restTeor2);

    }
    
    // // Restrição: Quantidade a ser retomada é limitada
    for (int i = 0; i < I; i++) { //para todo i em pilhas
        IloExpr quant(env);
        IloRange restQuant(env, -IloInfinity, x[i], QtdMax[i]);
        modelo.add(restQuant);
    }

    // // CARREGANDO E RESOLVENDO O MODELO
    IloCplex cplex(modelo);
    cplex.exportModel("minerios_pl.lp");
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
    printf("Custo total: %.2f\n", cplex.getObjValue());
    printf("Solução:\n");
    for (int i = 0; i < I; i++) {
        double valor = cplex.getValue(x[i]);
        printf("%s: %.2f toneladas\n", NomePilha[i], valor);
    }

    return 0;
}