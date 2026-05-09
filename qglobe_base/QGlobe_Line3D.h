#ifndef __QGLOBE_LINE_3D_H_INCLUDED__
#define __QGLOBE_LINE_3D_H_INCLUDED__

#include "QGlobe_Vector3D.h"


//! 3D line between two points with intersection methods.
template <class T>
class CQGlobe_Line3D
{
	public:

		//! Default constructor
		/** line from (0,0,0) to (1,1,1) */
		CQGlobe_Line3D() : m_cStart(0,0,0), m_cEnd(1,1,1) {}
		//! Constructor with two points
		CQGlobe_Line3D(T a_tXa, T a_tYa, T a_tZa, T a_tXb, T a_tYb, T a_tZb) : m_cStart(a_tXa, a_tYa, a_tZa), m_cEnd(a_tXb, a_tYb, a_tZb) {}
		//! Constructor with two points as vectors
		CQGlobe_Line3D(const CQGlobe_Vector3D<T>& a_cStart, const CQGlobe_Vector3D<T>& a_cEnd) : m_cStart(a_cStart), m_cEnd(a_cEnd) {}

		// operators

		CQGlobe_Line3D<T> operator+(const CQGlobe_Vector3D<T>& a_cPoint) const { return CQGlobe_Line3D<T>(m_cStart + a_cPoint, m_cEnd + a_cPoint); }
		CQGlobe_Line3D<T>& operator+=(const CQGlobe_Vector3D<T>& a_cPoint) { m_cStart += a_cPoint; m_cEnd += a_cPoint; return *this; }

		CQGlobe_Line3D<T> operator-(const CQGlobe_Vector3D<T>& a_cPoint) const { return CQGlobe_Line3D<T>(m_cStart - a_cPoint, m_cEnd - a_cPoint); }
		CQGlobe_Line3D<T>& operator-=(const CQGlobe_Vector3D<T>& a_cPoint) { m_cStart -= a_cPoint; m_cEnd -= a_cPoint; return *this; }

		bool operator==(const CQGlobe_Line3D<T>& a_cOther) const
		{ return (m_cStart==a_cOther.m_cStart && m_cEnd==a_cOther.m_cEnd) || (m_cEnd==a_cOther.m_cStart && m_cStart==a_cOther.m_cEnd);}
		bool operator!=(const CQGlobe_Line3D<T>& a_cOther) const
		{ return !(m_cStart==a_cOther.m_cStart && m_cEnd==a_cOther.m_cEnd) || (m_cEnd==a_cOther.m_cStart && m_cStart==a_cOther.m_cEnd);}

		// functions
		//! Set this line to a new line going through the two points.
		void setLine(const T& a_tXa, const T& a_tYa, const T& a_tZa, const T& a_tXb, const T& a_tYb, const T& a_tZb)
		{m_cStart.set(a_tXa, a_tYa, a_tZa); m_cEnd.set(a_tXb, a_tYb, a_tZb);}
		//! Set this line to a new line going through the two points.
		void setLine(const CQGlobe_Vector3D<T>& nstart, const CQGlobe_Vector3D<T>& nend)
		{m_cStart.set(nstart); m_cEnd.set(nend);}
		//! Set this line to new line given as parameter.
		void setLine(const CQGlobe_Line3D<T>& line)
		{m_cStart.set(line.m_cStart); m_cEnd.set(line.m_cEnd);}

		//! Get length of line
		/** \return Length of line. */
		T getLength() const { return m_cStart.getDistanceFrom(m_cEnd); }

		//! Get squared length of line
		/** \return Squared length of line. */
		T getLengthSQ() const { return m_cStart.getDistanceFromSQ(m_cEnd); }

		//! Get middle of line
		/** \return Center of line. */
		CQGlobe_Vector3D<T> getMiddle() const
		{
			return (m_cStart + m_cEnd) * (T)0.5;
		}

		//! Get vector of line
		/** \return vector of line. */
		CQGlobe_Vector3D<T> getVector() const
		{
			return m_cEnd - m_cStart;
		}

		//! Check if the given a_cPoint is between m_cStart and m_cEnd of the line.
		/** Assumes that the a_cPoint is already somewhere on the line.
		\param a_cPoint The a_cPoint to test.
		\return True if a_cPoint is on the line between m_cStart and m_cEnd, else false.
		*/
		bool isPointBetweenStartAndEnd(const CQGlobe_Vector3D<T>& a_cPoint) const
		{
			return a_cPoint.isBetweenPoints(m_cStart, m_cEnd);
		}

		//! Get the closest a_cPoint on this line to a a_cPoint
		/** \param a_cPoint The a_cPoint to compare to.
		\return The nearest a_cPoint which is part of the line. */
		CQGlobe_Vector3D<T> getClosestPoint(const CQGlobe_Vector3D<T>& a_cPoint) const
		{
			CQGlobe_Vector3D<T> c = a_cPoint - m_cStart;
			CQGlobe_Vector3D<T> v = m_cEnd - m_cStart;
			T d = (T)v.getLength();
			v /= d;
			T t = v.dotProduct(c);

			if (t < (T)0.0)
				return m_cStart;
			if (t > d)
				return m_cEnd;

			v *= t;
			return m_cStart + v;
		}

		//! Check if the line intersects with a shpere
		/** \param sorigin: Origin of the shpere.
		\param sradius: Radius of the sphere.
		\param outdistance: The distance to the first intersection a_cPoint.
		\return True if there is an intersection.
		If there is one, the distance to the first intersection a_cPoint
		is stored in outdistance. */
		bool getIntersectionWithSphere(CQGlobe_Vector3D<T> sorigin, T sradius, QGLOBE_DOUBLE& outdistance) const
		{
			const CQGlobe_Vector3D<T> q = sorigin - m_cStart;
			T c = q.getLength();
			T v = q.dotProduct(getVector().normalize());
			T d = sradius * sradius - (c*c - v*v);

			if (d < 0.0)
				return false;

			outdistance = v - sqrt((QGLOBE_DOUBLE)d);
			return true;
		}

		// member variables

		//! Start a_cPoint of line
		CQGlobe_Vector3D<T> m_cStart;
		//! End a_cPoint of line
		CQGlobe_Vector3D<T> m_cEnd;
};

	//! Typedef for an double line.
	typedef CQGlobe_Line3D<double> CQGlobe_Line3DF;
	//! Typedef for an integer line.
	typedef CQGlobe_Line3D<QGLOBE_INT> CQGlobe_Line3DI;

#endif // __QGLOBE_LINE_3D_H_INCLUDED__
