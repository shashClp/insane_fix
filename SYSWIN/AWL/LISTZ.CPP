// AWL - Anaconda Windows Library
// ListZ

#include <listZ.h>
#include <mem.h>

void LISTZ_Init (TListZ &a)
{
  a.count = 0;
  a.first = NULL;
  a.last = NULL;
}

void LISTZ_Free (TListZ &a)
{
  while (a.first)
  {
    TListZItem *l = a.first;
    a.first = a.first->next;
    FreeMem (l);
  }
  a.first = NULL;
  a.last = NULL;
  a.count = 0;
}

void LISTZ_Add (TListZ &a,void *Value,DWORD Size,DWORD Z)
{
  TListZItem *l = LISTZ_NewItem (Size);
  LISTZ_FillValue (l,Value,Size);
  l->z = Z;    
  if (a.last == NULL)
  {
    l->prev = NULL;
    l->next = NULL;
    a.first = a.last = l;
  } else {
    TListZItem *t,*ta;
    ta = t = a.first;
    while ((t!=NULL) && (l->z>t->z))
    {
      ta = t;
      t = t->next;
    }
    if (t==NULL)
    {
      ta->next = l;
      l->prev = ta;
      l->next = NULL;
      a.last = l;
    } else {      
      l->next = t;
      t->prev = l;
      if (t==a.first)
      {
        a.first = l;
        l->prev = NULL;
      } else {
        ta->next = l;
        l->prev = ta;
      }
    }
  }
  a.count++;
}

void LISTZ_Delete (TListZ &a,TListZItem *b)
{
  if (a.first==b)
  {
    if (a.last==b) { a.last = a.first = NULL; }
    else {
      a.first = b->next;
      b->next->prev = NULL;
    }
  } else {
    TListZItem *pr = b->prev;
    TListZItem *nx = b->next;
    if (nx) nx->prev = pr;
    pr->next = nx;
    if (a.last==b) a.last = pr;
  }
  FreeMem (b);
  a.count--;
}

void LISTZ_FillValue (TListZItem *l,void *Value,DWORD Size)
{
  memcpy((void *)(((DWORD) l)+sizeof(TListZItem)),Value,Size);
}

TListZItem* LISTZ_NewItem (DWORD Size)
{
  return (TListZItem *) GetMem (Size+sizeof(TListZItem));
}

