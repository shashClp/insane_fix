//////////////////////////////////////////////////////////////////////////////
//
//  Rutinas de texto
//  

#ifndef __TEXTO_H__
#define __TEXTO_H__

//  Pilla un BitMap de un layer alpha (sin optimizadar)
void GetBM8(int x1,int y1,int x2,int y2,char* buffer,char* src, int src_width);

//  Pone un BitMap en un layer alpha (sin optimizadar)
void PutBM8(int x1,int y1,char* buffer, void* dst = GBL_Bits,bool alpha = true,long width = GBL_ScreenWidth);

//////////////////////////////////////////////////////////////////////////////
//
//  Rutinas de Fuentes
//  ------------------
//  SFuente* FONT_Load(char* sFileName);
//  void FONT_Close(SFuente* fnt);
//  void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,bool alpha=true);

SFuente* FONT_Load(char* sFileName);
void FONT_Close(SFuente* fnt);
void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,int espacio = 0,bool alpha=true);

#endif //__TEXTO_H__