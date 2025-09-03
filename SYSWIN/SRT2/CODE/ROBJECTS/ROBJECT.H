//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Object
#define SystemR_V30_Object
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
extern TReal CompletDir;
extern TReal CompletPos;
extern TReal InvCompletDir;
extern TVertex SqrDir;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

extern long  R_ReturnStatus;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
typedef struct {

  OBjType       Type;
  TVertex     Position;
  dword         FLAGS;

  DefPigment    Pigment;

  TReal         IOR;
  byte          IOR_INTS;
  byte          REFL_INTS;
  long          Invers;
  long          TypeBump;

  TReal         PNG;
  TReal         PNG_S;
  TReal         Value1;

  void    *PortalToWorld1;
  void    *PortalToWorld2;

  void (*SetRGBColor)           ( byte , byte , byte )  ;
  void (*SetTexture)            ( rgbcol * )            ;
  void (*SetTextureMapping)     ( long )                ;
  void (*SetValuesIntersection) ( TVertex & )         ;

  void (*SetTextScale) ( TReal );
  void (*SetBump)      ( long );
  void (*SetProperty)  ( dword P );


  void (*GetColorInfo)                  ( RayInfo & , TVertex & );
  long (*GetPrimaryIntersection)        ( TVertex & , TReal & , TReal & ) ;
  long (*GetSecondaryIntersection)      ( TVertex & , TVertex & , TReal &, TReal &);
  long (*GetShadowIntersection)         ( TVertex & , TVertex & )   ;
  void (*GetNormal)                     ( TVertex & , TVertex &, TReal & , long );

} TObject_R;


extern TObject_R  *ActiveObject;

void TObject_R_SetRGBColor ( byte R , byte G , byte B );
void TObject_R_SetTexture  ( rgbcol *P );
void TObject_R_SetTextureMapping  ( long P );
void TObject_R_SetTextScale ( TReal P );
void TObject_R_SetProperty    ( dword P );
void TObject_R_SetBump       ( long P );


#define IsShadowed      (ActiveObject->FLAGS & R_RECIVE_SHADOW )
#define IsOpac          (ActiveObject->FLAGS & R_MAKE_SHADOW )
#define IsVisible       (ActiveObject->FLAGS & R_VISIBLE )
#define IsReflect       (ActiveObject->FLAGS & R_REFLECT )
#define IsReflectT      (ActiveObject->FLAGS & R_REFLECT_TEXT )
#define IsTransparent   (ActiveObject->FLAGS & R_TRANSPARENT )
#define IsTransparentT  (ActiveObject->FLAGS & R_TRANSPARENT_TEXT )
#define IsBumpped       (ActiveObject->FLAGS & R_BUMP )

#endif

