// Marching Cubes Blobs 
// by Elric

#include <windows.h>
#include <blobs3d.h>
#include <misc.h>
#include <defines.h>
#include <mem.h>
#include <log.h>

TObject BLOBS3D_object;
TVertex BLOBS3D_scale;
float BLOBS3D_limit;
float *BLOBS3D_data;
TTexture *BLOBS3D_txt;
TVertex BLOBS3D_pofv;
DWORD BLOBS3D_iDim;
DWORD BLOBS3D_jDim;
DWORD BLOBS3D_kDim;
DWORD BLOBS3D_ijDim;
DWORD BLOBS3D_ijkDim;

DWORD BLOBS3D_iDim2;
DWORD BLOBS3D_jDim2;
DWORD BLOBS3D_kDim2;
DWORD BLOBS3D_iDim12;
DWORD BLOBS3D_ijDim12;
long *BLOBS3D_edges;
float *BLOBS3D_bot;
float *BLOBS3D_top;

void BLOBS3D_MakeLayer (DWORD k);

void BLOBS3D_MakeMarch()
{
DWORD i,j,k;
long *edges;

  BLOBS3D_object.numvtx=0;
  BLOBS3D_object.numface=0;    
  BLOBS3D_bot = BLOBS3D_data;
  BLOBS3D_top = BLOBS3D_data+(BLOBS3D_ijDim);
  MISC_MemSet4 (BLOBS3D_edges,BLOBS3D_EMPTY_EDGE,BLOBS3D_ijDim12);
  
  BLOBS3D_MakeLayer(0);
  for (k=1; k<BLOBS3D_kDim-1; k++)
  {
    BLOBS3D_bot = BLOBS3D_top;
    BLOBS3D_top = BLOBS3D_top+BLOBS3D_ijDim;
    edges = BLOBS3D_edges;
    for (j=1; j<BLOBS3D_jDim; j++,edges+=12)
      for (i=1; i<BLOBS3D_iDim; i++,edges+=12)
      {
        MISC_MemCpy4 (edges,edges+4,4);
        MISC_MemSet4 (edges+4,BLOBS3D_EMPTY_EDGE,8);
      }
    BLOBS3D_MakeLayer(k);
  }
}

void BLOBS3D_MakeLayer (DWORD k)
{
  float *jbot = BLOBS3D_bot;
  float *jtop = BLOBS3D_top;
  float *jbot1 = BLOBS3D_bot+BLOBS3D_iDim;
  float *jtop1 = BLOBS3D_top+BLOBS3D_iDim;
  long *edges = BLOBS3D_edges;    
  for(DWORD j=0; j<BLOBS3D_jDim-1; j++,edges+=12)
  {
    for(DWORD i=0; i<BLOBS3D_iDim-1; i++,edges+=12)
    {
      float cell[8];
      DWORD index = 0;
      cell[0] = jbot1[i];
      cell[1] = jbot1[i+1];
      cell[2] = jbot[i+1];
      cell[3] = jbot[i];
      cell[4] = jtop1[i];
      cell[5] = jtop1[i+1];
      cell[6] = jtop[i+1];
      cell[7] = jtop[i];            
      if (cell[0] < BLOBS3D_limit) index |=   1;
      if (cell[1] < BLOBS3D_limit) index |=   2;
      if (cell[2] < BLOBS3D_limit) index |=   4;
      if (cell[3] < BLOBS3D_limit) index |=   8;
      if (cell[4] < BLOBS3D_limit) index |=  16;
      if (cell[5] < BLOBS3D_limit) index |=  32;
      if (cell[6] < BLOBS3D_limit) index |=  64;
      if (cell[7] < BLOBS3D_limit) index |= 128;

      if ((index==255) || (index==0)) continue;

      for (DWORD e=0; e < 12; e++)
      {
        if (BLOBS3D_edgetable[index] & (1L<<e))
        {
          if (edges[e]==BLOBS3D_EMPTY_EDGE)
          {
            TVertex *v = BLOBS3D_object.vtx+BLOBS3D_object.numvtx;
            edges[e] = BLOBS3D_object.numvtx;

            // Calcul factor interpolacio
            float vFr = cell[e & 7];            
            float d = (vFr-BLOBS3D_limit) / (vFr-cell[BLOBS3D_to[e]]);
            
            if (d < M_EPSILON) *v = BLOBS3D_pfr[e];
            else 
              if (d > (1.0f-M_EPSILON)) *v = BLOBS3D_pto[e];
              else {
                const TVertex *fr = BLOBS3D_pfr+e;
                const TVertex *to = BLOBS3D_pto+e;
                v->x = fr->x+(to->x-fr->x)*d;
                v->y = fr->y+(to->y-fr->y)*d;
                v->z = fr->z+(to->z-fr->z)*d;                  
              }              

            v->x = (v->x+i-BLOBS3D_iDim2)*BLOBS3D_scale.x;
            v->y = (v->y+j-BLOBS3D_jDim2)*BLOBS3D_scale.y;
            v->z = (v->z+k-BLOBS3D_kDim2)*BLOBS3D_scale.z;
                       
            BLOBS3D_object.numvtx++;            
          }
        }
      }
      long *edgesi = edges+12;
      long *edgesj = edges+BLOBS3D_iDim12;
      edgesi[3] = edges[1];
      edgesi[7] = edges[5];
      edgesi[8] = edges[9];
      edgesi[11]= edges[10];
      edgesj[2] = edges[0];
      edgesj[6] = edges[4];
      edgesj[11]= edges[8];
      edgesj[10]= edges[9];

      TFace *f = BLOBS3D_object.face+BLOBS3D_object.numface;
      TPlane *p = BLOBS3D_object.plane+BLOBS3D_object.numface;
      BYTE *ii = (BYTE *)BLOBS3D_tritable[index];
      while (*ii != 255)
      {
        f->a = (WORD)edges[*ii++];
        f->c = (WORD)edges[*ii++];
        f->b = (WORD)edges[*ii++];
        PLA_CalcABCD (*p,BLOBS3D_object.vtx[f->a],BLOBS3D_object.vtx[f->b],BLOBS3D_object.vtx[f->c]);
        if (PLA_Distance(*p,BLOBS3D_pofv) > 0)
        {
          p++; 
          f++;
          BLOBS3D_object.numface++;
        }
      }
    }
    jbot=jbot1;
    jtop=jtop1;
    jbot1+=BLOBS3D_iDim;
    jtop1+=BLOBS3D_iDim;
  }
}
 
