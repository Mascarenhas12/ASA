/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - proj2.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define MIN_NUM_PROVIDERS 1
#define MIN_NUM_SUPPLY_ST 0
#define MIN_NUM_CONNECTIONS 0

#define MIN_PROVIDER_ID 2
#define MIN_DESTINY_ID 1
#define MIN_CAPACITY 1

/************************************************************************************************************
 * edge.h
 ***********************************************************************************************************/

/* Struct that represents an edge of a flow network */
struct edge
{
	int u;
	int v;
	int cap;
	int flow;
};

/* Abstraction of the edge struct to a pointer type Edge */
typedef struct edge *Edge;

/************************************************************************************************************
 * list.h
 ***********************************************************************************************************/

/* Abstraction of the node struct to a pointer type Link */
typedef struct node *Link;

/* Struct that represents a node of an edge list */
/* opEdge points to the opposite edge between to vertices */
struct node
{
	Edge edge;
	Edge opEdge;
	Link next;
};

Link insertL(Link head, Edge edge);
void linkOppositesL(Link t, Link u);
void freeL(Link head);

/************************************************************************************************************
 * queue.h
 ***********************************************************************************************************/

/* Abstraction of the queueNode struct to a pointer type QueueLink */
typedef struct queueNode *QueueLink;

/* Abstraction of the queue struct to a pointer type Queue */
typedef struct queue *Queue;

/* Struct that represents a node of an integer queue */
struct queueNode
{
	int id;
	QueueLink next;
};

/* Struct that represents a queue */
struct queue
{
	int N;
	QueueLink front;
	QueueLink back;
};

Queue initQ();
void putQ(Queue Q, int u);
int getQ(Queue Q);
int isEmptyQ(Queue Q);
void freeQ(Queue Q);

/************************************************************************************************************
 * graph.h
 ***********************************************************************************************************/

/* Struct that represents the transportation flow network */
struct graph
{
	int V; /* Number of vertices */
	int P; /* Number of providers */
	int S; /* Number of stations */
	int E; /* Number of transport edges */
	Link *adj;
};

/* Struct that represents the state of the FIFO push-relabel algorithom,
 * storing its relevant variables */
struct PR_State
{
	int* h;
	int* e;
	char* active;
	int* gap;
};

/* Abstraction of the graph struct to a pointer type Graph */
typedef struct graph *Graph;

/* Abstraction of the PR_State struct to a type PR_State_t */
typedef struct PR_State PR_State_t;

/* Struct that represents the information to retrive from the network */
struct audit
{
	int maxFlow;
	char* minCutS;
	Edge* minCutE;

	int idx;
};

/* Abstraction of the audit struct to a pointer type NetAudit */
typedef struct audit *NetAudit;

Graph initG(int V,int P,int S);
void insertWeightedEdgeG(Graph G, int u, int v, int c);
void pushRelabelFIFO(Graph G, NetAudit output);
void freeG(Graph G);

/************************************************************************************************************
 * proj1.h
 ***********************************************************************************************************/

NetAudit initNetAudit(Graph G);
void printNetAudit(Graph G, NetAudit out);
void freeAudit(NetAudit a);

/************************************************************************************************************
 * proj1.c
 ***********************************************************************************************************/

int cmpfunc (const void * a, const void * b)
{
	if ((*(Edge*)a)->u - (*(Edge*)b)->u)
	{
		return (*(Edge*)a)->u - (*(Edge*)b)->u;
	}
	return (*(Edge*)a)->v - (*(Edge*)b)->v;
}

int main()
{
	int P; /* Number of providers */
	int S; /* Number of supply stations */
	int T; /* Number of network connections */

	int V;     /* Number of vertices in the flow network */
	Graph Net; /* Graph structure that models the transportation network as a flow network */

	int u, v; /* Vertices to connect */
	int cap;  /* Capacity of the connections */

	int i;

	if (!scanf("%d %d %d", &P, &S, &T) || P < MIN_NUM_PROVIDERS || S < MIN_NUM_SUPPLY_ST || T < MIN_NUM_CONNECTIONS)
	{
		printf("Invalid input!\n");
		exit(1);
	}

	V = P + 2*S + 2; /* Add source (hiper) and sink and double stations (have a inner capacity) */

	Net = initG(V, P, S);

	/* Add T edges plus edges from source to providers and for stations (have a inner capacity) */

	for (i = 2; i < P+2; i++)
	{
		scanf("%d", &cap);
		insertWeightedEdgeG(Net, i, 0, cap); /* 0 represents the sink */
	}

	for (i = P+2; i < S+(P+2); i++)
	{
		scanf("%d", &cap);
		insertWeightedEdgeG(Net, i+S, i, cap);
	}

	for (i = 0; i < T; i++)
	{
		if (!scanf("%d %d %d", &u, &v, &cap) || u < MIN_PROVIDER_ID || v < MIN_DESTINY_ID || cap < MIN_CAPACITY)
		{
			printf("Invalid input!\n");
			exit(1);
		}
		if (u > P+1)
		{
			u = u + S;
		}
		insertWeightedEdgeG(Net, v, u, cap);
	}

	NetAudit out = initNetAudit(Net);

	pushRelabelFIFO(Net, out);

	printNetAudit(Net, out);

	freeG(Net);
	freeAudit(out);

	return 0;
}

