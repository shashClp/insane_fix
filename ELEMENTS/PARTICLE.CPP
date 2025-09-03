#include <guio.h>
#include <log.h>
#include <misc.h>
#include <view.h>
#include <mem.h>
#include <aw3d.h>
#include <efx2d.h>
#include <math.h>
#include "particle.h"

#define   R_PIx2   3.1415*2
#define ParticleColorR 0x5A
#define ParticleColorG 0x1A
#define ParticleColorB 0x2A

typedef struct{
   float X,Y,Z;
   float Gx,Gy,Gz;
}G_Structure;

typedef struct{
    float X,Y,Z;
    float VelX,VelY,VelZ;

}Particle_Structure;

WORD   Num_Objectes=1000;
float  Ordre_Caos=0.9;
byte   Num_Punts_Gravetat=3;
Particle_Structure      *SystemP;
G_Structure             *SystemG;


TImage *Partflare[4];
TImage *Sombras[4];


DWORD *Buffer00;
DWORD *Buffer01;

bool PActive = true;
float DeepFac = 1.0;

void GenParticlesWorld ()
{

  SystemP = (Particle_Structure *) GetMem (Num_Objectes*sizeof (Particle_Structure));
  if ( SystemP == NULL ) exit(0);
  SystemG = (G_Structure *) GetMem (Num_Punts_Gravetat*sizeof (G_Structure));
  if ( SystemG == NULL ) exit(0);

  for (long Cmt=0;Cmt<Num_Objectes;Cmt++){
      SystemP[Cmt].X=50-rand()%100;
      SystemP[Cmt].Y=50-rand()%100;
      SystemP[Cmt].VelX=0;
      SystemP[Cmt].VelY=0;
      SystemP[Cmt].VelZ=0;
      SystemP[Cmt].Z=SystemP[Cmt].X;
//      SystemP[Cmt].Z=50-rand()%100;
  }
}

void RunParticles ()
{

  float VectX,VectY,VectZ;
  float NormX,NormY,NormZ;
  float GDist;
  long  Cmt,Cmt1;

  for (Cmt=0;Cmt<Num_Objectes;Cmt++)
  {

    NormX = 0;NormY = 0;NormZ = 0;
    for (Cmt1 = 0; Cmt1 < Num_Punts_Gravetat; Cmt1 ++)
    {

        VectX=SystemG[Cmt1].X-SystemP[Cmt].X;
        VectY=SystemG[Cmt1].Y-SystemP[Cmt].Y;
        VectZ=SystemG[Cmt1].Z-SystemP[Cmt].Z;
        GDist=1.0 / (VectY*VectY+VectX*VectX+VectZ*VectZ);

        NormX+=VectX*(SystemG[Cmt1].Gx)*(GDist);
        NormY+=VectY*(SystemG[Cmt1].Gy)*(GDist);
        NormZ+=VectZ*(SystemG[Cmt1].Gz)*(GDist);

    }
        SystemP[Cmt].X +=SystemP[Cmt].VelX;
        SystemP[Cmt].Y +=SystemP[Cmt].VelY;
        SystemP[Cmt].Z +=SystemP[Cmt].VelZ;
        SystemP[Cmt].VelX=SystemP[Cmt].VelX*Ordre_Caos+NormX;
        SystemP[Cmt].VelY=SystemP[Cmt].VelY*Ordre_Caos+NormY;
        SystemP[Cmt].VelZ=SystemP[Cmt].VelZ*Ordre_Caos+NormZ;
  }
}


