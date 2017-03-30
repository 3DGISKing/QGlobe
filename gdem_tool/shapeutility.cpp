//Shape_Utility.cpp
//2011 3 13 created by ugi

#include "shapedef.h"
#include <QFile>
#include <QDataStream>

//by RSH 2012.5.6
#include <QDir>
#include <stdio.h>
#include "../gdem_gis/GIS_funcs.h"
#include <QRectF>
#include <QFileInfo>
#include <QMessageBox>

#define MAX_TOLERANCE_PIXNUM_FOR_SHP 5

long _ConvertBigToLittle(long nBigInt)
{
	unsigned long uBuf;

	uBuf = 0x00000000;
	uBuf = uBuf | ((nBigInt >> 24) & 0x000000FF);
	uBuf = uBuf | ((nBigInt >> 8) & 0x0000FF00);
	uBuf = uBuf | ((nBigInt << 8) & 0x00FF0000);
	uBuf = uBuf | ((nBigInt << 24) & 0xFF000000);

	return (long)uBuf;
}

void ReadDataHeader(FILE * i_pfi, struct AVF_HDR * avHdr){

	double dData;
	long lData;

	fseek(i_pfi,0L,SEEK_SET);

	fread(&lData, sizeof(long), 1, i_pfi);
	avHdr->nFileCode = lData;

	fread(&lData, sizeof(long), 1, i_pfi);
	fread(&lData, sizeof(long), 1, i_pfi);
	fread(&lData, sizeof(long), 1, i_pfi);
	fread(&lData, sizeof(long), 1, i_pfi);
	fread(&lData, sizeof(long), 1, i_pfi);

	fread(&lData, sizeof(long), 1, i_pfi);
	avHdr->nFileLength = lData;

	fread(&lData, sizeof(long), 1, i_pfi);
	avHdr->nVer = lData;

	fread(&lData, sizeof(long), 1, i_pfi);
	avHdr->nShapeType = lData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Xmin = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Ymin = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Xmax = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Ymax = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Zmin = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Zmax = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Mmin = dData;

	fread(&dData, sizeof(double), 1, i_pfi);
	avHdr->Mmax = dData;


	avHdr->nFileCode = _ConvertBigToLittle(avHdr->nFileCode);
	avHdr->nFileLength = _ConvertBigToLittle(avHdr->nFileLength);

}

void ReadAvPolyHdr(FILE * i_pfi,struct AV_POLY_HDR * hdr){
	double dData;
	long lData;
	fread(&dData, sizeof(double), 1, i_pfi);
	hdr->BXmin = dData;
	fread(&dData, sizeof(double), 1, i_pfi);
	hdr->BYmin = dData;
	fread(&dData, sizeof(double), 1, i_pfi);
	hdr->BXmax = dData;
	fread(&dData, sizeof(double), 1, i_pfi);
	hdr->BYmax = dData;
	fread(&lData, sizeof(long), 1, i_pfi);
	hdr->nPartCount = lData;
	fread(&lData, sizeof(long), 1, i_pfi);
	hdr->nPointCount = lData;
}
void ReadAvPoint(FILE * i_pfi, struct AV_POINT * pt){
	double dData;
	fread(&dData, sizeof(double), 1, i_pfi);
	pt->x = dData;
	fread(&dData, sizeof(double), 1, i_pfi);
	pt->y = dData;
}

void ReadAvDataRecHdr(FILE * i_pfi,struct AV_DATA_REC_HDR * avDRhdr){
	long lData;
	fread(&lData, sizeof(long), 1, i_pfi);
	avDRhdr->nRecNum = lData;
	fread(&lData, sizeof(long), 1, i_pfi);
	avDRhdr->nContentLength = lData;
	fread(&lData, sizeof(long), 1, i_pfi);
	avDRhdr->nShapeType = lData;
}

