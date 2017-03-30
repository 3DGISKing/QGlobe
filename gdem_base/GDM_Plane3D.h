#ifndef __GDEM_PLANE_3D_H_INCLUDED__
#define __GDEM_PLANE_3D_H_INCLUDED__

#include "GDM_Vector3D.h"


//! Enumeration for intersection relations of 3d objects
enum EIntersectionRelation3D
{
	ISREL3D_FRONT = 0,
	ISREL3D_BACK,
	ISREL3D_PLANAR,
	ISREL3D_SPANNING,
	ISREL3D_CLIPPED
};

//! Template plane class with some intersection testing methods.
template <class T>
class CGDM_Plane3D
{
	public:

		// Constructors

		CGDM_Plane3D(): m_cNormal(0,1,0) { recalculateD(CGDM_Vector3D<T>(0,0,0)); }
		
		CGDM_Plane3D(const CGDM_Vector3D<T>& MPoint, const CGDM_Vector3D<T>& m_cNormal) : m_cNormal(m_cNormal) { recalculateD(MPoint); }
		
		CGDM_Plane3D(T px, T py, T pz, T nx, T ny, T nz) : m_cNormal(nx, ny, nz) { recalculateD(CGDM_Vector3D<T>(px, py, pz)); }
		
		CGDM_Plane3D(const CGDM_Vector3D<T>& point1, const CGDM_Vector3D<T>& point2, const CGDM_Vector3D<T>& point3)
		{ setPlane(point1, point2, point3); }
		
		CGDM_Plane3D(const CGDM_Vector3D<T> & normal, const T d) : m_cNormal(normal), m_tDist(d) { }

		// operators

		inline bool operator==(const CGDM_Plane3D<T>& other) const { return (equals(m_tDist, other.m_tDist) && m_cNormal==other.m_cNormal);}

		inline bool operator!=(const CGDM_Plane3D<T>& other) const { return !(*this == other);}

		// functions

		void setPlane(const CGDM_Vector3D<T>& point, const CGDM_Vector3D<T>& nvector)
		{
			m_cNormal = nvector;
			recalculateD(point);
		}

		void setPlane(const CGDM_Vector3D<T>& nvect, T d)
		{
			m_cNormal = nvect;
			m_tDist = d;
		}

		void setPlane(const CGDM_Vector3D<T>& point1, const CGDM_Vector3D<T>& point2, const CGDM_Vector3D<T>& point3)
		{
			// creates the plane from 3 memberpoints
			m_cNormal = (point2 - point1).crossProduct(point3 - point1);
			m_cNormal.normalize();

			recalculateD(point1);
		}


