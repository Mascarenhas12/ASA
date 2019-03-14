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

/* Struct that represents a node of a list of items */
struct edge
{
	int u;
	int v;
};

/* Struct that represents a graph */
struct graph
{
	int V;
	int E;
	Link *adjacencies;
};

/* Struct that represents an audit and stores its data */
struct audit {
	int numSubNets;
	int netID;
	int* subNetIDs;
	int numCutV;
	int* cutV;
	int maxNetSize;
};

/* Struct that represents the state of a DFS, storing its relevant variables */
struct dfsState {
	int* color;
	int* d;
	int* p;
	int* low;
};

/* Abstraction of the edge struct to a type Edge */
typedef struct edge Edge;

/* Abstraction of the graph struct to a pointer type Graph */
typedef struct graph *Graph;

/* Abstraction of the audit struct to a pointer type Audit */
typedef struct audit *Audit;

/* Abstraction of the dfsState struct to a type dfsState_t */
typedef struct dfsState dfsState_t;

Graph initG(int V);

Edge createEdgeG(int u, int v);

void insertEdgeG(Graph G, Edge e);

void doDFS_G(Graph G, Audit output);

void doTarjanSearchG(Graph G, Audit output);

void printG(Graph G);

void freeG(Graph G);

#endif
