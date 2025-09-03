// A3D - Anaconda 3D File System
// Anaconda 3D File (Main)

#include <windows.h>
#include <concat.h>
#include <a3d.h>


/////////////////////////////////////////////////////////////////
// Public functions                                            
/////////////////////////////////////////////////////////////////

CA3D_A3D::CA3D_A3D()
{
	object = NULL;
  camera = NULL;
  light = NULL;
  texture = NULL;
	numObjs = numCams = numLights = numTxts = 0;
};

CA3D_A3D::~CA3D_A3D()
{
  Free();
};

bool CA3D_A3D::Load(char *filename, char *txtpath)
{
  SEC_START("Anaconda 3D File System");
  if (!FILE_Open(&f,filename,"rb")) {
    ERR("Could not load A3D file: %s",filename);
    return false;
  }

  strcpy(this->txtpath,txtpath);

	object = NULL;
  camera = NULL;
  light = NULL;
  texture = NULL;
	numObjs = numCams = numLights = numTxts = 0;

  LOG("Loading A3D file %s",filename);

  char header[128];
  char version;

  int i=0, c=1;
  while (c!=0) {
    c = FILE_Getc(f);
    header[i] = (char) c;
    i++;
  }
  if (strcmp(header,"Anaconda 3D file format by Klauz/Anaconda")) {
    ERR("File %s is not an Anaconda 3D file", filename);
    FILE_Close(f);
    return false;
  }
  FILE_Read(&version,1,1,f);
  if (version != 2) {
    ERR("Wrong Anaconda 3D file version: %s", filename);
    FILE_Close(f);
    return false;
  }

  FILE_Read(&start,2,1,f);
  FILE_Read(&end,2,1,f);
  FILE_Read(&framestep,2,1,f);
  LOG("Start: %d",start);
  LOG("End: %d",end);
	frames = end + 1;

  TChunk chunk;
  while (!FILE_Eof(f)) {
    ReadChunk(chunk);
    switch(chunk.id) {
      case CHUNK_MATERIAL: ReadMaterials(); break;
      case     CHUNK_MESH: ReadMesh();      break;
      case   CHUNK_CAMERA: ReadCamera();    break;
      case    CHUNK_LIGHT: ReadLight();     break;
                  default: if (!FILE_Eof(f)) FILE_Seek(f,chunk.size,SEEK_CUR);
                           break;
    }
  }
  FILE_Close(f);
	AnalizeChild();

  LOG("Objects: %d",numObjs);
  LOG("Cameras: %d",numCams);
  LOG("Lights: %d",numLights);
  LOG("A3D file %s loaded OK!",filename);
	SEC_END();

	return true;
}

void CA3D_A3D::Free()
{
	if (object) {
		for(WORD i=0; i<numObjs; i++) {
			Free3DObject(&object[i]);
		}
		FreeMem(object);
		object = NULL;
	}

  if (camera) {
		for(WORD i=0; i<numCams; i++) {
			Free3DCamera(&camera[i]);
		}
		FreeMem(camera);
		camera = NULL;
	}

	if (light) {
		for(WORD i=0; i<numLights; i++) {
			Free3DLight(&light[i]);
		}
		FreeMem(light);
		light = NULL;
	}

  for(WORD i=0; i<numTxts; i++) {
    TXT_DelTexture(texture[i]);
  }
	texture = NULL;
  
	numObjs = numCams = numLights = numTxts = 0;

  LOG("Freeing A3D scene!");
}

/////////////////////////////////////////////////////////////////
// Private functions                                           
/////////////////////////////////////////////////////////////////

void CA3D_A3D::ReadMaterials()
{
  long seek=0, pathlen;
  char pathaux[128], txtpath[128], txtname[128];

  FILE_Read(&numTxts,2,1,f);
	if (numTxts>0) {
		strcpy(pathaux,this->txtpath);
		pathlen = strlen(this->txtpath);
		if (pathlen>0) {
			if (pathaux[pathlen-1] != '\\')	strcat(pathaux,"\\");
		}
		texture = (TTexture **) GetMem(sizeof(TTexture *)*numTxts);
		memset(texture,0,numTxts*sizeof(TTexture *));
		for(int i=0; i<numTxts; i++) {
			ReadName(txtname);
			strcpy(txtpath,pathaux);
			strcat(txtpath,txtname);
			if (CONCAT_Enabled) seek = FILE_Tell(f);
	    texture[i] = TXT_AddTexture(txtpath);
			if (CONCAT_Enabled) FILE_Seek(f,seek,SEEK_SET);
		}
	}
}