bool BLOBS3D_Init (DWORD i,DWORD j,DWORD k,DWORD vnum,DWORD fnum)
{
  BLOBS3D_iDim = i;
  BLOBS3D_jDim = j;
  BLOBS3D_kDim = k;
  BLOBS3D_ijDim = i*j;
  BLOBS3D_ijkDim = i*j*k;
  BLOBS3D_iDim2 = BLOBS3D_iDim/2;
  BLOBS3D_jDim2 = BLOBS3D_jDim/2;
  BLOBS3D_kDim2 = BLOBS3D_kDim/2;
  BLOBS3D_iDim12 = BLOBS3D_iDim*12;
  BLOBS3D_ijDim12 = BLOBS3D_ijDim*12;
  BLOBS3D_edges = (long *) GetMem (4*j*i*12);
  if (BLOBS3D_edges==NULL)
  {
    ERR ("Not enough memory for Blob3D edges buffer");
    return false;
  }  
  BLOBS3D_data = (float *) GetMem (4*j*i*k);
  if (BLOBS3D_data==NULL)
  {
    ERR ("Not enough memory for Blob3D data buffer");
    return false;
  }
  BLOBS3D_object.vtx = (TVertex *) GetMem (sizeof(TVertex)*vnum);
  if (BLOBS3D_object.vtx==NULL)
  {
    ERR ("Not enough memory for Blob3D vtx buffer");
    return false;
  }
  BLOBS3D_object.nrm = (TVertex *) GetMem (sizeof(TVertex)*vnum);
  if (BLOBS3D_object.nrm==NULL)
  {
    ERR ("Not enough memory for Blob3D nrm buffer");
    return false;
  }
  BLOBS3D_object.inf = (TInfo *) GetMem (4*2*vnum);
  if (BLOBS3D_object.inf==NULL)
  {
    ERR ("Not enough memory for Blob3D inf buffer");
    return false;
  }
  BLOBS3D_object.face = (TFace *) GetMem (sizeof(TFace)*fnum);
  if (BLOBS3D_object.face==NULL)
  {
    ERR ("Not enough memory for Blob3D face buffer");
    return false;
  }
  BLOBS3D_object.plane = (TPlane *) GetMem (sizeof(TPlane)*fnum);
  if (BLOBS3D_object.plane==NULL)
  {
    ERR ("Not enough memory for Blob3D plane buffer");
    return false;
  }
  return true;
}

void BLOBS3D_Close ()
{
  if (BLOBS3D_edges)
  {
    FreeMem (BLOBS3D_edges); 
    BLOBS3D_edges = NULL;
  }
  if (BLOBS3D_data)
  {
    FreeMem (BLOBS3D_data); 
    BLOBS3D_data = NULL;
  }
  if (BLOBS3D_object.inf)
  {
    FreeMem (BLOBS3D_object.inf); 
    BLOBS3D_object.inf = NULL;
  }
  if (BLOBS3D_object.vtx)
  {
    FreeMem (BLOBS3D_object.vtx); 
    BLOBS3D_object.vtx = NULL;
  }
  if (BLOBS3D_object.nrm)
  {
    FreeMem (BLOBS3D_object.nrm); 
    BLOBS3D_object.nrm = NULL;
  }
  if (BLOBS3D_object.face)
  {
    FreeMem (BLOBS3D_object.face); 
    BLOBS3D_object.face = NULL;
  }
  if (BLOBS3D_object.plane)
  {
    FreeMem (BLOBS3D_object.plane); 
    BLOBS3D_object.plane = NULL;
  }
}
