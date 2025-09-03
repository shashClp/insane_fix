// AW3D - Anaconda 3D Windows Library
// Painters Types

#ifndef AW3D_PAINTERS_TYPES
#define AW3D_PAINTERS_TYPES

#include <windows.h>
#include <texture.h>

// PIXEL
typedef struct {
  float x,y;  
  DWORD col;
} TPPIXEL;

// TRIANGLE FLAT
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  DWORD col;
} TPTRI_FLAT;

// TRIANGLE RGB
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float r1,g1,b1;
  float r2,g2,b2;
  float r3,g3,b3;
} TPTRI_RGB;

// TRIANGLE PRESPECTIVE INCORRECT MAP UV
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float u1,v1;
  float u2,v2;
  float u3,v3;
  TTexture *txt;
} TPTRI_PI_MAP;

// TRIANGLE PRESPECTIVE INCORRECT MAP UV with Flat light
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float u1,v1;
  float u2,v2;
  float u3,v3;
  DWORD col;
  TTexture *txt;
} TPTRI_PI_MAP_FLAT;


// TRIANGLE RGB
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float u1,v1,r1,g1,b1;
  float u2,v2,r2,g2,b2;
  float u3,v3,r3,g3,b3;
  TTexture *txt;
} TPTRI_PI_MAP_RGB;

// TRIANGLE PRESPECTIVE INCORRECT MAP with Alpha
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float u1,v1,a1;
  float u2,v2,a2;
  float u3,v3,a3;
  TTexture *txt;
} TPTRI_PI_MAP_Alpha;

// TRIANGLE PRESPECTIVE INCORRECT MAP DOUBLE
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float u1,v1,l11,l12;
  float u2,v2,l21,l22;
  float u3,v3,l31,l32;
  TTexture *txt1;
  TTexture *txt2;
} TPTRI_PI_MAP_DOUBLE;

// TRIANGLE PRESPECTIVE INCORRECT MAP DOUBLE with light
typedef struct {  
  float x1,y1;
  float x2,y2;
  float x3,y3;
  float u1,v1,l11,l12,a1;
  float u2,v2,l21,l22,a2;
  float u3,v3,l31,l32,a3;
  TTexture *txt1;
  TTexture *txt2;
} TPTRI_PI_MAP_DOUBLE_FLAT;

#endif