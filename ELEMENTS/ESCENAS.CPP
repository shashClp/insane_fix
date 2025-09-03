
//****************************************************************************
//* SPRAYS EN EL AGUA                                                        *
//****************************************************************************

//////////////////////////////////////////////////////////////////////////////
//
//  1 parte Sprays:
//  -fondo sprays
//  -doble scroll 'insane' con dist horiz [A + A(azul)]
//  -palabras con dist horiz por debajo [A]
//  -Dist horiz del rsultado [RGB]

void SCENA_SCROLLLEFT(long iTick)
{
    long iScrollPos = (iTick/30) % 1344;
    long iFactor = ((iTick/30) % 15) -7;  	

    FillMem((char*)imgAlpha8.bits,0,GBL_ScreenSize/16);              
    long iHDist = rand()%10;

    ScrollToScreen8((char*)imgAlpha8.bits,&imgScroll8,iScrollPos + iHDist,5);        
    HDist8((char*)imgAlpha8.bits,0,184,iFactor);
    FlatternAlphaBlue(GBL_Bits,imgTelon32.bits,imgAlpha8.bits);        
    ScrollToScreen8((char*)imgAlpha8.bits,&imgScroll8,iScrollPos - iHDist,5);    
    HDist8((char*)imgAlpha8.bits,0,184,10);
    for(int i=0;i<MAX_INTRO_PALABRAS;i++)
        if(2*afTimeIn[i]<iTick && 2*afTimeOut[i]>iTick)
            FONT_Print(aiPalabrasPos[i],185,apPalabrasText[i],fntAct,(char*)imgAlpha8.bits,25);
    long iHDistT = 1+((((iTick/2)-1000) % 3000) / 20);
	if(iHDistT>75)
		iHDistT = 75 + 1 - (iHDistT - 75);

    HDist8((char*)imgAlpha8.bits,185,215,iHDistT);
    FlatternAlpha(GBL_Bits,GBL_Bits,imgAlpha8.bits);            

    HDist32((dword*)GBL_Bits,20,220,iHDistT);
}

//////////////////////////////////////////////////////////////////////////////
//
//  2 parte Sprays:
//  -fondo sprays (scroll hacia rriba (sol) [RGB]
//  -doble scroll 'insane' con dist horiz [A + A(azul)] (terminando)
//  -palabras con dist horiz por debajo [A] (terminando)
//  -Dist Onda resultado progresivo (inc)  [RGB]

void SCENA_SCROLLUP(long iTick)
{
    
    long iScrollPos = ((iTick+TIME_FIN_SCENA_SCROLLLEFT)/30);
    long iFactor = (((iTick+TIME_FIN_SCENA_SCROLLLEFT)/30) % 15) -7;  	

    FillMem((char*)imgAlpha8.bits,0,GBL_ScreenSize/16);              
    long iHDist = rand()%10;
	long iVPos = 180 - (iTick/30);
	if(iVPos<0)
		iVPos = 0;

	ScrollToScreen32((dword*)imgTelon32.bits,&imgScroll32,0,20,iVPos);
	if(iScrollPos<imgScroll8.width)
		ScrollToScreen8((char*)imgAlpha8.bits,&imgScroll8,iScrollPos + iHDist,5);        
    HDist8((char*)imgAlpha8.bits,0,184,iFactor);
    FlatternAlphaBlue(GBL_Bits,imgTelon32.bits,imgAlpha8.bits);        
    if(iScrollPos<imgScroll8.width)
		ScrollToScreen8((char*)imgAlpha8.bits,&imgScroll8,iScrollPos - iHDist,5);    
    HDist8((char*)imgAlpha8.bits,0,184,10);
    for(int i=0;i<MAX_INTRO_PALABRAS;i++)
        if(2*afTimeIn[i]<(iTick + TIME_FIN_SCENA_SCROLLLEFT) && 2*afTimeOut[i]>(iTick + TIME_FIN_SCENA_SCROLLLEFT))
            FONT_Print(aiPalabrasPos[i],185,apPalabrasText[i],fntAct,(char*)imgAlpha8.bits,25);
    long iHDistT = (iTick/50)-70;
	if(iHDistT> -1)
		iHDistT = -1;
    
    HDist8((char*)imgAlpha8.bits,185,215,iHDistT);
FlatternAlpha((dword*)imgTelon32.bits,GBL_Bits,imgAlpha8.bits);            

CompletaTablaSenos((180-iVPos)/7.0f);
DrawWave((char*)imgAlpha8.bits,GBL_ScreenWidth>>1,-iVPos+150,iTick/10);
DrawPic(GBL_Bits,(char*)imgAlpha8.bits,(dword*)imgTelon32.bits);
//Recorta32(GBL_Bits,20);
	DrawPic(GBL_Bits,(char*)imgAlpha8.bits,(dword*)imgTelon32.bits);
	_asm
	{
		pushad
		mov edi,[GBL_Bits]
		mov ecx,320*20
		xor eax,eax
		rep stosd
		add edi,320*200*4
		mov ecx,320*20
		rep stosd
		popad 
	}
}