// AWL - Anaconda Windows Library
// Memory

#ifndef AWL_MEM
#define AWL_MEM

#include <windows.h>

void *GetMem (DWORD size);
void FreeMem (void *punter);
void MemLog();

#endif