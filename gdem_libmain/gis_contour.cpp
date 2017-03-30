#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "gis_contour.h"
#include "GDM_Define.h"
#include "GDM_Math.h"

/************************************************************************/
/*                        GDALContourGenerator()                        */
/************************************************************************/

GDALContour::GDALContour()
{
}
GDALContour::~GDALContour()
{
}

GDALContourGenerator::GDALContourGenerator( int nWidthIn, int nHeightIn )
{
    nWidth = nWidthIn;
    nHeight = nHeightIn;

    padfLastLine = (double *) calloc(sizeof(double),nWidth);
    padfThisLine = (double *) calloc(sizeof(double),nWidth);

    iLine = -1;

    bNoDataActive = FALSE;
    dfNoDataValue = -1000000.0;
    dfContourInterval = 10.0;
    dfContourOffset = 0.0;

    nLevelMax = 0;
    nLevelCount = 0;
    papoLevels = NULL;
    bFixedLevels = FALSE;
}

/************************************************************************/
/*                       ~GDALContourGenerator()                        */
/************************************************************************/

GDALContourGenerator::~GDALContourGenerator()

{
    int i;

    for( i = 0; i < nLevelCount; i++ )
        delete papoLevels[i];

    free ( papoLevels );
    free ( padfLastLine );
    free ( padfThisLine );
}

/************************************************************************/
/*                           SetFixedLevels()                           */
/************************************************************************/

void GDALContourGenerator::SetFixedLevels( int nFixedLevelCount, 
                                           double *padfFixedLevels )

{
    bFixedLevels = TRUE;
    for( int i = 0; i < nFixedLevelCount; i++ )
        FindLevel( padfFixedLevels[i] );
}

/************************************************************************/
/*                             SetNoData()                              */
/************************************************************************/

void GDALContourGenerator::SetNoData( double dfNewValue )

{
    bNoDataActive = TRUE;
    dfNoDataValue = dfNewValue;
}

/************************************************************************/
/*                            ProcessPixel()                            */
/************************************************************************/

CT_ERR GDALContourGenerator::ProcessPixel( int iPixel )

{
    double  dfUpLeft = 0, dfUpRight = 0, dfLoLeft = 0, dfLoRight = 0;
    int     bSubdivide = FALSE;

/* -------------------------------------------------------------------- */
/*      Collect the four corner pixel values.  Value left or right      */
/*      of the scanline are taken from the nearest pixel on the         */
/*      scanline itself.                                                */
/* -------------------------------------------------------------------- */
    dfUpLeft = padfLastLine[MAX(0,iPixel-1)];
    dfUpRight = padfLastLine[MIN(nWidth-1,iPixel)];
    
    dfLoLeft = padfThisLine[MAX(0,iPixel-1)];
    dfLoRight = padfThisLine[MIN(nWidth-1,iPixel)];

/* -------------------------------------------------------------------- */
/*      Check if we have any nodata values.                             */
/* -------------------------------------------------------------------- */
    if( bNoDataActive 
        && ( dfUpLeft == dfNoDataValue
             || dfLoLeft == dfNoDataValue
             || dfLoRight == dfNoDataValue
             || dfUpRight == dfNoDataValue ) )
        bSubdivide = TRUE;

/* -------------------------------------------------------------------- */
/*      Check if we have any nodata, if so, go to a special case of     */
/*      code.                                                           */
/* -------------------------------------------------------------------- */
    if( iPixel > 0 && iPixel < nWidth 
        && iLine > 0 && iLine < nHeight && !bSubdivide )
    {
        return ProcessRect( dfUpLeft, iPixel - 0.5, iLine - 0.5, 
                            dfLoLeft, iPixel - 0.5, iLine + 0.5, 
                            dfLoRight, iPixel + 0.5, iLine + 0.5, 
                            dfUpRight, iPixel + 0.5, iLine - 0.5 );
    }

/* -------------------------------------------------------------------- */
/*      Prepare subdivisions.                                           */
/* -------------------------------------------------------------------- */
    int nGoodCount = 0; 
    double dfASum = 0.0;
    double dfCenter, dfTop=0.0, dfRight=0.0, dfLeft=0.0, dfBottom=0.0;
    
    if( dfUpLeft != dfNoDataValue )
    {
        dfASum += dfUpLeft;
        nGoodCount++;
    }

    if( dfLoLeft != dfNoDataValue )
    {
        dfASum += dfLoLeft;
        nGoodCount++;
    }

    if( dfLoRight != dfNoDataValue )
    {
        dfASum += dfLoRight;
        nGoodCount++;
    }

    if( dfUpRight != dfNoDataValue )
    {
        dfASum += dfUpRight;
        nGoodCount++;
    }

    if( nGoodCount == 0.0 )
        return CTE_NONE;

    dfCenter = dfASum / nGoodCount;

    if( dfUpLeft != dfNoDataValue )
    {
        if( dfUpRight != dfNoDataValue )
            dfTop = (dfUpLeft + dfUpRight) / 2.0;
        else
            dfTop = dfUpLeft;

        if( dfLoLeft != dfNoDataValue )
            dfLeft = (dfUpLeft + dfLoLeft) / 2.0;
        else
            dfLeft = dfUpLeft;
    }
    else
    {
        dfTop = dfUpRight;
        dfLeft = dfLoLeft;
    }

    if( dfLoRight != dfNoDataValue )
    {
        if( dfUpRight != dfNoDataValue )
            dfRight = (dfLoRight + dfUpRight) / 2.0;
        else
            dfRight = dfLoRight;

        if( dfLoLeft != dfNoDataValue )
            dfBottom = (dfLoRight + dfLoLeft) / 2.0;
        else
            dfBottom = dfLoRight;
    }
    else
    {
        dfBottom = dfLoLeft;;
        dfRight = dfUpRight;
    }

/* -------------------------------------------------------------------- */
/*      Process any quadrants that aren't "nodata" anchored.            */
/* -------------------------------------------------------------------- */
    CT_ERR eErr = CTE_NONE;

    if( dfUpLeft != dfNoDataValue && iPixel > 0 && iLine > 0 )
    {
        eErr = ProcessRect( dfUpLeft, iPixel - 0.5, iLine - 0.5, 
                            dfLeft, iPixel - 0.5, iLine, 
                            dfCenter, iPixel, iLine, 
                            dfTop, iPixel, iLine - 0.5 );
    }

    if( dfLoLeft != dfNoDataValue && eErr == CTE_NONE 
        && iPixel > 0 && iLine < nHeight )
    {
        eErr = ProcessRect( dfLeft, iPixel - 0.5, iLine, 
                            dfLoLeft, iPixel - 0.5, iLine + 0.5,
                            dfBottom, iPixel, iLine + 0.5, 
                            dfCenter, iPixel, iLine );
    }

    if( dfLoRight != dfNoDataValue && iPixel < nWidth && iLine < nHeight )
    {
        eErr = ProcessRect( dfCenter, iPixel, iLine, 
                            dfBottom, iPixel, iLine + 0.5,
                            dfLoRight, iPixel + 0.5, iLine + 0.5, 
                            dfRight, iPixel + 0.5, iLine );
    }

    if( dfUpRight != dfNoDataValue && iPixel < nWidth && iLine > 0 )
    {
        eErr = ProcessRect( dfTop, iPixel, iLine - 0.5, 
                            dfCenter, iPixel, iLine,
                            dfRight, iPixel + 0.5, iLine, 
                            dfUpRight, iPixel + 0.5, iLine - 0.5 );
    }

    return eErr;
}

