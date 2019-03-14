#!/bin/bash

# ================================================
# IST LEIC-T Analise e Sintese de Algoritmos 18/19
# Project 1 - doTests.sh
# 
# Authors:
# Manuel Mascarenhas - 90751
# Miguel Levezinho   - 90756
# ================================================

# Executable should be named proj1
# Use chmod +x doTests.sh if permission is denied

i=0

for x in tests/*.in; do
	./proj1 < $x > ${x%.in}.out.mine

	diff ${x%.in}.out ${x%.in}.out.mine > ${x%.in}.out.diff
	if [ -s ${x%.in}.out.diff ]; then
		echo " * FAILED ${x##*/}"
		i=$[i+1]
	else
		rm -f ${x%.in}.out.diff ${x%.in}.out.mine
	fi
done

if [[ $i == 0 ]]; then
	echo "Finished."
else
	echo "Finished with $i failure(s)."
fi
