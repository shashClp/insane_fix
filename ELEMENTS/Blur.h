#include <math.h>
#include <view.h>
#include <defines.h>

#define BLUR_MAX_ESTELS         50

typedef struct {
  float sw;
  float sh;
  float cw;
  float ch;
  float sx;
  float sy;
  float cx;
  float cy;
  float isx;
  float isy;
  float icx;
  float icy;
  int r;
  int g;
  int b;
  int ir;
  int ig;
  int ib;
} TBLUR_Estel;

bool BLUR_iB;
TView *BLUR_View;
DWORD *BLUR_Inc;
DWORD *BLUR_Mix[2];
float BLUR_Angle;
float BLUR_Angle2;
DWORD BLUR_Pitch;
TBLUR_Estel BLUR_Estels[BLUR_MAX_ESTELS];

void BLUR_GenerarTunnel ()
{
DWORD i,j;
DWORD *Inc;
float mod,x,y,a,b;

  Inc = BLUR_Inc;
  for (j=1; j<BLUR_View->height-1; j++)
  {
    for (i=1; i<BLUR_View->width-1; i++)
    {
      a = ((float)i)-BLUR_View->centerx;
      b = ((float)j)-BLUR_View->centery;
      a+=0.5;
      b+=0.5;
      
      mod = (float)sqrt((a*a)+(b*b));
      if (mod < 0.01) mod = (float)0.01;
      x = a/mod;
      y = b/mod;
      x -= (float)(a/15.0);
      y -= (float)(b/15.0);

      Inc[0] = (DWORD)(256.0*(x+i));
      Inc[1] = (DWORD)(256.0*(y+j));

      if (Inc[0] >= BLUR_View->width*256) Inc[0] = (BLUR_View->width-1)*256;
      if (Inc[1] >= BLUR_View->height*256) Inc[1] = (BLUR_View->height-1)*256;
      if (Inc[0] < 0) Inc[0] = 0;
      if (Inc[1] < 0) Inc[1] = 0;

      Inc+=2;
    }
  }
}

void BLUR_ClearGarbage()
{
  memset (BLUR_View->bits,0,BLUR_View->width*4);
  memset (BLUR_View->bits+(BLUR_View->height-1)*BLUR_View->pitch,0,BLUR_View->width*4);
  DWORD *dst = BLUR_View->bits;
  for (DWORD i=0; i<BLUR_View->height; i++)
  {
    *dst = 0;
    *(dst+BLUR_View->width-1) = 0;
    dst+=BLUR_View->pitch;
  }
}

void BLUR_InitEstels ()
{
  for (DWORD i=0; i<BLUR_MAX_ESTELS; i++)
  {
    BLUR_Estels[i].sw = (float)((rand()%80)+8);
    BLUR_Estels[i].sh = (float)((rand()%60)+8);
    BLUR_Estels[i].cw = (float)((rand()%80)+8);
    BLUR_Estels[i].ch = (float)((rand()%60)+8);
    BLUR_Estels[i].sx = (float)(rand()%360);
    BLUR_Estels[i].isx = (float)((((float)(rand()%1000))/500)+1.0);
    BLUR_Estels[i].sy = (float)(rand()%360);
    BLUR_Estels[i].isy = (float)((((float)(rand()%1000))/500)+1.0);
    BLUR_Estels[i].cx = (float)(rand()%360);
    BLUR_Estels[i].icx = (float)((((float)(rand()%1000))/500)+1.0);
    BLUR_Estels[i].cy = (float)(rand()%360);    
    BLUR_Estels[i].icy = (float)((((float)(rand()%1000))/500)+1.0);
    BLUR_Estels[i].r = rand()%256;
    BLUR_Estels[i].ir = (rand()%5)+1;
    BLUR_Estels[i].g = rand()%256;
    BLUR_Estels[i].ig = (rand()%5)+1;
    BLUR_Estels[i].b = rand()%256;
    BLUR_Estels[i].ib = (rand()%5)+1;
  }
}

void BLUR_PutEstel (DWORD x,DWORD y,DWORD col)
{
  DWORD pos = (BLUR_View->pitch*y)+x;
  DWORD *dst = BLUR_View->bits+pos;
  *dst = 0xdfdfdf;
  *(dst+1) = col;
  *(dst+BLUR_View->pitch) = col;  
  *(dst-1) = col;
  *(dst-BLUR_View->pitch) = col;  
}

