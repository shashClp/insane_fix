// A3D - Anaconda 3D File System
// Anaconda 3D File System (Objects)

#include <windows.h>
#include <a3d.h>
#include <rgb.h>
#include <math.h>

/////////////////////////////////////////////////////////////////
// Private functions                                           //
/////////////////////////////////////////////////////////////////

void CA3D_A3D::ReadMesh()
{
  int n = numObjs;
	object = (TObject *) realloc(object,(n+1)*sizeof(TObject));

  TObject *obj = &object[n];
	memset(obj,0,sizeof(TObject));

  ReadName(obj->name);

	SEC_START("Object: %s", obj->name);

  char prop[128];
  int c = FILE_Getc(f);
  while (c != 0) {
    FILE_Seek(f,-1,SEEK_CUR);
    ReadName(prop);
    LIST_Add(obj->prop,prop,strlen(prop)+1);
    c = FILE_Getc(f);
  }

	FILE_Read(&obj->parent,2,1,f);
	if (obj->parent == 0xffff) obj->flags |= OBJ_ISROOT;

	TFace face;
	long w, h;
  TVertex v1,v2,v3;
  WORD faceid, nFrames;
  int i, numVerts, numFaces;

  TChunk chunk;
  bool mainChunk=FALSE, matFounded = FALSE;
  ReadChunk(chunk);
  while (!FILE_Eof(f) && !mainChunk) {
    switch(chunk.id) {
      case CHUNK_COLOR:
					FILE_Read(&obj->color,sizeof(DWORD),1,f);
					break;
      case CHUNK_INITPOS:
				  FILE_Read(&obj->inipos,sizeof(float),3,f);
					break;
      case CHUNK_INITROT:
				  FILE_Read(&obj->inirot,sizeof(float),4,f);
					break;
      case CHUNK_INITSCA:
				  FILE_Read(&obj->inisca,sizeof(float),3,f);
					break;
      case CHUNK_VERTEX:
          FILE_Read(&numVerts, sizeof(int), 1, f);
					LOG("Vertices: %d",numVerts);
          obj->numvtx = numVerts;
          obj->vtx = (TVertex *) GetMem(sizeof(TVertex)*numVerts);
					FILE_Read(obj->vtx,sizeof(TVertex),numVerts,f);
					BOUND_CalcFromVertex(obj->bound,obj->vtx,obj->numvtx);
          break;
      case CHUNK_FACES:
          FILE_Read(&numFaces, sizeof(int), 1, f);
					LOG("Faces: %d",numFaces);
          obj->numface = numFaces;
          obj->face = (TFace *) GetMem(sizeof(TFace)*numFaces);
          for (i=0; i<numFaces; i++) {
            FILE_Read(&obj->face[i],2,3,f);
            FILE_Read(&faceid,2,1,f);
            obj->face[i].i = NULL;
            if (numTxts) {
              if (faceid != 0xffff) {
                matFounded = TRUE;
                obj->face[i].i = texture[faceid];
              }
            }
          }
          break;
      case CHUNK_PLANES:
          // Normal faces.
          obj->plane = (TPlane *) GetMem(sizeof(TPlane)*obj->numface);
					for (i=0; i<(int)obj->numface; i++) {
						FILE_Read(&v1,sizeof(TVertex),1,f);
						VTX_Normalize(v1);
						PLA_Set(obj->plane[i],v1.x,v1.y,v1.z,0.0f);
						obj->plane[i].D = PLA_CalcD(obj->plane[i],v1);
					}
          break;
      case CHUNK_NORMALS:
          // Normal vertices.
          obj->nrm = (TVertex *) GetMem(sizeof(TVertex)*obj->numvtx);
          for (i=0; i<(int)obj->numface; i++) {
						face = obj->face[i];
            FILE_Read(&obj->nrm[face.a],sizeof(float),3,f);
            FILE_Read(&obj->nrm[face.b],sizeof(float),3,f);
            FILE_Read(&obj->nrm[face.c],sizeof(float),3,f);
          }
          break;
      case CHUNK_UVCOORDS:
          obj->inf = (TInfo *) GetMem(sizeof(TInfo)*3*obj->numface);
          obj->numinf = 3*obj->numface;
          for (i=0; i<(int)obj->numface; i++) {
            FILE_Read(&v1,sizeof(float),2,f);
            FILE_Read(&v2,sizeof(float),2,f);
            FILE_Read(&v3,sizeof(float),2,f);
            if (obj->face[i].i) {
              v1.y = 1.0f-v1.y;
              v2.y = 1.0f-v2.y;
              v3.y = 1.0f-v3.y;
              w = obj->face[i].i->w;
              h = obj->face[i].i->h;
              obj->inf[i*3+0].vv = v1.x*w;
              obj->inf[i*3+1].vv = v2.x*w;
              obj->inf[i*3+2].vv = v3.x*w;

              obj->inf[i*3+0].uu = v1.y*h;
              obj->inf[i*3+1].uu = v2.y*h;
              obj->inf[i*3+2].uu = v3.y*h;
            }
          }
          break;
      case  CHUNK_SAMPLE_ANIM:
					obj->anim = (TAnim *) GetMem(sizeof(TAnim));
          FILE_Read(&nFrames,2,1,f);
					obj->anim->frames = nFrames;
					obj->anim->sceneframes = frames;
					obj->anim->pos = (TAnimPoint *) GetMem(sizeof(TAnimPoint)*nFrames);
					obj->anim->rot = (TAnimQuat *) GetMem(sizeof(TAnimQuat)*nFrames);
					obj->anim->sca = (TAnimPoint *) GetMem(sizeof(TAnimPoint)*nFrames);
					for (i=0; i<nFrames; i++) {
						obj->anim->pos[i].p = ReadVertex();
						obj->anim->pos[i].frame = (float) i*framestep;
						obj->anim->rot[i].q = ReadQuat();
						obj->anim->rot[i].frame = (float) i*framestep;
						obj->anim->sca[i].p = ReadVertex();
						obj->anim->sca[i].frame = (float) i*framestep;
					}
					break;
      case     CHUNK_TRMATRIX: ReadMatrix(obj->trmtx);          break;
      case CHUNK_OBJOFFMATRIX: ReadMatrix(obj->objoffmtx);			break;
      case   CHUNK_SPLINE_POS: obj->pos = ReadPosSpline();      break;
      case   CHUNK_SPLINE_ROT: obj->rot = ReadRotSpline();      break;
      case   CHUNK_SPLINE_SCA: obj->sca = ReadPosSpline();      break;
                      default: mainChunk = AnalizeChunk(chunk); break;
    }
	  if (!mainChunk) ReadChunk(chunk);
  }
  if (matFounded) obj->flags |= OBJ_TEXTURED;
  if (obj->pos || obj->rot || obj->sca || obj->anim) {
		obj->flags |= OBJ_ANIMATED;
		LOG("Animated: yes");
	} else LOG("Animated: no");
	if (obj->anim) obj->flags |= OBJ_SAMPLED;

	numObjs++;

  if (!FILE_Eof(f)) FILE_Seek(f,-6,SEEK_CUR);  // tiro enrrera 2 pel chunk i un long
	                                         // pel size del chunk.
	SEC_END();
}

