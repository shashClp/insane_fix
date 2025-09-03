// AWL - Anaconda Windows Library
// Files

#ifndef AWL_FILE
#define AWL_FILE

#include <windows.h>
#include <stdio.h>

bool FILE_Eof  (FILE *File);
bool FILE_Open (FILE **File,char *Path,char *Mode);
bool FILE_Close(FILE *File);
bool FILE_Write(void *Buffer,DWORD Size,DWORD Number,FILE *File);
bool FILE_Read (void *Buffer,DWORD Size,DWORD Number,FILE *File);
int  FILE_Getc (FILE *File);
bool FILE_Seek (FILE *File,long Offset,DWORD Where);
long FILE_Tell(FILE *File);
long FILE_Size (FILE *File);

#endif