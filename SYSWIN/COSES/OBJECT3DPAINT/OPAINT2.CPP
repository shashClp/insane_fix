#include "paint.h"

#undef NoClip
#undef ClipBefore
#undef ClipBeforePaint
#undef ClipAfter

#define NoClip                                                                    \
        OBJ3DPAINT_BZO->Begin (TRI_FLAT50_Paint);                                 \
        OBJ3DPAINT_BZO->Push (v1p.x);                                             \
        OBJ3DPAINT_BZO->Push (v1p.y);                                             \
        OBJ3DPAINT_BZO->Push (v2p.x);                                             \
        OBJ3DPAINT_BZO->Push (v2p.y);                                             \
        OBJ3DPAINT_BZO->Push (v3p.x);                                             \
        OBJ3DPAINT_BZO->Push (v3p.y);                                             \
        OBJ3DPAINT_BZO->Push (OBJ3DPAINT_color);                                  \
        OBJ3DPAINT_BZO->End (v1p.z,v2p.z,v3p.z);

#define ClipBefore                                                                \
        clip = OBJ3DPAINT_TC->Clip (vr,3);                                        \
        if (clip!=0)                                                              \
        {

#define ClipBeforePaint

#define ClipAfter                                                                 \
        OBJ3DPAINT_BZO->Begin (TRI_FLAT50_Paint);                                 \
        OBJ3DPAINT_BZO->Push (vp[0].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[0].y);                                           \
        OBJ3DPAINT_BZO->Push (vp[j-1].x);                                         \
        OBJ3DPAINT_BZO->Push (vp[j-1].y);                                         \
        OBJ3DPAINT_BZO->Push (vp[j].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[j].y);                                           \
				OBJ3DPAINT_BZO->Push (OBJ3DPAINT_color);                                  \
        OBJ3DPAINT_BZO->End (vp[0].z,vp[j-1].z,vp[j].z);
  
OBJ3DPAINT_Order(OBJ3DPAINT_Order_Flat50,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_OrderRep_Flat50,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
