/* ------------------------------------------------------------------------ *\ 

   Name . . . . . : DLLIST.C
   
   Author . . . . : Paul Markham (3:711/934.1@fidonet)
   
   Date written . : September 1995
   
   ------------------------------------------------------------------------
   This code is released to the public domain. You may use and/or modified
   it in any way you choose.
  
   There is no warranty of any kind associated with this code. If you use
   it, you accept all responsibility for any loss or damages that may
   occur. So there!
   ------------------------------------------------------------------------

   Implementation for double linked list routines.
    
\* ------------------------------------------------------------------------ */

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <limits.h>
#include "dllist.h"

/* ------------------------------------------------------------------------ *\
   Defines
\* ------------------------------------------------------------------------ */

#define TRUE        1
#define FALSE       0

#define DLLIST_ID   "DLLIST"

#define VERIFY_LIST 0     /* define this as 1 to verify list after 
                             every update */

/* ------------------------------------------------------------------------ *\
   Structure definitions
\* ------------------------------------------------------------------------ */

typedef struct node
    {
    struct node *next;
    struct node *prev;
    size_t      dataLen;
    } node_t;

typedef struct DLList
    {
    char    id[sizeof(DLLIST_ID)];
    node_t  dummyHead;
    node_t  dummyTail;
    node_t  *current;
    long    count;
    } DLList_t;

/* ------------------------------------------------------------------------ *\
   Function prototypes
\* ------------------------------------------------------------------------ */

#if !defined(NDEBUG) && VERIFY_LIST
static void verifyList(DLLIST);
#endif

/* ------------------------------------------------------------------------ *\
   Create list
\* ------------------------------------------------------------------------ */
    
int DLListCreate(DLLIST *plist)
    {
    int rc;
    DLList_t *list;
        
    list = (DLList_t *)malloc(sizeof(DLList_t));
    if (list == NULL)
        rc = DLLIST_MALLOC_FAILED;
    else
        {
        rc = DLLIST_OK;
        strcpy(list->id, DLLIST_ID);    
        list->dummyHead.prev = NULL;
        list->dummyHead.next = &list->dummyTail;
        list->dummyTail.prev = &list->dummyHead;
        list->dummyTail.next = NULL;
        list->current = &list->dummyHead;
        list->count = 0;
        *plist = list;
        }
                 
    return rc;    
    }

/* ------------------------------------------------------------------------ *\
   Destroy list 
\* ------------------------------------------------------------------------ */

void DLListDestroy(DLLIST plist)
    {
    DLList_t    *list;
    node_t      *current;
    node_t      *next;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;
    current = list->dummyHead.next;
    while (current->next != NULL)
        {
        next = current->next;
        #if !defined(NDEBUG)
            memset(current, 0xAA, sizeof(node_t));
        #endif
        free(current);
        current = next;
        }

    list->id[0] = '\0';

    #if !defined(NDEBUG)
        memset(list, 0xAA, sizeof(DLList_t));
    #endif
    
    free(list);
    }        

/* ------------------------------------------------------------------------ *\
   Delete the current node
\* ------------------------------------------------------------------------ */

int DLListDelete(DLLIST plist)
    {
    int         rc;
    DLList_t    *list;
    node_t      *newCurrent;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t*)plist;  
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        {
        list->current->prev->next = list->current->next;
        list->current->next->prev = list->current->prev;
        
        /* 
        if this is is the last node before the dummy tail, then set the
        current pointer to the previous node else set it to the next node
        */
           
        if (list->current->next->next == NULL)
            newCurrent = list->current->prev;
        else
            newCurrent = list->current->next;
        #if !defined(NDEBUG)
            memset(list->current, 0xAA, sizeof(node_t));
        #endif
        free(list->current);
        list->current = newCurrent;
        list->count--;               
        rc = DLLIST_OK;
        }

    #if !defined(NDEBUG) && VERIFY_LIST
        verifyList(plist);
    #endif
        
    return rc;
    }
    
    

/* ------------------------------------------------------------------------ *\
   Update the current node. If the new data is longer then allocate a new
   node otherwise update the current one in place.
\* ------------------------------------------------------------------------ */

