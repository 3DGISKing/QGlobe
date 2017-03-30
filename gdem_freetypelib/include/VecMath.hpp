/*
 *
 * Copyright (C) 2003 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Template vector and matrix library.
// Use OpenGL compatible ordering ie. you can pass a matrix or vector to
// openGL functions without changes in the ordering

#ifndef _VECMATH_H_
#define _VECMATH_H_

#include <cmath>
#include <QString>
 
template<class T> class Vector2;
template<class T> class Vector3;
template<class T> class Vector4;
template<class T> class Matrix4;

typedef Vector2<double>	Vec2d;
typedef Vector2<float>	Vec2f;
typedef Vector2<int>	Vec2i;

//! @typedef Vec3d
//! A 3d vector of doubles compatible with openGL.
typedef Vector3<double>	Vec3d;

//! @typedef Vec3f
//! A 3d vector of floats compatible with openGL.
typedef Vector3<float>	Vec3f;

//! @typedef Vec4d
//! A 4d vector of doubles compatible with openGL.
typedef Vector4<double>	Vec4d;

//! @typedef Vec4f
//! A 4d vector of floats compatible with openGL.
typedef Vector4<float>	Vec4f;

//! @typedef Vec4i
//! A 4d vector of ints compatible with openGL.
typedef Vector4<int>	Vec4i;

//! @typedef Mat4d
//! A 4x4 matrix of doubles compatible with openGL.
typedef Matrix4<double>	Mat4d;

//! @typedef Mat4f
//! A 4x4 matrix of floats compatible with openGL.
typedef Matrix4<float>	Mat4f;


//! @class Vector2
//! A templatized 2d vector compatible with openGL.
//! Use Vec2d or Vec2f typdef for vectors of double and float respectively.
template<class T> class Vector2
{
public:
    inline Vector2();
    inline Vector2(const Vector2<T>&);
    inline Vector2(T, T);

	inline Vector2& operator=(const Vector2<T>&);
	inline Vector2& operator=(const T*);
    inline void set(T, T);

	inline bool operator==(const Vector2<T>&) const;
	inline bool operator!=(const Vector2<T>&) const;

	inline const T& operator[](int x) const;
    inline T& operator[](int);
	inline operator const T*() const;
	inline operator T*();

    inline Vector2& operator+=(const Vector2<T>&);
    inline Vector2& operator-=(const Vector2<T>&);
    inline Vector2& operator*=(T);
	inline Vector2& operator/=(T);

    inline Vector2 operator-(const Vector2<T>&) const;
    inline Vector2 operator+(const Vector2<T>&) const;

	inline Vector2 operator-() const;
	inline Vector2 operator+() const;

	inline Vector2 operator*(T) const;
	inline Vector2 operator/(T) const;


    inline T dot(const Vector2<T>&) const;

    inline T length() const;
    inline T lengthSquared() const;
    inline void normalize();

    T v[2];
};

//! @class Vector3
//! A templatized 3d vector compatible with openGL.
//! Use Vec3d or Vec3f typdef for vectors of double and float respectively.
template<class T> class Vector3
{
public:
    inline Vector3();
    inline Vector3(const Vector3&);
	template <class T2> inline Vector3(const Vector3<T2>&);
    inline Vector3(T, T, T);
	inline Vector3(T);
	
	inline Vector3& operator=(const Vector3&);
	inline Vector3& operator=(const T*);
	template <class T2> inline Vector3& operator=(const Vector3<T2>&);
    inline void set(T, T, T);

	inline bool operator==(const Vector3<T>&) const;
	inline bool operator!=(const Vector3<T>&) const;

    inline T& operator[](int);
    inline const T& operator[](int) const;
	inline operator const T*() const;
	inline operator T*();

    inline Vector3& operator+=(const Vector3<T>&);
    inline Vector3& operator-=(const Vector3<T>&);
    inline Vector3& operator*=(T);
	inline Vector3& operator/=(T);

    inline Vector3 operator-(const Vector3<T>&) const;
    inline Vector3 operator+(const Vector3<T>&) const;

	inline Vector3 operator-() const;
	inline Vector3 operator+() const;

	inline Vector3 operator*(T) const;
	inline Vector3 operator/(T) const;


    inline T dot(const Vector3<T>&) const;
	inline Vector3 operator^(const Vector3<T>&) const;

	// Return latitude in rad
	inline T latitude() const;
	// Return longitude in rad
	inline T longitude() const;
	
	// Distance in radian between two
	inline T angle(const Vector3<T>&) const;

    inline T length() const;
    inline T lengthSquared() const;
    inline void normalize();

	inline void transfo4d(const Mat4d&);
	inline void transfo4d(const Mat4f&);
	T v[3];		// The 3 values
	
	QString toString() const {return QString("[%1, %2, %3]").arg(v[0]).arg(v[1]).arg(v[2]);}
};


//! @class Vector4
//! A templatized 4d vector compatible with openGL.
//! Use Vec4d or Vec4f typdef for vectors of double and float respectively.
template<class T> class Vector4
{
public:
    inline Vector4();
    inline Vector4(const Vector4<T>&);
	inline Vector4(const Vector3<T>&);
	inline Vector4(T, T, T);
	inline Vector4(T, T, T, T);

	inline Vector4& operator=(const Vector4<T>&);
	inline Vector4& operator=(const Vector3<T>&);
	inline Vector4& operator=(const T*);
    inline void set(T, T, T, T);

	inline bool operator==(const Vector4<T>&) const;
	inline bool operator!=(const Vector4<T>&) const;

    inline T& operator[](int);
    inline const T& operator[](int) const;
	inline operator T*();
	inline operator const T*() const;

    inline Vector4& operator+=(const Vector4<T>&);
    inline Vector4& operator-=(const Vector4<T>&);
    inline Vector4& operator*=(T);
	inline Vector4& operator/=(T);

    inline Vector4 operator-(const Vector4<T>&) const;
    inline Vector4 operator+(const Vector4<T>&) const;

	inline Vector4 operator-() const;
	inline Vector4 operator+() const;

	inline Vector4 operator*(T) const;
	inline Vector4 operator/(T) const;


    inline T dot(const Vector4<T>&) const;

    inline T length() const;
    inline T lengthSquared() const;
    inline void normalize();

	inline void transfo4d(const Mat4d&);

	T v[4];		// The 4 values
};

//! @class Matrix4
//! A templatized column-major 4x4 matrix compatible with openGL.
//! Use Mat4d or Mat4f typdef for matrices of doubles and floats respectively.
template<class T> class Matrix4
{
 public:
    Matrix4();
    Matrix4(const Matrix4<T>& m);
    Matrix4(T,T,T,T,T,T,T,T,T,T,T,T,T,T,T,T);
    Matrix4(const T*);
    Matrix4(const Vector4<T>&, const Vector4<T>&,
            const Vector4<T>&, const Vector4<T>&);

	inline Matrix4& operator=(const Matrix4<T>&);
	inline Matrix4& operator=(const T*);
	inline void set(T,T,T,T,T,T,T,T,T,T,T,T,T,T,T,T);

    inline T* operator[](int);
	inline operator T*();
	inline operator const T*() const;

    inline Matrix4 operator-(const Matrix4<T>&) const;
    inline Matrix4 operator+(const Matrix4<T>&) const;
    inline Matrix4 operator*(const Matrix4<T>&) const;

    inline Vector3<T> operator*(const Vector3<T>&) const;
    inline Vector3<T> multiplyWithoutTranslation(const Vector3<T>& a) const;
	inline Vector4<T> operator*(const Vector4<T>&) const;
	
	inline void transfo(Vector3<T>&) const;
	
    static Matrix4<T> identity();
    static Matrix4<T> translation(const Vector3<T>&);

    //    static Matrix4<T> rotation(const Vector3<T>&);
    static Matrix4<T> rotation(const Vector3<T>&, T);
    static Matrix4<T> xrotation(T);
    static Matrix4<T> yrotation(T);
    static Matrix4<T> zrotation(T);
    static Matrix4<T> scaling(const Vector3<T>&);
    static Matrix4<T> scaling(T);

    Matrix4<T> transpose() const;
    Matrix4<T> inverse() const;

	inline void print(void) const;

    T r[16];
};



////////////////////////// Vector2 class methods ///////////////////////////////

template<class T> Vector2<T>::Vector2() {}

template<class T> Vector2<T>::Vector2(const Vector2<T>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1];
}

template<class T> Vector2<T>::Vector2(T x, T y)
{
	v[0]=x; v[1]=y;
}

template<class T> Vector2<T>& Vector2<T>::operator=(const Vector2<T>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1];
    return *this;
}

template<class T> Vector2<T>& Vector2<T>::operator=(const T* a)
{
	v[0]=a[0]; v[1]=a[1];
    return *this;
}

template<class T> void Vector2<T>::set(T x, T y)
{
	v[0]=x; v[1]=y;
}


template<class T> bool Vector2<T>::operator==(const Vector2<T>& a) const
{
	return (v[0] == a.v[0] && v[1] == a.v[1]);
}

template<class T> bool Vector2<T>::operator!=(const Vector2<T>& a) const
{
	return (v[0] != a.v[0] || v[1] != a.v[1]);
}

template<class T> const T& Vector2<T>::operator[](int x) const
{
	return v[x];
}

template<class T> T& Vector2<T>::operator[](int x)
{
	return v[x];
}

template<class T> Vector2<T>::operator const T*() const
{
	return v;
}

template<class T> Vector2<T>::operator T*()
{
	return v;
}


template<class T> Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& a)
{
    v[0] += a.v[0]; v[1] += a.v[1];
    return *this;
}

template<class T> Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& a)
{
    v[0] -= a.v[0]; v[1] -= a.v[1];
    return *this;
}

template<class T> Vector2<T>& Vector2<T>::operator*=(T s)
{
    v[0] *= s; v[1] *= s;
    return *this;
}

template<class T> Vector2<T> Vector2<T>::operator-() const
{
    return Vector2<T>(-v[0], -v[1]);
}

template<class T> Vector2<T> Vector2<T>::operator+() const
{
    return *this;
}

template<class T> Vector2<T> Vector2<T>::operator+(const Vector2<T>& b) const
{
    return Vector2<T>(v[0] + b.v[0], v[1] + b.v[1]);
}

template<class T> Vector2<T> Vector2<T>::operator-(const Vector2<T>& b) const
{
    return Vector2<T>(v[0] - b.v[0], v[1] - b.v[1]);
}

template<class T> Vector2<T> Vector2<T>::operator*(T s) const
{
    return Vector2<T>(s * v[0], s * v[1]);
}

template<class T> Vector2<T> Vector2<T>::operator/(T s) const
{
    return Vector2<T>(v[0]/s, v[1]/s);
}


template<class T> T Vector2<T>::dot(const Vector2<T>& b) const
{
    return v[0] * b.v[0] + v[1] * b.v[1];
}


template<class T> T Vector2<T>::length() const
{
    return (T) std::sqrt(v[0] * v[0] + v[1] * v[1]);
}

template<class T> T Vector2<T>::lengthSquared() const
{
    return v[0] * v[0] + v[1] * v[1];
}

template<class T> void Vector2<T>::normalize()
{
    T s = (T) 1 / std::sqrt(v[0] * v[0] + v[1] * v[1]);
    v[0] *= s;
    v[1] *= s;
}

// template<class T> 
// std::ostream& operator<<(std::ostream &o,const Vector2<T> &v) {
//   return o << '[' << v[0] << ',' << v[1] << ']';
// }

////////////////////////// Vector3 class methods ///////////////////////////////

template<class T> Vector3<T>::Vector3() {}

template<class T> Vector3<T>::Vector3(const Vector3& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2];
}

template<class T> template<class T2> Vector3<T>::Vector3(const Vector3<T2>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2];
}

template<class T> Vector3<T>::Vector3(T x)
{
	v[0]=x; v[1]=x; v[2]=x;
}

template<class T> Vector3<T>::Vector3(T x, T y, T z)
{
	v[0]=x; v[1]=y; v[2]=z;
}

template<class T> Vector3<T>& Vector3<T>::operator=(const Vector3& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2];
    return *this;
}

template<class T> template <class T2> Vector3<T>& Vector3<T>::operator=(const Vector3<T2>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2];
    return *this;
}

template<class T> Vector3<T>& Vector3<T>::operator=(const T* a)
{
	v[0]=a[0]; v[1]=a[1]; v[2]=a[2];
    return *this;
}

template<class T> void Vector3<T>::set(T x, T y, T z)
{
	v[0]=x; v[1]=y; v[2]=z;
}


template<class T> bool Vector3<T>::operator==(const Vector3<T>& a) const
{
	return (v[0] == a.v[0] && v[1] == a.v[1] && v[2] == a.v[2]);
}

template<class T> bool Vector3<T>::operator!=(const Vector3<T>& a) const
{
	return (v[0] != a.v[0] || v[1] != a.v[1] || v[2] != a.v[2]);
}


template<class T> T& Vector3<T>::operator[](int x)
{
	return v[x];
}

template<class T> const T& Vector3<T>::operator[](int x) const
{
	return v[x];
}

template<class T> Vector3<T>::operator const T*() const
{
	return v;
}

template<class T> Vector3<T>::operator T*()
{
	return v;
}

template<class T> Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& a)
{
    v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2];
    return *this;
}

template<class T> Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& a)
{
    v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2];
    return *this;
}

template<class T> Vector3<T>& Vector3<T>::operator*=(T s)
{
    v[0] *= s; v[1] *= s; v[2] *= s;
    return *this;
}

template<class T> Vector3<T>& Vector3<T>::operator/=(T s)
{
	v[0] /= s; v[1] /= s; v[2] /= s;
	return *this;
}

template<class T> Vector3<T> Vector3<T>::operator-() const
{
    return Vector3<T>(-v[0], -v[1], -v[2]);
}

template<class T> Vector3<T> Vector3<T>::operator+() const
{
    return *this;
}

template<class T> Vector3<T> Vector3<T>::operator+(const Vector3<T>& b) const
{
    return Vector3<T>(v[0] + b.v[0], v[1] + b.v[1], v[2] + b.v[2]);
}

template<class T> Vector3<T> Vector3<T>::operator-(const Vector3<T>& b) const
{
    return Vector3<T>(v[0] - b.v[0], v[1] - b.v[1], v[2] - b.v[2]);
}

template<class T> Vector3<T> Vector3<T>::operator*(T s) const
{
    return Vector3<T>(s * v[0], s * v[1], s * v[2]);
}

template<class T> Vector3<T> Vector3<T>::operator/(T s) const
{
    return Vector3<T>(v[0]/s, v[1]/s, v[2]/s);
}


template<class T> T Vector3<T>::dot(const Vector3<T>& b) const
{
    return v[0] * b.v[0] + v[1] * b.v[1] + v[2] * b.v[2];
}


// cross product
template<class T> Vector3<T> Vector3<T>::operator^(const Vector3<T>& b) const
{
    return Vector3<T>(v[1] * b.v[2] - v[2] * b.v[1],
                      v[2] * b.v[0] - v[0] * b.v[2],
                      v[0] * b.v[1] - v[1] * b.v[0]);
}

// Angle in radian between two normalized vectors
template<class T> T Vector3<T>::angle(const Vector3<T>& b) const
{
	return std::acos(dot(b)/sqrt(lengthSquared()*b.lengthSquared()));
}

template<class T> T Vector3<T>::length() const
{
    return (T) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

template<class T> T Vector3<T>::lengthSquared() const
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

template<class T> void Vector3<T>::normalize()
{
    T s = (T) (1. / std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]));
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
}

template<class T> void Vector3<T>::transfo4d(const Mat4d& m)
{
	(*this)=m*(*this);
}

template<class T> void Vector3<T>::transfo4d(const Mat4f& m)
{
	(*this)=m*(*this);
}

// Return latitude in rad
template<class T> T Vector3<T>::latitude() const
{
	return std::asin(v[2]/length());
}

// Return longitude in rad
template<class T> T Vector3<T>::longitude() const
{
	return std::atan2(v[1],v[0]);
}

// template<class T> 
// std::ostream& operator<<(std::ostream &o,const Vector3<T> &v) {
//   return o << '[' << v[0] << ',' << v[1] << ',' << v[2] << ']';
// }

// template<class T> 
// std::istream& operator>> (std::istream& is, Vector3<T> &v) {
// 	while(is.get()!='[' && !is.eof()) {;}
// 	assert(!is.eof() && "Vector must start with a '['");
// 	is >> v[0];
// 	is.ignore(256, ',');
// 	is >> v[1];
// 	is.ignore(256, ',');
// 	is >> v[2];
// 	while(is.get()!=']' && !is.eof()) {;}
// 	assert(!is.eof() && "Vector must be terminated by a ']'");
// 	return is;
// }
		
////////////////////////// Vector4 class methods ///////////////////////////////

template<class T> Vector4<T>::Vector4() {}

template<class T> Vector4<T>::Vector4(const Vector4<T>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2]; v[3]=a.v[3];
}

template<class T> Vector4<T>::Vector4(const Vector3<T>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2]; v[3]=1;
}

template<class T> Vector4<T>::Vector4(T x, T y, T z)
{
	v[0]=x; v[1]=y; v[2]=z; v[3]=1;
}

template<class T> Vector4<T>::Vector4(T x, T y, T z, T a = 1)
{
	v[0]=x; v[1]=y; v[2]=z; v[3]=a;
}

template<class T> Vector4<T>& Vector4<T>::operator=(const Vector4<T>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2]; v[3]=a.v[3];
    return *this;
}

template<class T> Vector4<T>& Vector4<T>::operator=(const Vector3<T>& a)
{
	v[0]=a.v[0]; v[1]=a.v[1]; v[2]=a.v[2]; v[3]=1;
    return *this;
}

template<class T> Vector4<T>& Vector4<T>::operator=(const T* a)
{
	v[0]=a[0]; v[1]=a[1]; v[2]=a[2]; v[3]=a[3];
    return *this;
}

template<class T> void Vector4<T>::set(T x, T y, T z, T a)
{
	v[0]=x; v[1]=y; v[2]=z; v[3]=a;
}

template<class T> bool Vector4<T>::operator==(const Vector4<T>& a) const
{
	return (v[0] == a.v[0] && v[1] == a.v[1] && v[2] == a.v[2] && v[3] == a.v[3]);
}

template<class T> bool Vector4<T>::operator!=(const Vector4<T>& a) const
{
	return (v[0] != a.v[0] || v[1] != a.v[1] || v[2] != a.v[2] || v[3] != a.v[3]);
}

template<class T> T& Vector4<T>::operator[](int x)
{
	return v[x];
}

template<class T> const T& Vector4<T>::operator[](int x) const
{
	return v[x];
}

template<class T> Vector4<T>::operator T*()
{
	return v;
}

template<class T> Vector4<T>::operator const T*() const
{
	return v;
}

template<class T> Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& a)
{
    v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2]; v[3] += a.v[3];
    return *this;
}

template<class T> Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& a)
{
    v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2]; v[3] -= a/v[3];
    return *this;
}

template<class T> Vector4<T>& Vector4<T>::operator*=(T s)
{
    v[0] *= s; v[1] *= s; v[2] *= s; v[3] *= s;
    return *this;
}

template<class T> Vector4<T> Vector4<T>::operator-() const
{
    return Vector4<T>(-v[0], -v[1], -v[2], -v[3]);
}

template<class T> Vector4<T> Vector4<T>::operator+() const
{
    return *this;
}

template<class T> Vector4<T> Vector4<T>::operator+(const Vector4<T>& b) const
{
    return Vector4<T>(v[0] + b.v[0], v[1] + b.v[1], v[2] + b.v[2], v[3] + b.v[3]);
}

template<class T> Vector4<T> Vector4<T>::operator-(const Vector4<T>& b) const
{
    return Vector4<T>(v[0] - b.v[0], v[1] - b.v[1], v[2] - b.v[2], v[3] - b.v[3]);
}

template<class T> Vector4<T> Vector4<T>::operator*(T s) const
{
    return Vector4<T>(s * v[0], s * v[1], s * v[2], s * v[3]);
}

template<class T> Vector4<T> Vector4<T>::operator/(T s) const
{
    return Vector4<T>(v[0]/s, v[1]/s, v[2]/s, v[3]/s);
}

template<class T> T Vector4<T>::dot(const Vector4<T>& b) const
{
    return v[0] * b.v[0] + v[1] * b.v[1] + v[2] * b.v[2] + v[3] * b.v[3];
}

template<class T> T Vector4<T>::length() const
{
    return (T) sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

template<class T> T Vector4<T>::lengthSquared() const
{
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}

template<class T> void Vector4<T>::normalize()
{
    T s = (T) (1. / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]));
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
	v[3] *= s;
}

template<class T> void Vector4<T>::transfo4d(const Mat4d& m)
{
	(*this)=m*(*this);
}
/*
template<class T> 
std::ostream& operator<<(std::ostream &o,const Vector4<T> &v) {
  return o << '[' << v[0] << ',' << v[1] << ',' << v[2] << ',' << v[3] << ']';
}*/


