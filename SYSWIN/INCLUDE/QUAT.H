// AW3D - Anaconda 3D Windows Library
// Quaternion

#ifndef AW3D_QUAT
#define AW3D_QUAT

#include <windows.h>
#include <matrix.h>

#define QUAT_EPSILON 1.0e-6f

typedef struct {
  float w,x,y,z;
} TQuat;

void  QUAT_Set              (TQuat &a, float w, float x, float y, float z);
void  QUAT_Add              (TQuat &a, TQuat &b);
void  QUAT_Add              (TQuat &a, TQuat &b, TQuat &c);
void  QUAT_Sub              (TQuat &a, TQuat &b);
void  QUAT_Sub              (TQuat &a, TQuat &b, TQuat &c);
void  QUAT_Mul              (TQuat &a, float k);
void  QUAT_Mul              (TQuat &a, TQuat &b);
void  QUAT_Mul              (TQuat &r, TQuat &a, TQuat &b);
void  QUAT_Div              (TQuat &a, TQuat &b);
void  QUAT_Neg              (TQuat &a);
void  QUAT_Identity         (TQuat &a);
void  QUAT_Square           (TQuat &a);
void  QUAT_Sqrt             (TQuat &a);
void  QUAT_Normalize        (TQuat &a);
void  QUAT_Inverse          (TQuat &a);
void  QUAT_Exp              (TQuat &a);
void  QUAT_Log              (TQuat &a);
void  QUAT_LnDif            (TQuat &a, TQuat &b);
void  QUAT_QuatFromAngle    (TQuat &q, float a, float x, float y, float z);
void  QUAT_QuatToAngle      (TQuat q, float &ang, float &x, float &y, float &z);
void  QUAT_Slerp            (TQuat &a, TQuat &b, float t);
float QUAT_Length           (TQuat &a);
float QUAT_DotProduct       (TQuat &a, TQuat &b);
float QUAT_DotUnitProduct   (TQuat &a, TQuat &b);
void  QUAT_RotationMatrix   (TMatrix &m, TQuat &a);

#endif

