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
        TVertex v1l,v2l,v3l;                                                      \
		    VTX_Sub (v1l,OBJ3DPAINT_Llum,obj.nrm[f->a]);                              \
        VTX_Sub (v2l,OBJ3DPAINT_Llum,obj.nrm[f->b]);                              \
        VTX_Sub (v3l,OBJ3DPAINT_Llum,obj.nrm[f->c]);                              \
        OBJ3DPAINT_BZO->Begin (TRI_PI_MAP_DOUBLE_Paint);                          \
        OBJ3DPAINT_BZO->Push (v1p.x);                                             \
        OBJ3DPAINT_BZO->Push (v1p.y);                                             \
        OBJ3DPAINT_BZO->Push (v2p.x);                                             \
        OBJ3DPAINT_BZO->Push (v2p.y);                                             \
        OBJ3DPAINT_BZO->Push (v3p.x);                                             \
        OBJ3DPAINT_BZO->Push (v3p.y);                                             \
        OBJ3DPAINT_BZO->Push (i1->uu);                                            \
        OBJ3DPAINT_BZO->Push (i1->vv);                                            \
        OBJ3DPAINT_BZO->Push (127.0f+v1l.x*127.0f);                                      \
        OBJ3DPAINT_BZO->Push (127.0f+v1l.y*127.0f);                                      \
        OBJ3DPAINT_BZO->Push (i2->uu);                                            \
        OBJ3DPAINT_BZO->Push (i2->vv);                                            \
        OBJ3DPAINT_BZO->Push (127.0f+v2l.x*127.0f);                                      \
        OBJ3DPAINT_BZO->Push (127.0f+v2l.y*127.0f);                                      \
        OBJ3DPAINT_BZO->Push (i3->uu);                                            \
        OBJ3DPAINT_BZO->Push (i3->vv);                                            \
        OBJ3DPAINT_BZO->Push (127.0f+v3l.x*127.0f);                                      \
        OBJ3DPAINT_BZO->Push (127.0f+v3l.y*127.0f);                                      \
        OBJ3DPAINT_BZO->Push (f->i);                                              \
        OBJ3DPAINT_BZO->Push (OBJ3DPAINT_txt2);                                   \
        OBJ3DPAINT_BZO->End (v1p.z,v2p.z,v3p.z);

#define ClipBefore                                                                \
        float inc[3][4];                                                          \
				inc[0][0] = obj.inf[f->a].uu;                                             \
				inc[0][1] = obj.inf[f->a].vv;                                             \
				inc[1][0] = obj.inf[f->b].uu;                                             \
				inc[1][1] = obj.inf[f->b].vv;                                             \
				inc[2][0] = obj.inf[f->c].uu;                                             \
				inc[2][1] = obj.inf[f->c].vv;                                             \
        TVertex v1l,v2l,v3l;                                                      \
		    VTX_Sub (v1l,OBJ3DPAINT_Llum,obj.nrm[f->a]);                              \
        VTX_Sub (v2l,OBJ3DPAINT_Llum,obj.nrm[f->b]);                              \
        VTX_Sub (v3l,OBJ3DPAINT_Llum,obj.nrm[f->c]);                              \
        inc[0][2] = 127.0f+v1l.x*127.0f;                                                 \
        inc[0][3] = 127.0f+v1l.y*127.0f;                                                 \
        inc[1][2] = 127.0f+v2l.x*127.0f;                                                 \
        inc[1][3] = 127.0f+v2l.y*127.0f;                                                 \
        inc[2][2] = 127.0f+v3l.x*127.0f;                                                 \
        inc[2][3] = 127.0f+v3l.y*127.0f;                                                 \
        clip = OBJ3DPAINT_TC->Clip (vr,3,(float *)inc,4);                         \
        if (clip!=0)                                                              \
        {                                                                         \
          float *ii = OBJ3DPAINT_TC->iOut;

#define ClipBeforePaint

#define ClipAfter                                                                 \
        OBJ3DPAINT_BZO->Begin (TRI_PI_MAP_DOUBLE_Paint);                          \
        OBJ3DPAINT_BZO->Push (vp[0].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[0].y);                                           \
        OBJ3DPAINT_BZO->Push (vp[j-1].x);                                         \
        OBJ3DPAINT_BZO->Push (vp[j-1].y);                                         \
        OBJ3DPAINT_BZO->Push (vp[j].x);                                           \
        OBJ3DPAINT_BZO->Push (vp[j].y);                                           \
				OBJ3DPAINT_BZO->Push (ii,4);                                              \
				OBJ3DPAINT_BZO->Push (ii+(j-1)*4,4);                                      \
				OBJ3DPAINT_BZO->Push (ii+j*4,4);                                          \
				OBJ3DPAINT_BZO->Push (f->i);                                              \
        OBJ3DPAINT_BZO->Push (OBJ3DPAINT_txt2);                                   \
        OBJ3DPAINT_BZO->End (vp[0].z,vp[j-1].z,vp[j].z);
  
OBJ3DPAINT_Order(OBJ3DPAINT_Order_PI_MAP_DOUBLE_Environemnt,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);
OBJ3DPAINT_Order(OBJ3DPAINT_OrderRep_PI_MAP_DOUBLE_Environemnt,NoClip,ClipBefore,ClipBeforePaint,ClipAfter);

