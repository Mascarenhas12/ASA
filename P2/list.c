/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - list.c
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
#include "list.h"

/* Function that inserts an integer into a list, given its head.
 * Insertion is at the beginning of the list.
 * Asymptotic complexity is O(1).
 * Returns a pointer to the inserted node so that the head can be updated.
 *
 * head - Pointer to the first element on the list
 * id   - Identifier of the item
 * cap  - Capacity of the link
 */
Link insertL(Link head, Edge edge)
{
	Link new = (Link) malloc(sizeof(struct node));

	new->edge = edge;
	new->next = head;
	new->opposite = NULL;

	return new;
}

void linkOppositesL(Link t, Link u)
{
	t->opposite = u;
	u->opposite = t;
}

void printL(Link head)
{
	Link t;

	for (t = head; t; t = t->next)
	{
		printf("%d", t->edge->v);
		if (t->next != NULL)
		{
			printf(" ");
		}
	}

	printf("\n");
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
		free(t->edge);
		free(t);
	}
}

/*
List initL()
{
	List new = (List) malloc(sizeof(struct list));

	new->head = NULL;
	new->tail = NULL;
	new->N = 0;

	return new;
}*/

/*
int sizeL(List L)
{
	return L->N;
}*/

/* Function that inserts an integer into a given list.
 * Insertion is at the end of the list.
 * Asymptotic complexity is O(1).
 *
 * L    - List in which to insert an edge
 * edge - Edge to insert

void oldinsertL(List L, Edge edge)
{
	Link new = (Link) malloc(sizeof(struct node));

	new->edge = edge;
	new->next = NULL;

	if (L->head == NULL)
		L->head = new;
	else
		L->tail->next = new;

	L->tail = new;
	L->N++;
}*/
