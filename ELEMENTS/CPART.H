#ifndef INSANE_CPART
#define INSANE_CPART

#define MAX_PART1 300
//#define MAX_PART1 60

#include <image.h>
#include <windows.h>
#include "3d.h"

class CPartSys1
{
public:
	CVector3 m_vPart[MAX_PART1];	
	float m_fRadio;
	float m_fVelocidad;
	float m_fDensidad;
	long m_iMaxPart;

	char* m_pbmAPart;
	DWORD* m_pbmRGBPart;
	TImage *m_pimgAPart;
	TImage *m_pimgRGBPart;

  void Init();
  void Close();
  void SetMaxPart(long n);
  bool SetBitmapAlpha(char* f);
  bool SetBitmapRGB(char* f);
  void MueveCirc(float t);
  void MueveWeird1(float t);
  void MueveADN(float t);
  void MueveRdm(float t);
  void MueveBruja(float t);
  void Posiciona(CVector3& centro,CMatrix3x3 base_norm,CVector3& desp);
  void Desplaza(CVector3& desp);
  void RenderPartA(char* dest);
  void RenderPartRGB(DWORD* dest,long n = -1);
  void RenderPartAdnRGB(DWORD* dest,long n = -1);
};

#endif