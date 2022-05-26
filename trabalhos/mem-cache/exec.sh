#!/bin/bash
for j in 2000
do
	echo "Testando Matrizes com $j tamanho (original)"
	for i in $(seq 1 10)
	do
		./multmatriz.x $j $j $j $j o >> ./resultados/resultadosO$j.txt
	done
	echo "Testando Matrizes com $j tamanho (transposta)"

	for i in $(seq 1 10)
	do
		./multmatriz.x $j $j $j $j t >> ./resultados/resultadosT$j.txt
	done
done
