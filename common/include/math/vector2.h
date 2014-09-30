//
//  Vector2T.h
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Vector2T_h
#define _Vector2T_h

#include "platformDef.h"


template <class T>
static T clamp(T val, T minval, T maxval) {
    assert (minval <= maxval && "Invalid clamp range");
    return std::max(std::min(val, maxval), minval);
}


// Uniform random number in [0, 1].
template <class T>
static T unifRand() {
    return rand() / static_cast<T>(RAND_MAX);
}


// Uniform random number in [a, b];
template <class T>
static T unifRand(const T& a, const T& b) {
    return (b - a) * (rand() / static_cast<T>(RAND_MAX)) + a;
}


template <class T>
class Vector2T {
public:
    T x, y;

public:
    inline Vector2T() {
        srand(time(0));
    }

    inline Vector2T(const T fX, const T fY) : x(fX), y(fY) {
    }
    
    inline explicit Vector2T(const T scaler) : x(scaler), y(scaler) {
    }
    
    inline explicit Vector2T(const T afCoordinate[2])
    : x(afCoordinate[0]), y(afCoordinate[1]) {
    }
    
    inline explicit Vector2T(const int afCoordinate[2]) {
        x = (T)afCoordinate[0];
        y = (T)afCoordinate[1];
    }
    
    inline explicit Vector2T(T* const r) : x(r[0]), y(r[1]){
    }
    
    inline void swap(Vector2T& other) {
        std::swap(x, other.x);
        std::swap(y, other.y);
    }
    
    inline T operator [] (const size_t i) const {
        assert(i < 2);
        return *(&x+i);
    }
    
    inline T& operator [] (const size_t i) {
        assert(i < 2);
        return *(&x+i);
    }
    
    inline T* ptr() {
        return &x;
    }

    inline const T* ptr() const {
        return &x;
    }
    
    inline Vector2T& operator = (const Vector2T& rkVector) {
        x = rkVector.x;
        y = rkVector.y;
        return *this;
    }
    
    inline Vector2T& operator = (const T fScalar) {
        x = fScalar;
        y = fScalar;
        return *this;
    }
    
    inline bool operator == (const Vector2T& rkVector) const {
        return (x == rkVector.x && y == rkVector.y);
    }
    
    inline bool operator != (const Vector2T& rkVector) const {
        return (x != rkVector.x || y != rkVector.y );
    }

    inline Vector2T operator + (const Vector2T& rkVector) const {
        return Vector2T(x + rkVector.x, y + rkVector.y);
    }
    
    inline Vector2T operator - (const Vector2T& rkVector) const {
        return Vector2T(x - rkVector.x, y - rkVector.y);
    }
    
    inline Vector2T operator * (const T fScalar) const {
        return Vector2T(x * fScalar, y * fScalar);
    }
    
    inline Vector2T operator * (const Vector2T& rhs) const {
        return Vector2T(x * rhs.x,
                       y * rhs.y);
    }
    
    inline Vector2T operator / (const T fScalar) const {
        assert(fScalar != 0.0);
        T fInv = 1.0f / fScalar;
        return Vector2T(x * fInv, y * fInv);
    }
    
    inline Vector2T operator / (const Vector2T& rhs) const {
        return Vector2T(x / rhs.x, y / rhs.y);
    }
    
    inline const Vector2T& operator + () const {
        return *this;
    }
    
    inline Vector2T operator - () const {
        return Vector2T(-x, -y);
    }

    inline friend Vector2T operator * (const T fScalar, const Vector2T& rkVector) {
        return Vector2T(fScalar * rkVector.x, fScalar * rkVector.y);
    }
    
    inline friend Vector2T operator / (const T fScalar, const Vector2T& rkVector) {
        return Vector2T(fScalar / rkVector.x, fScalar / rkVector.y);
    }
    
    inline friend Vector2T operator + (const Vector2T& lhs, const T rhs) {
        return Vector2T(lhs.x + rhs, lhs.y + rhs);
    }
    
    inline friend Vector2T operator + (const T lhs, const Vector2T& rhs) {
        return Vector2T(lhs + rhs.x, lhs + rhs.y);
    }
    
    inline friend Vector2T operator - (const Vector2T& lhs, const T rhs) {
        return Vector2T(lhs.x - rhs, lhs.y - rhs);
    }
    
    inline friend Vector2T operator - (const T lhs, const Vector2T& rhs) {
        return Vector2T(lhs - rhs.x, lhs - rhs.y);
    }

    inline Vector2T& operator += (const Vector2T& rkVector) {
        x += rkVector.x;
        y += rkVector.y;
        return *this;
    }
    
    inline Vector2T& operator += (const T fScaler) {
        x += fScaler;
        y += fScaler;
        return *this;
    }
    
