// AW3D - Anaconda 3D Windows Library
// Painters

#ifndef AW3D_PAINTERS
#define AW3D_PAINTERS

#include <windows.h>
#include <paintertypes.h>

extern void _fastcall PIXEL_Paint (TPPIXEL *src);

extern void _fastcall TRI_FLAT_BORDER_Paint (TPTRI_FLAT *src);

extern void _fastcall TRI_FLAT_Paint (TPTRI_FLAT *src);

extern void _fastcall TRI_FLAT50_Paint (TPTRI_FLAT *src);

extern void _fastcall TRI_RGB_Paint (TPTRI_RGB *src);

extern void _fastcall TRI_PI_MAP_Paint (TPTRI_PI_MAP *src);
extern void _fastcall TRI_PI_MAP_FLAT_Paint (TPTRI_PI_MAP_FLAT *src);
extern void _fastcall TRI_PI_MAP_RGB_Paint (TPTRI_PI_MAP_RGB *src);
extern void _fastcall TRI_PI_MAP_DOUBLE_Paint (TPTRI_PI_MAP_DOUBLE *src);
extern void _fastcall TRI_PI_MAP_BLACK_Paint (TPTRI_PI_MAP *src);
extern void _fastcall TRI_PI_MAP_ALPHA_Paint (TPTRI_PI_MAP_Alpha *src);

extern void _fastcall TRI_PI_MAP50_Paint (TPTRI_PI_MAP *src);

#endif