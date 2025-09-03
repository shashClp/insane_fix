//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_World
#define SystemR_V30_World

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include <time.h>
#include "..\RDEFINES\RDEFINES.H"
#include "RSCENE.H"
#include "..\REFECTS\RLIGHT.H"
#include "RCAMERA.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
extern long R_RAY_DIM;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

#define NumMaxObjectes 20000
#define NumMaxLight 10
#define NumMaxWorldList 10
#define ERRORCODE 6565                  // Cal controlar... si es vol.

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef struct{

  word         Num_Of_Obj;
  word         Num_Of_Lights;

  TObject_R         *Objects[NumMaxObjectes];
  TLightPoint_R     *Lights[NumMaxLight];
  byte              ShadowList[NumMaxObjectes];
  TReal             AMBIENT;
  dword             Flags;
  long              Precalculated;

  void (*GetFrame)  ( TCamera_R *, long SX, long SY, long Deep , long GX, long Gy, rgbcol *Dst);

} TWorld_R ;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

extern TWorld_R    *ActiveWorld;
extern long        IDActiveObject;
extern long        IDActiveLight;
extern TWorld_R    *WorldList[NumMaxWorldList];
extern long        IndexWorldLis;


extern long        RPrimRay;
extern long        RShadRay;
extern long        RSecRay;
extern long        RHitPrimRay;
extern long        RHitSecRay;
extern long        RTestInt1;
extern long        RTestInt2;
extern long        RTestInt3;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifdef SYR_PROCEDURAL_TEXTURES
#include "..\REFECTS\RTEXT.H"
#endif
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

extern long RBorderV ;
extern long RBorderH ;
extern float R_FAR_CLIP;

extern long  R_FOG;
extern float R_FOG_DISTANCE_FACTOR;
extern float R_FOG_R;
extern float R_FOG_B;
extern float R_FOG_G;
extern long R_BACK_COLOR;

extern long R_RAY_DIM ;
extern long R_ScreenX ;
extern long R_ScreenY ;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

extern void            SystemR_V30_Init();
extern void            TWorld_Reflect ( long &ID,TVertex &Dir,TVertex &Norm,TVertex &Check,RayInfo &RayPtr,int Deep,TReal const &Index,long const &Fact);
extern void            TWorld_Init ( TVertex Pos);
extern void            TWorld_GetFrame ( TCamera_R *Panasonic, long SX, long SY, long Deep , long GX, long GY, rgbcol *DST);
extern TWorld_R *      CreateWorldDefault();
extern void            DestroyWorld(TWorld_R *M);

extern long     Ray_ADD_Object ( TObject_R *Obj );
extern void     Ray_Del_Object ( long NObj );
extern long     Ray_ADD_Light ( TLightPoint_R *Ll );
extern void 	Ray_Active_Object_Internal ( long ID );
extern void 	Ray_Active_Object ( long ID );
extern void     Ray_Active_Light ( long ID );
extern void     Ray_Active_World ( TWorld_R *ID );
extern void     Ray_Active_Camera ( TCamera_R *ID );
extern void     Ray_Initialize_World ( TWorld_R *ID );
extern void	Ray_PrecalcIntersection ( TVertex &Pos );
extern void     RW_Enable ( dword P );
extern void     RW_Disable ( dword P );
extern void 	Ray_GetColorInfo ( RayInfo &RayPtr,TVertex &CH);

extern void 	Ray_Set_Color ( byte R, byte G, byte B );
extern void 	Ray_Set_Texture ( rgbcol *P );
extern void 	Ray_Set_TextureMode ( long P );
extern void 	Ray_Set_TextScale ( TReal P );
extern void 	Ray_Set_Bump ( long P );
extern void 	Ray_Set_ReflectInts ( byte P );
extern void 	Ray_Set_IORValues ( TReal P , byte I );
extern void 	Ray_Set_Phong ( TReal Value, TReal Size );
extern void 	Ray_SetPortalTo ( TWorld_R *W1 );
extern void 	Ray_SetTranslate ( TVertex &P );
extern void 	Ray_Enable ( dword P );
extern void 	Ray_Disable ( dword P );

extern long 	Ray_GetPrimaryIntersection ( TVertex &Dir, TReal &T0, TReal &T1 );
extern long 	Ray_GetSecondaryIntersection ( TVertex &Ch, TVertex &Dir, TReal &T0, TReal &T1 );
extern long 	Ray_GetShadowIntersection ( TVertex &P1 , TVertex &P2 );
extern float 	Ray_GetDiffuseLight( TVertex &P1 , TVertex &P2 );
extern float 	Ray_GetPhongLight( TVertex &P1 , TVertex &P2,TReal R1,TReal R2);
extern TReal 	Ray_GetObjectPhong ();
extern TReal 	Ray_GetObjectPhongSize ();


//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
extern byte *Bounder;
extern byte *BounderCheck;
extern long RCheck_Bounder_Range;

extern void Bounder_Init(long X,long Y,long GX, long GY);
extern void Bounder_Close();
extern void Check_Bounder(long X,long Y);


extern long FallasValue = 0;
#endif
