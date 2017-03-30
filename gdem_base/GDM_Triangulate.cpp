
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "GDM_Triangulate.h"


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

void Delauney_Triangulate(CGDM_LocationArr &Vertexs,TriangleArray &Triangles);
bool isInTriangle(Vector3d p, GDEM_MESH T)
{
	Vector3d    u, v, w;             // triangle vectors

	// get triangle edge vectors and plane normal
	u = T.pt2 - T.pt1;
	v = T.pt3 - T.pt1;	

	double    uu, uv, vv, wu, wv, D;
	uu = u.dotProduct(u);
	uv = u.dotProduct(v);
	vv = v.dotProduct(v);
	w = p - T.pt1;
	wu = w.dotProduct(u);
	wv = w.dotProduct(v);
	D = 1/(uv * uv - uu * vv);

	// get and test parametric coords
	double s, t;
	s = (uv * wv - vv * wu) * D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) * D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;
	return true;
}
int
intersect_RayTriangle(Vector3d rayOrg, Vector3d rayDir, GDEM_MESH T, Vector3d &destPt, double *distance)
{
	Vector3d    u, v, n;             // triangle vectors
	Vector3d    w0, w;          // ray vectors
	double     r, a, b;             // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = T.pt2 - T.pt1;
	v = T.pt3 - T.pt1;	
	n = u.crossProduct(v);
	if (n == Vector3d(0,0,0))            // triangle is degenerate
		return -1;                 // do not deal with this case

	a = -n.dotProduct(rayOrg - T.pt1);
	b = n.dotProduct(rayDir);
	if (fabs(b) < EPSILON) {     // ray is parallel to triangle plane
		if (a == 0)                // ray lies in triangle plane
			return 2;
		else 
			return 0;             // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0 || b > 0)                   // ray goes away from triangle
		return 0;                  // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	destPt = rayOrg + rayDir*r;           // intersect point of ray and plane

	// is I inside T?
	double    uu, uv, vv, wu, wv, D;
	uu = u.dotProduct(u);
	uv = u.dotProduct(v);
	vv = v.dotProduct(v);
	w = destPt - T.pt1;
	wu = w.dotProduct(u);
	wv = w.dotProduct(v);
	D = 1/(uv * uv - uu * vv);

	// get and test parametric coords
	double s, t;
	s = (uv * wv - vv * wu) * D;
	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) * D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	*distance = r;
	return 1;                      // I is in T
}

/// Returns true if the point (p) lies inside the circumcircle made up by points (p1,p2,p3)
/// NOTE: A point on the edge is inside the circumcircle
/// IN		"p"	:Point to check
///		    "p1":First point on circle
///		    "p2":Second point on circle
///		    "p3":Third point on circle
/// OUT		true if p is inside circle
bool InCircle(Vector3d &p, Vector3d &p1, Vector3d &p2, Vector3d &p3)
{
	//Return TRUE if the point (xp,yp) lies inside the circumcircle
	//made up by points (x1,y1) (x2,y2) (x3,y3)
	//NOTE: A point on the edge is inside the circumcircle

	if (fabs(p1.m_tY - p2.m_tY) < EPSILON && fabs(p2.m_tY - p3.m_tY) < EPSILON)
	{
		//INCIRCUM - F - Points are coincident !!
		return 0;
	}
	if (p.equals(p1) || p.equals(p2) || p.equals(p3))
		return true;

	double m1, m2;
	double mx1, mx2;
	double my1, my2;
	double xc, yc;

	if (fabs(p2.m_tY - p1.m_tY) < EPSILON)
	{
		m2 = -(p3.m_tX - p2.m_tX) / (p3.m_tY - p2.m_tY);
		mx2 = (p2.m_tX + p3.m_tX) * 0.5f;
		my2 = (p2.m_tY + p3.m_tY) * 0.5f;
		//Calculate CircumCircle center (xc,yc)
		xc = (p2.m_tX + p1.m_tX) * 0.5f;
		yc = m2 * (xc - mx2) + my2;
	}
	else if (fabs(p3.m_tY - p2.m_tY) < EPSILON)
	{
		m1 = -(p2.m_tX - p1.m_tX) / (p2.m_tY - p1.m_tY);
		mx1 = (p1.m_tX + p2.m_tX) * 0.5f;
		my1 = (p1.m_tY + p2.m_tY) * 0.5f;
		//Calculate CircumCircle center (xc,yc)
		xc = (p3.m_tX + p2.m_tX) * 0.5f;
		yc = m1 * (xc - mx1) + my1;
	}
	else
	{
		m1 = -(p2.m_tX - p1.m_tX) / (p2.m_tY - p1.m_tY);
		m2 = -(p3.m_tX - p2.m_tX) / (p3.m_tY - p2.m_tY);
		mx1 = (p1.m_tX + p2.m_tX) * 0.5f;
		mx2 = (p2.m_tX + p3.m_tX) * 0.5f;
		my1 = (p1.m_tY + p2.m_tY) * 0.5f;
		my2 = (p2.m_tY + p3.m_tY) * 0.5f;
		//Calculate CircumCircle center (xc,yc)
		xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		yc = m1 * (xc - mx1) + my1;
	}

	double dx = p2.m_tX - xc;
	double dy = p2.m_tY - yc;
	double rsqr = dx * dx + dy * dy;
	//double r = sqrt(rsqr); //Circumcircle radius
	dx = p.m_tX - xc;
	dy = p.m_tY - yc;
	double drsqr = dx * dx + dy * dy - rsqr;

	return (drsqr <= 0);
}