/************************************************************************/
/*                            ProcessRect()                             */
/************************************************************************/

CT_ERR GDALContourGenerator::ProcessRect( 
    double dfUpLeft, double dfUpLeftX, double dfUpLeftY, 
    double dfLoLeft, double dfLoLeftX, double dfLoLeftY, 
    double dfLoRight, double dfLoRightX, double dfLoRightY, 
    double dfUpRight, double dfUpRightX, double dfUpRightY )
    
{
/* -------------------------------------------------------------------- */
/*      Identify the range of elevations over this rect.                */
/* -------------------------------------------------------------------- */
    int iStartLevel, iEndLevel;

    double dfMin = MIN(MIN(dfUpLeft,dfUpRight),MIN(dfLoLeft,dfLoRight));
    double dfMax = MAX(MAX(dfUpLeft,dfUpRight),MAX(dfLoLeft,dfLoRight));
    

/* -------------------------------------------------------------------- */
/*      Compute the set of levels to compute contours for.              */
/* -------------------------------------------------------------------- */

    /* 
    ** If we are using fixed levels, then find the min/max in the levels
    ** table.
    */
    if( bFixedLevels )
    {
        int nStart=0, nEnd=nLevelCount-1, nMiddle;

        iStartLevel = -1;
        while( nStart <= nEnd )
        {
            nMiddle = (nEnd + nStart) / 2;
            
            double dfMiddleLevel = papoLevels[nMiddle]->GetLevel();
            
            if( dfMiddleLevel < dfMin )
                nStart = nMiddle + 1;
            else if( dfMiddleLevel > dfMin )
                nEnd = nMiddle - 1;
            else
            {
                iStartLevel = nMiddle;
                break;
            }
        }

        if( iStartLevel == -1 )
            iStartLevel = nEnd + 1;

        iEndLevel = iStartLevel;
        while( iEndLevel < nLevelCount-1 
               && papoLevels[iEndLevel+1]->GetLevel() < dfMax )
            iEndLevel++;

        if( iStartLevel >= nLevelCount )
            return CTE_NONE;
    }

    /*
    ** Otherwise figure out the start and end using the base and offset.
    */
    else
    {
        iStartLevel = (int) 
            ceil((dfMin - dfContourOffset) / dfContourInterval);
        iEndLevel = (int)   
            floor((dfMax - dfContourOffset) / dfContourInterval);
    }

    if( iStartLevel > iEndLevel )
        return CTE_NONE;

/* -------------------------------------------------------------------- */
/*      Loop over them.                                                 */
/* -------------------------------------------------------------------- */
    int iLevel;

    for( iLevel = iStartLevel; iLevel <= iEndLevel; iLevel++ )
    {
        double dfLevel;

        if( bFixedLevels )
            dfLevel = papoLevels[iLevel]->GetLevel();
        else
            dfLevel = iLevel * dfContourInterval + dfContourOffset;

        int  nPoints = 0; 
        double adfX[4], adfY[4];
        CT_ERR eErr;

        Intersect( dfUpLeft, dfUpLeftX, dfUpLeftY,
                   dfLoLeft, dfLoLeftX, dfLoLeftY,
                   dfLoRight, dfLevel, &nPoints, adfX, adfY );
        Intersect( dfLoLeft, dfLoLeftX, dfLoLeftY,
                   dfLoRight, dfLoRightX, dfLoRightY,
                   dfUpRight, dfLevel, &nPoints, adfX, adfY );
        Intersect( dfLoRight, dfLoRightX, dfLoRightY,
                   dfUpRight, dfUpRightX, dfUpRightY,
                   dfUpLeft, dfLevel, &nPoints, adfX, adfY );
        Intersect( dfUpRight, dfUpRightX, dfUpRightY,
                   dfUpLeft, dfUpLeftX, dfUpLeftY,
                   dfLoLeft, dfLevel, &nPoints, adfX, adfY );
        
        if( nPoints >= 2 )
        {
            eErr = AddSegment( dfLevel, adfX[0], adfY[0], adfX[1], adfY[1] );

            if( eErr != CTE_NONE )
                return eErr;
        }

        if( nPoints == 4 )
        {
            eErr = AddSegment( dfLevel, adfX[2], adfY[2], adfX[3], adfY[3] );
            if( eErr != CTE_NONE )
                return eErr;
        }
    }

    return CTE_NONE;
}

