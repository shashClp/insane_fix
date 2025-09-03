// AW3D - Anaconda 3D Windows Library
// Transform Buffer

#ifndef AW3D_TRANSFORMATION_BUFFER
#define AW3D_TRANSFORMATION_BUFFER

#include <defines.h>
#include <log.h>

class TAW3D_TransformationBuffer
{
private:
  DWORD *bits;
  float *buffer;
  DWORD entrysize; 

public:

#ifdef _DEBUG
  DWORD entries;  
#endif

  TAW3D_TransformationBuffer(DWORD entries,DWORD entrysize);
  ~TAW3D_TransformationBuffer();  

  void Prepare(DWORD entries);

inline float *TAW3D_TransformationBuffer::GetBuffer(DWORD entry)
{
#ifdef _DEBUG
  if (this->entries<entry)
  {
    ERR ("Prepare more than max entries in AW3D_TransformationBuffer");
    return 0;
  }
#endif
  return buffer+(entry*entrysize);
}

inline bool TAW3D_TransformationBuffer::Get(DWORD entry)
{
#ifdef _DEBUG
  if (this->entries<entry)
  {
    ERR ("Get out of entries in AW3D_TransformationBuffer");
    return false;
  }
#endif    
  return (bits[entry>>5] & (1L << (entry & 0x3f)))!=0;
}

inline void TAW3D_TransformationBuffer::Set(DWORD entry)
{
#ifdef _DEBUG
  if (this->entries<entry)
  {
    ERR ("Set out of entries in AW3D_TransformationBuffer");
    return;
  }
#endif
  bits[entry>>5]|=1L << (entry & 0x3f);  
}
};

#endif