QVector<AV_POINT> GetPointFileData(FILE * i_pfi, struct AVF_HDR * hd)
{
	struct AV_DATA_REC_HDR avDRhdr;
	struct AV_POINT _pt;
	int _k =0;

	QVector<AV_POINT> _pPointTheme;
	while (!feof(i_pfi))
	{
		ReadAvDataRecHdr(i_pfi, &avDRhdr);
		if (avDRhdr.nShapeType != AV_POINT_SHP) 
		{
			continue;		
		}
		ReadAvPoint(i_pfi, &_pt);
		_pPointTheme.append(_pt);
		_k += 1;
	}
	return _pPointTheme;
}

QVector<AV_POLYGON> GetPolyFileData(FILE * i_pfi, struct AVF_HDR * hd)
{
	long lData;
	struct AV_POLY_HDR hdr;
	struct AV_DATA_REC_HDR avDRhdr;
	struct AV_POINT _pt;
	QVector<AV_POLYGON> _pPolygonTheme;

	int _k =0;
	int _i;
	long * nFirstPTinPart;
	long * nNumPerPart;
	while (!feof(i_pfi))
	{
		ReadAvDataRecHdr(i_pfi, &avDRhdr);
		if ((avDRhdr.nShapeType != AV_POLYGON_SHP) && (avDRhdr.nShapeType != AV_POLYLINE_SHP))
		{
			continue;		
		}
		ReadAvPolyHdr(i_pfi, &hdr);
		nFirstPTinPart = new long [hdr.nPartCount];
		nNumPerPart = new long [hdr.nPartCount];

		AV_POLYGON * pPoly;
		pPoly = new AV_POLYGON [hdr.nPartCount];

		for (_i = 0; _i < hdr.nPartCount; _i++)
		{
			fread(&lData, sizeof(long), 1, i_pfi);
			nFirstPTinPart[_i] = lData;
		}
		for (int i=0; i<hdr.nPartCount; i++)
		{
			long fPtPos;
			if (i == hdr.nPartCount-1)
				fPtPos = hdr.nPointCount;
			else
				fPtPos = nFirstPTinPart[i+1];
			nNumPerPart[i] = fPtPos - nFirstPTinPart[i];
			pPoly[i].pPoints = new AV_POINT [nNumPerPart[i]];
		}

		int k=0;
		for (_i = 0; _i < hdr.nPointCount; _i++)
		{
			ReadAvPoint(i_pfi, &_pt);
			for (int tt = 0; tt< hdr.nPartCount; tt++)
			{
				if (nFirstPTinPart[tt] <= _i)
				{
					k = tt;
				}
			}
			pPoly[k].polyHd.BXmax = hdr.BXmax;
			pPoly[k].polyHd.BXmin = hdr.BXmin;
			pPoly[k].polyHd.BYmax = hdr.BYmax;
			pPoly[k].polyHd.BYmin = hdr.BYmin;
			pPoly[k].polyHd.nPartCount = 1;
			pPoly[k].polyHd.nPointCount = nNumPerPart[k];

			pPoly[k].pPoints[_i - nFirstPTinPart[k]].x = _pt.x;
			pPoly[k].pPoints[_i - nFirstPTinPart[k]].y = _pt.y;

		}
		for (int i=0; i<hdr.nPartCount; i++)
		{
			_pPolygonTheme.append(pPoly[i]);
		}

		_k += 1;
		delete [] nFirstPTinPart;
		delete [] nNumPerPart;
	}
	return _pPolygonTheme;
}
int GetShapeFileType(QString srcFileName)
{
	FILE * _pfi;

	if ((_pfi = _wfopen(srcFileName.utf16(), L"rb"))==NULL)
		return 0;

	struct AVF_HDR hd;
	ReadDataHeader(_pfi, &hd);
	return hd.nShapeType;
	fclose(_pfi);
}
void ConvertShapeFile(QString srcFileName, QString dstDirStr,  int level, bool hasID, int tolPixNum, int maxFSize, int maxNum)
{
	QVector<AV_POINT> pPointFile;
	QVector<AV_POLYGON> pPolyFile;
	struct AVF_HDR hd;

	FILE * _pfi;

	if ((_pfi = _wfopen(srcFileName.utf16(), L"rb"))==NULL)
		return;
	
	ReadDataHeader(_pfi, &hd);
	if (hd.nShapeType == AV_POINT_SHP)
	{
		//pPointFile = GetPointFileData(_pfi, &hd);
	}
	else if ((hd.nShapeType == AV_POLYGON_SHP) || (hd.nShapeType == AV_POLYLINE_SHP))
	{
		pPolyFile = GetPolyFileData(_pfi, &hd);
		ProcessPolyData(pPolyFile, &hd, dstDirStr, level, hasID, tolPixNum, maxFSize, maxNum);

		PostProcessPolyData(pPolyFile);
	}
	fclose(_pfi);
}

