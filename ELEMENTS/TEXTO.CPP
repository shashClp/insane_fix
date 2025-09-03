#include "texto.h"

//////////////////////////////////////////////////////////////////////////////
//
//  void GetBM8
//  Pilla un BitMap de un layer alpha (sin optimizadar)
void GetBM8(int x1,int y1,int x2,int y2,char* buffer,char* src, int src_width)
{
	long iH = x2-x1+1;
	long iV = y2-y1+1;

	*((long*)buffer) = iH;
    buffer += 4;
	*((long*)buffer) = iV;
    buffer += 4;

	for(int j=y1;j<y1+iV;j++)
	  for(int i=x1;i<x1+iH;i++)
		  *(buffer++) = *((char*)src + j*src_width + i);
}

//////////////////////////////////////////////////////////////////////////////
//
//  void PutBM8;
//  Pone un BitMap en un layer alpha (sin optimizadar)
void PutBM8(int x1,int y1,char* buffer, void* dst,bool alpha,long width)
{
	long iH = *((long*)buffer);	
    buffer += 4;
	long iV = *((long*)buffer);
    buffer += 4;

	if(alpha)
	for(int j=y1;j<y1+iV;j++)
	  for(int i=x1;i<x1+iH;i++)
      {
          if(*((unsigned char*)dst + j*width+i) < *((unsigned char* )buffer))
              *((char*)dst + j*width+i) = *(buffer);
          buffer++;
      }

}

//////////////////////////////////////////////////////////////////////////////
//
//  Rutinas de Fuentes
//  ------------------
//  SFuente* FONT_Load(char* sFileName);
//  void FONT_Close(SFuente* fnt);
//  void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,bool alpha=true);

SFuente* FONT_Load(char* sFileName)
{	
  SFuente* fnt = (SFuente*)GetMem(sizeof(SFuente));

  TImage imgTmp;
  IMAGE_LoadGIF(imgTmp,sFileName);
  char* pAct = (char*)imgTmp.bits; //Borra maya de delimitacion de caracteres (color 1)
  for(int ij=0;ij<<imgTmp.height;ij++) 
    for(int ii=0;ii<imgTmp.width;ii++)
        if(*pAct==1)
            *pAct = 0;

  fnt->iH = imgTmp.width / 26;
  fnt->iV = imgTmp.height / 4;

  LOG("Cargando fuente %s (%i,%i)",sFileName,fnt->iH,fnt->iV);
  
  for(int i=0;i<10;i++) //10 digitos
  {	    
		int iX=i*(fnt->iH);
		int iY=0;
        fnt->pBuf[i] = (char*)GetMem(8+fnt->iH*fnt->iV);
		GetBM8(iX,iY,iX+fnt->iH-1,iY+fnt->iV-1,fnt->pBuf[i],(char*)imgTmp.bits,imgTmp.width);
  }

  for(int j=0;j<3;j++) //3 linias de 26 caracteres
    for(int i=0;i<26;i++)
	{
	  int iX=i*(fnt->iH);
	  int iY=(1+j)*(fnt->iV);
	  fnt->pBuf[10+j*26+i] = (char*)GetMem(8+fnt->iH*fnt->iV);
	  GetBM8(iX,iY,iX+fnt->iH-1,iY+fnt->iV-1,fnt->pBuf[10+i+j*26],(char*)imgTmp.bits,imgTmp.width);
	}
  

  IMAGE_Free(imgTmp);

  return fnt;
}

void FONT_Close(SFuente* fnt)
{
  for(int i=0;i<26*3+10;i++)
	  FreeMem(fnt->pBuf[i]);
  FreeMem(fnt);
}

char FONT_Translate(char c)
{

  if(c>=47 && c<=56) //digitos
    return c-47;
  if(c>=65 && c<=90) //mayusculas
    return c-55+10;
  if(c>=97 && c<=122) //mayusculas
    return c-97+10+26;
  switch(c)
  {
        case '.':return 62;
        case ',':return 63;
        case '(':return 64;
        case ')':return 65;
        case '{':return 66;
        case '}':return 67;
        case '?':return 68;
        case ':':return 69;
        case '@':return 70;
        case '<':return 71;
        case '>':return 72;
        case '+':return 73;
        case '-':return 74;
        case '!':return 75;
        case '"':return 76;
        //case '''':return 77;
        default:return 78;
  };


}

void FONT_Print(long x,long y,char* pTxt,SFuente* fnt,void* dst,int espacio,bool alpha)
{
  if(espacio==0)
      espacio=fnt->iH;
  if(alpha)
    for(long i=0;pTxt[i]!=0;i++)
      PutBM8(x+i*espacio,y,(char*)fnt->pBuf[FONT_Translate(pTxt[i])],(char*)dst);
}