#ifndef EFFECT_JOINALPHA
#define EFFECT_JOINALPHA

class effectJoinAlpha
{
private:  
  DWORD Size;

public:  
  DWORD *Src1Buffer;  
  DWORD *Src2Buffer;  
  DWORD *DstBuffer;
  BYTE *AlphaBuffer;
  
  effectJoinAlpha();    
  ~effectJoinAlpha();    

  void setDimension (DWORD width,DWORD height);
  
  void makeEffect();  
};

extern effectJoinAlpha *effJoinAlpha;

#endif