// AWL - Anaconda Windows Library
// Defines section

#ifndef AWL_DEFINES
#define AWL_DEFINES

#define M_PI                  (float)3.141621
#define M_PIx2                M_PI*(float)2.0
#define M_PIDiv2              M_PI/(float)2.0
#define M_ToRad               M_PI/(float)180.0
#define M_ToGrd               (float)180.0/M_PI
#define M_ZeroFloat2Int       (int)0.0
#define M_EPSILON             (float)0.00001
#define M_1Div3               (float)0.333333333f
#define M_2Div3               (float)0.666666666f

#define TestBit(a,b)          ((a&b)==b)
#define sqr(a)                a*a

#endif