void CA3D_A3D::AnalizeChild()
{
	for(WORD i=0; i<numObjs; i++) {
		for(WORD j=0; j<numObjs; j++) {
			if (object[j].parent == i) {
				WORD parent = object[j].parent;
				LIST_Add(object[i].child,&j,2);
				if (object[parent].flags & OBJ_ANIMATED) {
					object[j].flags |= OBJ_ANIMATED;
				}
			}
		}
		if (object[i].child.count > 0) object[i].flags |= OBJ_HASCHILD;
		if (!(object[i].flags & OBJ_ISROOT) ||
		     (object[i].flags & OBJ_HASCHILD)) {
			object[i].flags |= OBJ_HIERARCHY;
		}
	}
}

/////////////////////////////////////////////////////////////////
// Public functions                                            //
/////////////////////////////////////////////////////////////////

WORD CA3D_A3D::GetObjCount()
{ return numObjs; }

WORD CA3D_A3D::GetObjNum(char *objname)
{
  if (!numObjs) return 0xffff;

  WORD i = 0;

  for(i=0; i<numObjs; i++)
    if (!stricmp(object[i].name,objname)) break;

  return i;
}

char* CA3D_A3D::GetObjName (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object name. Object: %d",n);
    return NULL;
  }
  return object[n].name;
}

TObject* CA3D_A3D::GetObject (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object %d",n);
    return 0;
  }
  return &object[n];
}

