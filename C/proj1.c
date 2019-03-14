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

Audit initNetAudit(Graph G);
void printAudit(Audit output, Graph G);
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
		printf("Invalid number of routers!\n");

	if (!scanf("%d", &E) || E < MIN_CONNECTION_NUM)
		printf("Invalid number of connections!\n");

	G = initG(V);

	for (i = 0; i < E; i++)
	{
		scanf("%d %d", &u, &v);
		insertEdgeG(G, createEdgeG(u, v));
	}

	output = initNetAudit(G);

	puts("luigi");
	doAuditG(G, output);//CHANGED
	puts("waluigi");
	biggestNetSizeAudit(G, output);
	//TODO new DFS func that calcs biggest net size
	printAudit(output,G);

	return 0;
}

/* Function that allocs space to store the audit info.
 * Returns a pointer to the audit structure.
 *
 * G - Graph in which the audit will be made
 */

Audit initNetAudit(Graph G)//CHANGED
{
	Audit new = (Audit) malloc(sizeof(struct audit));

	new->numSubNets = 0;
	new->numBNet = 0;
	new->numCutV = 0;
	new->biggestV = 0;
	new->cutV = (int*) malloc(sizeof(int) * G->V);
	new->subBigV = (int*) malloc(sizeof(int) * G->V);
	return new;
}

/* Function that prints the audit info.
 *
 * output - Points to the struct that stores the audit info
 */
void printAudit(Audit output, Graph G)
{
	printf("%d\n", output->numSubNets);
	for(int i=0;i<G->V;i++){
		if(output->subBigV[i] != NIL)
			printf("%d \n", output->subBigV[i]);
	}
	printf("%d\n", output->numCutV);
	printf("%d\n", output->numBNet);
	//TODO Biggest net size
}

/* Function that frees the audit info.
 *
 * a - Points to the struct that stores the audit info
 */
void freeAudit(Audit a)
{
	free(a->cutV);
	free(a->subBigV);
	free(a);
}
