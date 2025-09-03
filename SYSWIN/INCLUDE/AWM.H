// AWM - Anaconda Music Library
// Music Interface

#ifndef AWM_MUSIC
#define AWM_MUSIC

#define MUSIC_BASS

#include <windows.h>

#define MUSIC_MAX_VOLUME 255
/*
#define MUSIC_TYPE            1
#define MUSIC_MONO            1
#define MUSIC_STEREO          0

#define MUSIC_FREQ            6
#define MUSIC_11              4
#define MUSIC_22              2
#define MUSIC_44              0
*/

extern bool MUSIC_Init(DWORD freq, DWORD flags);
extern bool MUSIC_Load(char *filename);
extern void MUSIC_Play();
extern void MUSIC_Close();
extern void MUSIC_Volume(DWORD value);
extern DWORD MUSIC_GetVolume();
extern DWORD MUSIC_GetPos();
extern void MUSIC_PlayExt(DWORD Pos);

#endif