/// <summary>
/// Determines if a given point is located inside of a shape.
/// </summary>
/// <param name="point">The point to test.</param>
/// <param name="verts">The vertices of the shape.</param>
/// <returns>True if the point is in the shape; false otherwise.</returns>

bool IsPointInShape(Vector3d point, Vector3d *verts, int nVertex)
{
	/* http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/ */

	bool oddNodes = false;

	int j = nVertex - 1;
	double x = point.m_tX;
	double y = point.m_tY;

	for (int i = 0; i < nVertex; i++)
	{
		Vector3d tpi = verts[i];
		Vector3d tpj = verts[j];

		if (tpi.m_tY < y && tpj.m_tY >= y || tpj.m_tY < y && tpi.m_tY >= y)
			if (tpi.m_tX + (y - tpi.m_tY) / (tpj.m_tY - tpi.m_tY) * (tpj.m_tX - tpi.m_tX) < x)
				oddNodes = !oddNodes;

		j = i;
	}

	return oddNodes;
}

double Area(CGDM_LocationArr &contour)
{
	int n = (int)contour.size();

	double A = 0.0f;

	for(int p=n-1,q=0; q<n; p=q++)
	{
		A+= contour[p].m_tX*contour[q].m_tY - contour[q].m_tX*contour[p].m_tY;
	}
	return A*0.5f;
}

/*
//InsideTriangle decides if a point P is Inside of the triangle
//defined by A, B, C.
//*/
bool InsideTriangle(double Ax, double Ay,
					double Bx, double By,
					double Cx, double Cy,
					double Px, double Py)

{
	double ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
	double cCROSSap, bCROSScp, aCROSSbp;

	ax = Cx - Bx;  ay = Cy - By;
	bx = Ax - Cx;  by = Ay - Cy;
	cx = Bx - Ax;  cy = By - Ay;
	apx= Px - Ax;  apy= Py - Ay;
	bpx= Px - Bx;  bpy= Py - By;
	cpx= Px - Cx;  cpy= Py - Cy;

	aCROSSbp = ax*bpy - ay*bpx;
	cCROSSap = cx*apy - cy*apx;
	bCROSScp = bx*cpy - by*cpx;

	return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool Snip(CGDM_LocationArr &contour,int u,int v,int w,int n,int *V)
{
	int p;
	double Ax, Ay, Bx, By, Cx, Cy, Px, Py;

	Ax = contour[V[u]].m_tX;
	Ay = contour[V[u]].m_tY;

	Bx = contour[V[v]].m_tX;
	By = contour[V[v]].m_tY;

	Cx = contour[V[w]].m_tX;
	Cy = contour[V[w]].m_tY;

	if ( EPSILON > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax)))) 
		return false;

	for (p=0;p<n;p++)
	{
		if( (p == u) || (p == v) || (p == w) )
			continue;
		Px = contour[V[p]].m_tX;
		Py = contour[V[p]].m_tY;

		if (IS_IDENTICAL(Px, Ax) && IS_IDENTICAL(Py, Ay) || 
			IS_IDENTICAL(Px, Bx) && IS_IDENTICAL(Py, By) ||
			IS_IDENTICAL(Px, Cx) && IS_IDENTICAL(Py, Cy))
			continue;

		if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) 
			return false;
	}
	return true;
}

