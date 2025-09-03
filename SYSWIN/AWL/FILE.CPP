// AWL - Anaconda Windows Library
// Files

#include <log.h>
#include <concat.h>
#include <mem.h>
#include <misc.h>

bool FILE_Eof(FILE *File)
{
	if (!File) return true;

	if (CONCAT_Enabled) return CONCAT_Eof((BYTE *)File);
	else return (feof(File)!=0);
}

bool FILE_Open(FILE **res,char *Path,char *Mode)
{
	if (!Path)
  {
    ERR ("Opening with Path [] a file?");
    return false;
  }
  if (!Mode)
  {
    ERR ("Opening with Mode [] a file?");
    return false;
  }
  if (res==NULL)
  {
    ERR ("NULL Handle for file %s",Path);
    return false;
  }
  LOG ("Opening file %s with %s mode",Path,Mode);
  if (CONCAT_Enabled) {
		*res = CONCAT_Open((LPCTSTR)Path);
	} else {
	  *res = fopen((LPCTSTR)Path,(LPCTSTR)Mode);
	}
  if (*res==NULL)
  {
    ERR ("Opening File %s",Path);
    return false;
  }
  return true;
}

bool FILE_Close(FILE *File)
{  
  if (!File)
  {
    ERR ("Closing NULL File");
    return false;
  }
	if (CONCAT_Enabled) CONCAT_Close((BYTE *)File);
  else fclose(File);

  return true;
}

bool FILE_Write(void *Buffer,DWORD Size,DWORD Number,FILE *File)
{
  if (!File)
  {
    ERR ("Writing to NULL file, Size: %i Number: %i",Size,Number);
    return false;
  }
  if (!Buffer)
  {
    ERR ("File Writing to NULL Buffer: Size: %i Number: %i File: %p",Size,Number,File);
    return false;
  }
  if (Size*Number==0)
  {
    ERR ("Writing 0 from file: File: %p",File);
    return false;
  }
  size_t ok = fwrite(Buffer,Size,Number,File);
  if (ok < Size*Number)
  {
    ERR ("Writing to File: Size: %i Number: %i File: %p",Size,Number,File);
    return false;
  }
  return true;
}

bool FILE_Read(void *Buffer,DWORD Size,DWORD Number,FILE *File)
{
  if (!File)
  {
    ERR ("Reading from NULL file, Size: Size: %i Number: %i",Size,Number);
    return false;
  }
  if (!Buffer)
  {
    ERR ("File Reading to NULL Buffer: Size: %i Number: %i File: %p",Size,Number,File);
    return false;
  }
  if (Size*Number==0)
  {
    ERR ("Reading 0 from file: File: %p",File);
    return false;
  }
	if (CONCAT_Enabled) CONCAT_Read(Buffer,Size*Number,(BYTE *) File);
	else fread(Buffer,Size,Number,File);
  return true;
}

int FILE_Getc(FILE *File)
{
  if (!File)
  {
    ERR ("Reading a character from NULL file");
    return -1;
  }
	if (CONCAT_Enabled) return CONCAT_Getc((BYTE *) File);
	else return fgetc(File);
}

bool FILE_Seek(FILE *File,long Offset,DWORD Where)
{
  if (!File)
  {
    ERR ("Seeking from NULL file");
    return false;
  }
	if (CONCAT_Enabled) CONCAT_Seek((BYTE *) File,Offset,Where);
	else fseek(File,Offset,Where);
  return true;
}

long FILE_Tell(FILE *File)
{
  if (!File)
  {
    ERR ("FILE_Tell NULL File");
    return false;
  }
	if (CONCAT_Enabled) return CONCAT_Tell((BYTE *) File);
	else return ftell(File);
}

long FILE_Size(FILE *File)
{
  if (!File)
  {
    ERR ("FILE_Size NULL File");
    return false;
  }
	
	long Size;
	if (CONCAT_Enabled) Size = CONCAT_Size((BYTE *) File);
	else {
		long pos = ftell(File);
		fseek(File,0,SEEK_END);
		Size = ftell(File);
		fseek(File,pos,SEEK_SET);
	}
	return Size;
}