void BLUR_PutEstels ()
{
  for (DWORD i=0; i<BLUR_MAX_ESTELS; i++)
  {
    int x = (int)(BLUR_Estels[i].sw*sin(BLUR_Estels[i].sx*M_ToRad)+
                  BLUR_Estels[i].cw*cos(BLUR_Estels[i].cx*M_ToRad)+BLUR_View->centerx+0.5);
    int y = (int)(BLUR_Estels[i].sh*sin(BLUR_Estels[i].sy*M_ToRad)+
                  BLUR_Estels[i].ch*cos(BLUR_Estels[i].cy*M_ToRad)+BLUR_View->centery+0.5);

    BLUR_Estels[i].sx+=BLUR_Estels[i].isx;
    BLUR_Estels[i].sy+=BLUR_Estels[i].isy;
    BLUR_Estels[i].cx+=BLUR_Estels[i].icx;
    BLUR_Estels[i].cy+=BLUR_Estels[i].icy;
    
    if ((x<1) || (y<1) || (x>(int)BLUR_View->width-2) || (y>(int)BLUR_View->height-2)) continue;

    int r =(int)(192.0+(63.0*sin(BLUR_Estels[i].r*M_ToRad)));
    int g =(int)(192.0+(63.0*cos(BLUR_Estels[i].g*M_ToRad)));
    int b =(int)(192.0+(63.0*sin(BLUR_Estels[i].b*M_ToRad)));
    BLUR_Estels[i].r+=BLUR_Estels[i].ir;
    BLUR_Estels[i].b+=BLUR_Estels[i].ig;
    BLUR_Estels[i].g+=BLUR_Estels[i].ib;
    DWORD col = (r<<16)+(g<<8)+b;

    BLUR_PutEstel (x,y,col);
  }

  for (DWORD i=0; i<50; i++)
  {
    float BLUR_Angle3=(float)i;
    int r =(int)(128.0+i+(63.0*sin(BLUR_Estels[0].r*M_ToRad)));
    int g =(int)(128.0+i+(63.0*cos(BLUR_Estels[0].g*M_ToRad)));
    int b =(int)(128.0+i+(63.0*sin(BLUR_Estels[0].b*M_ToRad)));
    DWORD col = (r<<16)+(g<<8)+b;

    int x = (int)((60+20*cos((BLUR_Angle2+i*5)*M_ToRad))*sin ((BLUR_Angle+i)*M_ToRad));
    int y = (int)((50+20*cos((BLUR_Angle3+i*5)*M_ToRad))*cos ((BLUR_Angle+i)*M_ToRad));
    BLUR_PutEstel (BLUR_View->centerx+x,BLUR_View->centery+y,col);    

    r =(int)(128.0+i+(63.0*sin(BLUR_Estels[1].r*M_ToRad)));
    g =(int)(128.0+i+(63.0*cos(BLUR_Estels[1].g*M_ToRad)));
    b =(int)(128.0+i+(63.0*sin(BLUR_Estels[1].b*M_ToRad)));
    col = (r<<16)+(g<<8)+b;

    x = (int)((-60+20*cos((BLUR_Angle2+i*5)*M_ToRad))*sin ((BLUR_Angle+i)*M_ToRad));
    y = (int)((-50+20*cos((BLUR_Angle3+i*5)*M_ToRad))*cos ((BLUR_Angle+i)*M_ToRad));
    BLUR_PutEstel (BLUR_View->centerx+x,BLUR_View->centery+y,col);    
  }
  BLUR_Angle+=1;
  BLUR_Angle2+=5;  
}

void BLUR_Render()
{
DWORD i,j,x,y;
DWORD *In,*Inxy,*pOut;
DWORD *Inc;

  In = BLUR_Mix[BLUR_iB];
  BLUR_iB = !BLUR_iB;
  pOut = BLUR_Mix[BLUR_iB]+BLUR_View->pitch+1;
  Inc = BLUR_Inc;  

  _asm pxor mm5,mm5;

  for (j=0; j<BLUR_View->height-2; j++)
  {
    for (i=0; i<BLUR_View->width-2; i++)
    {
      x = *Inc++;
      y = *Inc++;
      Inxy = In+(BLUR_View->pitch*(y >> 8))+(x >> 8);
     
      _asm {        
        mov esi,Inxy
        mov edi,BLUR_Pitch
        mov eax,x
        mov ecx,y
        and eax,0xff
        and ecx,0xff
        mov ah,al
        mov ch,cl
        shl eax,8
        shl ecx,8
        mov al,ah
        mov cl,ch        

        mov ebx,eax
        mov edx,ecx
        xor eax,-1
        xor ecx,-1                
        
        movd       mm1,[esi-4]
        punpcklbw  mm1,mm5
        movd       mm6,eax
        punpcklbw  mm6,mm5
        pmullw     mm1,mm6

        movd       mm2,[esi+4]
        punpcklbw  mm2,mm5
        movd       mm6,ebx
        punpcklbw  mm6,mm5
        pmullw     mm2,mm6

        movd       mm3,[esi-320*4]
        punpcklbw  mm3,mm5
        movd       mm6,ecx
        punpcklbw  mm6,mm5
        pmullw     mm3,mm6

        movd       mm4,[esi+320*4]
        punpcklbw  mm4,mm5
        movd       mm6,edx
        punpcklbw  mm6,mm5
        pmullw     mm4,mm6

        movd       mm0,[esi]
        punpcklbw  mm0,mm5
        psrlw      mm1,8
        psrlw      mm2,8
        psrlw      mm3,8
        psrlw      mm4,8        
        paddw      mm1,mm2
        paddw      mm3,mm4
        paddw      mm0,mm0
        paddw      mm1,mm3

        paddw      mm0,mm1
        psrlw      mm0,2
        mov        edi,pOut
        packuswb   mm0,mm5        
        add        pOut,4
        movd       [edi],mm0                        
      }      
    }
    pOut+=2;
  }

  _asm emms;
}

void BLUR_PreCalc (DWORD w,DWORD h)
{
  BLUR_View = VIEW_Init (w,h,w/2,h/2,w,(DWORD *)0xffffff);
  BLUR_Inc = (DWORD *) GetMem ((w-2)*(h-2)*2*4);
  BLUR_Pitch = w*4;
  BLUR_iB = 0;
  BLUR_GenerarTunnel ();
  BLUR_InitEstels ();
}

void BLUR_Close ()
{  
  FreeMem (BLUR_View);
  FreeMem (BLUR_Inc);
  BLUR_Inc = NULL;
}

void BLUR_Flipx2(TView *view)
{   
  DWORD *punter32 = view->bits;
  DWORD *src = BLUR_View->bits;
  for (DWORD i=0; i<BLUR_View->height; i++)
  {
    DWORD *d = punter32;
    DWORD *ds = punter32+view->pitch;
    DWORD *s = src;
    for (DWORD j=0; j<BLUR_View->width; j++)
    {
      *d++=*s;
      *ds++=*s;
      *d++=*s;
      *ds++=*s++;
    }
    punter32+=2*view->pitch;    
    src+=BLUR_View->pitch;
  }
}