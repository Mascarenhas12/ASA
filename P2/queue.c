/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - queue.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

/*
 * O(1)
 */
Queue initQ()
{
	Queue new;

	new = (Queue) malloc(sizeof(struct queue));

	new->front = NULL;
	new->back = NULL;
	new->N = 0;

	return new;
}

/*
 * O(1)
 */
void putQ(Queue Q, int u)
{
	QueueLink new = (QueueLink) malloc(sizeof(struct queueNode));

	new->id = u;
	new->next = NULL;

	if (!isEmptyQ(Q))
	{
		Q->back->next = new;
	}
	else
	{
		Q->front = new;
	}

	Q->N++;
	Q->back = new;
}

/*
 * O(1)
 */
int getQ(Queue Q)
{
	QueueLink t;

	if (!isEmptyQ(Q))
	{
		int id = Q->front->id;
		t = Q->front;
		Q->front = t->next;
		free(t);

		if (isEmptyQ(Q))
		{
			Q->back = NULL;
		}

		Q->N--;
		return id;
	}

	return 0;
}

/*
 * O(1)
 */
int isEmptyQ(Queue Q)
{
	return Q->N == 0;
}

/*
 * O(N)
 */
void printQ(Queue Q)
{
	QueueLink t;

	for (t = Q->front; t; t = t->next)
	{
		printf("%d", t->id);
		
		if (t->next != NULL)
		{
			printf(" ");
		}
	}
	printf("\n");
}

/*
 * O(N)
 */
void freeQ(Queue Q)
{
	QueueLink t;

	while (Q->front != NULL)
	{
		t = Q->front;
		Q->front = t->next;
		free(t);
	}
	free(Q);
}