/////////////////////////////////////////////////////////////////

bool CA3D_A3D::AnalizeChunk(TChunk chunk)
{
  if (!FILE_Eof(f)) {
    if (chunk.id & 0x00ff) {
      FILE_Seek(f,chunk.size,SEEK_CUR);
      return false;
    }
  }
  return true;
}

void CA3D_A3D::ReadChunk(TChunk &chunk)
{
  FILE_Read(&chunk.id,2,1,f);
  FILE_Read(&chunk.size,sizeof(long),1,f);
}

void CA3D_A3D::ReadName(char *name)
{
  int i=0, c=1;
  while(c!=0) {
    c = FILE_Getc(f);
    name[i++] = (char) c;
  }
}

TSpline* CA3D_A3D::ReadPosSpline()
{
  TSpline *s;
  s = (TSpline *) GetMem(sizeof(TSpline));
  memset(s,0,sizeof(TSpline));

  TKey key;
  WORD numKeys;
  FILE_Read(&numKeys,2,1,f);
  for(int i=0; i<numKeys; i++) {
    key = ReadPosKey();
		SPLINE_AddKey(s,&key);
		if (i==0) s->start = key.frame;
  }
  SPLINE_Init(s);
  s->end = s->last->frame;
	s->frames = s->end + 1;
	s->sceneframes = frames;

  return s;
}

TSpline* CA3D_A3D::ReadRotSpline()
{
  TSpline *s;

  s = (TSpline *) GetMem(sizeof(TSpline));
  memset(s,0,sizeof(TSpline));

  TKey key;
  WORD numKeys;
  FILE_Read(&numKeys,2,1,f);

	TQuat oldq;
	float angle=0.0f;
	QUAT_Identity(oldq);
  for(int i=0; i<numKeys; i++) {
    key = ReadRotKey(angle,oldq);
    SPLINE_AddKey(s,&key);
		if (i==0) s->start = key.frame;
  }
	SPLINE_InitRotation(s);
  s->end = s->last->frame;
	s->frames = s->end + 1;
	s->sceneframes = frames;

  return s;
}

TSpline* CA3D_A3D::ReadRollSpline()
{
  TSpline *s;

  s = (TSpline *) GetMem(sizeof(TSpline));
  memset(s,0,sizeof(TSpline));

  TKey key;
  WORD numKeys;
  FILE_Read(&numKeys,2,1,f);
  for(int i=0; i<numKeys; i++) {
    key = ReadRollKey();
    SPLINE_AddKey(s,&key);
		if (i==0) s->start = key.frame;
  }
  SPLINE_Init(s);
  s->end = s->last->frame;
	s->frames = s->end + 1;
	s->sceneframes = frames;

  return s;
}

TKey CA3D_A3D::ReadPosKey()
{
  TKey key;
  memset(&key,0,sizeof(TKey));

  FILE_Read(&key.frame,2,1,f);
  FILE_Read(&key.tens,sizeof(float),1,f);
  FILE_Read(&key.cont,sizeof(float),1,f);
  FILE_Read(&key.bias,sizeof(float),1,f);
  FILE_Read(&key.easeFrom,sizeof(float),1,f);
  FILE_Read(&key.easeTo,sizeof(float),1,f);
  FILE_Read(&key.pos.x,sizeof(float),1,f);
  FILE_Read(&key.pos.y,sizeof(float),1,f);
  FILE_Read(&key.pos.z,sizeof(float),1,f);

  return key;
}

