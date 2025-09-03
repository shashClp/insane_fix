#include <windows.h>
#include <mem.h>
#include <awm.h>
#include <view.h>
#include <syswin.h>
#include <log.h>
#include <traverse.h>
#include <listz.h>
#include <elisium.h>
#include <traverseZ.h>
#include <default.h>
#include <file.h>
#include <debug.h>
#include "guio.h"

#define GVersion                    "2.0"
#define GUIO_SECOND                 1000

/*******************************************************************************/
//#define DEBUG_TIME
/*******************************************************************************/

typedef struct {
  long start;
  long end;
  long duration;
  long real_duration;
  long start_efect;  
  void (*Action)(float now);
} TSequence;

typedef struct {
  char Name[255];
  void (*Action)(float now);
} TPublicFN;

TList GUIO_Base;
TListZ GUIO_Layers;
TList GUIO_Actions;
TList GUIO_PublicFN;
bool GUIO_Initialized=false;
volatile long GUIO_Time;
long GUIO_StartRun;
long GUIO_Last;
long GUIO_LastBefore;
long GUIO_Old;
long OLD_Time;
long NEW_Time;
double GUIO_cicle2tick;

/*******************************************************************************/

FILE *FileInput;
BYTE *FileInputBuffer = NULL;
DWORD GLine;
DWORD GCol;
DWORD TamFileBuffer;
DWORD IndexFileBuffer;
char TOKEN_DELIMITERS[10] = {'[',']','(',')',' ','#',';',','};
char ACT_TOKEN[200];

//*******************************************************************************
DWORD *GP_Position = NULL;
DWORD GP_Size;
DWORD GP_Counter = 0;
bool GP_PosLoaded = false;

void GUIO_InitBufferPos(DWORD S)
{
   GP_Position = (DWORD*) GetMem (S*1000*sizeof(DWORD));
	 GP_Size = S*1000;
}

void GUIO_SaveBufferPos()
{
	FILE *OutF;
	FILE_Open (&OutF,"GUI.POS","wb");
	FILE_Write (&GP_Counter,1,sizeof(DWORD),OutF);
	FILE_Write (GP_Position,1,GP_Counter*sizeof(DWORD),OutF);
	FILE_Close (OutF);
}

void GUIO_LoadBufferPos()
{
	FILE *OutF;
	FILE_Open (&OutF,"GUI.POS","rb");
	FILE_Read (&GP_Counter,1,sizeof(DWORD),OutF);
	FILE_Read  (GP_Position,1,GP_Counter*sizeof(DWORD),OutF);
	FILE_Close (OutF);
	GP_PosLoaded = true;
}

DWORD  GUIO_GetPosition ( long Tick )
{
	if ((!GP_PosLoaded) || (Tick<2000))
	{
		GUIO_StartRun = 0;
	  return 0;
	}
  long i = Tick;
	DWORD Val;
	long Pos;

	while (GP_Position[i]==0) {i--;}
	Val = GP_Position[i];
	Pos = i;
	i--;
	while ((GP_Position[i]==Val) || (GP_Position[i]==0)) 
	{
	   if (GP_Position[i]==Val) Pos=i;
	   i--;
  }
	GUIO_StartRun = Pos;
	return GP_Position[Pos];
}

//*******************************************************************************

void GUIO_Nothing (float a)
{  
  DWORD dur = (long)(a*GBL_Size);
  memset (GBL_Bits,0xffffff,dur*4);
  dur = (GBL_Size)-dur;
  memset (GBL_Bits+dur,0x0,dur*4);
}

//*******************************************************************************

void GUIO_AppendBaseSequence (void (*Action)(float now),float duration,float realduration,float start_efect)
{
TSequence g;

  g.Action = Action;
  g.start = GUIO_Last;
  g.start_efect = (long)(start_efect*GUIO_SECOND);
  g.duration = (long)(duration*GUIO_SECOND);
  if (realduration==0) g.real_duration = (long)(duration*GUIO_SECOND);
  else g.real_duration = (long)(realduration*GUIO_SECOND);
  g.end = g.start+g.real_duration;
  LIST_Add (GUIO_Base,&g,sizeof(TSequence));
  
  GUIO_LastBefore = GUIO_Last;
  GUIO_Last = g.end;
  LOG ("GUIOv2 Base Append [%d][%d]",g.start,g.end);
}

