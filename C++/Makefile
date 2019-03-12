# IST LEIC-T Analise e Sintese de Algoritmos 18/19
# Project 1 - Makefile
#
# Authors:
# Manuel Mascarenhas - 90751
# Miguel Levezinho   - 90756

proj1: proj1.o Network.o
	g++ -o proj1 proj1.o Network.o

proj1.o: proj1.cpp Network.h
	g++ -O3 -c proj1.cpp -o proj1.o

Network.o: Network.cpp Network.h
	g++ -O3 -c Network.cpp -o Network.o

clean:
	rm -f proj1 proj1.o Network.o
