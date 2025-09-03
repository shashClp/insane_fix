#ifndef EFFECT_ROTOZOOM
#define EFFECT_ROTOZOOM

class effectRotoZoom
{
private:  
  DWORD Width;
  DWORD Height;

public:  
  DWORD *SrcBuffer;  
  DWORD *DstBuffer;
  
  effectRotoZoom();    
  ~effectRotoZoom();    

  void setDimension (DWORD width,DWORD height);
  
  void makeEffect1(float sizex,float sizey,float anglex,float angley);
  void makeEffect2(float sizex,float sizey,float anglex,float angley);
};

extern effectRotoZoom *effRotoZoom;

#endif