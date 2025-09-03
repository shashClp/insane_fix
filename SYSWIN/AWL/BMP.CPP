// AWL - Anaconda Windows Library
// BMP Loader

#include <file.h>
#include <image.h>
#include <log.h>
#include <mem.h>
#include <misc.h>

TImage *BMP_Load (char *FileName)
{
FILE *in;
TImage *img;
BITMAPINFOHEADER infocore;
BITMAPFILEHEADER infoheader;

  if (!FileName)
  {
    ERR ("Openng a BMP with NULL filename");  
    return false;
  }

  SEC_START ("Loading BMP File %s",FileName);

  if (!FILE_Open(&in,FileName,"rb")) 
  {
    ERR ("Opening a BMP file %s",FileName);
    return NULL;
  }

  FILE_Read(&infoheader,sizeof(BITMAPFILEHEADER),1,in);

  if (infoheader.bfType!=19778)
  {
    ERR ("Not a valid BMP file %s",FileName);
    FILE_Close(in);
    return NULL;
  }

  FILE_Read(&infocore,sizeof(BITMAPFILEHEADER),1,in);

  LOG ("Width: %i Height: %i Color: %i",infocore.biWidth,infocore.biHeight,infocore.biBitCount);

	FILE_Seek(in,infoheader.bfOffBits,SEEK_CUR);

  img = (TImage *) GetMem (sizeof(TImage));
  if (img==NULL) {
    ERR ("Error getting memory for TImage in BMP loading of %s",FileName);
    FILE_Close(in);
    return NULL;
  }

  img->width = infocore.biWidth;
  img->height = infocore.biHeight;
  img->bpp = infocore.biBitCount;
  img->pitch = img->width*((infocore.biBitCount+1)/8);
  img->paleta = NULL;

  img->bits = GetMem (img->pitch*img->height);
  if (img->bits==NULL) {
    ERR ("Error getting memory for Bits in BMP loading of %s",FileName);
    FreeMem(img);
    FILE_Close(in);
    return NULL;
  }

  FILE_Read(img->bits,img->pitch*img->height,1,in);

	FILE_Close(in);

  SEC_END();

  return img;
}
