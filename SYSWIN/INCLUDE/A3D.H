// A3D - Anaconda 3D File System
// Anaconda 3D File (Main)

#ifndef A3D_A3D
#define A3D_A3D

#include <windows.h>
#include <stdio.h>
#include <defines.h>
#include <log.h>
#include <debug.h>
#include <file.h>
#include <mem.h>
#include <list.h>
#include <traverse.h>
#include <stack.h>
#include <vertex.h>
#include <face.h>
#include <plane.h>
#include <bound.h>
#include <matrix.h>
#include <texture.h>
#include <info.h>
#include <spline.h>

#include <a3dchunks.h>
#include <a3ddefines.h>
#include <a3dtypes.h>


// A3D.CPP
void Free3DObject(TObject *obj);
void Free3DCamera(TA3DCamera *cam);
void Free3DLight (TA3DLight *light);	

// A3DOBJECT.CPP
void CenterObject(TObject *obj);

class CA3D_A3D {
public:
	DWORD count;

  CA3D_A3D();
  ~CA3D_A3D();

  bool Load(char *filename, char *txtpath);
  void Free();

/////////////////////////////////////////////////////////////////////////

  WORD     GetObjCount      ();
  WORD     GetObjNum        (char *objname);
  char*    GetObjName       (WORD n);
	TObject* GetObject        (WORD n);
	TObject* GetObject        (char *objname);

	TObject* GetWorldObject(WORD n);
	TObject* GetWorldObject(char *objname);

	TObject* GetRGBWorldObject(WORD n, DWORD r, DWORD g, DWORD b);
	TObject* GetRGBWorldObject(char *objname, DWORD r, DWORD g, DWORD b);

  DWORD    GetObjFlags      (WORD n);
  DWORD    GetObjFlags      (char *objname);
  DWORD    GetObjColor      (WORD n);
  DWORD    GetObjColor      (char *objname);
  TVertex* GetObjVertex     (WORD n);
  TVertex* GetObjVertex     (char *objname);
  TFace*   GetObjFace       (WORD n);
  TFace*   GetObjFace       (char *objname);
  TPlane*  GetObjPlane      (WORD n);
  TPlane*  GetObjPlane      (char *objname);
  TVertex* GetObjNormal     (WORD n);
  TVertex* GetObjNormal     (char *objname);
  TInfo*   GetObjInfo       (WORD n);
  TInfo*   GetObjInfo       (char *objname);
  TBound   GetObjBound      (WORD n);
  TBound   GetObjBound      (char *objname);
  TSpline* GetObjSplinePos  (WORD n);
  TSpline* GetObjSplinePos  (char *objname);
  TSpline* GetObjSplineRot  (WORD n);
  TSpline* GetObjSplineRot  (char *objname);
  TSpline* GetObjSplineSca  (WORD n);
  TSpline* GetObjSplineSca  (char *objname);

  TVertex* GetObjVertexCopy (WORD n);
  TVertex* GetObjVertexCopy (char *objname);
  TFace*   GetObjFaceCopy   (WORD n);
  TFace*   GetObjFaceCopy   (char *objname);
  TPlane*  GetObjPlaneCopy  (WORD n);
  TPlane*  GetObjPlaneCopy  (char *objname);
  TVertex* GetObjNormalCopy (WORD n);
  TVertex* GetObjNormalCopy (char *objname);
  TInfo*   GetObjInfoCopy   (WORD n);
  TInfo*   GetObjInfoCopy   (char *objname);
  char*    GetObjIniStr     (WORD n, char *key, char *def);
  char*    GetObjIniStr     (char *objname, char *key, char *def);
  int      GetObjIniInt     (WORD n, char *key, int def);
  int      GetObjIniInt     (char *objname, char *key, int def);
  float    GetObjIniFloat   (WORD n, char *key, float def);
  float    GetObjIniFloat   (char *objname, char *key, float def);
  bool     GetObjIniBool    (WORD n, char *key, bool def);
  bool     GetObjIniBool    (char *objname, char *key, bool def);

  TVertex	 GetObjPosFrame	  (WORD n, float frame, DWORD flags);
  TVertex	 GetObjPosFrame	  (char *objname, float frame, DWORD flags);
  TVertex	 GetObjScaFrame	  (WORD n, float frame, DWORD flags);
  TVertex	 GetObjScaFrame	  (char *objname, float frame, DWORD flags);
	void     GetObjFrame      (TMatrix &m, WORD n, float frame, DWORD flags);
  void     GetObjFrame      (TMatrix &m, char *objname, float frame, DWORD flags);

/////////////////////////////////////////////////////////////////////////