TObject* CA3D_A3D::GetObject (char *objname)
{ return GetObject(GetObjNum(objname)); }

TObject* CA3D_A3D::GetRGBWorldObject (WORD n, DWORD r, DWORD g, DWORD b)
{
  if (n >= numObjs) {
    ERR("Could not get object %d",n);
    return 0;
  }
	TObject *obj = &object[n];
	TVertex *v = obj->vtx;
	TVertex vaux;
	for (WORD i=0; i<obj->numvtx; i++) {
		MTX_Mul(vaux,v[i],obj->trmtx);
		v[i] = vaux;
	}
	DWORD col;
	RGB_Set(col,r,g,b);
	TFace *f = obj->face;
	TPlane *p = obj->plane;
	for (WORD i=0; i<obj->numface; i++) {
		PLA_CalcABCD(*p,v[f->a],v[f->b],v[f->c]);
		f->i = (TTexture *) col;
		p++;
		f++;
	}
	BOUND_CalcFromVertex(obj->bound,v,obj->numvtx);

  return obj;
}

TObject* CA3D_A3D::GetRGBWorldObject (char *objname, DWORD r, DWORD g, DWORD b)
{ return GetRGBWorldObject(GetObjNum(objname),r,g,b); }

TObject* CA3D_A3D::GetWorldObject (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object %d",n);
    return 0;
  }
	TObject *obj = &object[n];
	TVertex *v = obj->vtx;
	TVertex vaux;
	for (WORD i=0; i<obj->numvtx; i++) {
		MTX_Mul(vaux,v[i],obj->trmtx);
		v[i] = vaux;
	}
	TFace *f = obj->face;
	TPlane *p = obj->plane;
	for (WORD i=0; i<obj->numface; i++) {
		PLA_CalcABCD(*p,v[f->a],v[f->b],v[f->c]);		
		p++; f++;
	}
	BOUND_CalcFromVertex(obj->bound,v,obj->numvtx);

  return obj;
}

TObject* CA3D_A3D::GetWorldObject (char *objname)
{ return GetWorldObject(GetObjNum(objname)); }

DWORD CA3D_A3D::GetObjFlags (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object flags. Object: %d",n);
    return 0;
  }
  return object[n].flags;
}

DWORD CA3D_A3D::GetObjFlags (char *objname)
{ return GetObjFlags(GetObjNum(objname)); }

DWORD CA3D_A3D::GetObjColor (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object color. Object: %d",n);
    return 0;
  }
  return object[n].color;
}

DWORD CA3D_A3D::GetObjColor (char *objname)
{ return GetObjColor(GetObjNum(objname)); }

TVertex* CA3D_A3D::GetObjVertex(WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object vertexs. Object: %d",n);
    return NULL;
  }

  count = object[n].numvtx;
  return object[n].vtx;
}

TVertex* CA3D_A3D::GetObjVertex(char *objname)
{ return GetObjVertex(GetObjNum(objname)); }

TFace* CA3D_A3D::GetObjFace(WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object faces. Object: %d",n);
    return NULL;
  }

  count = object[n].numface;
  return object[n].face;
}

TFace* CA3D_A3D::GetObjFace(char *objname)
{ return GetObjFace(GetObjNum(objname)); }

TPlane* CA3D_A3D::GetObjPlane (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object planes. Object: %d",n);
    return NULL;
  }

  count = object[n].numface;
  return object[n].plane;
}

TPlane* CA3D_A3D::GetObjPlane (char *objname)
{ return GetObjPlane(GetObjNum(objname)); }

TVertex* CA3D_A3D::GetObjNormal (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object normals. Object: %d",n);
    return NULL;
  }

  count = object[n].numvtx;
  return object[n].nrm;
}

TVertex* CA3D_A3D::GetObjNormal (char *objname)
{ return GetObjNormal(GetObjNum(objname)); }

TInfo* CA3D_A3D::GetObjInfo(WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object info. Object: %d",n);
    return NULL;
  }

	count = object[n].numinf; 
  return object[n].inf;
}

TInfo* CA3D_A3D::GetObjInfo(char *objname)
{ return GetObjInfo(GetObjNum(objname)); }

TBound CA3D_A3D::GetObjBound (WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object bounding box. Object: %d",n);
  }

  return object[n].bound;
}

