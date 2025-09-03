#ifndef INSANE_TRASH
#define INSANE_TRASH

#define SCENE(scene,time)		while(!Keys[VK_ESCAPE] && iTick<time)\
								{																			\
									iTick = GetTickCount()-dwStartCount;\
									scene(iTick);												\
									VIDEO_Flip();												\
									iFrames++;													\
								}

#define SCENET(scene,time)		while(!Keys[VK_ESCAPE] && iTick<time)\
								{																			\
									iTick = GetTickCount()-dwStartCount;\
									scene;															\
									VIDEO_Flip();												\
									iFrames++;													\
								}

#define SINCRO(init)			dwStartCount = GetTickCount();\
								iTick = 0;															\
								init;									
								


//--------------------------------------------------------------------------
// Globales
extern TImage *img8GBL_Ruido;
extern SFuente* fntGBL_TypeWriter;

bool GBL_Init();
void GBL_Close();
void GBL_Guio();

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
//  MAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAINMAIN
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define TIME_FIN_LOGOINIIN          4000
#define TIME_FIN_LOGOINILUZ         5500
#define TIME_FIN_LOGOINIOUT         2500

#define TIME_FIN_ADN_IN				10000
#define TIME_FIN_ADN_2				13000
#define TIME_FIN_ADN				28000

#define TIME_FIN_SCENA_PLASMAIN     8000
#define TIME_FIN_SCENA_SCROLLLEFT   24000
#define TIME_FIN_SCENA_SCROLLUP     7000
#define TIME_FIN_SCENA_OUT          2500

#endif