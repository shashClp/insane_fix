#ifndef __SHAPES_H__
#define __SHAPES_H__

#define NULL 0
#define dword unsigned long 
#define byte unsigned char

extern dword* GBL_Bits;

void Plot(int x1,int y1,dword color,dword* dest=GBL_Bits);
void AAPlot(float x1,float y1,dword color,dword* dest=GBL_Bits);
void AAPlot(float x1,float y1,byte colorR,byte colorG,byte colorB,dword* dest);
void AAPlot2(float x1,float y1,dword color,dword* dest);
void Line(int x1,int y1,int x2,int y2,dword color,dword* dest=GBL_Bits);
void AALine(float fx1,float fy1,float fx2,float fy2,dword color,dword* dest=GBL_Bits);
void AALine2(float fx1,float fy1,float fx2,float fy2,dword color,dword* dest=GBL_Bits);


#undef dword 
#undef byte
#endif //__SHAPES_H__