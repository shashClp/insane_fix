#ifndef AWL_TRAVERSE
#define AWL_TRAVERSE

//#define TRAVERSE_PREV

#include <windows.h>
#include <list.h>

typedef struct {
  TListItem *prev;
  TListItem *current;
  TListItem *next;
} TTraverse;

void *LIST_Get (TTraverse &a);
void *LIST_GetFromItem (TListItem *a);
void LIST_Next (TTraverse &a);
void LIST_GetTraverse (TTraverse &tl,TList &a);
bool LIST_Eol (TTraverse &a);
TListItem *LIST_GetItem (TTraverse &a);

#ifdef TRAVERSE_PRIOR
  void LIST_Prev (TTraverse &a);
  bool LIST_Bol (TTraverse &a);
#endif

#endif
