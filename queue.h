/* ------------------------------------------------------------------------ *\ 

   Name . . . . . : QUEUE.H
   
   Author . . . . : Paul Markham (3:711/934.1@fidonet)
   
   Date written . : September 1995
   
   ------------------------------------------------------------------------
   This code is released to the public domain. You may use and/or modified
   it in any way you choose.
  
   There is no warranty of any kind associated with this code. If you use
   it, you accept all responsibility for any loss or damages that may
   occur. So there!
   ------------------------------------------------------------------------

   Header for a FIFO queue.
   
\* ------------------------------------------------------------------------ */

#if !defined(__QUEUE_H)
#define __QUEUE_H

#include "dllist.h"

typedef DLLIST QUEUE;

enum
    {
    QUEUE_OK,
    QUEUE_MALLOC_FAILED,
    QUEUE_EMPTY_LIST,
    QUEUE_END_OF_LIST,
    };
    
    
int  QueueCreate(QUEUE *);
void QueueDestroy(QUEUE);
int  QueuePut(QUEUE, void *, size_t);
int  QueueGet(QUEUE, void *, size_t);
int  QueuePeek(QUEUE, void *, size_t);
int  QueueIsEmpty(QUEUE);
long QueueGetCount(QUEUE);
    
#endif
