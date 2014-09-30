//
//  Vector4T.h
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Vector4T_h
#define _Vector4T_h

#include "vector3.h"


template <class T>
class Vector4T {
public:
    T x, y, z, w;

    inline Vector4T() {
    }
    
    inline Vector4T(const T fX, const T fY, const T fZ, const T fW)
    : x(fX), y(fY), z(fZ), w(fW) {
    }
    
    inline explicit Vector4T(const T afCoordinate[4])
    : x(afCoordinate[0]), y(afCoordinate[1]), z(afCoordinate[2]),
    w(afCoordinate[3]) {
    }
    
    inline explicit Vector4T(const int afCoordinate[4]) {
        x = (T)afCoordinate[0];
        y = (T)afCoordinate[1];
        z = (T)afCoordinate[2];
        w = (T)afCoordinate[3];
    }
    
    inline explicit Vector4T(T* const r)
    : x(r[0]), y(r[1]), z(r[2]), w(r[3]) {
    }
    
    inline explicit Vector4T(const T scaler)
    : x(scaler), y(scaler), z(scaler), w(scaler) {
    }
    
    inline explicit Vector4T(const Vector3& rhs)
    : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f) {
    }
    
    inline void swap(Vector4T& other) {
        std::swap(x, other.x);
        std::swap(y, other.y);
        std::swap(z, other.z);
        std::swap(w, other.w);
    }
	
    inline T operator [] (const size_t i) const {
        assert(i < 4);
        return *(&x+i);
    }
    
    inline T& operator [] (const size_t i) {
        assert(i < 4);
        return *(&x+i);
    }
    
    inline T* ptr() {
        return &x;
    }

    inline const T* ptr() const {
        return &x;
    }
    
    inline Vector4T& operator = (const Vector4T& rkVector) {
        x = rkVector.x;
        y = rkVector.y;
        z = rkVector.z;
        w = rkVector.w;
        
        return *this;
    }
    
    inline Vector4T& operator = (const T fScalar) {
        x = fScalar;
        y = fScalar;
        z = fScalar;
        w = fScalar;
        return *this;
    }
    
    inline bool operator == (const Vector4T& rkVector) const {
        return (x == rkVector.x &&
                y == rkVector.y &&
                z == rkVector.z &&
                w == rkVector.w);
    }
    
    inline bool operator != (const Vector4T& rkVector) const {
        return (x != rkVector.x ||
                y != rkVector.y ||
                z != rkVector.z ||
                w != rkVector.w);
    }
    
    inline Vector4T& operator = (const Vector3& rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = 1.0f;
        return *this;
    }
    
    // arithmetic operations
    inline Vector4T operator + (const Vector4T& rkVector) const {
        return Vector4T(
                       x + rkVector.x,
                       y + rkVector.y,
                       z + rkVector.z,
                       w + rkVector.w);
    }
    
    inline Vector4T operator - (const Vector4T& rkVector) const {
        return Vector4T(
                       x - rkVector.x,
                       y - rkVector.y,
                       z - rkVector.z,
                       w - rkVector.w);
    }
    
    inline Vector4T operator * (const T fScalar) const {
        return Vector4T(
                       x * fScalar,
                       y * fScalar,
                       z * fScalar,
                       w * fScalar);
    }
    
    inline Vector4T operator * (const Vector4T& rhs) const {
        return Vector4T(
                       rhs.x * x,
                       rhs.y * y,
                       rhs.z * z,
                       rhs.w * w);
    }
    
    inline Vector4T operator / (const T fScalar) const{
        assert(fScalar != 0.0);
        T fInv = 1.0f / fScalar;
        return Vector4T(
                       x * fInv,
                       y * fInv,
                       z * fInv,
                       w * fInv);
    }
    
    inline Vector4T operator / (const Vector4T& rhs) const {
        return Vector4T(
                       x / rhs.x,
                       y / rhs.y,
                       z / rhs.z,
                       w / rhs.w);
    }
    
    inline const Vector4T& operator + () const {
        return *this;
    }
    
    inline Vector4T operator - () const {
        return Vector4T(-x, -y, -z, -w);
    }
    
    inline friend Vector4T operator * (const T fScalar, const Vector4T& rkVector) {
        return Vector4T(
                       fScalar * rkVector.x,
                       fScalar * rkVector.y,
                       fScalar * rkVector.z,
                       fScalar * rkVector.w);
    }
    
    inline friend Vector4T operator / (const T fScalar, const Vector4T& rkVector) {
        return Vector4T(
                       fScalar / rkVector.x,
                       fScalar / rkVector.y,
                       fScalar / rkVector.z,
                       fScalar / rkVector.w);
    }
    
    inline friend Vector4T operator + (const Vector4T& lhs, const T rhs) {
        return Vector4T(
                       lhs.x + rhs,
                       lhs.y + rhs,
                       lhs.z + rhs,
                       lhs.w + rhs);
    }
    
    inline friend Vector4T operator + (const T lhs, const Vector4T& rhs) {
        return Vector4T(
                       lhs + rhs.x,
                       lhs + rhs.y,
                       lhs + rhs.z,
                       lhs + rhs.w);
    }
    
    inline friend Vector4T operator - (const Vector4T& lhs, T rhs) {
        return Vector4T(
                       lhs.x - rhs,
                       lhs.y - rhs,
                       lhs.z - rhs,
                       lhs.w - rhs);
    }
    
    inline friend Vector4T operator - (const T lhs, const Vector4T& rhs) {
        return Vector4T(
                       lhs - rhs.x,
                       lhs - rhs.y,
                       lhs - rhs.z,
                       lhs - rhs.w);
    }

    inline Vector4T& operator += (const Vector4T& rkVector) {
        x += rkVector.x;
        y += rkVector.y;
        z += rkVector.z;
        w += rkVector.w;
        
        return *this;
    }
    
    inline Vector4T& operator -= (const Vector4T& rkVector) {
        x -= rkVector.x;
        y -= rkVector.y;
        z -= rkVector.z;
        w -= rkVector.w;
        
        return *this;
    }
    
    inline Vector4T& operator *= (const T fScalar) {
        x *= fScalar;
        y *= fScalar;
        z *= fScalar;
        w *= fScalar;
        return *this;
    }
    
    inline Vector4T& operator += (const T fScalar)
    {
        x += fScalar;
        y += fScalar;
        z += fScalar;
        w += fScalar;
        return *this;
    }
    
    inline Vector4T& operator -= (const T fScalar) {
        x -= fScalar;
        y -= fScalar;
        z -= fScalar;
        w -= fScalar;
        return *this;
    }
    
    inline Vector4T& operator *= (const Vector4T& rkVector) {
        x *= rkVector.x;
        y *= rkVector.y;
        z *= rkVector.z;
        w *= rkVector.w;
        
        return *this;
    }
    
    inline Vector4T& operator /= (const T fScalar) {
        assert(fScalar != 0.0);

        T fInv = 1.0f / fScalar;
        x *= fInv;
        y *= fInv;
        z *= fInv;
        w *= fInv;
        
        return *this;
    }
    
    inline Vector4T& operator /= (const Vector4T& rkVector) {
        x /= rkVector.x;
        y /= rkVector.y;
        z /= rkVector.z;
        w /= rkVector.w;
        
        return *this;
    }
    
    inline T dotProduct(const Vector4T& vec) const {
        return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
    }

    inline bool isNaN() const {
        return x != x || y != y || z != z || w != w;
    }


    inline friend std::ostream& operator << (std::ostream& o, const Vector4T& v) {
        o << "Vector4T(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return o;
    }

    static const Vector4T ZERO;
};

typedef Vector4T<float> Vector4;
typedef Vector4T<uchar> Vector4Byte;

#endif
