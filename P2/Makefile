# ================================================
# IST LEIC-T Analise e Sintese de Algoritmos 18/19
# Project 2 - Makefile
#
# Authors:
# Manuel Mascarenhas - 90751
# Miguel Levezinho   - 90756
# ================================================

proj2: proj2.o graph.o list.o queue.o
	gcc -g -o proj2 proj2.o graph.o list.o queue.o

proj2.o: proj2.c graph.h edge.h
	gcc -g -O3 -ansi -Wall -c proj2.c -o proj2.o

graph.o: graph.c graph.h list.h queue.h edge.h
	gcc -g -O3 -ansi -Wall -c graph.c -o graph.o

queue.o: queue.c queue.h
	gcc -g -O3 -ansi -Wall -c queue.c -o queue.o

list.o: list.c list.h edge.h
	gcc -g -O3 -ansi -Wall -c list.c -o list.o

clean:
	rm -f proj2 proj2.o graph.o list.o queue.o