////////////////////////// Matrix4 class methods ///////////////////////////////

template<class T> Matrix4<T>::Matrix4() {}

template<class T> Matrix4<T>::Matrix4(const Matrix4<T>& m)
{
	memcpy(r,m.r,sizeof(T)*16);
}

template<class T> Matrix4<T>::Matrix4(const T* m)
{
	memcpy(r,m,sizeof(T)*16);
}

template<class T> Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& m)
{
	memcpy(r,m.r,sizeof(T)*16);
	return (*this);
}

template<class T> Matrix4<T>::Matrix4(const Vector4<T>& v0,
                                      const Vector4<T>& v1,
                                      const Vector4<T>& v2,
                                      const Vector4<T>& v3)
{
    r[0] = v0.v[0];
    r[1] = v0.v[1];
    r[2] = v0.v[2];
    r[3] = v0.v[3];
    r[4] = v1.v[0];
    r[5] = v1.v[1];
    r[6] = v1.v[2];
    r[7] = v1.v[3];
    r[8] = v2.v[0];
    r[9] = v2.v[1];
    r[10] = v2.v[2];
    r[11] = v2.v[3];
    r[12] = v3.v[0];
    r[13] = v3.v[1];
    r[14] = v3.v[2];
    r[15] = v3.v[3];
}


