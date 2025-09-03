//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±
//±±±   Physic Routines V1.0b   by Hlod-Wig of Anaconda
//±±±

#ifndef SystemP_Defines
#define SystemP_Defines
#include <vertex.h>
#include <defines.h>

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#define  TReal float
#define  byte  unsigned char

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

typedef struct nodePhysic {

  TVertex Pos;
  TVertex OldPos;
  TVertex DPos;
  TVertex Dir;

  TVertex DirA;

  TVertex Vel;
  TVertex Acc;

  TReal *Dist;

  nodePhysic     **LNK_B;
  nodePhysic     **LNK_B2;
  TVertex *LNK_A;

  long INV;
  long N;

  long Fixe;
  long ACT;

} NDummy;


typedef struct Wall
{
  TVertex Vertex[3];
  TVertex Normal;
  TVertex Position;
} WDummy;

void  PInitialize (long Num);
long    PGetIntersection (TVertex  &Dir,TReal &T0,TReal &T1,TVertex Normal,TReal TMP);
TReal   PAngle ( TVertex &P1, TVertex &P2 );
long    IsInTriangle (TVertex *TR,TVertex &CH);
void    PCollision (nodePhysic*Point,Wall *Faces,long NumF,long B,float factor);
void    PCalcDat (Wall *Faces,long NumF,int V);
void    PCreatePhysicObject (nodePhysic *&Malla,long NumOfNodes);
void    PCreateCube (nodePhysic *Malla,long NumPs,long VCub);
void    PDist ();
void    PCalc1 (nodePhysic *IN,float IntTime,float Tens);
void    PCalc2 (byte N,nodePhysic *Malla,long NumP);
void    PCreatePhysicObject2 (nodePhysic *&Malla,long NumOfNodes,TVertex *VTX);

#endif
