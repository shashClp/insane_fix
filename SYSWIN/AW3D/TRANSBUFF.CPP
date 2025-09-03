// AW3D - Anaconda 3D Windows Library
// Transform Buffer

#include <windows.h>
#include <log.h>
#include <mem.h>
#include <transbuff.h>
#include <defines.h>

TAW3D_TransformationBuffer::TAW3D_TransformationBuffer(DWORD entries,DWORD entrysize)
{
  SEC_START ("Initializing AW3D_TransformationBuffer");

  LOG ("Entries: %i (%i)",entries,entrysize);

#ifdef _DEBUG
  this->entries =  entries;
#endif
  this->entrysize = entrysize;
  
  bits = (DWORD *) GetMem ((entries>>3)+1);
  buffer = (float *) GetMem (4*entries*entrysize);

  Prepare(entries);

  SEC_END ();
}

TAW3D_TransformationBuffer::~TAW3D_TransformationBuffer()
{
  LOG ("Freeing AW3D_TransformationBuffer");
  if (buffer) 
  {
    FreeMem (buffer);
    buffer = NULL;
  }
  if (bits) 
  {
    FreeMem (bits);
    bits = NULL;
  }
}

void TAW3D_TransformationBuffer::Prepare(DWORD entries)
{
#ifdef _DEBUG
  if (this->entries<entries)
  {
    ERR ("Prepare more than max entries in AW3D_TransformationBuffer");
    return;
  }
#endif

  memset (bits,0,(entries>>3)+1);
}