TKey CA3D_A3D::ReadRotKey(float &angle, TQuat &oldq)
{
  TKey key;
  memset(&key,0,sizeof(TKey));

	TQuat q;
	float rot[4];
  FILE_Read(&key.frame,2,1,f);
  FILE_Read(&key.tens,sizeof(float),1,f);
  FILE_Read(&key.cont,sizeof(float),1,f);
  FILE_Read(&key.bias,sizeof(float),1,f);
  FILE_Read(&key.easeFrom,sizeof(float),1,f);
  FILE_Read(&key.easeTo,sizeof(float),1,f);
  FILE_Read(rot,sizeof(float),4,f);
	QUAT_QuatFromAngle(q,rot[0],rot[1],rot[2],rot[3]);
	angle += rot[0];
	QUAT_Set(key.quat,angle,rot[1],rot[2],rot[3]);
	QUAT_Mul(oldq,q,oldq);
	key.qa=oldq;

  return key;
}

TKey CA3D_A3D::ReadRollKey()
{
  TKey key;
  memset(&key,0,sizeof(TKey));

  FILE_Read(&key.frame,2,1,f);
  FILE_Read(&key.tens,sizeof(float),1,f);
  FILE_Read(&key.cont,sizeof(float),1,f);
  FILE_Read(&key.bias,sizeof(float),1,f);
  FILE_Read(&key.easeFrom,sizeof(float),1,f);
  FILE_Read(&key.easeTo,sizeof(float),1,f);
  FILE_Read(&key.pos.x,sizeof(float),1,f);

  return key;
}

void CA3D_A3D::ReadMatrix(TMatrix &m)
{
  for (int i=0; i<3; i++) {
	  for (int j=0; j<4; j++) {
		  FILE_Read(&m[i][j],sizeof(float),1,f);
		}
	}
}

TVertex CA3D_A3D::ReadVertex()
{
	TVertex v;

  FILE_Read(&v.x,sizeof(float),1,f);
  FILE_Read(&v.y,sizeof(float),1,f);
  FILE_Read(&v.z,sizeof(float),1,f);

	return v;
}

TQuat CA3D_A3D::ReadQuat()
{
	TQuat q;

  FILE_Read(&q.w,sizeof(float),1,f);
  FILE_Read(&q.x,sizeof(float),1,f);
  FILE_Read(&q.y,sizeof(float),1,f);
  FILE_Read(&q.z,sizeof(float),1,f);

	return q;
}


/////////////////////////////////////////////////////////////////
// Free memory routines
/////////////////////////////////////////////////////////////////

void Free3DObject(TObject *obj)
{
	if (obj) {
		if (obj->vtx) FreeMem(obj->vtx);	
		if (obj->face) FreeMem(obj->face);	
		if (obj->plane) FreeMem(obj->plane);	
		if (obj->nrm) FreeMem(obj->nrm);	
		if (obj->inf) FreeMem(obj->inf);
		if (obj->pos) {
			SPLINE_Free(obj->pos);
			FreeMem(obj->pos);
		}
		if (obj->rot) {
			SPLINE_Free(obj->rot);
			FreeMem(obj->rot);
		}
		if (obj->sca) {
			SPLINE_Free(obj->sca);
			FreeMem(obj->sca);
		}
		LIST_Free(obj->prop);
		if (obj->anim) {
			if (obj->anim->pos) FreeMem(obj->anim->pos);
			if (obj->anim->rot) FreeMem(obj->anim->rot);
			if (obj->anim->sca) FreeMem(obj->anim->sca);
			FreeMem(obj->anim);
		}
	}
}

void Free3DCamera(TA3DCamera *cam)
{
	if (cam) {
		if (cam->campos) {
			SPLINE_Free(cam->campos);
			FreeMem(cam->campos);
		}
		if (cam->camroll) {
			SPLINE_Free(cam->camroll);
			FreeMem(cam->camroll);
		}
		if (cam->tarpos) {
			SPLINE_Free(cam->tarpos);
			FreeMem(cam->tarpos);
		}
	}
}

void Free3DLight(TA3DLight *light)
{
	if (light) {
    if (light->pos) {
			SPLINE_Free(light->pos);
			FreeMem(light->pos);
		}
		LIST_Free(light->prop);
	}
}
