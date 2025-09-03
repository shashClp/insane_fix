// AWL - Anaconda Windows Library
// Dll Support System

#ifndef AWL_DLL
#define AWL_DLL

HINSTANCE DLL_Init (char *dllname);
bool DLL_Close (HINSTANCE hDLL);
bool DLL_Call (HINSTANCE hDLL,void **r,char *s);

#endif