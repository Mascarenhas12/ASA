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
 * g - Graph in which to insert an edge
 * e - Edge to be inserted
 */
void insertEdgeG(Graph g, Edge e)
{
	int u = e.u;
	int v = e.v;

	g->adjacencies[u] = insertL(g->adjacencies[u], v);
	g->adjacencies[v] = insertL(g->adjacencies[v], u);
	g->E++;
}

/* Function that removes an edge from the graph.
 * Asymptotic complexity is O(V).
 *
 * g - Graph from which to remove an edge
 * e - Edge to be removed
 */
void removeEdgeG(Graph g, Edge e)
{
	int u = e.u;
	int v = e.v;

	removeL(g->adjacencies[u], v);
	removeL(g->adjacencies[v], u);
	g->E--;
}

static void dfsVisitG(Graph g, int u,void* arg, int count)
{
	Link v;
  DFSarg_t* temp = (DFSarg_t*)arg;
	temp->color[u] = GRAY;
	temp->d[u] = count++;
  puts("ffnuoanfoansdoaw");
  int i = 0;
	for (v = g->adjacencies[u]; v!= NULL || i > 20; i++, v = g->adjacencies[u]->next)
	{
    printf("%d\n", g->adjacencies[u]->id);
    //printf("%d\n", g->adjacencies[u]->next->id);
		if (temp->color[v->id] == WHITE)
		{
			temp->p[v->id] = u;
			dfsVisitG(g, v->id, (void*)temp, count);
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
 * g - Graph to apply a dfs
 */
DFSarg_t depthFirstSearchG(Graph g)
{
	int color[g->V+1]; /* Vertex visit states */
	int d[g->V+1];     /* Discovery times */
	int f[g->V+1];     /* Finish times */
	int p[g->V+1];     /* Precedents */
  DFSarg_t temp= {color,d,f,p};
  puts("fuck scanf");
	int u;
	int count = 1; /* Time count of the algorithm */

	for (u = 1; u < g->V+1; u++)
	{
		color[u] = WHITE;
		p[u] = -1;
	}
	for (u = 1; u < g->V+1; u++)
	{
    puts("cnacnancnacn");
		if (color[u] == WHITE)
		{
			dfsVisitG(g, u,(void*)&temp, count);
		}
	}

  return temp;
}

/* Function that frees a graph from memory, given a pointer to it.
 * Asymptotic complexity is O(E).
 *
 * g - Pointer to the graph to free from memory
 */
void freeG(Graph g)
{
	int i;

	for (i = 0; i < g->V; i++)
	{
		freeL(g->adjacencies[i]);
	}

	free(g->adjacencies);
	free(g);
}
