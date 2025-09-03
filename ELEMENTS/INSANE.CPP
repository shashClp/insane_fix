//	- E L E M E N T S - by Anaconda

#include <awm.h>
#include <a3d.h>
#include <awl.h>
#include <aw3d.h>
#include <default.h>
#include <efx2d.h>

// Includes
#include "def.h"
#include "3d.h"
#include "rt.h"
#include "blurs.h"

// Escenas
#include "trash.h"
#include "logoini.h"
#include "blur.h"
#include "adn.h"
#include "sprays.h"
#include "blobs.h"
#include "rotoalpha.h"
#include "elite.h"
#include "txt.h"
#include "start.h"
#include "creedz.h"
#include "celulas.h"
#include "chip.h"
#include "bruja.h"
#include "greetz.h"
#include "klauz.h"
#include "cristal.h"
#include "jloguich.h"
#include "particle.h"
#include "sphere.h"
#include "video2.h"
#include "estel.h"

void main (int argc,char *argv[])
{
  //--------------------------------------------------------------------------
  // Inicializaciones
  if (!DEFAULT_Init()) return;

  if (!MUSIC_Load ("insane.it")) return;

  GUIO_Init ();

  if (!GBL_Init()) return;
  if (!BLURS_Init()) return; 
  if (!START_Init()) return; 
  if (!LOGOINI_Init()) return; 
  if (!ADN_Init()) return;
  if (!SPRAYS_Init()) return;
  if (!insaneBLOBS_Init()) return;
  if (!ROTOALPHA_Init()) return;
  if (!ELITE_Init()) return;
  if (!TXT2_Init()) return;
	if (!CREEDZ_Init()) return;
	if (!CELULA_Init()) return;
	if (!CHIP_Init()) return;
  if (!BRUJA_Init()) return;
  if (!GREETZ_Init()) return;
  if (!VIDEOS_Init()) return;
  if (!KLAUZ_Init()) return;
  if (!CRISTALL_Init()) return;
	if (!ECPMC_Init2()) return;
	if (!SPHERE_Init()) return;
  if (!Particles_Init()) return;
  if (!ESTEL_Init()) return;

	VIDEO_Guio();

	START_Guio();
  LOGOINI_Guio();
  ADN_Guio();
  SPRAYS_Guio();
  insaneBLOBS_Guio();
  ROTOALPHA_Guio();
  ELITE_Guio();
  TXT2_Guio();
  BRUJA_Guio();
  CELULA_Guio();
  CREEDZ_Guio();
  GREETZ_Guio();
  VIDEOS_Guio();  
	KLAUZ_Guio();
	CRISTALL_Guio();
	ECPMC_Guio();
  SPHERE_Guio();
  GBL_Guio();
  ESTEL_Guio();
  Particles_Guio();

	CONCAT_FilesUsed();

  GUIO_Load("elements.agf");

	GUIO_InitBufferPos(60*5);
	GUIO_LoadBufferPos();
	GUIO_Run (GUIO_StartRun);

  ESTEL_Close();
  VIDEOS_Close();
  TXT2_Close();
  ELITE_Close();
  ROTOALPHA_Close();
  insaneBLOBS_Close();
  SPRAYS_Close(); 
  ADN_Close();
  LOGOINI_Close();  
  BLURS_Close();
  GREETZ_Close();
  BRUJA_Close();
  CHIP_Close();
  CREEDZ_Close();
  CELULA_Close();
	START_Close();
	KLAUZ_Close();
	CRISTALL_Close();
	ECPMC_Close();
  SPHERE_Close();
  Particles_Close();
  GUIO_Close();
  GBL_Close();

  DEFAULT_Close();
}
