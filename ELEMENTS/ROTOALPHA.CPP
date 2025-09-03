#include <windows.h>
#include <joinalpha.h>
#include <defines.h>
#include <efx2d.h>
#include <math.h>
#include <guio.h>
#include <halo_light.h>
#include <mem.h>
#include <log.h>
#include <view.h>
#include <image.h>
#include <rotozoom.h>

TImage *ROTOALPHA_img1,*ROTOALPHA_img2;
TImage *ROTOALPHA_alpha1;
TImage *ROTOALPHA_flare;
TImage *ROTOALPHA_Sol1;
TImage *ROTOALPHA_Sol2;
TView *ROTOALPHA_v1;
float ROTOALPHA_flaresx;
float ROTOALPHA_flaresy;
float ROTOALPHA_flaresi;
DWORD ROTOALPHA_flaresnum;
float ROTOALPHA_solsgrow;

bool ROTOALPHA_Init()
{
  SEC_START ("Initializing ROTOALPHA");

  effRotoZoom = new effectRotoZoom();
  effJoinAlpha = new effectJoinAlpha();
  

  ROTOALPHA_img1 = IMAGE_Load ("data\\fons1.tga",32);
  if (!ROTOALPHA_img1) return false;

  ROTOALPHA_img2 = IMAGE_Load ("data\\fons2.tga",32);
  if (!ROTOALPHA_img2) return false;

  ROTOALPHA_alpha1 = IMAGE_Load ("data\\alpha1.gif",8);
  
  ROTOALPHA_v1 = VIEW_Init ();
      
  effRotoZoom->setDimension (GBL_Width,GBL_Height);
  effJoinAlpha->setDimension (GBL_Width,GBL_Height);  

  ROTOALPHA_flare = IMAGE_Load ("data\\flare.tga",32);
  if (ROTOALPHA_flare==NULL) return false;  

  ROTOALPHA_Sol1 = IMAGE_Load ("data\\sol1.tga",32);
  if (ROTOALPHA_Sol1==NULL) return false;  

  ROTOALPHA_Sol2 = IMAGE_Load ("data\\sol2.tga",32);
  if (ROTOALPHA_Sol2==NULL) return false;  

  ROTOALPHA_flaresi = 0;
  ROTOALPHA_flaresnum = 8;
  ROTOALPHA_solsgrow = 0;

  LOG ("ROTOALPHA initialized!");
  SEC_END();
  return true;
}

void ROTOALPHA_SolsGrow(float tick)
{
  ROTOALPHA_solsgrow = 40.0f*sin(tick*M_PI);  
}

void ROTOALPHA_Sols(float tick)
{
DWORD i;

  for (i=0; i<4; i++)
  {
    float t=120.0f+80.0f*sin(tick*i*8*M_PI);
    PutPartRGB (GBL_Bits,(DWORD *)ROTOALPHA_Sol1->bits,40,t,(DWORD)(ROTOALPHA_solsgrow+32));
  }
  for (i=0; i<4; i++)
  {
    float t=120.0f+80.0f*cos(tick*i*8*M_PI);
    PutPartRGB (GBL_Bits,(DWORD *)ROTOALPHA_Sol2->bits,GBL_Width-40,t,(DWORD)(ROTOALPHA_solsgrow+32));
  }
}

void ROTOALPHA_Flares(float tick)
{
  float t = tick*32;
  float pt1 = 60+25*(float)sin(tick*2*M_PIx2);
  float pt2 = 40+25*(float)cos(tick*3*M_PIx2);
  DWORD ppx,ppy;
  // Posar flares 
  for (DWORD i=0; i<ROTOALPHA_flaresnum; i++)
  {
    ppx = (DWORD)(ROTOALPHA_flaresx+(pt1*(float)sin(((float)i+t)*4*M_PI/ROTOALPHA_flaresnum)));
    ppy = (DWORD)(ROTOALPHA_flaresy+(pt2*(float)cos(((float)i+t)*2*M_PI/ROTOALPHA_flaresnum)));
    PutPartRGB (GBL_Bits,(DWORD *)ROTOALPHA_flare->bits,ppx,ppy,(DWORD)(48+ROTOALPHA_flaresi));
  }
}

