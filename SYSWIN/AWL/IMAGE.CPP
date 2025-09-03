// AWL - Anaconda Windows Library
// Image

#include <string.h>
#include <log.h>
#include <misc.h>
#include <mem.h>
#include <rgb.h>
#include <gif.h>
#include <bmp.h>
#include <tga.h>
#include <image.h>

TImage *IMAGE_Load(char *FileName,DWORD bppdesired)
{
  if (!FileName)
  {
    ERR ("Loading a NULL filename image?");
    return false;
  }

  char *ext = MISC_GetFileExtension(FileName);
  if (!ext)
  {
    ERR ("Bad extension returned from MISC_GetFileExtension in IMAGE_Load");
    return false;
  }

  TImage *tmp;
	if (stricmp (ext,"gif")==0) tmp = GIF_Load(FileName);
	if (stricmp (ext,"bmp")==0) tmp = BMP_Load(FileName);
	if (stricmp (ext,"tga")==0) tmp = TGA_Load(FileName);	
  if (!tmp) return NULL;
  if (tmp->bpp==bppdesired) return tmp;
  TImage *tmp2 = (TImage *)GetMem (sizeof(TImage));
  switch (tmp->bpp)
  {
    case 8:
      if (bppdesired==32) IMAGE_8To32 (tmp2,tmp);
      break;
    case 24:
      if (bppdesired==32) IMAGE_24To32 (tmp2,tmp);
      break;
  }
  FreeMem (tmp);
  return tmp2;
}

bool IMAGE_FlipV (TImage *img)
{
  if (img==NULL)
  {
    ERR ("IMAGE_FlipV NULL source");
    return false;
  }    
  if (img->bits==NULL)
  {
    ERR ("IMAGE_FlipV NULL source bits");
    return false;
  }
  void *tmp = GetMem (img->pitch);
  if (tmp==NULL)
  {
    ERR ("IMAGE_FlipV Not enough memory for tmp exchange bits");
    return false;
  }
  LOG ("Flipping image");
  char *src = (char *)img->bits;
  char *dst = ((char *)img->bits)+img->pitch*(img->height-1);
  while (src<dst)  
  {
    memcpy (tmp,src,img->pitch);
    memcpy (src,dst,img->pitch);
    memcpy (dst,tmp,img->pitch);
    src+=img->pitch;
    dst-=img->pitch;
  }
  FreeMem (tmp);
  
  return true;
}

bool IMAGE_24To32 (TImage *img1,TImage *img2)
{  
  if (img1==NULL)
  {
    ERR ("IMAGE_24To32 NULL destination");
    return false;
  }
  if (img2==NULL)
  {
    ERR ("IMAGE_24To32 NULL source");
    return false;
  }
  if (img2->bits==NULL)
  {
    ERR ("IMAGE_24To32 NULL source bits");
    return false;
  }
  if (img2->bpp!=24)
  {
    ERR ("IMAGE_24To32 source is not 32bpp");
    return false;
  }

	LOG("Converting image 24 to 32");

  img1->width = img2->width;
  img1->height = img2->height;  
	img1->pitch = img2->width*4;
  img1->bpp = 32;
  img1->paleta = NULL;
  img1->bits = GetMem (img1->pitch*img1->height);
  if (img1->bits==NULL)
  {
    ERR ("IMAGE_24To32 Error getting memory from destination");
    return false;
  }

  DWORD *dst = (DWORD *)img1->bits;
  BYTE *src = (BYTE *)img2->bits;
  for (DWORD i=0; i<img1->width*img1->height; i++)
  {    
    *dst++ = *((DWORD *)src) & 0xffffff;
    src+=3;
  }

  return true;	
}

bool IMAGE_8To32 (TImage *img1,TImage *img2)
{  
  if (img1==NULL)
  {
    ERR ("IMAGE_8To32 NULL destination");
    return false;
  }
  if (img2==NULL)
  {
    ERR ("IMAGE_8To32 NULL source");
    return false;
  }
  if (img2->bits==NULL)
  {
    ERR ("IMAGE_8To32 NULL source bits");
    return false;
  }
  if (img2->bpp!=8)
  {
    ERR ("IMAGE_8To32 source is not 8bpp");
    return false;
  }

	LOG("Converting image 8 to 32");

  img1->width = img2->width;
  img1->height = img2->height;  
	img1->pitch = img2->width*4;
  img1->bpp = 32;
  img1->paleta = NULL;
  img1->bits = GetMem (img1->pitch*img1->height);
  if (img1->bits==NULL)
  {
    ERR ("IMAGE_8To32 Error getting memory from destination");
    return false;
  }

  DWORD *dst = (DWORD *)img1->bits;
  BYTE *src = (BYTE *)img2->bits;
  for (DWORD i=0; i<img1->width*img1->height; i++)
  {
    BYTE *pal = img2->paleta+(*src++)*3;
    RGB_Set (*dst++,pal[0]<<2,pal[1]<<2,pal[2]<<2);
  }
 
  return true;	
}

void FreeMem (TImage *img)
{
  if (img->bits) FreeMem (img->bits);
  if (img->paleta) FreeMem (img->paleta);    
}

