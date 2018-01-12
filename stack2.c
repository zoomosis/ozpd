/*
 *  STACK.C - Stack management functions.
 *  Written 1995,96 by Andrew Clarke and released to the public domain.
 */

#include <stdlib.h>
#include "stack2.h"
#include "unused.h"

void stackDefaults(STACK * p_stack)
{
    p_stack->p_top = NULL;
    p_stack->items = 0L;
}

void stackInit(STACK * p_stack)
{
    unused(p_stack);
}

void stackTerm(STACK * p_stack)
{
    while (!stackIsEmpty(p_stack)) {
        stackPopItem(p_stack);
    }
}

int stackPushItem(STACK * p_stack, void *p_item)
{
    STACKNODE *p_node;
    p_node = malloc(sizeof(STACKNODE));
    if (!p_node)
        return 0;
    p_node->p_item = p_item;
    if (!p_stack->p_top)
        p_node->p_prev = NULL;
    else
        p_node->p_prev = p_stack->p_top;
    p_stack->p_top = p_node;
    p_stack->items++;
    return 1;
}

void *stackPopItem(STACK * p_stack)
{
    void *p_item;
    STACKNODE *p_node;
    if (!p_stack->items)
        return NULL;
    p_item = p_stack->p_top->p_item;
    p_node = p_stack->p_top;
    p_stack->p_top = p_stack->p_top->p_prev;
    free(p_node);
    p_stack->items--;
    return p_item;
}

unsigned long stackTotalItems(STACK * p_stack)
{
    return p_stack->items;
}

int stackIsEmpty(STACK * p_stack)
{
    return p_stack->items == 0;
}

#ifdef TEST_STACK

#include <stdio.h>

int main(void)
{
    STACK p_stack;
    char *str;

    stackDefaults(&p_stack);
    stackInit(&p_stack);
    stackPushItem(&p_stack, "One banana");
    stackPushItem(&p_stack, "Two banana");
    stackPushItem(&p_stack, "Three banana");
    stackPushItem(&p_stack, "Four banana");
    stackPushItem(&p_stack, "Five banana");

    str = stackPopItem(&p_stack);
    while (str) {
        printf("%s\n", str);
        str = stackPopItem(&p_stack);
    }
    stackTerm(&p_stack);

    return 0;
}

#endif

