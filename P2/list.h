/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - list.h
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#ifndef LIST_H
#define LIST_H

#include "edge.h"

/* Abstraction of the node struct to a type Link */
typedef struct node *Link;

/* Abstraction of the list struct to a type List 
typedef struct list *List;*/

/* Struct that represents a node of a generic list */
struct node
{
	Edge edge;
	Edge opEdge;
	Link next;
};

/*
struct list
{
	Link head;
	Link tail;
	int N;
};*/

/*List initL();
void oldinsertL(List L, Edge edge);
int sizeL(List L);*/
Link insertL(Link head, Edge edge);
void linkOppositesL(Link t, Link u);
void printL(Link head);
void freeL(Link head);

#endif