/************************************************************************/
/*                             Intersect()                              */
/************************************************************************/

void GDALContourGenerator::Intersect( double dfVal1, double dfX1, double dfY1, 
                                      double dfVal2, double dfX2, double dfY2,
                                      double dfNext, 
                                      double dfLevel, int *pnPoints, 
                                      double *padfX, double *padfY )

{
	if (padfX == NULL || padfY == NULL) return;

    if( dfVal1 < dfLevel && dfVal2 >= dfLevel )
    {
        double dfRatio = (dfLevel - dfVal1) / (dfVal2 - dfVal1);

        padfX[*pnPoints] = dfX1 * (1.0 - dfRatio) + dfX2 * dfRatio;
        padfY[*pnPoints] = dfY1 * (1.0 - dfRatio) + dfY2 * dfRatio;
        (*pnPoints)++;
    }
    else if( dfVal1 > dfLevel && dfVal2 <= dfLevel )
    {
        double dfRatio = (dfLevel - dfVal2) / (dfVal1 - dfVal2);

        padfX[*pnPoints] = dfX2 * (1.0 - dfRatio) + dfX1 * dfRatio;
        padfY[*pnPoints] = dfY2 * (1.0 - dfRatio) + dfY1 * dfRatio;
        (*pnPoints)++;
    }
    else if( dfVal1 == dfLevel && dfVal2 == dfLevel && dfNext != dfLevel )
    {
        padfX[*pnPoints] = dfX2;
        padfY[*pnPoints] = dfY2;
        (*pnPoints)++;
    }
}

/************************************************************************/
/*                             AddSegment()                             */
/************************************************************************/

CT_ERR GDALContourGenerator::AddSegment( double dfLevel, 
                                         double dfX1, double dfY1,
                                         double dfX2, double dfY2 )

{
    GDALContourLevel *poLevel = FindLevel( dfLevel );
    GDALContourItem *poTarget;
    int iTarget;

/* -------------------------------------------------------------------- */
/*      Check all active contours for any that this might attach        */
/*      to. Eventually this should be recoded to find the contours      */
/*      of the correct level more efficiently.                          */
/* -------------------------------------------------------------------- */

	if(poLevel == NULL)
		return CTE_NONE;
    if( dfY1 < dfY2 )
        iTarget = poLevel->FindContour( dfX1, dfY1 );
    else
        iTarget = poLevel->FindContour( dfX2, dfY2 );

    if( iTarget != -1 )
    {
        poTarget = poLevel->GetContour( iTarget );

		if (poTarget == NULL) return CTE_NONE;

        poTarget->AddSegment( dfX1, dfY1, dfX2, dfY2 );

        poLevel->AdjustContour( iTarget );
        
        return CTE_NONE;
    }

/* -------------------------------------------------------------------- */
/*      No existing contour found, lets create a new one.               */
/* -------------------------------------------------------------------- */
    poTarget = new GDALContourItem();
	poTarget->dfLevel = dfLevel;

    poTarget->AddSegment( dfX1, dfY1, dfX2, dfY2 );

    poLevel->InsertContour( poTarget );

    return CTE_NONE;
}

/************************************************************************/
/*                              FeedLine()                              */
/************************************************************************/

CT_ERR GDALContourGenerator::FeedLine( double *padfScanline, GDALContour *pContour )

