Formato ficheiro de input.txt

Primeira linha representa a dimensão da matriz (N*M)

5 5

Segundo conjunto de valores representa o orçamento em t
200

Segundo conjunto de valores representa os custos de manutenção para uma intervenção (cij):

192 160 118 221 165 -> Substituicao
58 48 35 66 50 -> Manutenção tipo 1
38 32 24 44 33 -> Manutenção tipo 2
19 16 12 22 17 -> Manutenção tipo 3
0 0 0 442 0 -> Não fazer nada. O valor será diferente de 0 caso não fazer nada leve à falha

Terceiro conjunto de valores representa os custos de manutenção para uma intervenção ponderado com o outcome do RUL (cij):

289 558 819 273 543 -> Substituicao
412 510 650 100500 530 -> Manutenção tipo 1
414 509 644 100500 529 -> Manutenção tipo 2
408 509 652 100500 531 -> Manutenção tipo 3
419 507 632 273 527 -> Não fazer nada

Quarto conjunto de valores representa a procura de cada ativo:
1 1 1 1 1

Quinto conjunto de valores representa a capacidade de cada ação de manutenção:
2
0
0
0
5

########################################################################################################################################################################
Original input file:
5 5

200

192 160 118 221 165
58 48 35 66 50
38 32 24 44 33
19 16 12 22 17
0 0 0 442 0

289 558 819 273 543
412 510 650 100500 530
414 509 644 100500 529
408 509 652 100500 531
419 507 632 273 527

1 1 1 1 1

2
0
0
0
5
