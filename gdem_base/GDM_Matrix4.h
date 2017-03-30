#ifndef __GDEM_MATRIX4_H_INCLUDED__
#define __GDEM_MATRIX4_H_INCLUDED__

#include "GDM_Define.h"
#include "GDM_Math.h"
#include "GDM_Vector2D.h"
#include "GDM_Vector3D.h"
#include "GDM_Plane3D.h"
#include "GDM_Aabbox3D.h"
#include "GDM_Rect.h"
#include <string.h>

template <class T>
class TGDM_Matrix4
{
	public:

		//! Constructor Flags
		enum eConstructor
		{
			EM4CONST_NOTHING = 0,
			EM4CONST_COPY,
			EM4CONST_IDENTITY,
			EM4CONST_TRANSPOSED,
			EM4CONST_INVERSE,
			EM4CONST_INVERSE_TRANSPOSED
		};

		//! Default constructor
		/** \param constructor Choose the initialization style */
		TGDM_Matrix4( eConstructor constructor = EM4CONST_IDENTITY );
		//! Copy constructor
		/** \param other Other matrix to copy from
		\param constructor Choose the initialization style */
		TGDM_Matrix4( const TGDM_Matrix4<T>& other,eConstructor constructor = EM4CONST_COPY);

		//! Simple operator for directly accessing every element of the matrix.
		T& operator()(const GDEM_INT row, const GDEM_INT col) { definitelyIdentityMatrix=false; return M[ row * 4 + col ]; }

		//! Simple operator for directly accessing every element of the matrix.
		const T& operator()(const GDEM_INT row, const GDEM_INT col) const { return M[row * 4 + col]; }

		//! Simple operator for linearly accessing every element of the matrix.
		T& operator[](GDEM_UINT index) { definitelyIdentityMatrix=false; return M[index]; }

		//! Simple operator for linearly accessing every element of the matrix.
		const T& operator[](GDEM_UINT index) const { return M[index]; }

		//! Sets this matrix equal to the other matrix.
		inline TGDM_Matrix4<T>& operator=(const TGDM_Matrix4<T> &other);

		//! Sets all elements of this matrix to the value.
		inline TGDM_Matrix4<T>& operator=(const T& scalar);

		//! Returns pointer to internal array
		const T* pointer() const { return M; }
		T* pointer() { definitelyIdentityMatrix=false; return M; }

		//! Returns true if other matrix is equal to this matrix.
		bool operator==(const TGDM_Matrix4<T> &other) const;

		//! Returns true if other matrix is not equal to this matrix.
		bool operator!=(const TGDM_Matrix4<T> &other) const;

		//! Add another matrix.
		TGDM_Matrix4<T> operator+(const TGDM_Matrix4<T>& other) const;

		//! Add another matrix.
		TGDM_Matrix4<T>& operator+=(const TGDM_Matrix4<T>& other);

		//! Subtract another matrix.
		TGDM_Matrix4<T> operator-(const TGDM_Matrix4<T>& other) const;

		//! Subtract another matrix.
		TGDM_Matrix4<T>& operator-=(const TGDM_Matrix4<T>& other);

		//! set this matrix to the product of two matrices
		inline TGDM_Matrix4<T>& setbyproduct(const TGDM_Matrix4<T>& other_a,const TGDM_Matrix4<T>& other_b );

		//! Set this matrix to the product of two matrices
		/** no optimization used,
		use it if you know you never have a identity matrix */
		TGDM_Matrix4<T>& setbyproduct_nocheck(const TGDM_Matrix4<T>& other_a,const TGDM_Matrix4<T>& other_b );

		//! Multiply by another matrix.
		TGDM_Matrix4<T> operator*(const TGDM_Matrix4<T>& other) const;

		//! Multiply by another matrix.
		TGDM_Matrix4<T>& operator*=(const TGDM_Matrix4<T>& other);

		//! Multiply by scalar.
		TGDM_Matrix4<T> operator*(const T& scalar) const;

		//! Multiply by scalar.
		TGDM_Matrix4<T>& operator*=(const T& scalar);

		//! Set matrix to identity.
		inline TGDM_Matrix4<T>& makeIdentity();

		//! Returns true if the matrix is the identity matrix
		inline bool isIdentity() const;

		//! Returns true if the matrix is the identity matrix
		bool isIdentity_integer_base () const;

		//! Set the translation of the current matrix. Will erase any previous values.
		TGDM_Matrix4<T>& setTranslation( const CGDM_Vector3D<T>& translation );

		//! Gets the current translation
		CGDM_Vector3D<T> getTranslation() const;

		//! Set the inverse translation of the current matrix. Will erase any previous values.
		TGDM_Matrix4<T>& setInverseTranslation( const CGDM_Vector3D<T>& translation );

		//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
		inline TGDM_Matrix4<T>& setRotationRadians( const CGDM_Vector3D<T>& rotation );

		//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
		TGDM_Matrix4<T>& setRotationDegrees( const CGDM_Vector3D<T>& rotation );

		//! Returns the rotation, as set by setRotation().
		/** This code was orginally written by by Chev. */
		CGDM_Vector3D<T> getRotationDegrees() const;

		//! Make an inverted rotation matrix from Euler angles.
		/** The 4th row and column are unmodified. */
		inline TGDM_Matrix4<T>& setInverseRotationRadians( const CGDM_Vector3D<T>& rotation );

		//! Make an inverted rotation matrix from Euler angles.
		/** The 4th row and column are unmodified. */
		TGDM_Matrix4<T>& setInverseRotationDegrees( const CGDM_Vector3D<T>& rotation );

		//! Set Scale
		TGDM_Matrix4<T>& setScale( const CGDM_Vector3D<T>& scale );

		//! Set Scale
		TGDM_Matrix4<T>& setScale( const T scale ) { return setScale(CGDM_Vector3D<T>(scale,scale,scale)); }

		//! Get Scale
		CGDM_Vector3D<T> getScale() const;

		//! Translate a vector by the inverse of the translation part of this matrix.
		void inverseTranslateVect( CGDM_Vector3DF& vect ) const;

		//! Rotate a vector by the inverse of the rotation part of this matrix.
		void inverseRotateVect( CGDM_Vector3DF& vect ) const;

		//! Rotate a vector by the rotation part of this matrix.
		void rotateVect( CGDM_Vector3DF& vect ) const;

		//! An alternate transform vector method, writing into a second vector
		void rotateVect(CGDM_Vector3DF& out, const CGDM_Vector3DF& in) const;

		//! An alternate transform vector method, writing into an array of 3 floats
		void rotateVect(T *out,const CGDM_Vector3DF &in) const;

		//! Transforms the vector by this matrix
		void transformVect( CGDM_Vector3DF& vect) const;

		//! Transforms input vector by this matrix and stores result in output vector
		void transformVect( CGDM_Vector3DF& out, const CGDM_Vector3DF& in ) const;

