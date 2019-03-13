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

int main()
{
	int V;   /* Number of routers */
	int E;   /* Number of connections bettween routers */
	Graph G; /* Graph structure that maps the router network */

	int u, v; /* Routers to connect */

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

	/* ********** DEBUG ********** */
	//printf("***** ROUTER NETWORK *****\n");
	//printG(G);
	/* ********** DEBUG ********** */

	//TODO passar struct* de info de cut vertices para funcao
	dfsState_t state = dfsG(G);

	return 0;
}
