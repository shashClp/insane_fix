#ifndef OBJ3DPAINT_PAINT
#define OBJ3DPAINT_PAINT

#include <matrix.h>
#include <vertex.h>
#include <a3d.h>
#include <view.h>
#include <view3d.h>
#include <face.h>
#include <plane.h>
#include <rgb.h>
#include <info.h>
#include <painter.h>
#include <object3dpaint.h>

#define OBJ3DPAINT_Order(TName,TNoClip,TClipBefore,TClipBeforePaint,TClipAfter)         \
void TName (TMatrix &m,TObject &obj)                                                    \
{                                                                                       \
  OBJ3DPAINT_TC->clip = BOUND_ClipToFrustrum (obj.bound,OBJ3DPAINT_TC->frustrum);       \
  if (OBJ3DPAINT_TC->clip==0) return;                                                   \
  if (OBJ3DPAINT_TC->clip<0)                                                            \
  {                                                                                     \
    TFace *f = obj.face;                                                                \
    TPlane *p = obj.plane;                                                              \
    for (DWORD i=0; i<obj.numface; i++)                                                 \
    {                                                                                   \
      if (PLA_Distance (*p,OBJ3DPAINT_TC->frustrum.org)>0)                              \
      {                                                                                 \
        TVertex v1r,v2r,v3r;                                                            \
        MTX_Mul (v1r,obj.vtx[f->a],m);                                                  \
        MTX_Mul (v2r,obj.vtx[f->b],m);                                                  \
        MTX_Mul (v3r,obj.vtx[f->c],m);                                                  \
        TVertex v1p,v2p,v3p;                                                            \
        VIEW3D_Project (v1p,v1r);                                                       \
        VIEW3D_Project (v2p,v2r);                                                       \
        VIEW3D_Project (v3p,v3r);                                                       \
        TNoClip                                                                         \
      }                                                                                 \
      f++;                                                                              \
      p++;                                                                              \
    }                                                                                   \
  } else {                                                                              \
    TFace *f = obj.face;                                                                \
    TPlane *p = obj.plane;                                                              \
    for (DWORD i=0; i<obj.numface; i++)                                                 \
    {                                                                                   \
      if (PLA_Distance (*p,OBJ3DPAINT_TC->frustrum.org)>0)                              \
      {                                                                                 \
        TVertex vr[3];                                                                  \
        vr[0] = obj.vtx[f->a];                                                          \
        vr[1] = obj.vtx[f->b];                                                          \
        vr[2] = obj.vtx[f->c];                                                          \
        DWORD clip;                                                                     \
        TClipBefore;                                                                    \
          TVertex vp[32];                                                               \
          for (DWORD j=0; j<OBJ3DPAINT_TC->count; j++)                                  \
          {                                                                             \
            MTX_Mul (vp[j],OBJ3DPAINT_TC->vOut[j],m);                                   \
            VIEW3D_Project (vp[j],vp[j]);                                               \
          }                                                                             \
          TClipBeforePaint                                                              \
          for (DWORD j=2; j<OBJ3DPAINT_TC->count; j++)                                        \
          {                                                                             \
            TClipAfter                                                                  \
          }                                                                             \
        }                                                                               \
      }                                                                                 \
      f++;                                                                              \
      p++;                                                                              \
    }                                                                                   \
  }                                                                                     \
}