  WORD     GetCamCount		 ();
  WORD     GetCamNum			 (char *camname);
  char*    GetCamName			 (WORD n);
  DWORD    GetCamFlags		 (WORD n);
  DWORD    GetCamFlags		 (char *camname);
  TVertex  GetCamStPos		 (WORD n);
  TVertex  GetCamStPos		 (char *camname);
  float    GetCamStRoll		 (WORD n);
  float    GetCamStRoll		 (char *camname);

  TVertex  GetCamPosFrame  (WORD n, float frame, DWORD flags);
  TVertex  GetCamPosFrame  (char *camname, float frame, DWORD flags);
  float		 GetCamRollFrame (WORD n, float frame, DWORD flags);
  float		 GetCamRollFrame (char *camname, float frame, DWORD flags);
  void     GetCamFrame	   (TMatrix &m, WORD n, float frame, DWORD flags);
  void     GetCamFrame		 (TMatrix &m, char *camname, float frame, DWORD flags);

  char*    GetTarName			 (WORD n); // from camera n;
  DWORD    GetTarFlags		 (WORD n); // from camera n;
  DWORD    GetTarFlags		 (char *camname);
  TVertex  GetTarStPos	   (WORD n); // from camera n;
  TVertex  GetTarStPos		 (char *camname);

  TVertex  GetTarPosFrame  (WORD n, float frame, DWORD flags);
  TVertex  GetTarPosFrame  (char *camname, float frame, DWORD flags);

/////////////////////////////////////////////////////////////////////////

  WORD     GetLightCount   ();
  WORD     GetLightNum     (char *lightname);
  char*    GetLightName    (WORD n);
  DWORD    GetLightFlags   (WORD n);
  DWORD    GetLightFlags   (char *lightname);
  TVertex  GetLightStPos   (WORD n);
  TVertex  GetLightStPos   (char *lightname);
  DWORD    GetLightColor   (WORD n);
  DWORD    GetLightColor   (char *lightname);
  char*    GetLightIniStr  (WORD n, char *key, char *def);
  char*    GetLightIniStr  (char *lightname, char *key, char *def);
  int      GetLightIniInt  (WORD n, char *key, int def);
  int      GetLightIniInt  (char *lightname, char *key, int def);
  float    GetLightIniFloat(WORD n, char *key, float def);
  float    GetLightIniFloat(char *lightname, char *key, float def);
  bool     GetLightIniBool (WORD n, char *key, bool def);
  bool     GetLightIniBool (char *lightname, char *key, bool def);
	TVertex  GetLightFrame   (WORD n, float frame, DWORD flags);
  TVertex  GetLightFrame   (char *lightname, float frame, DWORD flags);

/////////////////////////////////////////////////////////////////////////

private:
  TObject *object;
  TA3DCamera *camera;
  TA3DLight *light;
  TTexture **texture;
  WORD numObjs,numCams,numLights,numTxts;

  char txtpath[128];
  WORD start, end, frames, framestep;

  FILE *f;

	bool AnalizeChunk (TChunk chunk);
  void ReadChunk    (TChunk &chunk);
  void ReadName     (char *name);
  void ReadMaterials();
  void ReadMesh     ();
  void ReadCamera   ();
  void ReadTarget   ();
  void ReadLight    ();
	
  TSpline* ReadPosSpline ();
	void     ReadPosSample (TObject *obj);
  TSpline* ReadRotSpline ();
	void     ReadRotSample (TObject *obj);
  TSpline* ReadRollSpline();
  TKey     ReadPosKey    ();
	TKey     ReadRotKey    (float &angle, TQuat &quat);
  TKey     ReadRollKey   ();
	void     ReadMatrix    (TMatrix &m);
	TVertex	 ReadVertex    ();
	TQuat		 ReadQuat      ();


	// Object private functions
	void AnalizeChild();
	void GetSampledTM(TMatrix &m,WORD n,float frame);
	void GetObjectTM (TMatrix &m,WORD n,float frame,DWORD flags);
	void GetNodeTM   (TMatrix &m,WORD n,float frame,DWORD flags);
};

#endif

