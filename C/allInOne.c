/* ==========================================================================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - proj1.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ==========================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#define MIN_ROUTER_NUM 2
#define MIN_CONNECTION_NUM 1
#define NIL -1

/************************************************************************************************************
 * list.h
 ***********************************************************************************************************/

/* Abstraction of the node struct to a type Link */
typedef struct node *Link;

/* Struct that represents a node of a list of items */
struct node
{
	int id;
	Link next;
};

Link insertL(Link head, int id);
void freeL(Link head);

/************************************************************************************************************
 * graph.h
 ***********************************************************************************************************/

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

enum dfs_colors {
    WHITE = 0,
    GRAY  = 1,
    BLACK = 2,
};

Graph initG(int V);
Edge createEdgeG(int u, int v);
void insertEdgeG(Graph G, Edge e);
void doDFS_G(Graph G, Audit output);
void doTarjanSearchG(Graph G, Audit output);
void freeG(Graph G);

/************************************************************************************************************
 * proj1.h
 ***********************************************************************************************************/

Audit initAudit(Graph G);
void doAudit(Graph G, Audit output);
void printAudit(Graph G, Audit output);
void freeAudit(Audit a);

/************************************************************************************************************
 * proj1.c
 ***********************************************************************************************************/

int main()
{
	int V;   /* Number of routers */
	int E;   /* Number of connections bettween routers */
	Graph G; /* Graph structure that maps the router network */

	int u, v; /* Routers to connect */

	Audit output; /* Stores the results of the audit made to the network */

	int i;

	if (!scanf("%d", &V) || V < MIN_ROUTER_NUM)
	{
		printf("Invalid number of routers!\n");
	}

	if (!scanf("%d", &E) || E < MIN_CONNECTION_NUM)
	{
		printf("Invalid number of connections!\n");
	}

	G = initG(V);

	for (i = 0; i < E; i++)
	{
		scanf("%d %d", &u, &v);
		insertEdgeG(G, createEdgeG(u, v));
	}

	output = initAudit(G);

	doAudit(G, output);

	printAudit(G, output);

	freeAudit(output);
	freeG(G);

	return 0;
}

/* Function that allocs space to store the audit info.
 * Returns a pointer to the audit structure.
 *
 * G - Graph in which the audit will be made
 */
Audit initAudit(Graph G)
{
	Audit new = (Audit) malloc(sizeof(struct audit));

	new->numSubNets = 0;
	new->netID = 0;
	new->subNetIDs = (int*) malloc(sizeof(int) * G->V);
	new->numCutV = 0;
	new->cutV = (int*) malloc(sizeof(int) * G->V);
	new->maxNetSize = 0;
	return new;
}

void doAudit(Graph G, Audit output)
{
	doTarjanSearchG(G, output);
	doDFS_G(G, output);
}

/* Function that prints the audit info.
 *
 * output - Points to the struct that stores the audit info
 */
void printAudit(Graph G, Audit output)
{
	int i;
	int flag = 0;

	printf("%d\n", output->numSubNets);

	for (i = 0; i < G->V; i++)
	{
		if (output->subNetIDs[i] != NIL)
		{
			if (!flag)
			{
				printf("%d", output->subNetIDs[i]);
				flag = 1;
			}
			else
			{
				printf(" %d", output->subNetIDs[i]);
			}
		}
	}

	printf("\n%d\n", output->numCutV);
	printf("%d\n", output->maxNetSize);
}

/* Function that frees the audit info.
 *
 * a - Points to the struct that stores the audit info
 */
void freeAudit(Audit a)
{
	free(a->subNetIDs);
	free(a->cutV);
	free(a);
}

/************************************************************************************************************
 * graph.c
 ***********************************************************************************************************/

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
			state->p[v->id-1] = u;
			output->netID = MAX(output->netID, v->id);
			count = tarjanVisitG(G, v->id, (void*) state, count, output);

			/* Peeks at the finalized (Black) child vertex and retrieves its low if its smaller */
			state->low[u-1] = MIN(state->low[u-1], state->low[v->id-1]);

			if (state->p[u-1] != NIL && state->low[v->id-1] >= state->d[u-1] && !output->cutV[u-1])
			{
				output->numCutV++;
				output->cutV[u-1] = 1;
			}
		}

		if (state->color[v->id-1] == GRAY && state->p[u-1] != v->id)
		{
			/* Peeks at the ancestor vertex and retrieves its discovery if its smaller */
			state->low[u-1] = MIN(state->low[u-1], state->d[v->id-1]);
		}
	}

	/* If u is the root of a DFS tree and has more than two child vertices, its a cut vertice */
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
 * The SCCs will be of the resultant DFS forest.
 * Cut vertices can be detected in the root or on the transitions between SCCs in this forest.
 * Asymptotic complexity is O(V + E).
 *
 * G      - Graph to apply a dfs
 * output - Points to the struct that will store the above graph info
 */
void doTarjanSearchG(Graph G, Audit output)
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
		d[u-1] = NIL;
		p[u-1] = NIL;
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
static int dfsVisitG(Graph G, int u, int* color, int numVertices, Audit output)
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
	int color[G->V]; 	 /* Vertex visit states */
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

/************************************************************************************************************
 * list.c
 ***********************************************************************************************************/

/* Function that inserts an integer into a list, given its head.
 * Insertion is at the beginning of the list.
 * Asymptotic complexity is O(1).
 * Returns a pointer to the inserted node so that the head can be updated.
 *
 * head - Pointer to the first element on the list
 * id   - Integer to be removed
 */
Link insertL(Link head, int id)
{
	Link new = (Link) malloc(sizeof(struct node));

	new->id = id;
	new->next = head;

	return new;
}

/* Function that frees a list from memory, given its head.
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
		free(t);
	}
}