#define OBJ3DPAINT_OrderRep(TName,TNoClip,TClipBefore,TClipBeforePaint,TClipAfter)      \
void TName (TMatrix &m,TObject &obj)                                                    \
{                                                                                       \
  OBJ3DPAINT_TC->clip = BOUND_ClipToFrustrum (obj.bound,OBJ3DPAINT_TC->frustrum);       \
  if (OBJ3DPAINT_TC->clip==0) return;                                                   \
  OBJ3DPAINT_TB->Prepare(obj.numvtx);                                                   \
  if (OBJ3DPAINT_TC->clip<0)                                                            \
  {                                                                                     \
    TFace *f = obj.face;                                                                \
    TPlane *p = obj.plane;                                                              \
    for (DWORD i=0; i<obj.numface; i++)                                                 \
    {                                                                                   \
      if (PLA_Distance (*p,OBJ3DPAINT_TC->frustrum.org)>0)                              \
      {                                                                                 \
        bool A = OBJ3DPAINT_TB->Get (f->a);                                             \
        bool B = OBJ3DPAINT_TB->Get (f->b);                                             \
        bool C = OBJ3DPAINT_TB->Get (f->c);                                             \
        TVertex *v1p = (TVertex *)OBJ3DPAINT_TB->GetBuffer (f->a);                      \
        TVertex *v2p = (TVertex *)OBJ3DPAINT_TB->GetBuffer (f->b);                      \
        TVertex *v3p = (TVertex *)OBJ3DPAINT_TB->GetBuffer (f->c);                      \
        if (!(A && B && C))                                                             \
        {                                                                               \
          if (!A)                                                                       \
          {                                                                             \
            TVertex vr;                                                                 \
            MTX_Mul (vr,obj.vtx[f->a],m);                                               \
            VIEW3D_Project (*v1p,vr);                                                   \
            OBJ3DPAINT_TB->Set (f->a);                                                  \
          }                                                                             \
          if (!B)                                                                       \
          {                                                                             \
            TVertex vr;                                                                 \
            MTX_Mul (vr,obj.vtx[f->b],m);                                               \
            VIEW3D_Project (*v2p,vr);                                                   \
            OBJ3DPAINT_TB->Set (f->b);                                                  \
          }                                                                             \
          if (!C)                                                                       \
          {                                                                             \
            TVertex vr;                                                                 \
            MTX_Mul (vr,obj.vtx[f->c],m);                                               \
            VIEW3D_Project (*v3p,vr);                                                   \
            OBJ3DPAINT_TB->Set (f->c);                                                  \
          }                                                                             \
        }                                                                               \
        TNoClip                                                                         \
      }                                                                                 \
      f++;                                                                              \
      p++;                                                                              \
    }                                                                                   \
  } else {                                                                              \
    TFace *f = BLOBS3D_face;                                                            \
    TPlane *p = BLOBS3D_plane;                                                          \
    for (DWORD i=0; i<BLOBS3D_face_num; i++)                                            \
    {                                                                                   \
      if (PLA_Distance (*p,OBJ3DPAINT_TC->frustrum.org)>0)                              \
      {                                                                                 \
        TVertex vr[3];                                                                  \
        vr[0] = BLOBS3D_vtx[f->a];                                                      \
        vr[1] = BLOBS3D_vtx[f->b];                                                      \
        vr[2] = BLOBS3D_vtx[f->c];                                                      \
        DWORD clip;                                                                     \
        TClipBefore;                                                                    \
          if (clip>0)                                                                   \
          {                                                                             \
            TVertex vp[32];                                                             \
            for (DWORD j=0; j<BLOBS3D_TC->count; j++)                                   \
            {                                                                           \
              MTX_Mul (vp[j],BLOBS3D_TC->vOut[j],m);                                    \
              VIEW3D_Project (vp[j],vp[j]);                                             \
            }                                                                           \
            TClipBeforePaint                                                            \
            for (j=2; j<BLOBS3D_TC->count; j++)                                         \
            {                                                                           \
              TClipAfter                                                                \
            }                                                                           \
          } else {                                                                      \
            bool A = OBJ3DPAINT_TB->Get (f->a);                                         \
            bool B = OBJ3DPAINT_TB->Get (f->b);                                         \
            bool C = OBJ3DPAINT_TB->Get (f->c);                                         \
            TVertex *v1p = (TVertex *)OBJ3DPAINT_TB->GetBuffer (f->a);                  \
            TVertex *v2p = (TVertex *)OBJ3DPAINT_TB->GetBuffer (f->b);                  \
            TVertex *v3p = (TVertex *)OBJ3DPAINT_TB->GetBuffer (f->c);                  \
            if (!(A && B && C))                                                         \
            {                                                                           \
              if (!A)                                                                   \
              {                                                                         \
                TVertex vr;                                                             \
                MTX_Mul (vr,vr[0],m);                                                   \
                VIEW3D_Project (*v1p,vr);                                               \
                OBJ3DPAINT_TB->Set (f->a);                                              \
              }                                                                         \
              if (!B)                                                                   \
              {                                                                         \
                TVertex vr;                                                             \
                MTX_Mul (vr,vr[1],m);                                                   \
                VIEW3D_Project (*v2p,vr);                                               \
                OBJ3DPAINT_TB->Set (f->b);                                              \
              }                                                                         \
              if (!C)                                                                   \
              {                                                                         \
                TVertex vr;                                                             \
                MTX_Mul (vr,vr[2],m);                                                   \
                VIEW3D_Project (*v3p,vr);                                               \
                OBJ3DPAINT_TB->Set (f->c);                                              \
              }                                                                         \
            }                                                                           \
            TNoClip                                                                     \
          }                                                                             \
        }                                                                               \
      }                                                                                 \
      f++;                                                                              \
      p++;                                                                              \
    }                                                                                   \
  }                                                                                     \
}

#endif