{
/* -------------------------------------------------------------------- */
/*      Switch current line to "lastline" slot, and copy new data       */
/*      into new "this line".                                           */
/* -------------------------------------------------------------------- */
    double *padfTempLine = padfLastLine;
    padfLastLine = padfThisLine;
    padfThisLine = padfTempLine;

/* -------------------------------------------------------------------- */
/*      If this is the end of the lines (NULL passed in), copy the      */
/*      last line.                                                      */
/* -------------------------------------------------------------------- */
    if( padfScanline == NULL )
    {
        memcpy( padfThisLine, padfLastLine, sizeof(double) * nWidth );
    }
    else
    {
        memcpy( padfThisLine, padfScanline, sizeof(double) * nWidth );
    }

/* -------------------------------------------------------------------- */
/*      Perturb any values that occur exactly on level boundaries.      */
/* -------------------------------------------------------------------- */
    int iPixel;

    for( iPixel = 0; iPixel < nWidth; iPixel++ )
    {
        if( bNoDataActive && padfThisLine[iPixel] == dfNoDataValue )
            continue;

        double dfLevel = (padfThisLine[iPixel] - dfContourOffset) 
            / dfContourInterval;

        if( dfLevel - (int) dfLevel == 0.0 )
        {
            padfThisLine[iPixel] += dfContourInterval * FUDGE_EXACT;
        }
    }

/* -------------------------------------------------------------------- */
/*      If this is the first line we need to initialize the previous    */
/*      line from the first line of data.                               */
/* -------------------------------------------------------------------- */
    if( iLine == -1 )
    {
        memcpy( padfLastLine, padfThisLine, sizeof(double) * nWidth );
        iLine = 0;
    }

/* -------------------------------------------------------------------- */
/*      Clear the recently used flags on the contours so we can         */
/*      check later which ones were touched for this scanline.          */
/* -------------------------------------------------------------------- */
    int iLevel, iContour;

    for( iLevel = 0; iLevel < nLevelCount; iLevel++ )
    {
        GDALContourLevel *poLevel = papoLevels[iLevel];

		if (poLevel == NULL)	break;

        for( iContour = 0; iContour < poLevel->GetContourCount(); iContour++ )
            poLevel->GetContour( iContour )->bRecentlyAccessed = FALSE;
    }

/* -------------------------------------------------------------------- */
/*      Process each pixel.                                             */
/* -------------------------------------------------------------------- */
    for( iPixel = 0; iPixel < nWidth+1; iPixel++ )
    {
        CT_ERR eErr = ProcessPixel( iPixel );
        if( eErr != CTE_NONE )
            return eErr;
    }
    
/* -------------------------------------------------------------------- */
/*      eject any pending contours.                                     */
/* -------------------------------------------------------------------- */
    CT_ERR eErr = EjectContours( padfScanline != NULL, pContour );

    iLine++;

    if( iLine == nHeight && eErr == CTE_NONE )
        return FeedLine( NULL, pContour );
    else
        return eErr;
}

/************************************************************************/
/*                           EjectContours()                            */
/************************************************************************/

CT_ERR GDALContourGenerator::EjectContours( int bOnlyUnused, GDALContour *pContour )

{
    int iLevel;
    CT_ERR eErr = CTE_NONE;

	if (pContour == NULL) return CTE_NONE;

/* -------------------------------------------------------------------- */
/*      Process all contours of all levels that match our criteria      */
/* -------------------------------------------------------------------- */
    for( iLevel = 0; iLevel < nLevelCount && eErr == CTE_NONE; iLevel++ )
    {
        GDALContourLevel *poLevel = papoLevels[iLevel];

		if (poLevel == NULL)  continue;

        int iContour;

        for( iContour = 0; 
             iContour < poLevel->GetContourCount() && eErr == CTE_NONE; 
             /* increment in loop if we don't consume it. */ )
        {
            int  iC2;
            GDALContourItem *poTarget = poLevel->GetContour( iContour );

           	if (poTarget == NULL)  continue;

            if( bOnlyUnused && poTarget->bRecentlyAccessed )
            {
                iContour++;
                continue;
            }

            poLevel->RemoveContour( iContour );

            // Try to find another contour we can merge with in this level.
            
            for( iC2 = 0; iC2 < poLevel->GetContourCount(); iC2++ )
            {
                GDALContourItem *poOther = poLevel->GetContour( iC2 );

				if (poOther == NULL)  continue;

                if( poOther->Merge( poTarget ) )
                    break;
            }

            // If we didn't merge it, then eject (write) it out. 
            if( iC2 == poLevel->GetContourCount() )
            {
				int nCount = pContour->nCount;

				if (nCount < MAX_CONTOUR_POLYGON_COUNT)
				{
					memcpy(&pContour->items[nCount], poTarget, sizeof(GDALContourItem));
					pContour->items[nCount].padfX = (double *) malloc (sizeof(double) * poTarget->nPoints);
					pContour->items[nCount].padfY = (double *) malloc (sizeof(double) * poTarget->nPoints);
					pContour->items[nCount].padPt = (GIS_POINT *) malloc (sizeof(GIS_POINT) * poTarget->nPoints);
					memcpy(pContour->items[nCount].padfX, poTarget->padfX, sizeof(double)*poTarget->nPoints);
					memcpy(pContour->items[nCount].padfY, poTarget->padfY, sizeof(double)*poTarget->nPoints);
					pContour->nCount++;
				}
				
            }

			delete poTarget;
        }
    }

    return eErr;
}

/************************************************************************/
/*                             FindLevel()                              */
/************************************************************************/

GDALContourLevel *GDALContourGenerator::FindLevel( double dfLevel )

{
    int nStart=0, nEnd=nLevelCount-1, nMiddle;

/* -------------------------------------------------------------------- */
/*      Binary search to find the requested level.                      */
/* -------------------------------------------------------------------- */
    while( nStart <= nEnd )
    {
        nMiddle = (nEnd + nStart) / 2;

        double dfMiddleLevel = papoLevels[nMiddle]->GetLevel();

        if( dfMiddleLevel < dfLevel )
            nStart = nMiddle + 1;
        else if( dfMiddleLevel > dfLevel )
            nEnd = nMiddle - 1;
        else
            return papoLevels[nMiddle];
    }

/* -------------------------------------------------------------------- */
/*      Didn't find the level, create a new one and insert it in        */
/*      order.                                                          */
/* -------------------------------------------------------------------- */
    GDALContourLevel *poLevel = new GDALContourLevel( dfLevel );

    if( nLevelMax == nLevelCount )
    {
        nLevelMax = nLevelMax * 2 + 10;

		if (papoLevels == NULL)
			papoLevels = (GDALContourLevel **) malloc (sizeof(void*) * nLevelMax);
		else
		{
		//	papoLevels = (GDALContourLevel **) realloc (papoLevels, sizeof(void*) * nLevelMax);
			GDALContourLevel **tmpPapoLevels = (GDALContourLevel **) realloc (papoLevels, sizeof(void*) * nLevelMax);
			if (tmpPapoLevels != NULL)
				papoLevels = tmpPapoLevels;
			else
				free (papoLevels);
		}
    }

	if(papoLevels == NULL)
	{
		delete poLevel;
		poLevel = NULL;
		return NULL;
	}

    if( nLevelCount - nEnd - 1 > 0 )
        memmove( papoLevels + nEnd + 2, papoLevels + nEnd + 1, 
                 (nLevelCount - nEnd - 1) * sizeof(void*) );
    papoLevels[nEnd+1] = poLevel;
    nLevelCount++;

    return poLevel;
}

