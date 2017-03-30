/**********************************************************************/
/************ HEADER FILE FOR TRIANGULATE.H ***************************/
/**********************************************************************/
#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include "GDM_Vector3D.h"

/*****************************************************************/
/** Static class to triangulate any contour/polygon efficiently **/
/** You should replace CGDM_Location3D with whatever your own Vector   **/
/** class might be.  Does not support polygons with holes.      **/
/** Uses STL vectors to represent a dynamic array of vertices.  **/
/** This code snippet was submitted to FlipCode.com by          **/
/** John W. Ratcliff (jratcliff@verant.com) on July 22, 2000    **/
/** I did not write the original code/algorithm for this        **/
/** this triangulator, in fact, I can't even remember where I   **/
/** found it in the first place.  However, I did rework it into **/
/** the following black-box static class so you can make easy   **/
/** use of it in your own code.  Simply replace CGDM_Location3D with   **/
/** whatever your own Vector implementation might be.           **/
/*****************************************************************/

#define EPSILON 0.00000000000001
#define IS_IDENTICAL(a, b)	(fabs(a - b) < EPSILON)
#define IS_IDENTICAL_VERTEX(v1,v2) (IS_IDENTICAL((v1).m_tX, (v2).m_tX) && IS_IDENTICAL((v1).m_tY, (v2).m_tY))
typedef CGDM_Location3D Vector3d;


bool InCircle(CGDM_Location3D &p, CGDM_Location3D &p1, CGDM_Location3D &p2, CGDM_Location3D &p3);
int CircumCircle(double xp, double yp,
				 double x1, double y1,
				 double x2, double y2, 
				 double x3, double y3,
				 double *xc, double *yc,
				 double *r);

//Check if three points is arranged in counterclockwise order
bool IsCCW(CGDM_Location3D &A,CGDM_Location3D &B,CGDM_Location3D &C);

typedef struct _Edge
{	
	int p1;
	int p2;

	int side1, side2;

	_Edge(int point1, int point2):side1(-1),side2(-1)
	{
		p1 = point1; p2 = point2;	
	}

	_Edge():p1(-1),p2(-1),side1(-1),side2(-1)
	{

	}

	void SetSide(int orig_side, int new_side)
	{
		if (side1 == orig_side)
			side1 = new_side;
		else
			side2 = new_side;
	}

	bool GetSide(int side)
	{
		return (side1 == side || side2 == side);
	}


	void Set(int a, int b)
	{
		p1 = a;
		p2 = b;		
		side1 = -1;
		side2 = -1;
	}

	bool Get(int a)
	{
		return (p1 == a || p2 == a);
	}


	_Edge& operator=(const _Edge& a_cOther) 
	{
		p1 = a_cOther.p1; p2 = a_cOther.p2; 
		side1 = a_cOther.side1;
		side2 = a_cOther.side2;
		return *this; 
	}
	//! Direct access operator

	/// Checks whether two edges are equal disregarding the direction of the edges
	bool Equals(_Edge other)
	{
		return ((p1 == other.p2) && (p2 == other.p1)) ||
			((p1 == other.p1) && (p2 == other.p2));
	}

	bool Equals(int a, int b)
	{
		return ((p1 == b) && (p2 == a)) ||
			((p1 == a) && (p2 == b));
	}
} Edge;

struct Triangle{
	int p1;
	int p2;
	int p3;

	void Set(int a, int b,int c)
	{
		p1 = a;
		p2 = b;
		p3 = c;
	}
	int OppsiteVertex(Triangle &other, Edge &edge)
	{
		int a = other.isVertex(p1);
		int b = other.isVertex(p2);
		int c = other.isVertex(p3);
		int sum = other.p1+other.p2+other.p3;
		if (a+b == 2) {
			edge.Set(p1,p2);
			edge.side1 = sum - (p1+p2);
			return p3;
		}
		if (b+c == 2) {			
			edge.Set(p2,p3);
			edge.side1 = sum - (p2+p3);
			return p1;
		}
		if (c+a == 2) {
			edge.Set(p3,p1);
			edge.side1 = sum - (p3+p1);
			return p2;
		}
		return -1;
	}