void ProcessPolyData(QVector<AV_POLYGON> pPolyData, struct AVF_HDR * hd, QString dstDir, int level,
					 bool hasID, int tolPix, int maxFSize, int maxNum)
{	
	int startX, endX, startY, endY;
	
	double tileWidth = 360.0f / (1 << level);//¼ÑÊÌ¼ê ¼ÓÌ¡Ëæº· À°ËÎÌ© ²¸¹¾°Î¼³

	//Áç¶®°Ø¹ÉµÛËË ½Ó¼èÂ×²÷ À°ËÎ´ÉÌ© ÃÔ ·ñ µÛ¸ÜÂö ±Ëº¤ 
	startX = (int)((hd->Xmin + 180.0f)/tileWidth); 
	endX = (int)((hd->Xmax + 180.0f)/tileWidth) + 1;
	startY = (int)((hd->Ymin + 180.0f)/tileWidth);
	endY = (int)((hd->Ymax + 180.0f)/tileWidth) + 1;

	//Áç¶®°Ø¹ÉµÛËË ½Ó¼èÂÚ ·ô±¶ À°ËÎËæ ´ÝÂ×ÊÞ  
	for (int nY = startY; nY < endY; nY++)
	{
		for (int nX = startX; nX < endX; nX++)
		{
			double minX, maxX, minY, maxY;

			//À°ËÎÌ© °Ò±Ë¼°´É ±Ëº¤
			minX = tileWidth * nX - 180.0f;
			minY = tileWidth * nY - 180.0f;
			maxX = minX + tileWidth;
			maxY = minY + tileWidth;

			CGDM_LocationPolygonArr oneTilePolyArr;
			QVector<int>   idArr;    
			oneTilePolyArr.clear();
			idArr.clear();
			QVector<CGDM_Rect2D> boundingArr;

			//¼­¾Ë Áç¶®°ØËæ ´ÝÂ×ÊÞ 
			for (int i=0; i<pPolyData.size(); i++)
			{
				CGDM_LocationArr inPtArr;
				CGDM_LocationPolygonArr outLineArr;
				AV_POLYGON poly = pPolyData.at(i);
				
				//Áç¶®°ØÌ© °Ò±Ë4°¢ÂôÌ© ·²¼¬ËË ÊÌÅð ÂÙ±ËËËÂ×ËË·² 
				if (((poly.polyHd.BXmax - poly.polyHd.BXmin) < tolPix*tileWidth/256.0f) &&
						((poly.polyHd.BYmax - poly.polyHd.BYmin) < tolPix*tileWidth/256.0f))
					continue;
				
				//ËË Áç¶®°ØËË Âï¼õ À°ËÎ±á º¡±Ù·²  
				if (isIntersecting(poly.polyHd.BXmin, poly.polyHd.BXmax, poly.polyHd.BYmin, poly.polyHd.BYmax, minX, maxX, minY, maxY))
				{
					//Áç¶®°ØË¾ °éºÂÂ×²÷ ·º´Ç ¼°´ÉË¾ ¹ÉµÛµá ÊÐ±¨
					for(int k=0; k<poly.polyHd.nPointCount; k++)
					{
						AV_POINT pt;
						pt.x = poly.pPoints[k].x; pt.y = poly.pPoints[k].y;
						CGDM_Location3D pPt;
						pPt.set(pt.x, pt.y, 0);
						inPtArr.push_back(pPt);
					}

					//ËË Áç¶®°ØË¾ À°ËÎÌ© °Ò±Ë4°¢ÂôËºµá »ô¶£±¨ 
					gdem_GetLineArrInRectangle(inPtArr, minX, maxX, minY, maxY, outLineArr);

					//»ùµ¹¼ê Áç¶®°Ø´ÉË¾ ¸êºãËæ ¸ó±ãÂ×±¨
					for (int nLine=0; nLine<outLineArr.size(); nLine++)
					{
						CGDM_LocationArr line = outLineArr[nLine];
						oneTilePolyArr.push_back(line);

						//»ùµ¹¼ê ·ô±¶ É·°¢´ÉÌ© id¶¦ ¸ó±ã 

						idArr.push_back(i+1);//id²÷ 1¹¢À¾ »¤»õ´ô³Þ°Ö °¡¼³ÂÙ³Þ.
					}
				}
			}
			if (oneTilePolyArr.size() > maxNum)
			{
				QMessageBox msg;
				msg.setText("Polygon array has too many features!");
				msg.exec();
				continue;
			}
			if (oneTilePolyArr.size() > 0)
				WriteShapeFile(nX, nY, level, oneTilePolyArr, dstDir,idArr, hasID, maxFSize);
		}
	}
}

