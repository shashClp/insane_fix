// AWL - Anaconda Windows Library
// Multiple View library

#include <ddraw.h>
#include <view.h>
#include <log.h>
#include <mem.h>
#include <video.h>
#include <view.h>
#include <misc.h>
#include <defines.h>
#include <syswin.h>

DWORD GBL_Size;
DWORD GBL_Size4;
DWORD GBL_Width;
DWORD GBL_Height;
DWORD GBL_Pitch;
DWORD GBL_Pitch4;
DWORD GBL_CenterX;
DWORD GBL_CenterY;
DWORD *GBL_Bits;

TView *VIEW_Init (DWORD width,DWORD height,DWORD cx,DWORD cy,DWORD pitch,DWORD *bits)
{
  SEC_START ("Initializing a View!");
  TView *view = (TView *) GetMem (sizeof(TView));
  if (view==NULL)
  {
    ERR ("Not enough memory to make a view");
    return NULL;
  }
  if (bits) 
  {
    view->bits = bits;
    view->flags = VIEW_SHAREDMEM;
    LOG ("Shared Mem");
  } else {
    view->bits = (DWORD *) GetMem (width*height*4);
    if (view->bits==NULL)
    {
      ERR ("Not enough memory to get a buffer bits");
      FreeMem(view); 
      return NULL;
    }
    view->flags = VIEW_FIRSTGETMEM;
    LOG ("First GetMem");
  }
  view->width = width;
  view->height = height;
  view->pitch = pitch;
  view->centerx = cx;
  view->centery = cy;
  LOG ("Width: %i Height: %i Pointer: %p",width,height,view->bits);
  SEC_END();
  return view;  
}

void VIEW_Clear ()
{
  if (GBL_Bits==NULL)
  {
    ERR ("GBL_Bits is NULL and view can't clear");
    return;
  }
  DWORD *v = GBL_Bits;
  for (DWORD i=0; i<GBL_Height; i++)
  {
    memset (v,0,GBL_Width*4);
    v+=GBL_Pitch;
  }  
}

void FreeMem (TView *view)
{  
  LOG ("Freeing View bits buffer");
  if (view==NULL)
  {
    ERR ("FreeMem of a NULL view?");
    return; 
  }
  if TestBit(view->flags,VIEW_SHAREDMEM) return;  
  FreeMem (view->bits); 
  return;
}

void VIEW_Set (TView *view)
{
  if (view==NULL)
  {
    ERR ("Setting a NULL view?");
    return;  
  }
  GBL_Size = view->pitch*view->height;
  GBL_Size4 = GBL_Size*4;
  GBL_Width = view->width;
  GBL_Height = view->height;
  GBL_Pitch = view->pitch;
  GBL_Pitch4 = view->pitch*4;
  GBL_Bits = view->bits;
  GBL_CenterX = view->centerx;
  GBL_CenterY = view->centery;    
}

HICON VIEW_FlipMinimized()
{   
  // shash: Hack to force to 320x240
  DWORD VIDEO_ScreenWidth = 320;
  DWORD VIDEO_ScreenHeight = 240;

  DWORD bits[32][32];  
  ZeroMemory (bits,sizeof(bits));
	DWORD cx = 32;
	DWORD cy = (32*VIDEO_ScreenHeight)/VIDEO_ScreenWidth;
	DWORD dx = VIDEO_ScreenWidth/cx;
	DWORD dy = (VIDEO_ScreenHeight/cy)*GBL_Pitch;	  
  DWORD x=0;
  DWORD y=(cx-cy)/2;

  DWORD ddy = 0;
  for (DWORD j=0; j<cy; j++)
  {    
    DWORD ddx = ddy;
    for (DWORD i=0; i<cx; i++)
    {      
      bits[y+j][x+i] = GBL_Bits[ddx];
      ddx+=dx;
    }
    ddy+=dy;
  }		    
  
  return CreateIcon(HInst,32,32,1,32,(BYTE *)bits,(BYTE *)bits);
}