template<class T> Matrix4<T>::Matrix4(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p)
{
	r[0]=a; r[1]=b; r[2]=c; r[3]=d; r[4]=e; r[5]=f; r[6]=g; r[7]=h;
	r[8]=i; r[9]=j; r[10]=k; r[11]=l; r[12]=m; r[13]=n; r[14]=o; r[15]=p;
}

template<class T> void Matrix4<T>::set(T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T m, T n, T o, T p)
{
	r[0]=a; r[1]=b; r[2]=c; r[3]=d; r[4]=e; r[5]=f; r[6]=g; r[7]=h;
	r[8]=i; r[9]=j; r[10]=k; r[11]=l; r[12]=m; r[13]=n; r[14]=o; r[15]=p;
}

template<class T> T* Matrix4<T>::operator[](int n)
{
    return &(r[n*4]);
}

template<class T> Matrix4<T>::operator T*()
{
	return r;
}

template<class T> Matrix4<T>::operator const T*() const
{
	return r;
}

template<class T> Matrix4<T> Matrix4<T>::identity()
{
    return Matrix4<T>(	1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1  );
}


template<class T> Matrix4<T> Matrix4<T>::translation(const Vector3<T>& a)
{
    return Matrix4<T>(	1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						a.v[0], a.v[1], a.v[2], 1);
}

