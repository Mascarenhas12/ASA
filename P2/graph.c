/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - graph.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edge.h"
#include "graph.h"
#include "list.h"
#include "queue.h"

#define MIN(a, b) (a < b ? a : b)

enum bfs_colors {
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
	new->adj = (Link*) malloc(sizeof(Link) * V);

	for (i = 0; i < V; i++)
	{
		new->adj[i] = NULL;
	}

	return new;

	/*
	int i, j;

	Graph new = (Graph) malloc(sizeof(struct graph));

	new->V = V;
	new->E = 0;
	new->adj = (int**) malloc(sizeof(int*) * V);

	for (i = 0; i < V; i++)
	{
		new->adj[i] = (int*) malloc(sizeof(int) * V);
		for (j = 0; j < V; j++)
		{
			new->adj[i][j] = 0;
		}
	}

	return new;
	*/
}

/*
 * O(1)
 */
static Edge createWeightedEdgeG(int u, int v, int c)
{
	Edge new = (Edge) malloc(sizeof(struct edge));

	new->u = u;
	new->v = v;
	new->cap = c;
	new->flow = 0;

	return new;
}

/* Function that inserts a weighted edge in a graph that represents a flux network.
 * Asymptotic complexity is O(1).
 *
 * G - Graph in which to insert an edge
 * u - Vertex of the edge
 * v - Vertex of the edge
 * c - Capacity of the edge
 */
void insertWeightedEdgeG(Graph G, int u, int v, int c)
{
	Edge e1 = createWeightedEdgeG(u, v, c);
	Edge e2 = createWeightedEdgeG(v, u, 0);

	G->adj[u] = insertL(G->adj[u], e1);
	G->adj[v] = insertL(G->adj[v], e2);

	linkOppositesL(G->adj[u], G->adj[v]);

	G->E+=2;

	/*
	e1->reverseIdx = sizeL(G->adj[v] - 1);
	e2->reverseIdx = sizeL(G->adj[u] - 1);
	*/

	/*
	G->adj[u][v] = c;
	G->E++;
	*/
}

/* Uses BFS, also initializes heights.
 * Time O(V+E)
 */
static void initializePreFLow(Graph G, Queue FIFO, PR_State_t* state, int s, int t)
{
	Link l;
	Edge opEdge;
	int u, v;

	char* color = (char*) malloc(sizeof(char) * G->V); /* Vertex visit states */

	Queue bfsQ = initQ(G->V);

	for (u = 0; u < G->V; u++)
	{
		color[u] = WHITE;
		state->h[u] = 0;
		state->e[u] = 0;
		state->active[u] = 0;
	}

	state->h[s] = G->V;

	color[t] = GRAY;
	putQ(bfsQ, t);

	while (!isEmptyQ(bfsQ))
	{
		u = getQ(bfsQ);

		for (l = G->adj[u]; l; l = l->next)
		{
			v = l->edge->v;

			if (v == s)
			{
				opEdge = l->opposite->edge;

				opEdge->flow = opEdge->cap;
				l->edge->flow = opEdge->flow * -1;
				state->e[u] = opEdge->flow;
				state->e[v] -= opEdge->flow;
				state->active[u] = 1;
				putQ(FIFO, u);
			}
			else if (color[v] == WHITE)
			{
				color[v] = GRAY;
				state->h[v] = state->h[u] + 1;
				putQ(bfsQ, v);
			}
		}

		/*
		for (v = 0; v < G->V; v++)
		{
			if (u == s && G->adj[u][v] > 0)
			{
				G->adj[v][u] = G->adj[u][v]; // Max flow from s to v means reverse edge in residual net
				state->e[v] = G->adj[u][v];  // Update excess of v
				state->e[u] -= G->adj[u][v]; // Update excess of s
				G->adj[u][v] = 0;			 // Edge is full, so it stops existing in residual net
				putQ(FIFO, v);				 // Add vertex to push-relabel FIFO
			}
			else if (G->adj[v][u] > 0 && color[v] == WHITE)
			{
				color[v] = GRAY;
				state->h[v] = state->h[u] + 1;
				putQ(bfsQ, v);
			}
		}*/

		color[u] = BLACK;
	}

	free(color);
	freeQ(bfsQ);
}

/*
 * O(V)
 */
