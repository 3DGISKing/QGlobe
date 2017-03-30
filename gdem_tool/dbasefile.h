#pragma once

#include <QList>
#include <QString>
#define DBASE_FILE_SUFFIX        ".dbf"

#define DBASE_FILE_HEADER_SIZE   32
#define FIELD_DESCRIPTOR_SIZE    32

#pragma  pack(1)

struct  DBaseFileHeader
{
	unsigned char _FileCode;
	unsigned char _UpdatedYear;
	unsigned char _UpdatedMonth;
	unsigned char _UpdatedDay;
	unsigned int  _NumberOfRecords;

	unsigned short _HeaderLength;
	unsigned short _RecordLength;
};

struct DBaseFieldDescriptor
{
	unsigned char _Name[11];
	unsigned char _DataType;
	unsigned char _Unused1[4];
	unsigned char _FieldLength;
	unsigned char _IsDecimal;
	unsigned char _Unused2[14];
};

#pragma  pack()

class DBaseFile;

class Record
{
	friend class DBaseFile;
	QList<QString> _ValueList;
public:
	void AddValue(QString val) {_ValueList.push_back(val);};
	QString GetFieldValue(int i)
	{
		if (i >= _ValueList.count()) return "";	
		return _ValueList.at(i);
	}
};

class DBaseFile
{
public:	
	QList<Record*>        _RecordSet;            
	unsigned int          _NumberOfFiled;
	DBaseFieldDescriptor* _Fields;
	unsigned char*        _pDbfFileData;
	DBaseFileHeader*      _pDBaseFileHeader;
	
	QString               _TableName;

	DBaseFile(QString dBaseFileName);
	~DBaseFile(void);
};
