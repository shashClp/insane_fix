// AW3D - Anaconda 3D Windows Library
// Triangle Clipping

#include <windows.h>
#include <math.h>
#include <triclip.h>
#include <defines.h>
#include <mem.h>
#include <log.h>

long TAW3D_TriangleClipper::Clip (TVertex *vtx,DWORD vtx_count,float *inf,DWORD inf_size)
{
  long res;
  bool clipped = FALSE;

  this->vOut = vtx;
  this->iOut = inf;
  this->count = vtx_count;
  this->inf_size = inf_size;
  this->inf_size4 = inf_size*4;

  TVertex *vTmp = vbuff1;
  TVertex *vOut = vbuff2;
  float *iTmp = ibuff1;
  float *iOut = ibuff2;

  for (DWORD i=0; i<=frustrum.count; i++)
  {
    if (clip & (1<<i))
    {
      res = ClipToPlane (vOut,iOut,this->vOut,this->iOut,frustrum.plane[i]);
      if (!res) return 0;
      if (res>0)
      {
        this->vOut = vOut; vOut = vTmp; vTmp = this->vOut;
        this->iOut = iOut; iOut = iTmp; iTmp = this->iOut;
        clipped = true;
      }
    }
  }
  if (clipped) return -1;
  return 1;
}

long TAW3D_TriangleClipper::ClipToPlane (TVertex *ovtx,float *oinf,TVertex *ivtx,float *iinf,TPlane &pla)
{
DWORD i,j,inum,icount,next;
float Dist,Dist2;
TVertex *Vtx2;
float *Inf2;

  inum = count;
  bool AllIn =true;
  bool AllOut=true;
  float Distance[AW3D_TriangleClipper_BufferSize];
  for (i=0; i<inum; i++)
  {
    Distance[i] = PLA_Distance (pla,ivtx[i]);
    if (Distance[i] < -M_EPSILON) AllIn = false;
    if (Distance[i] >= M_EPSILON) AllOut = false;
	}
  if (AllIn) return -1;
  if (AllOut) return 0;  

  count = 0;
  icount = 0;
  TVertex *Vtx1 = ivtx;
  float *Inf1 = iinf;
  float Dist1 = Distance[0];
  bool Inside = Dist1 >= 0.0f;  
  for (i=1; i <= inum; i++)
  {
    next = i % inum;
    Vtx2 = ivtx+next;
    Inf2 = iinf+next*inf_size;
    Dist2 = Distance[next];
    if (Inside && (Dist2 >= 0.0f))
    {
      ovtx[count++] = *Vtx2;
      memcpy (oinf+icount,Inf2,inf_size4);
      icount+=inf_size;
    } else {
      if ((!Inside) && (Dist2 >= M_EPSILON))
      {
        Inside = true;
        Dist = Dist1/(Dist1-Dist2);
        VTX_Between (ovtx[count++],*Vtx1,*Vtx2,Dist);
        for (j=0; j<inf_size; j++)         
          oinf[icount++] = Inf1[j]+(Inf2[j]-Inf1[j])*Dist;
        ovtx[count++] = *Vtx2;
        memcpy (oinf+icount,Inf2,inf_size4);
        icount+=inf_size;        
      } else {
        if (Inside && (Dist2 < -M_EPSILON))
        {
          Inside = false;
          Dist = Dist1/(Dist1-Dist2);
          VTX_Between (ovtx[count++],*Vtx1,*Vtx2,Dist);
          for (j=0; j<inf_size; j++)         
            oinf[icount++] = Inf1[j]+(Inf2[j]-Inf1[j])*Dist;
        }
      }
    }
    Vtx1 = Vtx2;
    Inf1 = Inf2;
    Dist1 = Dist2;
  }
  return (count>=3);
}

