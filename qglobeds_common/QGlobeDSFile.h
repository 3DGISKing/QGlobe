#ifndef __GDSFILE_H__
#define __GDSFILE_H__

#include <stdio.h>
#include <QGlobeDSType.h>

enum eGDSFileMode{
    eGDSFile_ReadOnly	= 0x01,
    eGDSFile_Write		= 0x02
};

FILE*	GDS_fileOpen(const char* filename, eGDSFileMode mode);
QGlobeDSINT GDS_fileRead(FILE* fp, char* data, QGlobeDSINT size);
QGlobeDSINT GDS_fileWrite(FILE* fp, const char* data, QGlobeDSINT size);
QGlobeDSINT	GDS_fileClose(FILE* fp);

#endif /* __GDSFILE_H__ */
