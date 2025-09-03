//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±
//±±±   Physic Routines V1.0b   by Hlod-Wig of Anaconda
//±±±

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include <awl.h>
#include <math.h>
#include "Physic.h"

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

nodePhysic **PhysicStack;
long SizePhysicStack;

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
void  PInitialize (long Num)
{
  PhysicStack = (nodePhysic**) malloc (Num*sizeof (nodePhysic*));
  SizePhysicStack = Num-1;
}

long FGetIntersection (TVertex  &Dir,float &T0,float &T1,TVertex Normal,float TMP)
{
  float RES,T;

  RES = Normal.x*Dir.x +
        Normal.y*Dir.y +
        Normal.z*Dir.z;

  if (RES != 0)
  {
    T = TMP / RES;
      T0 = T;
      return 1;
  }
  return 0;
}



float PAngle ( TVertex &P1, TVertex &P2 )
{
  float PP= VTX_Length(P1)*VTX_Length(P2);
  if (PP==0) return 0.0;
  return (P1.x*P2.x + P1.y*P2.y + P1.z*P2.z) / PP;
}

long IsInTriangle (TVertex *TR,TVertex &CH)
{
  TVertex P1,P2,P3;
  P1 = TR[0];
  P2 = TR[1];
  P3 = TR[2];
  VTX_Sub (P1,CH);
  VTX_Sub (P2,CH);
  VTX_Sub (P3,CH);

  float SS = 0;

  SS = (float)acos(PAngle ( P1, P2 ));
  SS += (float)acos(PAngle ( P2, P3 ));
  SS += (float)acos(PAngle ( P3, P1 ));
  if (fabs(SS-6.283153f)<0.001f) return -1;
  return 1;

}

void  PCollision (nodePhysic*Point,Wall *Faces,long NumF,long B,float Factor)
{
   TVertex Direction,Check,Check2;

   for (long i=0; i< NumF; i++)
   {
    float D = Faces[i].Normal.x*Faces[i].Position.x
      +Faces[i].Normal.y*Faces[i].Position.y
      +Faces[i].Normal.z*Faces[i].Position.z;

    float TMP = D-(Faces[i].Normal.x*Point->OldPos.x
          +Faces[i].Normal.y*Point->OldPos.y
          +Faces[i].Normal.z*Point->OldPos.z);

    Direction = Point->Pos;

    VTX_Sub  (Direction,Point->OldPos);


    VTX_Normalize (Direction);
    float T0,T1,T2;

    T1 = PAngle (Direction,Faces[i].Normal);
    if ( T1<0.0 )
    {

      Check2.x = Faces[i].Position.x-Point->OldPos.x;
      Check2.y = Faces[i].Position.y-Point->OldPos.y;
      Check2.z = Faces[i].Position.z-Point->OldPos.z;

      T1 = PAngle (Check2,Faces[i].Normal);


      Check2.x = Faces[i].Position.x-Point->Pos.x;
      Check2.y = Faces[i].Position.y-Point->Pos.y;
      Check2.z = Faces[i].Position.z-Point->Pos.z;
      T2 = PAngle (Check2,Faces[i].Normal);

      float L1 ;
      FGetIntersection (Faces[i].Normal,L1,T1,Faces[i].Normal,TMP);
      if (  ( (T1>0.0) ||  (T2>0.0) ) &&  (fabs(L1) < Factor)  )
      {

        if (FGetIntersection (Direction,T0,T1,Faces[i].Normal,TMP))
        {

          Check.x = Point->OldPos.x+T0*Direction.x;
          Check.y = Point->OldPos.y+T0*Direction.y;
          Check.z = Point->OldPos.z+T0*Direction.z;

          if (IsInTriangle (Faces[i].Vertex,Check)<0)
          {
            T2 = PAngle (Point->Dir,Faces[i].Normal)*(-2.0f);
            if (T2>0.0)
            {
              Point->Dir.x += (Faces[i].Normal.x*T2 + Point->Dir.x);
              Point->Dir.y += (Faces[i].Normal.y*T2 + Point->Dir.y);
              Point->Dir.z += (Faces[i].Normal.z*T2 + Point->Dir.z);

              Point->Dir.x *= 0.5f;
              Point->Dir.y *= 0.5f;
              Point->Dir.z *= 0.5f;
            }

            T2 = PAngle (Point->Acc,Faces[i].Normal)*(-2.0f);
            if (T2>0.0)
            {
              Point->Acc.x += (Faces[i].Normal.x*T2 + Point->Acc.x);
              Point->Acc.y += (Faces[i].Normal.y*T2 + Point->Acc.y);
              Point->Acc.z += (Faces[i].Normal.z*T2 + Point->Acc.z);

              Point->Acc.x *= 0.25;
              Point->Acc.y *= 0.25;
              Point->Acc.z *= 0.25;
            }
/*
            T2 = PAngle (Point->Vel,Faces[i].Normal)*(-2.0);
            if (T2>0.0)
            {

              Point->Vel.x += (Faces[i].Normal.x*T2 + Point->Vel.x);
              Point->Vel.y += (Faces[i].Normal.y*T2 + Point->Vel.y);
              Point->Vel.z += (Faces[i].Normal.z*T2 + Point->Vel.z);

              Point->Vel.x *= 0.25;
              Point->Vel.y *= 0.25;
              Point->Vel.z *= 0.25;
            }
*/
            Point->Pos.x = Check.x;
            Point->Pos.y = Check.y;
            Point->Pos.z = Check.z;
          }
        }
      }
    }
  }
}