/* Function that allocs space to store the network audit info.
 * The vector that stores the edges of the min-cut has max size V and not E
 * because the number of edges of the min-cut cannot be bigger than V.
 * Returns a pointer to the audit structure.
 *
 * G - Flow network in which the audit will be preformed
 */
NetAudit initNetAudit(Graph G)
{
	NetAudit new = (NetAudit) malloc(sizeof(struct audit));

	new->maxFlow = 0;
	new->minCutS = (char*) calloc(G->V, sizeof(char));
	new->minCutE = (Edge*) calloc(G->E, sizeof(Edge));
	new->idx = 0;

	return new;
}

/* Function that prints the audit info.
 * Uses part of the counting sort algorithom to sort stations to print.
 * Uses the qsort lib function to sort edges by their vertices IDs.
 *
 * G      - Flow Network the audit was applied on
 * output - Points to the struct that stores the audit info
 */
void printNetAudit(Graph G, NetAudit out)
{
	int i;
	int flag = 0;

	printf("%d\n", out->maxFlow);

	for (i = G->P+2; i < (G->V - G->S); i++)
	{
		if (out->minCutS[i] != 1)
			continue;

		if (!flag)
		{
			printf("%d", i);
			flag = 1;
		}
		else
		{
			printf(" %d", i);
		}
	}
	printf("\n");

	qsort(out->minCutE, out->idx, sizeof(Edge), cmpfunc);

	for (i = 0; i < G->E; i++)
	{
		if (i == out->idx)
			break;

		if (out->minCutE[i] == NULL)
			continue;

		if (out->minCutE[i]->u >= (G->V - G->S))
		{
			out->minCutE[i]->u -= G->S;
		}
		printf("%d %d\n", out->minCutE[i]->u, out->minCutE[i]->v);
	}
}

/* Function that frees the network audit info.
 *
 * a - Points to the struct that stores the audit info
 */
void freeAudit(NetAudit a)
{
	free(a->minCutS);
	free(a->minCutE);
	free(a);
}

/************************************************************************************************************
 * graph.c
 ***********************************************************************************************************/

/* Function that creates and initializes a graph, given the number of vertices.
 * The implementation uses an enhanced list of adjacencies and the graph stores edges.
 * Asymptotic complexity is O(V).
 * Returns a pointer to the graph structure.
 *
 * V - Number of vertices in the graph
 * P - Number of providers
 * S - Number of supply stations
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

/* Function that creates a flow restricted edge.
 * Asymptotic complexity is O(1).
 * Returns a pointer to the created edge.
 *
 * u - Vertex of the edge
 * v - Vertex of the edge
 * c - Capacity of the edge
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

	G->E++;
}

/* Function that initializes the flow network and begins the push-relabel algorithom.
 * Asymptotic complexity is O(V).
 *
 * G     - Graph that represents the flow network
 * FIFO  - Queue in which vertices with excess are stored
 * state - Push-relabel state that stores state variables, like heights
 * s     - Source of the flow network
 * t     - Sink of the flow network
 */
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
	}

	state->h[s] = G->V;
	state->gap[G->V] = 1;
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

/* Function that applies the Gap Relabeling heuristic every time there is a mim-cut in the network.
 * Asymptotic complexity is O(V).
 *
 * G     - Graph that represents the flow network
 * FIFO  - Queue in which vertices with excess are stored
 * state - Push-relabel state that stores state variables
 * h     - Height gap that will represent a cut
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

/* Function that relabels a vertex, increasing its height to 1+ the minimum height in the neighboors.
 * Asymptotic complexity is O(V).
 *
 * G     - Graph that represents the flow network
 * state - Push-relabel state that stores state variables
 * u     - Vertex to relabel
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

/* Function that pushes flow to a neighboor vertex, through the edge between them.
 * A push is saturating if it pushes all the excess flow out of the vertex,
 * and non-sturating if excess remains in the vertex.
 * Asymptotic complexity is O(1).
 *
 * state - Push-relabel state that stores state variables
 * t     - Vertex to relabel
 * u     - Vertex of the edge, in which excess will be removed
 * v     - Vertex of the edge in which flow will pass
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

/* Function that determines when to apply pushes or relabels to vertices with excess,
 * managing between the two depending on neighboor residual capacities, heights, and height gaps.
 * Discharge continues until the vertex has no more excess flow.
 *
 * G     - Graph that represents the flow network
 * FIFO  - Queue in which vertices with excess are stored
 * state - Push-relabel state that stores state variables
 * u     - Vertex to discharge of excess
 */
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
			if (state->h[u] < G->V && state->gap[state->h[u]] == 1)
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

