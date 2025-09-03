//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#define VGA256
#define PlaneOK
#define SphereOK
#define EllipseOK
#define CSGOK
#define BlobOK
#define SorOK
#define CylinderOK
#define EllipseOK
#define CubeOK

#define NDEBUG
//#define DEBUG
//#define DEBUG_RENDER
//#define DEBUG_RENDER_REFLECT
#define RayDim 3
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
#include "..\include\srt301.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
void Sortir ()
{
  exit(0);
}
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
void main(int num , char *aw[])
{

  RAY_EXIT = &Sortir;
  TVertex P,L,N;

  InitScreen();
  InitContext();

  CloseScreen();
  CloseContext();
}

