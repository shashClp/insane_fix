// AW3D - Anaconda 3D Windows Library
// Matrix

#ifndef AW3D_MATRIX
#define AW3D_MATRIX

#include <vertex.h>

#define MTX_INTERACTIVE_KEYBOARD

#ifdef MTX_INTERACTIVE_KEYBOARD
  extern TVertex MTX_IK_target;
  extern TVertex MTX_IK_pofv;
  extern bool MTX_IK_Initialized;
  extern float MTX_IK_roll,MTX_IK_ControlVelocity;
#endif

typedef float TMatrix[3][4];

extern void MTX_Identity (TMatrix &out);
extern bool MTX_IsIdentity(TMatrix &in);
extern void MTX_Copy (TMatrix &out, TMatrix &in);
extern void MTX_Inverse (TMatrix &b,TMatrix &a);
extern void MTX_MulNoT (TVertex &a,TVertex &b,TMatrix &c);
extern void MTX_Mul (TVertex &out,TVertex &in,TMatrix &mat);
extern void MTX_Mul (TMatrix &out,TMatrix &a,TMatrix &b);
extern void MTX_Rotate (TMatrix &out,float x,float y,float z);
extern void MTX_Scale (TMatrix &out,float x,float y,float z);
extern void MTX_Translate (TMatrix &out,float x,float y,float z);
extern void MTX_LookAt (TMatrix &out,
                        float orgx,float orgy,float orgz,
                        float finx,float finy,float finz,float roll);                        
extern void MTX_LookAtWithOutIK (TMatrix &out,
                                 float orgx,float orgy,float orgz,
                                 float finx,float finy,float finz,float roll);

#endif