bool IsCCW(Vector3d &A,Vector3d &B,Vector3d &C)
{
	if ( EPSILON > (((B.m_tX-A.m_tX)*(C.m_tY-A.m_tY)) - ((B.m_tY-A.m_tY)*(C.m_tX-A.m_tX)))) 
		return false;
	return true;
}




//#define IS_IDENTICAL(a, b)	(a == b)

////---------------------------------------------------------
////	Return true if a point (xp,yp) is inside the circumcircle made up
////	of the points (x1,y1), (x2,y2), (x3,y3)
////	The circumcircle centre is returned in (xc,yc) and the radius r
////	NOTE: A point on the edge is inside the circumcircle
//---------------------------------------------------------
int CircumCircle(double xp, double yp,
				 double x1, double y1,
				 double x2, double y2, 
				 double x3, double y3,
				 double *xc, double *yc,
				 double *r)
{
	double	m1, m2, mx1, mx2, my1, my2,
		dx, dy, rsqr, drsqr;

	// Check for coincident points
	if( IS_IDENTICAL(y1, y2) && IS_IDENTICAL(y2, y3) )
	{
		return 1;
	}

	//-----------------------------------------------------
	if( IS_IDENTICAL(y2, y1) )
	{
		m2	= -(x3 - x2) / (y3 - y2);
		mx2	=  (x2 + x3) / 2.0f;
		my2	=  (y2 + y3) / 2.0f;

		*xc	=  (x2 + x1) / 2.0f;
		*yc	= m2 * (*xc - mx2) + my2;
	}
	else if( IS_IDENTICAL(y3, y2) )
	{
		m1	= -(x2 - x1) / (y2 - y1);
		mx1	=  (x1 + x2) / 2.0f;
		my1	=  (y1 + y2) / 2.0f;

		*xc	=  (x3 + x2) / 2.0f;
		*yc	= m1 * (*xc - mx1) + my1;
	}
	else
	{
		m1	= -(x2 - x1) / (y2 - y1);
		m2	= -(x3 - x2) / (y3 - y2);
		mx1	=  (x1 + x2) / 2.0f;
		mx2	=  (x2 + x3) / 2.0f;
		my1	=  (y1 + y2) / 2.0f;
		my2	=  (y2 + y3) / 2.0f;

		*xc	= (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		*yc	= m1 * (*xc - mx1) + my1;
	}

	//-----------------------------------------------------
	dx		= x2 - *xc;
	dy		= y2 - *yc;
	rsqr	= dx*dx + dy*dy;
	*r		= sqrtf(rsqr);

	dx		= xp - *xc;
	dy		= yp - *yc;
	drsqr	= dx*dx + dy*dy;

	return (drsqr - rsqr<0)? -1:1;
}

void SwapTest(TriangleArray &src, TriangleArray &result,CGDM_LocationArr &Vertexs, int index)
{
	int a,nv = Vertexs.size();
	Edge e;
	bool flag = false;
	int i;
	for (i = src.size()-1;i > index;i--)
	{
		a = src[i].OppsiteVertex(src[index],e);
		if (a >= 0 && InCircle(Vertexs[a], Vertexs[src[index].p1],Vertexs[src[index].p2],Vertexs[src[index].p3]))
		{				
			src[i].Set(a,e.side1,e.p1);
			src[index].Set(a,e.p2,e.side1);
			flag = true;
			break;
		}
	}
	if (!flag)
		return;

	if (src[i].isDelauney(Vertexs)){
		result.push_back(src[i]);
		src.erase(i);	
	}else
		SwapTest(src,result,Vertexs,i);

	if (src[index].isDelauney(Vertexs)){
		result.push_back(src[index]);
		src.erase(index);	
		return;
	}	
	//SwapTest(src,result,Vertexs,index);
}


void CheckTriangles(CGDM_LocationArr &Vertexs,TriangleArray &Triangles)
{		
	int nv = Vertexs.size(), nt = Triangles.size();	
	TriangleArray result;
	result.reallocate(Triangles.size()*2);

	for (int j = nt-2; j >= 0; j--)
	{		
		if (Triangles[j].isDelauney(Vertexs)){
			result.push_back(Triangles[j]);
			Triangles.erase(j);			
			continue;
		}
		SwapTest(Triangles,result,Vertexs,j);		
	}
	
	for (int i = 0; i < (int)result.size();i++)
		Triangles.push_back(result[i]);
}

/**
detecting-line-to-line-intersection taken from http://processinghacks.com/hacks:detecting-line-to-line-intersection
@author Ryan Alexander
*/

// Infinite Line Intersection
bool lineIntersection(double x1, double y1, double x2, double y2, 
					  double x3, double y3, double x4, double y4,
					  double &xi, double &yi)
{
	double bx = x2 - x1;
	double by = y2 - y1;
	double dx = x4 - x3;
	double dy = y4 - y3;

	double b_dot_d_perp = bx*dy - by*dx;

	if(b_dot_d_perp == 0) return 0;

	double cx = x3-x1;
	double cy = y3-y1;

	double t = (cx*dy - cy*dx) / b_dot_d_perp;

	xi = x1+t*bx;
	yi = y1+t*by;

	return 1;
}


//  Determines the intersection point of the line segment defined by points A and B
//  with the line segment defined by points C and D.
//
//  Returns YES if the intersection point was found, and stores that point in X,Y.
//  Returns NO if there is no determinable intersection point, in which case X,Y will
//  be unmodified.
#define NO 0
#define YES 1
bool lineSegmentIntersection(
							 double Ax, double Ay,
							 double Bx, double By,
							 double Cx, double Cy,
							 double Dx, double Dy,
							 double &X, double &Y)
{

	double  distAB, theCos, theSin, newX, ABpos ;

	//  Fail if either line segment is zero-length.
	if (Ax==Bx && Ay==By || Cx==Dx && Cy==Dy) return NO;

	//  Fail if the segments share an end-point.
	if (Ax==Cx && Ay==Cy || Bx==Cx && By==Cy
		||  Ax==Dx && Ay==Dy || Bx==Dx && By==Dy) {
			return NO; }

	//  (1) Translate the system so that point A is on the origin.
	Bx-=Ax; By-=Ay;
	Cx-=Ax; Cy-=Ay;
	Dx-=Ax; Dy-=Ay;

	//  Discover the length of segment A-B.
	distAB=sqrt(Bx*Bx+By*By);

	//  (2) Rotate the system so that point B is on the positive X axis.
	theCos=Bx/distAB;
	theSin=By/distAB;
	newX=Cx*theCos+Cy*theSin;
	Cy  =Cy*theCos-Cx*theSin; Cx=newX;
	newX=Dx*theCos+Dy*theSin;
	Dy  =Dy*theCos-Dx*theSin; Dx=newX;

	//  Fail if segment C-D doesn't cross line A-B.
	if (Cy<0. && Dy<0. || Cy>=0. && Dy>=0.) return NO;

	//  (3) Discover the position of the intersection point along line A-B.
	ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

	//  Fail if segment C-D crosses line A-B outside of segment A-B.
	if (ABpos<0. || ABpos>distAB) return NO;

	//  (4) Apply the discovered position to line A-B in the original coordinate system.
	X=Ax+ABpos*theCos;
	Y=Ay+ABpos*theSin;

	//  Success.
	return YES;
} 


// Line Segment Intersection
bool segIntersection(double x1, double y1, double x2, double y2, 
					 double x3, double y3, double x4, double y4,
					 double &xi, double &yi)
{
	double bx = x2-x1;
	double by = y2-y1;
	double dx = x4-x3;
	double dy = y4-y3;

	double denominator = dy*bx-dx*by;
	if (IS_IDENTICAL(denominator,0))
		return 0;
	double cx = x1-x3;
	double cy = y1-y3;
	double t = (dx*cy-dy*cx)/denominator;
	double s = (bx*cy-by*cx)/denominator;

	if(t < 0 || t > 1) return 0;
	if(s < 0 || s > 1) return 0;

	xi = x1+t*bx;
	yi = y1+t*by;

	return 1;
}


int isInVertexArray(CGDM_LocationArr &pArray, Vector3d pt, int index = 0)
{
	int n = 0;
	for (int i=index;i<(int)pArray.size();i++)
	{
		if (IS_IDENTICAL(pArray[i].m_tX, pt.m_tX) && IS_IDENTICAL(pArray[i].m_tY, pt.m_tY))		
			return i;
	}

	if (index > 0){
		for (int i=0;i<index-1;i++)
		{
			if (IS_IDENTICAL(pArray[i].m_tX, pt.m_tX) && IS_IDENTICAL(pArray[i].m_tY, pt.m_tY))		
				return i;
		}
	}

	return -1;
}

int isCoLinear(double x1, double y1, double x2, double y2, 
			   double x3, double y3)
{
	if (IS_IDENTICAL(x1,x2) && IS_IDENTICAL(y1,y2) || 
		IS_IDENTICAL(x3,x2) && IS_IDENTICAL(y3,y2))
		return 2;

	double d1 = x2-x1;
	double d2 = x3-x2;

	if (IS_IDENTICAL(d1,0) && IS_IDENTICAL(d2,0) || 
		IS_IDENTICAL(y2-y1, 0) && IS_IDENTICAL(y3-y2,0))
		return 1;

	if (d1==0)
		d1 = EPSILON;
	if (d2==0)
		d2 = EPSILON;

	return IS_IDENTICAL((y2-y1)/d1,(y3-y2)/d2);
}

void ValidatePoints(CGDM_LocationArr &ptArray)
{
	int n = ptArray.size();
	for (int i=0;i<n;i++)
	{		
		if (i+1<n && IS_IDENTICAL_VERTEX(ptArray[i], ptArray[i+1]))		
		{
			ptArray.erase(i+1);			
			i--;
			n--;
			continue;
		}		

		if (i+2 < n && IS_IDENTICAL_VERTEX(ptArray[i], ptArray[i+2]))		
		{
			ptArray.erase(i+1);			
			i--;
			n--;			
		}	
	}	
}

int  getPointOrientation(Vector3d &start, Vector3d &end, Vector3d &pt) 
{
	double ori = ( (end.m_tX - start.m_tX) * (pt.m_tY - start.m_tY) -
		(pt.m_tX - start.m_tX) * (end.m_tY - start.m_tY) );
	if (IS_IDENTICAL(ori,0))
		return 0;
	if (ori < 0)
		return -1;
	return 1;
}

bool isPointinLine(double x1, double y1, double x2, double y2, 
				   double x3, double y3)
{
	double minx, maxx;
	minx = min(x1, x3);
	maxx = max(x1, x3);

	if (x2 < minx || x2 > maxx)
		return 0;

	minx = min(y1, y3);
	maxx = max(y1, y3);

	if (y2 < minx || y2 > maxx)
		return 0;

	return 1;
}
bool isNearPoint(CGDM_LocationArr &pArray,Vector3d pa,Vector3d &pb)
{
	int k,n = pArray.size();
	for (int i = 0;i < n;i++)
	{
		k = (i+1)%n;
		if (IS_IDENTICAL_VERTEX(pArray[i], pa) && IS_IDENTICAL_VERTEX(pArray[k], pb) ||
			IS_IDENTICAL_VERTEX(pArray[i], pb) && IS_IDENTICAL_VERTEX(pArray[k], pa))
			return true;
	}
	return false;
}


void CheckIntersection(CGDM_LocationArr &ptArray,CGDM_LocationArr &iArray)
{	
	iArray.set_used(0);
	int n = ptArray.size();

	while(IS_IDENTICAL_VERTEX(ptArray[0], ptArray[n-1])){
		ptArray.erase(n-1);
		n--;
	}		

	ValidatePoints(ptArray)	;	
	bool b1, b2;
	Vector3d p;
	double x,y;
	int k;

	n = ptArray.size();
	for (int i = 0; i < n;i++)
	{			
		for (int j = 0;j < n;j++)
		{	
			k = (i+1)%n;
			if (j == i || j == k)
				continue;		

			if (isCoLinear(ptArray[i].m_tX,ptArray[i].m_tY,ptArray[j].m_tX,ptArray[j].m_tY, ptArray[k].m_tX, ptArray[k].m_tY) == 1)
			{					
				if (isPointinLine(ptArray[i].m_tX,ptArray[i].m_tY,ptArray[j].m_tX,ptArray[j].m_tY, ptArray[k].m_tX, ptArray[k].m_tY))
				{
					ptArray.insert(ptArray[j],i+1);							
					n++;
				}else
				{
					if (j == (i+2)%n && isPointinLine(ptArray[k].m_tX,ptArray[k].m_tY,ptArray[i].m_tX,ptArray[i].m_tY, ptArray[j].m_tX, ptArray[j].m_tY))
					{
						ptArray.erase(k);
						n--;
						continue;
					}
				}

			}
		}
	}

	b1 = b2 = 0;
	int d1, d2;
	for (int i = 0; i < n-1;i++)
	{			
		for (int j = i+2;j < n;j++){
			k = (j+1)%n;
			d1 = getPointOrientation(ptArray[i],ptArray[i+1], ptArray[j]);
			d2 = getPointOrientation(ptArray[i],ptArray[i+1], ptArray[k]);
			if (!d1 || !d2 || d1 == d2)
			{
				continue;
			}

			d1 = getPointOrientation(ptArray[j],ptArray[k], ptArray[i]);
			d2 = getPointOrientation(ptArray[j],ptArray[k], ptArray[i+1]);
			if (!d1 || !d2 || d1 == d2)
			{
				continue;
			}

			if (segIntersection(ptArray[i].m_tX,ptArray[i].m_tY, ptArray[i+1].m_tX,ptArray[i+1].m_tY,
				ptArray[j].m_tX,ptArray[j].m_tY, 	 ptArray[k].m_tX, ptArray[k].m_tY, x, y))
			{
				p.set((double)x,(double)y,0);				
				ptArray.insert(p,j+1);																		
				ptArray.insert(p,i+1);					
				j++;					
				n+=2;				
			}			
		}
	}

	//ValidatePoints(ptArray);	
	for (int i = 0;i< n-3;i++){
		k = 0;
		for (int j=i+3;j<n;j++){
			if (IS_IDENTICAL_VERTEX(ptArray[i], ptArray[j])){
				iArray.push_back(ptArray[j]);						
				k++;
			}
		}
		if (k > 0)
			iArray.push_back(ptArray[i]);						
	}
}


int getClosedPolygon(CGDM_LocationArr &pArray, CGDM_LocationArr &iArray, CGDM_LocationArr &result, int base = 0)
{
	int n = pArray.size();
	bool flag = 0;
	int i, j, index;
	result.set_used(0);
	if (!iArray.size() || !n)
		return -1;

	index = isInVertexArray(pArray, iArray[0],base);
	result.push_back(pArray[index]);
	i = index+1;	

	n = pArray.size();
	while(1)
	{
		if (i == n)
			i = 0;
		if (IS_IDENTICAL_VERTEX(pArray[i] , result[0]) || !n)		
			break;

		result.push_back(pArray[i]);		
		index = isInVertexArray(iArray, pArray[i]);
		if (index >= 0)
		{
			j = isInVertexArray(pArray, pArray[i],i+1);
			iArray.erase(index);
			i = j+1;
		}
		else
		{
			pArray.erase(i);
			n--;
		}		
	}	
	iArray.erase(0);
	index = isInVertexArray(iArray, result[0]);	
	base = 0;
	if (index >= 0)
		base = i;
	else
	{
		while(1)
		{
			index = isInVertexArray(pArray, result[0]);	
			if (index < 0)
				break;
			pArray.erase(index);
		}
	}
	return base;
}

bool NormalTriangluate(CGDM_LocationArr &ptArray,TriangleArray &result)
{
	int n = (int)ptArray.size();
	if ( n < 3 ) return false;

	int *V = new int[n];
	if ( 0.0f < Area(ptArray) ) {
		for (int v=0; v<n; v++) {V[v] = v;}
	}else{
		for(int v=0; v<n; v++) {V[v] = (n-1)-v;}
	}

	int nv = n;

	/*  remove nv-2 Vertices, creating 1 triangle every time */
	int count = 2*nv;   /* error detection */
	Triangle tri;
	int u,v,w;
	int a,b,c,s,t;

	result.reallocate(nv+10);
	result.set_used(0);

	for(v=nv-1; nv>2; )
	{
		/* if we loop, it is probably a non-simple polygon */
		if (0 >= (count--))
		{
			//** Triangulate: ERROR - probable bad polygon!
			delete V;
			result.set_used(0);			
			return false;
		}

		/* three consecutive vertices in current polygon, <u,v,w> */
		u = v; 
		if (nv <= u) u = 0;     /* previous */
		v = u+1; 
		if (nv <= v) v = 0;     /* new v    */
		w = v+1; 
		if (nv <= w) w = 0;     /* next     */

		if ( Snip(ptArray,u,v,w,nv,V) )
		{
			/* true names of the vertices */
			a = V[u]; b = V[v]; c = V[w];

			/* output Triangle */			
			tri.p1 = a;
			tri.p2 = b;
			tri.p3 = c;
			result.push_back(tri);

			/* remove v from remaining polygon */
			for(s=v,t=v+1;t<nv;s++,t++) 
				V[s] = V[t]; 

			nv--;
			/* resest error detection counter */
			count = 2*nv;
		}
	}
	delete [] V;
	CheckTriangles(ptArray,result);	
	return true;
}


void polygon2Polygons(CGDM_LocationArr &pArray,CGDM_LocationPolygonArr &polyArray)
{
	int n = (int)pArray.size();
	if ( n < 3 ) 
		return ;
		
	CGDM_LocationArr ptArray = pArray,iArray(n);
	CheckIntersection(ptArray,iArray);	

	n = ptArray.size();
	polyArray.reallocate(iArray.size()+1);
	polyArray.set_used(0);
	
	CGDM_LocationArr parray(n);	
	int base = 0;
	if (iArray.size())
	{			
		parray.set_used(0);
		base = getClosedPolygon(ptArray, iArray,parray);
		while (base >= 0)
		{				
			polyArray.push_back(parray);
			base = getClosedPolygon(ptArray, iArray,parray,base);
		}				
		return ;
	}
	polyArray.push_back(pArray);	
	return ;
}

bool polygonTriangulate(CGDM_LocationArr &contour,Triangle3DArray &triArray)
{
	int n = (int)contour.size();
	TriangleArray result(n+n);	
	GDEM_MESH mesh;
	Vector3d u,v,w;

	CGDM_LocationPolygonArr polyArray;
	polygon2Polygons(contour,polyArray);

	CGDM_LocationArr parray;	
	triArray.reallocate(n+n);

	for (int i=0;i<(int)polyArray.size();i++)
	{
		parray = polyArray[i];
		result.set_used(0);
		if (!NormalTriangluate(parray, result))
			Delauney_Triangulate(parray,result);	

		for (int j=0;j<(int)result.size();j++){						
			u = parray[result[j].p1];
			v = parray[result[j].p2];
			w = parray[result[j].p3];
			if (!IsCCW(u,v,w))
			{
				mesh.pt1 = w;
				mesh.pt2 = v;
				mesh.pt3 = u;
			}else{
				mesh.pt1 = u;
				mesh.pt2 = v;
				mesh.pt3 = w;
			}
			triArray.push_back(mesh);
		}
	}

	return true;
}


bool IsValidTriangle(CGDM_LocationArr &Vertexs, Triangle &tri)
{
	if (IS_IDENTICAL_VERTEX(Vertexs[tri.p1], Vertexs[tri.p2]) || 
		IS_IDENTICAL_VERTEX(Vertexs[tri.p2], Vertexs[tri.p3]) ||
		IS_IDENTICAL_VERTEX(Vertexs[tri.p1], Vertexs[tri.p3]))
		return 0;
	return 1;
}

bool IsValidEdge(Edge *edge, int nMax)
{
	int n = abs(edge->p1-edge->p2);
	if (edge->side1 >= 0 && edge->side2 >= 0 || n == 1 || n == nMax)
		return TRUE;		
	return FALSE;
}
void CheckPointOrder2(CGDM_LocationArr &Vertexs,TriangleArray &Triangles)
{
	Triangle tri;	
	Edge edge, *e1, *e2;
	int nMax = Vertexs.size()-1;
	EdgeArray edges(nMax+nMax);
	for (int i = 0;i < (int)Triangles.size();i++)
	{
		tri = Triangles[i];
		if (!IsValidTriangle(Vertexs, tri))
			continue;
		edges.push_back(tri.p1, tri.p2, tri.p3);
		edges.push_back(tri.p2, tri.p3, tri.p1);
		edges.push_back(tri.p3, tri.p1, tri.p2);
	}
	Triangles.set_used(0);
	for (int i = 0;i < edges.nCount;i++)
	{
		edge = edges[i];
		if (IsValidEdge(&edge,nMax))
		{
			if (edge.side1 >= 0)
			{
				e1 = edges.Find(edge.p1, edge.side1);
				e2 = edges.Find(edge.p2, edge.side1);
				if (e1 && e2 && IsValidEdge(e1,nMax) && IsValidEdge(e2,nMax))
					Triangles.push_back(edge.p1, edge.p2, edge.side1);
			}
			if (edge.side2 >= 0)
			{
				e1 = edges.Find(edge.p1, edge.side2);
				e2 = edges.Find(edge.p2, edge.side2);
				if (e1 && e2 && IsValidEdge(e1,nMax) && IsValidEdge(e2,nMax))
					Triangles.push_back(edge.p1, edge.p2, edge.side2);
			}
			edges.erase(i);
			i--;
		}
	}	
}

void Delauney_Triangulate(CGDM_LocationArr &Vertexs,TriangleArray &Triangles)
{
	int nv = (int)Vertexs.size();
	if (nv < 3)		
		return;

	int trimax = 4 * nv;
	Triangles.set_used(0);

	// Find the maximum and minimum vertex bounds.
	// This is to allow calculation of the bounding supertriangle
	double xmin = Vertexs[0].m_tX;
	double ymin = Vertexs[0].m_tY;
	double xmax = xmin;
	double ymax = ymin;
	for (int i = 1; i < nv; i++)
	{
		if (Vertexs[i].m_tX < xmin) xmin = Vertexs[i].m_tX;
		if (Vertexs[i].m_tX > xmax) xmax = Vertexs[i].m_tX;
		if (Vertexs[i].m_tY < ymin) ymin = Vertexs[i].m_tY;
		if (Vertexs[i].m_tY > ymax) ymax = Vertexs[i].m_tY;
	}

	double dx = xmax - xmin;
	double dy = ymax - ymin;
	double dmax = (dx > dy) ? dx : dy;

	double xmid = (xmax + xmin) * 0.5f;
	double ymid = (ymax + ymin) * 0.5f;

	// Set up the supertriangle
	// This is a triangle which encompasses all the sample points.
	// The supertriangle coordinates are added to the end of the
	// vertex list. The supertriangle is the first triangle in
	// the triangle list.

	Vector3d v;
	v.set((xmid - 2 * dmax), (ymid - dmax),0);
	Vertexs.push_back(v);
	v.set(xmid, (ymid + 2 * dmax),0);
	Vertexs.push_back(v);
	v.set((xmid + 2 * dmax), (ymid - dmax),0);
	Vertexs.push_back(v);

	Triangles.push_back(nv, nv + 1, nv + 2); //SuperTriangle placed at index 0

	EdgeArray Edges(nv+nv);	
	// Include each point one at a time into the existing mesh
	for (int i = 0; i < nv; i++)
	{		
		// Set up the edge buffer.
		// If the point (Vertexs(i).m_tX,Vertexs(i).m_tY) lies inside the circumcircle then the
		// three edges of that triangle are added to the edge buffer and the triangle is removed from list.

		for (int j = 0; j < (int)Triangles.size(); j++)
		{
			if (InCircle(Vertexs[i], Vertexs[Triangles[j].p1], Vertexs[Triangles[j].p2], Vertexs[Triangles[j].p3]))
			{
				Edges.push_back(Triangles[j].p1, Triangles[j].p2);
				Edges.push_back(Triangles[j].p2, Triangles[j].p3);
				Edges.push_back(Triangles[j].p3, Triangles[j].p1);
				Triangles.erase(j);
				j--;
			}
		}
		if (i >= nv) continue; 
		//In case we the last duplicate point we removed was the last in the array

		// Remove duplicate edges
		// Note: if all triangles are specified anticlockwise then all
		// interior edges are opposite pointing in direction.
		for (int j = Edges.nCount - 2; j >= 0; j--)
		{
			for (int k = Edges.nCount - 1; k >= j + 1; k--)
			{
				if (Edges[j].Equals(Edges[k]))
				{
					Edges.erase(k);
					Edges.erase(j);
				}
			}
		}
		// Form new triangles for the current point
		// Skipping over any tagged edges.
		// All edges are arranged in clockwise order.
		for (int j = 0; j < Edges.nCount; j++)
		{
			Triangles.push_back(Edges[j].p1, Edges[j].p2, i);
		}
		Edges.set_used(0);
	}
	// Remove triangles with supertriangle vertices
	// These are triangles which have a vertex number greater than nv
	for (int i = Triangles.size() - 1; i >= 0; i--)
	{
		if (Triangles[i].p1 >= nv || Triangles[i].p2 >= nv || Triangles[i].p3 >= nv)
			Triangles.erase(i);
	}

	//Remove SuperTriangle vertices
	Vertexs.erase(Vertexs.size()-3,3);	
	CheckPointOrder2(Vertexs, Triangles);
}