    inline Vector2T& operator -= (const Vector2T& rkVector) {
        x -= rkVector.x;
        y -= rkVector.y;
        return *this;
    }
    
    inline Vector2T& operator -= (const T fScaler) {
        x -= fScaler;
        y -= fScaler;
        return *this;
    }
    
    inline Vector2T& operator *= (const T fScalar) {
        x *= fScalar;
        y *= fScalar;
        return *this;
    }
    
    inline Vector2T& operator *= (const Vector2T& rkVector) {
        x *= rkVector.x;
        y *= rkVector.y;
        return *this;
    }
    
    inline Vector2T& operator /= (const T fScalar) {
        assert(fScalar != 0.0);
        T fInv = 1.0f / fScalar;
        x *= fInv;
        y *= fInv;
        return *this;
    }
    
    inline Vector2T& operator /= (const Vector2T& rkVector) {
        x /= rkVector.x;
        y /= rkVector.y;
        
        return *this;
    }
    
    inline T length () const {
        return sqrtf(x * x + y * y);
    }
    
    inline T squaredLength () const {
        return x * x + y * y;
    }
    
    inline T distance(const Vector2T& rhs) const {
        return (*this - rhs).length();
    }
    
    inline T squaredDistance(const Vector2T& rhs) const {
        return (*this - rhs).squaredLength();
    }
    
    inline T dotProduct(const Vector2T& vec) const {
        return x * vec.x + y * vec.y;
    }
    
    inline T normalise() {
        T fLength = sqrtf(x * x + y * y);
        if (fLength > 1e-06) {
            T fInvLength = 1.0f / fLength;
            x *= fInvLength;
            y *= fInvLength;
        }
        
        return fLength;
    }
    
    inline Vector2T midPoint(const Vector2T& vec) const {
        return Vector2T((x + vec.x) * 0.5f, (y + vec.y) * 0.5f);
    }
    
    inline bool operator < (const Vector2T& rhs) const {
        return (x < rhs.x && y < rhs.y);
    }
    
    inline bool operator > (const Vector2T& rhs) const {
        return (x > rhs.x && y > rhs.y);
    }
    
    inline void makeFloor(const Vector2T& cmp) {
        if(cmp.x < x) x = cmp.x;
        if(cmp.y < y) y = cmp.y;
    }
    
    inline void makeCeil(const Vector2T& cmp)
    {
        if(cmp.x > x) x = cmp.x;
        if(cmp.y > y) y = cmp.y;
    }

    // Generate a vector perpendicular to the current vector.
    inline Vector2T perpendicular(void) const {
        return Vector2T (-y, x);
    }
    
    inline T crossProduct(const Vector2T& rkVector) const {
        return x * rkVector.y - y * rkVector.x;
    }
    
    inline Vector2T randomDeviant(T angle) const {
        angle *=  unifRand<T>() * (2.f * M_PI);
        T cosa = cosf(angle);
        T sina = sinf(angle);
        return  Vector2T(cosa * x - sina * y, sina * x + cosa * y);
    }
    
    inline bool isZeroLength(void) const {
        T sqlen = (x * x) + (y * y);
        return (sqlen < (1e-06 * 1e-06));
        
    }
    
    inline Vector2T normalisedCopy(void) const {
        Vector2T ret = *this;
        ret.normalise();
        return ret;
    }
    
    // Calculates a reflection vector to the plane with the given normal .
    inline Vector2T reflect(const Vector2T& normal) const {
        return Vector2T(*this - (2 * this->dotProduct(normal) * normal));
    }
    
    inline bool isNaN() const {
        return x != x || y != y;
    }
    
    inline Radian angleBetween(const Vector2T& other) const {
        T lenProduct = length() * other.length();
        if(lenProduct < 1e-6f)
            lenProduct = 1e-6f;
		
        T f = dotProduct(other) / lenProduct;
        
        f = clamp(f, -1.f, 1.f);
        return acosf(f);
    }

    //Gets the oriented angle between 2 vectors. The angle is comprised between
    // 0 and 2 PI.
    inline Radian angleTo(const Vector2T& other) const {
        Radian angle = angleBetween(other);
        if (crossProduct(other)<0)			
            angle = 2.f * M_PI - angle;

        return angle;
    }

    // Function for writing to a stream.
    inline friend std::ostream& operator << (std::ostream& o, const Vector2T& v) {
        o << "Vector2T(" << v.x << ", " << v.y << ")";
        return o;
    }

    // special points
    static const Vector2T ZERO;
    static const Vector2T UNIT_X;
    static const Vector2T UNIT_Y;
    static const Vector2T NEGATIVE_UNIT_X;
    static const Vector2T NEGATIVE_UNIT_Y;
    static const Vector2T UNIT_SCALE;    
};

typedef Vector2T<float> Vector2;
typedef Vector2T<float> Vector2f;

#endif
