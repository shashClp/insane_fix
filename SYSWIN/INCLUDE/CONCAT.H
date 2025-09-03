// AWL - Anaconda Windows Library
// Concat: Anaconda Windows Data File System


#ifndef AWL_CONCAT
#define AWL_CONCAT

#define CONCAT_VERSION					3

#include <windows.h>
#include <stdio.h>

typedef struct {
	char name[128];
	long seek;	  // Begining of data block
	long size;	  // Total data block size (compression included)
	long fsize;		// Normal file size
	long pos;			// Actual buffer position (Where are we?)
	bool used;
	BYTE *buffer; // file data buffer
} TConcat;

extern bool CONCAT_Enabled;
extern DWORD CONCAT_nTable;
extern TConcat *CONCAT_Table;

void  CONCAT_Init (char *filename,bool enable);
void  CONCAT_Close();
void  CONCAT_FilesUsed();
bool  CONCAT_Eof  (BYTE *File);
FILE* CONCAT_Open (const char *filename);
void  CONCAT_Close(BYTE *File);
void  CONCAT_Read (void *Buffer,DWORD Size,BYTE *File);
int   CONCAT_Getc (BYTE *File);
void  CONCAT_Seek (BYTE *File,long Offset,DWORD Where);
long  CONCAT_Tell (BYTE *File);
long  CONCAT_Size (BYTE *File);

#endif