int DLListUpdate(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    DLList_t    *list;
    node_t      *newNode;
    size_t      len;
    char        *dataStart;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;  
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        {
        if (list->current->dataLen >= dataLen)
            {
            dataStart = (char *)list->current + sizeof(node_t);
            memcpy(dataStart, data, dataLen);
            }
        else
            {
            len = sizeof(node_t) + dataLen;
            newNode = (node_t *)malloc(len);
            if (newNode == NULL)
                rc = DLLIST_MALLOC_FAILED;
            else
                {
                newNode->dataLen = dataLen;
                dataStart = (char *)newNode + sizeof(node_t);
                memcpy(dataStart, data, dataLen);
                newNode->next = list->current->next;
                newNode->prev = list->current->prev;
                newNode->next->prev = newNode;
                newNode->prev->next = newNode;
                #if !defined(NDEBUG)
                    memset(list->current, 0xAA, sizeof(node_t));
                #endif
                free(list->current);
                list->current = newNode;                
                }
            }    
        rc = DLLIST_OK;
        }

    #if !defined(NDEBUG) && VERIFY_LIST
        verifyList(plist);
    #endif
        
    return rc;
    }
    
/* ------------------------------------------------------------------------ *\
   Insert a new node after the current one
\* ------------------------------------------------------------------------ */

int DLListInsertAfter(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    DLList_t    *list;
    node_t      *newNode;
    size_t      len;
    char        *dataStart;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    rc = DLLIST_OK;
    list = (DLList_t *)plist;
    
    len = dataLen + sizeof(node_t);
    assert(len > sizeof(node_t));

    newNode = (node_t *)malloc(len);
    if (newNode == NULL)
        rc = DLLIST_MALLOC_FAILED;
    else
        {
        newNode->dataLen = dataLen;
        dataStart = (char *)newNode + sizeof(node_t);
        memcpy(dataStart, data, dataLen);
        newNode->next = list->current->next;
        newNode->prev = list->current;
        list->current->next = newNode;
        newNode->next->prev = newNode;    
        list->current = newNode;    
        assert(list->count != LONG_MAX);
        list->count++;
        } 
           
    #if !defined(NDEBUG) && VERIFY_LIST
        verifyList(plist);
    #endif
        
    return rc;
    }    

/* ------------------------------------------------------------------------ *\
   Insert a new node before the current one
\* ------------------------------------------------------------------------ */

int DLListInsertBefore(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    DLList_t    *list;
    node_t      *newNode;
    size_t      len;
    char        *dataStart;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    rc = DLLIST_OK;
    list = (DLList_t *)plist;
    len = dataLen + sizeof(node_t);
    assert(len > sizeof(node_t));

    newNode = (node_t *)malloc(len);
    if (newNode == NULL)
        rc = DLLIST_MALLOC_FAILED;
    else
        {
        if (list->count == 0) 
            list->current = &list->dummyTail;
        newNode->dataLen = dataLen;
        dataStart = (char *)newNode + sizeof(node_t);
        memcpy(dataStart, data, dataLen);
        newNode->next = list->current;
        newNode->prev = list->current->prev;
        list->current->prev = newNode;
        newNode->prev->next = newNode;    
        list->current = newNode;    
        assert(list->count != LONG_MAX);
        list->count++;
        } 
           
    #if !defined(NDEBUG) && VERIFY_LIST
        verifyList(plist);
    #endif
        
    return rc;
    }    
        
/* ------------------------------------------------------------------------ *\
   Check if the list is empty
\* ------------------------------------------------------------------------ */

int DLListIsEmpty(DLLIST plist)
    {
    int         rc;
    DLList_t    *list;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    if (list->count == 0)
        rc = TRUE;
    else
        rc = FALSE;
        
    return rc;        
    }        

/* ------------------------------------------------------------------------ *\
   Return the data for the current node
\* ------------------------------------------------------------------------ */

int DLListGetCurrent(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    DLList_t    *list;
    char        *dataStart;
    size_t      len;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        {
        if (dataLen > list->current->dataLen)
            len = list->current->dataLen;
        else
            len = dataLen;
            
        dataStart = (char *)list->current + sizeof(node_t);    
        memcpy(data, dataStart, len);        
        rc = DLLIST_OK;
        }
    
    return rc;
    }
    
/* ------------------------------------------------------------------------ *\
   Return the data for the next node
\* ------------------------------------------------------------------------ */

