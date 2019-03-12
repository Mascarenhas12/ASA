/* IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - graph.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "list.h"

enum dfs_colors {
    WHITE = 0,
    GRAY  = 1,
    BLACK = 2,
};

/* Function that creates and initializes a graph, given the number of vertices.
 * The implementation uses a list of adjacencies and the graph stores integers.
 * Asymptotic complexity is O(V).
 * Returns a pointer to the graph structure.
 *
 * V - Number of vertices in the graph
 */
Graph initG(int V)
{
	int i;

	Graph new = (Graph) malloc(sizeof(struct graph));

	new->V = V;
	new->E = 0;
	new->adjacencies = (Link*) malloc(sizeof(Link) * V);

	for (i = 0; i < V; i++)
	{
		new->adjacencies[i] = NULL;
	}

	return new;
}

/* Function that creates an edge for the graph.
 * Asymptotic complexity is O(1).
 *
 * u - Start vertice of the edge
 * v - End vertice of the edge
 */
Edge createEdgeG(int u, int v)
{
	Edge e = {u, v};
	return e;
}

/* Function that inserts an edge in the graph.
 * Asymptotic complexity is O(1).
 *
 * G - Graph in which to insert an edge
 * e - Edge to be inserted
 */
void insertEdgeG(Graph G, Edge e)
{
	int u = e.u;
	int v = e.v;

	G->adjacencies[u] = insertL(G->adjacencies[u], v);
	G->adjacencies[v] = insertL(G->adjacencies[v], u);
	G->E++;
}

/* Function that removes an edge from the graph.
 * Asymptotic complexity is O(V).
 *
 * G - Graph from which to remove an edge
 * e - Edge to be removed
 */
void removeEdgeG(Graph G, Edge e)
{
	int u = e.u;
	int v = e.v;

	removeL(G->adjacencies[u], v);
	removeL(G->adjacencies[v], u);
	G->E--;
}

static void dfsVisitG(Graph G, int u,void* arg, int count)
{
	Link v;
  DFSarg_t* temp = (DFSarg_t*)arg;
	temp->color[u] = GRAY;
	temp->d[u] = count++;
  puts("ffnuoanfoansdoaw");
  int i = 0;
	for (v = G->adjacencies[u]; v!= NULL || i > 20; i++, v = G->adjacencies[u]->next)
	{
    printf("%d\n", G->adjacencies[u]->id);
    //printf("%d\n", G->adjacencies[u]->next->id);
		if (temp->color[v->id] == WHITE)
		{
			temp->p[v->id] = u;
			dfsVisitG(G, v->id, (void*)temp, count);
		}

		if (temp->color[v->id] == GRAY && temp->p[u] != v->id)
		{
			/* Its a B-Edge */
		}
	}

	temp->color[u] = BLACK;
	temp->f[u] = count++;
}

/* Function that applies a depth-first search to the given graph.
 * Asymptotic complexity is O(V + E).
 *
 * G - Graph to apply a dfs
 */
DFSarg_t depthFirstSearchG(Graph G)
{
	int color[G->V+1]; /* Vertex visit states */
	int d[G->V+1];     /* Discovery times */
	int f[G->V+1];     /* Finish times */
	int p[G->V+1];     /* Precedents */
  DFSarg_t temp= {color,d,f,p};
  puts("fuck scanf");
	int u;
	int count = 1; /* Time count of the algorithm */

	for (u = 1; u < G->V+1; u++)
	{
		color[u] = WHITE;
		p[u] = -1;
	}
	for (u = 1; u < G->V+1; u++)
	{
    puts("cnacnancnacn");
		if (color[u] == WHITE)
		{
			dfsVisitG(G, u,(void*)&temp, count);
		}
	}

  return temp;
}

/* Function that prints the adjacency list representing the given graph.
 *
 * G - Graph to print
 */
void printG(Graph G)
{
	int i;

	for (i = 0; i < G->V; i++)
	{
		printf("%d: ", i + 1);
		printL(G->adjacencies[i]);
	}
	printf("\n");
}

/* Function that frees a graph from memory, given a pointer to it.
 * Asymptotic complexity is O(E).
 *
 * G - Pointer to the graph to free from memory
 */
void freeG(Graph G)
{
	int i;

	for (i = 0; i < G->V; i++)
	{
		freeL(G->adjacencies[i]);
	}

	free(G->adjacencies);
	free(G);
}