TBound CA3D_A3D::GetObjBound (char *objname)
{ return GetObjBound(GetObjNum(objname)); }

TSpline* CA3D_A3D::GetObjSplinePos(WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object position spline. Object: %d",n);
    return NULL;
  }

  count = 0;
	if (object[n].pos!=NULL) count = object[n].pos->numKeys;
  return object[n].pos;
}

TSpline* CA3D_A3D::GetObjSplinePos(char *objname)
{ return GetObjSplinePos(GetObjNum(objname)); }

TSpline* CA3D_A3D::GetObjSplineRot(WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object rotation spline. Object: %d",n);
    return NULL;
  }

  count = 0;
	if (object[n].rot!=NULL) count = object[n].rot->numKeys;
  return object[n].rot;
}

TSpline* CA3D_A3D::GetObjSplineRot(char *objname)
{ return GetObjSplineRot(GetObjNum(objname)); }

TSpline* CA3D_A3D::GetObjSplineSca(WORD n)
{
  if (n >= numObjs) {
    ERR("Could not get object scale spline. Object: %d",n);
    return NULL;
  }

  count = 0;
	if (object[n].sca!=NULL) count = object[n].sca->numKeys;
  return object[n].sca;
}

TSpline* CA3D_A3D::GetObjSplineSca(char *objname)
{ return GetObjSplineSca(GetObjNum(objname)); }

TVertex* CA3D_A3D::GetObjVertexCopy(WORD n)
{
  TVertex *v = GetObjVertex (n);
  TVertex *s = (TVertex *) GetMem (sizeof(TVertex)*count);
  memcpy(v,s,(sizeof(TVertex)*count)>>2);

  count = object[n].numvtx;
  return s;
}

TVertex* CA3D_A3D::GetObjVertexCopy (char *objname)
{ return GetObjVertexCopy (GetObjNum(objname)); }

TFace* CA3D_A3D::GetObjFaceCopy(WORD n)
{
  TFace *f = GetObjFace(n);
  TFace *s = (TFace *) GetMem(sizeof(TFace)*count);
  memcpy(f,s,(sizeof(TFace)*count)>>2);

  count = object[n].numface;
  return s;
}

TFace* CA3D_A3D::GetObjFaceCopy (char *objname)
{ return GetObjFaceCopy (GetObjNum(objname)); }

TPlane* CA3D_A3D::GetObjPlaneCopy(WORD n)
{
  TPlane *p = GetObjPlane(n);
  TPlane *s = (TPlane *) GetMem(sizeof(TPlane)*count);
  memcpy(p,s,(sizeof(TPlane)*count)>>2);

  count = object[n].numface;
  return s;
}

TPlane* CA3D_A3D::GetObjPlaneCopy (char *objname)
{ return GetObjPlaneCopy (GetObjNum(objname)); }

TVertex* CA3D_A3D::GetObjNormalCopy(WORD n)
{
  TVertex *v = GetObjNormal(n);
  TVertex *s = (TVertex *) GetMem(sizeof(TVertex)*count);
  memcpy(v,s,(sizeof(TVertex)*count)>>2);

  count = object[n].numvtx;
  return s;
}

TVertex* CA3D_A3D::GetObjNormalCopy (char *objname)
{ return GetObjNormalCopy (GetObjNum(objname)); }

TInfo* CA3D_A3D::GetObjInfoCopy(WORD n)
{
  TInfo *i = GetObjInfo(n);
  TInfo *s = (TInfo *) GetMem(sizeof(TInfo)*count);
  memcpy(i,s,(sizeof(TInfo)*count)>>2);

  return s;
}

TInfo* CA3D_A3D::GetObjInfoCopy (char *objname)
{ return GetObjInfoCopy (GetObjNum(objname)); }

char* CA3D_A3D::GetObjIniStr(WORD n, char *key, char *def)
{
  if (n >= numObjs) {
		ERR("Could not get object ini value. Object: %d   String: %s",n,key);
		return NULL;
	}
  TObject *obj = &object[n];

  TTraverse t;
  static char value[128];
  char keyaux[128];

  LIST_GetTraverse(t,obj->prop);
  int i=obj->prop.count;
  while (i>0) {
    char *prop = (char *) LIST_Get(t);
    int j=0;
    while(prop[j] != '=') {
      keyaux[j]=prop[j];
      j++;
    }
    keyaux[j]=0;
    int k=0; j++;
    while(prop[j] != 0) value[k++]=prop[j++];
    value[k]=0;
    if (!stricmp(key,keyaux)) return value;
    LIST_Next(t);
    i--;
  }

  return def;
}

