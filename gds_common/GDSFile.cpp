#include "../GDSFile.h"
#include <string.h>

FILE* GDS_fileOpen(const char* filename, eGDSFileMode mode)
{
	FILE* fp = NULL;

	if( mode == eGDSFile_ReadOnly )
		fp = fopen(filename, "r");
	else if( mode == eGDSFile_Write )
		fp = fopen(filename, "w");

	return fp;
}

GDSINT GDS_fileRead(FILE* fp, char* data, GDSINT size)
{
	return fread(data, size, 1, fp);
}

GDSINT GDS_fileWrite(FILE* fp, const char* data, GDSINT size)
{
	return fwrite(data, size, 1, fp);
}

GDSINT	GDS_fileClose(FILE* fp)
{
	return fclose(fp);
}