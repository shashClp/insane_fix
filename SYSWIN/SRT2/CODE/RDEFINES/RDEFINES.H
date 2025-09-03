//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Defines
#define SystemR_V30_Defines

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

#define PlaneOK
//#define CubeOK
#define SphereOK
#define EllipseOK
#define RayDimX R_RAY_DIM
#define RayDimY R_RAY_DIM
#define SYR_PROCEDURAL_TEXTURES

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± General Defines

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef float TReal;

extern long R_ScreenX;
extern long R_ScreenY;
#define R_ScreenSize      R_ScreenY*R_ScreenX

#define R_PI                                  3.14159265358979323846
#define InvR_PI                               1.0/3.14159265358979323846
#define R_PIx2                                3.14159265358979323846*2.0
#define R_TextureSize                           128

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± General Include Files

//#include "RVertex.h"

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± World Flags

#define RW_PREDICTION_ANIM           1<<1
#define RW_SHADOWS                   1<<2
#define RW_REFLECT                   1<<3
#define RW_REFRACT                   1<<4

#define RW_RESULTS                   1<<30

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± Object Flags & Types

#define R_RECIVE_SHADOW           1<<1
#define R_MAKE_SHADOW             1<<2
#define R_VISIBLE                 1<<3
#define R_REFLECT                 1<<4
#define R_TRANSPARENT             1<<5
#define R_BUMP                    1<<6
#define R_REFLECT_TEXT            1<<7
#define R_MASK_TEXT               1<<8
#define R_CENTRAL_TEXT            1<<9
#define R_PORTAL_TEXT             1<<10
#define R_VAMPIRE                 1<<11
#define R_TRANSPARENT_TEXT        1<<12
#define R_NO_LIGHT                1<<13

typedef enum  {
  R_SPHERE    = 1,
  R_PLANE     = 2,
  R_CYLINDER  = 3,
  R_CSG       = 4,
  R_CUBE      = 5,
  R_ELLIPSE   = 6,
  R_BLOB      = 7,
  R_SOR       = 8,
  R_HFIELD    = 9
} OBjType;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± Color

#ifndef RGBCOL
#define RGBCOL
typedef struct {
  union {
    dword ColRGB;
    struct { byte B,G,R,A; } B;
  };
} rgbcol;
#endif

//±±±±±±±

typedef struct {
    float RGBA[4];
}RealColor;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± Ray Info

typedef struct {
  long      ID;
  rgbcol    Color;
  float     LigthIntens;
  long      Shadowed;
  long      Desp;
} RayInfo;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± Procedural Textures

#ifdef SYR_PROCEDURAL_TEXTURES

struct StepPoint{
  TReal S;
  TReal RGBA[4];
};

struct DefTurb{
  TReal         Lambda,Omega;
  long          Octaves;
  struct {
    TReal x,y,z;
  }Factor;
};

struct ProceduralTexture{
  long  Type;
  long  N_Steps;
  StepPoint Dat[10];
  TReal TScale ;
  DefTurb Turbulence;
  ProceduralTexture *Next;
  ProceduralTexture *CNext1;
  ProceduralTexture *CNext2;
};

#endif

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± Pigment

struct DefPigment{

  rgbcol Color;
  rgbcol        *TexMap;
  TReal         TextScale;
  TReal         InvTextScale;
  long          TxtMode;
  long          BitsDefinition;

#ifdef SYR_PROCEDURAL_TEXTURES
  ProceduralTexture   *Texture;
#endif

};

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±± Extras


extern  long R_GLOBAL_FLAGS;
extern long Not(long d);
#ifdef R_ENABLE_FILES_DEPENDENCIES
 #include <log.h>
 #define R_LOG  LOG
 #define R_ERR  ERR
#else
#include <stdarg.h>
extern void R_LOG (char *a,...);
extern void R_ERR (char *a,...);
#endif
extern void (*RAY_EXIT)           ( );
extern void *R_GetMem (long size);
extern void R_FreeMem (void *);
extern void InitContext();
extern void CloseContext();

extern double sqr (double a);
#include "RVertex.h"
#endif
