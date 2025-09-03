#include "paint.h"

#undef NoClip
#undef ClipBefore
#undef ClipBeforePaint
#undef ClipAfter

#define SetLight                                                                  \
        float ang = VTX_DotProduct (*((TVertex *)p),OBJ3DPAINT_Llum);             \
        ang *=0.5f; ang +=0.5f;                                                   \
        ang *= ang*ang;                                                           \
        DWORD col;                                                                \
        DWORD r = (DWORD)(ang*Get_R(OBJ3DPAINT_color));                           \
        DWORD g = (DWORD)(ang*Get_G(OBJ3DPAINT_color));                           \
        DWORD b = (DWORD)(ang*Get_B(OBJ3DPAINT_color));                           \
        RGB_Set (col,r,g,b);

#define NoClip                                                                    \
        TInfo *i1,*i2,*i3;                                                        \
				i1 = obj.inf+f->a;                                                        \
				i2 = obj.inf+f->b;                                                        \
				i3 = obj.inf+f->c;                                                        \
        OBJ3DPAINT_BZO->Begin (TRI_PI_MAP_FLAT_Paint);                            \
        OBJ3DPAINT_BZO->Push (v1p.x);                                             \
        OBJ3DPAINT_BZO->Push (v1p.y);                                             \
        OBJ3DPAINT_BZO->Push (v2p.x);                                             \
        OBJ3DPAINT_BZO->Push (v2p.y);                                             \
        OBJ3DPAINT_BZO->Push (v3p.x);                                             \
        OBJ3DPAINT_BZO->Push (v3p.y);                                             \
        OBJ3DPAINT_BZO->Push (i1->uu);                                            \
        OBJ3DPAINT_BZO->Push (i1->vv);                                            \
        OBJ3DPAINT_BZO->Push (i2->uu);                                            \
        OBJ3DPAINT_BZO->Push (i2->vv);                                            \
        OBJ3DPAINT_BZO->Push (i3->uu);                                            \
        OBJ3DPAINT_BZO->Push (i3->vv);                                            \
        SetLight                                                                  \
        OBJ3DPAINT_BZO->Push (col);                                               \
        OBJ3DPAINT_BZO->Push (f->i);                                              \
        OBJ3DPAINT_BZO->End (v1p.z,v2p.z,v3p.z);

#define ClipBefore                                                                \
        TInfo inc[3];                                                             \
				inc[0] = obj.inf[f->a];                                                   \
				inc[1] = obj.inf[f->b];                                                   \
				inc[2] = obj.inf[f->c];                                                   \
        clip = OBJ3DPAINT_TC->Clip (vr,3,(float *)inc,2);                         \
        if (clip!=0)                                                              \
        {                                                                         \
          TInfo *ii=(TInfo *)OBJ3DPAINT_TC->iOut;                                 

#define ClipBeforePaint SetLight

#define ClipAfter                                                                 \
        OBJ3DPAINT_BZO->Begin (TRI_PI_MAP_FLAT_Paint);                            \
        OBJ3DPAINT_BZO->Push (vp[0].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[0].y);                                           \
        OBJ3DPAINT_BZO->Push (vp[j-1].x);                                         \
        OBJ3DPAINT_BZO->Push (vp[j-1].y);                                         \
        OBJ3DPAINT_BZO->Push (vp[j].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[j].y);                                           \
				OBJ3DPAINT_BZO->Push (ii[0].uu);                                          \
				OBJ3DPAINT_BZO->Push (ii[0].vv);                                          \
				OBJ3DPAINT_BZO->Push (ii[j-1].uu);                                        \
				OBJ3DPAINT_BZO->Push (ii[j-1].vv);                                        \
				OBJ3DPAINT_BZO->Push (ii[j].uu);                                          \
				OBJ3DPAINT_BZO->Push (ii[j].vv);                                          \
        OBJ3DPAINT_BZO->Push (col);                                               \
        OBJ3DPAINT_BZO->Push (f->i);                                              \
        OBJ3DPAINT_BZO->End (vp[0].z,vp[j-1].z,vp[j].z);

  
OBJ3DPAINT_Order(OBJ3DPAINT_Order_PI_MAP_Flat_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_OrderRep_PI_MAP_Flat_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
