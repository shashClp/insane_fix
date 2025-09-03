
#include <windows.h>
#include <ball.h>
#include <blobs3d.h>
#include <defines.h>
#include <misc.h>
#include <bound.h>

TBound BALL_bound;
float BALL_Ad,BALL_AdX,BALL_AdY,BALL_AdZ;
float BALL_DeX,BALL_DeY,BALL_DeZ;

void BALL_Init (TVertex min,TVertex max)
{
  float dx = max.x-min.x;
  float dy = max.y-min.y;
  float dz = max.z-min.z;
  
  BALL_AdX = BLOBS3D_iDim / dx;
  BALL_AdY = BLOBS3D_jDim / dy;
  BALL_AdZ = BLOBS3D_kDim / dz;

  BALL_DeX = ((float)BLOBS3D_iDim * 0.5f)-0.5f;
  BALL_DeY = ((float)BLOBS3D_jDim * 0.5f)-0.5f;
  BALL_DeZ = ((float)BLOBS3D_kDim * 0.5f)-0.5f;
    
  BALL_Ad = (BALL_AdX+BALL_AdY+BALL_AdZ)*M_1Div3;
}

void BALL_Set (TBall &b,float r,float s)
{
  r*=BALL_Ad;
  b.nradi = r;
  b.radi = sqr(r);
  b.invradi = 1.0f/b.radi;
  b.str = s;
  b.active = true;
}
  
void BALL_Set (TBall &b,float x,float y,float z)
{  
  x*=BALL_AdX;
  y*=BALL_AdY;
  z*=BALL_AdZ;
  b.x = x+BALL_DeX;
  b.y = y+BALL_DeY;
  b.z = z+BALL_DeZ;  

  float rx = b.nradi;
  float ry = b.nradi;
  float rz = b.nradi;

  b.ini[0] = (long)(b.x-rx); if (b.ini[0]<0) b.ini[0] = 0;
  b.ini[1] = (long)(b.y-ry); if (b.ini[1]<0) b.ini[1] = 0;
  b.ini[2] = (long)(b.z-rz); if (b.ini[2]<0) b.ini[2] = 0;

  if (b.ini[0]>(long)BLOBS3D_iDim) { b.active = FALSE; return; }
  if (b.ini[1]>(long)BLOBS3D_jDim) { b.active = FALSE; return; }
  if (b.ini[2]>(long)BLOBS3D_kDim) { b.active = FALSE; return; }

  b.fin[0] = (long)(b.x+rx); if (b.fin[0]>(long)BLOBS3D_iDim) b.fin[0] = BLOBS3D_iDim;
  b.fin[1] = (long)(b.y+ry); if (b.fin[1]>(long)BLOBS3D_jDim) b.fin[1] = BLOBS3D_jDim;
  b.fin[2] = (long)(b.z+rz); if (b.fin[2]>(long)BLOBS3D_kDim) b.fin[2] = BLOBS3D_kDim;

  if (b.fin[0]<0) { b.active = FALSE; return; }
  if (b.fin[1]<0) { b.active = FALSE; return; }
  if (b.fin[2]<0) { b.active = FALSE; return; }

  b.active = true;
}

void BALL_Put (TBall* balls,long n)
{ 
  for(DWORD k=0; k<(DWORD)n; k++)
  {
    TBall *b = balls+k;
    if (b->active)
    {
      float *bz = BLOBS3D_data+(b->ini[2]*BLOBS3D_ijDim)+(b->ini[1]*BLOBS3D_iDim)+b->ini[0];
      for (long z=b->ini[2]; z<b->fin[2]; z++)
      {
        float *by = bz;
        bz+=BLOBS3D_ijDim;
        for(long y=b->ini[1]; y<b->fin[1]; y++)
        {
          float *bx = by;
          by+=BLOBS3D_iDim;
          for(long x=b->ini[0]; x<b->fin[0]; x++)
          {
            float dx = x-b->x;
            float dy = y-b->y;
            float dz = z-b->z;
            float l = sqr(dx)+sqr(dy)+sqr(dz);
            if (l<b->radi)
            {
              l = (b->radi-l)*b->invradi;
              *bx+=b->str*sqr(l);
            }
            bx++;
          }
        }
      }
    }
  }
}
