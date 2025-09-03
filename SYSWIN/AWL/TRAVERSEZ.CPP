// AWL - Anaconda Windows Library
// Traverse

#include <traverseZ.h>

void LISTZ_GetTraverse (TTraverseZ &tl,TListZ &a)
{
  if (a.first)
  {
#ifdef TRAVERSEZ_PREV
    tl.prev = NULL;
#endif
    tl.current = a.first;
    tl.next = tl.current->next;
  } else {
#ifdef TRAVERSEZ_PREV
    tl.prev = NULL;
#endif
    tl.next = NULL;
    tl.current = NULL;
  }
}

void *LISTZ_GetFromItem (TListZItem *a)
{
  return (void *)(((DWORD) a)+sizeof(TListZItem));
}

void *LISTZ_Get (TTraverseZ &a)
{
  return (void *)(((DWORD) a.current)+sizeof(TListZItem));
}

TListZItem *LISTZ_GetItem (TTraverseZ &a)
{
  return a.current;
}

void LISTZ_Next (TTraverseZ &a)
{
  if (a.next)
  {
#ifdef TRAVERSEZ_PREV
    a.prev = a.current;
#endif
    a.current = a.next;
    a.next = a.current->next;
  }
}

#ifdef TRAVERSEZ_PREV
  void LISTZ_Prev (TTraverseZ &a)
  {
    if (a.prev)
    {
      a.next = a.current;
      a.current = a.prior;
      a.prev = a.current->prior;
    }
  }

  bool LISTZ_Bol (TTraverseZ &a)
  {
    return (a.prior == NULL);
  }
#endif

bool LISTZ_Eol (TTraverseZ &a)
{
  return (a.next == NULL);
}
