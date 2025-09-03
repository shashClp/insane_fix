// AWL - Anaconda Windows Library
// Miscelaneous routines

#include <ddraw.h>
#include <log.h>
#include <string.h>

HRESULT hr;

void MISC_MemSetFloat (void *dst,float value,DWORD count)
{
  _asm {
    mov edi,dst
    mov eax,value
    mov ecx,count
    rep stosd
  }    
}

void MISC_MemCpy4 (void *dst,void *src,DWORD count)
{
  _asm {
    mov esi,src
    mov edi,dst    
    mov ecx,count
    rep movsd
  }    
}

void MISC_MemSet4 (void *dst,DWORD value,DWORD count)
{
  _asm {
    mov edi,dst
    mov eax,value
    mov ecx,count
    rep stosd
  }    
}

char *MISC_GetFileExtension(char *Path)
{
  if (!Path)
  {
    ERR ("Get file extension from empty Path?");
    return NULL;
  }

  static char ret[5];
	int i=strlen(Path)-1;
  int j=4;
  ret[4]=0;  
	while ((i>0) && (ret[j]!='.'))
  {
	  ret[--j] = Path[i--];
	}
  if (ret[j]!='.')
  {
    ERR ("Not . found in MISC_GetFileExtension");
    return NULL;
  }   
  return ret+j+1;	
}

char *MISC_Guid2String (LPGUID Guid)
{
  if (Guid==NULL)
  {
    ERR ("Guid2String can't convert a NULL GUID");
    return NULL;
  }

  static char c[256];
  sprintf (c,"%8x.%4x.%4x.%2x.%2x.%2x.%2x.%2x.%2x.%2x.%2x",Guid->Data1,
            Guid->Data2,Guid->Data3,Guid->Data4[0],Guid->Data4[1],
            Guid->Data4[2],Guid->Data4[3],Guid->Data4[4],
            Guid->Data4[5],Guid->Data4[6],Guid->Data4[7]);
  return c;
}

char *MISC_DXError (HRESULT hr)
{
  switch (hr)
  {
    case DD_OK: return "DD_OK";
    case DDERR_GENERIC: return "DDERR_GENERIC";
    case DDERR_INVALIDCLIPLIST: return "DDERR_INVALIDCLIPLIST";
    case DDERR_INVALIDOBJECT: return "DDERR_INVALIDOBJECT";
    case DDERR_INVALIDPARAMS: return "DDERR_INVALIDPARAMS";
    case DDERR_INVALIDRECT: return "DDERR_INVALIDRECT";
    case DDERR_NOALPHAHW: return "DDERR_NOALPHAHW";
    case DDERR_NOBLTHW: return "DDERR_NOBLTHW";
    case DDERR_NOCLIPLIST: return "DDERR_NOCLIPLIST";
    case DDERR_NODDROPSHW: return "DDERR_NODDROPSHW";
    case DDERR_NOMIRRORHW: return "DDERR_NOMIRRORHW";
    case DDERR_NORASTEROPHW: return "DDERR_NORASTEROPHW";
    case DDERR_NOROTATIONHW: return "DDERR_NOROTATIONHW";
    case DDERR_NOSTRETCHHW: return "DDERR_NOSTRETCHHW";
    case DDERR_NOZBUFFERHW: return "DDERR_NOZBUFFERHW";
    case DDERR_SURFACEBUSY: return "DDERR_SURFACEBUSY";
    case DDERR_SURFACELOST: return "DDERR_SURFACELOST";
    case DDERR_UNSUPPORTED: return "DDERR_UNSUPPORTED";
    case DDERR_WASSTILLDRAWING: return "DDERR_WASSTILLDRAWING";
  } 
  return "Undefined";
}

