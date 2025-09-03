//////////////////////////////////////////////////////////////////////////////
//
//	efx2d.h
//  ultima actualizacion: 25/9/99

#ifndef _EFX2D_H_
#define _EFX2D_H_

#include <view.h>
#include <image.h>

typedef struct{
  char* pBuf[10 + 26*3];
  int iH;
  int iV;
} SFuente;


void GetBM8(int x1,int y1,int x2,int y2,char* buffer,char* src, int src_width);
//  Pilla un BitMap de un layer alpha (sin optimizadar)

void PutBM8(int x1,int y1,char* buffer, void* dst = GBL_Bits,bool alpha = true,long width = GBL_Width);
//  Pone un BitMap en un layer alpha (sin optimizadar)

//  Rutinas de Fuentes
SFuente* FONT_Load(char* sFileName);
void FONT_Close(SFuente* fnt);
void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,int espacio = 0,bool alpha=true);

void RotoZoom(float fAlpha,float fZoom,float cpX,float cpY,bool clip,DWORD* dst,DWORD* src);
//Rotozoomer RGB con bilineal
void RotoZoom_Texel(float fAlpha,float fZoom,float cpX,float cpY,DWORD* dst,DWORD* src);
//Rotozoomer RGB 'texelado'
void RotoZoomA(float fAlpha,float fZoom,float cpX,float cpY,char* dst,char* src);
//Rotozoomer Alpha con bilineal
void RotoZoomA_Texel(float fAlpha,float fZoom,float cpX,float cpY,char* dst,char* src);
//Rotozoomer Alpha 'texelado'

void Morph8(char* dst,char* src1,char* src2,DWORD fact);
//   Transparencia al %indicado entre 2 layers de alpha
void MorphARGB(DWORD* dst,DWORD* src1,DWORD* src2,DWORD fact);
//   Transparencia al %indicado entre 2 layers ARGB

void MotionBlur32(DWORD* dst,DWORD* src);
//  Motion Blur al 50%

void FlatternAlpha(void* dst,void* src,void* alpha,int iSize = GBL_Size);
//  Superpone un layer alpha en un layer RGB (alpha -> blanco)
//dst y src en 32bits ARGB; alpha en 8 bits A

void FlatternAlphaBlue(void* dst,void* src,void* alpha);
//dst y src en 32bits ARGB; alpha en 8 bits A
//  Superpone un layer alpha en un layer RGB (alpha -> cyan)

void FlatternAlphaInv(void* dst,void* src,void* alpha,int iSize = GBL_Size);
void FlatternAlphaInvRed(void* dst,void* src,void* alpha,int iSize = GBL_Size);
void FlatternAlphaInvGreen(void* dst,void* src,void* alpha,int iSize = GBL_Size);
void FlatternAlphaInvBlue(void* dst,void* src,void* alpha,int iSize = GBL_Size);
//  Elimina de un layer RGB la componente alpha (alpha -> negro/rojo/verde/azul)
//dst y src en 32bits ARGB; alpha en 8 bits A

void Alpha2RGB(void* dst,void* src,int iSize = GBL_Size);
//  convierte un layer alpha en un layer RGB (alpha -> blanco)
//dst en 32bits ARGB; src en 8 bits A

void Alpha2RGBPal(void* dst,void* src,int iSize = GBL_Size);
bool LoadPal(char *filename); //pal de animator  256*3*8bits
//  convierte un layer alpha en un layer RGB (alpha -> indice en la paleta 256c)
//dst en 32bits ARGB; src en 8 bits A

void MergeAlphaADD(void* dst,void* src1,void* src2,int size);
//  Superpone un layer alpha en otro layer alpha (alpha -> blanco)

void MergeAlphaMIX(void* dst,void* src1,void* src2,int size);
//  Superpone un layer alpha en otro layer alpha (alpha -> blanco)

void MergeAlphaMAX(void* dst,void* src1,void* src2,int size);
//  Superpone un layer alpha en otro layer alpha (alpha -> blanco)

void HDistAlpha(void* dst,void* src,DWORD factor);
//  Distorsion Horizontal a lo 303 fast para layers alpha (resultado muy cutre!)
//dst y src en 8 bits A

void HDistAlpha1(void* dst,void* src,DWORD factor);
//  Distorsion Horizontal a lo 303 fast para layers alpha (resultado muy cutre!)
//dst y src en 8 bits A
//4 muestras por pixel
//src no puede pasar del indice 64 (=256/4) en cada pixel

void PutPart(char* dst,long px,long py,long t,char* PartBitMap);
//  Dibuja un particula alpha (TImage Bubble) de 64x64 de 8 bits en un layer alpha
//  (escalable y con clipping (muy guarro))
//  la comparacion entre layers puede ser MAX(x,y) o SUMA_ACOTADA(x,y) segun codigo

void ScrollToScreen8(char* dst,TImage* scroll,long xpos,long ypos);
//  Dibuja la parte visible de un layer alpha que puede ser mas grande que
//  que el layer alpha sobre el cual se renderiza (clipping 100%)

void ScrollToScreen32(DWORD* dst,TImage* scroll,long xpos,long ypos,long ystart);
//  Dibuja la parte visible de un layer RGB que puede ser mas grande que
//  que el layer aRGB sobre el cual se renderiza (clipping 100%)

void Recorta32(TImage* img_dst, long margen);
//	Anyade los margenes superior e inferior (simulando formato 16:9)

void HDist8(char* dst,int lin_ini,int lin_fin,char factor);
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina el blanco
// Factor<0 -> se difumina el negro

void HDist8Des(char* dst,int lin_ini,int lin_fin,char factor);
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina el blanco
// Factor<0 -> se difumina el negro
// Un factor positivo bajo (mas distorsion) hace que se desbanezca mas

void HDist32(DWORD* dst,int lin_ini,int lin_fin,char factor);
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina la maxima intensidad
// Factor<0 -> se difumina la minima intensidad

void Margen320x240(DWORD *dst = GBL_Bits);

typedef struct
{
	int x,y,u,v;
} VerTex;
void TexTri(DWORD* Dest,VerTex* vv1,VerTex* vv2,VerTex* vv3,DWORD* Bitmap);


void PutPartA(char* dest,char* PartBitMapA,long px,long py,long t);
void PutPartRGB(DWORD* dest,DWORD* PartBitMapRGB,long px,long py,long t);
void PutPartRGBNoClip(DWORD* dest,DWORD* PartBitMapRGB,long px,long py,long t);

#endif //_EFX2D_H_