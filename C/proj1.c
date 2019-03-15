/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - proj1.c
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

#define MIN_ROUTER_NUM 2
#define MIN_CONNECTION_NUM 1
#define NIL -1

Audit initAudit(Graph G);
void printAudit(Graph G, Audit output);
void freeAudit(Audit a);

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
		insertEdgeG(G, u, v);
	}

	output = initAudit(G);

	doTarjanSearchG(G, output);
	doDFS_G(G, output);
	freeG(G);

	printAudit(G, output);
	freeAudit(output);

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
	new->cutV = (char*) malloc(sizeof(char) * G->V);
	new->maxNetSize = 0;
	return new;
}

/* Function that prints the audit info.
 *
 * G      - Graph the audit was applied on
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