//added by RSH for 2013.3.3 for bounding box
void GetBoundingRect(CGDM_LocationArr ptArr, CGDM_Rect2D *bound)
{
	double minX, maxX, minY, maxY;
	for (int i=0; i<ptArr.size(); i++)
	{
		CGDM_Location3D pt;
		pt.m_tX = ptArr[i].m_tX;
		pt.m_tY = ptArr[i].m_tY;

		if (i == 0)
		{
			minX = maxX = pt.m_tX;
			minY = maxY = pt.m_tY;
		}
		else
		{
			minX = qMin(pt.m_tX, minX);
			maxX = qMax(pt.m_tX, maxX);
			minY = qMin(pt.m_tY, minY);
			maxY = qMax(pt.m_tY, maxY);
		}
	}
	bound->m_cMinPt.m_tX = minX;
	bound->m_cMinPt.m_tY = minY;
	bound->m_cMaxPt.m_tX = maxX;
	bound->m_cMaxPt.m_tY = maxY;
	return;
}
//end addition by RSH

void PostProcessPolyData(QVector<AV_POLYGON> pPolyData)
{
	for (int i=0; i<pPolyData.size(); i++)
	{
		AV_POLYGON poly;
		poly = pPolyData.at(i);
		if (poly.polyHd.nPointCount > 0)
			delete [] poly.pPoints;
	}
}
bool isIntersecting(double Xmin, double Xmax, double Ymin, double Ymax, double minX, double maxX, double minY, double maxY)
{
	QRectF rect1;
	rect1.setBottom(Ymin); rect1.setTop(Ymax);
	rect1.setLeft(Xmin); rect1.setRight(Xmax);
	QRectF rect2;
	rect2.setBottom(minY); rect2.setTop(maxY);
	rect2.setLeft(minX); rect2.setRight(maxX);
	return rect1.intersects(rect2);
}
QString MakeFileName(int nX, int nY, int level, QString extension)
{
	QString res;
	res.setNum(level);
	if (level > 7)
		res = res + "/" + QString().setNum((int)(nX/128)) + "_" + QString().setNum((int)(nY/128)) + "_" + QString().setNum(level);

	res = res + "/" + QString().setNum(nX) + "_" + QString().setNum(nY) + "_" + QString().setNum(level) + "." + extension;
	return res;
}


