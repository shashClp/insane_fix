// AWL - Anaconda Windows Library
// GIF Loader

#include <image.h>
#include <mem.h>
#include <log.h>
#include <file.h>
#include <misc.h>

#define MAX_CODES     4096

FILE *fp;
long curr_size;
long clear;
long ending;
long newcodes;
long top_slot;
long slot;
long navail_BYTEs = 0;
long nbits_left = 0;
BYTE b1;
BYTE BYTE_buff[257];
BYTE *pBYTEs;
BYTE *stack;
BYTE *suffix;
WORD *prefix;
DWORD code_mask[13] = { 0L,0x0001L,0x0003L,0x0007L,0x000FL,0x001FL,0x003FL,
                        0x007FL,0x00FFL,0x01FFL,0x03FFL,0x07FFL,0x0FFFL };

inline long gif_getw(FILE *f)
{
  long k = (unsigned) FILE_Getc(f);
  k += 256*FILE_Getc(f);

return k;
}

static long gif_get_next_code(void)
{
register long i;
static unsigned long ret;

  if(!nbits_left)
  {
    if(navail_BYTEs <= 0)
    {
      pBYTEs = BYTE_buff;
      navail_BYTEs = FILE_Getc(fp);
      if(navail_BYTEs)
        for( i = 0; i < navail_BYTEs; ++i )
          BYTE_buff[i] = (BYTE)FILE_Getc( fp );
    }
    b1 = *pBYTEs++;
    nbits_left = 8;
    --navail_BYTEs;
  }
  ret = b1 >> (8 - nbits_left);
  while( curr_size > nbits_left )
  {
    if( navail_BYTEs <= 0 )
    {
      pBYTEs = BYTE_buff;
      navail_BYTEs = FILE_Getc( fp );
      if( navail_BYTEs )
        for( i = 0; i < navail_BYTEs; ++i )
          BYTE_buff[i] = (BYTE)FILE_Getc( fp );
    }
    b1 = *pBYTEs++;
    ret |= b1 << nbits_left;
    nbits_left += 8;
    --navail_BYTEs;
  }
  nbits_left -= curr_size;
  return( (int) (ret & code_mask[curr_size]) );
}


TImage *GIF_Load (char *FileName)
{
BYTE* sp;
char *outpix;
long code,fc,oc,i,c;
BYTE size;
BYTE buf[1028];
TImage *img;
DWORD tam;

  if (!FileName)
  {
    ERR ("Opening a GIF with NULL filename");
    return false;
  }

  SEC_START ("Loading GIF image %s",FileName);

  if (!FILE_Open(&fp,FileName,"rb")) 
  {
    ERR ("Opening a GIF file %s",FileName);
    return NULL;
  }
  
  if (!FILE_Read(buf,1,6,fp))
  {
    ERR ("Reading header from GIF file %s",FileName);
    FILE_Close (fp);
    return NULL;
  }

  if ((buf[0]!='G') || (buf[1]!='I') || (buf[2]!='F'))
  {
    ERR ("Not a GIF file %s",FileName);
    FILE_Close (fp);
    return NULL;
  }

  if (!FILE_Read(buf,1,7,fp))
  {
    ERR ("Reading header from GIF file %s",FileName);
    FILE_Close (fp);
    return NULL;
  }

  img = (TImage *) GetMem (sizeof(TImage));
  if (img==NULL)
  {
    ERR ("Error getting memory for TImage in GIF loading of %s",FileName);
    FILE_Close (fp);
    return NULL;
  }

  img->paleta = (BYTE *) GetMem(256*3);
  if (img->paleta==NULL)
  {
    ERR ("Error getting memory for Paleta in GIF loading of %s",FileName);
    FreeMem ((void *)img);
    FILE_Close (fp);
    return NULL;
  }

  for(i=0; i<3*(2<<(buf[4] & 7)); i++)
  {
    c = FILE_Getc(fp);
    *(img->paleta+i) = (BYTE)(c >> 2);
  }

  if (!FILE_Read(buf,1,5,fp))
  {
    ERR ("Reading header from GIF file %s",FileName);
    FreeMem ((void *)img->paleta);
    FreeMem ((void *)img);
    FILE_Close (fp);
    return NULL;
  }
  
  img->bpp = 8;
  img->width = gif_getw(fp);
  img->height = gif_getw(fp);
  img->pitch = img->width;
  tam = img->pitch*img->height;
  img->bits = GetMem (tam);  
  if (img->bits==NULL)
  {
    ERR ("Error getting memory for Bits in GIF loading of %s",FileName);
    FreeMem (img);
    FILE_Close (fp);
    return NULL;
  }

  LOG ("Width: %i Height: %i Color: %i",img->width,img->height,img->bpp);

  if (!FILE_Seek(fp,1,1))
  {
    ERR ("Error seeking in GIF loading of %s",FileName);
    FreeMem (img);
    FILE_Close (fp);
    return NULL;
  }
    
  size = (BYTE)FILE_Getc(fp);
  if ((size<2 || 9<size))
  {
    ERR ("Not a GIF file %s",FileName);
    FreeMem (img);
    FILE_Close (fp);
    return NULL;
  }

  // Aqui ja no miro més error per no complicar el codi
  outpix = (char *) img->bits;
  stack  = (BYTE *) GetMem (MAX_CODES+1);
  suffix = (BYTE *) GetMem (MAX_CODES+1);
  prefix = (WORD *) GetMem (sizeof(*prefix) * (MAX_CODES + 1));
  curr_size = size + 1;
  top_slot = 1 << curr_size;
  clear = 1 << size;
  ending = clear + 1;
  slot = newcodes = ending + 1;
  navail_BYTEs = nbits_left = oc = fc = 0;
  sp = stack;
  while( (c=gif_get_next_code()) != ending)
  {
    if(c == clear)
    {
      curr_size = size + 1;
      slot = newcodes;
      top_slot = 1 << curr_size;
      while((c = gif_get_next_code()) == clear);
      if(c == ending) break;
      if(c >= slot) c = 0;
      oc = fc = c;
      *outpix++ = (BYTE)c;
    } else {
      code = c;
      if(code >= slot) { code = oc; *sp++ = (BYTE)fc; }
      while(code >= newcodes)
      { *sp++ = suffix[code]; code = prefix[code]; }
      *sp++ = (BYTE)code;
      if(slot < top_slot)
      {
        fc = code;
        suffix[slot]   = (BYTE)fc;
        prefix[slot++] = (WORD)oc;
        oc = c;
      }
      if(slot >= top_slot && curr_size < 12)
      { top_slot <<= 1; ++curr_size; }
      while(sp > stack) { --sp; *outpix++ = (BYTE)*sp; }
    }
  }
  FreeMem (stack);
  FreeMem (suffix);
  FreeMem (prefix);

  if (!FILE_Close(fp))
  {
    ERR ("Closing GIF file %s",FileName);
    FreeMem (img);
    FILE_Close (fp);
    return NULL;
  }

  SEC_END();

  return img;
}
