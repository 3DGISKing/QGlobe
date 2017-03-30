#include <QString>
#include <QFile>
#include "DBaseFile.h"

#define SQL_CRATE_TABLE      "CREATE TABLE "
#define DOT                  "'"
#define SPACE                " "
#define LEFT_PARENTHESIS     "("
#define RIGHT_PARENTHESIS    ")"


DBaseFile::DBaseFile(QString dBaseFileName)
{
	_pDbfFileData=NULL;

	//read raw dbf file data
	QFile dBaseFile(dBaseFileName);
	
	if(!dBaseFile.open(QIODevice::ReadOnly)) return;

	unsigned char* lpData = NULL;

    lpData=dBaseFile.map(0,dBaseFile.size());

	if (lpData != NULL)
	{
		_pDbfFileData=new unsigned char[dBaseFile.size()];
		memcpy(_pDbfFileData, lpData, dBaseFile.size());
		dBaseFile.unmap(lpData);
	}
	

	_pDBaseFileHeader=(DBaseFileHeader*)_pDbfFileData;

	if(_pDBaseFileHeader->_FileCode>5)		return;

	_NumberOfFiled=(_pDBaseFileHeader->_HeaderLength-DBASE_FILE_HEADER_SIZE)/FIELD_DESCRIPTOR_SIZE;
	
	_Fields=(DBaseFieldDescriptor*)(_pDbfFileData+DBASE_FILE_HEADER_SIZE);

	Record *pRecord; 
	unsigned int offset=_pDBaseFileHeader->_HeaderLength+1;

	for(unsigned int i=0;i<_pDBaseFileHeader->_NumberOfRecords;i++)
	{
		pRecord=new Record();
		DBaseFieldDescriptor *pFieldDescriptor=_Fields;

		for(unsigned int j=0;j<_NumberOfFiled;j++)
		{
			char *asciidata=new char[pFieldDescriptor->_FieldLength+1];

			memcpy(asciidata,_pDbfFileData+offset,pFieldDescriptor->_FieldLength);
			asciidata[pFieldDescriptor->_FieldLength]='\0';
			offset+=pFieldDescriptor->_FieldLength;

			QString strdata;
			strdata=strdata.fromLocal8Bit(asciidata);

			delete [] asciidata;
			pRecord->AddValue(strdata);
			pFieldDescriptor++;
		}
		
		offset++;  //note that one space exists between records!
		_RecordSet.push_back(pRecord);
	}

/*	AttributeDBManager *pAttributeDBManager=new AttributeDBManager();

	if(!pAttributeDBManager->Connect()) return;


	//get table name
	int index=dBaseFileName.lastIndexOf("/");
	int len=dBaseFileName.length();

	_TableName=dBaseFileName.mid(index+1,len-index-5);

	//create table

	DBaseFieldDescriptor *pFieldDescriptor=_Fields;

	QString sql="CREATE TABLE "+_TableName+" (";
	sql+="'ID' INTEGER PRIMARY KEY,"; 

	QString fieldname, datatype;
	QString fieldlist;

	for(unsigned int j=0;j<_NumberOfFiled;j++)
	{
		fieldname=fieldname.fromAscii((char*)pFieldDescriptor->_Name);
		fieldlist+=DOT+fieldname+DOT;
		if(pFieldDescriptor->_DataType=='C')
		{
			QString temp;
			temp.setNum(pFieldDescriptor->_FieldLength);
			datatype="VARCHAR( "+temp+" )";
		}	
		else if(pFieldDescriptor->_DataType=='N')
			datatype="BOOLEAN";
		else if(pFieldDescriptor->_DataType=='D')
			datatype="DATE";
		else if(pFieldDescriptor->_DataType=='F' || pFieldDescriptor->_DataType=='N')
		{
			if(pFieldDescriptor->_IsDecimal>0)
				datatype="DOUBLE";	
			else
				datatype="INTEGER";	
		}
		
		sql+=DOT+fieldname+DOT+SPACE+datatype;	
		if(j!=_NumberOfFiled-1)	
		{
			sql+=",";
			fieldlist+=",";
		}
		pFieldDescriptor++;
	}

	sql=sql+RIGHT_PARENTHESIS;

	pAttributeDBManager->ExecuteSQL(sql);
	
	QSqlQuery query;

	for(int i=0;i<_RecordSet.size();i++)
	{
		sql="INSERT INTO "+_TableName+" ( "+fieldlist+" ) "+" VALUES (";

		QString valuelist;
		Record* record=_RecordSet[i];
		for(int j=0;j<record->_ValueList.size();j++)
		{
			//processing special character
			QString refinedval=record->_ValueList[j].replace(DOT,QString(QChar('"')));
			valuelist+=DOT+refinedval+DOT;
			if(j!=record->_ValueList.size()-1)
				valuelist+=",";
		}
		sql=sql+valuelist+RIGHT_PARENTHESIS;
		//pAttributeDBManager->ExecuteSQL(sql);
		query.exec(sql);
	}*/
}

DBaseFile::~DBaseFile(void)
{
	delete [] _pDbfFileData;
}