QString getDirName(QString filename)
{
	int i=filename.lastIndexOf("/");
	filename.remove(i,filename.length()-i);
	return filename;
}
bool WriteShapeFile(int x,int y,int level, CGDM_LocationPolygonArr polyarray, QString dstDir,QVector<int> idArr, 
					bool hasID, int maxFSize)
{
	int MAX_ALLOWED_FILESIZE_IN_KB = maxFSize;
	CGDM_LocationPolygonArr polygonarray;
	if (level < 11)
		polygonarray = GetReducedPolygonArray(polyarray, level);
	else
		polygonarray = polyarray;

	QDir tmp;
	QString filename = MakeFileName(x, y, level, "shp");
	filename = dstDir + "/" + filename;
	QString dirName = getDirName(filename);

	if(!tmp.exists(dirName))    //°æ·Î¸¦ »ý¼ºÇÑ´Ù
		tmp.mkpath(dirName);


	QFile file;
	file.setFileName(filename);
	if(!file.open(QIODevice::WriteOnly)) 	return false; //»õ·Î¿î ÆÄÀÏÀ» Ã¢Á¶ÇÑ´Ù

	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);
	
	int fileSize = 0;
	out<<(int)0;
	fileSize = fileSize + sizeof(int);
	out<<(int)polygonarray.size();                            //ÆÄ¶ó¸ÞÅÍ·Î ¹ÞÀº Æú¸®°ï¸¸ ¾²´Â °æ¿ì
	fileSize = fileSize + sizeof(int);
	CGDM_Location3D point;                                       //¸ÕÀú ÆÄ¶ó¸ÞÅÍ·Î ¹ÞÀº Æú¸®°ï¸¸ ¾´´Ù

	double convertx,converty;

	//debug
	if(polyarray.size()!=idArr.size())
	{
		qDebug("bug");
	}
	//end
	for(unsigned int npoly = 0; npoly < polygonarray.size(); npoly++)
	{
		CGDM_LocationArr &polygon = polygonarray[npoly];

			
		out<<(int)polygon.size();
		fileSize = fileSize + sizeof(int);
		//2012 6 12 added by ugi ShapeÁÄËÎÌ© °é¼¿ ¸êÃù
		if (hasID)
			out<<(int)idArr[npoly];
		fileSize = fileSize + sizeof(int);
		//end
		//Added by RSH 2013.3.3 for bounding box
		CGDM_Rect2D bound;
		GetBoundingRect(polygon, &bound);

		out<<(double)bound.m_cMinPt.m_tX*PAI/180.0f;
		out<<(double)bound.m_cMinPt.m_tY*PAI/180.0f;
		out<<(double)bound.m_cMaxPt.m_tX*PAI/180.0f;
		out<<(double)bound.m_cMaxPt.m_tY*PAI/180.0f;
		fileSize = fileSize + 4*sizeof(double);
		//end addition

		for(unsigned int npoint=0;npoint<polygon.size();npoint++)
		{
			point=polygon[npoint];
			convertx=PAI*/*Added by RSH 2013.3.3*/point.m_tX/180.0f;
			converty=PAI*/*Added by RSH 2013.3.3*/point.m_tY/180.0f;
			out<<(double)convertx;
			out<<(double)converty;
			fileSize = fileSize + 2*sizeof(double);
		}
	}
	file.close();
	if (fileSize > 1024*MAX_ALLOWED_FILESIZE_IN_KB)
	{
		QMessageBox msgBox;
		msgBox.setText("File size exceeded!");
		msgBox.exec();
		QDir tmp(dstDir);
		tmp.remove(filename);
	}
	return true;
}

