#include "GDM_Camera.h"
#include "GDM_PyramidMgr.h"
#include "GeoTerrainSceneNode.h"
#include "GDM_TextureIDCache.h"
#include "GIS_TextureIDCache.h"
#include "GDM_TerrainPtCache.h"
#include "GDM_DataMgr.h"
#include "RasterTextureIDCache.h"

GDM_MESH_TILE_ARR g_gdm_MeshTileArr;

#define GDEM_MIN_RATE_DISP_LEVEL	8

unsigned char bc[GDEM_MAX_DEM_PT_COUNT][GDEM_MAX_DEM_PT_COUNT];
unsigned int  triangleindex;
unsigned int  fcP;
unsigned int  prevP;
unsigned int  gridSize;

unsigned short     * g_IndexBuffer;
unsigned int        IndicesToRender;

TerrainQuadTreeList *pWriteQuadList=NULL;
TerrainQuadTreeList *pReadQuadList=NULL;
TreeListSelector	*gdm_TileListSelector = NULL;	

QuadTree	    *g_pCurWriteTree = 0;
int              currIndex = -1;

void TerrainQuadTreeList::start()
{			
	m_nUsed = 0;		
	pIBuf = pIndexBuf;
	pVBuf = pVertexBuf;		
	LRUHead = 0;
	gdm_CameraGetLocation(gdm_GetDataMgr(),&m_CameraLocation);
}

void BeginFan(int i, int j)
{			
	triangleindex=1;
	fcP = j*gridSize + i;	
	g_pCurWriteTree->m_nBboxArray[currIndex].addInternalPoint(g_pCurWriteTree->m_pVertexBuf[fcP].m_tX,g_pCurWriteTree->m_pVertexBuf[fcP].m_tY,g_pCurWriteTree->m_pVertexBuf[fcP].m_tZ);
}

// so just add each vertex to a vertex buffer
void FanVertex(int i, int j)
{
	int index = j*gridSize + i;
	g_pCurWriteTree->m_nBboxArray[currIndex].addInternalPoint(g_pCurWriteTree->m_pVertexBuf[index].m_tX,g_pCurWriteTree->m_pVertexBuf[index].m_tY,g_pCurWriteTree->m_pVertexBuf[index].m_tZ);
	if (triangleindex >= 2)
	{
		 g_pCurWriteTree->m_pIndexBuf[IndicesToRender] = g_IndexBuffer[IndicesToRender]=fcP;IndicesToRender++;
		 g_pCurWriteTree->m_pIndexBuf[IndicesToRender] = g_IndexBuffer[IndicesToRender]=index;IndicesToRender++;
		 g_pCurWriteTree->m_pIndexBuf[IndicesToRender] = g_IndexBuffer[IndicesToRender]=prevP;IndicesToRender++;		
	}
	prevP = index;
	triangleindex++;	
}

void initBC(int ndivision)
{	
	for (int i = 0; i <= ndivision; i++)
		memset(bc[i],1,sizeof(char)*ndivision+1);
}

void initBC(GDM_MESH_TILE *pMeshTile)
{	
	initBC(pMeshTile->nDivision);
	int step = GDEM_MAX_DEM_DIVISION/pMeshTile->nDivision, index = 0;
	int sp1 = 0, sp2 = sp1+GDEM_MAX_DEM_DIVISION, sp3 = GDEM_MAX_DEM_DIVISION3-step, sp4 = GDEM_MAX_DEM_DIVISION4-step;
	for (int i = 0; i < pMeshTile->nDivision; i++)
	{		
		if (pMeshTile->mBU[sp1])		//bottom
			bc[i][0] = pMeshTile->mBC[sp1]; 

		if (pMeshTile->mBU[sp2])		//right
			bc[pMeshTile->nDivision][i] = pMeshTile->mBC[sp2];

		if (pMeshTile->mBU[sp3])		//top
			bc[i+1][pMeshTile->nDivision] = pMeshTile->mBC[sp3]; 

		if (pMeshTile->mBU[sp4])		//left
			bc[0][i+1] = pMeshTile->mBC[sp4]; 

		sp1+=step;
		sp2+=step;
		sp3-=step;
		sp4-=step;
	}	
}

