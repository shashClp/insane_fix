#include <math.h>
#include <windows.h>
#include <shapes.h>
#include <view.h>

//extern long GBL_ScreenWidth;
//extern long GBL_ScreenHeight;
long GBL_ClipX1 = 20;
long GBL_ClipY1 = 20;
long GBL_ClipX2 = 300; 
long GBL_ClipY2 = 220;


DWORD GBL_Color;
DWORD GBL_ColorR;
DWORD GBL_ColorG;
DWORD GBL_ColorB;
DWORD* GBL_Dest;

//////////////////////////////////////////////////////////////////////
//
// void Plot(int x1,int y1,DWORD color);

void Plot(int x1,int y1,DWORD color,DWORD* dest)
{    
    if(x1>=GBL_ClipX1 && x1<=GBL_ClipX2 && y1>=GBL_ClipY1 && y1<=GBL_ClipY2)
        dest[(((GBL_Width*y1)+x1))] = color;
}

//////////////////////////////////////////////////////////////////////
//
// void Line(int x1,int y1,int x2,int y2,DWORD color);

void Line(int x1,int y1,int x2,int y2,DWORD color,DWORD* dest)
{
    int  d, ax, ay, sx, sy, dx, dy;

    dx = x2-x1;
    ax = abs(dx)<<1;
    if(dx<0)
        sx = -1;
    else 
        sx = 1;
    dy = y2-y1;
    ay = abs(dy)<<1;
    if(dy<0)
        sy = -1;
    else 
        sy = 1;
    Plot(x1, y1, color, dest);
    if(ax>ay)
    {
        d = ay-(ax>>1);
        while(x1!=x2)
        {
            if(d>=0)
            {
                y1 += sy;
                d -= ax;
            };
            x1 += sx;
            d += ay;
            Plot(x1, y1, color, dest);
        };
    }
    else
    {
        d = ax-(ay>>1);
        while(y1!=y2)
        {     
            if(d>=0)
            {
                x1 += sx;
                d -= ay;
            };
            y1 += sy;
            d += ax;
            Plot(x1, y1, color, dest);
        };
    };
}

//////////////////////////////////////////////////////////////////////
//
// void AAPlot(float x1,float y1,DWORD color);

void AAPlot(float x1,float y1,DWORD color,DWORD* dest)
{
  AAPlot(x1,y1,color & 0x0FF,(color>>8) & 0x0FF,(color>>16) & 0x0FF,dest);
}

//////////////////////////////////////////////////////////////////////
//
// void AAPlot(float x1,float y1,byte R,byte G,byte B,DWORD* dest);

void AAPlot(float x1,float y1,byte colorR,byte colorG,byte colorB,DWORD* dest)
{
   if(x1>=GBL_ClipX1 && x1<=GBL_ClipX2 && y1>=GBL_ClipY1 && y1<=GBL_ClipY2)
   {
       long x0 = floor(x1);
       long y0 = floor(y1);
       float alphaX = 1.0f - x1 + x0; 
       float alphaY = 1.0f - y1 + y0;
       float alpha;       
       byte* t_dest = (byte*)(dest+(((GBL_Width*y0)+x0)));

       alpha = 1 - alphaX*alphaY;
       *(t_dest++) = alpha * (*t_dest - colorR) + colorR;       
       *(t_dest++) = alpha * (*t_dest - colorG) + colorG;
       *(t_dest++) = alpha * (*t_dest - colorB) + colorB;
       t_dest++;

       alphaX=1.0f-alphaX;
       alpha = 1 - alphaX*alphaY;
       *(t_dest++) = alpha*(*t_dest - colorR) + colorR;       
       *(t_dest++) = alpha*(*t_dest - colorG) + colorG;
       *(t_dest++) = alpha*(*t_dest - colorB) + colorB;        
       (byte*)t_dest += ((GBL_Width-2)<<2)+1;

       alphaX=1.0f-alphaX;
       alphaY=1.0f-alphaY;
       alpha = 1 - alphaX*alphaY;
       *(t_dest++) = alpha*(*t_dest - colorR) + colorR;       
       *(t_dest++) = alpha*(*t_dest - colorG) + colorG;
       *(t_dest++) = alpha*(*t_dest - colorB) + colorB;
       t_dest++;

       alphaX=1.0f-alphaX;
       alpha = 1 - alphaX*alphaY;
       *(t_dest++) = alpha*(*t_dest - colorR) + colorR;
       *(t_dest++) = alpha*(*t_dest - colorG) + colorG;
       *(t_dest++) = alpha*(*t_dest - colorB) + colorB;
   }         
}

