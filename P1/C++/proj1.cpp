#include <forward_list>
#include <iostream>
#include <stdio.h>
#include "Network.h"

#define MIN_ROUTER_NUM 2
#define MIN_CONNECTION_NUM 1

int main() {
  int V;   /* Number of routers */
	int E;   /* Number of connections bettween routers */

	int u, v; /* Routers to connect */

	if (!scanf("%d", &V) || V < MIN_ROUTER_NUM)
		printf("Invalid number of routers!\n");

	Network Net(V); /* Network structure that maps the router network */

	if (!scanf("%d", &E) || E < MIN_CONNECTION_NUM)
		printf("Invalid number of connections!\n");

  for (int i = 0; i < E; i++)
	{
		scanf("%d %d", &u, &v);
		insertEdgeG(G, createEdgeG(u, v));
	}
  return 0;
}
