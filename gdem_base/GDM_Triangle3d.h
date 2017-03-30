#ifndef __GDEM_TRIANGLE_3D_H_INCLUDED__
#define __GDEM_TRIANGLE_3D_H_INCLUDED__


#include "GDM_Vector3D.h"
#include "GDM_Line3D.h"
#include "GDM_Plane3D.h"
#include "GDM_Aabbox3D.h"

//! 3d triangle template class for doing collision detection and other things.
template <class T>
class CGDM_Triangle3D
{
public:

	//! Constructor for an all 0 triangle
	CGDM_Triangle3D() {}
	//! Constructor for triangle with given three vertices
	CGDM_Triangle3D(CGDM_Vector3D<T> v1, CGDM_Vector3D<T> v2, CGDM_Vector3D<T> v3) : pointA(v1), pointB(v2), pointC(v3) {}

	//! Equality operator
	bool operator==(const CGDM_Triangle3D<T>& other) const
	{
		return other.pointA==pointA && other.pointB==pointB && other.pointC==pointC;
	}

	//! Inequality operator
	bool operator!=(const CGDM_Triangle3D<T>& other) const
	{
		return !(*this==other);
	}

	//! Determines if the triangle is totally inside a bounding box.
	/** \param box Box to check.
	\return True if triangle is within the box, otherwise false. */
	bool isTotalInsideBox(const CGDM_Aabbox3D<T>& box) const
	{
		return (box.isPointInside(pointA) &&
			box.isPointInside(pointB) &&
			box.isPointInside(pointC));
	}

	//! Get the closest point on a triangle to a point on the same plane.
	/** \param p Point which must be on the same plane as the triangle.
	\return The closest point of the triangle */
	CGDM_Vector3D<T> closestPointOnTriangle(const CGDM_Vector3D<T>& p) const
	{
		const CGDM_Vector3D<T> rab = CGDM_Line3D<T>(pointA, pointB).getClosestPoint(p);
		const CGDM_Vector3D<T> rbc = CGDM_Line3D<T>(pointB, pointC).getClosestPoint(p);
		const CGDM_Vector3D<T> rca = CGDM_Line3D<T>(pointC, pointA).getClosestPoint(p);

		const T d1 = rab.getDistanceFrom(p);
		const T d2 = rbc.getDistanceFrom(p);
		const T d3 = rca.getDistanceFrom(p);

		if (d1 < d2)
			return d1 < d3 ? rab : rca;

		return d2 < d3 ? rbc : rca;
	}

	//! Check if a point is inside the triangle
	/** \param p Point to test. Assumes that this point is already
	on the plane of the triangle.
	\return True if the point is inside the triangle, otherwise false. */
	bool isPointInside(const CGDM_Vector3D<T>& p) const
	{

		return (isOnSameSide(p, pointA, pointB, pointC) &&
			isOnSameSide(p, pointB, pointC, pointA) &&
			isOnSameSide(p, pointC, pointA, pointB));
	}

	//! Check if a point is inside the triangle.
	/** This method is an implementation of the example used in a
	paper by Kasper Fauerby original written by Keidy from
	Mr-Gamemaker.
	\param p Point to test. Assumes that this point is already
	on the plane of the triangle.
	\return True if point is inside the triangle, otherwise false. */
	bool isPointInsideFast(const CGDM_Vector3D<T>& p) const
	{
		const CGDM_Vector3D<T> f = pointB - pointA;
		const CGDM_Vector3D<T> g = pointC - pointA;

		const double a = f.dotProduct(f);
		const double b = f.dotProduct(g);
		const double c = g.dotProduct(g);

		const CGDM_Vector3D<T> vp = p - pointA;
		const double d = vp.dotProduct(f);
		const double e = vp.dotProduct(g);

		double x = (d*c)-(e*b);
		double y = (e*a)-(d*b);
		const double ac_bb = (a*c)-(b*b);
		double z = x+y-ac_bb;

		//return sign(z) && !(sign(x)||sign(y))
                return (( (_IR(z)) & ~((_IR(x))|(_IR(y))) ) & 0x80000000)!=0;
	}


	//! Get an intersection with a 3d line.
	/** \param line Line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if not. */
	bool getIntersectionWithLimitedLine(const CGDM_Line3D<T>& line,
		CGDM_Vector3D<T>& outIntersection) const
	{
		return getIntersectionWithLine(line.start,
			line.getVector(), outIntersection) &&
			outIntersection.isBetweenPoints(line.start, line.end);
	}


