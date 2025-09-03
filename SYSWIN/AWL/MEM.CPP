// AWL - Anaconda Windows Library
// Memory

#include <windows.h>
#include <log.h>
#include <malloc.h>

DWORD TotalMemAllocated=0;
DWORD MemNotFreed=0;
DWORD MaxMemAllocated=0;
DWORD TotalNumPointers=0;
DWORD NotFreedNumPointers=0;
DWORD MaxNumPointers=0;

void MemLog()
{
  LOG ("Total Mem Allocated [%4i pointers]= %8i bytes",TotalNumPointers,TotalMemAllocated);
  LOG ("Max Mem Allocated   [%4i pointers]= %8i bytes",NotFreedNumPointers,MaxMemAllocated);
  LOG ("Mem Not Freed       [%4i pointers]= %8i bytes",MaxNumPointers,MemNotFreed);
}

void *GetMem (DWORD size)
{
  if (size==0)
  {
    ERR("GetMem with size=0");
    return NULL;
  }
  void *punter = malloc (size);
  if (punter==NULL)
  {
    ERR("Getting mem for size %i",size);
    return NULL;
  }
  DWORD t = _msize (punter);
  TotalMemAllocated+=t;
  MemNotFreed+=t;
  if (MemNotFreed>MaxMemAllocated) MaxMemAllocated=MemNotFreed;

  TotalNumPointers++;
  NotFreedNumPointers++;
  if (NotFreedNumPointers>MaxNumPointers) MaxNumPointers=NotFreedNumPointers;

  return punter;
}

void FreeMem (void *punter)
{
  if (punter==NULL)
  {
    ERR("Freeing NULL pointer?");
    return;
  }
  DWORD t = _msize (punter);
  MemNotFreed-=t;
  NotFreedNumPointers--;
  free(punter);
}
