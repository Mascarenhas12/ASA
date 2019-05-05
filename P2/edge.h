/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - edge.h
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#ifndef EDGE_H
#define EDGE_H

/* Struct that represents a node of a generic list */
struct edge
{
	int u;
	int v;
	int cap;
	int flow;
};

/* Abstraction of the node struct to a type Link */
typedef struct edge *Edge;

#endif