bool Particles_Init()
{
  GenParticlesWorld ();
  SystemG[0].Gx = 5;SystemG[0].Gy = 3;SystemG[0].Gy = 2 ;
  SystemG[1].Gx = 3;SystemG[1].Gy = 2;SystemG[1].Gz = 3;
  SystemG[2].Gx = 2;SystemG[2].Gy = -2;SystemG[2].Gz = 5;
	
	Buffer00 = (DWORD*) GetMem (GBL_Size4);
	Buffer01 = (DWORD*) GetMem (GBL_Size4);

	MISC_MemSet4 (Buffer01,0,GBL_Size);
  Partflare[0] = IMAGE_Load("DATA\\flare1.tga",32);
  Partflare[1] = IMAGE_Load("DATA\\flare2.tga",32);
  Partflare[2] = IMAGE_Load("DATA\\flare3.tga",32);
  Partflare[3] = IMAGE_Load("DATA\\flare4.tga",32);
	return true;
}

void Particles_Close() {

  FreeMem (SystemP);
  FreeMem (SystemG);
  FreeMem (Buffer00);
  FreeMem (Buffer01);
	FreeMem (Partflare[0]);
	FreeMem (Partflare[1]);
	FreeMem (Partflare[2]);
	FreeMem (Partflare[3]);

}

void Particles_Scene( float a)
{


  float C_b,S_b,S_a,S_c;
	DWORD Color;
  TMatrix Matriu;
  TVertex Pos1,Pos2;
  Ordre_Caos=0.98;

  C_b = cos(a);
  S_b = sin(a);
  S_a = sin(a);
  S_c = sin(a);

  SystemG[1].Gx = S_b+C_b;
  SystemG[2].Gx = S_a;
  SystemG[2].Gy = C_b;
  SystemG[0].Gz = S_a;

  SystemG[0].X = C_b*23;SystemG[0].Y = S_b*20;SystemG[0].Z = C_b*33;
  SystemG[1].X = S_c*28;SystemG[1].Y = C_b*23; SystemG[1].Z = S_b*23;
  SystemG[2].X = C_b*21;SystemG[2].Y = S_a*63; SystemG[2].Z = S_a*33;

  //-----------------------------------------------------------------------------------
  // shash: Framerate fix
  static DWORD lastTime = GetTickCount();
  DWORD currentTime = GetTickCount();
  if (currentTime-lastTime > 8)
  {
      if (PActive) RunParticles ();
      lastTime = currentTime;
  }
  //-----------------------------------------------------------------------------------
  
		
	MTX_Rotate (Matriu,sin(a*63.2)*360,sin(a*73.7)*320,cos(a*94.2)*360);

	MISC_MemSet4 (Buffer00,0,GBL_Size);
  for (register long Cmt=0;Cmt<Num_Objectes;Cmt++)
    {
      Pos1.x = SystemP[Cmt].X;
      Pos1.y = SystemP[Cmt].Y;
      Pos1.z = SystemP[Cmt].Z;
      MTX_Mul (Pos2,Pos1,Matriu);

      float TX = Pos2.x;
      float TY = Pos2.y;
      float TZ = 512.0/(Pos2.z+200*DeepFac);

      if (TZ>0)
      {
          WORD X  = (WORD)( TX*TZ + 160.5);
          WORD Y  = (WORD)( TY*TZ + 100.5) ;
					PutPartRGB(Buffer00,(unsigned long*)Partflare[Cmt%4]->bits,X,Y,12-Pos2.z/5);
      }

    }
		MotionBlur32(Buffer01,Buffer00);
		MergeAlphaADD(GBL_Bits,GBL_Bits,Buffer01,GBL_Size4);

		_asm{
			mov	edi,[GBL_Bits]
			add	edi,(320*21*4)-12
			xor	eax,eax
			//dec	eax //para debug
			mov	edx,200
			buc:
			 stosd
			 stosd
			 stosd
			 add	edi,(320-3)*4
			 dec	edx
			jnz	buc
		}
}

void Particles_OnOf (float Tick)
{

	PActive = !PActive;

}

void Particles_Zas (float Tick)
{

	DeepFac = cos(Tick*2*3.1415);

}

void Particles_Guio()
{

	GUIO_AddFN (Particles_Scene,"Particles_Scene");
	GUIO_AddFN (Particles_OnOf,"Particles_OnOf");
	GUIO_AddFN (Particles_Zas,"Particles_Zas");
}