int DLListGetNext(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    rc = DLListMoveNext(plist);
    if (rc == DLLIST_OK)
        rc = DLListGetCurrent(plist, data, dataLen);

    return rc;
    }
    
/* ------------------------------------------------------------------------ *\
   Return the data for the head node
\* ------------------------------------------------------------------------ */

int DLListGetHead(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    rc = DLListMoveHead(plist);
    if (rc == DLLIST_OK)
        rc = DLListGetCurrent(plist, data, dataLen);

    return rc;
    }
    
/* ------------------------------------------------------------------------ *\
   Return the data for the tail node
\* ------------------------------------------------------------------------ */

int DLListGetTail(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    rc = DLListMoveTail(plist);
    if (rc == DLLIST_OK)
        rc = DLListGetCurrent(plist, data, dataLen);

    return rc;
    }
    
/* ------------------------------------------------------------------------ *\
   Return the data for the previous node
\* ------------------------------------------------------------------------ */

int DLListGetPrev(DLLIST plist, void *data, size_t dataLen)
    {
    int         rc;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    rc = DLListMovePrev(plist);
    if (rc == DLLIST_OK)
        rc = DLListGetCurrent(plist, data, dataLen);

    return rc;
    }

/* ------------------------------------------------------------------------ *\
   Return the count of nodes
\* ------------------------------------------------------------------------ */

long DLListGetCount(DLLIST plist)
    {
    DLList_t    *list;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;
    
    return list->count;        
    }        

    
/* ------------------------------------------------------------------------ *\
   Move to the head node
\* ------------------------------------------------------------------------ */

int DLListMoveHead(DLLIST plist)
    {
    int         rc;
    DLList_t    *list;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        {
        list->current = list->dummyHead.next;
        rc = DLLIST_OK;
        }

    return rc;
    }    
    
/* ------------------------------------------------------------------------ *\
   Move to the tail node
\* ------------------------------------------------------------------------ */

int DLListMoveTail(DLLIST plist)
    {
    int         rc;
    DLList_t    *list;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        {
        list->current = list->dummyTail.prev;
        rc = DLLIST_OK;
        }

    return rc;
    }    
    
/* ------------------------------------------------------------------------ *\
   Move to the next node
\* ------------------------------------------------------------------------ */

int DLListMoveNext(DLLIST plist)
    {
    int         rc;
    DLList_t    *list;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        if (list->current->next->next == NULL)
            rc = DLLIST_END_OF_LIST;
        else
            {
            list->current = list->current->next;
            rc = DLLIST_OK;
            }
            
    return rc;
    }    

/* ------------------------------------------------------------------------ *\
   Move to the previous node
\* ------------------------------------------------------------------------ */

int DLListMovePrev(DLLIST plist)
    {
    int         rc;
    DLList_t    *list;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    if (list->count == 0)
        rc = DLLIST_EMPTY_LIST;
    else    
        if (list->current->prev->prev == NULL)
            rc = DLLIST_END_OF_LIST;
        else
            {
            list->current = list->current->prev;
            rc = DLLIST_OK;
            }

    return rc;
    }    

/* ------------------------------------------------------------------------ *\
   Verify the integrity of the list
\* ------------------------------------------------------------------------ */

#if !defined(NDEBUG) && VERIFY_LIST
static void verifyList(DLLIST plist)
    {
    DLList_t    *list;
    node_t      *current;
    long        count;
    
    assert(strcmp(((DLList_t *)plist)->id, DLLIST_ID) == 0);

    list = (DLList_t *)plist;    
    
    count = 0;
    current = list->dummyHead.next;
    while (current->next != NULL)
        {
        count++;
        current = current->next;
        }
    assert(count == list->count);
        
    if (list->count == 0)
        {
        assert(list->dummyHead.next == &list->dummyTail);
        assert(list->dummyHead.prev == NULL);
        assert(list->dummyTail.next == NULL);
        assert(list->dummyTail.prev == &list->dummyHead);
        assert(list->current == &list->dummyHead);
        }
    else
        {
        assert(list->dummyHead.prev == NULL);
        assert(list->dummyTail.next == NULL);
        current = &list->dummyHead;
        while(current != NULL)
            {
            if (current->next != NULL)
                assert(current->next->prev == current);
            if (current->prev != NULL)
                assert(current->prev->next == current);
            current = current->next;
            }
        }    
    }    
#endif    
