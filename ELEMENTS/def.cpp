#include <math.h>
#include <stdlib.h>
#include <view.h>

const float fPI = 3.1415f;
//long inc_frame =  25;
long SENO[256];
long COSENO[256];

void CalculaTablaSenos(float fAmplitud)
// 0<=fAmplitud<=255
{
	for(int i=0;i<256;i++)
	{
		float fAlpha = 2.0f * fPI * (float) i / 256.0f;
		SENO[i] = (long)(sin(fAlpha) * fAmplitud) + 128;
		COSENO[i] = (long)(cos(fAlpha) * fAmplitud) + 128;
	}
}



//====================================================================
//---Rutinas para la deformacion por mascara de niveles---------------

long CLIPTAB_X[1024];
long CLIPTAB_Y[1024];

void InitClipTab(int lim_inf,int lim_sup)
{
DWORD i;

	for(i=0;i<256;i++)
		CLIPTAB_X[i] = 0;		

    for(i=0;i<1024-256;i++)
		if(i<GBL_Width)
			CLIPTAB_X[i+256] = i;
		else
			CLIPTAB_X[i+256] = GBL_Width-1;


    for(i=0;i<256+lim_inf;i++)
		CLIPTAB_Y[i] = lim_inf*GBL_Width;

	for(i=lim_inf;i<1024-256;i++)
		if(i<lim_sup)
			CLIPTAB_Y[i+256] = i*GBL_Width;
		else
			CLIPTAB_Y[i+256] = (lim_sup-1)*GBL_Width;
}

void InitClipTabEspejo(int lim_inf,int lim_sup)
{
	for(long i=0;i<256;i++)
		CLIPTAB_X[i] = 256-i;

    for(long i=0;i<1024-256;i++)
		if(i<GBL_Width)
			CLIPTAB_X[i+256] = i;
		else
			CLIPTAB_X[i+256] = (GBL_Width<<1) -i -1;


    for(long i=0;i<256+lim_inf;i++)
		CLIPTAB_Y[i] = lim_inf*GBL_Width; //aqui aun no hay espejo

	for(long i=lim_inf;i<1024-256;i++)
		if(i<lim_sup)
			CLIPTAB_Y[i+256] = i*GBL_Width;
		else
        {
			CLIPTAB_Y[i+256] = ((lim_sup<<1) - i -1)*GBL_Width;
            if(CLIPTAB_Y[i+256]<0)
                CLIPTAB_Y[i+256] = 0; //si el alto es menor que 256, capar el overflow inferior
        }
}

void DrawPic(unsigned long* dst,char* src,unsigned long* textura)
//La tablas CLIPTAB_?[-256..?] dependen de las dimensiones de la textura a mapear
//La tabla Y ademas de clipping multiplica por GBL_Width la coordenada y
//optimizado para modo 320xY
{
	long iMaxX = GBL_Width-1;
	long iMaxY = GBL_Height-1;
	long xx, yy;

	_asm
	{
		pushad
		mov edi,[dst] //layer RGBA destino
		mov ebx,[src] //layer alpha mascara
		mov edx,[textura] //textura RGBA
		mov  esi,320+1 // puntero de recorrido [primer pixel en tratar (1,1)]
		mov   [yy],1
		y_loop2:
			MOV  [xx],1
			x_loop2:
				mov al,BYTE Ptr [ebx+esi-320]  // desnivel vertical
				sub al,BYTE Ptr [ebx+esi+320]
				shl eax,24
				sar eax,24 //extension de signo
				add eax,[yy]
				mov ecx,DWORD Ptr CLIPTAB_Y[eax*4 + 256*4] //la tabla empieza en -256
				mov al,BYTE Ptr [ebx+esi-1]  // desnivel horizontal
				sub al,BYTE Ptr [ebx+esi+1]
				shl eax,24
				sar eax,24 //extension de signo
				add eax,[xx]
				mov eax,DWORD Ptr CLIPTAB_X[eax*4 + 256*4] //la tabla empieza en -256
				add eax,ecx

				mov eax,[edx+eax*4] //cargamos el texel
				mov [edi+esi*4],eax //pintamos el pixel
				add esi,1

				inc [xx]
				cmp [xx],319
				jne x_loop2
			add esi,2
			inc [yy]
			cmp [yy],220
			jne y_loop2
		popad
	}
}

//--------------------------------------------------------------------