		//! An alternate transform vector method, writing into an array of 4 floats
		void transformVect(T *out,const CGDM_Vector3DF &in) const;

		//! Translate a vector by the translation part of this matrix.
		void translateVect( CGDM_Vector3DF& vect ) const;

		//! Transforms a plane by this matrix
		void transformPlane( CGDM_Plane3D<double> &plane) const;

		//! Transforms a plane by this matrix
		void transformPlane( const CGDM_Plane3D<double> &in, CGDM_Plane3D<double> &out) const;

		//! Transforms a axis aligned bounding box
		/** The result box of this operation may not be accurate at all. For
		correct results, use transformBoxEx() */
		void transformBox(CGDM_Aabbox3D<double>& box) const;

		//! Transforms a axis aligned bounding box
		/** The result box of this operation should by accurate, but this operation
		is slower than transformBox(). */
		void transformBoxEx(CGDM_Aabbox3D<double>& box) const;

		//! Multiplies this matrix by a 1x4 matrix
		void multiplyWith1x4Matrix(T* matrix) const;

		//! Calculates inverse of matrix. Slow.
		/** \return Returns false if there is no inverse matrix.*/
		bool makeInverse();


		//! Inverts a primitive matrix which only contains a translation and a rotation
		/** \param out: where result matrix is written to. */
		bool getInversePrimitive ( TGDM_Matrix4<T>& out ) const;

		//! Gets the inversed matrix of this one
		/** \param out: where result matrix is written to.
		\return Returns false if there is no inverse matrix. */
		bool getInverse(TGDM_Matrix4<T>& out) const;

		//! Builds a right-handed perspective projection matrix based on a field of view
		TGDM_Matrix4<T>& buildProjectionMatrixPerspectiveFovRH(double fieldOfViewRadians, double aspectRatio, double zNear, double zFar);

		//! Builds a left-handed perspective projection matrix based on a field of view
		TGDM_Matrix4<T>& buildProjectionMatrixPerspectiveFovLH(double fieldOfViewRadians, double aspectRatio, double zNear, double zFar);

		//! Builds a right-handed perspective projection matrix.
		TGDM_Matrix4<T>& buildProjectionMatrixPerspectiveRH(double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar);

		//! Builds a left-handed perspective projection matrix.
		TGDM_Matrix4<T>& buildProjectionMatrixPerspectiveLH(double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar);

		//! Builds a left-handed orthogonal projection matrix.
		TGDM_Matrix4<T>& buildProjectionMatrixOrthoLH(double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar);

		//! Builds a right-handed orthogonal projection matrix.
		TGDM_Matrix4<T>& buildProjectionMatrixOrthoRH(double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar);

		//! Builds a left-handed look-at matrix.
		TGDM_Matrix4<T>& buildCameraLookAtMatrixLH(
				const CGDM_Vector3DF& position,
				const CGDM_Vector3DF& target,
				const CGDM_Vector3DF& upVector);

		//! Builds a right-handed look-at matrix.
		TGDM_Matrix4<T>& buildCameraLookAtMatrixRH(
				const CGDM_Vector3DF& position,
				const CGDM_Vector3DF& target,
				const CGDM_Vector3DF& upVector);

		//! Builds a matrix that flattens geometry into a plane.
		/** \param light: light source
		\param plane: plane into which the geometry if flattened into
		\param point: value between 0 and 1, describing the light source.
		If this is 1, it is a point light, if it is 0, it is a directional light. */
		TGDM_Matrix4<T>& buildShadowMatrix(const CGDM_Vector3DF& light, CGDM_Plane3DF plane, double point=1.0f);

		//! Builds a matrix which transforms a normalized Device Coordinate to Device Coordinates.
		/** Used to scale <-1,-1><1,1> to viewport, for example from von <-1,-1> <1,1> to the viewport <0,0><0,640> */
		TGDM_Matrix4<T>& buildNDCToDCMatrix( const CGDM_Rect<GDEM_INT>& area, double zScale);

		//! Creates a new matrix as interpolated matrix from two other ones.
		/** \param b: other matrix to interpolate with
		\param time: Must be a value between 0 and 1. */
		TGDM_Matrix4<T> interpolate(const TGDM_Matrix4<T>& b, double time) const;

		//! Gets transposed matrix
		TGDM_Matrix4<T> getTransposed() const;

		//! Gets transposed matrix
		inline void getTransposed( TGDM_Matrix4<T>& dest ) const;

		/*
			construct 2D Texture transformations
			rotate about center, scale, and transform.
		*/
		//! Set to a texture transformation matrix with the given parameters.
		TGDM_Matrix4<T>& buildTextureTransform( double rotateRad,
				const CGDM_Vector2DF &rotatecenter,
				const CGDM_Vector2DF &translate,
				const CGDM_Vector2DF &scale);

		//! Set texture transformation rotation
		/** Rotate about z axis, recenter at (0.5,0.5).
		Doesn't clear other elements than those affected
		\param radAngle Angle in radians
		\return Altered matrix */
		TGDM_Matrix4<T>& setTextureRotationCenter( double radAngle );

		//! Set texture transformation translation
		/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
		TGDM_Matrix4<T>& setTextureTranslate( double x, double y );

		//! Set texture transformation translation, using a transposed representation
		/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
		TGDM_Matrix4<T>& setTextureTranslateTransposed( double x, double y );

		//! Set texture transformation scale
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		TGDM_Matrix4<T>& setTextureScale( double sx, double sy );

		//! Set texture transformation scale, and recenter at (0.5,0.5)
		/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
		TGDM_Matrix4<T>& setTextureScaleCenter( double sx, double sy );

		//! Sets all matrix data members at once
		TGDM_Matrix4<T>& setM(const T* data);

		//! Sets if the matrix is definitely identity matrix
		void setDefinitelyIdentityMatrix( bool isDefinitelyIdentityMatrix);

		//! Gets if the matrix is definitely identity matrix
		bool getDefinitelyIdentityMatrix() const;

