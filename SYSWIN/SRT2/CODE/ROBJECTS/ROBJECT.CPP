//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
#include "ROBJECT.H"

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
TReal CompletDir;
TReal CompletPos;
TReal InvCompletDir;
TVertex SqrDir;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

long    R_ReturnStatus;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

TObject_R  *ActiveObject;


void TObject_R_SetRGBColor ( byte R , byte G , byte B )
{
#ifdef R_LOG_DEF
  R_LOG ("    Color   : <%3d,%3d,%3d>",R,G,B);
#endif
  ActiveObject->Pigment.Color.B.R = R;
  ActiveObject->Pigment.Color.B.G = G;
  ActiveObject->Pigment.Color.B.B = B;
}

void TObject_R_SetTexture  ( rgbcol *P )
{
#ifdef R_LOG_DEF
  R_LOG ("    Texture: <%p>",P);
#endif
  ActiveObject->Pigment.TexMap = P;
}

void TObject_R_SetTextureMapping  ( long P )
{
#ifdef R_LOG_DEF
  R_LOG ("    MapTexture: <%d>",P);
#endif
  ActiveObject->Pigment.TxtMode = P;
}

void TObject_R_SetTextScale ( TReal P )
{
#ifdef R_LOG_DEF
  R_LOG ("    TextScale: <%f>",P);
#endif
  ActiveObject->Pigment.TextScale = P;
  ActiveObject->Pigment.InvTextScale = 1.0/P;
}


void TObject_R_SetProperty    ( dword P )
{
  ActiveObject->FLAGS = ActiveObject->FLAGS | P;
#ifdef R_LOG_DEF
  switch (P)
  {
    case R_RECIVE_SHADOW   : R_LOG ("    Set: RECIVE_SHADOW"); break;
    case R_MAKE_SHADOW     : R_LOG ("    Set: MAKE_SHADOW"); break;
    case R_VISIBLE         : R_LOG ("    Set: VISIBLE"); break;
    case R_REFLECT         : R_LOG ("    Set: REFLECT"); break;
    case R_TRANSPARENT     : R_LOG ("    Set: TRANSPARENT"); break;
    case R_BUMP            : R_LOG ("    Set: BUMP"); break;
    case R_REFLECT_TEXT    : R_LOG ("    Set: REFLECT_TEXT"); break;
    case R_MASK_TEXT       : R_LOG ("    Set: MASK_TEXT"); break;
    case R_CENTRAL_TEXT    : R_LOG ("    Set: CENTRAL_TEXT"); break;
    case R_PORTAL_TEXT     : R_LOG ("    Set: PORTAL_TEXT"); break;
    case R_VAMPIRE         : R_LOG ("    Set: VAMPIRE"); break;
    case R_TRANSPARENT_TEXT: R_LOG ("    Set: TRANSPARENT_TEXT"); break;
  }
#endif

}

void TObject_R_SetBump       ( long P )
{
  ActiveObject->FLAGS = ActiveObject->FLAGS | R_BUMP;
  ActiveObject->TypeBump = P;
}