/************************************************************************/
/* ==================================================================== */
/*                           GDALContourLevel                           */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                          GDALContourLevel()                          */
/************************************************************************/

GDALContourLevel::GDALContourLevel( double dfLevelIn )

{
    dfLevel = dfLevelIn;
    nEntryMax = 0;
    nEntryCount = 0;
    papoEntries = NULL;
}

/************************************************************************/
/*                         ~GDALContourLevel()                          */
/************************************************************************/

GDALContourLevel::~GDALContourLevel()

{
    free (papoEntries);
}

/************************************************************************/
/*                           AdjustContour()                            */
/*                                                                      */
/*      Assume the indicated contour's tail may have changed, and       */
/*      adjust it up or down in the list of contours to re-establish    */
/*      proper ordering.                                                */
/************************************************************************/

void GDALContourLevel::AdjustContour( int iChanged )

{
    while( iChanged > 0 
         && papoEntries[iChanged]->dfTailX < papoEntries[iChanged-1]->dfTailX )
    {
        GDALContourItem *poTemp = papoEntries[iChanged];
        papoEntries[iChanged] = papoEntries[iChanged-1];
        papoEntries[iChanged-1] = poTemp;
        iChanged--;
    }

    while( iChanged < nEntryCount-1
         && papoEntries[iChanged]->dfTailX > papoEntries[iChanged+1]->dfTailX )
    {
        GDALContourItem *poTemp = papoEntries[iChanged];

		if (poTemp == NULL)  continue;

        papoEntries[iChanged] = papoEntries[iChanged+1];
        papoEntries[iChanged+1] = poTemp;
        iChanged++;
    }
}

/************************************************************************/
/*                           RemoveContour()                            */
/************************************************************************/

void GDALContourLevel::RemoveContour( int iTarget )

{
    if( iTarget < nEntryCount )
        memmove( papoEntries + iTarget, papoEntries + iTarget + 1, 
                 (nEntryCount - iTarget - 1) * sizeof(void*) );
    nEntryCount--;
}

/************************************************************************/
/*                            FindContour()                             */
/*                                                                      */
/*      Perform a binary search to find the requested "tail"            */
/*      location.  If not available return -1.  In theory there can     */
/*      be more than one contour with the same tail X and different     */
/*      Y tails ... ensure we check against them all.                   */
/************************************************************************/

int GDALContourLevel::FindContour( double dfX, double dfY )

{
    int nStart = 0, nEnd = nEntryCount-1, nMiddle;

    while( nEnd >= nStart )
    {
        nMiddle = (nEnd + nStart) / 2;

        double dfMiddleX = papoEntries[nMiddle]->dfTailX;

        if( dfMiddleX < dfX )
            nStart = nMiddle + 1;
        else if( dfMiddleX > dfX )
            nEnd = nMiddle - 1;
        else
        {
            while( nMiddle > 0 
                   && fabs(papoEntries[nMiddle]->dfTailX-dfX) < JOIN_DIST )
                nMiddle--;

            while( nMiddle < nEntryCount
                   && fabs(papoEntries[nMiddle]->dfTailX-dfX) < JOIN_DIST )
            {
                if( fabs(papoEntries[nMiddle]->padfY[papoEntries[nMiddle]->nPoints-1] - dfY) < JOIN_DIST )
                    return nMiddle;
                nMiddle++;
            }

            return -1;
        }
    }

    return -1;
}

/************************************************************************/
/*                           InsertContour()                            */
/*                                                                      */
/*      Ensure the newly added contour is placed in order according     */
/*      to the X value relative to the other contours.                  */
/************************************************************************/

int GDALContourLevel::InsertContour( GDALContourItem *poNewContour )

{
/* -------------------------------------------------------------------- */
/*      Find where to insert by binary search.                          */
/* -------------------------------------------------------------------- */
    int nStart = 0, nEnd = nEntryCount-1, nMiddle;

	if (poNewContour == NULL) return -1;
	
    while( nEnd >= nStart )
    {
        nMiddle = (nEnd + nStart) / 2;

        double dfMiddleX = papoEntries[nMiddle]->dfTailX;

        if( dfMiddleX < poNewContour->dfLevel )
            nStart = nMiddle + 1;
        else if( dfMiddleX > poNewContour->dfLevel )
            nEnd = nMiddle - 1;
        else
        {
            nEnd = nMiddle - 1;
            break;
        }
    }

/* -------------------------------------------------------------------- */
/*      Do we need to grow the array?                                   */
/* -------------------------------------------------------------------- */
    if( nEntryMax == nEntryCount )
    {
        nEntryMax = nEntryMax * 2 + 10;

		if (papoEntries == NULL)
			papoEntries = (GDALContourItem **) malloc (sizeof(void*) * nEntryMax);
		else
		{
		//	papoEntries = (GDALContourItem **) realloc (papoEntries, sizeof(void*) * nEntryMax);
			GDALContourItem **tmpPapoEntries = (GDALContourItem **) realloc (papoEntries, sizeof(void*) * nEntryMax);
			if(tmpPapoEntries != NULL)
				papoEntries = tmpPapoEntries;
			else
				free (papoEntries);
		}
    }

/* -------------------------------------------------------------------- */
/*      Insert the new contour at the appropriate location.             */
/* -------------------------------------------------------------------- */
	if(papoEntries == NULL)
	{
		if (poNewContour)
			delete poNewContour;
		return 0;
	}

    if( nEntryCount - nEnd - 1 > 0 )
        memmove( papoEntries + nEnd + 2, papoEntries + nEnd + 1, 
                 (nEntryCount - nEnd - 1) * sizeof(void*) );
    papoEntries[nEnd+1] = poNewContour;
    nEntryCount++;

    return nEnd+1;
}


