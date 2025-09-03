// AWL - Anaconda Windows Library
// Stack

#ifndef AWL_STACK
#define AWL_STACK

#include <windows.h>

typedef struct _TStackItem {
  _TStackItem *prev;
	_TStackItem *next;
} TStackItem;

typedef struct {
  DWORD count;
  TStackItem *first;
  TStackItem *last;
} TStack;

void        STACK_Init     (TStack &a);
void        STACK_Free     (TStack &a);
void        STACK_Push     (TStack &a,void *Value,DWORD Size);
void        STACK_Pop      (TStack &a,void *Value,DWORD Size);
bool        STACK_Empty    (TStack &a);
TStackItem* STACK_NewItem  (DWORD Size);
void        STACK_FillValue(TStackItem *l,void *Value,DWORD Size);

#endif
