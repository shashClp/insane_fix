//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Log
#define SystemR_V30_Log

#include <conio.h>
#include <stdarg.h>

void LOG (char *a,...)
{
    auto va_list arglist;

    cprintf("LG: ");
    va_start( arglist, a );
    vcprintf( a , arglist );
    va_end( arglist );
    cprintf ("\n");
}

void ERR (char *a,...)
{
    auto va_list arglist;

    cprintf("ER: ");
    va_start( arglist, a );
    vcprintf( a , arglist );
    va_end( arglist );
    cprintf ("\n");
}

#endif
