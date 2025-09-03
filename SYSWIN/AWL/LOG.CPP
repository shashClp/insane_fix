// AWL - Anaconda Windows Library
// Log & Error system

#include <stdio.h>
#include <stdarg.h>
#include <video.h>
#include <misc.h>
#include <syswin.h>

FILE *LOG_Log;
bool LOG_Crashing;
int LOG_Counter=0;
int LOG_SecDepth;
char LOG_Buffer[512];
va_list LOG_VaList;

#ifdef LOG_LOGS

bool ERRDX(char *a,...)
{
  if (hr!=DD_OK)
  {
    LOG_Counter++;
    va_start (LOG_VaList,a);
    vsprintf (LOG_Buffer,a,LOG_VaList);
    char s[256];
    sprintf (s,"ERROR - %s | %s\n",MISC_DXError(hr),LOG_Buffer);    
    fprintf (LOG_Log,s); 
    fflush (LOG_Log);   
    if ((!LOG_Crashing) && (LOG_SecDepth>0)) LOG_SecDepth--;
    LOG_Crashing = true;
    AfxMessageBox (s,MB_OK,0);
    return true;
  }  
  return false;
}

void ERR(char *a,...)
{
  LOG_Counter++;
  va_start (LOG_VaList,a);
  vsprintf (LOG_Buffer,a,LOG_VaList);  
  char s[256];
  sprintf (s,"ERROR - %s \n",LOG_Buffer);    
  fprintf (LOG_Log,s); 
  fflush (LOG_Log);   
  if ((!LOG_Crashing) && (LOG_SecDepth>0)) LOG_SecDepth--;
  LOG_Crashing = true;
  AfxMessageBox (s,MB_OK,0);
}

#else

bool ERRDX(char *a,...)
{
  if (hr!=DD_OK)
  {
    va_start (LOG_VaList,a);
    vsprintf (LOG_Buffer,a,LOG_VaList);      
    char s[256];
    sprintf (s,"ERROR - %s | %s\n",MISC_DXError(hr),LOG_Buffer);    
    MessageBox (HWnd,s,"Direct X Error",MB_OK);
    return true;
  }  
  return false;
}

void ERR(char *a,...)
{
  va_start (LOG_VaList,a);
  vsprintf (LOG_Buffer,a,LOG_VaList);  
  char s[256];
  sprintf (s,"ERROR - %s \n",LOG_Buffer);
  MessageBox (HWnd,s,"Error",MB_OK);
}

#endif