//*******************************************************************************

void GUIO_InsertLayerSequence (void (*Action)(float now),long level,float start,float duration,float realduration,float start_efect)
{
TSequence g;

  g.Action = Action;
  g.start = GUIO_LastBefore+(long)(start*GUIO_SECOND);;
  g.start_efect = (long)(start_efect*GUIO_SECOND);
  g.duration = (long)(duration*GUIO_SECOND);
  if (realduration==0)  g.real_duration = (long)(duration*GUIO_SECOND);
  else g.real_duration = (long)(realduration*GUIO_SECOND);
  g.end = g.start + g.real_duration;  
  LISTZ_Add (GUIO_Layers,&g,sizeof(TSequence),level);

  LOG ("GUIOv2 Layer Append [%d][%d]",g.start,g.end);
}

//******************************************************************************************

void GUIO_InsertAction (void (*Action)(float now),float start)
{
TSequence g;

  g.Action = Action;
  g.start = GUIO_LastBefore+(long)(start*GUIO_SECOND);
  g.start_efect = 0;
  g.duration = 10000;
  g.real_duration = 10000;
  g.end = g.start + 10000;
  LIST_Add (GUIO_Actions,&g,sizeof(TSequence));

  LOG ("GUIOv2 Action Inserted",g.start,g.end);
}

//******************************************************************************************

inline void GUIO_Timer()
{
  NEW_Time = GetTickCount();//(long)(((double)MISC_GetTickCount())/GUIO_cicle2tick);
  GUIO_Time += NEW_Time - OLD_Time;
  OLD_Time = NEW_Time;
#ifdef DEBUG_TIME
  DEBUG (GBL_Width-100,1,"%d",GUIO_Time);
#endif
}

void GUIO_InfoPos(DWORD Fin)
{

  OLD_Time = GetTickCount(); //(long)(((double)MISC_GetTickCount())/GUIO_cicle2tick);
	while (GP_Counter<Fin)
	{
		NEW_Time = GetTickCount(); //(long)(((double)MISC_GetTickCount())/GUIO_cicle2tick);
		GUIO_Time += NEW_Time - OLD_Time;
		OLD_Time = NEW_Time;
		if (!GP_PosLoaded)
		{
				if (GUIO_Time<GP_Size)
				{
					GP_Position[GUIO_Time] = 	MUSIC_GetPos();	
					GP_Counter = GUIO_Time;
				}
		}
		DEBUG (GBL_Width-50,1,"%d" ,GUIO_Time);
    DEFAULT_Flip();    
	}
}

