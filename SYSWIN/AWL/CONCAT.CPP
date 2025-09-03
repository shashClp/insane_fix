// AWL - Anaconda Windows Library
// Concat: Anaconda Windows Data File System

#include <log.h>
#include <misc.h>
#include <concat.h>
#include <mem.h>
#include <oxcompr.h>
#include <oxbitbuf.h>

#define CONCAT_MAX_BUFFER	1000000

// External variables
bool CONCAT_Enabled;
DWORD CONCAT_nTable;
TConcat *CONCAT_Table;

// Local variables
FILE *CONCAT_File;
COXCompressor compressor;
char FileHeader[50]="CONCAT v.3.0 by Klauz/Anaconda";

void ReadFileName(FILE *in, char *name)
{
  int i=0, c=1;
  while(c != 0) {
    c = fgetc(in);
    name[i++] = (char) c;
  }
}

// Find table index with buffer
long IndexBufferTable(BYTE *p)
{
	for(DWORD i=0; i<CONCAT_nTable; i++) {
		if (CONCAT_Table[i].buffer==p) {
			return i;
		}
	}
	return -1;
}

// Find table index with file name
long IndexNameTable(const char *filename)
{
	for(DWORD i=0; i<CONCAT_nTable; i++) {
		if (!stricmp(CONCAT_Table[i].name,filename)) {
			return i;
		}
	}
	return -1;
}

void CONCAT_Init(char *filename,bool enable)
{
	SEC_START("Initializing Anaconda Data File System");
	CONCAT_Enabled = false;
	CONCAT_nTable = 0;
	CONCAT_Table = NULL;
	if (enable) {
		CONCAT_File = fopen(filename,"rb");
		if (CONCAT_File==NULL) {
			LOG("Could not open file: %s",filename);
		} else {
			char Version;
			char Header[50];

			int	j=0, c=1;
			while (c!=0) {
				c = fgetc(CONCAT_File);
				Header[j++] = (char) c;
			}
			fread(&Version,1,1,CONCAT_File);
			if ((strcmp(Header, FileHeader)) || (Version != CONCAT_VERSION)) {
				LOG("Invalid Anaconda Data File: %s",filename);
	      fclose(CONCAT_File);
			} else {
				DWORD i, nf;
			  fread(&nf,4,1,CONCAT_File);
				CONCAT_nTable = nf;
				CONCAT_Table = (TConcat *) GetMem(sizeof(TConcat)*nf);
				memset(CONCAT_Table,0,sizeof(TConcat)*nf);
				for (i=0; i<nf; i++) {
			    ReadFileName(CONCAT_File,CONCAT_Table[i].name);
					fread(&CONCAT_Table[i].size,4,1,CONCAT_File);
					CONCAT_Table[i].seek = ftell(CONCAT_File);
					fseek(CONCAT_File,CONCAT_Table[i].size,SEEK_CUR);
        }
				CONCAT_Enabled = true;
			}
		}
	}
	SEC_END();
}

void CONCAT_Close()
{
  LOG ("Freeing CONCAT!");
	if (CONCAT_Enabled) {
		for(DWORD i=0; i<CONCAT_nTable; i++) {
			if (CONCAT_Table[i].buffer!=NULL) {
				FreeMem(CONCAT_Table[i].buffer);
				CONCAT_Table[i].buffer = NULL;
			}
		}

		CONCAT_Enabled = false;
		CONCAT_nTable = 0;
		fclose(CONCAT_File);
		FreeMem(CONCAT_Table);
	}
}

void CONCAT_FilesUsed()
{
	for(DWORD i=0; i<CONCAT_nTable; i++) {
		if (CONCAT_Table[i].used) {
			LOG_NET("copy %s c:\\klzfinal",CONCAT_Table[i].name);
		}
	}
}

bool CONCAT_Eof (BYTE *File)
{
	long i=IndexBufferTable(File);

	if (CONCAT_Table[i].pos>=CONCAT_Table[i].fsize) return true;
	else return false;
}

FILE* CONCAT_Open(const char *filename)
{
	long i=IndexNameTable(filename);

	if (i==-1) {
		ERR("CONCAT: Opening file %s",filename);
		return NULL;
	}

	if (CONCAT_Table[i].buffer!=NULL) {
		CONCAT_Table[i].pos = 0;
		return ((FILE *) CONCAT_Table[i].buffer);
	}

	BYTE *buffer;
	BYTE compressed;
	long Tamany, Actual, DataLen, DecompDataLen, OutDataLen;

	fseek(CONCAT_File,CONCAT_Table[i].seek,SEEK_SET);
	fread(&Tamany,4,1,CONCAT_File);
	CONCAT_Table[i].fsize = Tamany;

	buffer = (BYTE *) GetMem(CONCAT_MAX_BUFFER);
	CONCAT_Table[i].buffer = (BYTE *) GetMem(Tamany);
	CONCAT_Table[i].pos = 0;
  Actual = 0;
  while(Actual != Tamany) {
		fread(&compressed, 1, 1, CONCAT_File);
		if (compressed) {
			fread(&DecompDataLen, 4, 1, CONCAT_File);
			fread (buffer, DecompDataLen, 1, CONCAT_File);
			OutDataLen = compressor.Expand(buffer, DecompDataLen, CONCAT_Table[i].buffer, Tamany);
			Actual += OutDataLen;
		} else {
			fread(&DataLen, 4, 1, CONCAT_File);
			fread (CONCAT_Table[i].buffer, DataLen, 1, CONCAT_File);
			Actual += DataLen;
		}
	}
	FreeMem(buffer);

	return ((FILE *) CONCAT_Table[i].buffer);
}

void CONCAT_Close(BYTE *File)
{
	long i=IndexBufferTable(File);

	CONCAT_Table[i].pos = 0;
}

void CONCAT_Read(void *Buffer,DWORD Size,BYTE *File)
{
	long i=IndexBufferTable(File);

	if ((CONCAT_Table[i].pos+Size) >= CONCAT_Table[i].fsize) {
		Size = CONCAT_Table[i].fsize - CONCAT_Table[i].pos;
	}

	memcpy(Buffer,File+CONCAT_Table[i].pos,Size);
	CONCAT_Table[i].pos += Size;

	if (CONCAT_Table[i].pos >= CONCAT_Table[i].fsize) CONCAT_Table[i].pos = 0;

	CONCAT_Table[i].used = true;
}

int CONCAT_Getc(BYTE *File)
{
	long i=IndexBufferTable(File);

	CONCAT_Table[i].used = true;

	return (int) File[CONCAT_Table[i].pos++];
}

void CONCAT_Seek(BYTE *File,long Offset,DWORD Where)
{
	long i=IndexBufferTable(File);

	switch(Where) {
		case 0: CONCAT_Table[i].pos = Offset;													break;
		case 1: CONCAT_Table[i].pos += Offset;												break;
		case 2: CONCAT_Table[i].pos = CONCAT_Table[i].fsize + Offset; break;	
	}	
}

long CONCAT_Tell(BYTE *File)
{
	long i=IndexBufferTable(File);

	return CONCAT_Table[i].pos;
}

long CONCAT_Size(BYTE *File)
{
	long i=IndexBufferTable(File);

	return CONCAT_Table[i].fsize;
}

