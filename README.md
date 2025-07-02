# Exercício de Mistura de Minérios

Este repositório contém a implementação em C++ de um problema de otimização linear relacionado à mistura de minérios, utilizando a biblioteca CPLEX para resolver os modelos matemáticos. O objetivo é determinar a quantidade ótima de minério a ser retirada de diferentes pilhas para formar uma mistura que atenda a restrições específicas de custo e qualidade, minimizando o custo total.

## Descrição do Problema

O problema de mistura de minérios envolve a combinação de minérios de várias pilhas, cada uma com uma quantidade máxima disponível, um custo por tonelada e teores de diferentes parâmetros (como ferro, sílica e alumínio). A mistura final deve atender a:
- Uma quantidade total específica (em toneladas).
- Limites mínimo e máximo de teores para cada parâmetro, garantindo a qualidade do produto final.
- Restrições de disponibilidade de cada pilha.

O modelo é resolvido como um problema de programação linear (PL), onde a função objetivo busca minimizar o custo total, sujeito às restrições mencionadas. Este exercício explora diferentes variações do modelo, incluindo abordagens com relaxação de limites e metas de qualidade.

## Estrutura do Repositório

O repositório contém três arquivos de código C++ que implementam diferentes modelos para o problema dos minérios:

1. **`minerios_pl.cpp`**: Implementa o modelo básico de programação linear (PL) para a mistura de minérios, com a função objetivo de minimizar o custo total e restrições rígidas de quantidade e teores.
2. **`minerios_pl_relax.cpp`**: Extensão do modelo básico que inclui relaxação dos limites de teores, permitindo desvios controlados para lidar com casos inviáveis.
3. **`minerios_pl_meta.cpp`**: Implementa um modelo com metas de qualidade, onde os teores ideais são definidos como objetivos a serem aproximados, em vez de restrições rígidas.

Cada arquivo lê os dados de entrada a partir de um arquivo de texto (`minerios_pl.txt`) e gera um arquivo `.lp` que representa o modelo em formato legível. Os resultados, incluindo o status da solução, custo total e quantidades retiradas de cada pilha, são exibidos no console.

## Pré-requisitos

- **CPLEX**: Biblioteca de otimização da IBM (versão 12.6 ou superior recomendada). Certifique-se de configurar os caminhos de inclusão e linkagem corretamente.
- **Compilador C++**: Compatível com a biblioteca CPLEX (ex.: g++).
- **Sistema Operacional**: Testado em ambientes Linux/Unix (ajustes podem ser necessários para Windows).