void GUIO_Run (DWORD Pos)
{
  GUIO_Time = Pos;
  OLD_Time = GetTickCount(); //(long)(((double)MISC_GetTickCount())/GUIO_cicle2tick);
	MUSIC_PlayExt (GUIO_GetPosition(GUIO_StartRun));
  while ((GUIO_Time<GUIO_Last) && (!Exit))
  {
    GUIO_Timer();
  
    // Accions, funcions que nomes s'executen 1 cop
    TTraverse ta;
    TListItem *l;    
    TListItem **ll;
    TSequence *g;
    TList list_eliminar;
    LIST_Init (list_eliminar);
    LIST_GetTraverse (ta,GUIO_Actions);
    for (DWORD i=0; i<GUIO_Actions.count; i++)
    {
      l = LIST_GetItem (ta);
      g = (TSequence *)(l+1);
      if (g->start<=GUIO_Time)
      {
        g->Action(0.0f);
        LIST_Add (list_eliminar,&l,4);
      }
      LIST_Next (ta);
    }
    // Eliminem les funcions ja utilitzades
    LIST_GetTraverse (ta,list_eliminar);
    for (i=0; i<list_eliminar.count; i++)
    {
      l = LIST_GetItem (ta);
      ll = (TListItem **)(l+1);
      LIST_Delete (GUIO_Actions,*ll);
      LIST_Next (ta);
    }
    LIST_Free (list_eliminar);


    // Base Sequences s'eliminen quan ja han expirat el temps
    LIST_Init (list_eliminar);
    LIST_GetTraverse (ta,GUIO_Base);
    for (i=0; i<GUIO_Base.count; i++)
    {
      l = LIST_GetItem (ta);
      g = (TSequence *)(l+1);
      if ((g->start+g->start_efect<=GUIO_Time) && (g->end>GUIO_Time))
      {
        g->Action(((float)GUIO_Time-g->start)/g->duration);        
      } else {
        if (g->end<=GUIO_Time) LIST_Add (list_eliminar,&l,4);
      }
      LIST_Next (ta);
    }

    // Eliminem les Base sequences ja expirades
    LIST_GetTraverse (ta,list_eliminar);
    for (i=0; i<list_eliminar.count; i++)
    {
      l = LIST_GetItem (ta);
      ll = (TListItem **)(l+1);
      LIST_Delete (GUIO_Base,*ll);
      LIST_Next (ta);
    }
    LIST_Free (list_eliminar);

    // Layers s'eliminen quan ja han expirat el temps
    TTraverseZ taZ;
    TListZItem *lz;    
    TListZItem **llz;    
    LIST_Init (list_eliminar);
    LISTZ_GetTraverse (taZ,GUIO_Layers);
    for (i=0; i<GUIO_Layers.count; i++)
    {
      lz = LISTZ_GetItem (taZ);
      g = (TSequence *)(lz+1);
      if ((g->start+g->start_efect<=GUIO_Time) && (g->end>GUIO_Time))
      {
        g->Action(((float)GUIO_Time-g->start)/g->duration);
      } else {
        if (g->end<=GUIO_Time) LIST_Add (list_eliminar,&lz,4);
      }
      LISTZ_Next (taZ);
    }    

    // Eliminem els layers ja expirats
    LIST_GetTraverse (ta,list_eliminar);
    for (i=0; i<list_eliminar.count; i++)
    {
      l = LIST_GetItem (ta);
      llz = (TListZItem **)(l+1);
      LISTZ_Delete (GUIO_Layers,*llz);
      LIST_Next (ta);
    }
    LIST_Free (list_eliminar);

    DEFAULT_Flip();    
  }
}

//******************************************************************************************
//******************************************************************************************

void GUIO_AddFN(void (*Action)(float now),char *Name)
{
TPublicFN g;

  g.Action = Action;
  strcpy (g.Name,Name);  
  LIST_Add (GUIO_PublicFN,&g,sizeof(TPublicFN));
  LOG ("GUIOv2 Public Function[%s] Added",Name);
}

TPublicFN *GUIO_GetFN (char *Name)
{
  TTraverse ta;
  LIST_GetTraverse (ta,GUIO_PublicFN);
  TPublicFN *Nothing = (TPublicFN *)LIST_Get(ta);
  LIST_Next (ta);
  for (DWORD i=0; i<GUIO_PublicFN.count; i++)
  {
    TPublicFN *g = (TPublicFN *) LIST_Get (ta);
    if (!stricmp (Name,g->Name)) return g;    
    LIST_Next (ta);
  }
  return Nothing;
}

//******************************************************************************************

char GUIO_GetChar ( void )
{
  long Comment = 0;
  if ( IndexFileBuffer == TamFileBuffer ) return 1;
  char CH = FileInputBuffer[IndexFileBuffer++];
InitComment:
  if ( CH == '/' )
  {                                               //± Skip comments
    CH = FileInputBuffer[IndexFileBuffer++];
    if ( CH == '/' )
      while ((CH != 13)&&(CH != 10)) CH = FileInputBuffer[IndexFileBuffer++];
    if ( CH == '*' )
      while (!Comment)
      {
        while (CH != '*') CH = FileInputBuffer[IndexFileBuffer++];
        CH = FileInputBuffer[IndexFileBuffer++];
        if (CH == '/') Comment=1;
      }
    CH = FileInputBuffer[IndexFileBuffer++];
    goto InitComment;
  }
  return CH;
}