// geomorph and pipe the triangulation
void pipemap(const int i,const int j,const int s,int index)
{
	int s2,s4;
	int bc1,bc2,bc3,bc4;

	int chi1,chi2,chi3,chi4, chi;		
	
	chi1 = (index<<2)+1;		
	chi2 = chi1+1;		
	chi3 = chi2+1;		
	chi4 = chi3+1;	
	
	s2=s>>1;
	s4=s>>2;
	
	if (s4)
	{
		if (!bc[i-s2][j]) 
			bc[i][j+s4] = bc[i][j-s4] = bc[i-s4][j] = bc[i-s4][j+s4] = bc[i-s4][j-s4] = 0;

		if (!bc[i+s2][j]) 
			bc[i][j+s4] = bc[i][j-s4] = bc[i+s4][j] = bc[i+s4][j+s4] = bc[i+s4][j-s4] = 0;

		if (!bc[i][j-s2]) 
			bc[i-s4][j] = bc[i+s4][j] = bc[i][j-s4] = bc[i-s4][j-s4] = bc[i+s4][j-s4] = 0;

		if (!bc[i][j+s2]) 
			bc[i-s4][j] = bc[i+s4][j] = bc[i][j+s4] = bc[i-s4][j+s4] = bc[i+s4][j+s4] = 0;
	}

	// subdivision factors		
	if (s4>0)
	{				
		bc1 = bc[i+s4][j+s4]; 
		bc2 = bc[i-s4][j+s4]; 
		bc3 = bc[i-s4][j-s4]; 
		bc4 = bc[i+s4][j-s4];
	}
	else bc1=bc2=bc3=bc4=0;

	g_pCurWriteTree->m_nIndexArray[chi1].end = 
		g_pCurWriteTree->m_nIndexArray[chi2].end = 
		g_pCurWriteTree->m_nIndexArray[chi3].end = 
		g_pCurWriteTree->m_nIndexArray[chi4].end = 0;

	if (gridSize-1 != s && s2 < 8)
		chi1 = chi2 = chi3 = chi4 = index;		

	if (chi1 != index)
	{
		g_pCurWriteTree->m_nBboxArray[chi1].reset(g_pCurWriteTree->m_pVertexBuf[i+j*gridSize]);				
		g_pCurWriteTree->m_nBboxArray[chi2].reset(g_pCurWriteTree->m_pVertexBuf[i+j*gridSize]);				
		g_pCurWriteTree->m_nBboxArray[chi3].reset(g_pCurWriteTree->m_pVertexBuf[i+j*gridSize]);				
		g_pCurWriteTree->m_nBboxArray[chi4].reset(g_pCurWriteTree->m_pVertexBuf[i+j*gridSize]);		
	}
	currIndex = index;	

	if (chi1 != index)
		g_pCurWriteTree->m_nIndexArray[chi1].start = IndicesToRender;

	// first triangle fan quarter
	if (bc1!=0)	
		pipemap(i+s4,j+s4,s2,chi1);			
	else
	{			
		currIndex = chi1;
		if (chi1 != index)
		{
			chi = (chi1 << 2)+1;
			g_pCurWriteTree->m_nIndexArray[chi].end = 
				g_pCurWriteTree->m_nIndexArray[chi+1].end = 
				g_pCurWriteTree->m_nIndexArray[chi+2].end = 
				g_pCurWriteTree->m_nIndexArray[chi+3].end = 0;
		}
		BeginFan(i,j);
		if (bc[i+s2][j]) 
		{
			FanVertex(i+s2,j);	
			bc[i+s2][j] += 1;

			for (int k = j+1; k < j+s2;k++)
			{
				if (bc[i+s2][k]>0)
				{
					FanVertex(i+s2,k);	
					bc[i+s2][k] += 1;
				}

			/*	if (bc[i+s2][k]>1)
					FanVertex(i+s2,k);									
				else
					bc[i+s2][k] = 0;*/
			}
		}

		FanVertex(i+s2,j+s2);
		bc[i+s2][j+s2] += 1;

		if (bc[i][j+s2])
		{
			for (int k = i+s2-1; k > i;k--)
			{
				if (bc[k][j+s2] > 1) 
				{	FanVertex(k,j+s2);									
				
					bc[k][j+s2] +=1;
				}

				/*if (bc[k][j+s2] > 1) 
					FanVertex(k,j+s2);									
				else
					bc[k][j+s2] = 0;*/
			}
			FanVertex(i,j+s2);
		}			
	}
	if (chi1 != index)
	{
		g_pCurWriteTree->m_nIndexArray[chi1].end = IndicesToRender;
		g_pCurWriteTree->m_nBboxArray[index].addInternalBox(g_pCurWriteTree->m_nBboxArray[chi1]);	
	}	
	if (chi2 != index)
		g_pCurWriteTree->m_nIndexArray[chi2].start = IndicesToRender;
	
	// second triangle fan quarter
	if (bc2!=0)
	{
		if (bc1==0)
		{
			FanVertex(i,j+s2);
			bc[i][j+s2] += 1;
		}		
		pipemap(i-s4,j+s4,s2,chi2);
	}
	else
	{
		currIndex = chi2;
		if (chi2 != index)
		{
			chi = (chi2 << 2)+1;
			g_pCurWriteTree->m_nIndexArray[chi].end = 
				g_pCurWriteTree->m_nIndexArray[chi+1].end = 
				g_pCurWriteTree->m_nIndexArray[chi+2].end = 
				g_pCurWriteTree->m_nIndexArray[chi+3].end = 0;
		}

		if (bc1!=0)
			BeginFan(i,j);		

		if (bc[i][j+s2])
		{
			if (bc1)  FanVertex(i,j+s2);
			bc[i][j+s2] += 1;

			for (int k =i-1; k>i-s2;k--)
			{
				if (bc[k][j+s2] > 1) 
					FanVertex(k,j+s2);					
				else 
					bc[k][j+s2] = 0;				
			}

		}

		FanVertex(i-s2,j+s2);		
		bc[i-s2][j+s2] += 1;

		if (bc[i-s2][j])
		{
			for (int k = j + s2 - 1; k > j;k--)
			{
				if (bc[i-s2][k] > 1)
					FanVertex(i-s2,k);					
				else
					bc[i-s2][k] = 0;				
			}
			FanVertex(i-s2,j);
		}			
	}
	if (chi2 != index)
	{
		g_pCurWriteTree->m_nIndexArray[chi2].end = IndicesToRender;
		g_pCurWriteTree->m_nBboxArray[index].addInternalBox(g_pCurWriteTree->m_nBboxArray[chi2]);	
	}	
	if (chi3 != index)
		g_pCurWriteTree->m_nIndexArray[chi3].start = IndicesToRender;

	// third triangle fan quarter
	if (bc3!=0)
	{
		if (bc2==0)
		{					
			FanVertex(i-s2,j);
			bc[i-s2][j] += 1;
		}				
		pipemap(i-s4,j-s4,s2,chi3);
	}
	else
	{
		currIndex = chi3;
		if (chi3 != index)
		{
			chi = (chi3 << 2)+1;
			g_pCurWriteTree->m_nIndexArray[chi].end = 
				g_pCurWriteTree->m_nIndexArray[chi+1].end = 
				g_pCurWriteTree->m_nIndexArray[chi+2].end = 
				g_pCurWriteTree->m_nIndexArray[chi+3].end = 0;
		}

		if (bc2!=0)
			BeginFan(i,j);	

		if (bc[i-s2][j])
		{
			if (bc2) FanVertex(i-s2,j);
			bc[i-s2][j] += 1;

			for (int k = j-1; k > j-s2;k--)
			{
				if (bc[i-s2][k] > 1)
					FanVertex(i-s2,k);					
				else
					bc[i-s2][k] = 0;				
			}
		}

		FanVertex(i-s2,j-s2);
		bc[i-s2][j-s2] += 1;

		if (bc[i][j-s2])
		{
			for (int k = i-s2 + 1; k < i;k++)
			{
				if (bc[k][j-s2] > 1)
					FanVertex(k,j-s2);					
				else
					bc[k][j-s2] = 0;				
			}
			FanVertex(i,j-s2);
		}			
	}
	if (chi3 != index)
	{
		g_pCurWriteTree->m_nIndexArray[chi3].end = IndicesToRender;
		g_pCurWriteTree->m_nBboxArray[index].addInternalBox(g_pCurWriteTree->m_nBboxArray[chi3]);	
	}	
	if (chi4 != index)
		g_pCurWriteTree->m_nIndexArray[chi4].start = IndicesToRender;

	// fourth triangle fan quarter
	if (bc4!=0)
	{
		if (bc3==0)
		{					
			FanVertex(i,j-s2);
			bc[i][j-s2] += 1;
		}		
		pipemap(i+s4,j-s4,s2, chi4);
	}
	else
	{
		currIndex = chi4;
		if (chi4 != index)
		{
			chi = (chi4 << 2)+1;
			g_pCurWriteTree->m_nIndexArray[chi].end = 
				g_pCurWriteTree->m_nIndexArray[chi+1].end = 
				g_pCurWriteTree->m_nIndexArray[chi+2].end = 
				g_pCurWriteTree->m_nIndexArray[chi+3].end = 0;
		}
		if (bc3!=0)
			BeginFan(i,j);	

		if (bc[i][j-s2])
		{
			if (bc3) FanVertex(i,j-s2);
			bc[i][j-s2] += 1;

			for (int k = i + 1; k < i+s2;k++)
			{
				if (bc[k][j-s2]>1) 
					FanVertex(k,j-s2);					
				else
					bc[k][j-s2]=0;				
			}
		}

		FanVertex(i+s2,j-s2);
		bc[i+s2][j-s2] += 1;
		
		if (bc[i+s2][j])
		{
			for (int k = j-s2 + 1; k < j;k++)
			{
				if (bc[i+s2][k]>1) 
					FanVertex(i+s2,k);					
				else
					bc[i+s2][k]=0;				
			}
		}

		if (bc1==0)
		{
			if (bc[i+s2][j])
			{			
				FanVertex(i+s2,j);			
				bc[i+s2][j] += 1;
			} 
			else
			{
				for (int k = j + 1; k < j+s2;k++)
				{
					if (bc[i+s2][k]>1) 
						FanVertex(i+s2,k);					
					else
						bc[i+s2][k]=0;				
				}

				FanVertex(i+s2,j+s2);
				bc[i+s2][j+s2] += 1;
			}
		}
		else 
		{
			FanVertex(i+s2,j);
			bc[i+s2][j] += 1;
		}
	}

	if (chi4 != index)
	{
		g_pCurWriteTree->m_nIndexArray[chi4].end = IndicesToRender;
		g_pCurWriteTree->m_nBboxArray[index].addInternalBox(g_pCurWriteTree->m_nBboxArray[chi4]);	
	}		
}