void DrawPicOffset(unsigned long* dst,char* src,unsigned long* textura)
//La tablas CLIPTAB_?[-256..?] dependen de las dimensiones de la textura a mapear
//La tabla Y ademas de clipping multiplica por GBL_Width la coordenada y
//optimizado para modo 320xY
{
	long iMaxX = GBL_Width-1;
	long iMaxY = GBL_Height-1;
	long xx, yy;

	_asm
	{
		pushad
		mov edi,[dst] //layer RGBA destino
		mov ebx,[src] //layer alpha mascara
		mov edx,[textura] //textura RGBA
		mov  esi,320+1 // puntero de recorrido [primer pixel en tratar (1,1)]
		mov   [yy],1
		y_loop2:
			MOV  [xx],1
			x_loop2:
				mov al,BYTE Ptr [ebx+esi]  // desnivel vertical
                 and eax,0x000000ff
//				shl eax,24
//				sar eax,24 //extension de signo
				add eax,[yy]
				mov ecx,DWORD Ptr CLIPTAB_Y[eax*4 + 256*4] //la tabla empieza en -256
				mov al,BYTE Ptr [ebx+esi+160]  // desnivel horizontal 160:es un random
                 and eax,0x000000ff
//				shl eax,24
//				sar eax,24 //extension de signo
				add eax,[xx]
				mov eax,DWORD Ptr CLIPTAB_X[eax*4 + 256*4] //la tabla empieza en -256 
				add eax,ecx

				mov eax,[edx+eax*4] //cargamos el texel
				mov [edi+esi*4],eax //pintamos el pixel
				add esi,1

				inc [xx]
				cmp [xx],319
				jne x_loop2
			add esi,2
			inc [yy]
			cmp [yy],220
			jne y_loop2
		popad
	}
}

//--------------------------------------------------------------------

void DrawWave(char *dst,int centro_x,int centro_y,long alpha)
{
	long px,py,yy;
	_asm
	{
		pushad
		MOV   eBX,[centro_x]
		neg   eBX
		//MOV   eAX,[inc_frame]
		//SUB   [frame],eAX                     //velocidad de propagacion de la onda
		MOV   edi,[dst]
		MOV   [yy],0
		y_loop:
			MOV  [px],eBX
			MOV  eAX,[yy]                 //py=DistY^2;
			SUB  eAX,[centro_y]
			XOR  eDX,eDX
			mul  eAX
			MOV  [py],eAX
			MOV  eCX,[GBL_Width]
			x_loop:
				ADD [px],1
				MOV eAX,[px]
				ADD [py],eAX
				MOV eSI,[py]
				SHR eSI,4
				sub eSI,[alpha]
				AND eSI,255
				MOV AL,BYTE Ptr SENO[eSI*4]
				SHR AL,1                    //factor de distorsion (o cambiando la amplitud de SENO)
				MOV BYTE Ptr [eDI],AL
				INC eDI
				DEC eCX
				jnz   x_loop
			INC   [yy]
			mov eax,[GBL_Height]
			CMP   [yy],eax
			JNE   y_loop
		popad
	}
}

#define MAX_CIRC    10

long    aiPosCircX[MAX_CIRC];
long    aiPosCircY[MAX_CIRC];

#define MAX_SPEED   3

long    aiIncCircX[MAX_CIRC];
long    aiIncCircY[MAX_CIRC];

unsigned char campana[256][256];

void CreaCampanaPlasmaCirc()
{
    for(int i=0;i<=255;i++)
        for(int ii=0;ii<=255;ii++)
        {
            long t = abs((SENO[i>>1]-128)*(SENO[ii>>1]-128)) >> 8;
            campana[ii][i] = t & 0xFF;
        }
}

void RenderPlasmaCircAlpha(char* dst)
{
    dst += 320*20; //centrar imagen verticalmente (solo se dibujan 204 lineas)
    _asm
    {        
        pushad
        //borra el fondo  
        mov     edi,[dst]
        xor     eax,eax
        mov     ecx,64000/4
        rep     stosd
        //superpone MAX_CIRC circulos (campanas)
        mov     dl,MAX_CIRC       //indice del circulo actual
        //dijua cada circulo
        cp_otro_Circ:

         xor    ebx,ebx
         mov    bl,dl
         mov    edi,[aiPosCircY+ebx*4]
         mov    eax,edi
         shl    eax,8
         shl    edi,6
         add    edi,eax         
         add    edi,[aiPosCircX+ebx*4]  //edi ->offset esquina sup-izq. del circulo         
         lea    esi,[campana]           //esi ->campana

         xor    ch,ch  //contador y (256)
         cp_bucy:
          xor   bh,bh  //contador x (256)
          cp_bucx:
           and    edi,0x0FFFF
           add    edi,[dst]               //guarro!
           lodsb
           add  [edi],al
           sub    edi,[dst]               //guarro!
           inc  edi
           dec  bh
           jnz  cp_bucx
          add  edi,320-256
          dec  ch
          jnz  cp_bucy

         dec  dl
         jnz  cp_otro_circ

        popad
    }

    //paletizar y blur
}
 
void MuevePlasmaCirc()
{
    for(int i=0;i<MAX_CIRC;i++)
    {
        //Nuevas velocidades
        aiIncCircX[i] += (rand() % 3)-1;
        if(aiIncCircX[i] < -MAX_SPEED)
            aiIncCircX[i] = -MAX_SPEED;
        if(aiIncCircX[i] > MAX_SPEED)
            aiIncCircX[i] = MAX_SPEED;

        aiIncCircY[i] += (rand() % 3)-1;
        if(aiIncCircY[i] < -MAX_SPEED)
            aiIncCircY[i] = -MAX_SPEED;
        if(aiIncCircY[i] > MAX_SPEED)
            aiIncCircY[i] = MAX_SPEED;

        //Nuevas posiciones
        aiPosCircX[i] += aiIncCircX[i];
        aiPosCircY[i] += aiIncCircY[i];
    }
}
