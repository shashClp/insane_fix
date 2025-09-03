// AW3D - Anaconda 3D Library
// Bezier

#ifndef AW3D_BEZIER
#define AW3D_BEZIER

#include <windows.h>
#include <a3dtypes.h>
#include <texture.h>
#include <vertex.h>

#define BEZIER_NORMAL	 1
#define BEZIER_LOOP		 2

#define BEZIER_MAX_CP 100

typedef struct {
	TVertex *cp;	// Control points
	WORD n, act;	// Number of curve control points
	DWORD flags;	// Curve flags
} TBezierCurve;

typedef struct {
	TObject *obj;			// 3D Object, if you want to convert the surface to a 3D object
	TTexture *txt;    // Texture for 3D Object
	TBezierCurve *cu; // Bezier curves (u)
	TBezierCurve *cv; // Bezier curves (v)
	WORD n, m, act;
} TBezierSurface;

void BEZIER_Init ();
void BEZIER_Close();

void BEZIER_CurveInit    (TBezierCurve &c, int n, DWORD flags);
void BEZIER_CurveClose   (TBezierCurve &c);
void BEZIER_CurveAddPoint(TBezierCurve &c, float x, float y, float z);
void BEZIER_CurveAddPoint(TBezierCurve &c, TVertex &p);
void BEZIER_CurvePoint   (TVertex &out, TBezierCurve &c, float u);
void BEZIER_Curve        (TVertex *out, TBezierCurve &c, int detail);

void     BEZIER_SurfaceInit        (TBezierSurface &s, WORD m);
void     BEZIER_SurfaceClose       (TBezierSurface &s);
void     BEZIER_SurfaceAddCurve    (TBezierSurface &s, TBezierCurve &c);
void     BEZIER_SurfaceGetPoint    (TVertex &out, TBezierSurface &s, WORD m, WORD n);
void     BEZIER_SurfaceSetPoint    (TBezierSurface &s, WORD m, WORD n, TVertex &in);
void     BEZIER_SurfacePoint       (TVertex &out, TBezierSurface &s, float u, float v);
void     BEZIER_Surface            (TVertex *out, TBezierSurface &s, int s_detail, int c_detail);
bool     BEZIER_SurfaceInit3DObject(TBezierSurface &s, int s_detail, int c_detail, char *txtname);
TObject* BEZIER_Surface2Object     (TBezierSurface &s, int s_detail, int c_detail);


#endif

