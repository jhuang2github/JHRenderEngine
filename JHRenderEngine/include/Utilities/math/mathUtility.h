//
//  mathUtility.h
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __mathUtility__
#define __mathUtility__


#include "matrix4.h"
#include "plane.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"


class AxisAlignedBox;
class Line;
class Sphere;


class  MathUtility {
public:
    /** Default constructor.
     trigTableSize Optional parameter to set the size of the
     tables used to implement Sin, Cos, Tan
     */
    MathUtility(unsigned int trigTableSize = 4096);
    ~MathUtility();

    static int nearestPow2(int val);
    static int countSetBits(int n);
    static int shiftNumberToBitPos(int n, int pos);

    static float Sign(float fValue);
    static inline float Sqr (float fValue) { return fValue * fValue; }
    static inline float Sqrt (float fValue) { return sqrtf(fValue); }
    
    // A random number in the range from [0,1].
    static float UnitRandom ();
    // A random number in the range from [fLow,fHigh].
    static float RangeRandom (float fLow, float fHigh);
    // A random number in the range from [-1,1].
    static float SymmetricRandom ();
    
    static inline float DegreesToRadians(float degrees) { return degrees * fDeg2Rad; }
    static inline float RadiansToDegrees(float radians) { return radians * fRad2Deg; }
    
    // angular coordinates to cartesian
    static Vector2 AngularCoordToCartesian(float degree, float radius);
    
    /** Checks whether a given point is inside a triangle, in a
     2-dimensional (Cartesian) space.
     @remarks
     The vertices of the triangle must be given in either
     trigonometrical (anticlockwise) or inverse trigonometrical
     (clockwise) order.
     p The point.
     a The triangle's first vertex.
     b The triangle's second vertex.
     c The triangle's third vertex.
     */
    static bool pointInTri2D(const Vector2& p, const Vector2& a,
                             const Vector2& b, const Vector2& c);
    /** Checks whether a given 3D point is inside a triangle.
     The vertices of the triangle must be given in either
     trigonometrical (anticlockwise) or inverse trigonometrical
     (clockwise) order, and the point must be guaranteed to be in the
     same plane as the triangle
     p The point.
     a The triangle's first vertex.
     b The triangle's second vertex.
     c The triangle's third vertex.
     normal The triangle plane's normal (passed in rather than calculated
     on demand since the caller may already have it)
     */
    static bool pointInTri3D(const Vector3& p, const Vector3& a,
                const Vector3& b, const Vector3& c, const Vector3& normal);
    // Line / plane intersection, returns boolean result and distance.
    static std::pair<bool, float> intersects(const Line& Line, const Plane& plane);
    
    // Line / sphere intersection, returns boolean result and distance.
    static std::pair<bool, float> intersects(const Line& Line, const Sphere& sphere,
                                            bool discardInside = true);
    
    /** Line / box intersection, returns boolean result and distance. */
    static std::pair<bool, float> intersects(const Line& Line, const AxisAlignedBox& box);
    
    /** Line / box intersection, returns boolean result and two intersection distance.
     @param
     Line The Line.
     @param
     box The box.
     @param
     d1 A float pointer to retrieve the near intersection distance
     from the Line origin, maybe <b>null</b> which means don't care
     about the near intersection distance.
     @param
     d2 A float pointer to retrieve the far intersection distance
     from the Line origin, maybe <b>null</b> which means don't care
     about the far intersection distance.
     @return
     If the Line is intersects the box, <b>true</b> is returned, and
     the near intersection distance is return by <i>d1</i>, the
     far intersection distance is return by <i>d2</i>. Guarantee
     <b>0</b> <= <i>d1</i> <= <i>d2</i>.
     @par
     If the Line isn't intersects the box, <b>false</b> is returned, and
     <i>d1</i> and <i>d2</i> is unmodified.
     */
    static bool intersects(const Line& Line, const AxisAlignedBox& box,
                           float* d1, float* d2);
    
    /** Line / triangle intersection, returns boolean result and distance.
     @param
     Line The Line.
     @param
     a The triangle's first vertex.
     @param
     b The triangle's second vertex.
     @param
     c The triangle's third vertex.
     @param
     normal The triangle plane's normal (passed in rather than calculated
     on demand since the caller may already have it), doesn't need
     normalised since we don't care.
     @param
     positiveSide Intersect with "positive side" of the triangle
     @param
     negativeSide Intersect with "negative side" of the triangle
     @return
     If the Line is intersects the triangle, a pair of <b>true</b> and the
     distance between intersection point and Line origin returned.
     @par
     If the Line isn't intersects the triangle, a pair of <b>false</b> and
     <b>0</b> returned.
     */
    static std::pair<bool, float> intersects(const Line& Line, const Vector3& a,
                                            const Vector3& b, const Vector3& c, const Vector3& normal,
                                            bool positiveSide = true, bool negativeSide = true);
    
