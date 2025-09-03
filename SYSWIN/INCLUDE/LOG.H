// AWL - Anaconda Windows Library
// Log & Error system

#ifndef AWL_LOG
#define AWL_LOG

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//#define LOG_LOGS

extern char LOG_Buffer[512];
extern va_list LOG_VaList;
extern FILE *LOG_Log;
extern bool LOG_Crashing;
extern int LOG_Counter;
extern int LOG_SecDepth;

bool ERRDX(char *a,...);
void ERR(char *a,...);

#ifdef LOG_LOGS

inline void LOG_NET(char *a,...)
{
  LOG_Counter++;
  va_start (LOG_VaList,a);
  vsprintf (LOG_Buffer,a,LOG_VaList);
  char s[256];
  sprintf (s,"%s\n",LOG_Buffer);
  fprintf (LOG_Log,s);
  fflush (LOG_Log);  
}

inline void LOG(char *a,...)
{
  LOG_Counter++;
  va_start (LOG_VaList,a);
  vsprintf (LOG_Buffer,a,LOG_VaList);
  char s[256];
  char c[256]="";    
  for (int i=0; i<LOG_SecDepth; i++) strcat (c,"  ");
  sprintf (s,"%s(%3i) - %s\n",c,LOG_Counter,LOG_Buffer);
  fprintf (LOG_Log,s);
  fflush (LOG_Log);  
}

inline void SEC_START(char *a,...)
{
  va_start (LOG_VaList,a);
  vsprintf (LOG_Buffer,a,LOG_VaList);  
  char s[256];
  char c[256]="";    
  for (int i=0; i<LOG_SecDepth; i++) strcat (c,"  ");  
  sprintf (s,"%s[- %s -]\n",c,LOG_Buffer);  
  fprintf (LOG_Log,s);
  fflush (LOG_Log);  
  LOG_SecDepth++;
}

inline void SEC_END()
{
  if (LOG_SecDepth>0) LOG_SecDepth--;
}

inline bool LOG_Init (char *s="log.txt")
{  
  if ((LOG_Log=fopen(s,"wt"))==NULL) return false;
  LOG_Counter = 0;
  LOG_SecDepth = 0;    
  LOG_Crashing = false;
  LOG ("Starting Log-System");  
  return true;
}

inline void LOG_Close ()
{  
  LOG ("Finishing Log-System");  
  fclose (LOG_Log);
}

#else

inline void LOG_NET(char *a,...)
{
}

inline void LOG(char *a,...)
{
}

inline void SEC_START(char *a,...)
{
}

inline void SEC_END()
{
}

inline bool LOG_Init (char *s="log.txt")
{  
  return true;
}

inline void LOG_Close ()
{  
}

#endif

#endif
