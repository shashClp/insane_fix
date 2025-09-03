//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "rdefines.h"
#include "mem.h"

long R_GLOBAL_FLAGS = 0;

long Not(long d)
{
  long s;
  _asm{
    mov eax,d
    not eax
    mov s,eax
  }
  return s;
}

double sqr (double a){return a*a;}

#ifndef R_ENABLE_FILES_DEPENDENCIES
#include <conio.h>
#include <stdarg.h>

void R_LOG (char *a,...)
{
  /*
  auto va_list arglist;
    cprintf("SRT: ");
    va_start( arglist, a );
    vcprintf( a , arglist );
    va_end( arglist );
    cprintf ("\n");*/
}

void R_ERR (char *a,...)
{
    /*
	auto va_list arglist;
    cprintf("SRT:!! ");
    va_start( arglist, a );
    vcprintf( a , arglist );
    va_end( arglist );
    cprintf (" !!\n");
	*/
#ifdef R_ERROR_EXIT
    RAY_EXIT();
#endif
}
#endif

void (*RAY_EXIT) ( );

void *R_GetMem (long size)
{
  void *Dummy = NULL;
  Dummy = GetMem (size);
  if (Dummy == NULL) R_ERR ("Cant get %d bytes of mem",size);
  return Dummy;
}

void R_FreeMem (void *Dummy)
{
  if (Dummy == NULL) R_ERR ("Cant free null pointer");
}

void InitContext()
{
#ifndef R_ENABLE_FILES_DEPENDENCIES
   freopen ("RLOG.TXT","wt",stdout);
   cprintf ("System-RV30: <LOG FILE>\n");
#endif
}

void CloseContext()
{
#ifndef R_ENABLE_FILES_DEPENDENCIES
#endif
}