GDM_POINT3D tmpPts[GDEM_MAX_DEM_PT_COUNT * GDEM_MAX_DEM_PT_COUNT];  // for overflow of call stack size

void gdm_SetRenderParamForTexture(GDEM_RENDER_GEO_PARAM *pParam,GDM_MESH_TILE* pMeshTile,GDM_POINT3D *pBuf)
{
	int vertexindex = 0;

	float fTexRow = pMeshTile->fTextureBottom;
	float fTexCol;
	float fTextureStep = pMeshTile->fTextureWidth / pMeshTile->nDivision;
	int ptIndex;
	int rowPts;

	double tileWidth = gdm_TwoPI / (1 << pMeshTile->nLevel);
	double cellWidth = tileWidth / pMeshTile->nDivision;

	if(pMeshTile->nDivision > pMeshTile->nDemDivision)
		rowPts = (pMeshTile->nDivision + 1);
	else
		rowPts = GDEM_MAX_DEM_PT_COUNT;
		
	for (int row = 0; row <= pMeshTile->nDivision;row++)   //from bottom to top
	{		
		fTexCol = pMeshTile->fTextureLeft;
		for(int col = 0; col <= pMeshTile->nDivision; col++) //from left to right
		{
			ptIndex=col+row*rowPts;

			pParam->pVertexBuf[vertexindex].Pos.X =  pBuf[ptIndex].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[vertexindex].Pos.Y =  - (pBuf[ptIndex].m_Y- g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[vertexindex].Pos.Z =  pBuf[ptIndex].m_Z - g_OffsetBodyPt.m_tZ;

			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tX =  pBuf[ptIndex].m_X;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tY =  pBuf[ptIndex].m_Y;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tZ =  pBuf[ptIndex].m_Z;			

			if(pParam->nTextureID>0)
			{
				pParam->pVertexBuf[vertexindex].TCoords.X = fTexCol;
				pParam->pVertexBuf[vertexindex].TCoords.Y = fTexRow;
			}
			else
			{
				double dLongi = pMeshTile->minLong + cellWidth * col;
				double dLati  = pMeshTile->minLati + cellWidth * row;
				pParam->pVertexBuf[vertexindex].TCoords.X = (float)(0.5 + dLongi / gdm_PI / 2);
				pParam->pVertexBuf[vertexindex].TCoords.Y = (float)(0.5 - dLati / gdm_PI);
			}

			// calcu overlay texture
			if(pMeshTile->nOverlayTextureID > 0)
			{
				pParam->pVertexBuf[vertexindex].TCoords2 = pParam->pVertexBuf[vertexindex].TCoords;

				//GIS texture coordinate
				pParam->pVertexBuf[vertexindex].TCoords.X = (float)col / pMeshTile->nDivision;
				pParam->pVertexBuf[vertexindex].TCoords.Y = 1.0f - (float)row / pMeshTile->nDivision;
			}

			fTexCol += fTextureStep;
			vertexindex++;
		}
		fTexRow -= fTextureStep;
	}
	pParam->nVertexCount = vertexindex;
}

void gdm_SetRenderParamForHeight(GDEM_RENDER_GEO_PARAM *pParam,GDM_MESH_TILE* pMeshTile,GDM_POINT3D *pBuf)
{
	int vertexindex = 0;

	float fTexRow = pMeshTile->fTextureBottom;
	float fTexCol;
	float fTextureStep = pMeshTile->fTextureWidth / pMeshTile->nDivision;
	int ptIndex;
	int rowPts;

	double tileWidth = gdm_TwoPI / (1 << pMeshTile->nLevel);
	double cellWidth = tileWidth / pMeshTile->nDivision;

	bool blheight;
	float height=0 ;
	float maxHeight = GDM_MAX_HEIGHT;
	float minHeight = 10;

	if(pMeshTile->nDivision > pMeshTile->nDemDivision)
	{
		blheight=false;
		rowPts = (pMeshTile->nDivision + 1);
	}
	else
	{
		blheight=true;
		rowPts = GDEM_MAX_DEM_PT_COUNT;
	}

	for (int row = 0; row <= pMeshTile->nDivision;row++)   //from bottom to top
	{		
		fTexCol = pMeshTile->fTextureLeft;
		for(int col = 0; col <= pMeshTile->nDivision; col++) //from left to right
		{
			ptIndex=col+row*rowPts;

			pParam->pVertexBuf[vertexindex].Pos.X =  pBuf[ptIndex].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[vertexindex].Pos.Y =  - (pBuf[ptIndex].m_Y- g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[vertexindex].Pos.Z =  pBuf[ptIndex].m_Z - g_OffsetBodyPt.m_tZ;

			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tX =  pBuf[ptIndex].m_X;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tY =  pBuf[ptIndex].m_Y;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tZ =  pBuf[ptIndex].m_Z;			

			{
				height=0;
				if(blheight)
					height = pMeshTile->buf_height[ptIndex];

				height=qBound(minHeight,height,maxHeight);

				pParam->pVertexBuf[vertexindex].TCoords.Y = 1.0f - height / maxHeight;
				pParam->pVertexBuf[vertexindex].TCoords.X = 0.5f;
			}
			// calcu overlay texture
			if(pMeshTile->nOverlayTextureID > 0)
			{
				pParam->pVertexBuf[vertexindex].TCoords2 = pParam->pVertexBuf[vertexindex].TCoords;

				//GIS texture coordinate
				pParam->pVertexBuf[vertexindex].TCoords.X = (float)col / pMeshTile->nDivision;
				pParam->pVertexBuf[vertexindex].TCoords.Y = 1.0f - (float)row / pMeshTile->nDivision;
			}

			fTexCol += fTextureStep;
			vertexindex++;
		}
		fTexRow -= fTextureStep;
	}
	pParam->nVertexCount = vertexindex;
}

void gdm_SetRenderParamForRateAngle(GDEM_RENDER_GEO_PARAM *pParam,GDM_MESH_TILE* pMeshTile,GDM_POINT3D *pBuf)
{
	int vertexindex = 0;

	float fTexRow = pMeshTile->fTextureBottom;
	float fTexCol;
	float fTextureStep = pMeshTile->fTextureWidth / pMeshTile->nDivision;
	int ptIndex;
	int rowPts;

	double tileWidth = gdm_TwoPI / (1 << pMeshTile->nLevel);
	double cellWidth = tileWidth / pMeshTile->nDivision;

	float angle = 0; //0 ~ 90
	float maxAngle = GDM_GIS_MAX_ANGLE;

	bool blNormalInfo=false;

	if(pMeshTile->nDivision <=  pMeshTile->nDemDivision && pMeshTile->buf_dirInfo!=NULL)
		blNormalInfo=true;

	if(pMeshTile->nDivision > pMeshTile->nDemDivision)
		rowPts = (pMeshTile->nDivision + 1);
	else
		rowPts = GDEM_MAX_DEM_PT_COUNT;

	for (int row = 0; row <= pMeshTile->nDivision;row++)   //from bottom to top
	{		
		fTexCol = pMeshTile->fTextureLeft;
		for(int col = 0; col <= pMeshTile->nDivision; col++) //from left to right
		{
			ptIndex=col+row*rowPts;

			pParam->pVertexBuf[vertexindex].Pos.X =  pBuf[ptIndex].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[vertexindex].Pos.Y =  - (pBuf[ptIndex].m_Y- g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[vertexindex].Pos.Z =  pBuf[ptIndex].m_Z - g_OffsetBodyPt.m_tZ;

			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tX =  pBuf[ptIndex].m_X;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tY =  pBuf[ptIndex].m_Y;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tZ =  pBuf[ptIndex].m_Z;

			{
				angle = 0; //0 ~ 90
				if(blNormalInfo && g_gdm_MeshTileArr.nLevel >= GDEM_MIN_RATE_DISP_LEVEL)
					angle = pMeshTile->buf_dirInfo[ptIndex].angle;

				if(angle > maxAngle)
					angle = maxAngle;

				angle=qBound(0.0f,angle,maxAngle);

				pParam->pVertexBuf[vertexindex].TCoords.X = 0.5f;
				pParam->pVertexBuf[vertexindex].TCoords.Y = 1.0f - (float)(angle / maxAngle);
			}

			// calcu overlay texture
			if(pMeshTile->nOverlayTextureID > 0)
			{
				pParam->pVertexBuf[vertexindex].TCoords2 = pParam->pVertexBuf[vertexindex].TCoords;

				//GIS texture coordinate
				pParam->pVertexBuf[vertexindex].TCoords.X = (float)col / pMeshTile->nDivision;
				pParam->pVertexBuf[vertexindex].TCoords.Y = 1.0f - (float)row / pMeshTile->nDivision;
			}

			fTexCol += fTextureStep;
			vertexindex++;
		}
		fTexRow -= fTextureStep;
	}
	pParam->nVertexCount = vertexindex;
}

void gdm_SetRenderParamForRatePlan(GDEM_RENDER_GEO_PARAM *pParam,GDM_MESH_TILE* pMeshTile,GDM_POINT3D *pBuf)
{
	int vertexindex = 0;

	float fTexRow = pMeshTile->fTextureBottom;
	float fTexCol;
	float fTextureStep = pMeshTile->fTextureWidth / pMeshTile->nDivision;
	int ptIndex;
	int rowPts;

	double tileWidth = gdm_TwoPI / (1 << pMeshTile->nLevel);
	double cellWidth = tileWidth / pMeshTile->nDivision;

	double angle;

	bool blNormalInfo=false;

	if(pMeshTile->nDivision <= pMeshTile->nDemDivision && pMeshTile->buf_dirInfo!=NULL)
		blNormalInfo=true;

	if(pMeshTile->nDivision > pMeshTile->nDemDivision)
		rowPts = (pMeshTile->nDivision + 1);
	else
		rowPts = GDEM_MAX_DEM_PT_COUNT;

	for (int row = 0; row <= pMeshTile->nDivision;row++)   //from bottom to top
	{		
		fTexCol = pMeshTile->fTextureLeft;
		for(int col = 0; col <= pMeshTile->nDivision; col++) //from left to right
		{
			ptIndex=col+row*rowPts;

			pParam->pVertexBuf[vertexindex].Pos.X =  pBuf[ptIndex].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[vertexindex].Pos.Y =  - (pBuf[ptIndex].m_Y- g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[vertexindex].Pos.Z =  pBuf[ptIndex].m_Z - g_OffsetBodyPt.m_tZ;

			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tX =  pBuf[ptIndex].m_X;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tY =  pBuf[ptIndex].m_Y;
			g_pCurWriteTree->m_pVertexBuf[vertexindex].m_tZ =  pBuf[ptIndex].m_Z;
			
			angle = 361; // 0 ~ 360
			if(blNormalInfo && g_gdm_MeshTileArr.nLevel >= GDEM_MIN_RATE_DISP_LEVEL)
				angle = pMeshTile->buf_dirInfo[ptIndex].direction;

			if(angle <= 360)
			{
				angle = angle * PAI / 180;
				pParam->pVertexBuf[vertexindex].TCoords.X = (float)(0.3 * cos(angle) + 0.5);
				pParam->pVertexBuf[vertexindex].TCoords.Y = (float)(0.5 - 0.3 * sin(angle));
			}
			else
			{
				pParam->pVertexBuf[vertexindex].TCoords.X = 0.5f;
				pParam->pVertexBuf[vertexindex].TCoords.Y = 0.5f;
			}
			

			// calcu overlay texture
			if(pMeshTile->nOverlayTextureID > 0)
			{
				pParam->pVertexBuf[vertexindex].TCoords2 = pParam->pVertexBuf[vertexindex].TCoords;

				//GIS texture coordinate
				pParam->pVertexBuf[vertexindex].TCoords.X = (float)col / pMeshTile->nDivision;
				pParam->pVertexBuf[vertexindex].TCoords.Y = 1.0f - (float)row / pMeshTile->nDivision;
			}

			fTexCol += fTextureStep;
			vertexindex++;
		}
		fTexRow -= fTextureStep;
	}
	pParam->nVertexCount = vertexindex;
}

int	gdm_GetRenderCallBackTerrain(GDEM_RENDER_GEO_PARAM *pParam)
{	
	Q_ASSERT(pParam!=NULL);

	if(pParam->nIndex >= g_gdm_MeshTileArr.nCount)
		return 0;

	GDM_POINT3D *pBuf;
	
	GDM_MESH_TILE *pMeshTile;
			
	pMeshTile = &g_gdm_MeshTileArr.pData[pParam->nIndex];

	// set textureID
	pParam->nTextureID = pMeshTile->nTextureID;
	pParam->nOverlayTextureID = pMeshTile->nOverlayTextureID;
	
	double tileWidth = gdm_TwoPI / (1 << pMeshTile->nLevel);

	if(pMeshTile->nDivision > pMeshTile->nDemDivision)
	{		
		// calcu edges
		double dBaseLongitude;
		double dBaseLatitude = pMeshTile->minLati;	
		int nVertex = 0;
		
		double cellWidth = tileWidth / pMeshTile->nDivision;
		for(int row = 0; row <= pMeshTile->nDivision; row++) //from bottom to top
		{
			dBaseLongitude = pMeshTile->minLong;
			for(int col = 0; col <= pMeshTile->nDivision; col++) //from left to right 
			{				
				gdm_get3DWorldPoint(dBaseLongitude, dBaseLatitude, tmpPts[nVertex++], 0, pMeshTile->pDemTile, true);
				dBaseLongitude += cellWidth;
			}
			dBaseLatitude += cellWidth;
		}

		pBuf = tmpPts;
	}
	else
	{
		pBuf = pMeshTile->buf_dem;
	}

	g_IndexBuffer = pParam->pIndexBuf;
	gridSize = pMeshTile->nDivision+1;
	
	g_pCurWriteTree = pWriteQuadList->add(pMeshTile->nX,pMeshTile->nY,pMeshTile->nLevel, pMeshTile->mBC); 	
	g_pCurWriteTree->m_pVertexBuf = pWriteQuadList->getVBuffer((pMeshTile->nDivision+1)*(pMeshTile->nDivision+1));//pVBuf;
	g_pCurWriteTree->ngridSize = gridSize; 
	g_pCurWriteTree->LRU = 0;
		
	g_pCurWriteTree->minLongitude = pMeshTile->minLong;	
	g_pCurWriteTree->minLatitude = pMeshTile->minLati;	

	g_pCurWriteTree->tileWidth = tileWidth;
	pWriteQuadList->calcLRU(g_pCurWriteTree,pMeshTile->minLong+tileWidth/2,pMeshTile->minLati+tileWidth/2);
	pWriteQuadList->updateLRU(g_pCurWriteTree);

	Q_ASSERT(g_pCurWriteTree->m_pVertexBuf!=NULL);

	switch(gdm_GetDataMgr()->m_sOption.layerTextureMode)
	{
		case LTM_TEXTURE:
		case LTM_RASTER:
			gdm_SetRenderParamForTexture(pParam,pMeshTile,pBuf);
			break;
		case LTM_HEIGHT:
			gdm_SetRenderParamForHeight(pParam,pMeshTile,pBuf);
			break;
		case LTM_RATEANGLE:
			gdm_SetRenderParamForRateAngle(pParam,pMeshTile,pBuf);
			break;
		case LTM_RATEPLAN:
			gdm_SetRenderParamForRatePlan(pParam,pMeshTile,pBuf);
	}

	g_pCurWriteTree->m_nVertex = pParam->nVertexCount;

	Vector3DF p1,p2, p3,p4;

	p1 = g_pCurWriteTree->m_pVertexBuf[0]; 
	p1.setLength(EARTH_RADIUS);

	p2 = g_pCurWriteTree->m_pVertexBuf[gridSize-1];
	p2.setLength(EARTH_RADIUS);

	p3 = g_pCurWriteTree->m_pVertexBuf[g_pCurWriteTree->m_nVertex-gridSize];
	p3.setLength(EARTH_RADIUS);

	p4 = g_pCurWriteTree->m_pVertexBuf[g_pCurWriteTree->m_nVertex-1];
	p4.setLength(EARTH_RADIUS);

	g_pCurWriteTree->m_pLeftQuad = Triangle3DF(p1,p3 ,p4);
	g_pCurWriteTree->m_pRightQuad = Triangle3DF(p1,p4, p2);

	//bound pt
		
	int col = pMeshTile->nDivision;
	int row = gridSize*pMeshTile->nDivision+1;
	int step = GDEM_MAX_DEM_DIVISION/pMeshTile->nDivision;
	int sp1 = 0, sp2 = GDEM_MAX_DEM_DIVISION, sp3 = GDEM_MAX_DEM_DIVISION3-step, sp4 = GDEM_MAX_DEM_DIVISION4-step;

	for (int i = 0;i < pMeshTile->nDivision;i++)
	{	
		if (pMeshTile->mBC[sp1])
		{  //bottom bounding condition
			pParam->pVertexBuf[i].Pos.X = pMeshTile->mBP[sp1].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[i].Pos.Y = -(pMeshTile->mBP[sp1].m_Y - g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[i].Pos.Z = pMeshTile->mBP[sp1].m_Z - g_OffsetBodyPt.m_tZ;
			g_pCurWriteTree->m_pVertexBuf[i]	= Vector3DF(pMeshTile->mBP[sp1].m_X,pMeshTile->mBP[sp1].m_Y,pMeshTile->mBP[sp1].m_Z);				
		}

		if (pMeshTile->mBC[sp2])
		{   //right bounding condition
			pParam->pVertexBuf[col].Pos.X = pMeshTile->mBP[sp2].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[col].Pos.Y = -(pMeshTile->mBP[sp2].m_Y - g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[col].Pos.Z = pMeshTile->mBP[sp2].m_Z - g_OffsetBodyPt.m_tZ;
			g_pCurWriteTree->m_pVertexBuf[col] =  Vector3DF(pMeshTile->mBP[sp2].m_X, pMeshTile->mBP[sp2].m_Y, pMeshTile->mBP[sp2].m_Z);				 
		}

		if (pMeshTile->mBC[sp3])
		{     //top bounding condition
			pParam->pVertexBuf[row+i].Pos.X = pMeshTile->mBP[sp3].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[row+i].Pos.Y = -(pMeshTile->mBP[sp3].m_Y - g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[row+i].Pos.Z = pMeshTile->mBP[sp3].m_Z - g_OffsetBodyPt.m_tZ;
			g_pCurWriteTree->m_pVertexBuf[row+i] = Vector3DF(pMeshTile->mBP[sp3].m_X,pMeshTile->mBP[sp3].m_Y,pMeshTile->mBP[sp3].m_Z);								
		}

		if (pMeshTile->mBC[sp4])
		{    //left bounding condition
			pParam->pVertexBuf[col+1].Pos.X = pMeshTile->mBP[sp4].m_X - g_OffsetBodyPt.m_tX;
			pParam->pVertexBuf[col+1].Pos.Y = -(pMeshTile->mBP[sp4].m_Y - g_OffsetBodyPt.m_tY);
			pParam->pVertexBuf[col+1].Pos.Z = pMeshTile->mBP[sp4].m_Z -g_OffsetBodyPt.m_tZ;
			g_pCurWriteTree->m_pVertexBuf[col+1] = Vector3DF(pMeshTile->mBP[sp4].m_X,pMeshTile->mBP[sp4].m_Y,pMeshTile->mBP[sp4].m_Z);
		}
		
		col += gridSize;			
		sp1+=step;
		sp2+=step;
		sp3-=step;
		sp4-=step;
	}			

	bool updated = true;
	QuadTree *pReadTree=NULL;	

	if (pReadQuadList)
	{
		pReadTree  = pReadQuadList->find_exist(pMeshTile->nX,pMeshTile->nY,pMeshTile->nLevel); 
		if (pReadTree)
		{
			updated = pReadTree->isUpdated(pMeshTile->mBC,gridSize);	
			pReadTree->LRU=1;
		}
	}

	if (!updated)
	{	
		memcpy(pParam->pIndexBuf, pReadTree->m_pIndexBuf, sizeof(unsigned short)*pReadTree->m_nIndices);
		memcpy(g_pCurWriteTree->m_nIndexArray, pReadTree->m_nIndexArray, sizeof(NodeIndex)*400);
		memcpy(g_pCurWriteTree->m_nBboxArray,  pReadTree->m_nBboxArray, sizeof(Aabbox3DF)*400);
		pWriteQuadList->copyIndexBuffer(g_pCurWriteTree, pReadTree);					
		pParam->nIndex++;
		return g_pCurWriteTree->m_nIndices; 
	}		

	g_pCurWriteTree->m_pIndexBuf = pWriteQuadList->getIBuffer(gridSize*gridSize*6);//pIBuf;
	Q_ASSERT(g_pCurWriteTree->m_pIndexBuf!=NULL);
	g_pCurWriteTree->m_nIndices = 0;
	g_pCurWriteTree->m_nBboxArray[0].reset(g_pCurWriteTree->m_pVertexBuf[0]);	
	g_pCurWriteTree->m_nIndexArray[1].end = 
		g_pCurWriteTree->m_nIndexArray[2].end = 
		g_pCurWriteTree->m_nIndexArray[3].end =
		g_pCurWriteTree->m_nIndexArray[4].end = 0;
	
	IndicesToRender = 0;
	if (gridSize == 2)
	{
		Aabbox3DF *currBBox = &g_pCurWriteTree->m_nBboxArray[0];	

		 g_pCurWriteTree->m_pIndexBuf[0] = g_IndexBuffer[0] = 0;
		 g_pCurWriteTree->m_pIndexBuf[1] = g_IndexBuffer[1] = 2;
		 g_pCurWriteTree->m_pIndexBuf[2] = g_IndexBuffer[2] = 3;

		 g_pCurWriteTree->m_pIndexBuf[3] = g_IndexBuffer[3] = 0;
		 g_pCurWriteTree->m_pIndexBuf[4] = g_IndexBuffer[4] = 3;
		 g_pCurWriteTree->m_pIndexBuf[5] = g_IndexBuffer[5] = 1;
		
		IndicesToRender = 6;
		currBBox->reset(g_pCurWriteTree->m_pVertexBuf[0]);
		currBBox->addInternalPoint(g_pCurWriteTree->m_pVertexBuf[1]);
		currBBox->addInternalPoint(g_pCurWriteTree->m_pVertexBuf[2]);
		currBBox->addInternalPoint(g_pCurWriteTree->m_pVertexBuf[3]);			
	}
	else
	{
		initBC(pMeshTile);		
		pipemap(gridSize/2, gridSize/2, gridSize-1,0);			
	}		
	
	g_pCurWriteTree->m_nIndexArray[0].start = 0;
	g_pCurWriteTree->m_nIndexArray[0].end = IndicesToRender;

	g_pCurWriteTree->m_nIndices = IndicesToRender;
	pWriteQuadList->pIBuf = & g_pCurWriteTree->m_pIndexBuf[IndicesToRender+1];
	
	pParam->nIndex++;	
	return IndicesToRender;
}

void gdm_MatchNeighbor(GDM_MESH_TILE *pMeshTile,GDM_MESH_TILE *pOther,int type)
{
	int ox,oy,nx,ny,mx,my;	
	GDM_POINT3D *p1, *p2;
	unsigned char *m1, *m2, *n1, *n2;
	int step1 = GDEM_MAX_DEM_DIVISION/pMeshTile->nSubs;
	int step2 = GDEM_MAX_DEM_DIVISION/pOther->nSubs;

	ox = qMax(pMeshTile->nSubX, pOther->nSubX);
	oy = qMax(pMeshTile->nSubY, pOther->nSubY);
	nx = (ox-pMeshTile->nSubX)*step1;
	ny = (oy-pMeshTile->nSubY)*step1;
	mx = (ox-pOther->nSubX)*step2;
	my = (oy-pOther->nSubY)*step2;

	switch(type)
	{
		case 0:		//bottom-top : pMeshTile은 꼭대기, pOther는 아래에 놓여있음
			p1 = &pMeshTile->mBP[nx]; p2 = &pOther->mBP[GDEM_MAX_DEM_DIVISION3-mx];
			m1 = &pMeshTile->mBC[nx]; m2 = &pOther->mBC[GDEM_MAX_DEM_DIVISION3-mx];
			n1 = &pMeshTile->mBU[nx]; n2 = &pOther->mBU[GDEM_MAX_DEM_DIVISION3-mx];							
			break;
		case 1:		//left-right : pMeshTile은 왼쪽에, pOther는 오른쪽에 있음
			p1 = &pMeshTile->mBP[GDEM_MAX_DEM_DIVISION+ny]; p2 = &pOther->mBP[GDEM_MAX_DEM_DIVISION4-my];
			m1 = &pMeshTile->mBC[GDEM_MAX_DEM_DIVISION+ny]; m2 = &pOther->mBC[GDEM_MAX_DEM_DIVISION4-my];
			n1 = &pMeshTile->mBU[GDEM_MAX_DEM_DIVISION+ny]; n2 = &pOther->mBU[GDEM_MAX_DEM_DIVISION4-my];
			break;	
		default:
			break;
	}

	if (n1[0] || (!n1[0] && !n2[0]))		
		p2[0] = p1[0];	
	if (n2[0])
		p1[0] = p2[0];

	if (n1[step1] || (!n1[step1] && !n2[-step2]))
		p2[-step2] = p1[step1];
	if (n2[-step2])
		p1[step1] = p2[-step2];

	m1[0] = m2[0] = m1[step1] = m2[-step2] = 2;	

	int substep = qMin(pMeshTile->nSubDivision, pOther->nSubDivision);	
	int n = step1/substep;
	int m = step2/substep;
	int i,j,k = n;

	for (i=n,j=m;i < step1;i+=n,j+=m)
	{		
		m1[i] = m2[-j] = 2;
		if (step1 >= step2)
			p2[-j] = p1[i];
		else
			p1[i] = p2[-j];		
	}

	for (i = 0;i < GDEM_MAX_DEM_PT_COUNT;i++)
	{
		if (i<=step1) {	n1[i] = 1;}
		if (i<=step2) { n2[-i] = 1;}
	}

	if (pMeshTile->mBU[0] && !pMeshTile->mBU[GDEM_MAX_DEM_DIVISION4])
	{
		pMeshTile->mBU[GDEM_MAX_DEM_DIVISION4] = 1;
		pMeshTile->mBP[GDEM_MAX_DEM_DIVISION4] = pMeshTile->mBP[0];
		pMeshTile->mBC[GDEM_MAX_DEM_DIVISION4] = pMeshTile->mBC[0];
	}

	if (!pMeshTile->mBU[0] && pMeshTile->mBU[GDEM_MAX_DEM_DIVISION4])
	{
		pMeshTile->mBU[0] = 1;
		pMeshTile->mBP[0] = pMeshTile->mBP[GDEM_MAX_DEM_DIVISION4];
		pMeshTile->mBC[0] = pMeshTile->mBC[GDEM_MAX_DEM_DIVISION4];
	}

	if (pOther->mBU[0] && !pOther->mBU[GDEM_MAX_DEM_DIVISION4])
	{
		pOther->mBU[GDEM_MAX_DEM_DIVISION4] = 1;
		pOther->mBP[GDEM_MAX_DEM_DIVISION4] = pOther->mBP[0];
		pOther->mBC[GDEM_MAX_DEM_DIVISION4] = pOther->mBC[0];
	}

	if (!pOther->mBU[0] && pOther->mBU[GDEM_MAX_DEM_DIVISION4])
	{
		pOther->mBU[0] = 1;
		pOther->mBP[0] = pOther->mBP[GDEM_MAX_DEM_DIVISION4];
		pOther->mBC[0] = pOther->mBC[GDEM_MAX_DEM_DIVISION4];
	}
}

void gdm_MakeBounds(GDM_MESH_TILE* pMeshTile)
{		
	if (pMeshTile->nDemDivision >= 1 && pMeshTile->buf_dem)
	{	
		int row = GDEM_MAX_DEM_PT_COUNT*pMeshTile->nDivision+1;  
		int rcol = pMeshTile->nDivision, lcol = GDEM_MAX_DEM_PT_COUNT;
		int step = GDEM_MAX_DEM_DIVISION/pMeshTile->nDivision;
		int sp1 = 0, sp2 = GDEM_MAX_DEM_DIVISION, sp3 = GDEM_MAX_DEM_DIVISION3-step, sp4 = GDEM_MAX_DEM_DIVISION4-step;

		for (int j = 0; j < pMeshTile->nDivision;j++)
		{			
			pMeshTile->mBP[sp1]	= pMeshTile->buf_dem[j];
			pMeshTile->mBP[sp2]	= pMeshTile->buf_dem[rcol];
			pMeshTile->mBP[sp3]	= pMeshTile->buf_dem[row+j];
			pMeshTile->mBP[sp4]	= pMeshTile->buf_dem[lcol];
						
			sp1+=step;
			sp2+=step;
			sp3-=step;
			sp4-=step;	
			rcol += GDEM_MAX_DEM_PT_COUNT;			
			lcol += GDEM_MAX_DEM_PT_COUNT;
		}
		pMeshTile->mBP[GDEM_MAX_DEM_DIVISION4] = pMeshTile->mBP[0];
	}
}

void gdm_MakeExtraBounds(GDM_MESH_TILE* pMeshTile)
{	
	// calcu edges
	double tileWidth = gdm_TwoPI / (1 << pMeshTile->nLevel);
	double cellWidth = tileWidth / pMeshTile->nDivision;
	double dBaseLongitude = pMeshTile->minLong;
	double dBaseLatitude  = pMeshTile->minLati;
	double longi = dBaseLongitude, lati=dBaseLatitude,rightlongi = dBaseLongitude+tileWidth,toplati = dBaseLatitude+tileWidth;
	
	int step = GDEM_MAX_DEM_DIVISION/pMeshTile->nDivision;
	int sp1 = 0, sp2 = GDEM_MAX_DEM_DIVISION, sp3 = GDEM_MAX_DEM_DIVISION3-step, sp4 = GDEM_MAX_DEM_DIVISION4-step;
	
	for(int i = 0; i < pMeshTile->nDivision; i++)
	{		
		if (!pMeshTile->mBU[sp1])	//bottom
			gdm_get3DWorldPoint(longi, dBaseLatitude, pMeshTile->mBP[sp1], 0, NULL, true);			

		if (!pMeshTile->mBU[sp2])	//right
			gdm_get3DWorldPoint(rightlongi, lati, pMeshTile->mBP[sp2], 0, NULL, true);				

		longi+=cellWidth;
		lati +=cellWidth;
		if (!pMeshTile->mBU[sp3])	//top
			gdm_get3DWorldPoint(longi, toplati, pMeshTile->mBP[sp3], 0, NULL, true);			

		if (!pMeshTile->mBU[sp4])	//left	
			gdm_get3DWorldPoint(dBaseLongitude, lati, pMeshTile->mBP[sp4], 0, NULL, true);		

		sp1+=step;
		sp2+=step;
		sp3-=step;
		sp4-=step;	
	}		
}


// return logic tiles for render

void gdm_GetPyramidDivisionTiles(GDM_MESH_TILE_ARR *pArr)
{
	const GDM_LOGIC_TILE_ARR *pBaseArr = gdm_GetRenderTiles();
	GDM_DEM_TILE *pDemTile;
	int i,j, nX, nY, nDelta;
	int nMaxX,base;
	double width;	
	GDM_MESH_TILE  *pMeshTile, *pOther;
	const GDM_LOGIC_TILE *pLogicTile;

	memset(pArr, 0, sizeof(GDM_MESH_TILE_ARR));

	pArr->nLevel = pBaseArr->nLevel;
	pArr->nCount = pBaseArr->nCount;

	for(i= 0; i < pBaseArr->nCount; i++)
	{
		pMeshTile = &pArr->pData[i];
		pLogicTile = &pBaseArr->pData[i];

		pMeshTile->nX = pLogicTile->nX;
		pMeshTile->nY = pLogicTile->nY;
		pMeshTile->nLevel = pLogicTile->nLevel;

		pMeshTile->minLong = pLogicTile->minLongitude;
		pMeshTile->minLati = pLogicTile->minLatitude;

		pMeshTile->nDivision = 1;
		pMeshTile->nDemDivision = pMeshTile->nSubDivision = 0;		
		pMeshTile->buf_dem = NULL;
		pMeshTile->buf_height = NULL;

		pDemTile = pLogicTile->pDemTile;
		pMeshTile->pDemTile = pDemTile;

		if (pDemTile)
		{
			nDelta = pMeshTile->nLevel - pDemTile->m_sInfo.nLevel;
			if(nDelta >= 0 && nDelta < 8)
			{
				pMeshTile->nDemDivision = pMeshTile->nDivision = GDEM_MAX_DEM_DIVISION/(1<<nDelta);		
				nX = pMeshTile->nX - (pDemTile->m_sInfo.nX << nDelta);
				nY = pMeshTile->nY - (pDemTile->m_sInfo.nY << nDelta);
				base = GDEM_MAX_DEM_PT_COUNT * pMeshTile->nDivision * nY + pMeshTile->nDivision * nX;
				pMeshTile->buf_dem	  = gdm_GetDataMgr()->m_pTerrainPtCache->GetPts(pDemTile) + base;				
				pMeshTile->buf_height = pDemTile->m_pData + base;
				if(gdm_GetDataMgr()->IsNecessaryDirInfo())
					pMeshTile->buf_dirInfo = gdm_GetDataMgr()->m_pTerrainDirCache->GetInfos(pDemTile) + base;	
				gdm_MakeBounds(pMeshTile);
			}
			else
			{
				gdm_MakeExtraBounds(pMeshTile);
				pMeshTile->mBP[GDEM_MAX_DEM_DIVISION4] = pMeshTile->mBP[0];
			}
		}
		else
		{
			gdm_MakeExtraBounds(pMeshTile);			
			pMeshTile->mBP[GDEM_MAX_DEM_DIVISION4] = pMeshTile->mBP[0];
		}

		// decide texture coord
		pMeshTile->nTextureID = 0;
	
		if(gdm_GetDataMgr()->m_sOption.layerTextureMode==LTM_TEXTURE && pLogicTile->pImgTile != NULL)
		{
			pMeshTile->nTextureID = gdm_GetTextureIDCache()->GetTextureID(pLogicTile->pImgTile);
			nDelta = pMeshTile->nLevel - pLogicTile->pImgTile->m_sInfo.nLevel;
			nX = pMeshTile->nX - (pLogicTile->pImgTile->m_sInfo.nX << nDelta);
			nY = pMeshTile->nY - (pLogicTile->pImgTile->m_sInfo.nY << nDelta);

			width = 1.0 / (1 << nDelta);
			pMeshTile->fTextureLeft = width * nX;			
			pMeshTile->fTextureBottom   = 1 - width * nY;
			pMeshTile->fTextureWidth = width;			
		}

		if(gdm_GetDataMgr()->m_sOption.layerTextureMode==LTM_RASTER && pLogicTile->prasterTile != NULL)
		{
			pMeshTile->nTextureID = gdm_GetRasterTextureIDCache()->GetTextureID(pLogicTile->prasterTile);
			nDelta = pMeshTile->nLevel - pLogicTile->prasterTile->m_sInfo.nLevel;
			nX = pMeshTile->nX - (pLogicTile->prasterTile->m_sInfo.nX << nDelta);
			nY = pMeshTile->nY - (pLogicTile->prasterTile->m_sInfo.nY << nDelta);

			width = 1.0 / (1 << nDelta);
			pMeshTile->fTextureLeft = width * nX;			
			pMeshTile->fTextureBottom   = 1 - width * nY;
			pMeshTile->fTextureWidth = width;			
		}


		pMeshTile->nOverlayTextureID = gdm_GetGISTextureIDCache()->GetTextureID(pLogicTile->nLevel,pLogicTile->nX,pLogicTile->nY);
	}

	// set link information

	for (int i = 0; i < pArr->nCount;i++)
	{				
		pMeshTile = &pArr->pData[i];	

		for (j = i+1;j < pArr->nCount;j++)
		{
			pOther = &pArr->pData[j];

			if (pMeshTile->nLevel >= pOther->nLevel)
			{
				// division with same level
				nDelta = pMeshTile->nLevel - pOther->nLevel;
				pOther->nSubs =  1 << nDelta; // get resolution
				pOther->nSubX = pOther->nX<<nDelta;
				pOther->nSubY = pOther->nY<<nDelta;
				pMeshTile->nSubX = pMeshTile->nX;
				pMeshTile->nSubY = pMeshTile->nY;
				pMeshTile->nSubs = 1;
			}
			else
			{
				nDelta = pOther->nLevel - pMeshTile->nLevel;
				pMeshTile->nSubs = 1 << nDelta; // get resolution
				pMeshTile->nSubX = pMeshTile->nX<<nDelta;
				pMeshTile->nSubY = pMeshTile->nY<<nDelta;
				pOther->nSubX = pOther->nX;
				pOther->nSubY = pOther->nY;
				pOther->nSubs = 1;
			}

			if (pOther->nSubs > GDEM_MAX_DEM_DIVISION || pMeshTile->nSubs > GDEM_MAX_DEM_DIVISION)
				continue;

			nMaxX = 1 << qMax(pMeshTile->nLevel, pOther->nLevel);

			if ((pMeshTile->nSubX <= pOther->nSubX) && (pMeshTile->nSubX+pMeshTile->nSubs >= pOther->nSubX+pOther->nSubs)||
				(pMeshTile->nSubX >= pOther->nSubX) && (pMeshTile->nSubX+pMeshTile->nSubs <= pOther->nSubX+pOther->nSubs))
			{
				//first bottom
				if (pMeshTile->nSubY == pOther->nSubY+pOther->nSubs || pOther->nSubY == pMeshTile->nSubY+pMeshTile->nSubs){	

					if (pOther->nDivision < pOther->nSubs)
					{
						pOther->nDivision = pOther->nSubs;
						gdm_MakeExtraBounds(pOther);					
					}

					if ( pMeshTile->nDivision < pMeshTile->nSubs)
					{
						pMeshTile->nDivision = pMeshTile->nSubs;
						gdm_MakeExtraBounds(pMeshTile);					
					}

					pOther->nSubDivision = pOther->nDivision/pOther->nSubs;
					pMeshTile->nSubDivision = pMeshTile->nDivision/pMeshTile->nSubs;					

					if (pMeshTile->nSubY == pOther->nSubY+pOther->nSubs)
						gdm_MatchNeighbor(pMeshTile,pOther,0);										
					else if (pOther->nSubY == pMeshTile->nSubY+pMeshTile->nSubs)
						gdm_MatchNeighbor(pOther, pMeshTile,0);		

					continue;
				}
			}


			if ((pMeshTile->nSubY <= pOther->nSubY) && (pMeshTile->nSubY+pMeshTile->nSubs >= pOther->nSubY+pOther->nSubs)||
				(pMeshTile->nSubY >= pOther->nSubY) && (pMeshTile->nSubY+pMeshTile->nSubs <= pOther->nSubY+pOther->nSubs))
			{
				//left
				if (pMeshTile->nSubX == (pOther->nSubX+pOther->nSubs)%nMaxX || pOther->nSubX == (pMeshTile->nSubX+pMeshTile->nSubs)%nMaxX){						
					if (pOther->nDivision < pOther->nSubs)
					{
						pOther->nDivision = pOther->nSubs;
						gdm_MakeExtraBounds(pOther);					
					}

					if (pMeshTile->nDivision < pMeshTile->nSubs)
					{
						pMeshTile->nDivision = pMeshTile->nSubs;
						gdm_MakeExtraBounds(pMeshTile);					
					}

					pOther->nSubDivision = pOther->nDivision/pOther->nSubs;
					pMeshTile->nSubDivision = pMeshTile->nDivision/pMeshTile->nSubs;					

					if (pMeshTile->nSubX == (pOther->nSubX+pOther->nSubs)%nMaxX )
						gdm_MatchNeighbor(pOther,pMeshTile,1);																							
					else if (pOther->nSubX == (pMeshTile->nSubX+pMeshTile->nSubs)%nMaxX)
						gdm_MatchNeighbor(pMeshTile,pOther,1);
				}	
			}			
		}		
	}
}


