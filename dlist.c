/*
 *  DLIST.H - Doubly linked-list management functions.
 *  Adapted from 1995 public domain C code by Scott Pitcher.
 *  Written 1995,96 by Andrew Clarke and released to the public domain.
 */

#include <stdlib.h>
#include "dlist.h"

DLIST_T *dlistCreate(void)
{
    DLIST_T *p_list;
    p_list = malloc(sizeof(DLIST_T));
    if (p_list) {
        p_list->L_first = NULL;
        p_list->L_last = NULL;
        p_list->L_elements = 0L;
    }
    return p_list;
}

void dlistDestroy(DLIST_T * p_list)
{
    DLISTNODE_T *p_node;
    if (p_list) {
        if (p_list->L_elements) {
            p_node = p_list->L_first;
            while (p_node) {
                p_list->L_first = p_node->L_next;
                free(p_node);
                p_node = p_list->L_first;
            }
        }
        free(p_list);
    }
}

void *dlistGetElement(DLISTNODE_T * p_node)
{
    if (p_node)
        return p_node->L_element;
    else
        return NULL;
}

void dlistSetElement(DLISTNODE_T * p_node, void *p_element)
{
    if (p_node)
        p_node->L_element = p_element;
}

DLISTNODE_T *dlistCreateNode(void *p_element)
{
    DLISTNODE_T *p_node;
    p_node = malloc(sizeof(DLISTNODE_T));
    if (p_node) {
        p_node->L_next = NULL;
        p_node->L_prev = NULL;
        p_node->L_element = p_element;
    }
    return p_node;
}

void dlistDeleteNode(DLISTNODE_T * p_node)
{
    if (p_node)
        free(p_node);
}

void dlistAddNode(DLIST_T * p_list, DLISTNODE_T * p_node)
{
    if (p_list) {
        p_node->L_prev = p_list->L_last;
        if (p_node->L_prev)
            p_list->L_last->L_next = p_node;
        p_node->L_next = NULL;
        if (!p_list->L_first)
            p_list->L_first = p_node;
        p_list->L_last = p_node;
        p_list->L_elements++;
    }
}

void dlistDropNode(DLIST_T * p_list, DLISTNODE_T * p_node)
{
    DLISTNODE_T *poldnext;

    poldnext = p_node->L_next;  /* SVP 15-Apr-95 bug fix */
    if (p_list) {
        if (p_list->L_first == p_node)
            p_list->L_first = p_node->L_next;
        if (p_list->L_last == p_node)
            p_list->L_last = p_node->L_prev;
        if (p_node->L_next) {
            (p_node->L_next)->L_prev = p_node->L_prev;
            p_node->L_next = NULL;
        }
        if (p_node->L_prev) {
            p_node->L_prev->L_next = poldnext;  /* SVP 15-Apr-95 bug fix */
            p_node->L_prev = NULL;
        }
        p_list->L_elements--;
    }
}

DLISTNODE_T *dlistTravFirst(DLIST_T * p_list)
{
    if (p_list)
        return p_list->L_first;
    else
        return NULL;
}

DLISTNODE_T *dlistTravLast(DLIST_T * p_list)
{
    if (p_list)
        return p_list->L_last;
    else
        return NULL;
}

DLISTNODE_T *dlistTravPrevious(DLISTNODE_T * p_node)
{
    if (p_node)
        return p_node->L_prev;
    else
        return NULL;
}

DLISTNODE_T *dlistTravNext(DLISTNODE_T * p_node)
{
    if (p_node)
        return p_node->L_next;
    else
        return NULL;
}

int dlistCompareNodes(DLISTNODE_T * p_node1, DLISTNODE_T * p_node2, int (*fcmp) (const void *, const void *))
{
    return fcmp(p_node1->L_element, p_node2->L_element);
}

void dlistSwapNodes(DLISTNODE_T * p_node1, DLISTNODE_T * p_node2)
{
    DLISTNODE_T *p_temp;
    if (p_node1 && p_node2) {
        p_temp = p_node1->L_next;
        p_node1->L_next = p_node2->L_next;
        p_node2->L_next = p_temp;
        p_temp = p_node1->L_prev;
        p_node1->L_prev = p_node2->L_prev;
        p_node2->L_prev = p_temp;
    }
}

DLISTNODE_T *dlistSearch(DLIST_T * p_list, void *p_element, int (*fcmp) (const void *, const void *))
{
    DLISTNODE_T *p_node;
    if (p_list && p_list->L_elements) {
        p_node = p_list->L_first;
        while (p_node) {
            if (!fcmp(p_node->L_element, p_element))
                return p_node;
            else
                p_node = p_node->L_next;
        }
    }
    return NULL;
}

unsigned long dlistTotalNodes(DLIST_T * p_list)
{
    if (p_list)
        return p_list->L_elements;
    else
        return 0;
}

int dlistIsEmpty(DLIST_T * p_list)
{
    if (dlistTotalNodes(p_list) != 0)
        return 0;
    else
        return 1;
}

#ifdef TEST_DLIST

#include <stdio.h>

int main(void)
{
    DLIST_T *p_list;
    DLISTNODE_T *p_node;

    p_list = dlistCreate();

    p_node = dlistCreateNode("One banana");
    dlistAddNode(p_list, p_node);
    p_node = dlistCreateNode("Two banana");
    dlistAddNode(p_list, p_node);
    p_node = dlistCreateNode("Three banana");
    dlistAddNode(p_list, p_node);
    p_node = dlistCreateNode("Four banana");
    dlistAddNode(p_list, p_node);
    p_node = dlistCreateNode("Five banana");
    dlistAddNode(p_list, p_node);

    p_node = dlistTravFirst(p_list);
    while(p_node) {
        printf("%s\n", (char *) dlistGetElement(p_node));
        p_node = dlistTravNext(p_node);
    }

    dlistDestroy(p_list);
    return 0;
}

#endif

