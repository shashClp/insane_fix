/*北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
    Library
    v1.01 by Elric & Klauz of Anaconda
    February 1999
  北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
    Comentaris:
      - Definicions globals.
      - Variables globals.
      - Operacions generals.

    19/03/99: Primera implementacio.
    16/04/99: Alguns retocs.
  北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北*/

#ifndef LIBRARY
#define LIBRARY

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

//#define VIDEO_PAINT
//#define VIDEO_FPS

#ifndef VIDEO_PAINT
  long DebugNum=0;
  #define Debug(a)  cprintf("\n\r[%4i] - %s",DebugNum++,a);
#else
  #define Debug(a)
#endif

#ifdef VIDEO_FPS
  clock_t time_start;
  clock_t time_end;

  #define TimeStart()   time_start = clock();
  #define TimeEnd()     time_end = clock();
  #define TimePrint()   printf ("Ticks == [%i]\n\r",time_end-time_start);
#else
  #define TimeStart()
  #define TimeEnd()
  #define TimePrint()
#endif

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#define TRUE                  1
#define FALSE                 0
#define true                  1
#define false                 0

#define M_PI                  3.14159265358979323846
#define M_PIx2                M_PI*2.0
#define M_PIDiv2              M_PI/2.0
#define M_ToRad               M_PI/180.0
#define M_ToGrd               180.0/M_PI

#define Assigned(a)           (a!=NULL)

#include "ltypes.h"

dword GBL_Height;
dword GBL_Width;
dword GBL_ScreenWidth;
dword GBL_ScreenHeight;
dword GBL_ScreenSize,GBL_ScreenSize4;
dword GBL_Size,GBL_Size4;
float GBL_WidthDiv2,GBL_HeightDiv2;
float GBL_CenterX,GBL_CenterY;
float GBL_Depth,GBL_Factor,GBL_NegFactor,GBL_InvFactor,GBL_ScaleXY2Z;
float *GBL_ZBuffer;
dword *GBL_Bits;
dword GBL_Frame;

// 北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "mem.h"
#include "file.h"
#include "tinymath.h"
#include "matrix.h"
#include "plane.h"
#include "vertex.h"
#include "face.h"
#include "quat.h"
#include "spline.h"
#include "camera.h"
#include "bound.h"
#include "tree.h"
#include "list.h"
#include "traverse.h"
#include "screen.h"
#include "video.h"
#include "rgb.h"
#include "image.h"
#include "keys.h"

#endif