/* Function that determines the edges that the min-cut passes through.
 * The considered min-cut is the one nearest to the source.
 * Asymptotic complexity is O(E).
 *
 * G      - Graph that represents the flow network
 * FIFO   - Queue in which vertices with excess are stored
 * output - Points to the struct that stores the audit info
 * s      - Source of the flow network
 */
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

/* Start function of the push-relabel method, implemented with the FIFO rule and Gap Relabeling heuristic.
 * Determines max flow and the edges that cross the min-cut.
 * Asymptotic complexity is O(V^3).
 *
 * G      - Graph that represents the flow network
 * output - Points to the struct that stores the audit info
 */
void pushRelabelFIFO(Graph G, NetAudit output)
{
	int u;

	int* h = (int*) malloc(sizeof(int) * G->V); 		/* Heights of vertices */
	int* e = (int*) malloc(sizeof(int) * G->V); 		/* Excesses of vertices */
	char* active = (char*) malloc(sizeof(char) * G->V); /* Active vertices (that have excess) */
	int* gap = (int*) calloc(2 * G->V, sizeof(int));     /* Counter of heights */

	PR_State_t state = {h, e, active, gap}; /* Push-Relabel state variables declaration */

	Queue FIFO = initQ(); /* Queue that will store active vertices (with excess) */

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

/************************************************************************************************************
 * queue.c
 ***********************************************************************************************************/

/* Function that creates a firt-in-first-out queue.
 * Mantains a pointer to the front and back, for efficient removal and insertion respectively.
 * Asymptotic complexity is O(1).
 * Returns a pointer to the created queue.
 */
Queue initQ()
{
	Queue new;

	new = (Queue) malloc(sizeof(struct queue));

	new->front = NULL;
	new->back = NULL;
	new->N = 0;

	return new;
}

/* Function that inserts an integer into a queue, given the queue.
 * Insertion is at the end of the queue.
 * Asymptotic complexity is O(1).
 *
 * Q - Queue in which to insert an integer
 * u - Integer to insert
 */
void putQ(Queue Q, int u)
{
	QueueLink new = (QueueLink) malloc(sizeof(struct queueNode));

	new->id = u;
	new->next = NULL;

	if (!isEmptyQ(Q))
	{
		Q->back->next = new;
	}
	else
	{
		Q->front = new;
	}

	Q->back = new;
	Q->N++;
}

/* Function that removes an integer from a queue, given the queue.
 * Removal is at the beginning of the queue.
 * Asymptotic complexity is O(1).
 * Returns the removed integer.
 *
 * Q - Queue from which to remove an integer
 */
int getQ(Queue Q)
{
	QueueLink t;
	int id;

	if (!isEmptyQ(Q))
	{
		id = Q->front->id;
		t = Q->front;
		Q->front = t->next;
		free(t);

		if (isEmptyQ(Q))
		{
			Q->back = NULL;
		}

		Q->N--;
		return id;
	}

	return 0;
}

/* Function that determines if a queue is empty, given the queue.
 * Asymptotic complexity is O(1).
 * Returns true if empty and false otherwise.
 *
 * Q - Queue to query
 */
int isEmptyQ(Queue Q)
{
	return Q->N == 0;
}

/* Function frees a queue, given the queue.
 * Asymptotic complexity is O(N).
 *
 * Q - Queue to free
 */
void freeQ(Queue Q)
{
	QueueLink t;

	while (Q->front != NULL)
	{
		t = Q->front;
		Q->front = t->next;
		free(t);
	}
	free(Q);
}

/************************************************************************************************************
 * list.c
 ***********************************************************************************************************/

/* Function that inserts an edge into a list, given its head.
 * Insertion is at the beginning of the list.
 * Asymptotic complexity is O(1).
 * Returns a pointer to the inserted node so that the head can be updated.
 *
 * head - Pointer to the first element on the list
 * edge - Edge to insert
 */
Link insertL(Link head, Edge edge)
{
	Link new = (Link) malloc(sizeof(struct node));

	new->edge = edge;
	new->next = head;
	new->opEdge = NULL;

	return new;
}

/* Function that sets two edges in the list as opposites, mantaining a pointer to each of them.
 * Asymptotic complexity is O(1).
 *
 * t - Link of the edge
 * u - Link of the opposite edge
 */
void linkOppositesL(Link t, Link u)
{
	t->opEdge = u->edge;
	u->opEdge = t->edge;
}

/* Function that frees a list from memory, given its head.
 * Asymptotic complexity is O(N).
 *
 * head - Pointer to the first element on the list
 */
void freeL(Link head)
{
	Link t;

	while (head != NULL)
	{
		t = head;
		head = t->next;
		free(t->edge);
		free(t);
	}
}