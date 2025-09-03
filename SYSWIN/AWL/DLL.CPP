// AWL - Anaconda Windows Library
// Dll Support System

#include <windows.h>
#include <log.h>

HINSTANCE DLL_Init (char *dllname)
{
  if (!dllname)
  {
    ERR ("Init of NULL dll name");
    return false;  
  }
  LOG ("Calling DLL %s",dllname);
  HINSTANCE hDLL = LoadLibrary(dllname);
  if (hDLL==NULL)
  {
    ERR ("Calling DLL %s",dllname);
    return NULL;  
  }
  return hDLL;
}

bool DLL_Close (HINSTANCE hDLL)
{
  if (hDLL==NULL)
  {
    ERR ("Close of NULL handle of DLL");
    return false;
  }
  FreeLibrary(hDLL);  
  LOG ("DLL closed!");
  return true;
}

bool DLL_Call (HINSTANCE hDLL,void **r,char *s)
{
  if (!s)
  {
    ERR ("Call to empty string function in DLL");
    return false;
  }
  if (hDLL==NULL)
  {
    ERR ("Call with NULL handle of DLL function %s",s);
    return false;
  }
  if (r==NULL)
  {
    ERR ("Variable function is NULL in dll call of %s",s);
    return false;
  }
  LOG ("Searching DLL function %s",s);
  *r = (void *)GetProcAddress(hDLL,s);
  if (*r==NULL)
  {
    ERR ("Searching DLL function %s",s);
    return false;
  }
  return true;
}
