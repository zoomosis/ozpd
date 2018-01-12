/*
 *  DLIST.H - Doubly linked-list management functions.
 *  Adapted from 1995 public domain C code by Scott Pitcher.
 *  Written 1995,96 by Andrew Clarke and released to the public domain.
 */

#ifndef __DLIST_H__
#define __DLIST_H__

typedef struct dlistnode_t {
    struct dlistnode_t *L_prev;
    struct dlistnode_t *L_next;
    void *L_element;
} DLISTNODE_T;

typedef struct {
    DLISTNODE_T *L_first;
    DLISTNODE_T *L_last;
    unsigned long L_elements;
} DLIST_T;

DLIST_T *dlistCreate(void);
void dlistDestroy(DLIST_T * p_list);
void *dlistGetElement(DLISTNODE_T * p_node);
void dlistSetElement(DLISTNODE_T * p_node, void *p_element);
DLISTNODE_T *dlistCreateNode(void *p_element);
void dlistDeleteNode(DLISTNODE_T * p_node);
void dlistAddNode(DLIST_T * p_list, DLISTNODE_T * p_node);
void dlistDropNode(DLIST_T * p_list, DLISTNODE_T * p_node);

DLISTNODE_T *dlistTravFirst(DLIST_T * p_list);
DLISTNODE_T *dlistTravLast(DLIST_T * p_list);
DLISTNODE_T *dlistTravPrevious(DLISTNODE_T * p_node);
DLISTNODE_T *dlistTravNext(DLISTNODE_T * p_node);

int dlistCompareNodes(DLISTNODE_T * p_node1, DLISTNODE_T * p_node2, int (*fcmp) (const void *, const void *));
void dlistSwapNodes(DLISTNODE_T * p_node1, DLISTNODE_T * p_node2);
DLISTNODE_T *dlistSearch(DLIST_T * p_list, void *p_element, int (*fcmp) (const void *, const void *));
unsigned long dlistTotalNodes(DLIST_T * p_list);
int dlistIsEmpty(DLIST_T * p_list);

#endif