char* CA3D_A3D::GetObjIniStr(char *objname, char *key, char *def)
{ return  GetObjIniStr(GetObjNum(objname),key,def); }

int CA3D_A3D::GetObjIniInt(WORD n, char *key, int def)
{
  char strdefault[128];

  return atoi(GetObjIniStr(n,key,itoa(def,strdefault,10)));
}

int CA3D_A3D::GetObjIniInt(char *objname, char *key, int def)
{ return GetObjIniInt(GetObjNum(objname),key,def); }

float CA3D_A3D::GetObjIniFloat(WORD n, char *key, float def)
{
  char strdefault[128];

  sprintf(strdefault,"%f",def);
  return (float) atof(GetObjIniStr(n,key,strdefault));
}

float CA3D_A3D::GetObjIniFloat(char *objname, char *key, float def)
{ return  GetObjIniFloat(GetObjNum(objname),key,def); }

bool CA3D_A3D::GetObjIniBool(WORD n, char *key, bool def)
{
  char strdefault[128];

  return atoi(GetObjIniStr(n,key,itoa(def,strdefault,10)))!=0;
}

bool CA3D_A3D::GetObjIniBool(char *objname, char *key, bool def)
{ return  GetObjIniBool(GetObjNum(objname),key,def); }

TVertex CA3D_A3D::GetObjPosFrame (WORD n, float frame, DWORD flags)
{
	TVertex v;
	VTX_Set(v,0.0,0.0,0.0);

  if (n >= numObjs) {
		ERR("Could not get object position (frame). Object: %d",n);
		return v;
	}

	SPLINE_GetVertexFrameValues(v, object[n].pos,frame,flags);

	return v;
}

TVertex CA3D_A3D::GetObjPosFrame (char *objname, float frame, DWORD flags)
{ return GetObjPosFrame (GetObjNum(objname),frame,flags); }

TVertex CA3D_A3D::GetObjScaFrame (WORD n, float frame, DWORD flags)
{
	TVertex s;
	VTX_Set(s,1.0,1.0,1.0);

  if (n >= numObjs) {
		ERR("Could not get object scale (frame). Object: %d",n);
		return s;
	}

	SPLINE_GetVertexFrameValues(s, object[n].sca,frame,flags);

	return s;
}

TVertex CA3D_A3D::GetObjScaFrame (char *objname, float frame, DWORD flags)
{ return GetObjScaFrame (GetObjNum(objname),frame,flags); }

// This is a private function
void CA3D_A3D::GetSampledTM(TMatrix &m,WORD n,float frame)
{
	TObject *obj = &object[n];

  WORD iframe = (WORD)frame;
  frame -= (float) iframe;
	iframe = iframe%obj->anim->sceneframes;
	frame += (float) iframe;
  int i = 0, j;
	if (frame>obj->anim->pos[obj->anim->frames-1].frame) {
		i = obj->anim->frames-2;
		frame = obj->anim->pos[i+1].frame;
	} else {
		while ((frame>obj->anim->pos[i].frame) &&
					 (obj->anim->pos[i+1].frame<frame)) i++;
	}
	j = i + 1;

	TVertex pos1 = obj->anim->pos[i].p;
	TVertex pos2 = obj->anim->pos[j].p;
	TQuat rot1 = obj->anim->rot[i].q;
	TQuat rot2 = obj->anim->rot[j].q;
	TVertex sca1 = obj->anim->sca[i].p;
	TVertex sca2 = obj->anim->sca[j].p;

	float t = (frame - obj->anim->pos[i].frame)/
	          (obj->anim->pos[j].frame - obj->anim->pos[i].frame);

	TQuat newrot;
	TVertex newpos, newsca;
	VTX_Between (newpos,pos1,pos2,t);
	VTX_Between (newsca,sca1,sca2,t);

	QUAT_Slerp(rot1, rot2, t);
	newrot = rot1;

	//if (n==0 && t==0.5f) LOG("frame %f: %f %f %f %f",frame,rot1.w,rot1.x,rot1.y,rot1.z);

	TMatrix mrot, msca;
	MTX_Scale (msca,newsca.x,newsca.y,newsca.z);
	QUAT_RotationMatrix(mrot,newrot);
	mrot[0][3] = newpos.x; mrot[1][3] = newpos.y; mrot[2][3] = newpos.z;
	MTX_Mul(m,mrot,msca);
}