	//! Get an intersection with a 3d line.
	/** Please note that also points are returned as intersection which
	are on the line, but not between the start and end point of the line.
	If you want the returned point be between start and end
	use getIntersectionWithLimitedLine().
	\param linePoint Point of the line to intersect with.
	\param lineVect Vector of the line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, false if there was not. */
	bool getIntersectionWithLine(const CGDM_Vector3D<T>& linePoint,
		const CGDM_Vector3D<T>& lineVect, CGDM_Vector3D<T>& outIntersection) const
	{
		if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
			return isPointInside(outIntersection);

		return false;
	}


	//! Calculates the intersection between a 3d line and the plane the triangle is on.
	/** \param lineVect Vector of the line to intersect with.
	\param linePoint Point of the line to intersect with.
	\param outIntersection Place to store the intersection point, if there is one.
	\return True if there was an intersection, else false. */
	bool getIntersectionOfPlaneWithLine(const CGDM_Vector3D<T>& linePoint,
		const CGDM_Vector3D<T>& lineVect, CGDM_Vector3D<T>& outIntersection) const
	{
		const CGDM_Vector3D<T> normal = getNormal().normalize();
		T t2;

// 		if ( iszero ( t2 = normal.dotProduct(lineVect) ) )
// 			return false;
		t2 = normal.dotProduct(lineVect);
		if ( gdm_equals( (T) t2, (T) 0.0f ) )
			return false;

		T d = pointA.dotProduct(normal);
		T t = -(normal.dotProduct(linePoint) - d) / t2;
		outIntersection = linePoint + (lineVect * t);
		return true;
	}


	//! Get the normal of the triangle.
	/** Please note: The normal is not always normalized. */
	CGDM_Vector3D<T> getNormal() const
	{
		return (pointB - pointA).crossProduct(pointC - pointA);
	}

	//! Test if the triangle would be front or backfacing from any point.
	/** Thus, this method assumes a camera position from which the
	triangle is definitely visible when looking at the given direction.
	Do not use this method with points as it will give wrong results!
	\param lookDirection Look direction.
	\return True if the plane is front facing and false if it is backfacing. */
	bool isFrontFacing(const CGDM_Vector3D<T>& lookDirection) const
	{
		const CGDM_Vector3D<T> n = getNormal().normalize();
		const double d = (double)n.dotProduct(lookDirection);
		return F_32_LOWER_EQUAL_0(d);
	}

	//! Get the plane of this triangle.
	CGDM_Plane3D<T> getPlane() const
	{
		return CGDM_Plane3D<T>(pointA, pointB, pointC);
	}

	//! Get the area of the triangle
	T getArea() const
	{
		return (pointB - pointA).crossProduct(pointC - pointA).getLength() * 0.5;

	}

	//! sets the triangle's points
	void set(const CGDM_Vector3D<T>& a, const CGDM_Vector3D<T>& b, const CGDM_Vector3D<T>& c)
	{
		pointA = a;
		pointB = b;
		pointC = c;
	}

	//! the three points of the triangle
	CGDM_Vector3D<T> pointA;
	CGDM_Vector3D<T> pointB;
	CGDM_Vector3D<T> pointC;

private:
	bool isOnSameSide(const CGDM_Vector3D<T>& p1, const CGDM_Vector3D<T>& p2,
		const CGDM_Vector3D<T>& a, const CGDM_Vector3D<T>& b) const
	{
		CGDM_Vector3D<T> bminusa = b - a;
		CGDM_Vector3D<T> cp1 = bminusa.crossProduct(p1 - a);
		CGDM_Vector3D<T> cp2 = bminusa.crossProduct(p2 - a);
		return (cp1.dotProduct(cp2) >= 0.0f);
	}
};


	//! Typedef for a double 3d triangle.
	typedef CGDM_Triangle3D<double> CGDM_Triangle3DF;
	typedef CGDM_Triangle3D<double> Triangle3DF;

	//! Typedef for an integer 3d triangle.
	typedef CGDM_Triangle3D<GDEM_INT> CGDM_Triangle3DI;


#endif // __GDEM_TRIANGLE_3D_H_INCLUDED__
