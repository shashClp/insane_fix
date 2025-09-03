// AWL - Anaconda Windows Library
// Registry routines

#ifndef AWL_REGISTRY
#define AWL_REGISTRY

#include <windows.h>

bool REGS_Connect (HKEY hk,char *name);
bool REGS_Connect (char *name);
bool REGS_SetStr (char *lab,char *val);
bool REGS_SetInt (char *lab,int val);
char *REGS_GetStr (char *lab,char *val);
int REGS_GetInt (char *lab,int val);
bool REGS_Disconnect();

#endif