    /** Line / triangle intersection, returns boolean result and distance.
     @param
     Line The Line.
     @param
     a The triangle's first vertex.
     @param
     b The triangle's second vertex.
     @param
     c The triangle's third vertex.
     @param
     positiveSide Intersect with "positive side" of the triangle
     @param
     negativeSide Intersect with "negative side" of the triangle
     @return
     If the Line is intersects the triangle, a pair of <b>true</b> and the
     distance between intersection point and Line origin returned.
     @par
     If the Line isn't intersects the triangle, a pair of <b>false</b> and
     <b>0</b> returned.
     */
    static std::pair<bool, float> intersects(const Line& Line, const Vector3& a,
                                            const Vector3& b, const Vector3& c,
                                            bool positiveSide = true, bool negativeSide = true);
    
    /** Sphere / box intersection test. */
    static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);
    
    /** Plane / box intersection test. */
    static bool intersects(const Plane& plane, const AxisAlignedBox& box);
    
    /** Line / convex plane list intersection test.
     @param Line The Line to test with
     @param plaeList List of planes which form a convex volume
     @param normalIsOutside Does the normal point outside the volume
     */
    static std::pair<bool, float> intersects(
                                            const Line& Line, const vector<Plane>& planeList,
                                            bool normalIsOutside);
    /** Line / convex plane list intersection test.
     @param Line The Line to test with
     @param plaeList List of planes which form a convex volume
     @param normalIsOutside Does the normal point outside the volume
     */
//    static std::pair<bool, float> intersects(
//                                             const Line& Line, const list<Plane>& planeList,
//                                            bool normalIsOutside);
    
    /** Sphere / plane intersection test.
     @remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
     */
    static bool intersects(const Sphere& sphere, const Plane& plane);
    
    /** Compare 2 floats, using tolerance for inaccuracies.
     */
    static bool floatEqual(float a, float b,
                          float tolerance = std::numeric_limits<float>::epsilon());
    
    /** Calculates the tangent space vector for a given set of positions / texture coords. */
    static Vector3 calculateTangentSpaceVector(
                                               const Vector3& position1, const Vector3& position2, const Vector3& position3,
                                               float u1, float v1, float u2, float v2, float u3, float v3);
    
    /** Build a reflection matrix for the passed in plane. */
    static Matrix4 buildReflectionMatrix(const Plane& p);
    /** Calculate a face normal, including the w component which is the offset from the origin. */
    static Vector4 calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    /** Calculate a face normal, no w-information. */
    static Vector3 calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    /** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
    static Vector4 calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    /** Calculate a face normal without normalize, no w-information. */
    static Vector3 calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    
    /** Generates a value based on the Gaussian (normal) distribution function
     with the given offset and scale parameters.
     */
    static float gaussianDistribution(float x, float offset = 0.0f, float scale = 1.0f);
    
    /** Clamp a value within an inclusive range. */
    template <typename T>
    static T Clamp(T val, T minval, T maxval)
    {
        assert (minval <= maxval && "Invalid clamp range");
        return std::max(std::min(val, maxval), minval);
    }
    
    static Matrix4 makeViewMatrix(const Vector3& position, const Quaternion& orientation,
                                  const Matrix4* reflectMatrix = 0);
    
    /** Get a bounding radius value from a bounding box. The sphere centers at origin. */
    static float boundingRadiusCenteredAtOriginFromAABB(const AxisAlignedBox& aabb);


    static const float POS_INFINITY;
    static const float NEG_INFINITY;
    static const float PI;
    static const float TWO_PI;
    static const float HALF_PI;
    static const float fDeg2Rad;
    static const float fRad2Deg;

private:
    // angle units used by the api
//    static AngleUnit msAngleUnit;
    
    /// Size of the trig tables as determined by constructor.
    static int mTrigTableSize;
    
    /// Radian -> index factor value ( mTrigTableSize / 2 * PI )
    static float mTrigTableFactor;
    static float* mSinTable;
    static float* mTanTable;
    
    /** Private function to build trig tables.
     */
    void buildTrigTables();
    
    static float SinTable (float fValue);
    static float TanTable (float fValue);
    
};

#endif
