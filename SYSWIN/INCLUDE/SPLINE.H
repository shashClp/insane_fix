// AW3D - Anaconda 3D Windows Library
// Spline

#ifndef AW3D_SPLINE
#define AW3D_SPLINE

#include <windows.h>
#include <vertex.h>
#include <quat.h>

#define SPLINE_LOOP        0x0001
#define SPLINE_GLOBAL_LOOP 0x0002

typedef struct _TKey {
  TVertex pos;
  TVertex ds, dd;

  TQuat quat, qa;
  TQuat dsq, ddq;

  float tens, cont, bias, easeFrom, easeTo;
  WORD frame;
  _TKey *next;
} TKey;

typedef struct {
  WORD numKeys, start, end, frames, sceneframes;
  TKey *keys, *last;
} TSpline;

///////////////////////////////////////////////////////////////////////////////////

void SPLINE_Init                    (TSpline *a);
void SPLINE_InitRotation            (TSpline *a);
void SPLINE_Free                    (TSpline *a);
void SPLINE_Copy                    (TSpline *a, TSpline *b);
void SPLINE_CopyKey                 (TKey *a, TKey *b);
void SPLINE_AddKey                  (TSpline *a, TKey *k);
bool SPLINE_GetRealFrameValues      (float &value, TSpline *a, float frame, DWORD flags);
bool SPLINE_GetVertexFrameValues    (TVertex &r, TSpline *a, float frame, DWORD flags);
bool SPLINE_GetQuaternionFrameValues(TMatrix &m, TSpline *a, float frame, DWORD flags);

///////////////////////////////////////////////////////////////////////////////////

void  SPLINE_CompElementDeriv(float  pp, float   p, float  pn,
                              float *ds, float *dd, float ksm,
                              float ksp, float kdm, float kdp);
void  SPLINE_CompDeriv       (TKey *keyp, TKey *key, TKey *keyn);
void  SPLINE_CompDerivFirst  (TKey *key, TKey *keyn, TKey *keynn);
void  SPLINE_CompDerivLast   (TKey *keypp, TKey *keyp, TKey *key);
void  SPLINE_CompDerivTwo    (TKey *key, TKey *keyn);
void  SPLINE_CompDerivAB     (TKey *keyp, TKey *key, TKey *keyn);
float SPLINE_SplineEase      (float t, float a, float b);

#endif
