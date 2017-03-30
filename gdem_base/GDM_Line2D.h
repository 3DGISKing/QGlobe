#ifndef __GDEM_LINE_2D_H_INCLUDED__
#define __GDEM_LINE_2D_H_INCLUDED__

#include "GDM_Vector2D.h"


//! 2D a_cLine between two points with intersection methods.
template <class T>
class CGDM_Line2D
{
	public:
		//! Default constructor for a_cLine going from (0,0) to (1,1).
		CGDM_Line2D() : m_cStart(0,0), m_cEnd(1,1) {}
		//! Constructor for a_cLine between the two points.
		CGDM_Line2D(T a_tXa, T a_tYa, T a_tXb, T a_tYb) : m_cStart(a_tXa, a_tYa), m_cEnd(a_tXb, a_tYb) {}
		//! Constructor for a_cLine between the two points given as vectors.
		CGDM_Line2D(const CGDM_Vector2D<T>& a_cStart, const CGDM_Vector2D<T>& a_cEnd) : m_cStart(a_cStart), m_cEnd(a_cEnd) {}
		//! Copy constructor.
		CGDM_Line2D(const CGDM_Line2D<T>& a_cOther) : m_cStart(a_cOther.m_cStart), m_cEnd(a_cOther.m_cEnd) {}

		// operators

		CGDM_Line2D<T> operator+(const CGDM_Vector2D<T>& a_cPoint) const { return CGDM_Line2D<T>(m_cStart + a_cPoint, m_cEnd + a_cPoint); }
		CGDM_Line2D<T>& operator+=(const CGDM_Vector2D<T>& a_cPoint) { m_cStart += a_cPoint; m_cEnd += a_cPoint; return *this; }

		CGDM_Line2D<T> operator-(const CGDM_Vector2D<T>& a_cPoint) const { return CGDM_Line2D<T>(m_cStart - a_cPoint, m_cEnd - a_cPoint); }
		CGDM_Line2D<T>& operator-=(const CGDM_Vector2D<T>& a_cPoint) { m_cStart -= a_cPoint; m_cEnd -= a_cPoint; return *this; }

		bool operator==(const CGDM_Line2D<T>& a_cOther) const
		{ return (m_cStart==a_cOther.m_cStart && m_cEnd==a_cOther.m_cEnd) || (m_cEnd==a_cOther.m_cStart && m_cStart==a_cOther.m_cEnd);}
		bool operator!=(const CGDM_Line2D<T>& a_cOther) const
		{ return !(m_cStart==a_cOther.m_cStart && m_cEnd==a_cOther.m_cEnd) || (m_cEnd==a_cOther.m_cStart && m_cStart==a_cOther.m_cEnd);}

		// functions
		//! Set this a_cLine to new a_cLine going through the two points.
		void setLine(const T& a_tXa, const T& a_tYa, const T& a_tXb, const T& a_tYb){m_cStart.set(a_tXa, a_tYa); m_cEnd.set(a_tXb, a_tYb);}
		//! Set this a_cLine to new a_cLine going through the two points.
		void setLine(const CGDM_Vector2D<T>& nstart, const CGDM_Vector2D<T>& nend){m_cStart.set(nstart); m_cEnd.set(nend);}
		//! Set this a_cLine to new a_cLine given as parameter.
		void setLine(const CGDM_Line2D<T>& a_cLine){m_cStart.set(a_cLine.m_cStart); m_cEnd.set(a_cLine.m_cEnd);}

		//! Get length of a_cLine
		/** \return Length of the a_cLine. */
		GDEM_DOUBLE getLength() const { return m_cStart.getDistanceFrom(m_cEnd); }

		//! Get squared length of the a_cLine
		/** \return Squared length of a_cLine. */
		T getLengthSQ() const { return m_cStart.getDistanceFromSQ(m_cEnd); }

		//! Get middle of the a_cLine
		/** \return center of the a_cLine. */
		CGDM_Vector2D<T> getMiddle() const
		{
			return (m_cStart + m_cEnd) * (T)0.5;
		}

		//! Get the vector of the a_cLine.
		/** \return The vector of the a_cLine. */
		CGDM_Vector2D<T> getVector() const { return CGDM_Vector2D<T>(m_cStart.X - m_cEnd.X, m_cStart.Y - m_cEnd.Y); }

