#ifndef AWL_SYSR30
#define AWL_SYSR30

//==============================================================================
  #define PlaneOK
  #define CubeOK
  #define SphereOK
  #define EllipseOK
  #define RayDimX R_RAY_DIM
  #define RayDimY R_RAY_DIM
  #define SYR_PROCEDURAL_TEXTURES
  #define R_ENABLE_FILES_DEPENDENCIES
//==============================================================================
#define ERROR_LOG

extern long R_RAY_DIM;

#include <cmath>
//#include <..\CODE\RDefines\extra.h>
#include <..\CODE\Rbase\RWorld.h>
#include <..\CODE\Rbase\Rcamera.h>
#include <..\CODE\Refects\inter.h>
#include <..\CODE\RObjects\RObject.h>
#include <..\CODE\RObjects\RSphere.h>
#include <..\CODE\RObjects\RPlane.h>
#include <..\CODE\RObjects\REllipse.h>

#endif