	private:
		//! Matrix data, stored in row-major order
		T M[16];
		//! Flag is this matrix is identity matrix
		mutable bool definitelyIdentityMatrix;
};


	// Default constructor
	template <class T>
	inline TGDM_Matrix4<T>::TGDM_Matrix4( eConstructor constructor ) : definitelyIdentityMatrix(false)
	{
		switch ( constructor )
		{
			case EM4CONST_NOTHING:
			case EM4CONST_COPY:
				break;
			case EM4CONST_IDENTITY:
			case EM4CONST_INVERSE:
			default:
				makeIdentity();
				break;
		}
	}

	// Copy constructor
	template <class T>
	inline TGDM_Matrix4<T>::TGDM_Matrix4( const TGDM_Matrix4<T>& other, eConstructor constructor) : definitelyIdentityMatrix(false)
	{
		switch ( constructor )
		{
			case EM4CONST_IDENTITY:
				makeIdentity();
				break;
			case EM4CONST_NOTHING:
				break;
			case EM4CONST_COPY:
				*this = other;
				break;
			case EM4CONST_TRANSPOSED:
				other.getTransposed(*this);
				break;
			case EM4CONST_INVERSE:
				if (!other.getInverse(*this))
					memset(M, 0, 16*sizeof(T));
				break;
			case EM4CONST_INVERSE_TRANSPOSED:
				if (!other.getInverse(*this))
					memset(M, 0, 16*sizeof(T));
				else
					*this=getTransposed();
				break;
		}
	}

	//! Add another matrix.
	template <class T>
	inline TGDM_Matrix4<T> TGDM_Matrix4<T>::operator+(const TGDM_Matrix4<T>& other) const
	{
		TGDM_Matrix4<T> temp ( EM4CONST_NOTHING );

		temp[0] = M[0]+other[0];
		temp[1] = M[1]+other[1];
		temp[2] = M[2]+other[2];
		temp[3] = M[3]+other[3];
		temp[4] = M[4]+other[4];
		temp[5] = M[5]+other[5];
		temp[6] = M[6]+other[6];
		temp[7] = M[7]+other[7];
		temp[8] = M[8]+other[8];
		temp[9] = M[9]+other[9];
		temp[10] = M[10]+other[10];
		temp[11] = M[11]+other[11];
		temp[12] = M[12]+other[12];
		temp[13] = M[13]+other[13];
		temp[14] = M[14]+other[14];
		temp[15] = M[15]+other[15];

		return temp;
	}

	//! Add another matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::operator+=(const TGDM_Matrix4<T>& other)
	{
		M[0]+=other[0];
		M[1]+=other[1];
		M[2]+=other[2];
		M[3]+=other[3];
		M[4]+=other[4];
		M[5]+=other[5];
		M[6]+=other[6];
		M[7]+=other[7];
		M[8]+=other[8];
		M[9]+=other[9];
		M[10]+=other[10];
		M[11]+=other[11];
		M[12]+=other[12];
		M[13]+=other[13];
		M[14]+=other[14];
		M[15]+=other[15];

		return *this;
	}

	//! Subtract another matrix.
	template <class T>
	inline TGDM_Matrix4<T> TGDM_Matrix4<T>::operator-(const TGDM_Matrix4<T>& other) const
	{
		TGDM_Matrix4<T> temp ( EM4CONST_NOTHING );

		temp[0] = M[0]-other[0];
		temp[1] = M[1]-other[1];
		temp[2] = M[2]-other[2];
		temp[3] = M[3]-other[3];
		temp[4] = M[4]-other[4];
		temp[5] = M[5]-other[5];
		temp[6] = M[6]-other[6];
		temp[7] = M[7]-other[7];
		temp[8] = M[8]-other[8];
		temp[9] = M[9]-other[9];
		temp[10] = M[10]-other[10];
		temp[11] = M[11]-other[11];
		temp[12] = M[12]-other[12];
		temp[13] = M[13]-other[13];
		temp[14] = M[14]-other[14];
		temp[15] = M[15]-other[15];

		return temp;
	}

	//! Subtract another matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::operator-=(const TGDM_Matrix4<T>& other)
	{
		M[0]-=other[0];
		M[1]-=other[1];
		M[2]-=other[2];
		M[3]-=other[3];
		M[4]-=other[4];
		M[5]-=other[5];
		M[6]-=other[6];
		M[7]-=other[7];
		M[8]-=other[8];
		M[9]-=other[9];
		M[10]-=other[10];
		M[11]-=other[11];
		M[12]-=other[12];
		M[13]-=other[13];
		M[14]-=other[14];
		M[15]-=other[15];

		return *this;
	}

	//! Multiply by scalar.
	template <class T>
	inline TGDM_Matrix4<T> TGDM_Matrix4<T>::operator*(const T& scalar) const
	{
		TGDM_Matrix4<T> temp ( EM4CONST_NOTHING );

		temp[0] = M[0]*scalar;
		temp[1] = M[1]*scalar;
		temp[2] = M[2]*scalar;
		temp[3] = M[3]*scalar;
		temp[4] = M[4]*scalar;
		temp[5] = M[5]*scalar;
		temp[6] = M[6]*scalar;
		temp[7] = M[7]*scalar;
		temp[8] = M[8]*scalar;
		temp[9] = M[9]*scalar;
		temp[10] = M[10]*scalar;
		temp[11] = M[11]*scalar;
		temp[12] = M[12]*scalar;
		temp[13] = M[13]*scalar;
		temp[14] = M[14]*scalar;
		temp[15] = M[15]*scalar;

		return temp;
	}

	//! Multiply by scalar.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::operator*=(const T& scalar)
	{
		M[0]*=scalar;
		M[1]*=scalar;
		M[2]*=scalar;
		M[3]*=scalar;
		M[4]*=scalar;
		M[5]*=scalar;
		M[6]*=scalar;
		M[7]*=scalar;
		M[8]*=scalar;
		M[9]*=scalar;
		M[10]*=scalar;
		M[11]*=scalar;
		M[12]*=scalar;
		M[13]*=scalar;
		M[14]*=scalar;
		M[15]*=scalar;

		return *this;
	}

	//! Multiply by another matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::operator*=(const TGDM_Matrix4<T>& other)
	{
		// do checks on your own in order to avoid copy creation
		if ( !other.isIdentity() )
		{
			if ( this->isIdentity() )
			{
				return (*this = other);
			}
			else
			{
				TGDM_Matrix4<T> temp ( *this );
				return setbyproduct_nocheck( temp, other );
			}
		}
		return *this;
	}

	//! multiply by another matrix
	// set this matrix to the product of two other matrices
	// goal is to reduce stack use and copy
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setbyproduct_nocheck(const TGDM_Matrix4<T>& other_a,const TGDM_Matrix4<T>& other_b )
	{
		const T *m1 = other_a.M;
		const T *m2 = other_b.M;

		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		definitelyIdentityMatrix=false;
		return *this;
	}


	//! multiply by another matrix
	// set this matrix to the product of two other matrices
	// goal is to reduce stack use and copy
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setbyproduct(const TGDM_Matrix4<T>& other_a, const TGDM_Matrix4<T>& other_b )
	{
		if ( other_a.isIdentity () )
			return (*this = other_b);
		else
		if ( other_b.isIdentity () )
			return (*this = other_a);
		else
			return setbyproduct_nocheck(other_a,other_b);
	}

	//! multiply by another matrix
	template <class T>
	inline TGDM_Matrix4<T> TGDM_Matrix4<T>::operator*(const TGDM_Matrix4<T>& m2) const
	{
		// Testing purpose..
		if ( this->isIdentity() )
			return m2;
		if ( m2.isIdentity() )
			return *this;

		TGDM_Matrix4<T> m3 ( EM4CONST_NOTHING );

		const T *m1 = M;

		m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		return m3;
	}



	template <class T>
	inline CGDM_Vector3D<T> TGDM_Matrix4<T>::getTranslation() const
	{
		return CGDM_Vector3D<T>(M[12], M[13], M[14]);
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setTranslation( const CGDM_Vector3D<T>& translation )
	{
		M[12] = translation.m_tX;
		M[13] = translation.m_tY;
		M[14] = translation.m_tZ;
		definitelyIdentityMatrix=false;
		return *this;
	}

	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setInverseTranslation( const CGDM_Vector3D<T>& translation )
	{
		M[12] = -translation.m_tX;
		M[13] = -translation.m_tY;
		M[14] = -translation.m_tZ;
		definitelyIdentityMatrix=false;
		return *this;
	}

	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setScale( const CGDM_Vector3D<T>& scale )
	{
		M[0] = scale.m_tX;
		M[5] = scale.m_tY;
		M[10] = scale.m_tZ;
		definitelyIdentityMatrix=false;
		return *this;
	}

	template <class T>
	inline CGDM_Vector3D<T> TGDM_Matrix4<T>::getScale() const
	{
		return CGDM_Vector3D<T>(M[0],M[5],M[10]);
	}

	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setRotationDegrees( const CGDM_Vector3D<T>& rotation )
	{
		return setRotationRadians( rotation * gdm_DEGTORAD );
	}

	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setInverseRotationDegrees( const CGDM_Vector3D<T>& rotation )
	{
		return setInverseRotationRadians( rotation * gdm_DEGTORAD );
	}

	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setRotationRadians( const CGDM_Vector3D<T>& rotation )
	{
		const double cr = cos( rotation.m_tX );
		const double sr = sin( rotation.m_tX );
		const double cp = cos( rotation.m_tY );
		const double sp = sin( rotation.m_tY );
		const double cy = cos( rotation.m_tZ );
		const double sy = sin( rotation.m_tZ );

		M[0] = (T)( cp*cy );
		M[1] = (T)( cp*sy );
		M[2] = (T)( -sp );

		const double srsp = sr*sp;
		const double crsp = cr*sp;

		M[4] = (T)( srsp*cy-cr*sy );
		M[5] = (T)( srsp*sy+cr*cy );
		M[6] = (T)( sr*cp );

		M[8] = (T)( crsp*cy+sr*sy );
		M[9] = (T)( crsp*sy-sr*cy );
		M[10] = (T)( cr*cp );
		definitelyIdentityMatrix=false;
		return *this;
	}


	//! Returns a rotation that is equivalent to that set by setRotationDegrees().
	/** This code was sent in by Chev.  Note that it does not necessarily return
	the *same* Euler angles as those set by setRotationDegrees(), but the rotation will
	be equivalent, i.e. will have the same result when used to rotate a vector or node. */
	template <class T>
	inline CGDM_Vector3D<T> TGDM_Matrix4<T>::getRotationDegrees() const
	{
		const TGDM_Matrix4<T> &mat = *this;

		double Y = -asin(mat(0,2));
		const double C = cos(Y);
		Y *= gdm_RADTODEG64;

		double rotx, roty, X, Z;

		if (fabs(C)>gdm_ROUNDING_ERROR_64)
		{
			const T invC = (T)(1.0/C);
			rotx = mat(2,2) * invC;
			roty = mat(1,2) * invC;
			X = atan2( roty, rotx ) * gdm_RADTODEG64;
			rotx = mat(0,0) * invC;
			roty = mat(0,1) * invC;
			Z = atan2( roty, rotx ) * gdm_RADTODEG64;
		}
		else
		{
			X = 0.0;
			rotx = mat(1,1);
			roty = -mat(1,0);
			Z = atan2( roty, rotx ) * gdm_RADTODEG64;
		}

		// fix values that get below zero
		// before it would set (!) values to 360
		// that where above 360:
		if (X < 0.0) X += 360.0;
		if (Y < 0.0) Y += 360.0;
		if (Z < 0.0) Z += 360.0;

		return CGDM_Vector3D<T>((T)X,(T)Y,(T)Z);
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setInverseRotationRadians( const CGDM_Vector3D<T>& rotation )
	{
		double cr = cos( rotation.m_tX );
		double sr = sin( rotation.m_tX );
		double cp = cos( rotation.m_tY );
		double sp = sin( rotation.m_tY );
		double cy = cos( rotation.m_tZ );
		double sy = sin( rotation.m_tZ );

		M[0] = (T)( cp*cy );
		M[4] = (T)( cp*sy );
		M[8] = (T)( -sp );

		double srsp = sr*sp;
		double crsp = cr*sp;

		M[1] = (T)( srsp*cy-cr*sy );
		M[5] = (T)( srsp*sy+cr*cy );
		M[9] = (T)( sr*cp );

		M[2] = (T)( crsp*cy+sr*sy );
		M[6] = (T)( crsp*sy-sr*cy );
		M[10] = (T)( cr*cp );
		definitelyIdentityMatrix=false;
		return *this;
	}


	/*!
	*/
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::makeIdentity()
	{
		memset(M, 0, 16*sizeof(T));
		M[0] = M[5] = M[10] = M[15] = (T)1;
		definitelyIdentityMatrix=true;
		return *this;
	}


	/*
		check identity with epsilon
		solve floating range problems..
	*/
	template <class T>
	inline bool TGDM_Matrix4<T>::isIdentity() const
	{
		if (definitelyIdentityMatrix)
			return true;
		if (!gdm_equals( M[ 0], (T)1 ) ||
			!gdm_equals( M[ 5], (T)1 ) ||
			!gdm_equals( M[10], (T)1 ) ||
			!gdm_equals( M[15], (T)1 ))
			return false;

		for (GDEM_INT i=0; i<4; ++i)
			for (GDEM_INT j=0; j<4; ++j)
				if ((j != i) && (!gdm_iszero((*this)(i,j))))
					return false;

		definitelyIdentityMatrix=true;
		return true;
	}

	/*
		doesn't solve floating range problems..
		but takes care on +/- 0 on translation because we are changing it..
		reducing floating point branches
		but it needs the floats in memory..
	*/
	template <class T>
	inline bool TGDM_Matrix4<T>::isIdentity_integer_base() const
	{
		if (definitelyIdentityMatrix)
			return true;
		if(IR(M[0])!=F_32_VALUE_1)	return false;
		if(IR(M[1])!=0)			return false;
		if(IR(M[2])!=0)			return false;
		if(IR(M[3])!=0)			return false;

		if(IR(M[4])!=0)			return false;
		if(IR(M[5])!=F_32_VALUE_1)	return false;
		if(IR(M[6])!=0)			return false;
		if(IR(M[7])!=0)			return false;

		if(IR(M[8])!=0)			return false;
		if(IR(M[9])!=0)			return false;
		if(IR(M[10])!=F_32_VALUE_1)	return false;
		if(IR(M[11])!=0)		return false;

		if(IR(M[12])!=0)		return false;
		if(IR(M[13])!=0)		return false;
		if(IR(M[13])!=0)		return false;
		if(IR(M[15])!=F_32_VALUE_1)	return false;
		definitelyIdentityMatrix=true;
		return true;
	}


	template <class T>
	inline void TGDM_Matrix4<T>::rotateVect( CGDM_Vector3DF& vect ) const
	{
		CGDM_Vector3DF tmp = vect;
		vect.m_tX = tmp.m_tX*M[0] + tmp.m_tY*M[4] + tmp.m_tZ*M[8];
		vect.m_tY = tmp.m_tX*M[1] + tmp.m_tY*M[5] + tmp.m_tZ*M[9];
		vect.m_tZ = tmp.m_tX*M[2] + tmp.m_tY*M[6] + tmp.m_tZ*M[10];
	}

	//! An alternate transform vector method, writing into a second vector
	template <class T>
	inline void TGDM_Matrix4<T>::rotateVect(CGDM_Vector3DF& out, const CGDM_Vector3DF& in) const
	{
		out.m_tX = in.m_tX*M[0] + in.m_tY*M[4] + in.m_tZ*M[8];
		out.m_tY = in.m_tX*M[1] + in.m_tY*M[5] + in.m_tZ*M[9];
		out.m_tZ = in.m_tX*M[2] + in.m_tY*M[6] + in.m_tZ*M[10];
	}

	//! An alternate transform vector method, writing into an array of 3 floats
	template <class T>
	inline void TGDM_Matrix4<T>::rotateVect(T *out, const CGDM_Vector3DF& in) const
	{
		out[0] = in.m_tX*M[0] + in.m_tY*M[4] + in.m_tZ*M[8];
		out[1] = in.m_tX*M[1] + in.m_tY*M[5] + in.m_tZ*M[9];
		out[2] = in.m_tX*M[2] + in.m_tY*M[6] + in.m_tZ*M[10];
	}

	template <class T>
	inline void TGDM_Matrix4<T>::inverseRotateVect( CGDM_Vector3DF& vect ) const
	{
		CGDM_Vector3DF tmp = vect;
		vect.m_tX = tmp.m_tX*M[0] + tmp.m_tY*M[1] + tmp.m_tZ*M[2];
		vect.m_tY = tmp.m_tX*M[4] + tmp.m_tY*M[5] + tmp.m_tZ*M[6];
		vect.m_tZ = tmp.m_tX*M[8] + tmp.m_tY*M[9] + tmp.m_tZ*M[10];
	}

	template <class T>
	inline void TGDM_Matrix4<T>::transformVect( CGDM_Vector3DF& vect) const
	{
		double vector[3];

		vector[0] = vect.m_tX*M[0] + vect.m_tY*M[4] + vect.m_tZ*M[8] + M[12];
		vector[1] = vect.m_tX*M[1] + vect.m_tY*M[5] + vect.m_tZ*M[9] + M[13];
		vector[2] = vect.m_tX*M[2] + vect.m_tY*M[6] + vect.m_tZ*M[10] + M[14];

		vect.m_tX = vector[0];
		vect.m_tY = vector[1];
		vect.m_tZ = vector[2];
	}

	template <class T>
	inline void TGDM_Matrix4<T>::transformVect( CGDM_Vector3DF& out, const CGDM_Vector3DF& in) const
	{
		out.m_tX = in.m_tX*M[0] + in.m_tY*M[4] + in.m_tZ*M[8] + M[12];
		out.m_tY = in.m_tX*M[1] + in.m_tY*M[5] + in.m_tZ*M[9] + M[13];
		out.m_tZ = in.m_tX*M[2] + in.m_tY*M[6] + in.m_tZ*M[10] + M[14];
	}


	template <class T>
	inline void TGDM_Matrix4<T>::transformVect(T *out, const CGDM_Vector3DF &in) const
	{
		out[0] = in.m_tX*M[0] + in.m_tY*M[4] + in.m_tZ*M[8] + M[12];
		out[1] = in.m_tX*M[1] + in.m_tY*M[5] + in.m_tZ*M[9] + M[13];
		out[2] = in.m_tX*M[2] + in.m_tY*M[6] + in.m_tZ*M[10] + M[14];
		out[3] = in.m_tX*M[3] + in.m_tY*M[7] + in.m_tZ*M[11] + M[15];
	}


	//! Transforms a plane by this matrix
	template <class T>
	inline void TGDM_Matrix4<T>::transformPlane( CGDM_Plane3D<double> &plane) const
	{
	/*
		CGDM_Vector3DF member;
		// Fully transform the plane member point, i.e. rotate, translate and scale it.
		transformVect(member, plane.getMemberPoint());

		CGDM_Vector3DF normal = plane.Normal;
		normal.normalize();

		// The normal needs to be rotated and inverse scaled, but not translated.
		const CGDM_Vector3DF scale = getScale();

		if(!equals(scale.m_tX, 0.f) && !equals(scale.m_tY, 0.f) && !equals(scale.m_tZ, 0.f)
			&& (!equals(scale.m_tX, 1.f) || !equals(scale.m_tY, 1.f) || !equals(scale.m_tZ, 1.f)))
		{
			// Rotating the vector will also apply the scale, so we have to invert it twice.
			normal /= (scale * scale);
		}

		rotateVect(normal);

		normal.normalize();
		plane.setPlane(member, normal);
	*/
	}

	//! Transforms a plane by this matrix
	template <class T>
	inline void TGDM_Matrix4<T>::transformPlane( const CGDM_Plane3D<double> &in, CGDM_Plane3D<double> &out) const
	{
		out = in;
		transformPlane( out );
	}

	//! Transforms a axis aligned bounding box
	template <class T>
	inline void TGDM_Matrix4<T>::transformBox(CGDM_Aabbox3D<double>& box) const
	{
		if (isIdentity())
			return;

		transformVect(box.MinEdge);
		transformVect(box.MaxEdge);
		box.repair();
	}

	//! Transforms a axis aligned bounding box more accurately than transformBox()
	template <class T>
	inline void TGDM_Matrix4<T>::transformBoxEx(CGDM_Aabbox3D<double>& box) const
	{
		const double Amin[3] = {box.MinEdge.m_tX, box.MinEdge.m_tY, box.MinEdge.m_tZ};
		const double Amax[3] = {box.MaxEdge.m_tX, box.MaxEdge.m_tY, box.MaxEdge.m_tZ};

		double Bmin[3];
		double Bmax[3];

		Bmin[0] = Bmax[0] = M[12];
		Bmin[1] = Bmax[1] = M[13];
		Bmin[2] = Bmax[2] = M[14];

		const TGDM_Matrix4<T> &m = *this;

		for (GDEM_UINT i = 0; i < 3; ++i)
		{
			for (GDEM_UINT j = 0; j < 3; ++j)
			{
				const double a = m(j,i) * Amin[j];
				const double b = m(j,i) * Amax[j];

				if (a < b)
				{
					Bmin[i] += a;
					Bmax[i] += b;
				}
				else
				{
					Bmin[i] += b;
					Bmax[i] += a;
				}
			}
		}

		box.MinEdge.m_tX = Bmin[0];
		box.MinEdge.m_tY = Bmin[1];
		box.MinEdge.m_tZ = Bmin[2];

		box.MaxEdge.m_tX = Bmax[0];
		box.MaxEdge.m_tY = Bmax[1];
		box.MaxEdge.m_tZ = Bmax[2];
	}


	//! Multiplies this matrix by a 1x4 matrix
	template <class T>
	inline void TGDM_Matrix4<T>::multiplyWith1x4Matrix(T* matrix) const
	{
		/*
		0  1  2  3
		4  5  6  7
		8  9  10 11
		12 13 14 15
		*/

		T mat[4];
		mat[0] = matrix[0];
		mat[1] = matrix[1];
		mat[2] = matrix[2];
		mat[3] = matrix[3];

		matrix[0] = M[0]*mat[0] + M[4]*mat[1] + M[8]*mat[2] + M[12]*mat[3];
		matrix[1] = M[1]*mat[0] + M[5]*mat[1] + M[9]*mat[2] + M[13]*mat[3];
		matrix[2] = M[2]*mat[0] + M[6]*mat[1] + M[10]*mat[2] + M[14]*mat[3];
		matrix[3] = M[3]*mat[0] + M[7]*mat[1] + M[11]*mat[2] + M[15]*mat[3];
	}

	template <class T>
	inline void TGDM_Matrix4<T>::inverseTranslateVect( CGDM_Vector3DF& vect ) const
	{
		vect.m_tX = vect.m_tX-M[12];
		vect.m_tY = vect.m_tY-M[13];
		vect.m_tZ = vect.m_tZ-M[14];
	}

	template <class T>
	inline void TGDM_Matrix4<T>::translateVect( CGDM_Vector3DF& vect ) const
	{
		vect.m_tX = vect.m_tX+M[12];
		vect.m_tY = vect.m_tY+M[13];
		vect.m_tZ = vect.m_tZ+M[14];
	}


	template <class T>
	inline bool TGDM_Matrix4<T>::getInverse(TGDM_Matrix4<T>& out) const
	{
		/// Calculates the inverse of this Matrix
		/// The inverse is calculated using Cramers rule.
		/// If no inverse exists then 'false' is returned.

		if ( this->isIdentity() )
		{
			out=*this;
			return true;
		}

		const TGDM_Matrix4<T> &m = *this;

		double d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
			(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
			(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
			(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
			(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
			(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

		if( gdm_iszero ( d ) )
			return false;

		d = gdm_reciprocal ( d );

		out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
				m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
				m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
		out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
				m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
				m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
		out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
				m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
				m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
		out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
				m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
				m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
		out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
				m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
				m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
		out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
				m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
				m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
		out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
				m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
				m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
		out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
				m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
				m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
		out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
				m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
				m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
		out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
				m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
				m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
		out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
				m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
				m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
		out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
				m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
				m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
		out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
				m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
				m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
		out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
				m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
				m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
		out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
				m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
				m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
		out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
				m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
				m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));
		out.definitelyIdentityMatrix = definitelyIdentityMatrix;
		return true;
	}


	//! Inverts a primitive matrix which only contains a translation and a rotation
	//! \param out: where result matrix is written to.
	template <class T>
	inline bool TGDM_Matrix4<T>::getInversePrimitive ( TGDM_Matrix4<T>& out ) const
	{
		out.M[0 ] = M[0];
		out.M[1 ] = M[4];
		out.M[2 ] = M[8];
		out.M[3 ] = 0;

		out.M[4 ] = M[1];
		out.M[5 ] = M[5];
		out.M[6 ] = M[9];
		out.M[7 ] = 0;

		out.M[8 ] = M[2];
		out.M[9 ] = M[6];
		out.M[10] = M[10];
		out.M[11] = 0;

		out.M[12] = (T)-(M[12]*M[0] + M[13]*M[1] + M[14]*M[2]);
		out.M[13] = (T)-(M[12]*M[4] + M[13]*M[5] + M[14]*M[6]);
		out.M[14] = (T)-(M[12]*M[8] + M[13]*M[9] + M[14]*M[10]);
		out.M[15] = 1;
		out.definitelyIdentityMatrix = definitelyIdentityMatrix;
		return true;
	}

	/*!
	*/
	template <class T>
	inline bool TGDM_Matrix4<T>::makeInverse()
	{
		if (definitelyIdentityMatrix)
			return true;

		TGDM_Matrix4<T> temp ( EM4CONST_NOTHING );

		if (getInverse(temp))
		{
			*this = temp;
			return true;
		}

		return false;
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::operator=(const TGDM_Matrix4<T> &other)
	{
		if (this==&other)
			return *this;
		memcpy(M, other.M, 16*sizeof(T));
		definitelyIdentityMatrix=other.definitelyIdentityMatrix;
		return *this;
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::operator=(const T& scalar)
	{
		for (GDEM_INT i = 0; i < 16; ++i)
			M[i]=scalar;
		definitelyIdentityMatrix=false;
		return *this;
	}


	template <class T>
	inline bool TGDM_Matrix4<T>::operator==(const TGDM_Matrix4<T> &other) const
	{
		if (definitelyIdentityMatrix && other.definitelyIdentityMatrix)
			return true;
		for (GDEM_INT i = 0; i < 16; ++i)
			if (M[i] != other.M[i])
				return false;

		return true;
	}


	template <class T>
	inline bool TGDM_Matrix4<T>::operator!=(const TGDM_Matrix4<T> &other) const
	{
		return !(*this == other);
	}


	// Builds a right-handed perspective projection matrix based on a field of view
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildProjectionMatrixPerspectiveFovRH(
			double fieldOfViewRadians, double aspectRatio, double zNear, double zFar)
	{
		const double h = 1.0/tan(fieldOfViewRadians/2.0);
		const T w = h / aspectRatio;

		M[0] = w;
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = (T)h;
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = (T)(zFar/(zNear-zFar)); // DirectX version
	//		M[10] = (T)(zFar+zNear/(zNear-zFar)); // OpenGL version
		M[11] = -1;

		M[12] = 0;
		M[13] = 0;
		M[14] = (T)(zNear*zFar/(zNear-zFar)); // DirectX version
	//		M[14] = (T)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
		M[15] = 0;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// Builds a left-handed perspective projection matrix based on a field of view
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildProjectionMatrixPerspectiveFovLH(
			double fieldOfViewRadians, double aspectRatio, double zNear, double zFar)
	{
		const double h = 1.0/tan(fieldOfViewRadians/2.0);
		const T w = (T)(h / aspectRatio);

		M[0] = w;
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = (T)h;
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = (T)(zFar/(zFar-zNear));
		M[11] = 1;

		M[12] = 0;
		M[13] = 0;
		M[14] = (T)(-zNear*zFar/(zFar-zNear));
		M[15] = 0;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// Builds a left-handed orthogonal projection matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildProjectionMatrixOrthoLH(
			double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar)
	{
		M[0] = (T)(2/widthOfViewVolume);
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = (T)(2/heightOfViewVolume);
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = (T)(1/(zFar-zNear));
		M[11] = 0;

		M[12] = 0;
		M[13] = 0;
		M[14] = (T)(zNear/(zNear-zFar));
		M[15] = 1;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// Builds a right-handed orthogonal projection matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildProjectionMatrixOrthoRH(
			double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar)
	{
		M[0] = (T)(2/widthOfViewVolume);
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = (T)(2/heightOfViewVolume);
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = (T)(1/(zNear-zFar));
		M[11] = 0;

		M[12] = 0;
		M[13] = 0;
		M[14] = (T)(zNear/(zNear-zFar));
		M[15] = -1;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// Builds a right-handed perspective projection matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildProjectionMatrixPerspectiveRH(
			double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar)
	{
		M[0] = (T)(2*zNear/widthOfViewVolume);
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = (T)(2*zNear/heightOfViewVolume);
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = (T)(zFar/(zNear-zFar));
		M[11] = -1;

		M[12] = 0;
		M[13] = 0;
		M[14] = (T)(zNear*zFar/(zNear-zFar));
		M[15] = 0;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// Builds a left-handed perspective projection matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildProjectionMatrixPerspectiveLH(
			double widthOfViewVolume, double heightOfViewVolume, double zNear, double zFar)
	{
		M[0] = (T)(2*zNear/widthOfViewVolume);
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = (T)(2*zNear/heightOfViewVolume);
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;
		M[10] = (T)(zFar/(zFar-zNear));
		M[11] = 1;

		M[12] = 0;
		M[13] = 0;
		M[14] = (T)(zNear*zFar/(zNear-zFar));
		M[15] = 0;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// Builds a matrix that flattens geometry into a plane.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildShadowMatrix(const CGDM_Vector3DF& light, CGDM_Plane3DF plane, double point)
	{
	/*
		plane.m_cNormal.normalize();
		const double d = plane.m_cNormal.m_tDist.otProduct(light);

		M[ 0] = (T)(-plane.m_cNormal.m_tX * light.m_tX + d);
		M[ 1] = (T)(-plane.m_cNormal.m_tX * light.m_tY);
		M[ 2] = (T)(-plane.m_cNormal.m_tX * light.m_tZ);
		M[ 3] = (T)(-plane.m_cNormal.m_tX * point);

		M[ 4] = (T)(-plane.m_cNormal.m_tY * light.m_tX);
		M[ 5] = (T)(-plane.m_cNormal.m_tY * light.m_tY + d);
		M[ 6] = (T)(-plane.m_cNormal.m_tY * light.m_tZ);
		M[ 7] = (T)(-plane.m_cNormal.m_tY * point);

		M[ 8] = (T)(-plane.m_cNormal.m_tZ * light.m_tX);
		M[ 9] = (T)(-plane.m_cNormal.m_tZ * light.m_tY);
		M[10] = (T)(-plane.m_cNormal.m_tZ * light.m_tZ + d);
		M[11] = (T)(-plane.m_cNormal.m_tZ * point);

		M[12] = (T)(-plane.m_tDist * light.m_tX);
		M[13] = (T)(-plane.m_tDist * light.m_tY);
		M[14] = (T)(-plane.m_tDist * light.m_tZ);
		M[15] = (T)(-plane.m_tDist * point + d);
		definitelyIdentityMatrix=false;
	*/
		return *this;
	}

	// Builds a left-handed look-at matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildCameraLookAtMatrixLH(
				const CGDM_Vector3DF& position,
				const CGDM_Vector3DF& target,
				const CGDM_Vector3DF& upVector)
	{
	/*
		CGDM_Vector3DF zaxis = target - position;
		zaxis.normalize();

		CGDM_Vector3DF xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		CGDM_Vector3DF yaxis = zaxis.crossProduct(xaxis);

		M[0] = (T)xaxis.m_tX;
		M[1] = (T)yaxis.m_tX;
		M[2] = (T)zaxis.m_tX;
		M[3] = 0;

		M[4] = (T)xaxis.m_tY;
		M[5] = (T)yaxis.m_tY;
		M[6] = (T)zaxis.m_tY;
		M[7] = 0;

		M[8] = (T)xaxis.m_tZ;
		M[9] = (T)yaxis.m_tZ;
		M[10] = (T)zaxis.m_tZ;
		M[11] = 0;

		M[12] = (T)-xaxis.m_tDist.otProduct(position);
		M[13] = (T)-yaxis.m_tDist.otProduct(position);
		M[14] = (T)-zaxis.m_tDist.otProduct(position);
		M[15] = 1;
		definitelyIdentityMatrix=false;
	*/
		return *this;
	}


	// Builds a right-handed look-at matrix.
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildCameraLookAtMatrixRH(
				const CGDM_Vector3DF& position,
				const CGDM_Vector3DF& target,
				const CGDM_Vector3DF& upVector)
	{
	/*
		CGDM_Vector3DF zaxis = position - target;
		zaxis.normalize();

		CGDM_Vector3DF xaxis = upVector.crossProduct(zaxis);
		xaxis.normalize();

		CGDM_Vector3DF yaxis = zaxis.crossProduct(xaxis);

		M[0] = (T)xaxis.m_tX;
		M[1] = (T)yaxis.m_tX;
		M[2] = (T)zaxis.m_tX;
		M[3] = 0;

		M[4] = (T)xaxis.m_tY;
		M[5] = (T)yaxis.m_tY;
		M[6] = (T)zaxis.m_tY;
		M[7] = 0;

		M[8] = (T)xaxis.m_tZ;
		M[9] = (T)yaxis.m_tZ;
		M[10] = (T)zaxis.m_tZ;
		M[11] = 0;

		M[12] = (T)-xaxis.m_tDist.otProduct(position);
		M[13] = (T)-yaxis.m_tDist.otProduct(position);
		M[14] = (T)-zaxis.m_tDist.otProduct(position);
		M[15] = 1;
		definitelyIdentityMatrix=false;
	*/
		return *this;
	}


	// creates a new matrix as interpolated matrix from this and the passed one.
	template <class T>
	inline TGDM_Matrix4<T> TGDM_Matrix4<T>::interpolate(const TGDM_Matrix4<T>& b, double time) const
	{
		TGDM_Matrix4<T> mat ( EM4CONST_NOTHING );

		for (GDEM_UINT i=0; i < 16; i += 4)
		{
			mat.M[i+0] = (T)(M[i+0] + ( b.M[i+0] - M[i+0] ) * time);
			mat.M[i+1] = (T)(M[i+1] + ( b.M[i+1] - M[i+1] ) * time);
			mat.M[i+2] = (T)(M[i+2] + ( b.M[i+2] - M[i+2] ) * time);
			mat.M[i+3] = (T)(M[i+3] + ( b.M[i+3] - M[i+3] ) * time);
		}
		return mat;
	}


	// returns transposed matrix
	template <class T>
	inline TGDM_Matrix4<T> TGDM_Matrix4<T>::getTransposed() const
	{
		TGDM_Matrix4<T> t ( EM4CONST_NOTHING );
		getTransposed ( t );
		return t;
	}


	// returns transposed matrix
	template <class T>
	inline void TGDM_Matrix4<T>::getTransposed( TGDM_Matrix4<T>& o ) const
	{
		o[ 0] = M[ 0];
		o[ 1] = M[ 4];
		o[ 2] = M[ 8];
		o[ 3] = M[12];

		o[ 4] = M[ 1];
		o[ 5] = M[ 5];
		o[ 6] = M[ 9];
		o[ 7] = M[13];

		o[ 8] = M[ 2];
		o[ 9] = M[ 6];
		o[10] = M[10];
		o[11] = M[14];

		o[12] = M[ 3];
		o[13] = M[ 7];
		o[14] = M[11];
		o[15] = M[15];
		o.definitelyIdentityMatrix=definitelyIdentityMatrix;
	}


	// used to scale <-1,-1><1,1> to viewport
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildNDCToDCMatrix( const CGDM_Rect<GDEM_INT>& viewport, double zScale)
	{
		const double scaleX = (viewport.getWidth() - 0.75f ) / 2.0f;
		const double scaleY = -(viewport.getHeight() - 0.75f ) / 2.0f;

		const double dx = -0.5f + ( (viewport.m_cMinPt.m_tX + viewport.m_cMaxPt.m_tX ) / 2.0f );
		const double dy = -0.5f + ( (viewport.m_cMinPt.m_tY + viewport.m_cMaxPt.m_tY ) / 2.0f );

		makeIdentity();
		M[12] = (T)dx;
		M[13] = (T)dy;
		return setScale(CGDM_Vector3D<T>((T)scaleX, (T)scaleY, (T)zScale));
	}

	/*!
		Generate texture coordinates as linear functions so that:
			u = Ux*x + Uy*y + Uz*z + Uw
			v = Vx*x + Vy*y + Vz*z + Vw
		The matrix M for this case is:
			Ux  Vx  0  0
			Uy  Vy  0  0
			Uz  Vz  0  0
			Uw  Vw  0  0
	*/


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::buildTextureTransform( double rotateRad,
			const CGDM_Vector2DF &rotatecenter,
			const CGDM_Vector2DF &translate,
			const CGDM_Vector2DF &scale)
	{
		const double c = cosf(rotateRad);
		const double s = sinf(rotateRad);

		M[0] = (T)(c * scale.m_tX);
		M[1] = (T)(s * scale.m_tY);
		M[2] = 0;
		M[3] = 0;

		M[4] = (T)(-s * scale.m_tX);
		M[5] = (T)(c * scale.m_tY);
		M[6] = 0;
		M[7] = 0;

		M[8] = (T)(c * scale.m_tX * rotatecenter.m_tX + -s * rotatecenter.m_tY + translate.m_tX);
		M[9] = (T)(s * scale.m_tY * rotatecenter.m_tX +  c * rotatecenter.m_tY + translate.m_tY);
		M[10] = 1;
		M[11] = 0;

		M[12] = 0;
		M[13] = 0;
		M[14] = 0;
		M[15] = 1;
		definitelyIdentityMatrix=false;
		return *this;
	}


	// rotate about z axis, center ( 0.5, 0.5 )
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setTextureRotationCenter( double rotateRad )
	{
		const double c = cosf(rotateRad);
		const double s = sinf(rotateRad);
		M[0] = (T)c;
		M[1] = (T)s;

		M[4] = (T)-s;
		M[5] = (T)c;

		M[8] = (T)(0.5f * ( s - c) + 0.5f);
		M[9] = (T)(-0.5f * ( s + c) + 0.5f);
		definitelyIdentityMatrix = definitelyIdentityMatrix && (rotateRad==0.0f);
		return *this;
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setTextureTranslate ( double x, double y )
	{
		M[8] = (T)x;
		M[9] = (T)y;
		definitelyIdentityMatrix = definitelyIdentityMatrix && (x==0.0f) && (y==0.0f);
		return *this;
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setTextureTranslateTransposed ( double x, double y )
	{
		M[2] = (T)x;
		M[6] = (T)y;
		definitelyIdentityMatrix = definitelyIdentityMatrix && (x==0.0f) && (y==0.0f) ;
		return *this;
	}

	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setTextureScale ( double sx, double sy )
	{
		M[0] = (T)sx;
		M[5] = (T)sy;
		definitelyIdentityMatrix = definitelyIdentityMatrix && (sx==1.0f) && (sy==1.0f);
		return *this;
	}


	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setTextureScaleCenter( double sx, double sy )
	{
		M[0] = (T)sx;
		M[5] = (T)sy;
		M[8] = (T)(0.5f - 0.5f * sx);
		M[9] = (T)(0.5f - 0.5f * sy);
		definitelyIdentityMatrix = definitelyIdentityMatrix && (sx==1.0f) && (sy==1.0f);
		return *this;
	}


	// sets all matrix data members at once
	template <class T>
	inline TGDM_Matrix4<T>& TGDM_Matrix4<T>::setM(const T* data)
	{
		memcpy(M,data, 16*sizeof(T));

		definitelyIdentityMatrix = false;
		return *this;
	}


	// sets if the matrix is definitely identity matrix
	template <class T>
	inline void TGDM_Matrix4<T>::setDefinitelyIdentityMatrix( bool isDefinitelyIdentityMatrix)
	{
		definitelyIdentityMatrix = isDefinitelyIdentityMatrix;
	}


	// gets if the matrix is definitely identity matrix
	template <class T>
	inline bool TGDM_Matrix4<T>::getDefinitelyIdentityMatrix() const
	{
		return definitelyIdentityMatrix;
	}


	// Multiply by scalar.
	template <class T>
	inline TGDM_Matrix4<T> operator*(const T scalar, const TGDM_Matrix4<T>& mat)
	{
		return mat*scalar;
	}


//! Typedef for double matrix
typedef TGDM_Matrix4<double> CGDM_Matrix4;
//! global const identity matrix
extern const CGDM_Matrix4 IdentityMatrix;


#endif // __GDEM_MATRIX4_H_INCLUDED__