template<class T> Matrix4<T> Matrix4<T>::rotation(const Vector3<T>& a,
                                                  T angle)
{
	Vec3d axis(a);
	axis.normalize();
//    T c = (T) cos(angle);
//    T s = (T) sin(angle);
//    T t = 1 - c;
//
//    return Matrix4<T>(Vector4<T>(t * axis.v[0] * axis.v[0] + c,
//                                 t * axis.v[0] * axis.v[1] - s * axis.v[2],
//                                 t * axis.v[0] * axis.v[2] + s * axis.v[1],
//                                 0),
//                      Vector4<T>(t * axis.v[0] * axis.v[1] + s * axis.v[2],
//                                 t * axis.v[1] * axis.v[1] + c,
//                                 t * axis.v[1] * axis.v[2] - s * axis.v[0],
//                                 0),
//                      Vector4<T>(t * axis.v[0] * axis.v[2] - s * axis.v[1],
//                                 t * axis.v[1] * axis.v[2] + s * axis.v[0],
//                                 t * axis.v[2] * axis.v[2] + c,
//                                 0),
//                      Vector4<T>(0, 0, 0, 1));
                      
	T sin_a = std::sin( angle / 2 );
	T cos_a = std::cos( angle / 2 );
	T X    = axis[0] * sin_a;
	T Y    = axis[1] * sin_a;
	T Z    = axis[2] * sin_a;
	T W    = cos_a;
	
	T xx      = X * X;
	T xy      = X * Y;
	T xz      = X * Z;
	T xw      = X * W;
	
	T yy      = Y * Y;
	T yz      = Y * Z;
	T yw      = Y * W;
	
	T zz      = Z * Z;
	T zw      = Z * W;

	return Matrix4<T>(
	1. - 2. * ( yy + zz ), 2. * ( xy + zw ),      2. * ( xz - yw ),      0.,
	2. * ( xy - zw ),      1. - 2. * ( xx + zz ), 2. * ( yz + xw ),      0., 
	2. * ( xz + yw ),      2. * ( yz - xw ),      1. - 2. * ( xx + yy ), 0., 
	0.,                    0.,                    0.,                    1.);
}