		//! Get an intersection with a 3d line.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		bool getIntersectionWithLine(const CGDM_Vector3D<T>& linePoint,
				const CGDM_Vector3D<T>& lineVect,
				CGDM_Vector3D<T>& outIntersection) const
		{
			T t2 = m_cNormal.dotProduct(lineVect);

			if (t2 == 0)
				return false;

			T t =- (m_cNormal.dotProduct(linePoint) + m_tDist) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		//! Get percentage of line between two points where an intersection with this plane happens.
		/** Only useful if known that there is an intersection.
		\param linePoint1 Point1 of the line to intersect with.
		\param linePoint2 Point2 of the line to intersect with.
		\return Where on a line between two points an intersection with this plane happened.
		For example, 0.5 is returned if the intersection happened exactly in the middle of the two points.
		*/
		double getKnownIntersectionWithLine(const CGDM_Vector3D<T>& linePoint1,
			const CGDM_Vector3D<T>& linePoint2) const
		{
			CGDM_Vector3D<T> vect = linePoint2 - linePoint1;
			T t2 = (double)m_cNormal.dotProduct(vect);
			return (double)-((m_cNormal.dotProduct(linePoint1) + m_tDist) / t2);
		}

		//! Get an intersection with a 3d line, limited between two 3d points.
		/** \param linePoint1 Point 1 of the line.
		\param linePoint2 Point 2 of the line.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		bool getIntersectionWithLimitedLine(
				const CGDM_Vector3D<T>& linePoint1,
				const CGDM_Vector3D<T>& linePoint2,
				CGDM_Vector3D<T>& outIntersection) const
		{
			return (getIntersectionWithLine(linePoint1, linePoint2 - linePoint1, outIntersection) &&
					outIntersection.isBetweenPoints(linePoint1, linePoint2));
		}

		//! Classifies the relation of a point to this plane.
		/** \param point Point to classify its relation.
		\return ISREL3D_FRONT if the point is in front of the plane,
		ISREL3D_BACK if the point is behind of the plane, and
		ISREL3D_PLANAR if the point is within the plane. */
		EIntersectionRelation3D classifyPointRelation(const CGDM_Vector3D<T>& point) const
		{
			const T d = m_cNormal.dotProduct(point) + m_tDist;

			if (d < -gdm_ROUNDING_ERROR_32)
				return ISREL3D_BACK;

			if (d > gdm_ROUNDING_ERROR_32)
				return ISREL3D_FRONT;

			return ISREL3D_PLANAR;
		}

		//! Recalculates the distance from origin by applying a new member point to the plane.
		void recalculateD(const CGDM_Vector3D<T>& MPoint)
		{
			m_tDist = - MPoint.dotProduct(m_cNormal);
		}

		//! Gets a member point of the plane.
		CGDM_Vector3D<T> getMemberPoint() const
		{
			return m_cNormal * -m_tDist;
		}

		//! Tests if there is an intersection with the other plane
		/** \return True if there is a intersection. */
		bool existsIntersection(const CGDM_Plane3D<T>& other) const
		{
			CGDM_Vector3D<T> cross = other.m_cNormal.crossProduct(m_cNormal);
			return cross.getLength() > gdm_ROUNDING_ERROR_32;
		}

		//! Intersects this plane with another.
		/** \param other Other plane to intersect with.
		\param outLinePoint Base point of intersection line.
		\param outLineVect Vector of intersection.
		\return True if there is a intersection, false if not. */
		bool getIntersectionWithPlane(const CGDM_Plane3D<T>& other,
				CGDM_Vector3D<T>& outLinePoint,
				CGDM_Vector3D<T>& outLineVect) const
		{
			const T fn00 = m_cNormal.getLength();
			const T fn01 = m_cNormal.dotProduct(other.m_cNormal);
			const T fn11 = other.m_cNormal.getLength();
			const GDEM_DOUBLE det = fn00*fn11 - fn01*fn01;

			if (fabs(det) < gdm_ROUNDING_ERROR_64 )
				return false;

			const GDEM_DOUBLE invdet = 1.0 / det;
			const GDEM_DOUBLE fc0 = (fn11*-m_tDist + fn01*other.m_tDist) * invdet;
			const GDEM_DOUBLE fc1 = (fn00*-other.m_tDist + fn01*m_tDist) * invdet;

			outLineVect = m_cNormal.crossProduct(other.m_cNormal);
			outLinePoint = m_cNormal*(T)fc0 + other.m_cNormal*(T)fc1;
			return true;
		}

		//! Get the intersection point with two other planes if there is one.
		bool getIntersectionWithPlanes(const CGDM_Plane3D<T>& o1,
				const CGDM_Plane3D<T>& o2, CGDM_Vector3D<T>& outPoint) const
		{
			CGDM_Vector3D<T> linePoint, lineVect;
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

			return false;
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from
		which the triangle is definitely visible when looking into
		the given direction.
		Note that this only works if the normal is Normalized.
		Do not use this method with points as it will give wrong results!
		\param lookDirection: Look direction.
		\return True if the plane is front facing and
		false if it is backfacing. */
		bool isFrontFacing(const CGDM_Vector3D<T>& lookDirection) const
		{
			const double d = m_cNormal.dotProduct(lookDirection);
			return F_32_LOWER_EQUAL_0 ( d );
		}

		//! Get the distance to a point.
		/** Note that this only works if the normal is normalized. */
		T getDistanceTo(const CGDM_Vector3D<T>& point) const
		{
			return point.dotProduct(m_cNormal) + m_tDist;
		}

				//! Normalize the plane
		void normalize()
		{
			double length = (double)m_cNormal.getLength();
			if (gdm_equals(length, 1.0e-9))
				return;
			length = 1 / length;
			m_cNormal *= length;
			m_tDist *= length;
		}


		//! m_cNormal vector of the plane.
		CGDM_Vector3D<T> m_cNormal;
		//! Distance from origin.
		T m_tDist;
};


	//! Typedef for a double 3d plane.
	typedef CGDM_Plane3D<double> CGDM_Plane3DF;
	//! Typedef for an integer 3d plane.
	typedef CGDM_Plane3D<GDEM_INT> CGDM_Plane3DI;


#endif // __GDEM_PLANE_3D_H_INCLUDED__
