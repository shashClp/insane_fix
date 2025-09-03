#include "cpart.h"

void CPartSys1::Init()
{
	for(int i=0;i<MAX_PART1;i++)
		m_vPart[i].set(0,0,0);

	m_fRadio = 50.0;
	m_fVelocidad = 500.0;
	m_iMaxPart = MAX_PART1;

	m_pbmAPart = NULL;
	m_pbmRGBPart = NULL;	  
}

void CPartSys1::SetMaxPart(long n) 
{
  m_iMaxPart = n;
}

bool CPartSys1::SetBitmapAlpha(char* f)
{
	m_pimgAPart = IMAGE_Load(f,8);  
  if (!m_pimgAPart) return false;
	m_pbmAPart = (char*)m_pimgAPart->bits;
  return true;
}

bool CPartSys1::SetBitmapRGB(char* f)
{	  
	m_pimgRGBPart = IMAGE_Load(f,32);
  if (!m_pimgRGBPart) return false;
  m_pbmRGBPart = (DWORD *)m_pimgRGBPart->bits;
  return true;
}

void CPartSys1::Close()
{
	if(m_pbmAPart)
	{
		FreeMem(m_pimgAPart);
		m_pbmAPart = NULL;
	}
	if(m_pbmRGBPart)
	{
		FreeMem (m_pimgRGBPart);
		m_pbmRGBPart = NULL;
	}
}

void CPartSys1::MueveCirc(float t)
{
	CMatrix3x3 mRot;
	mRot.SetRotacion((float)cos(t),t,(float)fmod(t,2*fPi));

	m_fRadio = (float)(100.0f + cos(t)*50.0f);
	t = (float)fmod(t,2*fPi);	  
	float fBasePos = (float)(t*2*fPi*m_fVelocidad);
	float fRadio = m_fRadio;
	for(int i=0;i<m_iMaxPart;i++)
	{
		float fPos = (float)(fBasePos + i*(2*fPi/MAX_PART1));
		float fR = m_fRadio;
		m_vPart[i] = mRot*CVector3((float)(fR*cos(fPos)),(float)(fR*sin(fPos)),10);
	}
}

void CPartSys1::MueveWeird1(float t)
//unas 300 part
{
	CMatrix3x3 mRot(true);
	mRot.SetRotacion(t,0,t);

	m_fRadio = (float) (130.0f + cos(t)*20.0f);
	t = (float)fmod(t,2*fPi);
	float fBasePos = (float)(t*2*fPi*m_fVelocidad);
	float fRadio = m_fRadio;
	for(int i=0;i<m_iMaxPart;i++)
	{
		float fPos = (float)(fBasePos + i*(2*fPi/MAX_PART1));
		float fR = m_fRadio;
		float fI = (float)(i*2*fPi/MAX_PART1);
		m_vPart[i] = mRot*CVector3((float)(fR*cos(2*fI)*cos(fPos + 3*fI)),
			                         (float)(fR*sin((4*fI) + fPos)*cos(fPos + 2*fI)),
			                         (float)(15+cos(fPos)*10));
	}
}

void CPartSys1::MueveADN(float t)
//unas 60 part
{
	CMatrix3x3 mRot(true);
	mRot.SetRotacion(0.0,(float)(-0.2*cos(t*200.0)),(float)(0.5+0.5*cos(t*200.0)));

	m_fRadio = (float)(13.0 + cos(t)*10.0);
	t = (float)fmod(t,2*fPi);
	float fBasePos = t*2*fPi*m_fVelocidad;
	float fRadio = m_fRadio;
	float fSigno = 1;
	for(int i=0;i<m_iMaxPart;i++)
	{
		fSigno = -fSigno;
		float fPos = fBasePos + i*(2*fPi/m_iMaxPart) + fSigno*fPi/2;
		float fR = m_fRadio;
		float fI = i*2*fPi/m_iMaxPart;
		m_vPart[i] = mRot*CVector3(
			(float)(fR*cos(fPos + 2*fI)*(1.1+cos(fBasePos*0.6 + (float)i/10.0f))),
			120-(4*i),
			(float)(50.0f+fR*sin(fPos + 2*fI))/5.0f);
	}
}


void CPartSys1::MueveRdm(float t)
//unas 300 part
{
	m_fRadio = 70.0;
	t = (float)fmod(t,2*fPi);
	float fBasePos = t*2*fPi*m_fVelocidad;
	float fRadio = m_fRadio;
	for(int i=0;i<m_iMaxPart;i++)
	{
		float fPos = fBasePos + i*(2*fPi/m_iMaxPart);
		float fR = m_fRadio;
		float fI = i*2*fPi/m_iMaxPart;
		m_vPart[i] = CVector3(
			fR*cos(fPos + 2*fI),
			0,
			(50+fR*sin(fPos + 2*fI))/5.0f);
	}
}