	bool isDelauney(CGDM_LocationArr &Vertexs)
	{
		int nVertex = (int)Vertexs.size();
		double x1,y1,x2,y2,x3,y3;
		double xc,yc,r;
		x1 = Vertexs[p1].m_tX;
		y1 = Vertexs[p1].m_tY;
		x2 = Vertexs[p2].m_tX;
		y2 = Vertexs[p2].m_tY;
		x3 = Vertexs[p3].m_tX;
		y3 = Vertexs[p3].m_tY;

		CircumCircle(x1,y1,x1,y1,x2,y2,x3,y3,&xc,&yc,&r);
		r = r*r;

		for (int i = 0; i < nVertex;i++)
		{
			if (i == p1 || i == p2 || i == p3)
				continue;
			x1 = (Vertexs[i].m_tX-xc);
			y1 = (Vertexs[i].m_tY-yc);
			if (x1*x1+y1*y1 < r)
				return false;
		}
		return true;
	}

	bool isVertex(int p)
	{
		return (p1 == p || p2 == p || p3 == p);
	}

	Triangle(int point1, int point2, int point3)
	{
		p1 = point1; p2 = point2; p3 = point3;
	}
	Triangle()
	{
		p1 =  p2 = p3 = 0;
	}
};

class EdgeArray
{
public:
	Edge *edge;
	int nCount;
	int nAllocated;
	EdgeArray():nCount(0),nAllocated(0){edge=NULL;}
	EdgeArray(int size):nCount(0),nAllocated(0){reallocate(size);}
	~EdgeArray(){
		if (nAllocated>0) delete []edge;
	}
	void reallocate(int size)
	{
		if (size == nCount)
			return;
		if (nAllocated > 0)
			delete []edge;
		edge = new Edge[size];
		nCount = 0;
		nAllocated = size;
	}
	int size()
	{
		return nCount;
	}

	Edge *push_back(Edge &e)
	{		
		return push_back(e.p1,e.p2);
	}

	Edge *push_back(int a, int b)
	{
		nCount++;
		if (nCount >= nAllocated){
			nAllocated = nAllocated<<1;
			Edge *data = new Edge[nAllocated];
			memcpy(data, edge, sizeof(Edge)*(nCount-1));
			delete []edge;
			edge = data;			
		}
		edge[nCount-1].Set(a,b);		
		return &edge[nCount-1];
	}

	Edge *push_back(int a, int b, int c)
	{
		Edge *ab = Find(a,b);
		if (!ab){			
			ab = push_back(a,b);			
		}
		ab->SetSide(-1, c);
		return ab;
	}

	void erase(int index)
	{
		for (int i=index;i<nCount-1;i++)
		{
			edge[i] = edge[i+1];
		}
		nCount--;	
	}

	Edge *Find(int p1, int p2)
	{		
		for (int i =0;i<nCount;i++)
			if (edge[i].Equals(p1,p2))
				return &edge[i];
		return 0;
	}

	void set_used(int size=0){
		nCount = size;
	}
	_Edge& operator [](int index)
	{
		return edge[index];
	}
};

class TriangleArray
{
public:
	Triangle *data;
	int nCount;	
	int nAllocated;

	TriangleArray():nCount(0),nAllocated(0){data = NULL;}
	TriangleArray(int size):nCount(0),nAllocated(0){reallocate(size);}
	~TriangleArray(){
		if (nAllocated > 0)
			delete []data;
	}

	void reallocate(int size)
	{
		if (size == nCount)
			return;
		if (nAllocated > 0)
			delete []data;
		data = new Triangle[size];
		nCount = 0;
		nAllocated = size;
	}

	int size()
	{
		return nCount;
	}

	void push_back(int a, int b, int c)
	{
		nCount++;
		if (nCount >= nAllocated){
			nAllocated = nAllocated<<1;
			Triangle *tmp = new Triangle[nAllocated];
			memcpy(tmp, data, sizeof(Triangle)*(nCount-1));
			delete []data;
			data = tmp;	
		}
		data[nCount-1].Set(a,b,c);		
	}

	void push_back(Triangle &t)
	{
		push_back(t.p1,t.p2,t.p3);
	}
	
	void erase(int index)
	{
		for (int i=index;i<nCount-1;i++)
		{
			data[i] = data[i+1];
		}
		nCount--;
	}

	void set_used(int size=0){
		nCount = size;
	}
	Triangle& operator [](int index)
	{
		return data[index];
	}

	//! Assignment operator
	void operator=(const TriangleArray& other)
	{
		nCount = other.nCount;		
		for (int i=0; i<other.nCount; i++)
			data[i] = other.data[i];
	}
};


//typedef gdm_array<Triangle>	TriangleArray;

bool polygonTriangulate(CGDM_LocationArr &contour,Triangle3DArray &triArray);
void Delauney_Triangulate(CGDM_LocationArr &Vertexs,TriangleArray &Triangles);
void polygon2Polygons(CGDM_LocationArr &pArray,CGDM_LocationPolygonArr &polyArray);
#endif
