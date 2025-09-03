//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#include "..\RDEFINES\RDEFINES.H"
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

void GetTextureColor(ProceduralTexture *Texture,TVertex &CH,TReal Scale,TReal InvScale,RealColor &Color);

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
short   *hashTable;
TReal   RTable[267];

static  unsigned long next_rand = 1;

#define MINX (-10000)
#define MINY MINX
#define MINZ MINX
#define SCURVE(a) ((a)*(a)*(3.0-2.0*(a)))

#define RNDMASK       0x7FFF
#define RNDMULTIPLIER ((TReal)0.000030518509476)
#define FRAND() ((TReal)rand()*RNDMULTIPLIER)

#define FLOOR(x)  ((x) >= 0.0 ? floor(x) : (0.0 - floor(0.0 - (x)) - 1.0))
#define Hash3d(a,b,c)   hashTable[(int)(hashTable[(int)(hashTable[(int)((a) & 0xfffL)] ^ ((b) & 0xfffL))] ^ ((c) & 0xfffL))]
#define Hash2d(a,b)     hashTable[(int)(hashTable[(int)((a) & 0xfffL)] ^ ((b) & 0xfffL))]
#define Hash1d(a,b)     hashTable[(int)(a) ^ ((b) & 0xfffL)]
#define INCRSUM(m,s,x,y,z)     ((s)*(RTable[m]*0.5 + RTable[m+1]*(x) + RTable[m+2]*(y) + RTable[m+3]*(z)))
#define INCRSUMP(mp,s,x,y,z)   ((s)*((mp[0])*0.5 + (mp[1])*(x) + (mp[2])*(y) + (mp[3])*(z)))
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#define VScale(a, b, k) {a.x=b.x*k;a.y=b.y*k;a.z=b.z*k;}
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±  PSD-Random Texture Routines. Extracted and Based of POV-ray Source.
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

int Std_rand()
{
  next_rand = next_rand * 1812433253L + 12345L;
  return((int)(next_rand >> 16) & RNDMASK);
}

void Std_srand(unsigned long seed)
{
  next_rand =seed;
}

void CloseTextureTable()
{
#ifdef R_LOG_DEF
  R_LOG ("SystemR-DeAlloc Procedural textures Noise Tables");
#endif
  R_FreeMem (hashTable);

}
void InitTextureTable()
{
  int i, j, temp;
#ifdef R_LOG_DEF
  R_LOG ("SystemR-Alloc Procedural textures Noise Tables");
#endif

  Std_srand((unsigned long )0);

  if ((hashTable = (short int *)R_GetMem(4096*sizeof(short int))) == NULL ) perror ("Insuficient Memoria");

  for (i = 0; i < 4096; i++)
  {
    hashTable[i] = (short)i;
  }

  for (i = 4095; i >= 0; i--)  // Desorder hash table
  {
    j = Std_rand() % 4096;
    temp = hashTable[i];
    hashTable[i] = hashTable[j];
    hashTable[j] = (short)temp;
  }
  for (i = 0; i <267; i++) RTable[i] = 1.0-(FRAND()*2.0) ;
  
 // for (i = 0; i <267; i++) RTable[i] =  RTable[i]/50.0;
}

#ifdef ComplexNoise

