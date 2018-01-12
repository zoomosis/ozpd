/* ------------------------------------------------------------------------ *\ 

   Name . . . . . : DLLIST.H
   
   Author . . . . : Paul Markham (3:711/934.1@fidonet)
   
   Date written . : September 1995
   
   ------------------------------------------------------------------------
   This code is released to the public domain. You may use and/or modified
   it in any way you choose.
  
   There is no warranty of any kind associated with this code. If you use
   it, you accept all responsibility for any loss or damages that may
   occur. So there!
   ------------------------------------------------------------------------

   Header for double linked list routines.

   ------------------------------------------------------------------------

   Functions
   =========
       
       int  DLListCreate(DLLIST *);
       
           description: create a doubly linked list
           parameters:  1. Address of a variable of type DLLIST. This will be 
                           initialised to contain a handle that must be passed
                           to all other functions.
           returns:     DLList return code
       
       
       void DLListDestroy(DLLIST);

           description: Destroy the list and free all memory
           parameters:  1. handle to a double linked list 
           returns:     none
       
       int  DLListDelete(DLLIST);

           description: Delete the current node from the list
           parameters:  1. handle to a double linked list 
           returns:     DLList return code
       
       int  DLListUpdate(DLLIST, void *, size_t);

           description: Update the current node with new data
           parameters:  1. handle to a double linked list 
                        2. pointer to the data
                        3. size of data
           returns:     DLList return code
       
       int  DLListInsertAfter(DLLIST, void *, size_t);

           description: Insert a new node after the current one
           parameters:  1. handle to a double linked list 
                        2. pointer to the data
                        3. size of the data
           returns:     DLList return code
       
       int  DLListInsertBefore(DLLIST, void *, size_t);

           description: Insert a new node before the current one
           parameters:  1. handle to a double linked list 
                        2. pointer to the data
                        3. size of the data
           returns:     DLList return code
       
       int  DLListIsEmpty(DLLIST);

           description: Determine if the list is empty
           parameters:  1. handle to a double linked list 
           returns:     zero if the list is empty, non-zero otherwise
       
       int  DLListMoveHead(DLLIST);

           description: Move to the head of the list
           parameters:  1. handle to a double linked list 
           returns:     DLList return code
       
       int  DLListMoveTail(DLLIST);

           description: Move to the tail of the list
           parameters:  1. handle to a double linked list 
           returns:     DLList return code
       
       int  DLListMoveNext(DLLIST);

           description: Move to the next node
           parameters:  1. handle to a double linked list 
           returns:     DLList return code
       
       int  DLListMovePrev(DLLIST);

           description: Move to the previous node
           parameters:  1. handle to a double linked list 
           returns:     DLList return code
       
       int  DLListGetCurrent(DLLIST, void *, size_t);

           description: Retrieve the current node
           parameters:  1. handle to a double linked list 
                        2. pointer to a buffer to return the data in
                        3. maximum size of the data to retrieve. The amount of
                           data returned is the minimum of this size and the
                           size of the data stored in the node. 
           returns:     DLList return code
       
       int  DLListGetNext(DLLIST, void *, size_t);

           description: Move to the next node and retrieve the data
           parameters:  1. handle to a double linked list 
                        2. pointer to a buffer to return the data in
                        3. maximum size of the data to retrieve. The amount of
                           data returned is the minimum of this size and the
                           size of the data stored in the node. 
           returns:     DLList return code
       
       int  DLListGetPrev(DLLIST, void *, size_t);

           description: Move to the previous node and retrieve the data
           parameters:  1. handle to a double linked list 
                        2. pointer to a buffer to return the data in
                        3. maximum size of the data to retrieve. The amount of
                           data returned is the minimum of this size and the
                           size of the data stored in the node. 
           returns:     DLList return code
       
       int  DLListGetHead(DLLIST, void *, size_t);

           description: Move to the head of the list and retrieve the data
           parameters:  1. handle to a double linked list 
                        2. pointer to a buffer to return the data in
                        3. maximum size of the data to retrieve. The amount of
                           data returned is the minimum of this size and the
                           size of the data stored in the node. 
           returns:     DLList return code
       
       int  DLListGetTail(DLLIST, void *, size_t);

           description: Move to the tail of the list and retrieve the data
           parameters:  1. handle to a double linked list 
                        2. pointer to a buffer to return the data in
                        3. maximum size of the data to retrieve. The amount of
                           data returned is the minimum of this size and the
                           size of the data stored in the node. 
           returns:     DLList return code
       
       long DLListGetCount(DLLIST);

           description: return the count of nodes in the list
           parameters:  1. handle to a double linked list 
           returns:     count of nodes

   Example
   =======

       {
       int     rc;
       DLLIST  d;
    
       DLListCreate(&d);
       DLListInsertAfter(d, "One", 10);
       DLListInsertAfter(d, "Two", 10);
       DLListInsertBefore(d, "Three", 10);
       DLListMoveHead(d);
       DLListUpdate(d, "Four", 10);
       rc = DLListGetHead(d, buff, sizeof(buff));
       while(rc == DLLIST_OK)
           {
           printf("%s\n", buff);
           rc = DLListGetNext(d, buff, sizeof(buff));
           }                
       DLListDestroy(d);
       }
       
\* ------------------------------------------------------------------------ */

#if !defined (__DLLIST_H)
#define __DLLIST_H

/* ------------------------------------------------------------------------ *\
   Typedefs
\* ------------------------------------------------------------------------ */

typedef void * DLLIST;

/* ------------------------------------------------------------------------ *\
   Return codes
\* ------------------------------------------------------------------------ */

enum 
    {
    DLLIST_OK,
    DLLIST_MALLOC_FAILED,
    DLLIST_EMPTY_LIST,
    DLLIST_END_OF_LIST,
    };

/* ------------------------------------------------------------------------ *\
   Function prototypes
\* ------------------------------------------------------------------------ */

int  DLListCreate(DLLIST *);
void DLListDestroy(DLLIST);
int  DLListDelete(DLLIST);
int  DLListUpdate(DLLIST, void *, size_t);
int  DLListInsertAfter(DLLIST, void *, size_t);
int  DLListInsertBefore(DLLIST, void *, size_t);
int  DLListIsEmpty(DLLIST);
int  DLListMoveHead(DLLIST);
int  DLListMoveTail(DLLIST);
int  DLListMoveNext(DLLIST);
int  DLListMovePrev(DLLIST);
int  DLListGetCurrent(DLLIST, void *, size_t);
int  DLListGetNext(DLLIST, void *, size_t);
int  DLListGetPrev(DLLIST, void *, size_t);
int  DLListGetHead(DLLIST, void *, size_t);
int  DLListGetTail(DLLIST, void *, size_t);
long DLListGetCount(DLLIST);

#endif
