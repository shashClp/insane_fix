// AWL - Anaconda Windows Library
// FrameRate routines

#ifndef AWL_FRAMERATE
#define AWL_FRAMERATE

#include <windows.h>

extern DWORD FRAMERATE_Frames;
extern float FRAMERATE_FPS;
extern bool FRAMERATE_active;

void FRAMERATE_Paint();
void FRAMERATE_Init();

#endif
