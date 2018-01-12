/* ------------------------------------------------------------------------ *\ 

   Name . . . . . : STACK.H
   
   Author . . . . : Paul Markham (3:711/934.1@fidonet)
   
   Date written . : September 1995
   
   ------------------------------------------------------------------------
   This code is released to the public domain. You may use and/or modified
   it in any way you choose.
  
   There is no warranty of any kind associated with this code. If you use
   it, you accept all responsibility for any loss or damages that may
   occur. So there!
   ------------------------------------------------------------------------

   Header for a LIFO stack.
   
\* ------------------------------------------------------------------------ */

#if !defined(__STACK_H)
#define __STACK_H

#include "dllist.h"

/* ------------------------------------------------------------------------ *\
   Typedefs
\* ------------------------------------------------------------------------ */

typedef DLLIST STACK;

/* ------------------------------------------------------------------------ *\
   Return codes
\* ------------------------------------------------------------------------ */

enum
    {
    STACK_OK,
    STACK_MALLOC_FAILED,
    STACK_EMPTY_LIST,
    STACK_END_OF_LIST,
    };
    
    
/* ------------------------------------------------------------------------ *\
   Function prototypes
\* ------------------------------------------------------------------------ */

int  StackCreate(STACK *);
void StackDestroy(STACK);
int  StackPush(STACK, void *, size_t);
int  StackPop(STACK, void *, size_t);
int  StackPeek(STACK, void *, size_t);
int  StackIsEmpty(STACK);
long StackGetCount(STACK);
    
#endif