static void relabel(Graph G, PR_State_t* state, int u)
{
	Link t;
	int min = G->V * 2; /* Height upper bound */

	printf("relabel: h(%d)=%d\n", u, state->h[u]);

	for (t = G->adj[u]; t; t = t->next)
	{
		if (t->edge->cap - t->edge->flow > 0)
		{
			min = MIN(min, state->h[t->edge->v]);
		}
	}
	state->h[u] = 1 + min;

	printf("relabel: h(%d)=%d\n", u, state->h[u]);

	/*
	int v;
	int min = G->V * 2; // Height upper bound

	for (v = 0; v < G->V; v++)
	{
		if (G->adj[u][v] > 0)
		{
			min = MIN(min, state->h[v]);
		}
	}
	state->h[u] = 1 + min;
	*/
}

/*
 * O(1)
 */
static void push(PR_State_t* state, Link t, int u, int v)
{
	Edge edge = t->edge;

	printf("push: u:%d v:%d e(%d)=%d e(%d)=%d cf(u,v)=%d\n", u, v, u, state->e[u], v, state->e[v], edge->cap - edge->flow);

	int f = MIN(state->e[u], edge->cap - edge->flow);
	edge->flow += f;
	t->opposite->edge->flow = edge->flow * -1;
	state->e[u] -= f;
	state->e[v] += f;

	printf("push: e(%d)=%d e(%d)=%d\n", u, state->e[u], v, state->e[v]);

	/*
	int f = MIN(state->e[u], G->adj[u][v]);
	G->adj[v][u] += f;
	G->adj[u][v] -= f;
	state->e[u] -= f;
	state->e[v] += f;
	*/
}

static void discharge(Graph G, Queue FIFO, PR_State_t* state, int u)
{
	Link t = G->adj[u];
	Edge edge;

	while (state->e[u] > 0)
	{
		if (t != NULL)
		{
			edge = t->edge;
		}
		if (t == NULL)
		{
			relabel(G, state, u);
			putQ(FIFO, u);
			/* edge = G->adj[u]->edge; */
			break;
		}
		else if (edge->cap - edge->flow > 0 && state->h[u] == state->h[edge->v] + 1)
		{
			push(state, t, u, edge->v);
			if (!state->active[edge->v])
			{
				state->active[edge->v] = 1;
				putQ(FIFO, edge->v);
			}
		}
		else {
			t = t->next;
		}
		printQ(FIFO);
	}
}

void pushRelabelFIFO(Graph G, NetAudit output)
{
	int u;

	int* h = (int*) malloc(sizeof(int) * G->V); /* Heights of vertices */
	int* e = (int*) malloc(sizeof(int) * G->V); /* Excesses of vertices */
	char* active = (char*) malloc(sizeof(char) * G->V); /* Active vertices (that have excess) */

	PR_State_t state = {h, e, active}; /* Push-Relabel state variables declaration */

	Queue FIFO = initQ(G->V - 2);

	initializePreFLow(G, FIFO, (PR_State_t*) &state, 0, 1);

	/*printf("0:%d/%d: ->%d/%d-> %d:%d/%d\n", state.h[0], state.e[0], G->adj[0]->next->edge->flow, G->adj[0]->next->edge->cap, G->adj[0]->next->edge->v, state.h[3], state.e[3]);*/
	
	while (!isEmptyQ(FIFO))
	{
		u = getQ(FIFO);
		discharge(G, FIFO, (PR_State_t*) &state, u);
	}

	printG(G);

	printf("\nmax flux: %d\n\n", state.e[1]); /* Max flow */

	free(h);
	free(e);
	free(active);
}

void printG(Graph G)
{
	int i;

	for (i = 0; i < G->V; i++)
	{
		printf("%d: ", i);
		printL(G->adj[i]);
	}

	/*
	int i, j;

	for (i = 0; i < G->V; i++)
	{
		printf("%-3d", G->adj[i][0]);

		for (j = 1; j < G->V; j++)
		{
			printf(" ");
			printf("%-3d", G->adj[i][j]);
		}
		printf("\n");
	}
	*/
}

/* Function that frees a graph from memory, given a pointer to it.
 * Asymptotic complexity is O(V).
 *
 * G - Pointer to the graph to free from memory
 */
void freeG(Graph G)
{
	int i;

	for (i = 0; i < G->V; i++)
	{
		freeL(G->adj[i]);
	}

	free(G->adj);
	free(G);

	/*
	int i;

	for (i = 0; i < G->V; i++)
	{
		free(G->adj[i]);
	}

	free(G->adj);
	free(G);
	*/
}
