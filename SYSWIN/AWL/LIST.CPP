// AWL - Anaconda Windows Library
// List

#include <list.h>
#include <mem.h>

void LIST_Init (TList &a)
{
  a.count = 0;
  a.first = NULL;
  a.last = NULL;
}

void LIST_Free (TList &a)
{
  while (a.first)
  {
    TListItem *l = a.first;
    a.first = a.first->next;
    FreeMem (l);
  }
  a.first = NULL;
  a.last = NULL;
  a.count = 0;
}

void LIST_Add (TList &a,void *Value,DWORD Size)
{
  TListItem *l = LIST_NewItem (Size);
  LIST_FillValue (l,Value,Size);
  l->next = NULL;
  if (a.last == NULL)
  {
    l->prev = NULL;
    a.first = a.last = l;
  } else {
    l->prev = a.last;
    a.last->next = l;
    a.last = l;
  }
  a.count++;
}

void LIST_Delete (TList &a,TListItem *b)
{
  if (a.first==b)
  {
    if (a.last==b) { a.last = a.first = NULL; }
    else {
      a.first = b->next;
      b->next->prev = NULL;
    }
  } else {
    TListItem *pr = b->prev;
    TListItem *nx = b->next;
    if (nx) nx->prev = pr;
    pr->next = nx;
    if (a.last==b) a.last = pr;
  }
  FreeMem (b);
  a.count--;
}

void LIST_Insert (TList &a,TListItem *b,void *Value,DWORD Size)
{
  TListItem *l = LIST_NewItem (Size);
  LIST_FillValue (l,Value,Size);

  if (a.first==b)
  {
    l->prev = NULL;
    l->next = b;
    b->prev = l;
    a.first = l;
  } else {
    b->prev->next = l;
    l->prev = b->prev;
    b->prev = l;
    l->next = b;
  }
  a.count++;
}


void LIST_FillValue (TListItem *l,void *Value,DWORD Size)
{
  memcpy((void *)(((DWORD) l)+sizeof(TListItem)),Value,Size);
}

TListItem* LIST_NewItem (DWORD Size)
{
  return (TListItem *) GetMem (Size+sizeof(TListItem));
}

