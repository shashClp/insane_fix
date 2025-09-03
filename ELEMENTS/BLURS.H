#ifndef INSANE_BLURS
#define INSANE_BLURS

#include <math.h>
#include <windows.h>
#include <view.h>
#include <mem.h>

bool BLURS_Init();
void BLURS_Close();

void BLURS_GenerarTunnel (float cenX,float cenY,float dist);
void BLURS_GenerarTunnelWave(float cenX,float cenY,float dist,float fase = 0.0);
void BLURS_GenerarTunnelRadialBlur (float cenX,float cenY,float dist=1.25);
void BLURS_GenerarTunnelRadialBlurInv (float cenX,float cenY,float dist=1.25);
void BLURS_RenderCentro(DWORD* imgIn,float x,float y,float factor);
void BLURS_Render(DWORD* imgIn);
void BLURS_RenderMMX(DWORD* imgIn);
DWORD BLURS_CalcOffs(DWORD* In,DWORD a_X,DWORD a_Y);
void BLURS_RenderRadialADD(DWORD* imgIn,long cenX,long cenY,long fact);
void BLURS_RenderRadialMIX(DWORD* imgIn,long cenX,long cenY,long fact);
void BLURS_RenderRadialInv(DWORD* imgIn,long cenX,long cenY,long fact);
void BLURS_RenderRadial2(DWORD* imgIn,long fact);

/////////////////////////////////////////////////////////////////

#endif;