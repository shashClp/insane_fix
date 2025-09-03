#ifndef AWL_TRAVERSEZ
#define AWL_TRAVERSEZ

//#define TRAVERSEZ_PREV

#include <windows.h>
#include <listz.h>

typedef struct {
  TListZItem *prev;
  TListZItem *current;
  TListZItem *next;
} TTraverseZ;

void *LISTZ_Get (TTraverseZ &a);
void *LISTZ_GetFromItem (TListZItem *a);
void LISTZ_Next (TTraverseZ &a);
void LISTZ_GetTraverse (TTraverseZ &tl,TListZ &a);
bool LISTZ_Eol (TTraverseZ &a);
TListZItem *LISTZ_GetItem (TTraverseZ &a);

#ifdef TRAVERSEZ_PRIOR
  void LISTZ_Prev (TTraverseZ &a);
  bool LISTZ_Bol (TTraverseZ &a);
#endif

#endif
