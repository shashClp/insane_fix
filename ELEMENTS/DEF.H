#ifndef INSANE_DEF
#define INSANE_DEF

#include <math.h>
#include <stdlib.h>
#include <view.h>

extern long SENO[256];
extern long COSENO[256];

extern long CLIPTAB_X[1024];
extern long CLIPTAB_Y[1024];

void CalculaTablaSenos(float fAmplitud);
void InitClipTab(int lim_inf,int lim_sup);
void InitClipTabEspejo(int lim_inf,int lim_sup);
void DrawPic(unsigned long* dst,char* src,unsigned long* textura);
void DrawPicOffset(unsigned long* dst,char* src,unsigned long* textura);
void DrawWave(char *dst,int centro_x,int centro_y,long alpha);
void CreaCampanaPlasmaCirc();
void RenderPlasmaCircAlpha(char* dst);
void MuevePlasmaCirc();

#endif;