//¼ÑÊÌ¼ê ¼ÓÌ¡Ëæº· Ì©Ì©°¡ ÊÖ²÷ (²¸·Ë »õË¼ ±¬ËË¶¦ °¡¼è²÷ )¸ê´ÉË¾ ½£°ºÂÙ³Þ.
CGDM_LocationPolygonArr GetReducedPolygonArray(CGDM_LocationPolygonArr inArray, int level)
{
	CGDM_LocationPolygonArr res;
	double num_pix_tol;
	if (level < 7)
		num_pix_tol = 3.0f;
	else if (level < 9)
		num_pix_tol = 2.0f;
	else if (level < 11)
		num_pix_tol = 1.0f;
	else
		num_pix_tol = 0.0f;

	double dist_tolerance = num_pix_tol*360.0f/(1<<level)/256;	//distance tolerance is half pixel width.
	double angle_tolerance = 0.02;					    //arccos(1-0.02) is about 11 degree.
	if (level > 4)
		angle_tolerance = angle_tolerance/(level - 4);

	//¼­¾Ë Áç¶®°ØËæ ´ÝÂ×ÊÞ 
	for (int nPoly=0; nPoly<inArray.size(); nPoly++)
	{
		CGDM_LocationArr &polygon = inArray[nPoly];
		CGDM_LocationArr line;
		CGDM_Location3D pPt, oldPt, upPt;
		CGDM_Location3D fVector, sVector;
		
		//Áç¶®°ØÌ© ¼­¾Ë ¼°Ëæ ´ÝÂ×ÊÞ 
		for (int nPoint=0; nPoint<polygon.size()-1; nPoint++)
		{
			pPt = polygon[nPoint];
			upPt = polygon[nPoint+1];
			if (nPoint == 0)               //First point should be included.
			{
				line.push_back(pPt);
				oldPt = pPt;
			}
			else if (nPoint == polygon.size()-2) //Last point was assigned to be involved necessarily.
				line.push_back(upPt);

			double distance = pPt.getDistanceFrom(oldPt);
			CGDM_Location3D fVector = oldPt - pPt;        //Vector pointing from present point to former one.
			CGDM_Location3D sVector = upPt  - pPt;		   //Vector pointing from present point to next one.
			fVector.normalize(); sVector.normalize();
			double cos_angle = 1 - fabs(fVector.dotProduct(sVector));
			cos_angle = 1.0f;								//Presently disable angular judgement. 

			//´¸¼°º¡ËËÌ© °º¶®Ì® Curve¼³´ª¶¦ ±Ëº¤Â×ÊÞ ÁÆ³à
			if ((distance > dist_tolerance) && (cos_angle > angle_tolerance))  //Judgement part:
			{
				line.push_back(pPt);
				oldPt = pPt;
			}
		}
		res.push_back(line);
	}
	return res;
}


