//
//  quaternion.h
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _quaternion_h
#define _quaternion_h

#include "platformDef.h"
#include "matrix3.h"
#include "vector3.h"


class Quaternion {
public:
    /// Default constructor, initializes to identity rotation (aka 0¬∞)
    inline Quaternion ()
    : w(1), x(0), y(0), z(0)
    {
    }
    /// Construct from an explicit list of values
    inline Quaternion (float fW,
                       float fX, float fY, float fZ)
    : w(fW), x(fX), y(fY), z(fZ)
    {
    }
    /// Construct a quaternion from a rotation matrix
    inline Quaternion(const Matrix3& rot)
    {
        this->FromRotationMatrix(rot);
    }
    /// Construct a quaternion from an angle/axis
    inline Quaternion(const Radian& rfAngle, const Vector3& rkAxis)
    {
        this->FromAngleAxis(rfAngle, rkAxis);
    }
    /// Construct a quaternion from x, y, z angles/axes
    Quaternion(const Radian& rXAngle, const Radian& rYAngle, const Radian& rZAngle);
    /// Construct a quaternion from 3 orthonormal local axes
    inline Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
    {
        this->FromAxes(xaxis, yaxis, zaxis);
    }
    /// Construct a quaternion from 3 orthonormal local axes
    inline Quaternion(const Vector3* akAxis)
    {
        this->FromAxes(akAxis);
    }
    /// Construct a quaternion from 4 manual w/x/y/z values
    inline Quaternion(float* valptr)
    {
        memcpy(&w, valptr, sizeof(float)*4);
    }
    
    /** Exchange the contents of this quaternion with another. 
     */
    inline void swap(Quaternion& other)
    {
        std::swap(w, other.w);
        std::swap(x, other.x);
        std::swap(y, other.y);
        std::swap(z, other.z);
    }
    
    /// Array accessor operator
    inline float operator [] ( const size_t i ) const
    {
        assert( i < 4 );
        return *(&w+i);
    }
    
    /// Array accessor operator
    inline float& operator [] ( const size_t i )
    {
        assert( i < 4 );
        return *(&w+i);
    }
    
    /// Pointer accessor for direct copying
    inline float* ptr()
    {
        return &w;
    }
    
    /// Pointer accessor for direct copying
    inline const float* ptr() const
    {
        return &w;
    }
    
