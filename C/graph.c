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

/* Function that inserts an edge in the graph.
 * Asymptotic complexity is O(1).
 *
 * G - Graph in which to insert an edge
 * u - Vertex of the edge
 * v - Vertex of the edge
 */
void insertEdgeG(Graph G, int u, int v)
{
	G->adjacencies[u-1] = insertL(G->adjacencies[u-1], v);
	G->adjacencies[v-1] = insertL(G->adjacencies[v-1], u);
	G->E++;
}

/* Auxiliary function of doTarjanSearchG() that visits a vertex during a DFS.
 * Returns the time count of each step of the algorithom.
 *
 * G      - Graph to apply a dfs
 * u      - Vertex to visit
 * args   - State variables of the DFS
 * count  - Time count of the algorithm
 * output - Points to the struct that will store the above graph info
 */
static int tarjanVisitG(Graph G, int u, void* args, int count, Audit output)
{
	Link v;

	dfsState_t* state = (dfsState_t*) args; /* Pointer to the DFS state */

	state->color[u-1] = GRAY;  /* Mark vertex u has visited */
	state->d[u-1] = count;	   /* Mark discovery time of vertex u */
	state->low[u-1] = count++; /* Mark initial low value of vertex u */

	int numChildren = 0; /* Will store the children of u in the DFS tree */

	for (v = G->adjacencies[u-1]; v; v = v->next)
	{
		if (state->color[v->id-1] == WHITE)
		{
			numChildren++;
			state->root[v->id-1] = 0;
			output->netID = MAX(output->netID, v->id);
			count = tarjanVisitG(G, v->id, (void*) state, count, output);

			/* Peeks at the finalized (Black) child vertex and retrieves its low if its smaller */
			state->low[u-1] = MIN(state->low[u-1], state->low[v->id-1]);

			/* If u is not the root of a DFS tree and there isnt a back edge to an ancestor, its a cut vertex */
			if (!state->root[u-1] && state->low[v->id-1] >= state->d[u-1] && !output->cutV[u-1])
			{
				output->numCutV++;
				output->cutV[u-1] = 1;
			}
		}

		/* If u has a path back */
		if (state->color[v->id-1] == GRAY)
		{
			/* Peeks at the ancestor vertex and retrieves its discovery if its smaller */
			state->low[u-1] = MIN(state->low[u-1], state->d[v->id-1]);
		}
	}

	/* If u is the root of a DFS tree and has more than two child vertices, its a cut vertex */
	if (state->root[u-1] && numChildren > 1)
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
 * Cut vertices can be detected in the root or when low[child] >= d[u].
 * Asymptotic complexity is O(V + E).
 *
 * G      - Graph to apply a dfs
 * output - Points to the struct that will store the above graph info
 */
void doTarjanSearchG(Graph G, Audit output)
{
	char* color = (char*) malloc(sizeof(char) * G->V); /* Vertex visit states */
	int* d = (int*) malloc(sizeof(int) * G->V);        /* Discovery times */
	char* root = (char*) malloc(sizeof(char) * G->V);  /* Indicates if a vertex is a root (0 if it is) */
	int* low = (int*) malloc(sizeof(int) * G->V);      /* Lowest d[u] found by a vertex during DFS */
	
	dfsState_t state = {color, d, root, low}; /* DFS state variables declaration */

	int u;		   /* Vertex id */
	int count = 1; /* Time count of the algorithm */

	for (u = 1; u <= G->V; u++)
	{
		color[u-1] = WHITE;
		d[u-1] = NIL;
		root[u-1] = 1;
		low[u-1] = NIL;

		output->cutV[u-1] = 0;
		output->subNetIDs[u-1] = NIL;
	}

	for (u = 1; u <= G->V; u++)
	{
		if (color[u-1] == WHITE)
		{
			output->numSubNets++;
			output->netID = u;

			count = tarjanVisitG(G, u, (void*) &state, count, output);

			/* Before next sub graph search begins, puts biggest vertex found in subNetIDs */
			output->subNetIDs[output->netID-1] = output->netID;
		}
	}

	free(color);
	free(d);
	free(root);
	free(low);
}

/* Auxiliary function of doDFS_G() that visits a vertex during a DFS.
 * Returns the number of vertices in the sub-graph being analised.
 *
 * G            - Graph to apply a dfs
 * u            - Vertex to visit
 * color        - Vertex visit states
 * numVertices  - Current number of vertices in sub-graph
 * output       - Points to the struct that will store the above graph info
 */
static int dfsVisitG(Graph G, int u, char* color, int numVertices, Audit output)
{
	Link v;

	color[u-1] = GRAY;

	for (v = G->adjacencies[u-1]; v; v = v->next)
	{
		if (color[v->id-1] == WHITE && !output->cutV[v->id-1])
		{
			numVertices++;
			numVertices = dfsVisitG(G, v->id, color, numVertices, output);
		}
	}

	color[u-1] = BLACK;
	return numVertices;
}

/* Function that applies a depth-first search to the given graph to find:
 * - The biggest sub-graph after removal of all the cut vertices of the graph
 * - The number of vertices of said sub-graph
 *
 * This DFS is based on the normal DFS algorithom, but much simpler.
 * To get the info above, we only need to mantain the state of vertices. No find times needed.
 * Asymptotic complexity is O(V + E).
 *
 * G      - Graph to apply a dfs
 * output - Points to the struct that will store the above graph info
 */
void doDFS_G(Graph G, Audit output)
{
	char* color = (char*) malloc(sizeof(char) * G->V); /* Vertex visit states */
	int numVertices = 1; /* Will store the number of vertices in a sub-graph */

	int u;

	for (u = 1; u <= G->V; u++)
	{
		color[u-1] = WHITE;
	}

	for (u = 1; u <= G->V; u++)
	{
		if (color[u-1] == WHITE && !output->cutV[u-1])
		{
			numVertices = dfsVisitG(G, u, color, numVertices, output);
			output->maxNetSize = MAX(output->maxNetSize, numVertices);
			numVertices = 1;
		}
	}

	free(color);
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