void PCalcDat (Wall *Faces,long NumF,int V)
{
  TVertex Tmp1,Tmp2;
  for (long i = 0; i < NumF ; i++)
  {

    Tmp1 = Faces[i].Vertex[1];
    Tmp2 = Faces[i].Vertex[2];
    VTX_Sub (Tmp1,Faces[i].Vertex[0]);
    VTX_Sub (Tmp2,Faces[i].Vertex[0]);

    VTX_CrossProduct (Faces[i].Normal,Tmp1,Tmp2);

    VTX_Normalize (Faces[i].Normal);

    Faces[i].Position.x=(Faces[i].Vertex[0].x
                        +Faces[i].Vertex[1].x
                        +Faces[i].Vertex[2].x)/(3.0f*V);
    Faces[i].Position.y=(Faces[i].Vertex[0].y
                        +Faces[i].Vertex[1].y
                        +Faces[i].Vertex[2].y)/(3.0f*V);
    Faces[i].Position.z=(Faces[i].Vertex[0].z
                        +Faces[i].Vertex[1].z
                        +Faces[i].Vertex[2].z)/(3.0f*V);
  }
}


void PCreatePhysicObject (nodePhysic *&Malla,long NumOfNodes)
{
  Malla = (nodePhysic*) malloc (NumOfNodes*sizeof(nodePhysic));

  for (long j=0;j<NumOfNodes;j++)
  {
    Malla[j].Dist   = (float *)     malloc (NumOfNodes*sizeof(float));
    Malla[j].LNK_B  = (nodePhysic**) malloc (NumOfNodes*sizeof(nodePhysic*));
    Malla[j].LNK_B2 = (nodePhysic**) malloc (NumOfNodes*sizeof(nodePhysic*));
    Malla[j].LNK_A  = (TVertex *)   malloc (NumOfNodes*sizeof(TVertex));
  }
}

void PCreatePhysicObject2 (nodePhysic *&Malla,long NumOfNodes,TVertex *VTX)
{
  Malla = (nodePhysic*) malloc (NumOfNodes*sizeof(nodePhysic));

  for (long j=0;j<NumOfNodes;j++)
  {
    Malla[j].Pos    = VTX[j];
    Malla[j].Dist   = (float *)     malloc (NumOfNodes*sizeof(float));
    Malla[j].LNK_B  = (nodePhysic**) malloc (NumOfNodes*sizeof(nodePhysic*));
    Malla[j].LNK_B2 = (nodePhysic**) malloc (NumOfNodes*sizeof(nodePhysic*));
    Malla[j].LNK_A  = (TVertex *)   malloc (NumOfNodes*sizeof(TVertex));
    VTX_Set (Malla[j].OldPos,0,0,0);
    VTX_Set (Malla[j].DPos,0,0,0);
    VTX_Set (Malla[j].DirA,0,0,0);
//    VTX_Set (Malla[j].Vel,0,0,0);
    VTX_Set (Malla[j].Acc,0,0,0);
  }
  
  long K; 
  for (long i = 0; i < NumOfNodes; i++)
  {
    Malla[i].Fixe = 0;
    K = 0;
    for (j = 0; j < NumOfNodes; j++)
    {
        if (j!=i) { Malla[i].LNK_B[K] = &Malla[j]; K++;}
    }
    Malla[i].N = K;
  }

}

void PCreateCube (nodePhysic *Malla,long NumPs,long VCub,long PosInY)
{
  long l,k,j,i;

  long PosX = -30+rand()%60;
  long PosY = -30+rand()%60;
  long PosZ = -30+rand()%60;

  long NN = 0;

  for (l = 0 ; l < NumPs; l++ )
  for (j = 0 ; j < NumPs; j++ )
    for (k = 0 ; k < NumPs; k++ )
    {
        VTX_Set (Malla[NN].Acc,0,0,0);
        VTX_Set (Malla[NN].Vel,0,0,0);
        VTX_Set (Malla[NN].Dir,0,0,0);
        VTX_Set (Malla[NN++].Pos,(float)(PosX+k*VCub) ,(float)(l*VCub+PosInY +PosY) ,(float)(j*VCub +PosZ));
    }

  long NumP = NumPs*NumPs*NumPs;

  for (i = 0; i < NumP; i++)
  {
    Malla[i].Fixe = 0;
    k = 0;
    for (j = 0; j < NumP; j++)
    {
        if (j!=i) { Malla[i].LNK_B[k] = &Malla[j]; k++;}
    }
    Malla[i].N = k;
  }


}

