\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 450 dn(Ferro)#3 + 400 dn(Silica)#4 + 180 dn(Aluminio)#5
       + 300 dp(Ferro)#6 + 240 dp(Silica)#7 + 90 dp(Aluminio)#8
Subject To
 QuantidadeL(Ferro)#0:    3 x(Pilha1)#0 - 10 x(Pilha2)#1 - 2 x(Pilha3)#2
                          - 3 dp(Ferro)#6 <= 0
 QuantidadeL(Silica)#1:   - 3 x(Pilha1)#0 - x(Pilha2)#1 - 4 x(Pilha3)#2
                          - 3 dp(Silica)#7 <= 0
 QuantidadeL(Aluminio)#2: 45.5 x(Pilha1)#0 + 5.5 x(Pilha2)#1 + 0.5 x(Pilha3)#2
                          - 3 dp(Aluminio)#8 <= 0
 QuantidadeU(Ferro)#3:    8 x(Pilha1)#0 - 5 x(Pilha2)#1 + 3 x(Pilha3)#2
                          + 3 dn(Ferro)#3 >= 0
 QuantidadeU(Silica)#4:   5 x(Pilha1)#0 + 7 x(Pilha2)#1 + 4 x(Pilha3)#2
                          + 3 dn(Silica)#4 >= 0
 QuantidadeU(Aluminio)#5: 50 x(Pilha1)#0 + 10 x(Pilha2)#1 + 5 x(Pilha3)#2
                          + 3 dn(Aluminio)#5 >= 0
 c7#6:                    x(Pilha1)#0 <= 1500
 c8#7:                    x(Pilha2)#1 <= 2000
 c9#8:                    x(Pilha3)#2 <= 1700
 c10#9:                   x(Pilha1)#0 + x(Pilha2)#1 + x(Pilha3)#2  = 2
 c11#10:                  x(Pilha1)#0 >= 0
 c12#11:                  x(Pilha2)#1 >= 0
 c13#12:                  x(Pilha3)#2 >= 0
End
