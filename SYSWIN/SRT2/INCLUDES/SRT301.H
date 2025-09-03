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
#include "RWorld.h"
#include "Rcamera.h"
//#include "CODE\Refects\inter.h"
#include "RObject.h"
#include "RObject.cpp"

#ifdef BlobOK
    #include "rblob.h"
    #include "rblob.cpp"
#endif
#ifdef CSGOK
    #include "rcsg.h"
    #include "rcsg.cpp"
#endif
#ifdef CubeOK
    #include "rcube.h"
    #include "rcube.cpp"
#endif
#ifdef CylinderOK
    #include "rcylinde.h"
    #include "rcylinde.cpp"
#endif
#ifdef EllipseOK
    #include "rellipse.h"
    #include "rellipse.cpp"
#endif
#ifdef PlaneOK
    #include "rplane.h"
    #include "rplane.cpp"
#endif
#ifdef SorOK
    #include "rsor.h"
    #include "rsor.cpp"
#endif
#ifdef SphereOK
    #include "rsphere.h"
    #include "rsphere.cpp"
#endif

//==============================================================================
#include "rvertex.cpp"
//==============================================================================
//==============================================================================
#include "rbounder.cpp"
#include "rcamera.cpp"
#include "rget.cpp"
#include "rset.cpp"
#include "rworld.cpp"
#include "rlight.cpp"
//==============================================================================
#include "rdefines.cpp"
#endif
