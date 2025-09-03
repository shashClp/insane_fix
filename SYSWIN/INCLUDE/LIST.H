// AWL - Anaconda Windows Library
// List

#ifndef AWL_LIST
#define AWL_LIST

#include <windows.h>

typedef struct _TListItem {
  _TListItem *prev;
  _TListItem *next;
} TListItem;

typedef struct {
  DWORD count;
  TListItem *first;
  TListItem *last;
} TList;

void LIST_Init (TList &a);
void LIST_Free (TList &a);
void LIST_Add (TList &a,void *Value,DWORD Size);
void LIST_Delete (TList &a,TListItem *b);
void LIST_Insert (TList &a,TListItem *b,void *Value,DWORD Size);
TListItem *LIST_NewItem (DWORD Size);
void LIST_FillValue(TListItem *l,void *Value,DWORD Size);

#endif