void ROTOALPHA_Layer1(float tick)
{
  tick = (tick*4)-2;
  if (tick<0) tick = 0;
  if (tick>1) tick = 1;
  ROTOALPHA_flaresi = fabs(80*sin(tick*M_PIx2));
}

void ROTOALPHA_Layer2(float tick)
{
  ROTOALPHA_flaresnum = 8+fabs(8*sin(tick*M_PIx2));
}

void ROTOALPHA_Flares1(float tick)
{  
  ROTOALPHA_flaresx = -150+(160+150)*tick;
  ROTOALPHA_flaresy = +150+(120-150)*tick;
  ROTOALPHA_Flares(tick);
}

void ROTOALPHA_Flares2(float tick)
{
  tick+=1;
  ROTOALPHA_flaresx = 160;
  ROTOALPHA_flaresy = 120;
  ROTOALPHA_Flares(tick);
}

void ROTOALPHA_Flares3(float tick)
{
  tick+=2;
  ROTOALPHA_flaresnum = 8;
  ROTOALPHA_flaresx = 160;
  ROTOALPHA_flaresy = 120;
  ROTOALPHA_Flares(tick);
}
  
void ROTOALPHA_Scene1(float tick)
{  
  float a=tick*360;
  float b=tick*400;
  float c=tick*520;

  effRotoZoom->DstBuffer = ROTOALPHA_v1->bits;
  effRotoZoom->SrcBuffer = (DWORD *)ROTOALPHA_img1->bits;
  effRotoZoom->makeEffect2(512*sin(a*M_ToRad),512*cos(c*M_ToRad),a,a);

  effRotoZoom->DstBuffer = GBL_Bits;
  effRotoZoom->SrcBuffer = (DWORD *)ROTOALPHA_img2->bits;
  effRotoZoom->makeEffect2(512*sin(b*M_ToRad),512*cos(a*M_ToRad),b,b);

  effJoinAlpha->AlphaBuffer = (BYTE *)ROTOALPHA_alpha1->bits;
  effJoinAlpha->DstBuffer = GBL_Bits;
  effJoinAlpha->Src1Buffer = ROTOALPHA_v1->bits;
  effJoinAlpha->Src2Buffer = GBL_Bits;
  effJoinAlpha->makeEffect();

  Margen320x240();
}

void ROTOALPHA_Close()
{
  LOG ("Closing ROTOALPHA");

  delete (effRotoZoom);
  delete (effJoinAlpha);
  FreeMem (ROTOALPHA_v1);
  FreeMem (ROTOALPHA_img1);
  FreeMem (ROTOALPHA_img2);
  FreeMem (ROTOALPHA_alpha1);
  FreeMem (ROTOALPHA_flare);
  if (ROTOALPHA_Sol1)
  {
    FreeMem (ROTOALPHA_Sol1);
    ROTOALPHA_Sol1 = NULL;
  }
  if (ROTOALPHA_Sol2)
  {
    FreeMem (ROTOALPHA_Sol2);
    ROTOALPHA_Sol2 = NULL;
  }

  LOG ("ROTOALPHA closed!");
}

void ROTOALPHA_Guio()
{
  GUIO_AddFN (ROTOALPHA_Scene1,"ROTOALPHA_Scene1");
  GUIO_AddFN (ROTOALPHA_Flares1,"ROTOALPHA_Flares1");    
  GUIO_AddFN (ROTOALPHA_Flares2,"ROTOALPHA_Flares2");    
  GUIO_AddFN (ROTOALPHA_Flares3,"ROTOALPHA_Flares3");    
  GUIO_AddFN (ROTOALPHA_Layer1,"ROTOALPHA_Layer1");      
  GUIO_AddFN (ROTOALPHA_Layer2,"ROTOALPHA_Layer2");      
  GUIO_AddFN (ROTOALPHA_Sols,"ROTOALPHA_Sols");
  GUIO_AddFN (ROTOALPHA_SolsGrow,"ROTOALPHA_SolsGrow");
}
