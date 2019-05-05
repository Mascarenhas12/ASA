/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - graph.h
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"

/* Struct that represents a flow network */
struct graph
{
	int V;
	int P;
	int S;
	int E;
	Link *adj;
	/*int **adj;*/
};

/* Struct that represents the state of the FIFO push-relabel algorithom,
 * storing its relevant variables */
struct PR_State
{
	int* h;
	int* e;
	char* active;
};

/* Abstraction of the graph struct to a pointer type Graph */
typedef struct graph *Graph;

/* Abstraction of the PR_State struct to a type PR_State_t */
typedef struct PR_State PR_State_t;

/* Struct that represents the information to retrive from the network */
struct audit
{
	int maxFlow;
	char* updateStations;
	Edge* updateConnections;
	int idx;
};

/* Abstraction of the audit struct to a pointer type NetAudit */
typedef struct audit *NetAudit;

Graph initG(int V,int P,int S);
void insertWeightedEdgeG(Graph G, int u, int v, int c);
void pushRelabelFIFO(Graph G, NetAudit output);
void printG(Graph G);
void freeG(Graph G);

#endif
