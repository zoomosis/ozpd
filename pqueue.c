/******************************************************************************/
/*                                                                            */
/*  Name:        pqueue.c                                                     */
/*                                                                            */
/*  Written by: David Galbraith                                               */
/*                                                                            */
/*  Date:         October 1995                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  This code is hereby released to the public domain.  You may use and/or    */
/*  modify it in any way you choose without restriction.                      */
/*                                                                            */
/******************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __PQUEUE_H
#include "pqueue.h"
#endif

extern char *progname;

static void qTidyUp(QueuePtr pQueue, int start);

static void qTidyDown(QueuePtr pQueue, int start);

QueuePtr qCreate(int qsize)
{
	QueuePtr	pQueue;
	qItemPtr	ppQueueItemArray;

	if ((pQueue = (QueuePtr) malloc(sizeof(Queue))) == NULL)
	{
		fprintf(stderr, "%s: %s", progname, strerror(errno));
		exit(errno);
	}

	pQueue->MaxSize = qsize;
	pQueue->NumberElements = 0;

	if ((ppQueueItemArray = (qItemPtr) malloc(sizeof(qItem) * (qsize + 1))) == NULL)
	{
		fprintf(stderr, "%s: %s", progname, strerror(errno));
		exit(errno);
	}

	pQueue->ppqItemArray = &ppQueueItemArray;

	return pQueue;
}

void qFree(QueuePtr pQueue)
{
	free ((void *) pQueue->ppqItemArray);
	free ((void *) pQueue);
	return;
}

int qFull(QueuePtr pQueue)
{
	if (pQueue == NULL)
		exit(EXIT_FAILURE);
	return(pQueue->NumberElements == 0);
}

int qEmpty(QueuePtr pQueue)
{
	if (pQueue == NULL)
		exit(EXIT_FAILURE);
	return(pQueue->NumberElements == 0);
}

void qInsert(QueuePtr pQueue, qItemPtr pItem)
{
	pQueue->NumberElements++;
	pQueue->ppqItemArray[pQueue->NumberElements] = pItem;
	qTidyUp(pQueue, pQueue->NumberElements);
}

qItemPtr qRemove(QueuePtr pQueue)
{
    qItemPtr	pItem;

	pItem = pQueue->ppqItemArray[1];
	pQueue->ppqItemArray[1] = pQueue->ppqItemArray[pQueue->NumberElements];
	pQueue->NumberElements -= 1;
	qTidyDown(pQueue, 1);
	return pItem;
}

void qDisplay(QueuePtr pQueue)
{
	int		i;

	for (i = 1; i <= pQueue->NumberElements; i++)
		printf("%d: %ud ", i, (pQueue->ppqItemArray[i])->ItemKey);
	printf("\n");
	return;
}

void qTidyUp(QueuePtr pQueue, int start)
{
	int		i;
	qItemPtr	pItem;

	i = start;
	pItem = pQueue->ppqItemArray[i];

	for(;;)
	{
		int j;

		if (i == 1)
                        break;
		j = i / 2;
		if ((pQueue->ppqItemArray[j])->ItemKey < pItem->ItemKey)
			break;
		pQueue->ppqItemArray[i] = pQueue->ppqItemArray[j];
		i = j;
	}
	pQueue->ppqItemArray[i] = pItem;
	return;
}

void qTidyDown(QueuePtr pQueue, int start)
{
	int		i;
	int		elements;
	qItemPtr	pItem;

	i = start;
	pItem = pQueue->ppqItemArray[i];
	elements = pQueue->NumberElements;

	for(;;)
	{
		int j;

		if (i > elements / 2)
                        break;
		j = i + i;
		if ((j < elements) && ((pQueue->ppqItemArray[j])->ItemKey > pQueue->ppqItemArray[j+1]->ItemKey))
			j++;
		if (pItem->ItemKey <= (pQueue->ppqItemArray[j])->ItemKey)
			break;
		pQueue->ppqItemArray[i] = pQueue->ppqItemArray[j];
		i = j;
	}
	pQueue->ppqItemArray[i] = pItem;
}
