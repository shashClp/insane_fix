// AWL - Anaconda Windows Library
// Registry routines

#include <log.h>
#include <misc.h>

HKEY REGS_Regs;
HKEY REGS_Regs2;
DWORD REGS_type;
BYTE REGS_Buffer[255];
DWORD REGS_length;

bool REGS_Connect (HKEY hk,char *name)
{  
  if (!name)
  {
    ERR ("Conecting to regsitry with NULL name?");
    return false;
  }

  LOG ("Connecting to registry");
  RegConnectRegistry (NULL,hk,&REGS_Regs);

  LOG ("Creating key %s",name);
  DWORD Disposition;
  RegCreateKeyEx(REGS_Regs,(LPCTSTR)name,
                 0,NULL,REG_OPTION_NON_VOLATILE,
                 KEY_ALL_ACCESS,NULL,
                 &REGS_Regs2,&Disposition);  
  return true;
}

bool REGS_Connect (char *name)
{  
  return REGS_Connect (HKEY_LOCAL_MACHINE,name);
}

bool REGS_SetStr (char *lab,char *val)
{
  if (!lab)
  {
    ERR ("SetStr Lab is empty?");
    return false;  
  }
  LOG ("Setting %s to %s",val,lab);
  RegSetValueEx (REGS_Regs2,lab,0,REG_SZ,(const BYTE *)val,strlen(val)+1);
  return true;
}

bool REGS_SetInt (char *lab,int val)
{
  if (!lab)
  {
    ERR ("SetInt Lab is empty?");
    return false;  
  }
  LOG ("Setting %i to %s",val,lab);  
  RegSetValueEx (REGS_Regs2,(LPCTSTR)lab,0,REG_DWORD,(const BYTE *)&val,4);
  return true;
}

char *REGS_GetStr (char *lab,char *val)
{
  if (!lab)
  {
    ERR ("GetStr Lab is empty?");
    return NULL;
  }
  REGS_type = REG_SZ;
  REGS_length = 255;
  LOG ("Getting %s from %s",val,lab);
  long res = RegQueryValueEx (REGS_Regs2,lab,NULL,&REGS_type,REGS_Buffer,&REGS_length);
  if (res==ERROR_SUCCESS) return (char *)REGS_Buffer;
  return val;
}

int REGS_GetInt (char *lab,int val)
{  
  if (!lab)
  {    
    ERR ("GetInt Lab is empty?");
    return 0;
  }
  REGS_type = REG_DWORD;
  REGS_length = 255;
  LOG ("Getting %i from %s",val,lab);
  long res = RegQueryValueEx (REGS_Regs2,(LPCTSTR)lab,NULL,&REGS_type,REGS_Buffer,&REGS_length);
  if (res==ERROR_SUCCESS) return *((int *)REGS_Buffer);
  return val;
}

bool REGS_Disconnect()
{
  RegCloseKey (REGS_Regs);
  LOG ("Disconnected!");
  return true;
}