void CPartSys1::MueveBruja(float t)
//unas 60 part
{
	CMatrix3x3 mRot(true);
	
	m_fRadio = (float)(13.0 + cos(t)*10.0);
	t = (float)fmod(t,2*fPi);
	float fBasePos = t*2*fPi*m_fVelocidad;
	float fRadio = m_fRadio;
	float fSigno = 1;
	for(int i=0;i<m_iMaxPart;i++)
	{
		fSigno = -fSigno;
		float fPos = fBasePos + i*(2*fPi/m_iMaxPart) + fSigno*fPi/2;
		float fR = m_fRadio;
		float fI = i*2*fPi/m_iMaxPart;
		m_vPart[i] = mRot*CVector3(
			(float)(fR*cos(fPos + 2*fI)*(1.1+cos(fBasePos*0.6 + (float)i/10.0))),
			120-(4*i),
			(float)((50+fR*sin(fPos + 2*fI))/5.0));
	}
}

void CPartSys1::Posiciona(CVector3& centro,CMatrix3x3 base_norm,CVector3& desp)
{
	for(int i=0;i<m_iMaxPart;i++)
	{
		m_vPart[i] = (base_norm*(m_vPart[i]+desp-centro)) + centro;
	}
}

void CPartSys1::Desplaza(CVector3& desp)
{
	for(int i=0;i<m_iMaxPart;i++)
	{
		m_vPart[i] = m_vPart[i] + desp;
	}
}

void CPartSys1::RenderPartA(char* dest)
{	  
	float fZoomX = 100;
	float fZoomY = 100;
	
	for(int i=0;i<m_iMaxPart;i++) 
	{
		float fZ = 1/(m_vPart[i].m_z + 100);
		if(fZ > 0.0001 || fZ < -0.0001)
		PutPartA(dest,m_pbmAPart,(int)(160.0f+floor(fZoomX*m_vPart[i].m_x *fZ)),(int)(120.0f+floor(fZoomY*m_vPart[i].m_y *fZ)),(long)m_vPart[i].m_z);
	}
}

void CPartSys1::RenderPartRGB(DWORD* dest,long n)
{
	float fZoomX = 100;
	float fZoomY = 100;
	if(n == -1 )
		n = m_iMaxPart;

	/*	  for(int i=0;i<MAX_PART1;i++)
	{
		float fZ = 1/(m_vPart[i].m_z + 100);
		if(fZ > 0.0001 || fZ < -0.0001)
		AAPlot2(160.0+fZoomX*m_vPart[i].m_x *fZ, 120.0+fZoomY*m_vPart[i].m_y *fZ,0xefffef,dest);
	}*/

	/*
	for(int i=1;i<MAX_PART1;i++)
	{
		float fZ1 = 1/(m_vPart[i-1].m_z + 100);
		float fZ2 = 1/(m_vPart[i].m_z + 100);
		if((fZ1 > 0.0001 ) && (fZ2 > 0.0001 ))
		AALine(160.0+fZoomX*m_vPart[i-1].m_x *fZ1, 120.0+fZoomY*m_vPart[i-1].m_y *fZ1,
				 160.0+fZoomX*m_vPart[i].m_x *fZ2, 120.0+fZoomY*m_vPart[i].m_y *fZ2,
				 0xefffef,dest);			
	}
	*/

	for(int i=0;i<n;i++) 
	{
		float fZ = 1/(m_vPart[i].m_z + 100);
		if(fZ > 0.0001f || fZ < -0.0001f)
		PutPartRGB((DWORD*)dest,m_pbmRGBPart,160+floor(fZoomX*m_vPart[i].m_x *fZ), 120+floor(fZoomY*m_vPart[i].m_y *fZ),25-(long)m_vPart[i].m_z/2);
	}
}


void CPartSys1::RenderPartAdnRGB(DWORD* dest,long n)
{
	float fZoomX = 100;
	float fZoomY = 100;
	if (n == -1 )
		n = m_iMaxPart;

	for(int i=0;i<n;i++) 
	{
		float fZ = 1/(m_vPart[i].m_z + 100);
		if(fZ > 0.0001 || fZ < -0.0001)
		PutPartRGB((DWORD*)dest,m_pbmRGBPart,160+floor(fZoomX*m_vPart[i].m_x *fZ),120+floor(fZoomY*m_vPart[i].m_y*fZ),(long)m_vPart[i].m_z*0.8f);
	}
}
