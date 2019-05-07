#!/bin/bash

# ================================================
# IST LEIC-T Analise e Sintese de Algoritmos 18/19
# Project 2 - doTests.sh
# 
# Authors:
# Manuel Mascarenhas - 90751
# Miguel Levezinho   - 90756
# ================================================

# Executable should be named proj1
# Use chmod +x doTests.sh if permission is denied

i=0

for test in tests/*.in; do
	./proj2 < $test > ${test%.in}.out.mine

	diff -cB ${test%.in}.out ${test%.in}.out.mine > ${test%.in}.out.diff
	if [ -s ${test%.in}.out.diff ]; then
		echo " * FAILED ${test##*/}"
		i=$[i+1]
	else
		rm -f ${test%.in}.out.diff ${test%.in}.out.mine
	fi
done

if [[ $i == 0 ]]; then
	echo "Finished."
else
	echo "Finished with $i failure(s)."
fi
