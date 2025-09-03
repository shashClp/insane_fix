#include <windows.h>
#include <guio.h>
#include <view.h>
#include <misc.h>

bool START_Init()
{
	return true;
}

void START_Scene1(float tick)
{
	MISC_MemSet4 (GBL_Bits,0,GBL_Size);
}

void START_Close()
{
}

void START_Guio()
{
	GUIO_AddFN (START_Scene1,"START_Scene1");
}
