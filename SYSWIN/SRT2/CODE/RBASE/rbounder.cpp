//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
//±±±±±±±
//±±±±±±± SystemR Version 3.0   By Hlod-Wig of Anaconda Software
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
#ifndef SystemR_V30_Bounder
#define SystemR_V30_Bounder

//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
byte *Bounder;
byte *BounderCheck;
long RCheck_Bounder_Range = 4;
//±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
void Bounder_Init(long X,long Y,long GX, long GY);
void Bounder_Close();
void Check_Bounder(long X,long Y);
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²         Bounder Functions
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Bounder_Init(long X,long Y,long GX, long GY)
{
    Bounder = (byte*) R_GetMem ((1+ X/GX)*(1 + Y/GY));
    BounderCheck = (byte*) R_GetMem ((1+ X/GX)*(1 + Y/GY));
    for (long i = 0; i < (1+ X/GX)*(1 + Y/GY); i++) BounderCheck[i] = 0;
    for (long i = 0; i < (1+ X/GX)*(1 + Y/GY); i++) Bounder[i] = i%3;

}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Bounder_Close()
{
    R_FreeMem (Bounder);
    R_FreeMem (BounderCheck);
}

//²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
//²²
//²²
void Check_Bounder(long X,long Y)
{
    #ifdef DEBUG
     cprintf ("Enter CheckBounder\n\r");
    #endif

    if (!(ActiveWorld->Flags & RW_PREDICTION_ANIM)) return ;
//    if (Bounder == NULL) exit(0);
    long Val;
    for (long i = X*RCheck_Bounder_Range ; i < (Y*X-X*RCheck_Bounder_Range );)
    {
      Val = Bounder[i];

      for (long j = -RCheck_Bounder_Range *X; j < X*RCheck_Bounder_Range ; j +=X)
      {
        for (long d = -RCheck_Bounder_Range ; d < RCheck_Bounder_Range ; d ++)

      #ifdef DEBUG
        cprintf ("Enter4[%d]\n\r",i+j+d);
      #endif

          if (Val != Bounder[i+j+d])
          {
            #ifdef DEBUG
              cprintf ("Enter-2[%d]\n\r",i);
            #endif

            BounderCheck[i] = 0;
            goto Surt1;
          }
      }
      #ifdef DEBUG
        cprintf ("Enter[%d]\n\r",i);
      #endif

      BounderCheck[i] = Val;


      Surt1:
      #ifdef DEBUG
        cprintf ("Enter5\n\r");
      #endif
      i++;
    }

}
#endif
