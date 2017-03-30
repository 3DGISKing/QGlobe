#ifndef __GDSFILE_H__
#define __GDSFILE_H__

#include <stdio.h>
#include <GDSType.h>

enum eGDSFileMode{
    eGDSFile_ReadOnly	= 0x01,
    eGDSFile_Write		= 0x02
};

FILE*	GDS_fileOpen(const char* filename, eGDSFileMode mode);
GDSINT GDS_fileRead(FILE* fp, char* data, GDSINT size);
GDSINT GDS_fileWrite(FILE* fp, const char* data, GDSINT size);
GDSINT	GDS_fileClose(FILE* fp);

#endif /* __GDSFILE_H__ */