		//! Tests if this a_cLine intersects with another a_cLine.
		/** \param a_cLine: Other a_cLine to test intersection with.
		\param a_cLineOut: If there is an intersection, the location of the
		intersection will be stored in this vector.
		\return True if there is an intersection, false if not. */
		bool intersectWith(const CGDM_Line2D<T>& a_cLine, CGDM_Vector2D<T>& a_cLineOut) const
		{
			// Uses the method given at:
			// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/ 
			const double commonDenominator = (a_cLine.m_cEnd.Y - a_cLine.m_cStart.Y)*(m_cEnd.X - m_cStart.X) -
											(a_cLine.m_cEnd.X - a_cLine.m_cStart.X)*(m_cEnd.Y - m_cStart.Y);

			const double numeratorA = (a_cLine.m_cEnd.X - a_cLine.m_cStart.X)*(m_cStart.Y - a_cLine.m_cStart.Y) -
											(a_cLine.m_cEnd.Y - a_cLine.m_cStart.Y)*(m_cStart.X -a_cLine.m_cStart.X);

			const double numeratorB = (m_cEnd.X - m_cStart.X)*(m_cStart.Y - a_cLine.m_cStart.Y) -
											(m_cEnd.Y - m_cStart.Y)*(m_cStart.X -a_cLine.m_cStart.X); 
		/*
			if(equals(commonDenominator, 0.f))
			{ 
				// The lines are either coincident or parallel
				if(equals(numeratorA, 0.f) && equals(numeratorB, 0.f))
				{
					// Try and find a common endpoint
					if(a_cLine.m_cStart == m_cStart || a_cLine.m_cEnd == m_cStart)
						a_cLineOut = m_cStart;
					else if(a_cLine.m_cEnd == m_cEnd || a_cLine.m_cStart == m_cEnd)
						a_cLineOut = m_cEnd;
					else
						// one a_cLine is contained in the a_cOther, so for lack of a better
						// answer, pick the average of both lines
						a_cLineOut = ((m_cStart + m_cEnd + a_cLine.m_cStart + a_cLine.m_cEnd) * 0.25f);

					return true; // coincident
				}

				return false; // parallel
			}

			// Get the a_cPoint of intersection on this a_cLine, checking that
			// it is within the a_cLine segment.
			const double uA = numeratorA / commonDenominator;
			if(uA < 0.f || uA > 1.f)
				return false; // Outside the a_cLine segment

			const double uB = numeratorB / commonDenominator;
			if(uB < 0.f || uB > 1.f)
				return false; // Outside the a_cLine segment

			// Calculate the intersection a_cPoint.
			a_cLineOut.X = m_cStart.X + uA * (m_cEnd.X - m_cStart.X);
			a_cLineOut.Y = m_cStart.Y + uA * (m_cEnd.Y - m_cStart.Y);
		*/
			return true; 
		}

		//added by RSH 2013.3.18
		bool intersectWith(const CGDM_Line2D<T>& a_cLine, CGDM_Vector2D<T>& a_cLineOut, double tolerance) const
			{
			// Uses the method given at:
			// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/ 
			const double commonDenominator = (a_cLine.m_cEnd.m_tY - a_cLine.m_cStart.m_tY)*(m_cEnd.m_tX - m_cStart.m_tX) -
				(a_cLine.m_cEnd.m_tX - a_cLine.m_cStart.m_tX)*(m_cEnd.m_tY - m_cStart.m_tY);

			const double numeratorA = (a_cLine.m_cEnd.m_tX - a_cLine.m_cStart.m_tX)*(m_cStart.m_tY - a_cLine.m_cStart.m_tY) -
				(a_cLine.m_cEnd.m_tY - a_cLine.m_cStart.m_tY)*(m_cStart.m_tX -a_cLine.m_cStart.m_tX);

			const double numeratorB = (m_cEnd.m_tX - m_cStart.m_tX)*(m_cStart.m_tY - a_cLine.m_cStart.m_tY) -
				(m_cEnd.m_tY - m_cStart.m_tY)*(m_cStart.m_tX -a_cLine.m_cStart.m_tX); 

			if(gdm_equals(commonDenominator, 0.0, tolerance))
				{ 
				// The lines are either coincident or parallel
				if(gdm_equals(numeratorA, 0.0, tolerance) && gdm_equals(numeratorB, 0.0, tolerance))
					{
					// Try and find a common endpoint
					if(a_cLine.m_cStart == m_cStart || a_cLine.m_cEnd == m_cStart)
						a_cLineOut = m_cStart;
					else if(a_cLine.m_cEnd == m_cEnd || a_cLine.m_cStart == m_cEnd)
						a_cLineOut = m_cEnd;
					else
						// one a_cLine is contained in the a_cOther, so for lack of a better
						// answer, pick the average of both lines
						a_cLineOut = ((m_cStart + m_cEnd + a_cLine.m_cStart + a_cLine.m_cEnd) * 0.25f);

					return true; // coincident
					}

				return false; // parallel
				}

			// Get the a_cPoint of intersection on this a_cLine, checking that
			// it is within the a_cLine segment.
			const double uA = numeratorA / commonDenominator;
			if(uA < 0.f || uA > 1.f)
				return false; // Outside the a_cLine segment

			const double uB = numeratorB / commonDenominator;
			if(uB < 0.f || uB > 1.f)
				return false; // Outside the a_cLine segment

			// Calculate the intersection a_cPoint.
			a_cLineOut.m_tX = m_cStart.m_tX + uA * (m_cEnd.m_tX - m_cStart.m_tX);
			a_cLineOut.m_tY = m_cStart.m_tY + uA * (m_cEnd.m_tY - m_cStart.m_tY);
			return true; 
			}
			//end addition


