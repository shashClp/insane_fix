#include <defines.h>
#include <halo_light.h>
#include <math.h>
#include <rgb.h>

BYTE HALO_LIGHT_Alpha=0;
int ffa;
int ffb;
int ffc;
float rffa;
float rffb;
float rffc;

void HALO_LIGHT_Line320x240 (long x1,long y1,long x2,long y2,DWORD *dst,DWORD *src)
{
  long rx = x2-x1;
  long ry = y2-y1;
  long dx,dy,t,i;
  long r,g,b;
  long rf,gf,bf;
  long rr,gg,bb;    

  if (abs(rx)>abs(ry))
  {    
    t = rx;
  } else {    
    t = ry;
  }    
  if (t==0)
  {
    dy = 0;
    dx = 0;
  } else {
    t = abs(t);
    dy = (ry*65536)/t;
    dx = (rx*65536)/t;    
  }

  x1*=65536;
  y1*=65536;
    
  r = g = b = HALO_LIGHT_Alpha;

  int k=0;
  for (i=0; i<t; i++)
  {
    long pos = ((y1>>16)*320)+(x1>>16);
    long col = src[pos];

    rr = Get_R(col);
    gg = Get_G(col);
    bb = Get_B(col);

    rf=rr-i*2;
    if (rf>0) r += rf>>6;
    rr+=r;
    if (rr>255) rr=255;

    gf=gg-i*2;
    if (gf>0) g += gf>>6;
    gg+=g;
    if (gg>255) gg=255;

    bf=bb-i*2;
    if (bf>0) b += bf>>6;
    bb+=b;
    if (bb>255) bb=255;

    RGB_Set (dst[pos],rr,gg,bb);

    x1+=dx; 
    y1+=dy;
  }
}

void HALO_LIGHT_Render320x240 (DWORD x,DWORD y,DWORD *dst,DWORD *src)
{
DWORD i;

  for (i=0; i<GBL_Width; i++)
  {
    HALO_LIGHT_Line320x240 (x,y,i,0,dst,src);
    HALO_LIGHT_Line320x240 (x,y,i,GBL_Height-1,dst,src);
  }
  for (i=0; i<GBL_Height; i++)
  {
    HALO_LIGHT_Line320x240 (x,y,0,i,dst,src);
    HALO_LIGHT_Line320x240 (x,y,GBL_Width-1,i,dst,src);
  }
}

