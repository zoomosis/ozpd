/* ------------------------------------------------------------------------ *\ 

   Name . . . . . : QUEUE.C
   
   Author . . . . : Paul Markham (3:711/934.1@fidonet)
   
   Date written . : September 1995
   
   ------------------------------------------------------------------------
   This code is released to the public domain. You may use and/or modified
   it in any way you choose.
  
   There is no warranty of any kind associated with this code. If you use
   it, you accept all responsibility for any loss or damages that may
   occur. So there!
   ------------------------------------------------------------------------

   Implementation for a FIFO queue.
   
\* ------------------------------------------------------------------------ */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

/* ------------------------------------------------------------------------ *\
   Create a new queue
\* ------------------------------------------------------------------------ */

int QueueCreate(QUEUE *Queue)
    {
    int rc;
    
    rc = DLListCreate(Queue);
    
    return rc;
    }

/* ------------------------------------------------------------------------ *\
   Destroy the queue
\* ------------------------------------------------------------------------ */

void QueueDestroy(QUEUE Queue)
    {
    DLListDestroy(Queue);
    }
    
/* ------------------------------------------------------------------------ *\
   Add a new node to the tail of the queue
\* ------------------------------------------------------------------------ */

int QueuePut(QUEUE Queue, void *data, size_t len)
    {
    int rc;

    if (DLListIsEmpty(Queue))
        rc = DLLIST_OK;
    else    
        rc = DLListMoveTail(Queue);
    if (rc == DLLIST_OK)
        rc = DLListInsertAfter(Queue, data, len);
    
    return rc;
    }    
    
/* ------------------------------------------------------------------------ *\
   Retrieve the node at the head of the queue
\* ------------------------------------------------------------------------ */

int QueueGet(QUEUE Queue, void *data, size_t len)
    {
    int rc;
    
    rc = DLListGetHead(Queue, data, len);
    if (rc == DLLIST_OK)
        rc = DLListDelete(Queue);
        
    return rc;
    }        
    
/* ------------------------------------------------------------------------ *\
   Have a look at the node at the head of the queue but don't delete it
\* ------------------------------------------------------------------------ */

int QueuePeek(QUEUE Queue, void *data, size_t len)
    {
    int rc;
    
    rc = DLListGetHead(Queue, data, len);
        
    return rc;
    }        
    
/* ------------------------------------------------------------------------ *\
   Get number of nodes in queue
\* ------------------------------------------------------------------------ */

long QueueGetCount(QUEUE Queue)
    {
    long rc;
    
    rc = DLListGetCount(Queue);
    
    return rc;
    }    
