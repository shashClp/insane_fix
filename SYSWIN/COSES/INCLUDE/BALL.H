// Ball
// By Elric

#ifndef AW_BLOBS3D_BALL
#define AW_BLOBS3D_BALL

#include <vertex.h>
#include <bound.h>

typedef struct {
  bool active;
  float x,y,z;
  float nradi,radi,invradi,str;
  long ini[3];
  long fin[3];
} TBall;

void BALL_Init (TVertex min,TVertex max);
void BALL_Set (TBall &b,float r,float s);
void BALL_Set (TBall &b,float x,float y,float z);
void BALL_Put (TBall* balls,long n);

#endif