/*
template<class T> Matrix4<T> Matrix4<T>::rotation(const Vector3<T>& a)
{
    T c = (T) cos(angle);
    T s = (T) sin(angle);
	T d = 1-c;
	 return Matrix4<T>(	a.v[0]*a.v[0]*d+c, a.v[1]*a.v[0]*d+a.v[2]*s, a.v[0]*a.v[2]*d-a.v[1]*s, 0,
						a.v[0]*a.v[1]*d-a.v[2]*s, a.v[1]*a.v[1]*d+c, a.v[1]*a.v[2]*d+a.v[0]*s, 0,
						a.v[0]*a.v[2]*d+a.v[1]*s, a.v[1]*a.v[2]*d-a.v[0]*s, a.v[2]*a.v[2]*d+c, 0,
						0,0,0,1	);
}
*/
template<class T> Matrix4<T> Matrix4<T>::xrotation(T angle)
{
    T c = (T) cos(angle);
    T s = (T) sin(angle);

    return Matrix4<T>(1, 0, 0, 0,
                      0, c, s, 0,
                      0,-s, c, 0,
                      0, 0, 0, 1 );
}


template<class T> Matrix4<T> Matrix4<T>::yrotation(T angle)
{
    T c = (T) cos(angle);
    T s = (T) sin(angle);

    return Matrix4<T>( c, 0,-s, 0,
                       0, 1, 0, 0,
                       s, 0, c, 0,
                       0, 0, 0, 1 );
}


