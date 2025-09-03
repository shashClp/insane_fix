// AW3D - Anaconda 3D Windows Library
// Pixel

#include <windows.h>
#include <view.h>
#include <paintertypes.h>

void _fastcall PIXEL_Paint (TPPIXEL *src)
{
  *(GBL_Bits+((DWORD)src->x)+(((DWORD)src->y)*GBL_Pitch)) = src->col;
}
