// A3D - Anaconda 3D File System
// Anaconda 3D File System (Cameras)

#include <windows.h>
#include <a3d.h>

/////////////////////////////////////////////////////////////////
// Private functions                                           //
/////////////////////////////////////////////////////////////////

void CA3D_A3D::ReadCamera()
{
  int n = numCams;
	camera = (TA3DCamera *) realloc(camera,(n+1)*sizeof(TA3DCamera));

  TA3DCamera *cam = &camera[n];
	memset(cam,0,sizeof(TA3DCamera));

  ReadName(cam->camname);

	SEC_START("Camera: %s", cam->camname);

  TChunk chunk;
  bool mainChunk=FALSE;
  ReadChunk(chunk);
  while (!FILE_Eof(f) && !mainChunk)
  {
    switch(chunk.id)
    {
      case    CHUNK_POSITION: FILE_Read(&cam->camstpos,sizeof(float),3,f);  break;
      case        CHUNK_ROLL: FILE_Read(&cam->camstroll,sizeof(float),1,f); break;
      case  CHUNK_SPLINE_POS: cam->campos = ReadPosSpline();                break;
      case CHUNK_SPLINE_ROLL: cam->camroll = ReadRollSpline();              break;
      case      CHUNK_TARGET: ReadTarget();                                 break;
                  default: mainChunk = AnalizeChunk(chunk);              break;
    }
	  if (!mainChunk) ReadChunk(chunk);
  }
  if (cam->campos || cam->camroll) {
		cam->camflags |= OBJ_ANIMATED;
		LOG("Animated: yes");
	} else LOG("Animated: no");

  numCams++;

  if (!FILE_Eof(f)) FILE_Seek(f,-6,SEEK_CUR);

	SEC_END();
}

void CA3D_A3D::ReadTarget()
{
  int n = numCams;

  TA3DCamera *cam = &camera[n];
  ReadName(cam->tarname);

	SEC_START("Target: %s", cam->tarname);

  TChunk chunk;
  bool mainChunk=FALSE;
  ReadChunk(chunk);
  while (!FILE_Eof(f) && !mainChunk)
  {
    switch(chunk.id)
    {
      case   CHUNK_POSITION: FILE_Read(&cam->tarstpos,sizeof(float),3,f); break;
      case CHUNK_SPLINE_POS: cam->tarpos = ReadPosSpline();               break;
                  default: mainChunk = AnalizeChunk(chunk);             break;
    }
	  if (!mainChunk) ReadChunk(chunk);
  }
  if (cam->tarpos) {
		cam->tarflags |= OBJ_ANIMATED;
		LOG("Animated: yes");
	} else LOG("Animated: no");

  if (!FILE_Eof(f)) FILE_Seek(f,-6,SEEK_CUR);

	SEC_END();
}


/////////////////////////////////////////////////////////////////
// Public functions                                            //
/////////////////////////////////////////////////////////////////

WORD CA3D_A3D::GetCamCount()
{ return numCams; }

WORD CA3D_A3D::GetCamNum(char *camname)
{
  if (!numCams) return 0xffff;

  for(WORD i=0; i<numCams; i++)
    if (!stricmp(camera[i].camname,camname)) break;

  return i;
}

char* CA3D_A3D::GetCamName (WORD n)
{
  if (n >= numCams) {
    ERR("Could not get camera name (no cameras). Camera: %d",n);
    return NULL;
  }

  return camera[n].camname;
}

DWORD CA3D_A3D::GetCamFlags(WORD n)
{
  if (n >= numCams) {
    ERR("Could not get camera flags. Camera: %d",n);
    return 0;
  }
  return camera[n].camflags;
}

DWORD CA3D_A3D::GetCamFlags(char *camname)
{ return GetCamFlags(GetCamNum(camname)); }

TVertex CA3D_A3D::GetCamStPos(WORD n)
{
  if (n >= numCams) {
    ERR("Could not get camera static position. Camera: %d",n);

		TVertex v;
		VTX_Set(v,0.0f,0.0f,0.0f);
		return v;
  }

  return camera[n].camstpos;
}

TVertex CA3D_A3D::GetCamStPos(char *camname)
{ return GetCamStPos(GetCamNum(camname)); }

float CA3D_A3D::GetCamStRoll(WORD n)
{
  if (n >= numCams) {
    ERR("Could not get camera static roll. Camera: %d",n);
		return 0.0f;
  }
	
  return camera[n].camstroll;
}

