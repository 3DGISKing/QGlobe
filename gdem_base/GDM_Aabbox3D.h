#ifndef __GDEM_AABBOX_3D_H_INCLUDED__
#define __GDEM_AABBOX_3D_H_INCLUDED__

#include "GDM_Plane3D.h"
#include "GDM_Line3D.h"

//! Axis aligned bounding box in 3d dimensional space.
/** Has some useful methods used with occlusion culling or clipping.
*/
template <class T>
class CGDM_Aabbox3D
{
	public:

		//! Default Constructor.
		CGDM_Aabbox3D(): MinEdge(-1,-1,-1), MaxEdge(1,1,1) {}
		//! Constructor with min edge and max edge.
		CGDM_Aabbox3D(const CGDM_Vector3D<T>& min, const CGDM_Vector3D<T>& max): MinEdge(min), MaxEdge(max) {}
		//! Constructor with only one point.
		CGDM_Aabbox3D(const CGDM_Vector3D<T>& init): MinEdge(init), MaxEdge(init) {}
		//! Constructor with min edge and max edge as single values, not vectors.
		CGDM_Aabbox3D(T minx, T miny, T minz, T maxx, T maxy, T maxz): MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz) {}

		// operators
		//! Equality operator
		/** \param other box to compare with.
		\return True if both boxes are equal, else false. */
		inline bool operator==(const CGDM_Aabbox3D<T>& other) const { return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}
		//! Inequality operator
		/** \param other box to compare with.
		\return True if both boxes are different, else false. */
		inline bool operator!=(const CGDM_Aabbox3D<T>& other) const { return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}

		// functions

		//! Adds a point to the bounding box
		/** The box grows bigger, if point was outside of the box.
		\param p: Point to add into the box. */
		void addInternalPoint(const CGDM_Vector3D<T>& p)
		{
			addInternalPoint(p.m_tX, p.m_tY, p.m_tZ);
		}

		//! Adds another bounding box
		/** The box grows bigger, if the new box was outside of the box.
		\param b: Other bounding box to add into this box. */
		void addInternalBox(const CGDM_Aabbox3D<T>& b)
		{
			addInternalPoint(b.MaxEdge);
			addInternalPoint(b.MinEdge);
		}

		//! Resets the bounding box to a one-point box.
		/** \param x X coord of the point.
		\param y Y coord of the point.
		\param z Z coord of the point. */
		void reset(T x, T y, T z)
		{
			MaxEdge.set(x,y,z);
			MinEdge = MaxEdge;
		}

		//! Resets the bounding box.
		/** \param initValue New box to set this one to. */
		void reset(const CGDM_Aabbox3D<T>& initValue)
		{
			*this = initValue;
		}

		//! Resets the bounding box to a one-point box.
		/** \param initValue New point. */
		void reset(const CGDM_Vector3D<T>& initValue)
		{
			MaxEdge = initValue;
			MinEdge = initValue;
		}

		//! Adds a point to the bounding box
		/** The box grows bigger, if point is outside of the box.
		\param x X coordinate of the point to add to this box.
		\param y Y coordinate of the point to add to this box.
		\param z Z coordinate of the point to add to this box. */
		void addInternalPoint(T x, T y, T z)
		{
			if (x>MaxEdge.m_tX) MaxEdge.m_tX = x;
			if (y>MaxEdge.m_tY) MaxEdge.m_tY = y;
			if (z>MaxEdge.m_tZ) MaxEdge.m_tZ = z;

			if (x<MinEdge.m_tX) MinEdge.m_tX = x;
			if (y<MinEdge.m_tY) MinEdge.m_tY = y;
			if (z<MinEdge.m_tZ) MinEdge.m_tZ = z;
		}

		//! Determines if a point is within this box.
		/** \param p: Point to check.
		\return True if the point is within the box and false if not */
		bool isPointInside(const CGDM_Vector3D<T>& p) const
		{
			return (p.m_tX >= MinEdge.m_tX && p.m_tX <= MaxEdge.m_tX &&
				p.m_tY >= MinEdge.m_tY && p.m_tY <= MaxEdge.m_tY &&
				p.m_tZ >= MinEdge.m_tZ && p.m_tZ <= MaxEdge.m_tZ);
		}

		//! Determines if a point is within this box and its borders.
		/** \param p: Point to check.
		\return True if the point is within the box and false if not. */
		bool isPointTotalInside(const CGDM_Vector3D<T>& p) const
		{
			return (p.m_tX > MinEdge.m_tX && p.m_tX < MaxEdge.m_tX &&
				p.m_tY > MinEdge.m_tY && p.m_tY < MaxEdge.m_tY &&
				p.m_tZ > MinEdge.m_tZ && p.m_tZ < MaxEdge.m_tZ);
		}

		//! Determines if the box intersects with another box.
		/** \param other: Other box to check a intersection with.
		\return True if there is an intersection with the other box,
		otherwise false. */
		bool intersectsWithBox(const CGDM_Aabbox3D<T>& other) const
		{
			return (MinEdge <= other.MaxEdge && MaxEdge >= other.MinEdge);
		}

		//! Check if this box is completely inside the 'other' box.
		/** \param other: Other box to check against.
		\return True if this box is completly inside the other box,
		otherwise false. */
		bool isFullInside(const CGDM_Aabbox3D<T>& other) const
		{
			return MinEdge >= other.MinEdge && MaxEdge <= other.MaxEdge;
		}

		//! Tests if the box intersects with a line
		/** \param line: Line to test intersection with.
		\return True if there is an intersection , else false. */
		bool intersectsWithLine(const CGDM_Line3D<T>& line) const
		{
			return intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
					(T)(line.getLength() * 0.5));
		}

		//! Tests if the box intersects with a line
		/** \param linemiddle Center of the line.
		\param linevect Vector of the line.
		\param halflength Half length of the line.
		\return True if there is an intersection, else false. */
		bool intersectsWithLine(const CGDM_Vector3D<T>& linemiddle,
					const CGDM_Vector3D<T>& linevect,
					T halflength) const
		{
			const CGDM_Vector3D<T> e = getExtent() * (T)0.5;
			const CGDM_Vector3D<T> t = getCenter() - linemiddle;

			if ((fabs(t.m_tX) > e.m_tX + halflength * fabs(linevect.m_tX)) ||
				(fabs(t.m_tY) > e.m_tY + halflength * fabs(linevect.m_tY)) ||
				(fabs(t.m_tZ) > e.m_tZ + halflength * fabs(linevect.m_tZ)) )
				return false;

			T r = e.m_tY * (T)fabs(linevect.m_tZ) + e.m_tZ * (T)fabs(linevect.m_tY);
			if (fabs(t.m_tY*linevect.m_tZ - t.m_tZ*linevect.m_tY) > r )
				return false;

			r = e.m_tX * (T)fabs(linevect.m_tZ) + e.m_tZ * (T)fabs(linevect.m_tX);
			if (fabs(t.m_tZ*linevect.m_tX - t.m_tX*linevect.m_tZ) > r )
				return false;

			r = e.m_tX * (T)fabs(linevect.m_tY) + e.m_tY * (T)fabs(linevect.m_tX);
			if (fabs(t.m_tX*linevect.m_tY - t.m_tY*linevect.m_tX) > r)
				return false;

			return true;
		}

		//! Classifies a relation with a plane.
		/** \param plane Plane to classify relation to.
		\return Returns ISREL3D_FRONT if the box is in front of the plane,
		ISREL3D_BACK if the box is behind the plane, and
		ISREL3D_CLIPPED if it is on both sides of the plane. */
		EIntersectionRelation3D classifyPlaneRelation(const CGDM_Plane3D<T>& plane) const
		{
			CGDM_Vector3D<T> nearPoint(MaxEdge);
			CGDM_Vector3D<T> farPoint(MinEdge);

			if (plane.Normal.m_tX > (T)0)
			{
				nearPoint.m_tX = MinEdge.m_tX;
				farPoint.m_tX = MaxEdge.m_tX;
			}

			if (plane.Normal.m_tY > (T)0)
			{
				nearPoint.m_tY = MinEdge.m_tY;
				farPoint.m_tY = MaxEdge.m_tY;
			}

			if (plane.Normal.m_tZ > (T)0)
			{
				nearPoint.m_tZ = MinEdge.m_tZ;
				farPoint.m_tZ = MaxEdge.m_tZ;
			}

			if (plane.Normal.dotProduct(nearPoint) + plane.D > (T)0)
				return ISREL3D_FRONT;

			if (plane.Normal.dotProduct(farPoint) + plane.D > (T)0)
				return ISREL3D_CLIPPED;

			return ISREL3D_BACK;
		}


		//! Get center of the bounding box
		/** \return Center of the bounding box. */
		CGDM_Vector3D<T> getCenter() const
		{
			return (MinEdge + MaxEdge) / 2;
		}


		//! Get extent of the box
		/** \return Extent of the bounding box. */
		CGDM_Vector3D<T> getExtent() const
		{
			return MaxEdge - MinEdge;
		}


		//! Stores all 8 edges of the box into an array
		/** \param edges: Pointer to array of 8 edges. */
		void getEdges(CGDM_Vector3D<T> *edges) const
		{
			const CGDM_Vector3D<T> middle = getCenter();
			const CGDM_Vector3D<T> diag = middle - MaxEdge;

			/*
			Edges are stored in this way:
			Hey, am I an ascii artist, or what? :) niko.
                  /3--------/7
                 /  |      / |
                /   |     /  |
                1---------5  |
                |   2- - -| -6
                |  /      |  /
                |/        | /
                0---------4/
			*/

			edges[0].set(middle.m_tX + diag.m_tX, middle.m_tY + diag.m_tY, middle.m_tZ + diag.m_tZ);
			edges[1].set(middle.m_tX + diag.m_tX, middle.m_tY - diag.m_tY, middle.m_tZ + diag.m_tZ);
			edges[2].set(middle.m_tX + diag.m_tX, middle.m_tY + diag.m_tY, middle.m_tZ - diag.m_tZ);
			edges[3].set(middle.m_tX + diag.m_tX, middle.m_tY - diag.m_tY, middle.m_tZ - diag.m_tZ);
			edges[4].set(middle.m_tX - diag.m_tX, middle.m_tY + diag.m_tY, middle.m_tZ + diag.m_tZ);
			edges[5].set(middle.m_tX - diag.m_tX, middle.m_tY - diag.m_tY, middle.m_tZ + diag.m_tZ);
			edges[6].set(middle.m_tX - diag.m_tX, middle.m_tY + diag.m_tY, middle.m_tZ - diag.m_tZ);
			edges[7].set(middle.m_tX - diag.m_tX, middle.m_tY - diag.m_tY, middle.m_tZ - diag.m_tZ);
		}


		//! Check if the box is empty.
		/** This means that there is no space between the min and max
		edge.
		\return True if box is empty, else false. */
		bool isEmpty() const
		{
			return MinEdge.equals ( MaxEdge );
		}


		//! Repairs the box.
		/** Necessary if for example MinEdge and MaxEdge are swapped. */
		void repair()
		{
			T t;

			if (MinEdge.m_tX > MaxEdge.m_tX)
				{ t=MinEdge.m_tX; MinEdge.m_tX = MaxEdge.m_tX; MaxEdge.m_tX=t; }
			if (MinEdge.m_tY > MaxEdge.m_tY)
				{ t=MinEdge.m_tY; MinEdge.m_tY = MaxEdge.m_tY; MaxEdge.m_tY=t; }
			if (MinEdge.m_tZ > MaxEdge.m_tZ)
				{ t=MinEdge.m_tZ; MinEdge.m_tZ = MaxEdge.m_tZ; MaxEdge.m_tZ=t; }
		}

		//! Calculates a new interpolated bounding box.
		/** \param other: other box to interpolate between
		\param d: value between 0.0f and 1.0f.
		\return Interpolated box. */
		CGDM_Aabbox3D<T> getInterpolated(const CGDM_Aabbox3D<T>& other, double d) const
		{
			double inv = 1.0f - d;
			return CGDM_Aabbox3D<T>((other.MinEdge*inv) + (MinEdge*d),
				(other.MaxEdge*inv) + (MaxEdge*d));
		}

		//! Get the volume enclosed by the box in cubed units
		T getVolume() const
		{
			const CGDM_Vector3D<T> e = getExtent();
			return e.m_tX * e.m_tY * e.m_tZ;
		}

		//! Get the surface area of the box in squared units
		T getArea() const
		{
			const CGDM_Vector3D<T> e = getExtent();
			return 2*(e.m_tX*e.m_tY + e.m_tX*e.m_tZ + e.m_tY*e.m_tZ);
		}

		//! The near edge
		CGDM_Vector3D<T> MinEdge;
		//! The far edge
		CGDM_Vector3D<T> MaxEdge;
};

	typedef CGDM_Aabbox3D<double> Aabbox3DF;
	//! Typedef for a double 3d bounding box.
	typedef CGDM_Aabbox3D<double> CGDM_Aabbox3DF;
	//! Typedef for an integer 3d bounding box.
	typedef CGDM_Aabbox3D<GDEM_INT> CGDM_Aabbox3DI;

#endif // __GDEM_AABBOX_3D_H_INCLUDED__