    void FromRotationMatrix (const Matrix3& kRot);
    void ToRotationMatrix (Matrix3& kRot) const;
    /** Setups the quaternion using the supplied vector, and "roll" around
     that vector by the specified radians.
     */
    void FromAngleAxis (const Radian& rfAngle, const Vector3& rkAxis);
    inline void ToAngleAxis (Radian& dAngle, Vector3& rkAxis) const;
    /** Constructs the quaternion using 3 axes, the axes are assumed to be orthonormal
     @see FromAxes
     */
    void FromAxes (const Vector3* akAxis);
    void FromAxes (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
    /** Gets the 3 orthonormal axes defining the quaternion. @see FromAxes */
    void ToAxes (Vector3* akAxis) const;
    void ToAxes (Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;
    
    /** Returns the X orthonormal axis defining the quaternion. Same as doing
     xAxis = Vector3::UNIT_X * this. Also called the local X-axis
     */
    Vector3 xAxis(void) const;
    
    /** Returns the Y orthonormal axis defining the quaternion. Same as doing
     yAxis = Vector3::UNIT_Y * this. Also called the local Y-axis
     */
    Vector3 yAxis(void) const;
    
    /** Returns the Z orthonormal axis defining the quaternion. Same as doing
     zAxis = Vector3::UNIT_Z * this. Also called the local Z-axis
     */
    Vector3 zAxis(void) const;
    
    inline Quaternion& operator= (const Quaternion& rkQ)
    {
        w = rkQ.w;
        x = rkQ.x;
        y = rkQ.y;
        z = rkQ.z;
        return *this;
    }
    Quaternion operator+ (const Quaternion& rkQ) const;
    Quaternion operator- (const Quaternion& rkQ) const;
    Quaternion operator* (const Quaternion& rkQ) const;
    Quaternion operator* (float fScalar) const;
    friend Quaternion operator* (float fScalar,
                                             const Quaternion& rkQ);
    Quaternion operator- () const;
    inline bool operator== (const Quaternion& rhs) const
    {
        return (rhs.x == x) && (rhs.y == y) &&
        (rhs.z == z) && (rhs.w == w);
    }
    inline bool operator!= (const Quaternion& rhs) const
    {
        return !operator==(rhs);
    }
    // functions of a quaternion
    /// Returns the dot product of the quaternion
    float Dot (const Quaternion& rkQ) const;
    /* Returns the normal length of this quaternion.
     @note This does <b>not</b> alter any values.
     */
    float Norm () const;
    /// Normalises this quaternion, and returns the previous length
    float normalise(void); 
    Quaternion Inverse () const;  // apply to non-zero quaternion
    Quaternion UnitInverse () const;  // apply to unit-length quaternion
    Quaternion Exp () const;
    Quaternion Log () const;
    
    /// Rotation of a vector by a quaternion
    Vector3 operator* (const Vector3& rkVector) const;
    
    /** Calculate the local roll element of this quaternion.
     @param reprojectAxis By default the method returns the 'intuitive' result
     that is, if you projected the local Y of the quaternion onto the X and
     Y axes, the angle between them is returned. If set to false though, the
     result is the actual yaw that will be used to implement the quaternion,
     which is the shortest possible path to get to the same orientation and 
     may involve less axial rotation.  The co-domain of the returned value is 
     from -180 to 180 degrees.
     */
    Radian getRoll(bool reprojectAxis = true) const;
    /** Calculate the local pitch element of this quaternion
     @param reprojectAxis By default the method returns the 'intuitive' result
     that is, if you projected the local Z of the quaternion onto the X and
     Y axes, the angle between them is returned. If set to true though, the
     result is the actual yaw that will be used to implement the quaternion,
     which is the shortest possible path to get to the same orientation and 
     may involve less axial rotation.  The co-domain of the returned value is 
     from -180 to 180 degrees.
     */
    Radian getPitch(bool reprojectAxis = true) const;
    /** Calculate the local yaw element of this quaternion
     @param reprojectAxis By default the method returns the 'intuitive' result
     that is, if you projected the local Y of the quaternion onto the X and
     Z axes, the angle between them is returned. If set to true though, the
     result is the actual yaw that will be used to implement the quaternion,
     which is the shortest possible path to get to the same orientation and 
     may involve less axial rotation. The co-domain of the returned value is 
     from -180 to 180 degrees.
     */
    Radian getYaw(bool reprojectAxis = true) const;		
    /// Equality with tolerance (tolerance is max angle difference)
    bool equals(const Quaternion& rhs, const Radian& tolerance) const;
    
    /** Performs Spherical linear interpolation between two quaternions, and returns the result.
     Slerp ( 0.0f, A, B ) = A
     Slerp ( 1.0f, A, B ) = B
     @return Interpolated quaternion
     @remarks
     Slerp has the proprieties of performing the interpolation at constant
     velocity, and being torque-minimal (unless shortestPath=false).
     However, it's NOT commutative, which means
     Slerp ( 0.75f, A, B ) != Slerp ( 0.25f, B, A );
     therefore be careful if your code relies in the order of the operands.
     This is specially important in IK animation.
     */
    static Quaternion Slerp (float fT, const Quaternion& rkP,
                             const Quaternion& rkQ, bool shortestPath = false);
    
    /** @see Slerp. It adds extra "spins" (i.e. rotates several times) specified
     by parameter 'iExtraSpins' while interpolating before arriving to the
     final values
     */
    static Quaternion SlerpExtraSpins (float fT,
                                       const Quaternion& rkP, const Quaternion& rkQ,
                                       int iExtraSpins);
    
    // setup for spherical quadratic interpolation
    static void Intermediate (const Quaternion& rkQ0,
                              const Quaternion& rkQ1, const Quaternion& rkQ2,
                              Quaternion& rka, Quaternion& rkB);
    
    // spherical quadratic interpolation
    static Quaternion Squad (float fT, const Quaternion& rkP,
                             const Quaternion& rkA, const Quaternion& rkB,
                             const Quaternion& rkQ, bool shortestPath = false);
    
    /** Performs Normalised linear interpolation between two quaternions, and returns the result.
     nlerp ( 0.0f, A, B ) = A
     nlerp ( 1.0f, A, B ) = B
     @remarks
     Nlerp is faster than Slerp.
     Nlerp has the proprieties of being commutative (@see Slerp;
     commutativity is desired in certain places, like IK animation), and
     being torque-minimal (unless shortestPath=false). However, it's performing
     the interpolation at non-constant velocity; sometimes this is desired,
     sometimes it is not. Having a non-constant velocity can produce a more
     natural rotation feeling without the need of tweaking the weights; however
     if your scene relies on the timing of the rotation or assumes it will point
     at a specific angle at a specific weight value, Slerp is a better choice.
     */
    static Quaternion nlerp(float fT, const Quaternion& rkP, 
                            const Quaternion& rkQ, bool shortestPath = false);
    
    /// Cutoff for sine near zero
    static const float msEpsilon;
    
    // special values
    static const Quaternion ZERO;
    static const Quaternion IDENTITY;
    
    float w, x, y, z;
    
    /// Check whether this quaternion contains valid values
    inline bool isNaN() const
    {
        return x != x || y != y || z != z || w != w;
    }
    
    /** Function for writing to a stream. Outputs "Quaternion(w, x, y, z)" with w,x,y,z
     being the member values of the quaternion.
     */
    inline friend std::ostream& operator << ( std::ostream& o, const Quaternion& q )
    {
        o << "Quaternion(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
        return o;
    }
    
};

#endif