byte GUIO_Delimiter ( char CH )
{
  for ( long i = 0; i < 5; i ++ )
   if ( TOKEN_DELIMITERS[i] == CH ) return 1;
  for ( i = 5; i < 10; i ++ )
   if ( TOKEN_DELIMITERS[i] == CH ) return 2;
  if  ( ( CH==13 ) || ( CH==10 ) )  return 1;
 
return 0;
}

//******************************************************************************************

void GUIO_GetToken ()
{
  char TOKEN[200];
  long Col_TMP = 0;
  char ActCharacter;  

  ActCharacter = GUIO_GetChar( );
  
  if (ActCharacter == 1) {ERR ( "GUIOv2.0 Parse File" ); exit (0);};
    
  if ( GUIO_Delimiter ( ActCharacter ) == 2) TOKEN[Col_TMP++] = ActCharacter;

  while ( GUIO_Delimiter  (ActCharacter)==1 )
  {
    ActCharacter = GUIO_GetChar ( );
    if (ActCharacter==1) {ERR ( "GUIOv2.0 Parse File" ); exit (0);};
  }
  while (!GUIO_Delimiter(ActCharacter))
  {
    TOKEN[Col_TMP++] = ActCharacter;
    ActCharacter = GUIO_GetChar ( );
    if (ActCharacter == 1) {ERR ( "GUIOv2.0 Parse File" ); exit (0);};
    if ( GUIO_Delimiter ( ActCharacter ) == 2) IndexFileBuffer--;
  } 
  strcpy (ACT_TOKEN,TOKEN);
  ACT_TOKEN[Col_TMP] = '\x0';
}

//******************************************************************************************

float GUIO_Str2Real (char *Cad)
{
  float Enter = 0; long P = -1;long i = 0; byte Neg = 0;
  if ( Cad[0]  == '-' ) { Neg = 1; i++; }
  while ( Cad[i] != '\x0' )
  {
    if (Cad[i] == '.') P = i; else
     if ( (Cad[i] >='0') && (Cad[i] <='9') )
       Enter = Enter*10.0f+(Cad[i]-'0');
     else
     {
      ERR ("Expecting number in %s",Cad);
      exit (0);
     }
    i++;
  }
  if ( P != -1 )
  {
    i -=1;
    while ( i>(P) ) { Enter /=10.0f; i--; }
  }
  if ( Neg ) Enter = (Enter)*(-1.0f);
  return Enter;
}

//******************************************************************************************

void GUIO_Do0 ()
{
  char NameF[200];
  GUIO_GetToken ();
  strcpy (NameF,ACT_TOKEN);
  float Val1[3];
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[0]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[1]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[2]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=';') { ERR ("Expecting \";\" at %s",ACT_TOKEN); exit (0); }
  LOG ("GUIO_AppendBaseSequence ( %s , %3.3f , %3.3f , %3.3f );",NameF,Val1[0],Val1[1],Val1[2]);
  GUIO_AppendBaseSequence (GUIO_GetFN (NameF)->Action,Val1[0],Val1[1],Val1[2]);
}

//******************************************************************************************

void GUIO_Do1 ()
{
  char NameF[200];
  GUIO_GetToken ();
  strcpy (NameF,ACT_TOKEN);
  float Val1[5];
  DWORD Val2[5];
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val2[0]=(long)GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[0]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[1]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[2]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[3]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=';') { ERR ("Expecting \";\" at %s",ACT_TOKEN); exit (0); }
  LOG ("GUIO_InsertLayerSequence ( %s , %d , %3.3f , %3.3f , %3.3f , %3.3f );",NameF,Val2[0],Val1[0],Val1[1],Val1[2],Val1[3]);
  GUIO_InsertLayerSequence (GUIO_GetFN (NameF)->Action,Val2[0],Val1[0],Val1[1],Val1[2],Val1[3]);
}

//******************************************************************************************

