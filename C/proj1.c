/* IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 1 - proj1.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "list.h"

int main() {
	int numNodes;
	int numEdges;
	scanf("%d\n", &numNodes);
	printf("%d\n", numNodes);
	Graph G = initG(numNodes);
	scanf("%d\n", &numEdges);
	printf("%d\n", numEdges);
	for(int i=0;i<numEdges;i++){
		int u,v;
		scanf("%d %d\n", &u,&v);
		puts("wow");
		insertEdgeG(G,createEdgeG(u,v));
	}

	DFSarg_t DFS = depthFirstSearchG(G);
	return 0;
}
