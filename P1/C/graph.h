/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - graph.h
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"

/* Struct that represents a graph */
struct graph
{
	int V;
	int E;
	Link *adjacencies;
};

/* Struct that represents the state of a DFS, storing its relevant variables */
struct dfsState {
	char* color;
	int* d;
	char* root;
	int* low;
};

/* Struct that represents an audit and stores its data */
struct audit {
	int numSubNets;
	int netID;
	int* subNetIDs;
	int numCutV;
	char* cutV;
	int maxNetSize;
};

/* Abstraction of the graph struct to a pointer type Graph */
typedef struct graph *Graph;

/* Abstraction of the dfsState struct to a type dfsState_t */
typedef struct dfsState dfsState_t;

/* Abstraction of the audit struct to a pointer type Audit */
typedef struct audit *Audit;

Graph initG(int V);

void insertEdgeG(Graph G, int u, int v);

void doDFS_G(Graph G, Audit output);

void doTarjanSearchG(Graph G, Audit output);

void freeG(Graph G);

#endif