/*
void VIEW_Flip ()
{
  if (GBL_Bits==NULL)
  {
    ERR ("GBL_Bits is NULL and view can't flip");
    return;
  }

  DDSURFACEDESC2 ddsd;  
  ddsd.dwSize = sizeof(DDSURFACEDESC2);  
  hr = VIDEO_BackBuffer->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);    
  if (hr==DDERR_SURFACELOST) return;
  if (ERRDX ("VIDEO_BackBuffer->Lock, View flip!")) return;

  BYTE bpp = (BYTE)ddsd.ddpfPixelFormat.dwRGBBitCount;
  if (bpp==16) if (!TestBit(ddsd.ddpfPixelFormat.dwRBitMask,0x8000)) bpp=15;

  DWORD pitch = ddsd.lPitch;
  void *dst = ddsd.lpSurface;
  switch (bpp)
  {
    case 15:
      _asm {
        mov edi,dst
        mov esi,GBL_Bits
        mov edx,GBL_Height
        LoopY15:
          push esi
          push edi
          mov ecx,GBL_Width
          LoopX15:
            mov eax,[esi]
            mov ebx,[esi+2]
            shr ah,3            
            shl ebx,5+5-3            
            shr ax,3
            and ebx,111110000000000b
            and eax,1111111111b
            or ebx,eax
            add esi,4
            mov [edi],bx
            add edi,2            
            dec ecx
            jnz LoopX15
          pop edi
          pop esi
          add edi,pitch
          add esi,GBL_Pitch4
          dec edx
          jnz LoopY15
      }
    break;
    case 16:
      _asm {
        mov edi,dst
        mov esi,GBL_Bits
        mov edx,GBL_Height
        LoopY16:
          push esi
          push edi
          mov ecx,GBL_Width
          LoopX16:
            mov eax,[esi]
            mov ebx,[esi+2]
            shr ah,2
            shl ebx,5+6-3            
            shr ax,3
            and ebx,1111100000000000b
            and eax,11111111111b
            or ebx,eax
            add esi,4
            mov [edi],bx
            add edi,2            
            dec ecx
            jnz LoopX16
          pop edi
          pop esi
          add edi,pitch
          add esi,GBL_Pitch4
          dec edx
          jnz LoopY16
      }
    break;
    case 24:
      _asm {
        mov edi,dst
        mov esi,GBL_Bits
        mov edx,GBL_Height
        LoopY24:
          push esi
          push edi
          mov ecx,GBL_Width
          LoopX24:
            stosd
            dec edi
            dec ecx
            jnz LoopX24
          pop edi
          pop esi
          add edi,pitch
          add esi,GBL_Pitch4
          dec edx
          jnz LoopY24
      }
    break;
    case 32:      
      if (MMX)
      {
        DWORD count = GBL_Width/16;        
        DWORD width32 = GBL_Width*4;
        _asm {
          mov eax,GBL_Bits
          mov ebx,dst          
          mov edx,GBL_Height
          
          bucY:
            xor esi,esi
            mov ecx,count

            align 16
            bucX:              
              movq mm0,[eax+esi]
              movq mm1,[eax+esi+8]
              movq mm2,[eax+esi+16]
              movq mm3,[eax+esi+24]
              movq mm4,[eax+esi+32]
              movq mm5,[eax+esi+40]
              movq mm6,[eax+esi+48]
              movq mm7,[eax+esi+56]              

              movq [ebx+esi],mm0
              movq [ebx+esi+8],mm1
              movq [ebx+esi+16],mm2
              movq [ebx+esi+24],mm3
              movq [ebx+esi+32],mm4
              movq [ebx+esi+40],mm5
              movq [ebx+esi+48],mm6
              movq [ebx+esi+56],mm7

              add esi,64
              dec ecx
              jnz bucX
            add ebx,pitch
            add eax,width32
            dec edx
            jnz bucY
          emms
        }        
      } else {
        DWORD pitch32 = pitch/4;
        DWORD *dst32 = (DWORD *)dst;
        DWORD *src32 = GBL_Bits;
        DWORD width32 = GBL_Width*4;
        for (DWORD i=0; i<GBL_Height; i++)
        {
          memcpy (dst32,src32,width32);
          dst32+=pitch32;
          src32+=GBL_Pitch;
        }
      }
    break;
  }
  hr = VIDEO_BackBuffer->Unlock(NULL); 
  if (hr==DDERR_SURFACELOST) return;
  if (ERRDX ("VIDEO_BackBuffer->Unlock, View flip!")) return;
}
*/

