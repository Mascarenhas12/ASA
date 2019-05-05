/* ================================================
 * IST LEIC-T Analise e Sintese de Algoritmos 18/19
 * Project 2 - queue.c
 *
 * Authors:
 * Manuel Mascarenhas - 90751
 * Miguel Levezinho   - 90756
 * ================================================
 */

#ifndef QUEUE_H
#define QUEUE_H

typedef struct queueNode *QueueLink;

typedef struct queue *Queue;

struct queueNode
{
	int id;
	QueueLink next;
};

struct queue
{
	int N;
	QueueLink front;
	QueueLink back;
};

Queue initQ();
void putQ(Queue Q, int u);
int getQ(Queue Q);
int isEmptyQ(Queue Q);
void printQ(Queue Q);
void freeQ(Queue Q);

#endif