TReal Noise(TVertex EPoint)
{
  TReal x, y, z;
  TReal *mp;
  long ix, iy, iz, jx, jy, jz;
  int ixiy_hash, ixjy_hash, jxiy_hash, jxjy_hash;

  TReal sx, sy, sz, tx, ty, tz;
  TReal sum;

  TReal x_ix, x_jx, y_iy, y_jy, z_iz, z_jz, txty, sxty, txsy, sxsy;

  x = EPoint.x-MINX;
  y = EPoint.y-MINY;
  z = EPoint.z-MINZ;

  /* if x = 6.7 then ix=6 and jx = 7  */

  ix = (long)x; iy = (long)y; iz = (long)z;
  jx = ix + 1; jy = iy + 1; jz = iz + 1;


  /* if x = 6.7 then sx=SCURBE(0.7)  */

  sx = SCURVE(x - ix); sy = SCURVE(y - iy); sz = SCURVE(z - iz);

  /* the complement values of sx,sy,sz */

  tx = 1.0 - sx; ty = 1.0 - sy; tz = 1.0 - sz;


  /* interpolate */

  x_ix = x - ix;
  x_jx = x - jx;
  y_iy = y - iy;
  y_jy = y - jy;
  z_iz = z - iz;
  z_jz = z - jz;

  txty = tx * ty;
  sxty = sx * ty;
  txsy = tx * sy;
  sxsy = sx * sy;

  ixiy_hash = Hash2d(ix, iy);
  jxiy_hash = Hash2d(jx, iy);
  ixjy_hash = Hash2d(ix, jy);
  jxjy_hash = Hash2d(jx, jy);

  mp = &RTable[(int) Hash1d(ixiy_hash, iz) & 0xFF];
  sum = INCRSUMP(mp, (txty*tz), x_ix, y_iy, z_iz);

  mp = &RTable[(int) Hash1d(jxiy_hash, iz) & 0xFF];
  sum += INCRSUMP(mp, (sxty*tz), x_jx, y_iy, z_iz);

  mp = &RTable[(int) Hash1d(ixjy_hash, iz) & 0xFF];
  sum += INCRSUMP(mp, (txsy*tz), x_ix, y_jy, z_iz);

  mp = &RTable[(int) Hash1d(jxjy_hash, iz) & 0xFF];
  sum += INCRSUMP(mp, (sxsy*tz), x_jx, y_jy, z_iz);

  mp = &RTable[(int) Hash1d(ixiy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (txty*sz), x_ix, y_iy, z_jz);

  mp = &RTable[(int) Hash1d(jxiy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (sxty*sz), x_jx, y_iy, z_jz);

  mp = &RTable[(int) Hash1d(ixjy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (txsy*sz), x_ix, y_jy, z_jz);

  mp = &RTable[(int) Hash1d(jxjy_hash, jz) & 0xFF];
  sum += INCRSUMP(mp, (sxsy*sz), x_jx, y_jy, z_jz);

  sum = sum + 0.5;                     /* range at this point -0.5 - 0.5... */

  if (sum < 0.0)
    sum = 0.0;
  if (sum > 1.0)
    sum = 1.0;

  return (sum);
}
#else

TReal Noise(TVertex EPoint)
{
  TReal x,y,z;
  x = EPoint.x*0.010;
  y = EPoint.y*0.010;
  z = EPoint.z*0.010;

  /* if x = 6.7 then ix=6 and jx = 7  */

  long ix = (long)x;
  long iy = (long)y;
  long iz = (long)z;

  TReal jx = x - ix;
  TReal jy = y - iy;
  TReal jz = z - iz;

  TReal sum;

  sum =   RTable[((int)(jx*256))& 0xFF];
  sum +=  RTable[((int)(jy*256))& 0xFF];
  sum +=  RTable[((int)(jz*256))& 0xFF];


  sum = sum + 0.5;                     /* range at this point -0.5 - 0.5... */

  if (sum < 0.0)
    sum = 0.0;
  if (sum > 1.0)
    sum = 1.0;
  
  return (sum);
}

#endif
TReal Turbulence(TVertex EPoint,DefTurb *Turb)
{
  int i;
  TReal Lambda, Omega, l, o, value = 0;
  TVertex temp;
  int Octaves=Turb->Octaves;

  value = Noise(EPoint);

  l = Lambda = Turb->Lambda;
  o = Omega  = Turb->Omega;

  for (i = 2; i <= Octaves; i++)
  {
    VScale(temp,EPoint,l);
    value += o * Noise(temp);
    if (i < Octaves)
    {
      l *= Lambda;
      o *= Omega;
    }
  }
  return (value);
}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
void GetTextInfo (ProceduralTexture *TXT0,TReal Scale,TReal InvScale,TVertex &CH,RealColor &Col0){
  ProceduralTexture *TXT = TXT0;
  RealColor Col;
  RealColor Col1;

  byte FIRT_LEVEL = 1;
  Col1.RGBA[0] = Col1.RGBA[1]= Col1.RGBA[2]= Col1.RGBA[3] = 0;

  while (TXT != NULL )
  {
    GetTextureColor(TXT,CH,Scale,InvScale,Col);
    if (FIRT_LEVEL) { Col.RGBA[3] = 1; FIRT_LEVEL = 0; }
    Col1.RGBA[0] = Col.RGBA[0]*Col.RGBA[3] + Col1.RGBA[0]*(1-Col.RGBA[3]);
    Col1.RGBA[1] = Col.RGBA[1]*Col.RGBA[3] + Col1.RGBA[1]*(1-Col.RGBA[3]);
    Col1.RGBA[2] = Col.RGBA[2]*Col.RGBA[3] + Col1.RGBA[2]*(1-Col.RGBA[3]);
    Col1.RGBA[3] = Col.RGBA[3]*Col.RGBA[3] + Col1.RGBA[3]*(1-Col.RGBA[3]);
    TXT = TXT->Next;
  }

  Col0.RGBA[0] = Col1.RGBA[0]*Col1.RGBA[3] + Col0.RGBA[0]*(1-Col1.RGBA[3]);
  Col0.RGBA[1] = Col1.RGBA[1]*Col1.RGBA[3] + Col0.RGBA[1]*(1-Col1.RGBA[3]);
  Col0.RGBA[2] = Col1.RGBA[2]*Col1.RGBA[3] + Col0.RGBA[2]*(1-Col1.RGBA[3]);

}

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
void GetTextureColor(ProceduralTexture *Texture,TVertex &CH,TReal Scale,TReal InvScale,RealColor &Color)
{
  int INC;

  TReal Value = 0;
  TReal Value1 = 0;
  TReal Value2 = 0;
  TReal Fact;
  TReal Dist;
  TReal IR,IG,IB,IA;

  TReal TScale = 1.0/Texture->TScale;
  TVertex Check = CH ;
  TVertex Check2;
  Check2.x = CH.x*TScale ;
  Check2.y = CH.y*TScale ;
  Check2.z = CH.z*TScale ;
  DefTurb *Turb;
  Turb = &Texture->Turbulence;
  TReal ValueOfTurb = 0;

  if ( Texture->Type == 0)
  {
// checker //
            if ( (Turb->Factor.x != 0) || (Turb->Factor.y != 0)|| (Turb->Factor.z != 0))
            ValueOfTurb = Turbulence(Check2,Turb);
            Check.x  = Check.x + ValueOfTurb*Turb->Factor.x - MINX;
            Check.y  = Check.y + ValueOfTurb*Turb->Factor.y - MINY;
            Check.z  = Check.z + ValueOfTurb*Turb->Factor.z - MINZ;
            Value = fmod (Check.x,Texture->TScale)  *TScale;
            Value1 = fmod (Check.z,Texture->TScale) *TScale;
            Value2 = fmod (Check.y,Texture->TScale) *TScale;
            if ( ( Value <  0.5 )  && ( Value1 <  0.5 ) && ( Value2 <  0.5 ) )    GetTextInfo (Texture->CNext1,Scale,InvScale,CH,Color); else
            if ( ( Value <  0.5 )  && ( Value1 <  0.5 ) && ( Value2 >=  0.5 ) )   GetTextInfo (Texture->CNext2,Scale,InvScale,CH,Color); else
            if ( ( Value <  0.5 )  && ( Value1 >=  0.5 ) && ( Value2 <  0.5 ) )   GetTextInfo (Texture->CNext2,Scale,InvScale,CH,Color); else
            if ( ( Value <  0.5 )  && ( Value1 >=  0.5 ) && ( Value2 >=  0.5 ) )  GetTextInfo (Texture->CNext1,Scale,InvScale,CH,Color); else
            if ( ( Value >=  0.5 )  && ( Value1 <  0.5 ) && ( Value2 <  0.5 ) )   GetTextInfo (Texture->CNext2,Scale,InvScale,CH,Color); else
            if ( ( Value >=  0.5 )  && ( Value1 <  0.5 ) && ( Value2 >=  0.5 ) )  GetTextInfo (Texture->CNext1,Scale,InvScale,CH,Color); else
            if ( ( Value >=  0.5 )  && ( Value1 >=  0.5 ) && ( Value2 <  0.5 ) )  GetTextInfo (Texture->CNext1,Scale,InvScale,CH,Color); else
            if ( ( Value >=  0.5 )  && ( Value1 >=  0.5 ) && ( Value2 >=  0.5 ) ) GetTextInfo (Texture->CNext2,Scale,InvScale,CH,Color);


  } else
  {
    if (  Texture->Type == 1  )
    {
// wood   //
            if ( (Turb->Factor.x != 0) || (Turb->Factor.y != 0)|| (Turb->Factor.z != 0))
            ValueOfTurb = Turbulence(Check2,Turb);
            Value1 = Check.x + ValueOfTurb*Turb->Factor.x;
            Value = Value1*Value1;
            Value1 = Check.y + ValueOfTurb*Turb->Factor.y;
            Value += Value1*Value1;
            Value1 = sqrt(Value);
            Value = fmod (Value1,Scale) * InvScale;
    }
    if (  Texture->Type == 2  )
    {
// marble //
            if ( (Turb->Factor.x != 0) || (Turb->Factor.y != 0)|| (Turb->Factor.z != 0))
            ValueOfTurb = Turbulence(Check2,Turb);
            Check.x  = Check.x + ValueOfTurb*Turb->Factor.x;
            Value1 = Check.x - MINX;
            Value = fmod (Value1,Scale) * InvScale;
    }
    if (Value <= 0) Value = 0;
    while (Value > 1) Value = Value - 1;
    INC = 0;
    while ( (Value > Texture->Dat[INC+1].S) && (INC< Texture->N_Steps)) INC ++;

    Fact = Value - Texture->Dat[INC].S;
    Dist = Fact/ ( Texture->Dat[INC+1].S - Texture->Dat[INC].S ) ;
    IR = (Texture->Dat[INC+1].RGBA[0] - Texture->Dat[INC].RGBA[0])*Dist;
    IG = (Texture->Dat[INC+1].RGBA[1] - Texture->Dat[INC].RGBA[1])*Dist;
    IB = (Texture->Dat[INC+1].RGBA[2] - Texture->Dat[INC].RGBA[2])*Dist;
    IA = (Texture->Dat[INC+1].RGBA[3] - Texture->Dat[INC].RGBA[3])*Dist;

    Color.RGBA[0] = Texture->Dat[INC].RGBA[0] +IR;
    Color.RGBA[1] = Texture->Dat[INC].RGBA[1] +IG;
    Color.RGBA[2] = Texture->Dat[INC].RGBA[2] +IB;
    Color.RGBA[3] = Texture->Dat[INC].RGBA[3] +IA;
  }
}
