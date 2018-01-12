/******************************************************************************/
/*                                                                            */
/*  Name:        prioq.h                                                      */
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

#ifndef __PQUEUE_H
#define __PQUEUE_H
#endif

/* priority queue structure definitions */

typedef struct qItem {
	unsigned int	ItemKey;
	void		*data;
} qItem;

typedef qItem *qItemPtr;

typedef struct Queue {
	int		MaxSize;
	int		NumberElements;
	qItemPtr	*ppqItemArray;
} Queue;

typedef Queue *QueuePtr;

QueuePtr qCreate(int qsize);

void qFree(QueuePtr pQueue);

int qFull(QueuePtr pQueue);

int qEmpty(QueuePtr pQueue);

void qInsert(QueuePtr pQueue, qItemPtr pItem);

qItemPtr qRemove(QueuePtr pQueue);

void qDisplay(QueuePtr pQueue);
