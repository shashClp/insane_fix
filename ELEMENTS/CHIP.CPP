#include <image.h>
#include <log.h>

TImage *img8Chip1;
TImage *img8Chip1Txt;
TImage *img8Chip2;
TImage *img8Chip3;
TImage *img8Chip4;
TImage *img8Chip5;
TImage *img8Chip6;
TImage *img8Chip7;

//////////////////////////////////////////////////////////////////////////////
//
//  Inicializacion

bool CHIP_Init()
{
	SEC_START("Initializing CHIP!");
	img8Chip1 = IMAGE_Load("data\\chip1.gif",8);
	if (!img8Chip1) return false;
	img8Chip1Txt = IMAGE_Load("data\\chip1Txt.gif",8);
	if (!img8Chip1Txt) return false;
	img8Chip2 = IMAGE_Load("data\\chip2.gif",8);
	if (!img8Chip2) return false;
	img8Chip3 = IMAGE_Load("data\\chip3.gif",8);
	if (!img8Chip3) return false;
	img8Chip4 = IMAGE_Load("data\\chip4.gif",8);
	if (!img8Chip4) return false;
	img8Chip5 = IMAGE_Load("data\\chip5.gif",8);
	if (!img8Chip5) return false;
	img8Chip6 = IMAGE_Load("data\\chip6.gif",8);
	if (!img8Chip6) return false;
	img8Chip7 = IMAGE_Load("data\\chip7.gif",8);
	if (!img8Chip7) return false;

	LOG("CHIP initialized!");
	SEC_END();

	return true;
}


//////////////////////////////////////////////////////////////////////////////
//
//  Cierre

void CHIP_Close()
{
	FreeMem(img8Chip1);
	FreeMem(img8Chip1Txt);
	FreeMem(img8Chip2);
	FreeMem(img8Chip3);
	FreeMem(img8Chip4);
	FreeMem(img8Chip5);
	FreeMem(img8Chip6);
	FreeMem(img8Chip7);

  LOG ("CHIP closed!");
}

