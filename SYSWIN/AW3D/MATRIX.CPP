// AW3D - Anaconda 3D Windows Library
// Matrix

#include <windows.h>
#include <math.h>
#include <vertex.h>
#include <defines.h>
#include <matrix.h>

#ifdef MTX_INTERACTIVE_KEYBOARD
  #include <keys.h>
  #include <debug.h>

TVertex MTX_IK_target;
TVertex MTX_IK_pofv;
bool MTX_IK_Initialized = false;
float MTX_IK_roll,MTX_IK_ControlVelocity;

#endif

void MTX_Identity (TMatrix &out)
{
	out[0][0] = 1.0;
	out[0][1] = 0.0;
	out[0][2] = 0.0;
	out[0][3] = 0.0;
	out[1][0] = 0.0;
	out[1][1] = 1.0;
	out[1][2] = 0.0;
	out[1][3] = 0.0;
	out[2][0] = 0.0;
	out[2][1] = 0.0;
	out[2][2] = 1.0;
	out[2][3] = 0.0;
}

bool MTX_IsIdentity(TMatrix &in)
{
  bool r = true;

	if (in[0][0] != 1.0) r = false;
	if (in[0][1] != 0.0) r = false;
	if (in[0][2] != 0.0) r = false;
	if (in[0][3] != 0.0) r = false;
	if (in[1][0] != 0.0) r = false;
	if (in[1][1] != 1.0) r = false;
	if (in[1][2] != 0.0) r = false;
	if (in[1][3] != 0.0) r = false;
	if (in[2][0] != 0.0) r = false;
	if (in[2][1] != 0.0) r = false;
	if (in[2][2] != 1.0) r = false;
	if (in[2][3] != 0.0) r = false;

	return r;
}

void MTX_Copy (TMatrix &out, TMatrix &in)
{ memcpy (out,in,sizeof(TMatrix)); }

void MTX_Mul (TMatrix &out,TMatrix &a,TMatrix &b)
{
  out[0][0]=a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0];
  out[0][1]=a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1];
  out[0][2]=a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2];
  out[0][3]=a[0][0]*b[0][3]+a[0][1]*b[1][3]+a[0][2]*b[2][3]+a[0][3];
  out[1][0]=a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0];
  out[1][1]=a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1];
  out[1][2]=a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2];
  out[1][3]=a[1][0]*b[0][3]+a[1][1]*b[1][3]+a[1][2]*b[2][3]+a[1][3];
  out[2][0]=a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0];
  out[2][1]=a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1];
  out[2][2]=a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2];
  out[2][3]=a[2][0]*b[0][3]+a[2][1]*b[1][3]+a[2][2]*b[2][3]+a[2][3];
}

void MTX_Inverse (TMatrix &b,TMatrix &a)
{
TVertex apb,bpa;

  for(DWORD i=0; i<3; i++)
    for(DWORD j=0; j<3; j++)
      b[i][j] = a[j][i];
  apb.x = -a[0][3];
  apb.y = -a[1][3];
  apb.z = -a[2][3];
  MTX_MulNoT (bpa,apb,b);
  b[0][3] = bpa.x;
  b[1][3] = bpa.y;
  b[2][3] = bpa.z;
}

void MTX_MulNoT (TVertex &a,TVertex &b,TMatrix &c)
{
  a.x = b.x*c[0][0]+
        b.y*c[0][1]+
        b.z*c[0][2];
  a.y = b.x*c[1][0]+
        b.y*c[1][1]+
        b.z*c[1][2];
  a.z = b.x*c[2][0]+
        b.y*c[2][1]+
        b.z*c[2][2];
}


void MTX_Mul (TVertex &out,TVertex &in,TMatrix &mat)
{
  out.x = in.x*mat[0][0]+
          in.y*mat[0][1]+
          in.z*mat[0][2]+mat[0][3];
  out.y = in.x*mat[1][0]+
          in.y*mat[1][1]+
          in.z*mat[1][2]+mat[1][3];
  out.z = in.x*mat[2][0]+
          in.y*mat[2][1]+
          in.z*mat[2][2]+mat[2][3];
}

