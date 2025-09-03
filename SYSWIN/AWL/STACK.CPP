// AWL - Anaconda Windows Library
// Stack

#include <stack.h>
#include <mem.h>

void STACK_Init (TStack &a)
{
  a.count = 0;
  a.first = NULL;
  a.last = NULL;
}

void STACK_Free (TStack &a)
{
  while (a.first) {
    TStackItem *l = a.first;
    a.first = a.first->next;
    FreeMem (l);
  }
  a.first = NULL;
  a.last = NULL;
  a.count = 0;
}

void STACK_Push (TStack &a,void *Value,DWORD Size)
{
  TStackItem *l = STACK_NewItem (Size);
  STACK_FillValue (l,Value,Size);
  l->next = NULL;
  if (a.last == NULL) {
    l->prev = NULL;
    a.first = a.last = l;
  } else {
    l->prev = a.last;
    a.last->next = l;
    a.last = l;
  }
	a.count++;
}

void STACK_Pop (TStack &a,void *Value,DWORD Size)
{
  if (!a.last) return;

  memcpy(Value,(void *)(((DWORD) a.last)+sizeof(TStackItem)),Size);

	TStackItem *tmp = a.last;
	a.last = a.last->prev;
	FreeMem(tmp);
  a.count--;
	if (a.count==0) a.first = NULL;
	tmp=NULL;
}

bool STACK_Empty (TStack &a)
{
	if (a.count > 0) return false;
	else return true;
}

void STACK_FillValue (TStackItem *l,void *Value,DWORD Size)
{
  memcpy((void *)(((DWORD) l)+sizeof(TStackItem)),Value,Size);
}

TStackItem* STACK_NewItem (DWORD Size)
{
  return (TStackItem *) GetMem (Size+sizeof(TStackItem));
}