void HALO_LIGHT_Line (long x1,long y1,long x2,long y2,TView *dst,long a)
{
  long rx = x2-x1;
  long ry = y2-y1;
  long dx,dy,t,i,c1,pos;
  long r,g,b;
  long rf,gf,bf;
  long rr,gg,bb;    
  DWORD *d = dst->bits;

  if (abs(rx)>abs(ry))
  {    
    t = rx;
  } else {    
    t = ry;
  }    
  if (t==0)
  {
    dy = 0;
    dx = 0;
  } else {
    t = abs(t);
    dy = (ry*65536)/t;
    dx = (rx*65536)/t;    
  }

  x1*=65536;
  y1*=65536;
    
  pos = (GBL_Pitch*(y1/65536))+(x1/65536);
  c1 = GBL_Bits[pos];

  r = ((BYTE *)&c1)[2];
  g = ((BYTE *)&c1)[1];
  b = ((BYTE *)&c1)[0];

  r = HALO_LIGHT_Alpha;
  g = HALO_LIGHT_Alpha;
  b = HALO_LIGHT_Alpha;

  r = 255;
  g = 255;
  b = 255;

  int k=0;
  for (i=0; i<t; i++)
  {
    pos = (GBL_Pitch*(y1/65536))+(x1/65536);
    c1 = GBL_Bits[pos];

    rr = ((BYTE *)&c1)[2];
    gg = ((BYTE *)&c1)[1];
    bb = ((BYTE *)&c1)[0];

    r-=(bb-i-ffa);
    g-=(gg-k-ffb);
    b-=(rr-i-ffc);
    if (r<0) r = 0;
    if (g<0) g = 0;
    if (b<0) b = 0;    

    k+=2;
    rf = (rr+rr+rr+r)>>4;
    gf = (gg+gg+gg+g)>>4;
    bf = (bb+bb+bb+b)>>5;

    if (rf>255) rf = 255;
    if (gf>255) gf = 255;
    if (bf>255) bf = 255;

/*
    if (rr > 80) r = r+(rr>>1);
    if (gg > 80) g = g+(gg>>1);
    if (bb > 80) b = b+(bb>>1);

    rf = (r+rr+rr)>>2;
    gf = (g+gg+gg)>>2;
    bf = (b+bb+bb)>>2;

    if (rf>255) rf = 255;
    if (gf>255) gf = 255;
    if (bf>255) bf = 255;

    r-=20;
    g-=20;
    b-=20;

    if (r<0) r=0;
    if (g<0) g=0;
    if (b<0) b=0;
*/  
    
    /*
    r = (r*31+rr)/32;
    g = (g*31+gg)/32;
    b = (b*31+bb)/32;

    rf = (r+r+r+rr)>>2;
    gf = (g+g+g+gg)>>2;
    bf = (b+b+b+bb)>>2;
    */

/*
    r = (r*31+rr)/32;
    g = (g*15+gg)/16;
    b = (b*15+bb)/16;

    rf = (r+r+r+rr)>>2;
    gf = (g+g+g+gg)>>3;
    bf = (b+b+b+bb)>>3;
*/

/*
    r += rr/4;
    g += gg/4;
    b += bb/4;

    rf = (r+rr+rr+rr)>>2;
    if (rf>255) rf = 255;

    gf = (g+gg+gg+gg)>>2;
    if (gf>255) gf = 255;

    bf = (b+bb+bb+bb)>>2;
    if (bf>255) bf = 255;

    r -=20;
    g -=20;
    b -=20;
    if (r<0) r=0;
    if (g<0) g=0;
    if (b<0) b=0;
*/
    d[pos] = (((rf<<8)+gf)<<8)+bf;

    x1+=dx; y1+=dy;
  }
}

void HALO_LIGHT_Render (DWORD x,DWORD y,TView *dst)
{
DWORD i;
float b;
long a;

  ffa = (int)(30.0+(20.0*sin(rffa)));
  ffb = (int)(30.0+(20.0*cos(rffb)));
  ffc = (int)(30.0+(20.0*cos(rffc)));
  rffa+=0.01f;
  rffb+=0.03f;
  rffc+=0.05f;  
  b = 0; 
  HALO_LIGHT_Alpha++; 
  for (i=0; i<GBL_Width; i++)
  {
    a = (long) (GBL_Width*sin(b));
    HALO_LIGHT_Line (x,y,i,0,dst,a);
    HALO_LIGHT_Line (x,y,i,GBL_Height-1,dst,a);
    b+=0.01f;
  }
  for (i=0; i<GBL_Height; i++)
  {
    a = (long) (GBL_Height*cos(b));
    HALO_LIGHT_Line (x,y,0,i,dst,a);
    HALO_LIGHT_Line (x,y,GBL_Width-1,i,dst,a);
    b+=0.01f;
  }
}

void HALO_LIGHT_RenderReverse (DWORD x,DWORD y,TView *dst)
{
DWORD i;
float b;
long a;

  b = 0;
  HALO_LIGHT_Alpha+=5; 
  for (i=0; i<GBL_Width; i++)
  {
    a = (long) (GBL_Width*cos(b));
    HALO_LIGHT_Line (i,0,x,y,dst,a);
    HALO_LIGHT_Line (i,GBL_Height-1,x,y,dst,a);
    b+=0.01f;
  }
  for (i=0; i<GBL_Height; i++)
  {
    a = (long) (GBL_Height*sin(b));
    HALO_LIGHT_Line (0,i,x,y,dst,a);
    HALO_LIGHT_Line (GBL_Width-1,i,x,y,dst,a);
    b+=0.01f;
  }
}