void MTX_Scale (TMatrix &out,float x,float y,float z)
{
  out[0][0]=x; out[0][1]=0; out[0][2]=0; out[0][3]=0;
  out[1][0]=0; out[1][1]=y; out[1][2]=0; out[1][3]=0;
  out[2][0]=0; out[2][1]=0; out[2][2]=z; out[2][3]=0;
}

void MTX_Rotate (TMatrix &out,float x,float y,float z)
{
  x = x * M_ToRad;
  y = y * M_ToRad;
  z = z * M_ToRad;
  float sinx = (float)sin(x);
  float siny = (float)sin(y);
  float sinz = (float)sin(z);
  float cosx = (float)cos(x);
  float cosy = (float)cos(y);
  float cosz = (float)cos(z);

  out[0][0] = cosz * cosy;
  out[0][1] =-sinz * cosx + cosz * siny * sinx;
  out[0][2] =-sinz *-sinx + cosz * siny * cosx;
  out[0][3] = 0;
  out[1][0] = sinz * cosy;
  out[1][1] = cosz * cosx + sinz * siny * sinx;
  out[1][2] = cosz *-sinx + sinz * siny * cosx;
  out[1][3] = 0;
  out[2][0] =-siny;
  out[2][1] = cosy * sinx;
  out[2][2] = cosy * cosx;
  out[2][3] = 0;
}

void MTX_Translate (TMatrix &out,float x,float y,float z)
{
  out[0][0]=1; out[0][1]=0; out[0][2]=0; out[0][3]=x;
  out[1][0]=0; out[1][1]=1; out[1][2]=0; out[1][3]=y;
  out[2][0]=0; out[2][1]=0; out[2][2]=1; out[2][3]=z;
}

void MTX_LookAt (TMatrix &out,
                 float orgx,float orgy,float orgz,
                 float finx,float finy,float finz,float roll)
{
#ifdef MTX_INTERACTIVE_KEYBOARD
  if (!MTX_IK_Initialized)
  {    
    MTX_IK_ControlVelocity = 1.0;
    MTX_IK_pofv.x = orgx;
    MTX_IK_pofv.y = orgy;
    MTX_IK_pofv.z = orgz;
    MTX_IK_target.x = finx;
    MTX_IK_target.y = finy;
    MTX_IK_target.z = finz;
    MTX_IK_roll = roll;    
  } else {
    if (Keys[VK_SPACE])
    {
      if (Keys[VK_O]) MTX_IK_target.x+=MTX_IK_ControlVelocity;
      if (Keys[VK_P]) MTX_IK_target.x-=MTX_IK_ControlVelocity;
      if (Keys[VK_Q]) MTX_IK_target.z+=MTX_IK_ControlVelocity;
      if (Keys[VK_A]) MTX_IK_target.z-=MTX_IK_ControlVelocity;
      if (Keys[VK_W]) MTX_IK_target.y+=MTX_IK_ControlVelocity;
      if (Keys[VK_S]) MTX_IK_target.y-=MTX_IK_ControlVelocity;
    } else {
      if (Keys[VK_O]) MTX_IK_pofv.x+=MTX_IK_ControlVelocity;
      if (Keys[VK_P]) MTX_IK_pofv.x-=MTX_IK_ControlVelocity;
      if (Keys[VK_Q]) MTX_IK_pofv.z+=MTX_IK_ControlVelocity;
      if (Keys[VK_A]) MTX_IK_pofv.z-=MTX_IK_ControlVelocity;
      if (Keys[VK_W]) MTX_IK_pofv.y+=MTX_IK_ControlVelocity;
      if (Keys[VK_S]) MTX_IK_pofv.y-=MTX_IK_ControlVelocity;
    }
    if (Keys[VK_E]) MTX_IK_roll+=MTX_IK_ControlVelocity;
    if (Keys[VK_D]) MTX_IK_roll-=MTX_IK_ControlVelocity;

    if (Keys[VK_1]) MTX_IK_ControlVelocity+=0.25;
    if (Keys[VK_2]) MTX_IK_ControlVelocity-=0.25;

    orgx = MTX_IK_pofv.x;
    orgy = MTX_IK_pofv.y;
    orgz = MTX_IK_pofv.z;
    finx = MTX_IK_target.x;
    finy = MTX_IK_target.y;
    finz = MTX_IK_target.z;
    roll = MTX_IK_roll;
  }

//  DEBUG (0,0,"POFV   (%.2f,%.2f,%.2f)",orgx,orgy,orgz);
//  DEBUG (0,16,"TARGET (%.2f,%.2f,%.2f)",finx,finy,finz);
//  DEBUG (0,32,"ROLL   (%.2f)  SPEED (%.2f)",roll,MTX_IK_ControlVelocity);
#endif

TVertex Pivot,Dist;

  Pivot.x = -orgx;
  Pivot.y = -orgy;
  Pivot.z = -orgz;
  Dist.x = finx-orgx;
  Dist.y = finy-orgy;
  Dist.z = finz-orgz;
      
  float ax = (float) -atan2 (Dist.x,Dist.z);
  float ay = (float) asin (Dist.y / VTX_Length(Dist));
  float az = -roll * M_ToRad;

  float sinx = (float)sin(ax);
  float cosx = (float)cos(ax);
  float siny = (float)sin(ay);
  float cosy = (float)cos(ay);
  float sinz = (float)sin(az);
  float cosz = (float)cos(az);

  out[0][0] =  sinx * siny * sinz + cosx * cosz;
  out[0][1] =  cosy * sinz;
  out[0][2] =  sinx * cosz - cosx * siny * sinz;
  out[1][0] =  sinx * siny * cosz - cosx * sinz;
  out[1][1] =  cosy * cosz;
  out[1][2] = -cosx * siny * cosz - sinx * sinz;
  out[2][0] = -sinx * cosy;
  out[2][1] =  siny;
  out[2][2] =  cosx * cosy;

  out[0][3] = out[0][0]*Pivot.x +
              out[0][1]*Pivot.y +
              out[0][2]*Pivot.z;
  out[1][3] = out[1][0]*Pivot.x +
              out[1][1]*Pivot.y +
              out[1][2]*Pivot.z;
  out[2][3] = out[2][0]*Pivot.x +
              out[2][1]*Pivot.y +
              out[2][2]*Pivot.z;
}

