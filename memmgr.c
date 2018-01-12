/*********************************************************************/
/*                                                                   */
/*  This Program Written By Paul Edwards.                            */
/*  Released to the public domain.                                   */
/*                                                                   */
/*********************************************************************/
/*********************************************************************/
/*                                                                   */
/*  memmgr - manage memory                                           */
/*                                                                   */
/*********************************************************************/

#include "memmgr.h"

void memmgrDefaults(MEMMGR *memmgr)
{
    return;
}

void memmgrInit(MEMMGR *memmgr)
{
    memmgr->start = NULL;
    return;
}

void memmgrTerm(MEMMGR *memmgr)
{
    return;
}

void memmgrSupply(MEMMGR *memmgr, void *buffer, size_t szbuf)
{
    MEMMGRN *p, *l, *b;

    if (((int)buffer % MEMMGR_ALIGN) != 0)
    {
        szbuf -= (MEMMGR_ALIGN - (int)buffer % MEMMGR_ALIGN);
        buffer = (char *)buffer + (MEMMGR_ALIGN - (int)buffer % MEMMGR_ALIGN);
    }
    
    if ((szbuf % MEMMGR_ALIGN) != 0)
    {
        szbuf -= szbuf % MEMMGR_ALIGN;
    }
    
    p = memmgr->start;
    l = NULL;
    while ((p != NULL) && ((MEMMGRN *)buffer >= p))
    {
        l = p;
        p = p->next;
    }
        
    b = (MEMMGRN *)buffer;
        
    b->prev = l;
    b->next = p;
        
    if (l != NULL)
    {
        l->next = b;
    }
    else
    {
        memmgr->start = b;
    }
        
    if (p != NULL)
    {
        p->prev = b;
    }
        
    b->fixed = 1;
    b->size = szbuf - MEMMGRN_SZ;
    b->allocated = 0;
    return;
}

void *memmgrAllocate(MEMMGR *memmgr, size_t bytes, int id)
{
    MEMMGRN *p, *n;
    
    if ((bytes % MEMMGR_ALIGN) != 0)
    {
        bytes += (MEMMGR_ALIGN - bytes % MEMMGR_ALIGN);
    }
    
    p = memmgr->start;
    
    while (p != NULL)
    {
        if ((p->size >= bytes) && !p->allocated)
        {
            if ((p->size - bytes) > MEMMGRN_SZ)
            {
                n = (MEMMGRN *)((char *)p + MEMMGRN_SZ + bytes);
                n->next = p->next;
                n->prev = p;
                p->next = n;
                n->fixed = 0;
                n->size = p->size - bytes - MEMMGRN_SZ;
                n->allocated = 0;
                p->size = bytes;
            }
            p->allocated = 1;
            p->id = id;
            break;
        }
        p = p->next;
    }
    if (p == NULL)
    {
        return (p);
    }
    else
    {
        return ((char *)p + MEMMGRN_SZ);
    }
}

void memmgrFree(MEMMGR *memmgr, void *ptr)
{
    MEMMGRN *p, *n, *l;

    ptr = (char *)ptr - MEMMGRN_SZ;
    
    p = memmgr->start;
    l = NULL;
    
    while (p != NULL)
    {
        if (p == ptr)
        {
            p->allocated = 0;
            
            n = p->next;
            if ((n != NULL) && !n->allocated && !n->fixed)
            {
                p->size += n->size + MEMMGRN_SZ;
                p->next = n->next;
            }
            
            if (!p->fixed && (l != NULL) && !l->allocated)
            {
                l->size += p->size + MEMMGRN_SZ;
                l->next = p->next;
            }
            break;
        }
        l = p;
        p = p->next;
    }
    return;
}

void memmgrFreeId(MEMMGR *memmgr, int id)
{
    MEMMGRN *p, *n, *l;

    p = memmgr->start;
    l = NULL;
    
    while (p != NULL)
    {
        if ((p->id == id) && p->allocated)
        {
            p->allocated = 0;
            
            n = p->next;
            if ((n != NULL) && !n->allocated && !n->fixed)
            {
                p->size += n->size + MEMMGRN_SZ;
                p->next = n->next;
            }
            
            if (!p->fixed && (l != NULL) && !l->allocated)
            {
                l->size += p->size + MEMMGRN_SZ;
                l->next = p->next;
            }
        }
        l = p;
        p = p->next;
    }
    return;
}
