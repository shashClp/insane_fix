#ifndef INSANE_ZMRT
#define INSANE_ZMRT

#include <windows.h>

typedef struct
{
	long x,y;
} SElemBufInterp;

void VuelcaBufInterp8x8F(DWORD* dest,DWORD* textura);
void OldFreeEsfera(float x,float y,float z);

#endif //_ZMRT_H_