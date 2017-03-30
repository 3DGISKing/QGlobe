#ifndef __GDSLOG_H__
#define __GDSLOG_H__

#include "GDSType.h"
#include "GDSDefine.h"

#include <QString>

void InitializeLog(E_LOG_OUTPUT_TYPE a_LogType, E_LOG_OUTPUT_LEVEL a_LogLevel);
void FinalizeLog();

void GDSLogOutput(E_LOG_LEVEL a_LogLevel, IN const char *a_format, ...);

#endif // __GDSLOG_H__