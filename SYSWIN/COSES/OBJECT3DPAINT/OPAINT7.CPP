#include "paint.h"

#undef NoClip
#undef ClipBefore
#undef ClipBeforePaint
#undef ClipAfter

#define NoClip                                                                    \
        TInfo *i1,*i2,*i3;                                                        \
				i1 = obj.inf+f->a;                                                        \
				i2 = obj.inf+f->b;                                                        \
				i3 = obj.inf+f->c;                                                        \
        OBJ3DPAINT_BZO->Begin (TRI_PI_MAP_DOUBLE_Paint);                          \
        OBJ3DPAINT_BZO->Push (v1p.x);                                             \
        OBJ3DPAINT_BZO->Push (v1p.y);                                             \
        OBJ3DPAINT_BZO->Push (v2p.x);                                             \
        OBJ3DPAINT_BZO->Push (v2p.y);                                             \
        OBJ3DPAINT_BZO->Push (v3p.x);                                             \
        OBJ3DPAINT_BZO->Push (v3p.y);                                             \
        OBJ3DPAINT_BZO->Push (i1->uu);                                            \
        OBJ3DPAINT_BZO->Push (i1->vv);                                            \
        OBJ3DPAINT_BZO->Push (i1->uu+OBJ3DPAINT_despX);                           \
        OBJ3DPAINT_BZO->Push (i1->vv+OBJ3DPAINT_despY);                           \
        OBJ3DPAINT_BZO->Push (i2->uu);                                            \
        OBJ3DPAINT_BZO->Push (i2->vv);                                            \
        OBJ3DPAINT_BZO->Push (i2->uu+OBJ3DPAINT_despX);                           \
        OBJ3DPAINT_BZO->Push (i2->vv+OBJ3DPAINT_despY);                           \
        OBJ3DPAINT_BZO->Push (i3->uu);                                            \
        OBJ3DPAINT_BZO->Push (i3->vv);                                            \
        OBJ3DPAINT_BZO->Push (i3->uu+OBJ3DPAINT_despX);                           \
        OBJ3DPAINT_BZO->Push (i3->vv+OBJ3DPAINT_despY);                           \
        OBJ3DPAINT_BZO->Push (f->i);                                              \
        OBJ3DPAINT_BZO->Push (OBJ3DPAINT_txt2);                                   \
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

#define ClipBeforePaint                                                           \
        float ii0u = ii[0].uu+OBJ3DPAINT_despX;                                   \
        float ii0v = ii[0].vv+OBJ3DPAINT_despY;

#define ClipAfter                                                                 \
        OBJ3DPAINT_BZO->Begin (TRI_PI_MAP_DOUBLE_Paint);                          \
        OBJ3DPAINT_BZO->Push (vp[0].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[0].y);                                           \
        OBJ3DPAINT_BZO->Push (vp[j-1].x);                                         \
        OBJ3DPAINT_BZO->Push (vp[j-1].y);                                         \
        OBJ3DPAINT_BZO->Push (vp[j].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[j].y);                                           \
				OBJ3DPAINT_BZO->Push (ii[0].uu);                                          \
				OBJ3DPAINT_BZO->Push (ii[0].vv);                                          \
				OBJ3DPAINT_BZO->Push (ii0u);                                              \
				OBJ3DPAINT_BZO->Push (ii0v);                                              \
				OBJ3DPAINT_BZO->Push (ii[j-1].uu);                                        \
				OBJ3DPAINT_BZO->Push (ii[j-1].vv);                                        \
				OBJ3DPAINT_BZO->Push (ii[j-1].uu+OBJ3DPAINT_despX);                       \
				OBJ3DPAINT_BZO->Push (ii[j-1].vv+OBJ3DPAINT_despY);                       \
				OBJ3DPAINT_BZO->Push (ii[j].uu);                                          \
				OBJ3DPAINT_BZO->Push (ii[j].vv);                                          \
				OBJ3DPAINT_BZO->Push (ii[j].uu+OBJ3DPAINT_despX);                         \
				OBJ3DPAINT_BZO->Push (ii[j].vv+OBJ3DPAINT_despY);                         \
				OBJ3DPAINT_BZO->Push (f->i);                                              \
        OBJ3DPAINT_BZO->Push (OBJ3DPAINT_txt2);                                   \
        OBJ3DPAINT_BZO->End (vp[0].z,vp[j-1].z,vp[j].z);
  
OBJ3DPAINT_Order(OBJ3DPAINT_Order_PI_MAP_DOUBLE_Desplacement,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Desplacement,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);