TAW3D_TriangleClipper::TAW3D_TriangleClipper(TVertex *v1=NULL,TVertex *v2=NULL,float *i1=NULL,float *i2=NULL)
{  
  SEC_START ("Initializing AW3D_TriangleClipper");

  if (v1==NULL)
  {
    vbuff1 = (TVertex *) GetMem (sizeof(TVertex)*AW3D_TriangleClipper_BufferSize);
    flags = AW3D_TriangleClipper_FirstMem;
  } else {
    flags = AW3D_TriangleClipper_SharedMem;
    vbuff1 = v1;
  }
  if (v2==NULL)
  {
    vbuff2 = (TVertex *) GetMem (sizeof(TVertex)*AW3D_TriangleClipper_BufferSize);
    flags |= AW3D_TriangleClipper_FirstMem<<2;
  } else {
    flags |= AW3D_TriangleClipper_SharedMem<<2;
    vbuff2 = v2;
  }
  if (i1==NULL)
  {
    ibuff1 = (float *) GetMem (sizeof(float)*AW3D_TriangleClipper_BufferSize);
    flags |= AW3D_TriangleClipper_FirstMem<<4;
  } else {
    flags |= AW3D_TriangleClipper_SharedMem<<4;
    ibuff1 = i1;
  }
  if (i2==NULL)
  {
    ibuff2 = (float *) GetMem (sizeof(float)*AW3D_TriangleClipper_BufferSize);
    flags |= AW3D_TriangleClipper_FirstMem<<6;
  } else {
    flags |= AW3D_TriangleClipper_SharedMem<<6;
    ibuff2 = i2;
  }

  LOG ("v1: %p v2: %p i1: %p i2: %p ",vbuff1,vbuff2,ibuff1,ibuff2);
  SEC_END();
}

TAW3D_TriangleClipper::~TAW3D_TriangleClipper()
{
  LOG ("Freeing AW3D_TriangleClipper");

  if ((vbuff1) && (flags & AW3D_TriangleClipper_FirstMem))
  {
    FreeMem (vbuff1);
    vbuff1 = NULL;
  }
  if ((vbuff2) && ((flags>>2) & AW3D_TriangleClipper_FirstMem))
  {
    FreeMem (vbuff2);
    vbuff2 = NULL;
  }
  if ((ibuff1) && ((flags>>4) & AW3D_TriangleClipper_FirstMem))
  {
    FreeMem (ibuff1);
    ibuff1 = NULL;
  }
  if ((ibuff2) && ((flags>>6) & AW3D_TriangleClipper_FirstMem))
  {
    FreeMem (ibuff2);
    ibuff2 = NULL;
  }
}

long TAW3D_TriangleClipper::Clip (TVertex *vtx,DWORD vtx_count)
{
  long res;
  bool clipped = false;

  this->vOut = vtx;
  this->count = vtx_count;

  TVertex *vTmp = vbuff1;
  TVertex *vOut = vbuff2;

  for (DWORD i=0; i<frustrum.count; i++)
  {
    if (clip & (1<<i))
    {
      res = ClipToPlane (vOut,this->vOut,frustrum.plane[i]);
      if (!res) return 0;
      if (res>0)
      {
        this->vOut = vOut; vOut = vTmp; vTmp = this->vOut;
        clipped = true;
      }
    }
  }
  if (clipped) return -1;
  return 1;
}

long TAW3D_TriangleClipper::ClipToPlane (TVertex *ovtx,TVertex *ivtx,TPlane &pla)
{
DWORD i,inum,next;
TVertex *Vtx2;
float Dist2;
float Dist;

  inum = count;
  bool AllIn =true;
  bool AllOut=true;
  float Distance[AW3D_TriangleClipper_BufferSize];
  for (i=0; i<inum; i++)
  {
    Distance[i] = PLA_Distance (pla,ivtx[i]);
    if (Distance[i] < -M_EPSILON) AllIn = false;
    if (Distance[i] >= M_EPSILON) AllOut = false;
	}
  if (AllIn) return -1;
  if (AllOut) return 0;  

  count = 0;  
  TVertex *Vtx1 = ivtx;  
  float Dist1 = Distance[0];
  bool Inside = Dist1 >= 0.0f;  
  for (i=1; i <= inum; i++)
  {
    next = i % inum;
    Vtx2 = ivtx+next;
    Dist2 = Distance[next];
    if (Inside && (Dist2 >= 0.0f))
    {
      ovtx[count++] = *Vtx2;
    } else {
      if ((!Inside) && (Dist2 >= M_EPSILON))
      {
        Inside = true;
        Dist = Dist1/(Dist1-Dist2);
        VTX_Between (ovtx[count++],*Vtx1,*Vtx2,Dist);
        ovtx[count++] = *Vtx2;
      } else {
        if (Inside && (Dist2 < -M_EPSILON))
        {
          Inside = false;
          Dist = Dist1/(Dist1-Dist2);
          VTX_Between (ovtx[count++],*Vtx1,*Vtx2,Dist);
        }
      }
    }
    Vtx1 = Vtx2;
    Dist1 = Dist2;
  }
  return (count>=3);
}
