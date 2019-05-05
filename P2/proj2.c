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

#include "edge.h"
#include "graph.h"

#define MIN_NUM_PROVIDERS 1
#define MIN_NUM_SUPPLY_ST 0
#define MIN_NUM_CONNECTIONS 0

#define MIN_PROVIDER_ID 2
#define MIN_DESTINY_ID 1
#define MIN_CAPACITY 1

NetAudit initNetAudit(Graph G);
void freeAudit(NetAudit a);

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

	V = P + 2*S + 2; /* Add source and sink (hiper) and double stations (have a inner capacity) */

	Net = initG(V);

	/* Add T edges plus edges from source to providers and for stations (have a inner capacity) */

	for (i = 2; i < P+2; i++)
	{
		scanf("%d", &cap);
		insertWeightedEdgeG(Net, 0, i, cap); /* 0 represents the source */
	}

	for (i = P+2; i < S+(P+2); i++)
	{
		scanf("%d", &cap);
		insertWeightedEdgeG(Net, i, i+S, cap);
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
		insertWeightedEdgeG(Net, u, v, cap);
	}

	NetAudit out = initNetAudit(Net);

	printf("\n");
	pushRelabelFIFO(Net, out);

	freeG(Net);
	freeAudit(out);
	
	return 0;
}

/* Function that allocs space to store the audit info.
 * Returns a pointer to the audit structure.
 *
 * G - Graph in which the audit will be made
 */
NetAudit initNetAudit(Graph G)
{
	NetAudit new = (NetAudit) malloc(sizeof(struct audit));

	new->maxFlow = 0;
	new->updateStations = (int*) malloc(sizeof(int) * G->V);
	new->updateConnections = (Edge*) malloc(sizeof(struct edge) * G->E);

	return new;
}

/* Function that frees the audit info.
 *
 * a - Points to the struct that stores the audit info
 */
void freeAudit(NetAudit a)
{
	free(a->updateStations);
	free(a->updateConnections);
	free(a);
}



