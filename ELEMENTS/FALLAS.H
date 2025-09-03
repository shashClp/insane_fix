//////////////////////////////////////////////////////////////////////////////
//
//  void HDist8Des;
//  Distorsion horizontal a lo 303 con factor (1 maximo,+/-127 minimo)
// Factor>0 -> se difumina el blanco
// Factor<0 -> se difumina el negro
// Un factor positivo bajo (mas distorsion) hace que se desbanezca mas

void HDist8DesF(char* dst,int lin_ini,int lin_fin,char factor)
{
    if(factor==0)
        return;

    dst += GBL_ScreenWidth*lin_ini;

    char factor_des = (120 - factor>>1); //solo se utiliza con factores positivos (difusion de intensidad)
    if(factor_des < 0)
        factor_des = 0;

    if(factor<0)
        //Difuminar Negro
        _asm
        {
            pushad
            neg     [factor]
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            NbucY:

                //hacia la drecha
                mov     bl,255
                mov     ecx,[GBL_ScreenWidth]
    
               NbucX1: 
                mov     al,[edi]
                cmp     al,bl
                jb      Nnormalizado1
                 mov    al,bl
                Nnormalizado1:
                stosb

                mov     bl,al
                add     bl,[factor]

                jnc     Nnormalizado_factor1
                 mov    bl,255
                Nnormalizado_factor1:

                dec     ecx
                jnz     NbucX1

                //hacia la izquierda
                dec     edi
                std
                xor     bl,bl
                mov     ecx,[GBL_ScreenWidth]                
                    
               NbucX2: 
                mov     al,[edi]
                cmp     al,bl
                jb      Nnormalizado2
                 mov    al,bl
                Nnormalizado2:
                stosb

                mov     bl,al
                add     bl,[factor]

                jnc     Nnormalizado_factor2
                 mov    bl,255
                Nnormalizado_factor2:

                dec     ecx
                jnz     NbucX2

                mov     eax,[GBL_ScreenWidth]
                cld
                lea     edi,[edi + eax + 1]

            dec edx
            jnz NbucY
            popad        
        }
    else
        //Difuminar Blanco
        _asm
        {
            pushad                        
            mov     edx,[lin_fin]
            sub     edx,[lin_ini]
            mov     edi,[dst]    
            inc     edx
            BbucY:

                //hacia la drecha
                xor     bl,bl
                mov     ecx,[GBL_ScreenWidth]
    
               BbucX1: 
                mov     al,[edi]
                sub     al,[factor_des]
                jnc     Bnormalizada_entrada1
                 xor    al,al
                Bnormalizada_entrada1:

                cmp     al,bl
                ja      Bnormalizado1
                 mov    al,bl
                Bnormalizado1:
                stosb

                mov     bl,al
                sub     bl,[factor]

                jnc     Bnormalizado_factor1
                 xor    bl,bl
                Bnormalizado_factor1:

                dec     ecx
                jnz     BbucX1

                //hacia la izquierda
                dec     edi
                std
                xor     bl,bl
                mov     ecx,[GBL_ScreenWidth]                
                    
               BbucX2: 
                mov     al,[edi]
                sub     al,[factor_des]
                jnc     Bnormalizada_entrada2
                 xor    al,al
                Bnormalizada_entrada2:

                cmp     al,bl
                ja      Bnormalizado2
                 mov    al,bl
                Bnormalizado2:
                stosb

                mov     bl,al
                sub     bl,[factor]

                jnc     Bnormalizado_factor2
                 xor    bl,bl
                Bnormalizado_factor2:

                dec     ecx
                jnz     BbucX2

                mov     eax,[GBL_ScreenWidth]
                cld
                lea     edi,[edi + eax + 1]

            dec edx
            jnz BbucY
            popad        
        }
}
