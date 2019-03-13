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

#define MIN(a,b) (a<b?a:b)

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

	G->adjacencies[u-1] = insertL(G->adjacencies[u-1], v);
	G->adjacencies[v-1] = insertL(G->adjacencies[v-1], u);
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

	removeL(G->adjacencies[u-1], v);
	removeL(G->adjacencies[v-1], u);
	G->E--;
}

static int dfsVisitG(Graph G, int u, void* args, int count)
{
	Link v;
  int numChildren=0;
  dfsState_t* state = (dfsState_t*) args;
	state->color[u-1] = GRAY;
	state->d[u-1] = count++;
  state->low[u-1] = state->d[u-1]; //low init

	for (v = G->adjacencies[u-1]; v; v = v->next)
	{
		if (state->color[v->id-1] == WHITE)
		{
      numChildren++;
			state->p[v->id-1] = u;
			count = dfsVisitG(G, v->id, (void*) state, count);
      state->low[u-1] = MIN(state->low[u-1],state->low[v->id-1]); //low child pull
		}

		if (state->color[v->id-1] == GRAY)
		{
     state->low[u-1] = MIN(state->low[u-1],state->low[v->id-1]); //low ancestor/parent pull
		}
	}

  if(state->p[u-1]==-1 && numChildren > 1){//root with 2 children
    //Its a cut vertex TODO struct cut vertex
  }

	state->color[u-1] = BLACK;
	state->f[u-1] = count++;
	return count;
}

/* Function that applies a depth-first search to the given graph.
 * Asymptotic complexity is O(V + E).
 *
 * G - Graph to apply a dfs
 */
dfsState_t dfsG(Graph G)
{
	int color[G->V]; /* Vertex visit states */
	int d[G->V];     /* Discovery times */
	int f[G->V];     /* Finish times */
	int p[G->V];     /* Precedents */
  int low[G->V];   /* Low*/

  dfsState_t state = {color, d, f, p, low};

	int u;		   /* Vertex id */
	int count = 1;   /* Time count of the algorithm */

	for (u = 1; u <= G->V; u++)
	{
		color[u-1] = WHITE;
		p[u-1] = -1;
	}
	for (u = 1; u <= G->V; u++)
	{
		if (color[u-1] == WHITE)
		{
			count = dfsVisitG(G, u, (void*) &state, count);
		}
    //TODO contador numero de subredes
	}

  for (u=1;u<=G->V;u++){
    if(p[u-1] != -1 && (low[p[u-1]-1] < low[u-1])){
      //Its a cut vertex TODO struct cut vertex
    }
  }
	/* ********** DFS DEBUG ********** */
	printf("\n***** DFS State *****\n");
	for (u = 1; u <= G->V; u++)
	{
		printf("%.2d: ", u);
		printf("color=%d d[%.2d]=%-2d f[%.2d]=%-2d p[%.2d]=%-2d", color[u-1], u-1, d[u-1], u-1, f[u-1], u-1, p[u-1]);
		printf("\n");
	}
	/* ********** DFS DEBUG ********** */
  	return state;
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