/************************************************************************/
/* ==================================================================== */
/*                           GDALContourItem                            */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                          GDALContourItem()                           */
/************************************************************************/

GDALContourItem::GDALContourItem()

{
    dfLevel = 0;
    bRecentlyAccessed = FALSE;
    nPoints = 0;
    nMaxPoints = 0;
    padfX = NULL;
    padfY = NULL;
	padPt = NULL;
	padfAngle = NULL;
    
    dfTailX = 0.0;
}

/************************************************************************/
/*                          ~GDALContourItem()                          */
/************************************************************************/

GDALContourItem::~GDALContourItem()

{
    free (padfX);
    free (padfY);
	free (padPt);
	free (padfAngle);
}

/************************************************************************/
/*                             AddSegment()                             */
/************************************************************************/

int GDALContourItem::AddSegment( double dfXStart, double dfYStart, 
                                 double dfXEnd, double dfYEnd )

{
    MakeRoomFor( nPoints + 1 );

/* -------------------------------------------------------------------- */
/*      If there are no segments, just add now.                         */
/* -------------------------------------------------------------------- */
    if( nPoints == 0 )
    {
        nPoints = 2;

        padfX[0] = dfXStart;
        padfY[0] = dfYStart;
        padfX[1] = dfXEnd;
        padfY[1] = dfYEnd;
        bRecentlyAccessed = TRUE;

        dfTailX = padfX[1];

        return TRUE;
    }

/* -------------------------------------------------------------------- */
/*      Try to matching up with one of the ends, and insert.            */
/* -------------------------------------------------------------------- */
    if( fabs(padfX[nPoints-1]-dfXStart) < JOIN_DIST 
             && fabs(padfY[nPoints-1]-dfYStart) < JOIN_DIST )
    {
        padfX[nPoints] = dfXEnd;
        padfY[nPoints] = dfYEnd;
        nPoints++;

        bRecentlyAccessed = TRUE;

        dfTailX = dfXEnd;

        return TRUE;
    }
    else if( fabs(padfX[nPoints-1]-dfXEnd) < JOIN_DIST 
             && fabs(padfY[nPoints-1]-dfYEnd) < JOIN_DIST )
    {
        padfX[nPoints] = dfXStart;
        padfY[nPoints] = dfYStart;
        nPoints++;

        bRecentlyAccessed = TRUE;

        dfTailX = dfXStart;

        return TRUE;
    }
    else
        return FALSE;
}
 
/************************************************************************/
/*                               Merge()                                */
/************************************************************************/

int GDALContourItem::Merge( GDALContourItem *poOther )

{
    if( poOther->dfLevel != dfLevel )
        return FALSE;

/* -------------------------------------------------------------------- */
/*      Try to matching up with one of the ends, and insert.            */
/* -------------------------------------------------------------------- */
    if( fabs(padfX[nPoints-1]-poOther->padfX[0]) < JOIN_DIST 
        && fabs(padfY[nPoints-1]-poOther->padfY[0]) < JOIN_DIST )
    {
        MakeRoomFor( nPoints + poOther->nPoints - 1 );

        memcpy( padfX + nPoints, poOther->padfX + 1, 
                sizeof(double) * (poOther->nPoints-1) );
        memcpy( padfY + nPoints, poOther->padfY + 1, 
                sizeof(double) * (poOther->nPoints-1) );
        nPoints += poOther->nPoints - 1;

        bRecentlyAccessed = TRUE;

        dfTailX = padfX[nPoints-1];

        return TRUE;
    }
    else if( fabs(padfX[0]-poOther->padfX[poOther->nPoints-1]) < JOIN_DIST 
             && fabs(padfY[0]-poOther->padfY[poOther->nPoints-1]) < JOIN_DIST )
    {
        MakeRoomFor( nPoints + poOther->nPoints - 1 );

        memmove( padfX + poOther->nPoints - 1, padfX, 
                sizeof(double) * nPoints );
        memmove( padfY + poOther->nPoints - 1, padfY, 
                sizeof(double) * nPoints );
        memcpy( padfX, poOther->padfX, 
                sizeof(double) * (poOther->nPoints-1) );
        memcpy( padfY, poOther->padfY, 
                sizeof(double) * (poOther->nPoints-1) );
        nPoints += poOther->nPoints - 1;

        bRecentlyAccessed = TRUE;

        dfTailX = padfX[nPoints-1];

        return TRUE;
    }
    else if( fabs(padfX[nPoints-1]-poOther->padfX[poOther->nPoints-1]) < JOIN_DIST 
        && fabs(padfY[nPoints-1]-poOther->padfY[poOther->nPoints-1]) < JOIN_DIST )
    {
        int i;

        MakeRoomFor( nPoints + poOther->nPoints - 1 );

        for( i = 0; i < poOther->nPoints-1; i++ )
        {
            padfX[i+nPoints] = poOther->padfX[poOther->nPoints-i-2];
            padfY[i+nPoints] = poOther->padfY[poOther->nPoints-i-2];
        }

        nPoints += poOther->nPoints - 1;

        bRecentlyAccessed = TRUE;

        dfTailX = padfX[nPoints-1];

        return TRUE;
    }
    else if( fabs(padfX[0]-poOther->padfX[0]) < JOIN_DIST 
        && fabs(padfY[0]-poOther->padfY[0]) < JOIN_DIST )
    {
        int i;

        MakeRoomFor( nPoints + poOther->nPoints - 1 );

        memmove( padfX + poOther->nPoints - 1, padfX, 
                sizeof(double) * nPoints );
        memmove( padfY + poOther->nPoints - 1, padfY, 
                sizeof(double) * nPoints );

        for( i = 0; i < poOther->nPoints-1; i++ )
        {
            padfX[i] = poOther->padfX[poOther->nPoints - i - 1];
            padfY[i] = poOther->padfY[poOther->nPoints - i - 1];
        }

        nPoints += poOther->nPoints - 1;

        bRecentlyAccessed = TRUE;

        dfTailX = padfX[nPoints-1];

        return TRUE;
    }
    else
        return FALSE;
}

