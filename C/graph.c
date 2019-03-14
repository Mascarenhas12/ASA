/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - graph.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "list.h"

#define NIL -1
#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

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

/* Auxiliary function of doAuditG() that visits a vertex during a DFS.
 * Returns the time count of each step of the algorithom.
 *
 * G      - Graph to apply a dfs
 * u      - Vertex to visit
 * args   - State variables of the DFS
 * count  - Time count of the algorithm
 * output - Points to the struct that will store the asked audit info
 */
static int dfsVisitG(Graph G, int u, void* args, int count, Audit output)
{
	Link t;
	int v;

	dfsState_t* state = (dfsState_t*) args; /* Pointer to the DFS state */

	state->color[u-1] = GRAY;  /* Mark vertex u has visited */
	state->d[u-1] = count;	   /* Mark discovery time of vertex u */
	state->low[u-1] = count++; /* Mark initial low value of vertex u */

	int numChildren = 0; /* Will store the children of u in the DFS tree */

	for (t = G->adjacencies[u-1], v = t->id; t; t = t->next, v = t->id)
	{
		if (state->color[v-1] == WHITE)
		{
			numChildren++;
			state->p[v-1] = u;
			output->biggestV = MAX(output->biggestV,v);
			count = dfsVisitG(G, v, (void*) state, count, output);

			// Peeks at the finalized (Black) child vertex and retrieves its low if its smaller
			state->low[u-1] = MIN(state->low[u-1], state->low[v-1]);
		}

		if (state->color[v-1] == GRAY)
		{
			// Peeks at the parent/ancestor vertex and retrieves its low if its smaller
			state->low[u-1] = MIN(state->low[u-1], state->low[v-1]);
		}
	}

	// If u is the root of a DFS tree and has more than two child vertices, its a cut vertice
	if (state->p[u-1] == NIL && numChildren > 1)
	{
		output->numCutV++;
		output->cutV[u-1] = 1;
	}
	state->color[u-1] = BLACK;
	return count;
}

/* Function that applies a depth-first search to the given graph to find:
 * - The number of sub-graphs
 * - The ID of each sub-graph
 * - The number of cut vertices
 * - The IDs of the cut vertices
 *
 * This DFS is based on the Torjan algorithom, that finds strongly-connected components,
 * but applied to a undirected graph.
 * The SCCs will be of the resultant DFS forest, and some cut vertices can be detected on
 * the transitions between SCCs.
 * Asymptotic complexity is O(V + E).
 *
 * G      - Graph to apply a dfs
 * output - Points to the struct that will store the above graph info
 */
void doAuditG(Graph G, Audit output)
{
	int color[G->V]; /* Vertex visit states */
	int d[G->V];     /* Discovery times */
	int p[G->V];     /* Precedents */
	int low[G->V];   /* Lowest d[u] within a SCC of the DFS forest */

	dfsState_t state = {color, d, p, low}; /* DFS state variables declaration */

	int u;		   /* Vertex id */
	int count = 1; /* Time count of the algorithm */

	for (u = 1; u <= G->V; u++)
	{
		color[u-1] = WHITE;
		p[u-1] = NIL;
		output->cutV[u-1] = 0;
		output->subBigV[u-1] = NIL;
	}

	for (u = 1; u <= G->V; u++)
	{
		if (color[u-1] == WHITE)
		{
			output->numSubNets++;
			output->biggestV = 0;
			count = dfsVisitG(G, u, (void*) &state, count, output);

			output->subBigV[output->biggestV-1] = output->biggestV;
			//before next sub net search begins puts biggest vertex in subBigV
		}
	}

	for (u = 1; u <= G->V; u++)
	{
		// If u is not the root of a DFS tree and has a greater low than the parents low, its a cut vertice
		if (p[u-1] != NIL && (low[p[u-1]-1] < low[u-1]) && !output->cutV[p[u-1]-1])
		{
			output->numCutV++;
			output->cutV[p[u-1]-1] = 1;
		}
	}
	/* ********** DFS DEBUG ********** */
	printf("*************** DFS State ***************\n");
	for (u = 1; u <= G->V; u++)
	{
		printf("%.2d: ", u);
		printf("color=%d d[%.2d]=%-2d low[%.2d]=%-2d p[%.2d]=%-2d cutV[%.2d]=%-2d", color[u-1], u-1, d[u-1], u-1, low[u-1], u-1, p[u-1], u-1, output->cutV[u-1]);
		printf("\n");
	}
	/* ********** DFS DEBUG ********** */
}

static int dfsVisit2(Graph G,int u,int* color, int count, Audit output)
{
	Link t;
	int v;

	color[u-1] = GRAY;

	for (t = G->adjacencies[u-1], v = t->id; t; t = t->next, v = t->id)
	{
		if (color[v-1] == WHITE && !output->cutV[v-1])
		{
			count++;
			count = dfsVisit2(G, v, color, count, output);
		}
	}

	color[u-1] = BLACK;
	return count;
}

void biggestNetSizeAudit(Graph G, Audit output)
{
	int color[G->V]; /* Vertex visit states */
	int count = 1;
	int u;

	for (u = 1; u <= G->V; u++)
	{
		color[u-1] = WHITE;
	}

	for (u = 1; u <= G->V; u++)
	{
		if (color[u-1] == WHITE && !output->cutV[u-1])
		{
			count = dfsVisit2(G, u, color, count, output);
			output->numBNet = MAX(output->numBNet, count);
		}
	}
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
