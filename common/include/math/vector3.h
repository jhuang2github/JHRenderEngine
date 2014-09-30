//
//  Vector3T.h
//
//  Created by Jingshu Huang on 1/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _vector3t_h
#define _vector3t_h

#include "platformDef.h"
#include <math.h>


class Quaternion;

template <class T>
class Vector3T {
public:
	T x, y, z;

	inline Vector3T() { x = y = z = 0; }
	inline Vector3T(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
	inline Vector3T(T v[3]) : x(v[0]), y(v[1]), z(v[2]) {}
	inline Vector3T(int v[3]): x((T)v[0]), y((T)v[1]), z((T)v[2]) {}
	inline Vector3T(const T* const v) : x(v[0]), y(v[1]), z(v[2]) {}
	inline Vector3T(const Vector3T& v) : x(v.x), y(v.y), z(v.z) {}

	inline T operator [] (unsigned i) const {
		return *(&x+i);
	}
	
	inline T& operator [] (unsigned i) {
		return *(&x+i);
	}
	
	inline Vector3T& operator = (const Vector3T& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	
	inline bool operator == (const Vector3T& v) const {
		return (x == v.x && y == v.y && z == v.z);
	}
	
	inline bool operator != (const Vector3T& v) const {
		return (x != v.x || y != v.y || z != v.z);
	}
	
	// arithmetic operations
	inline Vector3T operator + (const Vector3T& v) const {
		return Vector3T(x + v.x, y + v.y, z + v.z);
	}
	
	inline Vector3T operator - (const Vector3T& v) const {
		return Vector3T(x - v.x, y - v.y, z - v.z);
	}
	
	inline Vector3T operator * (T f) const {
		return Vector3T(x * f, y * f, z * f);
	}
	
	inline Vector3T operator * (const Vector3T& v) const {
		return Vector3T(x * v.x, y * v.y, z * v.z);
	}
	
	inline Vector3T operator / (T f) const {
		f = 1.0f / f;		
		return Vector3T(x * f, y * f, z * f);
	}
	
	inline Vector3T operator - () const {
		return Vector3T(-x, -y, -z);
	}
	
	inline friend Vector3T operator * (T f, const Vector3T& v) {
		return Vector3T(f * v.x, f * v.y, f * v.z);
	}
	
	// arithmetic updates
	inline Vector3T& operator += (const Vector3T& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	inline Vector3T& operator -= (const Vector3T& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	inline Vector3T& operator *= (T f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	inline Vector3T& operator *= (const Vector3T& f) {
		x *= f.x;
		y *= f.y;
		z *= f.z;
		return *this;
	}

	inline Vector3T& operator /= (T f) {
		f = 1.0f / f;
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	
	inline T length() const {
		return (T)sqrt(x * x + y * y + z * z);
	}

	inline T squaredLength() const {
		return x * x + y * y + z * z;
	}

    inline bool isZeroLength(void) const {
        T sqlen = (x * x) + (y * y) + (z * z);
        return (sqlen < (1e-06 * 1e-06));
    }

	inline T dotProduct(const Vector3T& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

    // Calculates the absolute dot (scalar) product of this vector with another.
    // This function work similar dotProduct, except it use absolute value
    // of each component of the vector to computing.
    inline float absDotProduct(const Vector3T& vec) const {
        return fabs(x * vec.x) + fabs(y * vec.y) + fabs(z * vec.z);
    }

    inline T normalise() {
        T fLength = (T)sqrt(x * x + y * y + z * z);
        if (fLength > 1e-06f) {
            T fInvLength = 1.0f / fLength;
            x *= fInvLength;
            y *= fInvLength;
            z *= fInvLength;
        }
        return fLength;
    }

	inline Vector3T crossProduct(const Vector3T& v) const {
		return Vector3T(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	inline Vector3T midPoint(const Vector3T& v) const {
		return Vector3T((x + v.x) * 0.5f, (y + v.y) * 0.5f, (z + v.z) * 0.5f);
	}

	inline bool operator < (const Vector3T& v) const {
		return (x < v.x && y < v.y && z < v.z);
	}

	inline bool operator > (const Vector3T& v) const {
		return (x > v.x && y > v.y && z > v.z);
	}

	inline void makeFloor(const Vector3T& v) {
		if(v.x < x) x = v.x;
		if(v.y < y) y = v.y;
		if(v.z < z) z = v.z;
	}

	inline void makeCeil(const Vector3T& v) {
		if(v.x > x) x = v.x;
		if(v.y > y) y = v.y;
		if(v.z > z) z = v.z;
	}

	inline Vector3T perpendicular(void) {
		static T fSquareZero = 1e-06f * 1e-06f;
		Vector3T perp = this->crossProduct(Vector3T::UNIT_X);

		// Check length
		if(perp.squaredLength() < fSquareZero) {
            // This vector is the Y axis multiplied by a scalar, so we have to
            // use another axis.
			perp = this->crossProduct(Vector3T::UNIT_Y);
		}
		return perp;
	}

    inline friend std::ostream& operator<<(std::ostream& o, const Vector3T& v) {
        o << "Vector3T(" << v.x << ", " << v.y << ", " << v.z << ")";
        return o;
    }

    Quaternion getRotationTo(const Vector3T& dest,
                  const Vector3T& fallbackAxis = Vector3T::ZERO) const;

	// special points
	static const Vector3T ZERO;
	static const Vector3T UNIT_X;
	static const Vector3T UNIT_Y;
	static const Vector3T UNIT_Z;
	static const Vector3T UNIT_SCALE;
    static const Vector3T MAX_VEC;
    static const Vector3T MIN_VEC;
};

typedef Vector3T<float> Vector3;
typedef Vector3T<float> Vector3f;

#endif
