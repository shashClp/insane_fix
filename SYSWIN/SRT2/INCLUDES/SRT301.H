#ifndef SYSR301
#define SYSR301

//==============================================================================
  #define PlaneOK
  #define CubeOK
  #define BlobOK
  #define SphereOK
  #define EllipseOK
  #define RayDimX R_RAY_DIM
  #define RayDimY R_RAY_DIM
  #define SYR_PROCEDURAL_TEXTURES

//==============================================================================
#define R_ERROR_EXIT
#define R_LOG_DEF
//#define NDEBUG
//#define DEBUG
//#define DEBUG_RENDER
//#define DEBUG_RENDER_REFLECT

//#define R_ENABLE_FILES_DEPENDENCIES

#include <cmath>
//#include "CODE\RDefines\extra.h"
#include "CODE\Rbase\RWorld.h"
#include "CODE\Rbase\Rcamera.h"
//#include "CODE\Refects\inter.h"
#include "CODE\RObjects\RObject.h"
#include "CODE\RObjects\RObject.cpp"

#ifdef BlobOK
    #include "CODE\RObjects\rblob.h"
    #include "CODE\RObjects\rblob.cpp"
#endif
#ifdef CSGOK
    #include "CODE\RObjects\rcsg.h"
    #include "CODE\RObjects\rcsg.cpp"
#endif
#ifdef CubeOK
    #include "CODE\RObjects\rcube.h"
    #include "CODE\RObjects\rcube.cpp"
#endif
#ifdef CylinderOK
    #include "CODE\RObjects\rcylinde.h"
    #include "CODE\RObjects\rcylinde.cpp"
#endif
#ifdef EllipseOK
    #include "CODE\RObjects\rellipse.h"
    #include "CODE\RObjects\rellipse.cpp"
#endif
#ifdef PlaneOK
    #include "CODE\RObjects\rplane.h"
    #include "CODE\RObjects\rplane.cpp"
#endif
#ifdef SorOK
    #include "CODE\RObjects\rsor.h"
    #include "CODE\RObjects\rsor.cpp"
#endif
#ifdef SphereOK
    #include "CODE\RObjects\rsphere.h"
    #include "CODE\RObjects\rsphere.cpp"
#endif

//==============================================================================
#include "CODE\RDefines\rvertex.cpp"
//==============================================================================
//==============================================================================
#include "CODE\Rbase\rbounder.cpp"
#include "CODE\Rbase\rcamera.cpp"
#include "CODE\Rbase\rget.cpp"
#include "CODE\Rbase\rset.cpp"
#include "CODE\Rbase\rworld.cpp"
#include "CODE\REFECTS\rlight.cpp"
//==============================================================================
#include "CODE\RDefines\rdefines.cpp"
#endif
