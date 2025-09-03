// AWL - Anaconda Windows Library
// Bitmap Files

#include <mem.h>
#include <log.h>
#include <image.h>
#include <bitmap.h>

TImage *BITMAP_Load (char *FileName,int bpp)
{
TImage *img;
HBITMAP hbmp;
HDC hdc;
TBitMapInfo BITMAP_bi;

  if (!FileName)
  {
    ERR ("Filename is empty in BITMAP Load");
    return NULL;
  }
    
  hbmp = (HBITMAP) LoadImage (NULL,FileName,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION | LR_LOADFROMFILE);
  if (hbmp==NULL) 
  {
    ERR ("Loading Bitmap file %s",FileName);
    return NULL;
  }

  img = (TImage *) GetMem (sizeof(TImage));
  if (img==NULL)
  {    
    ERR ("Getting memory for image structure");
    DeleteObject (hbmp);  
    return NULL;
  }
  img->bits = NULL;
  img->paleta = NULL;
  
  hdc = GetDC (0);  
  ZeroMemory(&BITMAP_bi,sizeof(BITMAPV5HEADER));
  BITMAP_bi.Header.bV5Size = sizeof (BITMAPV5HEADER);
  if (GetDIBits(hdc,hbmp,0,0,NULL,(BITMAPINFO *)&BITMAP_bi,DIB_RGB_COLORS)==0)
  {
    ERR ("Getting DibBits info in file %s",FileName);
    ReleaseDC(0,hdc);    
    DeleteObject (hbmp);
    FreeMem(img);    
    return NULL;
  }

  img->width = BITMAP_bi.Header.bV5Width;
  img->height = BITMAP_bi.Header.bV5Height;
  img->bpp = bpp;
  img->pitch = ((bpp+1)>>3)*img->width;
  img->bits = GetMem (img->height*img->pitch);
  if (img->bits==NULL)
  {
    ERR ("Getting memory for bits info in file %s",FileName);
    ReleaseDC(0,hdc);    
    DeleteObject (hbmp);  
    return NULL;
  }

  LOG ("Width (%i) Height (%i) Bpp (%i) Pointer (%p)",img->width,img->height,img->bpp,img->bits);

  BITMAP_bi.Header.bV5Planes = 1;
  switch (bpp)
  {
    case 8:
      BITMAP_bi.Header.bV5Compression = BI_RGB;
      BITMAP_bi.Header.bV5BitCount = 8;      
      BITMAP_bi.Header.bV5SizeImage = 0;
      break;
    case 15:            
      BITMAP_bi.Header.bV5Compression = BI_BITFIELDS;
      BITMAP_bi.Header.bV5BitCount = 16;
      BITMAP_bi.Header.bV5RedMask = 0x00007C00;
      BITMAP_bi.Header.bV5GreenMask = 0x000003E0;
      BITMAP_bi.Header.bV5BlueMask = 0x0000001F;
      BITMAP_bi.Header.bV5AlphaMask = 0x0;                  
      break;
    case 16:
      BITMAP_bi.Header.bV5Compression = BI_BITFIELDS;
      BITMAP_bi.Header.bV5BitCount = 16;      
      BITMAP_bi.Header.bV5RedMask = 0x0000F800;
      BITMAP_bi.Header.bV5GreenMask = 0x000007E0;
      BITMAP_bi.Header.bV5BlueMask = 0x0000001F;
      BITMAP_bi.Header.bV5AlphaMask = 0x0;      
      break;
    case 24:
      BITMAP_bi.Header.bV5Compression = BI_BITFIELDS;
      BITMAP_bi.Header.bV5BitCount = 24;
      BITMAP_bi.Header.bV5RedMask = 0x00FF0000;
      BITMAP_bi.Header.bV5GreenMask = 0x00FF00;
      BITMAP_bi.Header.bV5BlueMask = 0x00FF;
      BITMAP_bi.Header.bV5AlphaMask = 0x0;      
      break;
    case 32:
      BITMAP_bi.Header.bV5Compression = BI_BITFIELDS;
      BITMAP_bi.Header.bV5BitCount = 32;
      BITMAP_bi.Header.bV5RedMask = 0x00FF0000;
      BITMAP_bi.Header.bV5GreenMask = 0x00FF00;
      BITMAP_bi.Header.bV5BlueMask = 0x00FF;
      BITMAP_bi.Header.bV5AlphaMask = 0xFF000000;
      break;
  }
  BITMAP_bi.Header.bV5Height = -BITMAP_bi.Header.bV5Height;  
  if ((DWORD)GetDIBits(hdc,hbmp,0,img->height,img->bits,(BITMAPINFO *)&BITMAP_bi,DIB_RGB_COLORS)<img->height)
  {
    ERR ("Getting DibBits in file %s",FileName);
    ReleaseDC(0,hdc);    
    DeleteObject (hbmp);  
    FreeMem(img);    
    return NULL;
  }
 
  ReleaseDC(0,hdc);
  DeleteObject (hbmp);

  SEC_END();

  return img;  
}

