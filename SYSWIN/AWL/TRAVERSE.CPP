// AWL - Anaconda Windows Library
// Traverse

#include <traverse.h>

void LIST_GetTraverse (TTraverse &tl,TList &a)
{
  if (a.first)
  {
#ifdef TRAVERSE_PREV
    tl.prev = NULL;
#endif
    tl.current = a.first;
    tl.next = tl.current->next;
  } else {
#ifdef TRAVERSE_PREV
    tl.prev = NULL;
#endif
    tl.next = NULL;
    tl.current = NULL;
  }
}

void *LIST_GetFromItem (TListItem *a)
{
  return (void *)(((DWORD) a)+sizeof(TListItem));
}

void *LIST_Get (TTraverse &a)
{
  return (void *)(((DWORD) a.current)+sizeof(TListItem));
}

TListItem *LIST_GetItem (TTraverse &a)
{
  return a.current;
}

void LIST_Next (TTraverse &a)
{
  if (a.next)
  {
#ifdef TRAVERSE_PREV
    a.prev = a.current;
#endif
    a.current = a.next;
    a.next = a.current->next;
  }
}

#ifdef TRAVERSE_PREV
  void LIST_Prev (TTraverse &a)
  {
    if (a.prev)
    {
      a.next = a.current;
      a.current = a.prior;
      a.prev = a.current->prior;
    }
  }

  bool LIST_Bol (TTraverse &a)
  {
    return (a.prior == NULL);
  }
#endif

bool LIST_Eol (TTraverse &a)
{
  return (a.next == NULL);
}