void GUIO_Do2 ()
{
  char NameF[200];
  GUIO_GetToken ();
  strcpy (NameF,ACT_TOKEN);
  float Val1[3];
  GUIO_GetToken ();if (ACT_TOKEN[0]!=',') { ERR ("Expecting \",\" at %s",ACT_TOKEN); exit (0); }
  GUIO_GetToken ();Val1[0]=GUIO_Str2Real ( ACT_TOKEN );
  GUIO_GetToken ();if (ACT_TOKEN[0]!=';') { ERR ("Expecting \";\" at %s",ACT_TOKEN); exit (0); }
  LOG ("GUIO_InsertAction  ( %s , %3.3f );",NameF,Val1[0]);
  GUIO_InsertAction  (GUIO_GetFN (NameF)->Action,Val1[0]);
  
}
//******************************************************************************************

bool GUIO_Load(char *Name)
{
  if (FileInputBuffer!=NULL) FreeMem (FileInputBuffer);
  GLine = GCol = 1;
  TamFileBuffer = IndexFileBuffer = 0;

  if (!FILE_Open (&FileInput,Name,"rt")) 
  { 
    ERR("Can't load GUIO File %s",Name); 
    return false; 
  }
  TamFileBuffer = FILE_Size(FileInput);
  FileInputBuffer = (byte *) GetMem( TamFileBuffer );
  FILE_Read (FileInputBuffer,TamFileBuffer,1,FileInput);
  FILE_Close(FileInput);

  GUIO_GetToken ();
  if (stricmp (ACT_TOKEN,"Version")) 
  {
    ERR ( "GUIOv2.0 Can't get version File" ); 
    return false;
  }
  GUIO_GetToken ();
  if (stricmp (ACT_TOKEN,GVersion)) 
  {
    ERR ( "GUIOv2.0 Unsuported version File" ); 
    return false;
  }
  GUIO_GetToken ();
  if (stricmp (ACT_TOKEN,";")) 
  {
    ERR ( "GUIOv2.0 Expecting \";\" at %s",ACT_TOKEN); 
    return false;
  }

  while (stricmp(ACT_TOKEN,"GUIO_End"))
  {
    GUIO_GetToken ();
    if (!stricmp (ACT_TOKEN,"GUIO_AppendBaseSequence")) GUIO_Do0 (); else
    if (!stricmp (ACT_TOKEN,"GUIO_AppendSequence")) GUIO_Do0 (); else
    if (!stricmp (ACT_TOKEN,"GUIO_InsertLayerSequence")) GUIO_Do1 (); else
    if (!stricmp (ACT_TOKEN,"GUIO_InsertLayer")) GUIO_Do1 (); else
    if (!stricmp (ACT_TOKEN,"GUIO_InsertAction")) GUIO_Do2 ();
  }
  GUIO_GetToken ();
  if (ACT_TOKEN[1]==')') 
    GUIO_StartRun = 0;
  else
    GUIO_StartRun = 1000*(int)GUIO_Str2Real ( ACT_TOKEN );

  return true;
}

//*******************************************************************************

void GUIO_Init()
{
  if (GUIO_Initialized) return;
  GUIO_Initialized = true;
  GUIO_Time = 0;
  GUIO_Last = 0;
  GUIO_LastBefore = 0;
  LIST_Init (GUIO_Base);
  LISTZ_Init (GUIO_Layers);
  LIST_Init (GUIO_Actions);
  LIST_Init (GUIO_PublicFN);
  GUIO_AddFN(GUIO_Nothing,"NOTHING");

  GUIO_cicle2tick = SYSWIN_GetCPUSpeed();

  LOG ("GUIOv20 initialized!");
}

void GUIO_Close()
{
  if (!GUIO_Initialized) return;
  GUIO_Initialized = FALSE;
  LIST_Free (GUIO_Base);
  LISTZ_Free (GUIO_Layers);
  LIST_Free (GUIO_Actions);
  LIST_Free (GUIO_PublicFN);
  LOG ("GUIOv20 closed. Final Time[%4.3f]",(float)GUIO_Time/1000.0);
}

//*******************************************************************************