		//! Get unit vector of the a_cLine.
		/** \return Unit vector of this a_cLine. */
		CGDM_Vector2D<T> getUnitVector() const
		{
			T len = (T)(1.0 / getLength());
			return CGDM_Vector2D<T>((m_cEnd.X - m_cStart.X) * len, (m_cEnd.Y - m_cStart.Y) * len);
		}

		//! Get angle between this a_cLine and given a_cLine.
		/** \param a_cLine Other a_cLine for test.
		\return Angle in degrees. */
		GDEM_DOUBLE getAngleWith(const CGDM_Line2D<T>& a_cLine) const
		{
			CGDM_Vector2D<T> vect = getVector();
			CGDM_Vector2D<T> vect2 = a_cLine.getVector();
			return vect.getAngleWith(vect2);
		}

		//! Tells us if the given a_cPoint lies to the left, right, or on the a_cLine.
		/** \return 0 if the a_cPoint is on the a_cLine
		<0 if to the left, or >0 if to the right. */
		T getPointOrientation(const CGDM_Vector2D<T>& a_cPoint) const
		{
			return ( (m_cEnd.X - m_cStart.X) * (a_cPoint.Y - m_cStart.Y) -
					(a_cPoint.X - m_cStart.X) * (m_cEnd.Y - m_cStart.Y) );
		}

		//! Check if the given a_cPoint is a member of the a_cLine
		/** \return True if a_cPoint is between m_cStart and m_cEnd, else false. */
		bool isPointOnLine(const CGDM_Vector2D<T>& a_cPoint) const
		{
			T d = getPointOrientation(a_cPoint);
			return (d == 0 && a_cPoint.isBetweenPoints(m_cStart, m_cEnd));
		}

		//! Check if the given a_cPoint is between m_cStart and m_cEnd of the a_cLine.
		/** Assumes that the a_cPoint is already somewhere on the a_cLine. */
		bool isPointBetweenStartAndEnd(const CGDM_Vector2D<T>& a_cPoint) const
		{
			return a_cPoint.isBetweenPoints(m_cStart, m_cEnd);
		}

		//! Get the closest a_cPoint on this a_cLine to a a_cPoint
		CGDM_Vector2D<T> getClosestPoint(const CGDM_Vector2D<T>& a_cPoint) const
		{
			CGDM_Vector2D<T> c = a_cPoint - m_cStart;
			CGDM_Vector2D<T> v = m_cEnd - m_cStart;
			T d = (T)v.getLength();
			v /= d;
			T t = v.dotProduct(c);

			if (t < (T)0.0) return m_cStart;
			if (t > d) return m_cEnd;

			v *= t;
			return m_cStart + v;
		}

		//! Start a_cPoint of the a_cLine.
		CGDM_Vector2D<T> m_cStart;
		//! End a_cPoint of the a_cLine.
		CGDM_Vector2D<T> m_cEnd;
};

	//! Typedef for an double a_cLine.
	typedef CGDM_Line2D<double> CGDM_Line2DF;
	//! Typedef for an integer a_cLine.
	typedef CGDM_Line2D<GDEM_INT> CGDM_Line2DI;

#endif // __GDEM_LINE_2D_H_INCLUDED__
