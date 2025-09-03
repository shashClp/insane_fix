// AW3D - Anaconda 3D Windows Library
// Texture

#ifndef AW3D_TEXTURE
#define AW3D_TEXTURE

#include <windows.h>
#include <list.h>

#define TXT_MAXTEXTURE      100

typedef struct {
  DWORD flags;
  DWORD w,h;
  DWORD color;
  DWORD *bits;
  void *mipmap;
} TTexture;

typedef struct {
  DWORD id;
  DWORD *bits;
  TList sets;
} TTxtBloc;

typedef struct {
  DWORD x,y;
  DWORD w,h;
  TTxtBloc *base;
} TTxtFreeBloc;

typedef struct {
  byte cops;
  char *id;
  TTexture *txt;
} TTxtInBloc;

////////////////////////////////////////////////////////////////////////////

void TXT_Init (DWORD max);
void TXT_Close ();
TTexture *TXT_AddTexture (char *filename);
TTexture *TXT_AddTexture (DWORD w,DWORD h,DWORD *src);
void TXT_DelTexture (TTexture *txt);
void TXT_DelTexture (DWORD w,DWORD h,DWORD *src);

////////////////////////////////////////////////////////////////////////////

#endif
