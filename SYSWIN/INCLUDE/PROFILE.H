// AWL - Anaconda Windows Library
// Profile routines

#ifndef AWL_PROFILE
#define AWL_PROFILE

#include <windows.h>

//#define PROFILE_Profiles

typedef struct {
  DWORD ticks;
  DWORD start;
  DWORD end;
  DWORD count;
  DWORD color;
  char name[256];
} TProfileSection;

extern TProfileSection *PROFILE_sections;
extern DWORD PROFILE_seccount;
extern DWORD PROFILE_totalticks;
extern bool PROFILE_active;
extern DWORD PROFILE_start;
extern DWORD PROFILE_end;

#ifdef PROFILE_Profiles

  #define PBEGIN(a) PROFILE_BeginSection(a);
  #define PEND      PROFILE_EndSection();

#else

  #define PBEGIN(a)
  #define PEND

#endif

void PROFILE_Init(DWORD sections);
void PROFILE_Close();
void PROFILE_Begin();
void PROFILE_End();
void PROFILE_BeginSection(DWORD num);
void PROFILE_EndSection();
void PROFILE_DefineSection(DWORD num,char *name);
void PROFILE_DefineSection(DWORD num,char *name,DWORD color);
void PROFILE_Paint();

#endif
