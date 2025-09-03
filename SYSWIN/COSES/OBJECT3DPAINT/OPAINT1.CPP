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
        RGB_Set (col,r,g,b);                                                      \

#define NoClip                                                                    \
        OBJ3DPAINT_BZO->Begin (TRI_FLAT_Paint);                                   \
        OBJ3DPAINT_BZO->Push (v1p.x);                                             \
        OBJ3DPAINT_BZO->Push (v1p.y);                                             \
        OBJ3DPAINT_BZO->Push (v2p.x);                                             \
        OBJ3DPAINT_BZO->Push (v2p.y);                                             \
        OBJ3DPAINT_BZO->Push (v3p.x);                                             \
        OBJ3DPAINT_BZO->Push (v3p.y);                                             \
        SetLight                                                                  \
        OBJ3DPAINT_BZO->Push (col);                                               \
        OBJ3DPAINT_BZO->End (v1p.z,v2p.z,v3p.z);

#define ClipBefore                                                                \
        clip = OBJ3DPAINT_TC->Clip (vr,3);                                        \
        if (clip!=0)                                                              \
        {

#define ClipBeforePaint SetLight

#define ClipAfter                                                                 \
        OBJ3DPAINT_BZO->Begin (TRI_FLAT_Paint);                                   \
        OBJ3DPAINT_BZO->Push (vp[0].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[0].y);                                           \
        OBJ3DPAINT_BZO->Push (vp[j-1].x);                                         \
        OBJ3DPAINT_BZO->Push (vp[j-1].y);                                         \
        OBJ3DPAINT_BZO->Push (vp[j].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[j].y);                                           \
				OBJ3DPAINT_BZO->Push (col);                                               \
        OBJ3DPAINT_BZO->End (vp[0].z,vp[j-1].z,vp[j].z);
  
OBJ3DPAINT_Order(OBJ3DPAINT_Order_Flat_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_OrderRep_Flat_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);

#undef SetLight