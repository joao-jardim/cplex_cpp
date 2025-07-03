\ENCODING=ISO-8859-1
\Problem name: IloCplex

Minimize
 obj1: 0 dp(Pilha12)#21 + 0 dp(Pilha13)#22
Subject To
 QuantidadeP(Pilha12)#0: 1320 x(nome)#0 + 15 dnm(Pilha12)#15
                         - 15 dpm(Pilha12)#17  = 0
 QuantidadeP(Pilha13)#1: 1240 x(nome)#0 + 15 dnm(Pilha13)#16
                         - 15 dpm(Pilha13)#18  = 0
 QuantidadeU(Pilha12)#2: 1320 x(nome)#0 + 15 dn(Pilha12)#19 >= 0
 QuantidadeU(Pilha13)#3: 1240 x(nome)#0 + 15 dn(Pilha13)#20 >= 0
 c5#4:                   x(nome)#0 <= 0
 c6#5:                   x(toneladas)#1 <= 0
 c7#6:                   x(da)#2 <= 0
 c8#7:                   x(pilha)#3 <= 0
 c9#8:                   x(Pilha1)#4 <= 1500
 c10#9:                  x(Pilha2)#5 <= 2000
 c11#10:                 x(Pilha3)#6 <= 1700
 c12#11:                 x(Pilha4)#7 <= 1450
 c13#12:                 x(Pilha5)#8 <= 1250
 c14#13:                 x(Pilha6)#9 <= 1890
 c15#14:                 x(Pilha7)#10 <= 1640
 c16#15:                 x(Pilha8)#11 <= 1124
 c17#16:                 x(Pilha9)#12 <= 1900
 c18#17:                 x(Pilha10)#13 <= 990
 c19#18:                 x(Pilha11)#14 <= 1540
 c20#19:                 x(nome)#0 + x(toneladas)#1 + x(da)#2 + x(pilha)#3
                         + x(Pilha1)#4 + x(Pilha2)#5 + x(Pilha3)#6
                         + x(Pilha4)#7 + x(Pilha5)#8 + x(Pilha6)#9
                         + x(Pilha7)#10 + x(Pilha8)#11 + x(Pilha9)#12
                         + x(Pilha10)#13 + x(Pilha11)#14
                          = 7.21668709127281e-43
 c21#20:                 x(nome)#0 >= 0
 c22#21:                 x(toneladas)#1 >= 0
 c23#22:                 x(da)#2 >= 0
 c24#23:                 x(pilha)#3 >= 0
 c25#24:                 x(Pilha1)#4 >= 0
 c26#25:                 x(Pilha2)#5 >= 0
 c27#26:                 x(Pilha3)#6 >= 0
 c28#27:                 x(Pilha4)#7 >= 0
 c29#28:                 x(Pilha5)#8 >= 0
 c30#29:                 x(Pilha6)#9 >= 0
 c31#30:                 x(Pilha7)#10 >= 0
 c32#31:                 x(Pilha8)#11 >= 0
 c33#32:                 x(Pilha9)#12 >= 0
 c34#33:                 x(Pilha10)#13 >= 0
 c35#34:                 x(Pilha11)#14 >= 0
End
