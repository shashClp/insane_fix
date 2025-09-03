#include <awl.h>
#include <awm.h>
#include <aw3d.h>
#include <a3d.h>
#include <default.h>

#include <joinalpha.h>
#include <rotozoom.h>
#include <deformation8b.h>
#include <math.h>

TImage *img1,*img2;
TImage *alpha1;
TView *v1,*v2;
BYTE *alpha1b;

void main (int argc,char *argv[])
{
  if (!DEFAULT_Init ()) return;

  effRotoZoom = new effectRotoZoom();
  effJoinAlpha = new effectJoinAlpha();
  effDeformation8b = new effectDeformation8b();

  TImage *img;

  img = IMAGE_Load ("c:\\demos\\elric\\p7\\text1.tga");
  img1 = (TImage *) GetMem (sizeof(TImage));
  IMAGE_24To32 (img1,img);
  FreeMem (img);

  img = IMAGE_Load ("c:\\demos\\elric\\p7\\text2.tga");
  img2 = (TImage *) GetMem (sizeof(TImage));
  IMAGE_24To32 (img2,img);
  FreeMem (img);

  alpha1 = IMAGE_Load ("c:\\demos\\elric\\p7\\alpha2.gif");
  alpha1b = (BYTE *) GetMem (GBL_Width*GBL_Height);
  
  v1 = VIEW_Init ();
      
  effRotoZoom->setDimension (GBL_Width,GBL_Height);
  effJoinAlpha->setDimension (GBL_Width,GBL_Height);
  effDeformation8b->setDimension (GBL_Width,GBL_Height);

  float a = 0;
  float b = 21;
  float c = 323;
  
  PROFILE_DefineSection (2,"RotoZoomer 1",0xff00);
  PROFILE_DefineSection (3,"RotoZoomer 2",0xff);
  PROFILE_DefineSection (4,"Deformation8b",0xff00ff);
  PROFILE_DefineSection (5,"Alpha",0xffff);
  while (!Exit)
  {
    PBEGIN(2)
    effRotoZoom->DstBuffer = v1->bits;
    effRotoZoom->SrcBuffer = (DWORD *)img1->bits;
    effRotoZoom->makeEffect2(512*sin(a*M_ToRad),512*sin(c*M_ToRad),a,a);
    PEND

    PBEGIN(3)
    effRotoZoom->DstBuffer = GBL_Bits;
    effRotoZoom->SrcBuffer = (DWORD *)img2->bits;
    effRotoZoom->makeEffect2(512*sin(b*M_ToRad),512*cos(a*M_ToRad),b,b);
    PEND

    PBEGIN(4)
    effDeformation8b->SrcBuffer = (BYTE *)alpha1->bits;
    effDeformation8b->DstBuffer = alpha1b;    
    effDeformation8b->setEffect1 (2,2,30,30);  
    effDeformation8b->makeEffect1 (sin(a*M_ToRad),sin(b*M_ToRad));
    PEND

    PBEGIN(5)
    effJoinAlpha->AlphaBuffer = alpha1b;
    effJoinAlpha->DstBuffer = GBL_Bits;
    effJoinAlpha->Src1Buffer = v1->bits;
    effJoinAlpha->Src2Buffer = GBL_Bits;
    effJoinAlpha->makeEffect();
    PEND
    
    DEFAULT_Flip();    

    a+=1.5f;
    b-=1.25f;
    c-=1.45f;
  }

  delete (effDeformation8b);
  delete (effRotoZoom);
  delete (effJoinAlpha);
  FreeMem (v1);
  FreeMem (v2);
  FreeMem (img1);
  FreeMem (img2);
  FreeMem (alpha1);
  FreeMem (alpha1b);

  DEFAULT_Close();
}