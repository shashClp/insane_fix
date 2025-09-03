// AWL - Anaconda Windows Library
// ListZ

#ifndef AWL_LISTZ
#define AWL_LISTZ

#include <windows.h>

typedef struct _TListZItem {
  _TListZItem *prev;
  _TListZItem *next;
  DWORD z;
} TListZItem;

typedef struct {
  DWORD count;
  TListZItem *first;
  TListZItem *last;
} TListZ;

void LISTZ_Init (TListZ &a);
void LISTZ_Free (TListZ &a);
void LISTZ_Add (TListZ &a,void *Value,DWORD Size,DWORD Z);
void LISTZ_Delete (TListZ &a,TListZItem *b);
TListZItem *LISTZ_NewItem (DWORD Size);
void LISTZ_FillValue(TListZItem *l,void *Value,DWORD Size);

#endif