template<class T> Matrix4<T> Matrix4<T>::zrotation(T angle)
{
    T c = (T) cos(angle);
    T s = (T) sin(angle);

    return Matrix4<T>(c, s, 0, 0,
                     -s, c, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1 );
}


template<class T> Matrix4<T> Matrix4<T>::scaling(const Vector3<T>& s)
{
    return Matrix4<T>(s[0], 0  , 0  , 0,
                      0   ,s[1], 0  , 0,
                      0   , 0  ,s[2], 0,
                      0   , 0  , 0  , 1);
}


template<class T> Matrix4<T> Matrix4<T>::scaling(T scale)
{
    return scaling(Vector3<T>(scale, scale, scale));
}

// multiply column vector by a 4x4 matrix in homogeneous coordinate (use a[3]=1)
template<class T> Vector3<T> Matrix4<T>::operator*(const Vector3<T>& a) const
{
    return Vector3<T>(	r[0]*a.v[0] + r[4]*a.v[1] +  r[8]*a.v[2] + r[12],
						r[1]*a.v[0] + r[5]*a.v[1] +  r[9]*a.v[2] + r[13],
						r[2]*a.v[0] + r[6]*a.v[1] + r[10]*a.v[2] + r[14] );
}

template<class T> Vector3<T> Matrix4<T>::multiplyWithoutTranslation(const Vector3<T>& a) const
{
    return Vector3<T>(	r[0]*a.v[0] + r[4]*a.v[1] +  r[8]*a.v[2],
						r[1]*a.v[0] + r[5]*a.v[1] +  r[9]*a.v[2],
						r[2]*a.v[0] + r[6]*a.v[1] + r[10]*a.v[2] );
}

