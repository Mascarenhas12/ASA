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
#define MAX(a, b) (a > b ? a : b)

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
Graph initG(int V, int P, int S)
{
	int i;

	Graph new = (Graph) malloc(sizeof(struct graph));

	new->V = V;
	new->P = P;
	new->S = S;
	new->E = 0;
	new->adj = (Link*) malloc(sizeof(Link) * V);

	for (i = 0; i < V; i++)
	{
		new->adj[i] = NULL;
	}

	return new;
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
	new->filled = 0;

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
}

static void initializePreFLow(Graph G, Queue FIFO, PR_State_t* state, int s, int t)
{
	int u, v;
	Link l;
	Edge edge;

	for (u = 0; u < G->V; u++)
	{
		state->h[u] = 0;
		state->e[u] = 0;
		state->active[u] = 0;
		state->gap[u] = 0;
		state->gap[2*u] = 0;
	}

	state->h[s] = G->V;
	state->gap[G->V] += 1;
	state->active[s] = 1;
	state->active[t] = 1;

	for (l = G->adj[s]; l; l = l->next)
	{
		edge = l->edge;
		v = l->edge->v;

		edge->flow = edge->cap;
		l->opEdge->flow = edge->flow * -1;
		state->e[v] = edge->flow;
		state->e[s] -= edge->flow;
		state->active[v] = 1;
		putQ(FIFO, v);
	}
}

/*
 * O(V) -> pode melhorar (muito) para O(degree(V)) se se usar array de listas
 */
static void gapRelabel(Graph G, Queue FIFO, PR_State_t* state, int h)
{
	int u;

	for (u = 2; u < G->V; u++)
	{
		if (state->h[u] >= h)
		{
			state->gap[state->h[u]] -= 1;
			state->h[u] = MAX(G->V, state->h[u]);
			state->gap[state->h[u]] += 1;
		}
	}
}

/*
 * O(V)
 */
static void relabel(Graph G, PR_State_t* state, int u)
{
	Link t;
	int min = G->V * 2;  /* Height upper bound */

	for (t = G->adj[u]; t; t = t->next)
	{
		if (t->edge->cap - t->edge->flow > 0)
		{
			min = MIN(min, state->h[t->edge->v]);
		}
	}
	state->gap[state->h[u]] -= 1;
	state->h[u] = 1 + min;
	state->gap[state->h[u]] += 1;
}

/*
 * O(1)
 */
static void push(PR_State_t* state, Link t, int u, int v)
{
	Edge edge = t->edge;

	int f = MIN(state->e[u], edge->cap - edge->flow);
	edge->flow += f;
	t->opEdge->flow = edge->flow * -1;
	state->e[u] -= f;
	state->e[v] += f;
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
			if (state->gap[state->h[u]] == 1)
			{
				gapRelabel(G, FIFO, state, state->h[u]);
			}
			else
			{
				relabel(G, state, u);
				t = G->adj[u];
			}
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
		else
		{
			t = t->next;
		}
	}
	if (state->e[u] == 0)
	{
		state->active[u] = 0;
	}
}

static void getMinCut(Graph G, PR_State_t* state, NetAudit output, int s)
{
	int u;
	int h;
	Link l;

	for (h = 2; h < G->V; h++)
	{
		if (state->gap[h] == 0)
			break;
	}

	for (u = 0; u < G->V; u++)
	{
		if (state->h[u] < h)
			continue;

		for (l = G->adj[u]; l; l = l->next)
		{
			if (state->h[l->edge->v] > h)
				continue;

			if (u >= G->V - G->S)
			{
				output->minCutS[l->edge->v] = 1;
			}
			else if (u == 1 || u > G->P+1)
			{
				output->minCutE[output->idx++] = l->opEdge;
			}
		}
	}
}

void pushRelabelFIFO(Graph G, NetAudit output)
{
	int u;

	int* h = (int*) malloc(sizeof(int) * G->V); /* Heights of vertices */
	int* e = (int*) malloc(sizeof(int) * G->V); /* Excesses of vertices */
	char* active = (char*) malloc(sizeof(char) * G->V); /* Active vertices (that have excess) */
	int* gap = (int*) malloc(sizeof(int) * (2*G->V)); /* Number of nodes gap[i] with height i */

	PR_State_t state = {h, e, active, gap}; /* Push-Relabel state variables declaration */

	Queue FIFO = initQ();

	initializePreFLow(G, FIFO, (PR_State_t*) &state, 1, 0);

	while (!isEmptyQ(FIFO))
	{
		u = getQ(FIFO);
		discharge(G, FIFO, (PR_State_t*) &state, u);
	}

	output->maxFlow = state.e[0];
	getMinCut(G, (PR_State_t*) &state, output, 0);

	free(h);
	free(e);
	free(active);
	free(gap);

	freeQ(FIFO);
}

void printG(Graph G)
{
	int i;

	for (i = 0; i < G->V; i++)
	{
		printf("%d: ", i);
		printL(G->adj[i]);
	}
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
}
