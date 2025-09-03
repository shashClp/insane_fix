// AWL - Anaconda Windows Library
// Miscelaneous routines

#ifndef AWL_MISC_FUNCTIONS
#define AWL_MISC_FUNCTIONS

#include <ddraw.h>

extern HRESULT hr;

#ifdef _VC5
  #define rdtsc _asm _emmit 0xf _asm _emmit 0x31
  #define cpuid _asm _emmit 0xf _asm _emmit 0xa2
#endif

inline _int64 MISC_GetTickCount()
{
  _asm rdtsc;
}

char *MISC_GetFileExtension (char *Path);
char *MISC_Guid2String (LPGUID Guid);
char *MISC_DXError (HRESULT hr);

void MISC_MemSet4 (void *dst,DWORD value,DWORD count);
void MISC_MemSetFloat (void *dst,float value,DWORD count);
void MISC_MemCpy4 (void *dst,void *src,DWORD count);

#endif