// multiply column vector by a 4x4 matrix in homogeneous coordinate (considere a[3]=1)
template<class T> Vector4<T> Matrix4<T>::operator*(const Vector4<T>& a) const
{
    return Vector4<T>(	r[0]*a.v[0] + r[4]*a.v[1] +  r[8]*a.v[2] + r[12]*a.v[3],
						r[1]*a.v[0] + r[5]*a.v[1] +  r[9]*a.v[2] + r[13]*a.v[3],
						r[2]*a.v[0] + r[6]*a.v[1] + r[10]*a.v[2] + r[14]*a.v[3] );
}

template<class T> void Matrix4<T>::transfo(Vector3<T>& a) const
{
	a.set(	r[0]*a.v[0] + r[4]*a.v[1] +  r[8]*a.v[2] + r[12],
			r[1]*a.v[0] + r[5]*a.v[1] +  r[9]*a.v[2] + r[13],
			r[2]*a.v[0] + r[6]*a.v[1] + r[10]*a.v[2] + r[14]);
}

template<class T> Matrix4<T> Matrix4<T>::transpose() const
{
    return Matrix4<T>(	r[0], r[4], r[8],  r[12],
						r[1], r[5], r[9],  r[13],
						r[2], r[6], r[10], r[14],
						r[3], r[7], r[11], r[15]);
}

template<class T> Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& a) const
{
#define MATMUL(R, C) (r[R] * a.r[C] + r[R+4] * a.r[C+1] + r[R+8] * a.r[C+2] + r[R+12] * a.r[C+3])
    return Matrix4<T>(	MATMUL(0,0), MATMUL(1,0), MATMUL(2,0), MATMUL(3,0),
						MATMUL(0,4), MATMUL(1,4), MATMUL(2,4), MATMUL(3,4),
						MATMUL(0,8), MATMUL(1,8), MATMUL(2,8), MATMUL(3,8),
						MATMUL(0,12), MATMUL(1,12), MATMUL(2,12), MATMUL(3,12) );
#undef MATMUL
}


template<class T> Matrix4<T> Matrix4<T>::operator+(const Matrix4<T>& a) const
{
    return Matrix4<T>(	r[0]+a.r[0], r[1]+a.r[1], r[2]+a.r[2], r[3]+a.r[3],
						r[4]+a.r[4], r[5]+a.r[5], r[6]+a.r[6], r[7]+a.r[7],
						r[8]+a.r[8], r[9]+a.r[9], r[10]+a.r[10], r[11]+a.r[11],
						r[12]+a.r[12], r[13]+a.r[13], r[14]+a.r[14], r[15]+a.r[15] );
}

template<class T> Matrix4<T> Matrix4<T>::operator-(const Matrix4<T>& a) const
{
    return Matrix4<T>(	r[0]-a.r[0], r[1]-a.r[1], r[2]-a.r[2], r[3]-a.r[3],
						r[4]-a.r[4], r[5]-a.r[5], r[6]-a.r[6], r[7]-a.r[7],
						r[8]-a.r[8], r[9]-a.r[9], r[10]-a.r[10], r[11]-a.r[11],
						r[12]-a.r[12], r[13]-a.r[13], r[14]-a.r[14], r[15]-a.r[15] );
}

/*
 * Code ripped from the GLU library
 * Compute inverse of 4x4 transformation matrix.
 * Code contributed by Jacques Leroy jle@star.be
 * Return zero matrix on failure (singular matrix)
 */
