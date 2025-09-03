//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_GetFunctions
#define SystemR_V30_GetFunctions
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²

inline long Ray_GetPrimaryIntersection ( TVertex &Dir, TReal &T0, TReal &T1 );
inline long Ray_GetSecondaryIntersection ( TVertex &Ch, TVertex &Dir, TReal &T0, TReal &T1 );
inline long Ray_GetShadowIntersection ( TVertex &P1 , TVertex &P2 );
inline float Ray_GetDiffuseLight( TVertex &P1 , TVertex &P2 );
inline float Ray_GetPhongLight( TVertex &P1 , TVertex &P2,TReal R1,TReal R2);
inline TReal Ray_GetObjectPhong ();
inline TReal Ray_GetObjectPhongSize ();

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²         Get Functions
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline long Ray_GetPrimaryIntersection ( TVertex &Dir, TReal &T0, TReal &T1 )
{
  RTestInt1++;
  return ActiveObject->GetPrimaryIntersection (Dir,T0,T1);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline long Ray_GetSecondaryIntersection ( TVertex &Ch, TVertex &Dir, TReal &T0, TReal &T1 )
{
  RTestInt2++;
  return ActiveObject->GetSecondaryIntersection (Ch,Dir,T0,T1);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline long Ray_GetShadowIntersection ( TVertex &P1 , TVertex &P2 )
{
  RTestInt3++;
  return ActiveObject->GetShadowIntersection (P1,P2);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline float Ray_GetDiffuseLight( TVertex &P1 , TVertex &P2 )
{
  TReal R;
  TLightPoint_R_Diffuse (P1,P2,R);
  return R;
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline float Ray_GetPhongLight( TVertex &P1 , TVertex &P2,TReal R1,TReal R2)
{
  return ActiveLight->TLightPoint_R_Phong (P1,P2,R1,R2);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline TReal Ray_GetObjectPhong ()
{
  return ActiveObject->PNG;
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
inline TReal Ray_GetObjectPhongSize ()
{
  return ActiveObject->PNG_S;
}


#endif