void PCalc2 (byte N,nodePhysic *Malla,long NumP)
{
  float DD;
  long i,j;
  if (N)
  {
    for ( i = 0; i < NumP ; i++)
    {
      for ( j=0; j < Malla[i].N; j++)
      {
        Malla[i].LNK_A[j] =  Malla[i].LNK_B[j]->Pos;
        VTX_Sub (Malla[i].LNK_A[j],Malla[i].Pos );
      }

      for ( j=0; j < Malla[i].N; j++)
      {
        Malla[i].Dist[j] = VTX_Length (Malla[i].LNK_A[j]);

      }
    }
  }

 for (i = 0; i < NumP ; i++)
  {

    Malla[i].ACT = 1;
    Malla[i].INV = 0;
    for (j=0; j < Malla[i].N; j++)
    {
      Malla[i].LNK_A[j] =  Malla[i].LNK_B[j]->Pos;

      VTX_Sub (Malla[i].LNK_A[j],Malla[i].Pos);

      DD = VTX_Length (Malla[i].LNK_A[j]) - Malla[i].Dist[j];

//      if (fabs(DD) <0.00001) DD = 0.0;

      VTX_Normalize (Malla[i].LNK_A[j]);

      Malla[i].LNK_A[j].x *= DD ;
      Malla[i].LNK_A[j].y *= DD ;
      Malla[i].LNK_A[j].z *= DD ;
/*
      if (fabs(Malla[i].LNK_A[j].x) < 0.00001) Malla[i].LNK_A[j].x = 0.0;
      if (fabs(Malla[i].LNK_A[j].y) < 0.00001) Malla[i].LNK_A[j].y = 0.0;
      if (fabs(Malla[i].LNK_A[j].z) < 0.00001) Malla[i].LNK_A[j].z = 0.0;
*/
    }
  }
}

void PDist (nodePhysic *Malla,long NumP)
{
  for (long i = 0; i < NumP ; i++)
  {
    for (long j=0; j < Malla[i].N; j++)
    {
      Malla[i].LNK_A[j] =  Malla[i].LNK_B[j]->Pos;
      VTX_Sub (Malla[i].LNK_A[j],Malla[i].Pos);
      Malla[i].Dist[j] = VTX_Length (Malla[i].LNK_A[j]);
    }
  }
}


void PCalc1 (nodePhysic *INV,float IntTime,float Tens)
{
   TVertex Suma;
   nodePhysic *Top;
   PhysicStack[0] = INV;

   float IntTime2 = IntTime*IntTime*0.5f;
   float FactorScale = 1-IntTime/100.0f;
   long  I = 0, F = 1;
   long i = 0,j = 0;

   INV->INV = 1;
   if (PhysicStack[0]->Fixe) return;

   VTX_Set (Suma,0,0,0);

   while ( I<F )
   {

     Top = PhysicStack[I];

     for (i=0; i < Top->N; i++)
     {
         Suma.x += Top->LNK_A[i].x;
         Suma.y += Top->LNK_A[i].y;
         Suma.z += Top->LNK_A[i].z;
     }

     Top->Acc.x = 0.5f*Top->Dir.x + Tens*(Suma.x) + Top->Acc.x*FactorScale;
     Top->Acc.y = 0.5f*Top->Dir.y + Tens*(Suma.y) + Top->Acc.y*FactorScale;
     Top->Acc.z = 0.5f*Top->Dir.z + Tens*(Suma.z) + Top->Acc.z*FactorScale;


     Top->DPos.x = Top->Acc.x*IntTime2;
     Top->DPos.y = Top->Acc.y*IntTime2;
     Top->DPos.z = Top->Acc.z*IntTime2;
     Top->Pos.x += Top->DPos.x;
     Top->Pos.y += Top->DPos.y;
     Top->Pos.z += Top->DPos.z;

/*
     Top->Vel.x += Top->Acc.x;
     Top->Vel.y += Top->Acc.y;
     Top->Vel.z += Top->Acc.z;
*/

     VTX_Set (Suma,0,0,0);

     Top->ACT = 0;

     for (i=0; i < Top->N; i++)
     {

       if ( (!Top->LNK_B[i]->Fixe) && (!Top->LNK_B[i]->INV) && (Top->LNK_B[i]->ACT) && (F<SizePhysicStack) )
       {
         PhysicStack[F] = Top->LNK_B[i];
         PhysicStack[F]->INV = 1;
         F++;
       }
     }

     I++;
   }
}

