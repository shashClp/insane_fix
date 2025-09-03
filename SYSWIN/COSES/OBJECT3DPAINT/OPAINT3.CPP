#include "paint.h"

#undef NoClip
#undef ClipBefore
#undef ClipBeforePaint
#undef ClipAfter

#define SetLight                                                                  \
        float ang = VTX_DotProduct (*((TVertex *)p),OBJ3DPAINT_Llum);             \
        ang *=127.0f; ang +=128.0f;                                               \
        DWORD col;                                                                \
        DWORD r = (DWORD)(ang*OBJ3DPAINT_R)/256;                                  \
        DWORD g = (DWORD)(ang*OBJ3DPAINT_G)/256;                                  \
        DWORD b = (DWORD)(ang*OBJ3DPAINT_B)/256;                                  \
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
  
OBJ3DPAINT_Order(OBJ3DPAINT_Order_Gouraud_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_OrderRep_Gouraud_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);

#undef SetLight