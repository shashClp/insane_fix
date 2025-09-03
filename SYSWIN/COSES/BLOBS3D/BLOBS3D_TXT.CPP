#include <windows.h>
#include <misc.h>
#include <defines.h>
#include <blobs3d.h>
#include <math.h>

void BLOBS3D_MakeLayer_Texture (DWORD k);

void BLOBS3D_MakeMarch_Texture()
{
DWORD i,j,k;
long *edges;

  BLOBS3D_object.numvtx=0;
  BLOBS3D_object.numface=0;    
  BLOBS3D_bot = BLOBS3D_data;
  BLOBS3D_top = BLOBS3D_data+(BLOBS3D_ijDim);
  MISC_MemSet4 (BLOBS3D_edges,BLOBS3D_EMPTY_EDGE,BLOBS3D_ijDim12);
  
  BLOBS3D_MakeLayer_Texture(0);
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
    BLOBS3D_MakeLayer_Texture(k);
  }
}

void BLOBS3D_MakeLayer_Texture (DWORD k)
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
            
            const TVertex *fr = BLOBS3D_pfr+e;
            const TVertex *to = BLOBS3D_pto+e;
            if (d < M_EPSILON) *v = BLOBS3D_pfr[e];
            else 
              if (d > (1.0f-M_EPSILON)) *v = BLOBS3D_pto[e];
              else {
                fr = BLOBS3D_pfr+e;
                to = BLOBS3D_pto+e;
                v->x = fr->x+(to->x-fr->x)*d;
                v->y = fr->y+(to->y-fr->y)*d;
                v->z = fr->z+(to->z-fr->z)*d;                  
              }              

            v->x = (v->x+i-BLOBS3D_iDim2)*BLOBS3D_scale.x;
            v->y = (v->y+j-BLOBS3D_jDim2)*BLOBS3D_scale.y;
            v->z = (v->z+k-BLOBS3D_kDim2)*BLOBS3D_scale.z;

            long fr0 = i+(int)fr->x;
            long fr1 = j+(int)fr->y;
            long fr2 = k+(int)fr->z;
            long to0 = i+(int)to->x;
            long to1 = j+(int)to->y;
            long to2 = k+(int)to->z;
            TVertex nfr,nto;
	          float *afr = BLOBS3D_data+fr0+fr1*BLOBS3D_iDim+fr2*BLOBS3D_ijDim;
	          float *ato = BLOBS3D_data+to0+to1*BLOBS3D_iDim+to2*BLOBS3D_ijDim;
	          nfr.x=0.5f*(afr[1]-afr[-1]);
	          nfr.y=0.5f*(afr[BLOBS3D_iDim]-afr[-BLOBS3D_iDim]);
	          nfr.z=0.5f*(afr[BLOBS3D_ijDim]-afr[-BLOBS3D_ijDim]);
	          nto.x=0.5f*(ato[1]-ato[-1]);
	          nto.y=0.5f*(ato[BLOBS3D_iDim]-ato[-BLOBS3D_iDim]);
	          nto.z=0.5f*(ato[BLOBS3D_ijDim]-ato[-BLOBS3D_ijDim]);
	      
            TVertex *n = BLOBS3D_object.nrm+BLOBS3D_object.numvtx;
            // Normal
            n->x = nfr.x + d * (nto.x - nfr.x);
            n->y = nfr.y + d * (nto.y - nfr.y);
            n->z = nfr.z + d * (nto.z - nfr.z);
            d = sqr(n->x)+sqr(n->y)+sqr(n->z);
            if (d > M_EPSILON)
            {
              float id = 1.0f / sqrt(d);
              n->x *= id;
              n->y *= id;
              n->z *= id;
            } else {
              n->x = 1.0;
              n->y = 0.0;
              n->z = 0.0;
            };

            // Mapping
            TInfo *inf = BLOBS3D_object.inf+BLOBS3D_object.numvtx;
            inf->uu = 127.0f*n->x;
            inf->vv = 127.0f*n->y;
            
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
          f->i = BLOBS3D_txt;
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