// This is a private function
void CA3D_A3D::GetObjectTM(TMatrix &m,WORD n,float frame,DWORD flags)
{
	if (object[n].flags & OBJ_SAMPLED) GetSampledTM(m,n,frame);
	else {
		MTX_Copy(m,object[n].objoffmtx);
		GetNodeTM(m,n,frame,flags);
	}
}

// This is a private function
void CA3D_A3D::GetNodeTM(TMatrix &m,WORD n,float frame,DWORD flags)
{
	TObject *obj;
	WORD node = n;
	TVertex vpos, vsca;
	TMatrix maux1, mrot, msca;
	MTX_Identity(msca);
	while (node != 0xffff) {
		obj = &object[node];
		if (!SPLINE_GetVertexFrameValues(vpos, obj->pos,frame,flags)) {
			vpos = obj->inipos;	
		}
		if (!SPLINE_GetQuaternionFrameValues(mrot,obj->rot,frame,flags)) {
			QUAT_RotationMatrix(mrot,obj->inirot);
		}
		if (!SPLINE_GetVertexFrameValues(vsca, obj->sca,frame,flags)) {
			vsca = obj->inisca;
		}
		mrot[0][3] = vpos.x; mrot[1][3] = vpos.y; mrot[2][3] = vpos.z;
		msca[0][0] = vsca.x; msca[1][1] = vsca.y; msca[2][2] = vsca.z;
		MTX_Mul(maux1,msca,m);
		MTX_Mul(m,mrot,maux1);
		node = obj->parent;
	}
}

void CA3D_A3D::GetObjFrame(TMatrix &m,WORD n,float frame,DWORD flags)
{
  if (n >= numObjs) {
		ERR("Could not get object matrix (frame). Object: %d",n);
		return;
	}

	if ((frame > 0.0) && (object[n].flags & OBJ_ANIMATED)) GetObjectTM(m,n,frame,flags);
	else MTX_Copy(m,object[n].trmtx);
}

void CA3D_A3D::GetObjFrame(TMatrix &m, char *objname, float frame, DWORD flags)
{ GetObjFrame(m,GetObjNum(objname),frame,flags); }


/////////////////////////////////////////////////////////////////
// Auxiliar functions
/////////////////////////////////////////////////////////////////

void CenterObject(TObject *obj)
{
	if (obj) 
  {
		float IncX,IncY,IncZ;
		float XMAX,YMAX,ZMAX,XMIN,YMIN,ZMIN;

		XMAX = -1000000.0; XMIN = 1000000.0;
		YMAX = -1000000.0; YMIN = 1000000.0;
		ZMAX = -1000000.0; ZMIN = 1000000.0;

		for (WORD i=0; i<obj->numvtx; i++) 
    {
			if (obj->vtx[i].x>XMAX) XMAX = obj->vtx[i].x;
			if (obj->vtx[i].x<XMIN) XMIN = obj->vtx[i].x;
			if (obj->vtx[i].y>YMAX) YMAX = obj->vtx[i].y;
			if (obj->vtx[i].y<YMIN) YMIN = obj->vtx[i].y;
			if (obj->vtx[i].z>ZMAX) ZMAX = obj->vtx[i].z;
			if (obj->vtx[i].z<ZMIN) ZMIN = obj->vtx[i].z;
		}

		if (XMAX > fabs (XMIN)) IncX = -((XMAX+XMIN)/2);
		else IncX = (float)fabs ((XMAX+XMIN) / 2);
		if (YMAX > fabs (YMIN)) IncY = -((YMAX+YMIN)/2);
		else IncY = (float)fabs ((YMAX+YMIN) / 2);
		if (ZMAX > fabs (ZMIN)) IncZ = -((ZMAX+ZMIN)/2);
		else IncZ = (float)fabs ((ZMAX+ZMIN) / 2);
		for (WORD i=0; i<obj->numvtx; i++) {
			obj->vtx[i].x += IncX;
			obj->vtx[i].y += IncY;
			obj->vtx[i].z += IncZ;
		}
	}
}


