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
        DWORD r = (DWORD)(ang*Get_R(OBJ3DPAINT_color));                       \
        DWORD g = (DWORD)(ang*Get_G(OBJ3DPAINT_color));                       \
        DWORD b = (DWORD)(ang*Get_B(OBJ3DPAINT_color));                       \
        RGB_Set (col,r,g,b);                                                      \

#define NoClip                                                                    \
				TPTRI_FLAT src; \
				src.x1 = v1p.x;                                             \
				src.y1 = v1p.y;                                             \
				src.x2 = v2p.x;                                             \
				src.y2 = v2p.y;                                             \
				src.x3 = v3p.x;                                             \
				src.y3 = v3p.y;                                             \
        SetLight                                                                  \
        src.col = col;                                               \
				TRI_FLAT_BORDER_Paint(&src);        

#define ClipBefore                                                                \
        clip = OBJ3DPAINT_TC->Clip (vr,3);                                        \
        if (clip!=0)                                                              \
        {

#define ClipBeforePaint SetLight

#define ClipAfter                                                                 \
				TPTRI_FLAT src; \
				src.x1 = vp[0].x;                                             \
				src.y1 = vp[0].y;                                             \
				src.x2 = vp[j-1].x;                                             \
				src.y2 = vp[j-1].y;                                             \
				src.x3 = vp[j].x;                                             \
				src.y3 = vp[j].y;                                             \
        src.col = col;                                               \
				TRI_FLAT_BORDER_Paint(&src);
  
OBJ3DPAINT_Order(OBJ3DPAINT_Flat_Border_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_Rep_Flat_Border_Light,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);

#undef SetLight