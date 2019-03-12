/* IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - graph.h
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
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

struct DFSarg{
	int* color;
	int* d;
	int* f;
	int* p;
};

typedef struct DFSarg DFSarg_t;

/* Struct that represents a node of a list of items */
struct graph
{
	int V;
	int E;
	Link *adjacencies;
};

/* Abstraction of the edge struct to a type Edge */
typedef struct edge Edge;

/* Abstraction of the graph struct to a type *Graph */
typedef struct graph *Graph;

Graph initG(int V);

Edge createEdgeG(int u, int v);

void insertEdgeG(Graph G, Edge e);

void removeEdgeG(Graph G, Edge e);

DFSarg_t depthFirstSearchG(Graph G);

void printG(Graph G);

void freeG(Graph G);

#endif
