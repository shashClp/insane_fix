#include <windows.h>
#include <joinalpha.h>
#include <rgb.h>
#include <syswin.h>

effectJoinAlpha *effJoinAlpha;

effectJoinAlpha::effectJoinAlpha()
{
}

effectJoinAlpha::~effectJoinAlpha()
{
}

void effectJoinAlpha::setDimension (DWORD width,DWORD height)
{
  Size = width*height;
}
  
void effectJoinAlpha::makeEffect()
{    
  DWORD *s1 = Src1Buffer;
  DWORD *s2 = Src2Buffer;
  DWORD *d = DstBuffer;
  BYTE *alpha = AlphaBuffer;

  if (MMX)
  {
    DWORD cops=Size;
    static DWORD tmp_alpha[2];    
    tmp_alpha[0] = 0;
    tmp_alpha[1] = 0;

    __asm {
      pushad
      mov ecx,cops
      mov esi,[s1]
      mov ebx,[s2]
      mov edx,[alpha]
      mov edi,[d]
      xor ebp,ebp      
      pxor mm0,mm0
    Buc:        
        mov al,[edx+ebp]       
        mov BYTE PTR [tmp_alpha],al
        mov BYTE PTR [tmp_alpha+2],al
        mov BYTE PTR [tmp_alpha+4],al
      
        movd mm1,DWORD PTR [esi+ebp*4]
        movd mm2,DWORD PTR [ebx+ebp*4]        
        punpcklbw mm1,mm0
        punpcklbw mm2,mm0
        psubusw mm1,mm2
        movq mm3,[tmp_alpha]
        
        pmullw mm1,mm3
        
        psrlw mm1,8
        paddusw mm1,mm2
        packuswb mm1,mm0

        movd [edi+ebp*4],mm1        

        inc ebp
        dec ecx
      jnz Buc
      emms
      popad
    }
    
  } else {
    for (DWORD i=0; i<Size; i++)
    {
      RGB_Set (d[i],(((((long)Get_R(s1[i]))-Get_R(s2[i]))*alpha[i]) / 256)+Get_R(s2[i]),
                    (((((long)Get_G(s1[i]))-Get_G(s2[i]))*alpha[i]) / 256)+Get_G(s2[i]),
                    (((((long)Get_B(s1[i]))-Get_B(s2[i]))*alpha[i]) / 256)+Get_B(s2[i]));
    } 
  }
}
