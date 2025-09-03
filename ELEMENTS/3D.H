#ifndef INSANE_3D
#define INSANE_3D

#include <math.h>
#include <image.h>
#include <defines.h>
#include <efx2d.h>
#include <shapes.h>
#include <mem.h>

#define fPi M_PI

//////////////////////////////////////////////////////////////////////////////
//	
//	class CVector3

class CVector3
{
  public:

	//atributos

	float m_x;
	float m_y;
	float m_z;

	//constructores

	CVector3() {;}

	CVector3(float x,float y,float z) 
	{ 
		m_x = x;
		m_y = y;
		m_z = z;
	}

	CVector3(float a[3]) 
	{ 
		m_x = a[0];
		m_y = a[1];
		m_z = a[2];
	}

	//metodos

	void set(float x,float y,float z) 
	{ 
		m_x = x;
		m_y = y;
		m_z = z;
	}

	float norma()
	{
		return (float)sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
	}

	void normaliza()
	{
		float n = norma();
		m_x /= n;
		m_y /= n;
		m_z /= n;
	}	

	//suma: vector x vector -> vector
	CVector3 operator+(CVector3& v)
	{
		return CVector3(v.m_x+m_x, v.m_y+m_y, v.m_z+m_z);
	}

	//resta: vector x vector -> vector
	CVector3 operator-(CVector3& v)
	{
		return CVector3(m_x-v.m_x, m_y-v.m_y, m_z-v.m_z);
	}

	//producto: vector x float -> vector
	CVector3 operator*(float f)
	{
		return CVector3(f*m_x, f*m_y, f*m_z);
	}

	//???
	float operator*(CVector3& v)
	{
		return v.m_x*m_x + v.m_y*m_y + v.m_z*m_z;
	}

	//producto ectorial: vector x vector -> vector
	CVector3 operator^(CVector3& v)
	{
		return CVector3(m_y*v.m_z - m_z*v.m_y, m_z*v.m_x - m_x*v.m_z, m_x*v.m_y - m_y*v.m_x);
	}

	//afinidades

	void escala(float x,float y,float z)
	{		
		m_x *= x;
		m_y *= y;
		m_z *= z;
	}
	void escala(CVector3& v)
	{		
		m_x *= v.m_x;
		m_y *= v.m_y;
		m_z *= v.m_z;
	}

	void translada(float x,float y,float z)
	{		
		m_x += x;
		m_y += y;
		m_z += z;
	}
	void translada(CVector3& v)
	{		
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
	}



};

//////////////////////////////////////////////////////////////////////////////
//	
//	class CMatrix3x3

class CMatrix3x3
{
  public:
	
	//atributos

	float m_x1,m_y1,m_z1;
	float m_x2,m_y2,m_z2;
	float m_x3,m_y3,m_z3;

	//constructores

	//constructor: matriz
	CMatrix3x3() {;}

	//constructor: vector x vector x vector -> matriz
	CMatrix3x3(CVector3& v1,CVector3& v2,CVector3& v3) 
	{
		m_x1 = v1.m_x; m_y1 = v1.m_y; m_z1 = v1.m_z;
		m_x2 = v2.m_x; m_y2 = v2.m_y; m_z2 = v2.m_z;
		m_x3 = v3.m_x; m_y3 = v3.m_y; m_z3 = v3.m_z;
	}

	//constructor: matriz_identidad
	CMatrix3x3(bool id)
	{
		m_x1 = 1; m_y1 = 0; m_z1 = 0;
		m_x2 = 0; m_y2 = 1; m_z2 = 0;
		m_x3 = 0; m_y3 = 0; m_z3 = 1;
	}

	//suma: matriz x matriz -> matriz
	CMatrix3x3 operator+(CMatrix3x3& m)
	{
		CMatrix3x3 r;
		r.m_x1 = m_x1 + m.m_x1; r.m_y1 = m_y1 + m.m_y1; r.m_z1 = m_z1 + m.m_z1;
		r.m_x2 = m_x2 + m.m_x2; r.m_y2 = m_y2 + m.m_y2; r.m_z2 = m_z2 + m.m_z2;
		r.m_x3 = m_x3 + m.m_x3; r.m_y3 = m_y3 + m.m_y3; r.m_z3 = m_z3 + m.m_z3;

		return r;
	}

	//producto: matriz x vector -> vector
	CVector3 operator*(CVector3& v)
	{
		float x = m_x1*v.m_x + m_x2*v.m_y + m_x3*v.m_z;
		float y = m_y1*v.m_x + m_y2*v.m_y + m_y3*v.m_z;
		float z = m_z1*v.m_x + m_z2*v.m_y + m_z3*v.m_z;

		return CVector3(x,y,z);
	}

	//producto: matriz x matriz -> matriz
	CMatrix3x3 operator*(CMatrix3x3& m)
	{
		CMatrix3x3 r;

		r.m_x1 = m_x1*m.m_x1 + m_x2*m.m_y1 + m_x3*m.m_z1;
		r.m_y1 = m_y1*m.m_x1 + m_y2*m.m_y1 + m_y3*m.m_z1;
		r.m_z1 = m_z1*m.m_x1 + m_z2*m.m_y1 + m_z3*m.m_z1;

		r.m_x2 = m_x1*m.m_x2 + m_x2*m.m_y2 + m_x3*m.m_z2;
		r.m_y2 = m_y1*m.m_x2 + m_y2*m.m_y2 + m_y3*m.m_z2;
		r.m_z2 = m_z1*m.m_x2 + m_z2*m.m_y2 + m_z3*m.m_z2;

		r.m_x3 = m_x1*m.m_x3 + m_x2*m.m_y3 + m_x3*m.m_z3;
		r.m_y3 = m_y1*m.m_x3 + m_y2*m.m_y3 + m_y3*m.m_z3;
		r.m_z3 = m_z1*m.m_x3 + m_z2*m.m_y3 + m_z3*m.m_z3;

		return r;
	}

	//convierte la matriz en una matriz para la afinidad de rotacion en los 3 ejes
	void SetRotacion(float x,float y,float z)
	{
		float sinx = (float)sin(x);
		float siny = (float)sin(y);
		float sinz = (float)sin(z);
		float cosx = (float)cos(x);
		float cosy = (float)cos(y);
		float cosz = (float)cos(z);

		m_x1 = cosz * cosy;
		m_y1 =-sinz * cosx + cosz * siny * sinx;
		m_z1 =-sinz *-sinx + cosz * siny * cosx;
		m_x2 = sinz * cosy;
		m_y2 = cosz * cosx + sinz * siny * sinx;
		m_z2 = cosz *-sinx + sinz * siny * cosx;
		m_x3 =-siny;
		m_y3 = cosy * sinx;
		m_z3 = cosy * cosx;
	}

};

#endif //_3D_H_