// AWL - Anaconda Windows Library
// Image

#ifndef AWL_IMAGE
#define AWL_IMAGE

#include <windows.h>

typedef struct {  
  DWORD width;
  DWORD height;
  DWORD bpp;
  DWORD pitch;
  BYTE *paleta;
  void *bits;
} TImage;

TImage *IMAGE_Load(char *filename,DWORD bppdesired);

bool IMAGE_FlipV (TImage *img);
bool IMAGE_8To32 (TImage *img1,TImage *img2);
bool IMAGE_24To32 (TImage *img1,TImage *img2);
void FreeMem (TImage *img);

#endif