// A3D - Anaconda 3D File System
// Anaconda 3D File System (Lights)

#include <windows.h>
#include <a3d.h>

/////////////////////////////////////////////////////////////////
// Private functions                                           //
/////////////////////////////////////////////////////////////////

void CA3D_A3D::ReadLight()
{
  int n = numLights;
	light = (TA3DLight *) realloc(light,(n+1)*sizeof(TA3DLight));

  TA3DLight *l = &light[n];
	memset(l,0,sizeof(TA3DLight));

  ReadName(l->name);

	SEC_START("Light: %s", l->name);

  char prop[128];
  int c = FILE_Getc(f);
  while (c != 0)
  {
    FILE_Seek(f,-1,SEEK_CUR);
    ReadName(prop);
    LIST_Add(l->prop,prop,strlen(prop)+1);
    c = FILE_Getc(f);
  }

  TChunk chunk;
  bool mainChunk=FALSE;
  ReadChunk(chunk);
  while (!FILE_Eof(f) && !mainChunk)
  {
    switch(chunk.id)
    {
			case      CHUNK_COLOR: FILE_Read(&l->color,sizeof(DWORD),1,f); break;
      case   CHUNK_POSITION: FILE_Read(&l->stpos,sizeof(float),3,f); break;
      case CHUNK_SPLINE_POS: l->pos = ReadPosSpline();               break;
                default: mainChunk = AnalizeChunk(chunk);          break;
    }
	  if (!mainChunk) ReadChunk(chunk);
  }
  if (l->pos) {
		l->flags |= OBJ_ANIMATED;
		LOG("Animated: yes");
	} else LOG("Animated: no");

  numLights++;

  if (!FILE_Eof(f)) FILE_Seek(f,-6,SEEK_CUR);

	SEC_END();
}


/////////////////////////////////////////////////////////////////
// Public functions                                            //
/////////////////////////////////////////////////////////////////

WORD CA3D_A3D::GetLightCount()
{
  count = numLights;
  return numLights;
}

WORD CA3D_A3D::GetLightNum(char *lightname)
{
  if (!numLights) return 0xffff;

  for(WORD i=0; i<numLights; i++)
    if (!stricmp(light[i].name,lightname)) break;

  return i;
}

char* CA3D_A3D::GetLightName(WORD n)
{
  if (n >= numLights) {
		ERR("Could not get light name (no lights). Light: %d",n);
		return NULL;
	}
  return light[n].name;
}

DWORD CA3D_A3D::GetLightFlags(WORD n)
{
  if (n >= numLights) {
		ERR("Could not get light flags. Light: %d",n);
		return 0xffffffff;
	}
  return light[n].flags;
}

DWORD CA3D_A3D::GetLightFlags(char *lightname)
{ return GetLightFlags(GetLightNum(lightname)); }

TVertex CA3D_A3D::GetLightStPos(WORD n)
{
  TVertex v;
	VTX_Set(v,0.0f,0.0f,0.0f);
  if (n >= numLights) {
		ERR("Could not get light name (no lights). Light: %d",n);
		return v;
	}
  return light[n].stpos;
}

TVertex CA3D_A3D::GetLightStPos(char *lightname)
{ return GetLightStPos(GetLightNum(lightname)); }

DWORD CA3D_A3D::GetLightColor(WORD n)
{ return light[n].color; }

DWORD CA3D_A3D::GetLightColor(char *lightname)
{ return GetLightColor(GetLightNum(lightname)); }

char* CA3D_A3D::GetLightIniStr(WORD n, char *key, char *def)
{
  if (n >= numLights) {
		ERR("Could not get light ini value. Light: %d   String: %s",n,key);
		return NULL;
	}
  TA3DLight *light = &this->light[n];

  TTraverse t;
  static char value[128];
  char keyaux[128];

  LIST_GetTraverse(t,light->prop);
  int i=light->prop.count;
  while (i>0)
  {
    char *prop = (char *) LIST_Get(t);
    int j=0;
    while(prop[j] != '=')
    {
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

char* CA3D_A3D::GetLightIniStr(char *lightname, char *key, char *def)
{ return  GetLightIniStr(GetLightNum(lightname),key,def); }

int CA3D_A3D::GetLightIniInt(WORD n, char *key, int def)
{
  char strdefault[128];

  return atoi(GetLightIniStr(n,key,itoa(def,strdefault,10)));
}

int CA3D_A3D::GetLightIniInt(char *lightname, char *key, int def)
{ return GetLightIniInt(GetLightNum(lightname),key,def); }

float CA3D_A3D::GetLightIniFloat(WORD n, char *key, float def)
{
  char strdefault[128];

  sprintf(strdefault,"%f",def);
  return (float) atof(GetLightIniStr(n,key,strdefault));
}

float CA3D_A3D::GetLightIniFloat(char *lightname, char *key, float def)
{ return  GetLightIniFloat(GetLightNum(lightname),key,def); }

bool CA3D_A3D::GetLightIniBool(WORD n, char *key, bool def)
{
  char strdefault[128];

  return atoi(GetLightIniStr(n,key,itoa(def,strdefault,10)))!=0;
}

bool CA3D_A3D::GetLightIniBool(char *lightname, char *key, bool def)
{ return  GetLightIniBool(GetLightNum(lightname),key,def); }

TVertex CA3D_A3D::GetLightFrame(WORD n, float frame, DWORD flags)
{
  TVertex v;
  if (n >= numLights) {
		ERR("Could not get light (frame). Light: %d",n);

		VTX_Set(v,0.0f,0.0f,0.0f);
		return v;
	}

  if ((frame > 0.0) && (light[n].flags & OBJ_ANIMATED)) {
    SPLINE_GetVertexFrameValues(v,light[n].pos,frame,flags);
		return v;
  } else {
    return light[n].stpos;
  }
}

TVertex CA3D_A3D::GetLightFrame(char *lightname, float frame, DWORD flags)
{ return GetLightFrame(GetLightNum(lightname),frame,flags); }




