// AW3D - Anaconda 3D Windows Library
// Triangle Clipping

#ifndef AW3D_TRICLIPPER
#define AW3D_TRICLIPPER

#include <windows.h>
#include <frustrum.h>
#include <vertex.h>
#include <plane.h>

#define AW3D_TriangleClipper_BufferSize           32
#define AW3D_TriangleClipper_FirstMem             1
#define AW3D_TriangleClipper_SharedMem            2

class TAW3D_TriangleClipper 
{
private:
  DWORD flags;
  DWORD inf_size;
  DWORD inf_size4;  
  float *ibuff1,*ibuff2;
  TVertex *vbuff1,*vbuff2;    

public:
  TFrustrum frustrum;
  int clip;
  DWORD count;
  TVertex *vOut;
  float *iOut;
  
  TAW3D_TriangleClipper(TVertex *v1,TVertex *v2,float *i1,float *i2);
  ~TAW3D_TriangleClipper();

  long Clip (TVertex *vtx,DWORD vtx_count,float *inf,DWORD inf_size);
  long ClipToPlane (TVertex *ovtx,float *oinf,TVertex *ivtx,float *iinf,TPlane &pla);

  long Clip (TVertex *vtx,DWORD vtx_count);
  long ClipToPlane (TVertex *ovtx,TVertex *ivtx,TPlane &pla);
};

#endif