//////////////////////////////////////////////////////////////////////
//
// AAPlotH y AAPlotH (aux de AALine)

inline void AAPlotH(long x,float y1)
{
   if(x>=GBL_ClipX1 && x<=GBL_ClipX2 && y1>=GBL_ClipY1 && y1<=GBL_ClipY2)
   {        
       float y0 = floor(y1);
       float alphaY = y1 - y0;        
       float alpha;       
       byte* t_dest = (byte*)(GBL_Dest+(((GBL_Width*(long)y0)+x)));

       alpha = alphaY;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorR) + GBL_ColorR;       
       *(t_dest++) = alpha * (*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorB) + GBL_ColorB;
       t_dest += (GBL_Width<<2)-3;
      
       alpha = 1 - alphaY;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorR) + GBL_ColorR;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorB) + GBL_ColorB;
   }         
}


inline void AAPlotV(float x1,long y)
{
   if(x1>=GBL_ClipX1 && x1<=GBL_ClipX2 && y>=GBL_ClipY1 && y<=GBL_ClipY2)
   {
       float x0 = floor(x1);
       float alphaX = x1 - x0;        
       float alpha;       
       byte* t_dest = (byte*)(GBL_Dest+(((GBL_Width*y)+(long)x0)));

       alpha = alphaX;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorR) + GBL_ColorR;       
       *(t_dest++) = alpha * (*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorB) + GBL_ColorB;
       t_dest++;
      
       alpha = 1 - alphaX;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorR) + GBL_ColorR;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorB) + GBL_ColorB;
   }         
}

//////////////////////////////////////////////////////////////////////
//
// void AALine(float x1,float y1,float x2,float y2,DWORD GBL_Color);

void AALine(float fx1,float fy1,float fx2,float fy2,DWORD color,DWORD* dest)
// se guardan en fp:16.16 y float
{

    GBL_Color  = color;
	GBL_ColorR = (color>>16) & 0x0FF;        
    GBL_ColorG = (color>>8) & 0x0FF;
    GBL_ColorB = color & 0x0FF;

    GBL_Dest = dest;

	AAPlot(fx1, fy1, color, dest);
	//AAPlot(fx2, fy2, color, dest);

    float fd;
    float fax = fabs(fx2-fx1);
    float fay = fabs(fy2-fy1);

    if(fax>fay)
    {
        long x1 = (long)floor(fx1);
        long x2 = (long)ceil(fx2);
		
        if(x1!=x2)  
            fd = (fy2-fy1)/fax;
        else
            fd = 0.0;
        if(x1<x2)
		{
			fy1 -= (fx1 - (float)x1)*(fy2-fy1)/(x2-x1);
            while(x1<x2)
            {
                fy1 += fd;   
                x1++;
                AAPlotH(x1,fy1);
            }
		}
        else
		{
			fy2 -= (fx2 - (float)x2)*(fy1-fy2)/(x1-x2);
            while(x1>x2)
            {
                fy2 -= fd;
                x2++;
                AAPlotH(x2,fy2);
            }
		}
    }
    else
    {
        long y1 = (long)floor(fy1);
        long y2 = (long)ceil(fy2);

        if(y1!=y2)
            fd = (fx2-fx1)/fay;
        else
            fd = 0.0;
        if(y1<y2)
		{
			fx1 -= (fy1 - (float)y1)*(fx2-fx1)/(y2-y1);
            while(y1<y2)
            {
                fx1 += fd; 
                y1++;
                AAPlotV(fx1,y1);
            }
		}
        else        
		{
			fx2 -= (fy2 - (float)y2)*(fx1-fx2)/(y1-y2);
            while(y1>y2)
            {
                fx2 -= fd;            
                y2++;
                AAPlotV(fx2,y2);
            }
		}
    };

}

//////////////////////////////////////////////////////////////////////
//
// void AAPlot2(float x1,float y1,DWORD color,DWORD* dest)

void AAPlot2(float x1,float y1,DWORD color,DWORD* dest)
{    
    AAPlot(x1-0.5f,y1-0.5f,color,dest);
    AAPlot(x1+0.5f,y1-0.5f,color,dest);
    AAPlot(x1+0.5f,y1+0.5f,color,dest);
    AAPlot(x1-0.5f,y1+0.5f,color,dest);
}

