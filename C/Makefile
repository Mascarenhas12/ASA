# ================================================
# IST LEIC-T Analise e Sintese de Algoritmos 18/19
# Project 1 - Makefile
#
# Authors:
# Manuel Mascarenhas - 90751
# Miguel Levezinho   - 90756
# ================================================

proj1: proj1.o graph.o list.o
	gcc -g -O3 -o proj1 proj1.o graph.o list.o

proj1.o: proj1.c graph.h list.h
	gcc -g -O3 -c proj1.c -o proj1.o

graph.o: graph.c graph.h list.h
	gcc -g -O3 -c graph.c -o graph.o

list.o: list.c list.h
	gcc -g -O3 -c list.c -o list.o

clean:
	rm -f proj1 proj1.o graph.o list.o
