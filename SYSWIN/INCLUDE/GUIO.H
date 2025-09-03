////////////////////////////////////////////////////////////////////////////
// Guio V2.0 by Hlod-Wig   (Modif.of.v1.Elric)
////////////////////////////////////////////////////////////////////////////
#ifndef GUIOV20
#define GUIOV20

#include <windows.h>

extern long GUIO_StartRun;

void GUIO_Init();
void GUIO_Close();
bool GUIO_Load(char *Name);
void GUIO_AddFN(void (*Action)(float now),char *Name);
void GUIO_AppendBaseSequence (void (*Action)(float now),float duration,float realduration=0.0f,float start_efect=0.0f);
void GUIO_InsertLayerSequence (void (*Action)(float now),long level,float start,float duration,float realduration=0.0f,float start_efect=0.0f);
void GUIO_InsertAction (void (*Action)(float now),float start);
void GUIO_Run (DWORD Pos);

void GUIO_InfoPos(DWORD);
void GUIO_InitBufferPos(DWORD);
void GUIO_SaveBufferPos();
void GUIO_LoadBufferPos();
extern DWORD *GP_Position;
extern long GUIO_StartRun;
extern bool GP_PosLoaded;
DWORD  GUIO_GetPosition ( long Tick );

#endif