//////////////////////////////////////////////////////////////////////
//
// AAPlotH2 y AAPlotH2 (aux de AALine)

inline void AAPlotH2(long x,float y1)
{
   if(x>=GBL_ClipX1 && x<=GBL_ClipX2 && y1>=GBL_ClipY1 && y1<=GBL_ClipY2)
   {
       y1--;

       float y0 = floor(y1);
       float alphaY = y1 - y0;        
       float alpha;       
       byte* t_dest = (byte*)(GBL_Dest+(((GBL_Width*(long)y0)+x)));

       alpha = alphaY;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorR) + GBL_ColorR;       
       *(t_dest++) = alpha * (*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorB) + GBL_ColorB;
       t_dest += (GBL_Width<<2)-3;

       *(DWORD*)(t_dest) = GBL_Color;              
       t_dest += (GBL_Width<<2);
      
       alpha = 1 - alphaY;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorR) + GBL_ColorR;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorB) + GBL_ColorB;
   }         
}


inline void AAPlotV2(float x1,long y)
{
   if(x1>=GBL_ClipX1 && x1<=GBL_ClipX2 && y>=GBL_ClipY1 && y<=GBL_ClipY2)
   {
       x1--;
       
       float x0 = floor(x1);
       float alphaX = x1 - x0;        
       float alpha;       
       byte* t_dest = (byte*)(GBL_Dest+(((GBL_Width*y)+(long)x0)));

       alpha = alphaX;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorR) + GBL_ColorR;       
       *(t_dest++) = alpha * (*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha * (*t_dest - GBL_ColorB) + GBL_ColorB;
       t_dest++;

       *(DWORD*)(t_dest) = GBL_Color;
       t_dest+=4;
      
       alpha = 1 - alphaX;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorR) + GBL_ColorR;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorG) + GBL_ColorG;
       *(t_dest++) = alpha*(*t_dest - GBL_ColorB) + GBL_ColorB;
   }         
}


//////////////////////////////////////////////////////////////////////
//
// void AALine2(float x1,float y1,float x2,float y2,DWORD GBL_Color);

void AALine2(float fx1,float fy1,float fx2,float fy2,DWORD color,DWORD* dest)
// se guardan en fp:16.16 y float
{

    GBL_Color  = color;
	GBL_ColorR = (color>>16) & 0x0FF;        
    GBL_ColorG = (color>>8) & 0x0FF;
    GBL_ColorB = color & 0x0FF;

    GBL_Dest = dest;

	AAPlot(fx1, fy1, color, dest);
	AAPlot(fx2, fy2, color, dest);

    float fd;
    float fax = fabs(fx2-fx1);
    float fay = fabs(fy2-fy1);

    if(fax>fay)
    {
        long x1 = (long)floor(fx1);
        long x2 = (long)ceil(fx2);
		
        if(x1!=x2)  
            fd = (fy2-fy1)/fax;
        else
            fd = 0.0;
        if(x1<x2)
		{
			fy1 -= (fx1 - (float)x1)*(fy2-fy1)/(x2-x1);
            while(x1<x2)
            {
                fy1 += fd;   
                x1++;
                AAPlotH2(x1,fy1);
            }
		}
        else
		{
			fy2 -= (fx2 - (float)x2)*(fy1-fy2)/(x1-x2);
            while(x1>x2)
            {
                fy2 -= fd;
                x2++;
                AAPlotH2(x2,fy2);
            }
		}
    }
    else
    {
        long y1 = (long)floor(fy1);
        long y2 = (long)ceil(fy2);

        if(y1!=y2)
            fd = (fx2-fx1)/fay;
        else
            fd = 0.0;
        if(y1<y2)
		{
			fx1 -= (fy1 - (float)y1)*(fx2-fx1)/(y2-y1);
            while(y1<y2)
            {
                fx1 += fd; 
                y1++;
                AAPlotV2(fx1,y1);
            }
		}
        else        
		{
			fx2 -= (fy2 - (float)y2)*(fx1-fx2)/(y1-y2);
            while(y1>y2)
            {
                fx2 -= fd;            
                y2++;
                AAPlotV2(fx2,y2);
            }
		}
    };

}

