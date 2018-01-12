/* ------------------------------------------------------------------------ *\ 

   Name . . . . . : STACK.C
   
   Author . . . . : Paul Markham (3:711/934.1@fidonet)
   
   Date written . : September 1995
   
   ------------------------------------------------------------------------
   This code is released to the public domain. You may use and/or modified
   it in any way you choose.
  
   There is no warranty of any kind associated with this code. If you use
   it, you accept all responsibility for any loss or damages that may
   occur. So there!
   ------------------------------------------------------------------------

   Implementation for a LIFO stack.
   
\* ------------------------------------------------------------------------ */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "stack.h"

/* ------------------------------------------------------------------------ *\
   Create a new stack
\* ------------------------------------------------------------------------ */

int StackCreate(STACK *stack)
    {
    int rc;
    
    rc = DLListCreate(stack);
    
    return rc;
    }

/* ------------------------------------------------------------------------ *\
   Destroy the stack
\* ------------------------------------------------------------------------ */

void StackDestroy(STACK stack)
    {
    DLListDestroy(stack);
    }
    
/* ------------------------------------------------------------------------ *\
   Push a node onto the head of the stack
\* ------------------------------------------------------------------------ */

int StackPush(STACK stack, void *data, size_t len)
    {
    int rc;

    rc = DLListInsertBefore(stack, data, len);
    
    return rc;
    }    
    
/* ------------------------------------------------------------------------ *\
   Pop a node off the head of the stack
\* ------------------------------------------------------------------------ */

int StackPop(STACK stack, void *data, size_t len)
    {
    int rc;
    
    rc = DLListGetCurrent(stack, data, len);
    if (rc == DLLIST_OK)
        rc = DLListDelete(stack);
        
    return rc;
    }        
    
/* ------------------------------------------------------------------------ *\
   Have a look at the node at the head of the list but don't delete it.
\* ------------------------------------------------------------------------ */

int StackPeek(STACK stack, void *data, size_t len)
    {
    int rc;
    
    rc = DLListGetCurrent(stack, data, len);
        
    return rc;
    }        
    
/* ------------------------------------------------------------------------ *\
   Get number of nodes in stack
\* ------------------------------------------------------------------------ */

long StackGetCount(STACK stack)
    {
    long rc;
    
    rc = DLListGetCount(stack);
    
    return rc;
    }    