template<class T> Matrix4<T> Matrix4<T>::inverse() const
{
	const T * m = r;
	T out[16];

/* NB. OpenGL Matrices are COLUMN major. */
#define SWAP_ROWS(a, b) { T *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

   T wtmp[4][8];
   T m0, m1, m2, m3, s;
   T *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
      r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
      r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
      r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
      r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
      r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
      r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
      r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
      r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
      r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
      r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
      r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabs(r3[0]) > fabs(r2[0]))
      SWAP_ROWS(r3, r2);
   if (fabs(r2[0]) > fabs(r1[0]))
      SWAP_ROWS(r2, r1);
   if (fabs(r1[0]) > fabs(r0[0]))
      SWAP_ROWS(r1, r0);
   if (0.0 == r0[0])
      return Matrix4<T>();

   /* eliminate first variable     */
   m1 = r1[0] / r0[0];
   m2 = r2[0] / r0[0];
   m3 = r3[0] / r0[0];
   s = r0[1];
   r1[1] -= m1 * s;
   r2[1] -= m2 * s;
   r3[1] -= m3 * s;
   s = r0[2];
   r1[2] -= m1 * s;
   r2[2] -= m2 * s;
   r3[2] -= m3 * s;
   s = r0[3];
   r1[3] -= m1 * s;
   r2[3] -= m2 * s;
   r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) {
      r1[4] -= m1 * s;
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r0[5];
   if (s != 0.0) {
      r1[5] -= m1 * s;
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r0[6];
   if (s != 0.0) {
      r1[6] -= m1 * s;
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r0[7];
   if (s != 0.0) {
      r1[7] -= m1 * s;
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }

   /* choose pivot - or die */
   if (fabs(r3[1]) > fabs(r2[1]))
      SWAP_ROWS(r3, r2);
   if (fabs(r2[1]) > fabs(r1[1]))
      SWAP_ROWS(r2, r1);
   if (0.0 == r1[1])
      return Matrix4<T>();

   /* eliminate second variable */
   m2 = r2[1] / r1[1];
   m3 = r3[1] / r1[1];
   r2[2] -= m2 * r1[2];
   r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3];
   r3[3] -= m3 * r1[3];
   s = r1[4];
   if (0.0 != s) {
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r1[5];
   if (0.0 != s) {
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r1[6];
   if (0.0 != s) {
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r1[7];
   if (0.0 != s) {
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }

   /* choose pivot - or die */
   if (fabs(r3[2]) > fabs(r2[2]))
      SWAP_ROWS(r3, r2);
   if (0.0 == r2[2])
      return Matrix4<T>();

   /* eliminate third variable */
   m3 = r3[2] / r2[2];
   r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
      r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3])
      return Matrix4<T>();

   s = 1.0 / r3[3];		/* now back substitute row 3 */
   r3[4] *= s;
   r3[5] *= s;
   r3[6] *= s;
   r3[7] *= s;

   m2 = r2[3];			/* now back substitute row 2 */
   s = 1.0 / r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
      r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
      r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
      r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

   m1 = r1[2];			/* now back substitute row 1 */
   s = 1.0 / r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
      r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
      r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

   m0 = r0[1];			/* now back substitute row 0 */
   s = 1.0 / r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
      r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

   MAT(out, 0, 0) = r0[4];
   MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
   MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
   MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
   MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
   MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
   MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
   MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
   MAT(out, 3, 3) = r3[7];

   return Matrix4<T>(out);

#undef MAT
#undef SWAP_ROWS
}


template<class T> void Matrix4<T>::print(void) const
{
    printf("[%5.2lf %5.2lf %5.2lf %17.12le]\n"
           "[%5.2lf %5.2lf %5.2lf %17.12le]\n"
           "[%5.2lf %5.2lf %5.2lf %17.12le]\n"
           "[%5.2lf %5.2lf %5.2lf %17.12le]\n\n",
    r[0],r[4],r[8],r[12],
    r[1],r[5],r[9],r[13],
    r[2],r[6],r[10],r[14],
    r[3],r[7],r[11],r[15]);
}


template<class T> inline
T operator*(const Vector2<T>&a,const Vector2<T>&b) {
  return a.v[0] * b.v[0] + a.v[1] * b.v[1];
}

template<class T> inline
T operator*(const Vector3<T>&a,const Vector3<T>&b) {
  return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}

template<class T> inline
T operator*(const Vector4<T>&a,const Vector4<T>&b) {
  return a.v[0]*b.v[0] + a.v[1]*b.v[1] + a.v[2]*b.v[2] + a.v[3]*b.v[3];
}

template<class T> inline
Vector2<T> operator*(T s,const Vector2<T>&v) {
  return Vector2<T>(s*v[0],s*v[1]);
}

template<class T> inline
Vector3<T> operator*(T s,const Vector3<T>&v) {
  return Vector3<T>(s*v[0],s*v[1],s*v[2]);
}

template<class T> inline
Vector4<T> operator*(T s,const Vector4<T>&v) {
  return Vector4<T>(s*v[0],s*v[1],s*v[2],s*v[3]);
}


#endif // _VECMATH_H_
