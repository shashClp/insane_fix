// AWL - Anaconda Windows Library

#include <image.h>
#include <mem.h>
#include <file.h>
#include <log.h>

TImage *TGA_Load (char *FileName)
{ 
FILE *in;
TImage *img;
char DummyA[12];
int DummyC;
int DummyC2;    

  if (!FileName)
  {
    ERR ("Opening a TGA with NULL filename");
    return false;
  }

  SEC_START ("Loading TGA image %s",FileName);
  
  if (!FILE_Open (&in,FileName,"rb")) 
  {
    ERR ("Can't load %s",FileName);
    return NULL;
  }

  if (!FILE_Read (DummyA,12,1,in))
  {
    ERR ("Getting header info in TGA load of file %s",FileName);
    FILE_Close(in); 
    return false;
  }

  DummyC2 = FILE_Getc(in);
  DummyC = FILE_Getc(in);
  DWORD Width = (DummyC<<8) + DummyC2;
  
  DummyC2 = FILE_Getc(in);
  DummyC = FILE_Getc(in);
  DWORD Height = (DummyC<<8) + DummyC2;

  DummyC = FILE_Getc(in);
  DWORD Bpp = DummyC;

  LOG ("Width: %i Height: %i Bpp: %i",Width,Height,Bpp);
  
  DummyC = FILE_Getc(in);
  
  img = (TImage *)GetMem (sizeof(TImage));
  if (img==NULL)
  {
    ERR ("Getting memory for TImage in TGA load of file %s",FileName);
    FILE_Close(in);
    return false;
  }

  img->width  = Width;
  img->height = Height;
  img->bpp = Bpp;
  img->pitch = Width*((Bpp+1)/8);
  img->paleta = NULL; 
  DWORD size = img->pitch*Height;
  img->bits = GetMem (size);
  if (img->bits==NULL)
  {
    ERR ("Getting memory for Bits in TGA load of file %s",FileName);
    FreeMem (img);
    FILE_Close(in);
    return false;
  }  

  if (!FILE_Read (img->bits,1,size,in))
  {
    ERR ("Reading in TGA load of file %s",FileName);
    FreeMem (img);
    FILE_Close(in);
    return false;
  }

  if (!FILE_Close(in))
  {
    ERR ("Closing in TGA load of file %s",FileName);
    FreeMem (img);
    FILE_Close(in);
    return false;
  }

  SEC_END();

  return img;
}
