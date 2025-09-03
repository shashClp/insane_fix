#include <efx2d.h>
#include <image.h>
#include <guio.h>
#include <log.h>

TImage *img8GBL_Ruido;
SFuente* fntGBL_TypeWriter;

bool GBL_Init()
{  
  SEC_START ("Initializing Globals");

  img8GBL_Ruido = IMAGE_Load("data\\ruido1.gif",8);
  if (!img8GBL_Ruido) return false;
  fntGBL_TypeWriter = FONT_Load("data\\fontzm01.gif");  
  if (!fntGBL_TypeWriter) return false;

  LOG ("Globals initialized!");
  SEC_END();
  return true;
}

void GBL_Close()
{
  FONT_Close(fntGBL_TypeWriter);
}

void GBL_Marge(float tick)
{
  Margen320x240();
}

void GBL_Guio()
{
  GUIO_AddFN (GBL_Marge,"GBL_Marge");
}
