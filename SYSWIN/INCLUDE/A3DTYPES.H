// A3D - Anaconda 3D File System
// Anaconda 3D File (Types)

#ifndef A3D_TYPES
#define A3D_TYPES

#include <quat.h>
#include <bound.h>
#include <vertex.h>
#include <plane.h>
#include <list.h>
#include <matrix.h>
#include <spline.h>
#include <face.h>
#include <info.h>

typedef struct {
  WORD id;
  long size;
} TChunk;

typedef struct {
	TVertex p;
	float frame;
} TAnimPoint;

typedef struct {
	TQuat q;
	float frame;
} TAnimQuat;

typedef struct {
	TAnimPoint *pos;
	TAnimQuat *rot;
	TAnimPoint *sca;
	WORD frames;
	WORD sceneframes;
} TAnim;

typedef struct {
	char name[32];
	WORD parent;		// object parent
	TList child;		// list of object child
  DWORD flags;
  DWORD color;
	DWORD numvtx;
	DWORD numface;
	DWORD numinf;
	TVertex *vtx;
	TFace *face;
  TPlane *plane;
  TVertex *nrm;
	TInfo *inf;
  TList prop;		  // user defined properties from 3D Studio MAX.

  TSpline *pos;
  TSpline *rot;
  TSpline *sca;
	TMatrix trmtx;			// transformation matrix
	TMatrix objoffmtx;	// object-offset matrix
	TVertex inipos;			// transformed position (from pivot)
	TQuat inirot;				// transformed rotation (from pivot)
	TVertex inisca;			// transformed scale (from pivot)
	TAnim *anim;				// sampled animation
	TBound bound;
} TObject;

typedef struct {
  char camname[32];
  TVertex camstpos; // static position.
  float camstroll;  // static roll.
  TSpline *campos;
  TSpline *camroll;
  char tarname[32];
  TVertex tarstpos; // static position.
  TSpline *tarpos;
  DWORD camflags,tarflags;
} TA3DCamera;

typedef struct {
  char name[32];
  DWORD flags;
	DWORD color;
  TVertex stpos;	// static position.
  TSpline *pos;
  TList prop;			// user defined properties from 3D Studio MAX.
} TA3DLight;

#endif

