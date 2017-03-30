#include "key.h"

#define BUFFER_SIZE 1052
QString GetPhysicalDrive0ModelNumber()
{
#ifdef Q_OS_WIN
	HANDLE hDevice;               // handle to the drive to be examined 

	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive0"),  // drive 
		GENERIC_READ|GENERIC_WRITE,                // no access to the drive
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE, 
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,                // file attributes
		NULL);            // do not copy file attributes

	if(hDevice==INVALID_HANDLE_VALUE)
		return "GeoBrowser";

	BOOL bResult;                 // results flag

	SENDCMDINPARAMS bin;

	memset(&bin,0,sizeof(SENDCMDINPARAMS));
	bin.bDriveNumber=0;
	bin.cBufferSize=512;
	bin.irDriveRegs.bDriveHeadReg=0xA0;
	bin.irDriveRegs.bCommandReg=0xEC;
	bin.irDriveRegs.bSectorCountReg=1;
	bin.irDriveRegs.bSectorNumberReg=1;

	SENDCMDOUTPARAMS* bout;

	bout=(SENDCMDOUTPARAMS*)(new char[sizeof(SENDCMDOUTPARAMS)+BUFFER_SIZE]);

	memset(bout,0,sizeof(SENDCMDOUTPARAMS)+BUFFER_SIZE);

	DWORD br=0;
	bResult = DeviceIoControl
		(
		hDevice,               // device to be queried
		SMART_RCV_DRIVE_DATA,  // operation to perform
		&bin, 
		sizeof(SENDCMDINPARAMS), // no input buffer
		bout, sizeof(SENDCMDOUTPARAMS)+BUFFER_SIZE,     // output buffer
		(LPDWORD)&br,                 // # bytes returned
		(LPOVERLAPPED) NULL);  // synchronous I/O

	CloseHandle(hDevice);

	DWORD d=GetLastError();
	if(!bResult) 
		return "";

	char strmodelnumber[BUFFER_SIZE];

	int pos=0;

	int hddfr=55;
	int hddln=40;
	for(int i=hddfr-1;i<hddfr + hddln - 1;i=i+2)
	{
		if(bout->bBuffer[i+1]==0)
			break;

		strmodelnumber[pos]=bout->bBuffer[i+1];
		pos++;
		if(bout->bBuffer[i]==0)
			break;

		strmodelnumber[pos]=bout->bBuffer[i];
		pos++;
	}

	strmodelnumber[pos]=0;

	QString modelnumber=QString::fromAscii(strmodelnumber);

	delete bout;
	return modelnumber.trimmed();
#else
	return ""
#endif
}
QString GetKey(QString deviceInfo)
{
	QString serialnumber = deviceInfo;

	if(serialnumber.isEmpty()) return "";
	QString serialupper=serialnumber.toUpper();

	QByteArray bytedata=serialnumber.toAscii();

	char* data=serialupper.toAscii().data();

	int average=0;

	for(int i=0;i<bytedata.size();i++)
	{
		char ch=bytedata.at(i);
		average+=ch;
	}

	average/=bytedata.size();

	char ch1=average;

	QString key;

	for(int i=0;i<bytedata.size();i++)
	{
		int number=bytedata[i]-average;
		number=number/(i+1);

		number=number*3.141592;


		QString strnumber;
		strnumber.setNum(number);

		key+=strnumber;

		if(i<bytedata.size()-1)
		{
			int number2=bytedata[i]+bytedata[i+1];

			char ss[2];
			ss[0]=number2/2;
			ss[1]=0;

			QString q=QString::fromAscii(ss);

			key=key+q;
		}
	}

	return key;
}

QString FormatKey(QString strKey, int strSize)
{
	QByteArray byteStr = strKey.toAscii();

	QList<int> xx;
	for(int jj = 0; jj < byteStr.size(); jj++)
		xx << byteStr[jj];

	QList<int> yy;

	int result_sum = 0;
	int pow_part = 0;
	int num_part = 0;

	for(int jj = 0; jj < strSize; jj++) {
		for(int ii = 0; ii < xx.count(); ii++) {
			pow_part = (long)pow((double)xx[ii], (double)((jj % 3) + 1)) % 10;
			num_part = (ii + jj) % 10;
			result_sum += (pow_part * num_part) % 10;
		}
		yy << (int)result_sum % 16;
	}

	QString formatedKey;
	for(int kk = 0; kk < strSize; kk++)
		formatedKey.append(QString::number(yy[kk], 16).toUpper());

	return formatedKey;
}