/************************************************************************/
/*                            MakeRoomFor()                             */
/************************************************************************/

void GDALContourItem::MakeRoomFor( int nNewPoints )

{
    if( nNewPoints > nMaxPoints )
    {
        nMaxPoints = nNewPoints * 2 + 50;

		if (padfX == NULL)
			padfX = (double *) malloc (sizeof(double) * nMaxPoints);
		else
		{
		//	padfX = (double *) realloc (padfX,sizeof(double) * nMaxPoints);
			double *tmpPadfX = (double *) realloc (padfX,sizeof(double) * nMaxPoints);
			if (tmpPadfX != NULL)
				padfX = tmpPadfX;
			else
				free (padfX);
		}

		if (padfY == NULL)
			padfY = (double *) malloc(sizeof(double) * nMaxPoints);
		else
		{
		//	padfY = (double *) realloc(padfY,sizeof(double) * nMaxPoints);
			double *tmpPadfY = (double *) realloc(padfY,sizeof(double) * nMaxPoints);
			if (tmpPadfY != NULL)
				padfY = tmpPadfY;
			else
				free (padfY);
		}
    }
}

/************************************************************************/
/*                         GenerateLabelInfo ()                         */
/************************************************************************/
void GDALContourItem::GenerateLabelInfo (int bCalc, int nLevel)
{
	int i, nRem;

	int nSkipPoints = SKIP_POINTS_11;
	if (nLevel == 12)
	{
		nSkipPoints = 60;
	}	
	else if (nLevel > 12)
	{
		nSkipPoints = 30;
	}
	
	padfAngle = (double *) malloc (sizeof(double) * nPoints);
	if(padfAngle == NULL)
		return;

	for (i = 0; i < nPoints; i++)
	{
		if (1 != (nRem = i % nSkipPoints))
		{
			padfAngle[i] = gdm_PI;
		}
		else
		{
			padfAngle[i] = 0;
		}
	}
#if 0
	double dfX1, dfY1, dfX2, dfY2;
	double dfA, dfB, *dfAngle;
	double dfNum, dfDen, dfSum;
	int i, j, nRem;

	padfAngle = (double *) malloc (sizeof(double) * nPoints);
	if(padfAngle == NULL)
		return;

	for (i = 0; i < nPoints; i++)
	{
		if (0 != (nRem = i % SKIP_POINTS))
		{
			if (CHECK_POINTS != nRem+1)
				padfAngle[i] = gdm_PI;
			continue;
		}

		if (i+CHECK_POINTS > nPoints)
		{
			padfAngle[i] = gdm_PI;
			continue;
		}

		if (padfX[i] < padfX[i+CHECK_POINTS-1])
		{
			dfX1 = padfX[i];
			dfY1 = padfY[i];
			dfX2 = padfX[i+CHECK_POINTS-1];
			dfY2 = padfY[i+CHECK_POINTS-1];
			dfAngle = &padfAngle[i];
		}
		else
		{
			dfX1 = padfX[i+CHECK_POINTS-1];
			dfY1 = padfY[i+CHECK_POINTS-1];
			dfX2 = padfX[i];
			dfY2 = padfY[i];
			dfAngle = &padfAngle[i+CHECK_POINTS-1];
		}

		if (bCalc)
		{
			padfAngle[i] = gdm_PI;
			padfAngle[i+CHECK_POINTS-1] = gdm_PI;
		}
		else
		{
			padfAngle[i] = 0;
			padfAngle[i+CHECK_POINTS-1] = gdm_PI;
			continue;
		}

		dfA = dfY2 - dfY1;
		dfB = dfX1 - dfX2;
		dfSum = 0;

		for (j = i+1; j < i+CHECK_POINTS-1; j++)
		{
			dfNum = (dfA*(padfX[j] - dfX1) + dfB*( padfY[j] - dfY1 )) * (dfA* (padfX[j] - dfX1) + dfB* (padfY[j] -  dfY1));
			dfDen = (dfA*dfA) + (dfB*dfB);
			dfSum += (dfNum/dfDen);
		}

		if (PERM_THRESHOLD > dfSum)
			*dfAngle = atan2 (dfA, -dfB);
	}
#endif
}