void MTX_LookAtWithOutIK (TMatrix &out,
                          float orgx,float orgy,float orgz,
                          float finx,float finy,float finz,float roll)
{
TVertex Pivot,Dist;

  Pivot.x = -orgx;
  Pivot.y = -orgy;
  Pivot.z = -orgz;
  Dist.x = finx-orgx;
  Dist.y = finy-orgy;
  Dist.z = finz-orgz;
      
  float ax = (float) -atan2 (Dist.x,Dist.z);
  float ay = (float) asin (Dist.y / VTX_Length(Dist));
  float az = -roll * M_ToRad;

  float sinx = (float)sin(ax);
  float cosx = (float)cos(ax);
  float siny = (float)sin(ay);
  float cosy = (float)cos(ay);
  float sinz = (float)sin(az);
  float cosz = (float)cos(az);

  out[0][0] =  sinx * siny * sinz + cosx * cosz;
  out[0][1] =  cosy * sinz;
  out[0][2] =  sinx * cosz - cosx * siny * sinz;
  out[1][0] =  sinx * siny * cosz - cosx * sinz;
  out[1][1] =  cosy * cosz;
  out[1][2] = -cosx * siny * cosz - sinx * sinz;
  out[2][0] = -sinx * cosy;
  out[2][1] =  siny;
  out[2][2] =  cosx * cosy;

  out[0][3] = out[0][0]*Pivot.x +
              out[0][1]*Pivot.y +
              out[0][2]*Pivot.z;
  out[1][3] = out[1][0]*Pivot.x +
              out[1][1]*Pivot.y +
              out[1][2]*Pivot.z;
  out[2][3] = out[2][0]*Pivot.x +
              out[2][1]*Pivot.y +
              out[2][2]*Pivot.z;
}
