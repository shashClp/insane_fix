#include "paint.h"

#undef NoClip
#undef ClipBefore
#undef ClipBeforePaint
#undef ClipAfter

#define NoClip                                                                    \
        TPTRI_FLAT src;                                                           \
        src.x1 = v1p.x;                                                           \
        src.y1 = v1p.y;                                                           \
        src.x2 = v2p.x;                                                           \
        src.y2 = v2p.y;                                                           \
        src.x3 = v3p.x;                                                           \
        src.y3 = v3p.y;                                                           \
        src.col = OBJ3DPAINT_color;                                               \
        TRI_FLAT50_Paint (&src);

#define ClipBefore                                                                \
        clip = OBJ3DPAINT_TC->Clip (vr,3);                                        \
        if (clip!=0)                                                              \
        {

#define ClipBeforePaint

#define ClipAfter                                                                 \
        TPTRI_FLAT src;                                                           \
        src.x1 = vp[0].x;                                                         \
        src.y1 = vp[0].y;                                                         \
        src.x2 = vp[j-1].x;                                                       \
        src.y2 = vp[j-1].y;                                                       \
        src.x3 = vp[j].x;                                                         \
        src.y3 = vp[j].y;                                                         \
        src.col = OBJ3DPAINT_color;                                               \
        TRI_FLAT50_Paint (&src);
  
OBJ3DPAINT_Order(OBJ3DPAINT_Flat50,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_Rep_Flat50,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
