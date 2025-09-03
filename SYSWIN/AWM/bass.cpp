#include <windows.h>
#include <awm.h>
#include <log.h>
#include "bass\bass.h"

HMUSIC mod;
HSTREAM str;

BOOL MUSIC=false;

bool MUSIC_Init(DWORD freq, DWORD flags)
{
#ifdef MUSIC_BASS

  SEC_START ("Initializing MUSIC System");
	if (!BASS_Init(-1,freq,flags,GetForegroundWindow())) {
		ERR ("Could not initialize device");
    return false;
  }
  BASS_Start();

	MUSIC=true;

  SEC_END ();
#endif

  return true;
}

bool MUSIC_Load(char *filename)
{
#ifdef MUSIC_BASS

	if (MUSIC) {
		SEC_START ("Loading module %s",filename);
		if ((str=BASS_StreamCreateFile(FALSE,filename,0,0,0))==NULL) {
			mod=BASS_MusicLoad(FALSE,filename,0,0,BASS_MUSIC_LOOP | BASS_MUSIC_RAMP);
			if (mod==0) {			
				ERR ("Could not load module %s",filename);
				return false;
			}
			LOG("Module %s loaded OK!",filename);
		}  
		SEC_END();
	}


#endif

  return true;
}

DWORD MUSIC_GetVolume()
{
#ifdef MUSIC_BASS
  if (MUSIC) return BASS_GetVolume();
#endif
  return 0;
}

void MUSIC_Volume(DWORD value)
{
#ifdef MUSIC_BASS
  if (MUSIC) BASS_SetVolume (value);
#endif
}

void MUSIC_Play()
{
#ifdef MUSIC_BASS
	if (MUSIC) {
		if (str) BASS_StreamPlay(str,FALSE,BASS_SAMPLE_LOOP);
		else BASS_MusicPlay(mod);
	}
#endif
}

void MUSIC_Close()
{
#ifdef MUSIC_BASS
  if (MUSIC) BASS_Free();
#endif
}


DWORD MUSIC_GetPos()
{
#ifdef MUSIC_BASS
	if (MUSIC) {
		if (!str) return BASS_ChannelGetPosition(mod);
	}
#endif
	return 0;
}

void MUSIC_PlayExt(DWORD Pos)
{
#ifdef MUSIC_BASS
	if (MUSIC) {
		if (str) BASS_StreamPlay(str,FALSE,BASS_SAMPLE_LOOP);
		{
				BASS_MusicPlayEx (mod,Pos,-1,true);
		}
	}
#endif
}