float CA3D_A3D::GetCamStRoll(char *camname)
{ return GetCamStRoll(GetCamNum(camname)); }

TVertex CA3D_A3D::GetCamPosFrame  (WORD n, float frame, DWORD flags)
{
  TVertex campos;
	campos = camera[n].camstpos;
  if (n >= numCams) {
		ERR("Could not get camera position (frame). Camera: %d",n);
		return campos;
	}

  SPLINE_GetVertexFrameValues(campos,camera[n].campos,frame,flags);

	return campos;
}

TVertex CA3D_A3D::GetCamPosFrame(char *camname, float frame, DWORD flags)
{ return GetCamPosFrame(GetCamNum(camname),frame,flags); }

float CA3D_A3D::GetCamRollFrame (WORD n, float frame, DWORD flags)
{
  float roll;
	roll = camera[n].camstroll;
  if (n >= numCams) {
		ERR("Could not get camera roll (frame). Camera: %d",n);
		return roll;
	}

	SPLINE_GetRealFrameValues(roll,camera[n].camroll,frame,flags);

	return roll;
}

float CA3D_A3D::GetCamRollFrame (char *camname, float frame, DWORD flags)
{ return GetCamRollFrame (GetCamNum(camname),frame,flags); }

void CA3D_A3D::GetCamFrame(TMatrix &m, WORD n, float frame, DWORD flags)
{
  if (n >= numCams) {
		ERR("Could not get camera (frame). Camera: %d",n);
		return;
	}

  float roll;
  TVertex campos,tarpos;

	if ((frame > 0.0) && (camera[n].camflags & OBJ_ANIMATED)) {
    if (!SPLINE_GetRealFrameValues(roll,camera[n].camroll,frame,flags)) roll = camera[n].camstroll;
    if (!SPLINE_GetVertexFrameValues(campos,camera[n].campos,frame,flags)) campos = camera[n].camstpos;
  } else {
    campos = camera[n].camstpos;
    roll = camera[n].camstroll;
  }

  if ((frame > 0.0) && (camera[n].tarflags & OBJ_ANIMATED)) {
    if (!SPLINE_GetVertexFrameValues(tarpos,camera[n].tarpos,frame,flags)) tarpos = camera[n].tarstpos;
  } else {
		tarpos = camera[n].tarstpos;
  }

  MTX_LookAt(m,campos.x, campos.y, campos.z,tarpos.x,tarpos.y,tarpos.z,roll*M_ToGrd);
}

void CA3D_A3D::GetCamFrame(TMatrix &m, char *camname, float frame, DWORD flags)
{ GetCamFrame(m,GetCamNum(camname),frame,flags); }

char* CA3D_A3D::GetTarName (WORD n)
{
  if (n >= numCams) {
    ERR("Could not get target name (no cameras). Camera: %d",n);
    return NULL;
  }
  return camera[n].tarname;
}

DWORD CA3D_A3D::GetTarFlags(WORD n)
{
  if (n >= numCams) {
    ERR("Could not get target flags. Camera: %d",n);
    return 0;
  }
  return camera[n].tarflags;
}

DWORD CA3D_A3D::GetTarFlags(char *camname)
{ return GetTarFlags(GetCamNum(camname)); }

TVertex CA3D_A3D::GetTarStPos(WORD n)
{
	if (n >= numCams) {
		ERR("Could not get target static position. Camera: %d",n);

		TVertex v;
		VTX_Set(v,0.0f,0.0f,0.0f);
		return v;
  }

  return camera[n].tarstpos;
}

TVertex CA3D_A3D::GetTarStPos(char *camname)
{ return GetTarStPos(GetCamNum(camname)); }

TVertex CA3D_A3D::GetTarPosFrame(WORD n, float frame, DWORD flags)
{
  TVertex tarpos;
	tarpos = camera[n].tarstpos;
  if (n >= numCams) {
		ERR("Could not get target position (frame). Camera: %d",n);
		return tarpos;
	}

  SPLINE_GetVertexFrameValues(tarpos,camera[n].tarpos,frame,flags);

	return tarpos;
}

TVertex CA3D_A3D::GetTarPosFrame(char *camname, float frame, DWORD flags)
{ return GetTarPosFrame(GetCamNum(camname),frame,flags); }