bool ConvertShapeToGS(QString shpFileDir, QString saveDir)
{
	QDir srcDir(shpFileDir);
	if (!srcDir.exists())
		return false;
	QFileInfoList dirList = srcDir.entryInfoList();
	for (int i=0; i<dirList.size(); i++)
	{
		QFileInfo dirInfo = dirList.at(i);
		if (dirInfo.isDir())
		{
			QString dirStr = dirInfo.absoluteFilePath();
			if (dirStr.lastIndexOf("/") == -1)
				return false;
			QString dirName = dirStr.mid(dirStr.lastIndexOf("/") + 1);
			if ((dirName == ".") || (dirName == ".."))
				continue;
			bool ok;
			int level = dirName.toInt(&ok, 10);
			if (!ok)
				return false;
			QDir subDir;
			subDir.setPath(dirStr);
			QStringList srcShpFileList;
			if (level < 8)
			{
				srcShpFileList = subDir.entryList(QStringList("*.shp"), QDir::Files | QDir::NoSymLinks);
				if (srcShpFileList.size() < 1)
					return false;
				QString saveDirStr = saveDir + "/" + dirName;
				QString saveFileName = saveDirStr + "/0_0_" + QString().setNum(level) + ".gs";
				if (!WriteOneBlockShapeFile(dirStr, srcShpFileList, saveFileName)) continue;
			}
			else
			{
				QFileInfoList subDirList = subDir.entryInfoList();
				for (int k=0; k<subDirList.size(); k++)
				{
					QFileInfo subDirInfo = subDirList.at(k);
					if (subDirInfo.isDir())
					{
						QString subDirStr = subDirInfo.absoluteFilePath();
						if (subDirStr.lastIndexOf("/") == -1)
							return false;
						QString subDirName = subDirStr.mid(subDirStr.lastIndexOf("/") + 1);
						if ((subDirName == ".") || (subDirName == ".."))
							continue;
						QDir subSubDir;
						subSubDir.setPath(subDirStr);
						srcShpFileList = subSubDir.entryList(QStringList("*.shp"), QDir::Files | QDir::NoSymLinks);
						QString saveDirStr = saveDir + "/" + dirName + "/" + subDirName + ".gs";
						if (!WriteOneBlockShapeFile(subDirStr, srcShpFileList, saveDirStr)) return false;
					}
				}
			}
		}
	}
	return true;
}
bool WriteOneBlockShapeFile(QString srcShpDirStr, QStringList srcFList, QString saveDirStr)
{
	int Xmin, Ymin, level;
	QString sfName = saveDirStr.mid(saveDirStr.lastIndexOf("/") + 1);
	Xmin = sfName.split("_").at(0).toInt();
	Ymin = sfName.split("_").at(1).toInt();
	QString lvlStr = sfName.split("_").at(2);
	level = lvlStr.left(lvlStr.indexOf(".")).toInt();
	int nRowCount;
	if (level < 7)
		nRowCount = 1<<level;
	else
		nRowCount = 128;
	int * foffset_size;
	foffset_size = new int [2 + 2*nRowCount*nRowCount];
	foffset_size[0] = level;
	//foffset_size[1] = srcFList.size();
	foffset_size[1] = nRowCount*nRowCount;

	int tot_offset = 4 + 4 + 8*nRowCount*nRowCount;
	Xmin = Xmin*nRowCount; Ymin = Ymin*nRowCount;
	for (int nY = Ymin; nY < Ymin + nRowCount; nY++)
	{
		for (int nX = Xmin; nX < Xmin + nRowCount; nX++)
		{
			int inxFile = (nY - Ymin)*nRowCount + (nX - Xmin);
			QString oneFName = QString().setNum(nX) + "_" +QString().setNum(nY) + "_" + QString().setNum(level) + ".shp";
			QString fPath = srcShpDirStr + "/" + oneFName;
			QFile oneShpFile(fPath);
			if (oneShpFile.exists())
			{
				foffset_size[2 + 2*inxFile] = tot_offset;
				foffset_size[2 + 2*inxFile + 1] = oneShpFile.size();
				tot_offset = tot_offset + foffset_size[2 + 2*inxFile + 1];	
			}
			else
			{
				foffset_size[2 + 2*inxFile] = 0;
				foffset_size[2 + 2*inxFile + 1] = 0;
			}
		}
	}
	if(QFile::exists(saveDirStr))
		QFile::remove(saveDirStr);

	QString saveFolderStr = saveDirStr.left(saveDirStr.lastIndexOf("/"));
	QDir saveDir(saveFolderStr);
	if (!saveDir.exists())
		saveDir.mkpath(saveFolderStr);

	QFile gsDstFile(saveDirStr);
	if(!gsDstFile.open(QIODevice::WriteOnly))
		return false;
	unsigned char * header = NULL;
	int writeSize = 0;
	writeSize = (2 + 2*nRowCount*nRowCount) * sizeof(int);
	header = new unsigned char [writeSize];
	memcpy(header, foffset_size, writeSize);
	gsDstFile.write((const char*)header, writeSize);
	delete [] header;

	for (int nY = Ymin; nY < Ymin + nRowCount; nY++)
	{
		for (int nX = Xmin; nX < Xmin + nRowCount; nX++)
		{
			int inx = (nY - Ymin)*nRowCount + (nX - Xmin);
			QString oneShpName = QString().setNum(nX) + "_" +QString().setNum(nY) + "_" + QString().setNum(level) + ".shp";
			QString fShpPath = srcShpDirStr + "/" + oneShpName;
			writeSize = foffset_size[2 + 2*inx + 1];
			if (writeSize > 0)			
			{
				QFile oneShpFile(fShpPath);
				if(!oneShpFile.open(QIODevice::ReadOnly)) return false;
				unsigned char * dataPointer = NULL;
				dataPointer = oneShpFile.map(0, writeSize);
				unsigned char * writeData = NULL;
				writeData = new unsigned char [writeSize];
				memcpy(writeData, dataPointer, writeSize);
				gsDstFile.write((const char *)writeData, writeSize);
				oneShpFile.unmap(dataPointer);
				delete [] writeData;
				oneShpFile.close();
			}
		}
	}
	delete [] foffset_size;
	gsDstFile.close();
	return true;
}
//end of RSH