/************************************************************************/
/*                        GDALContourGenerate()                         */
/************************************************************************/

/**
 * Create vector contours from raster DEM.
 *
 * This algorithm will generate contours vectors for the input raster band
 * on the requested set of contour levels.  The vector contours are written
 * to the passed in OGR vector layer.  Also, a NODATA value may be specified
 * to identify pixels that should not be considered in contour line generation.
 *
 * The gdal/apps/gdal_contour.cpp mainline can be used as an example of
 * how to use this function.
 *
 * ALGORITHM RULES

For contouring purposes raster pixel values are assumed to represent a point 
value at the center of the corresponding pixel region.  For the purpose of 
contour generation we virtually connect each pixel center to the values to
the left, right, top and bottom.  We assume that the pixel value is linearly
interpolated between the pixel centers along each line, and determine where
(if any) contour lines will appear onlong these line segements.  Then the
contour crossings are connected.  

This means that contour lines nodes won't actually be on pixel edges, but 
rather along vertical and horizontal lines connecting the pixel centers. 

\verbatim
General Case:

      5 |                  | 3
     -- + ---------------- + -- 
        |                  |
        |                  |
        |                  |
        |                  |
     10 +                  |
        |\                 |
        | \                |
     -- + -+-------------- + -- 
     12 |  10              | 1


Saddle Point:

      5 |                  | 12
     -- + -------------+-- + -- 
        |               \  |
        |                 \|
        |                  + 
        |                  |
        +                  |
        |\                 |
        | \                |
     -- + -+-------------- + -- 
     12 |                  | 1

or:

      5 |                  | 12
     -- + -------------+-- + -- 
        |          __/     |
        |      ___/        |
        |  ___/          __+ 
        | /           __/  |
        +'         __/     |
        |       __/        |
        |   ,__/           |
     -- + -+-------------- + -- 
     12 |                  | 1
\endverbatim

Nodata:

In the "nodata" case we treat the whole nodata pixel as a no-mans land. 
We extend the corner pixels near the nodata out to half way and then
construct extra lines from those points to the center which is assigned
an averaged value from the two nearby points (in this case (12+3+5)/3). 

\verbatim
      5 |                  | 3
     -- + ---------------- + -- 
        |                  |
        |                  |
        |      6.7         |
        |        +---------+ 3
     10 +___     |          
        |   \____+ 10       
        |        |          
     -- + -------+        +    
     12 |       12           (nodata)

\endverbatim

 *
 * @param hBand The band to read raster data from.  The whole band will be 
 * processed.
 *
 * @param dfContourInterval The elevation interval between contours generated.
 * 
 * @param dfContourBase The "base" relative to which contour intervals are 
 * applied.  This is normally zero, but could be different.  To generate 10m 
 * contours at 5, 15, 25, ... the ContourBase would be 5.
 *
 * @param  nFixedLevelCount The number of fixed levels. If this is greater than
 * zero, then fixed levels will be used, and ContourInterval and ContourBase 
 * are ignored.
 *
 * @param padfFixedLevels The list of fixed contour levels at which contours 
 * should be generated.  It will contain FixedLevelCount entries, and may be 
 * NULL if fixed levels are disabled (FixedLevelCount = 0). 
 *
 * @param bUseNoData If TRUE the dfNoDataValue will be used.
 *
 * @param dfNoDataValue the value to use as a "nodata" value. That is, a
 * pixel value which should be ignored in generating contours as if the value
 * of the pixel were not known. 
 *
 * @param hLayer the layer to which new contour vectors will be written. 
 * Each contour will have a LINESTRING geometry attached to it.   This
 * is really of type OGRLayerH, but void * is used to avoid pulling the
 * ogr_api.h file in here. 
 *
 * @param iIDField if not -1 this will be used as a field index to indicate
 * where a unique id should be written for each feature (contour) written.
 * 
 * @param iElevField if not -1 this will be used as a field index to indicate
 * where the elevation value of the contour should be written.
 *
 * @param pfnProgress a GDALProgressFunc that may be used to report progress
 * to the user, or to interrupt the algorithm.  May be NULL if not required.
 * 
 * @param pProgressArg the callback data for the pfnProgress function.
 *
 * @return CTE_NONE on success or CTE_FAILURE if an error occurs.
 */

int GDALFeedLine(short* pHeightData, int nXSize, int nYSize, int nInterval, GDALContour *pContour)
{
	GDALContourGenerator oCG( nXSize, nYSize );

	oCG.SetContourLevels( nInterval, 0 );

	CT_ERR eErr = CTE_NONE;
	int iLine;

	double *padfScanline;
	padfScanline = (double *) malloc (sizeof(double) * nXSize);
	if(padfScanline == NULL)
		return eErr;
	
	for( iLine = 0; iLine < nYSize && eErr == CTE_NONE; iLine++ )
	{
		for( int iWord = 0; iWord < nXSize; iWord++ )
		{
			double nheight = pHeightData[129 * iLine + iWord];
			padfScanline[iWord] = nheight;
		}

		eErr = oCG.FeedLine( padfScanline, pContour );
	}

	return eErr;
}

void GDALGenerateElevationLabelInfo (GDALContour* pContour, int nLevel)
{
	for (int i = 0; i < pContour->nCount; i++)
		pContour->items[i].GenerateLabelInfo (FALSE, nLevel);
}