bool ReadPolylineFromShapeFile(QString filename,PolyLine& polyline)
{
	QFile shapefile(filename);

	if(!shapefile.open(QIODevice::ReadOnly)) return false;

	if(!polyline.IsEmpty()) 
		polyline.Clear();

	QDataStream in(&shapefile);
	in.setByteOrder(QDataStream::LittleEndian);

	int unknown,linenum;

        in>>(int&)unknown;
        in>>(int&)linenum;

	int pointnum;
	Point point;
	Line line;
	double x,y;
	for(int i=0 ;i<linenum;i++)
	{
                in>>(qint32&)pointnum;
		for(int j=0;j<pointnum;j++)
		{
                        in>>(double&)x;
                        in>>(double&)y;
			point.Set(x,y);
			line.AddPoint(point);
		}
		polyline.AddLine(line);
	}

	shapefile.close();
	return true;
}

bool WriteShapeFile(QString filename,PolyLine polyline, bool hasID)
{
	if(polyline.GetLineCount()<1) return false;

	QFile file;
	file.setFileName(filename);
	if(!file.open(QIODevice::WriteOnly)) 	return false;

	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);

	out<<(int)0;
	out<<(int)polyline.GetLineCount();

	Line line;

	double x,y;
	for(int i=0;i<polyline.GetLineCount();i++)
	{
		line=polyline.GetLine(i);	
		int point_count=line.GetPointCount();
		out<<(int)point_count;
		for(int j=0;j<point_count;j++)
		{
			x = line.GetPoint(j).X();
			y = line.GetPoint(j).Y();
			out<<x;
			out<<y;
		}
	}
	file.close();
	return true;
}

bool ReadShapeFile(QString filename,CGDM_LocationPolygonArr& linearray)
{
	QFile shapefile(filename);

	if(!shapefile.open(QIODevice::ReadOnly)) return false;

	if(!linearray.empty()) 
		linearray.clear();

	QDataStream in(&shapefile);
	in.setByteOrder(QDataStream::LittleEndian);

	int unknown,linenum;

        in>>(int&)unknown;
        in>>(int&)linenum;

	int pointnum;
	CGDM_Location3D point;
	CGDM_LocationArr line;

	for(int i=0 ;i<linenum;i++)
	{
                in>>(int&)pointnum;
		for(int j=0;j<pointnum;j++)
		{
                        in>>(double&)point.m_tX;
                        in>>(double&)point.m_tY;
			line.push_back(point);
		}
		linearray.push_back(line);
		line.clear();
	}

	shapefile.close();
	return true;
}

bool WriteShapeFile(int x,int y,int level, CGDM_LocationPolygonArr polygonarray, bool hasID)
{
	QString maintype="";

	QString filename;	

	filename = "";

	CGDM_LocationPolygonArr polygonarray_on_disk;

	QFile file;
	file.setFileName(filename);
	if(!file.open(QIODevice::WriteOnly)) 	return false;

	QDataStream out(&file);
	out.setByteOrder(QDataStream::LittleEndian);

	out<<(int)0;
	out<<(int)polygonarray.size()+polygonarray_on_disk.size();

	CGDM_Location3D point;
	double convertx,converty;
	for(unsigned int npoly = 0; npoly < polygonarray.size(); npoly++)
	{
		CGDM_LocationArr &polygon = polygonarray[npoly];

		out<<(int)polygon.size();
		for(unsigned int npoint=0;npoint<polygon.size();npoint++)
		{
			point=polygon[npoint];
			convertx=point.m_tX/PAI;
			converty=point.m_tY/PAI;
			out<<(double)convertx;
			out<<(double)converty;
		}
	}

	for(unsigned int npoly = 0; npoly < polygonarray_on_disk.size(); npoly++)
	{
		CGDM_LocationArr &polygon = polygonarray_on_disk[npoly];

		out<<(int)polygon.size();
		for(unsigned int npoint=0;npoint<polygon.size();npoint++)
		{
			point=polygon[npoint];
			out<<(double)point.m_tX;
			out<<(double)point.m_tY;
		}
	}
	file.close();
	return true;
}
