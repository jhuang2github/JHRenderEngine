//
//  mathUtility.cpp
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#include "axisAlignedBox.h"
#include "line.h"
#include "mathUtility.h"
#include "sphere.h"


const float MathUtility::POS_INFINITY = std::numeric_limits<float>::infinity();
const float MathUtility::NEG_INFINITY = -std::numeric_limits<float>::infinity();
const float MathUtility::PI = float( 4.0 * atan( 1.0 ) );
const float MathUtility::TWO_PI = float( 2.0 * PI );
const float MathUtility::HALF_PI = float( 0.5 * PI );
const float MathUtility::fDeg2Rad = PI / float(180.0);
const float MathUtility::fRad2Deg = float(180.0) / PI;
//const float MathUtility::LOG2 = log(float(2.0));

int MathUtility::mTrigTableSize;
//MathUtility::AngleUnit MathUtility::msAngleUnit;

float  MathUtility::mTrigTableFactor;
float *MathUtility::mSinTable = NULL;
float *MathUtility::mTanTable = NULL;


MathUtility::MathUtility( unsigned int trigTableSize ) {
}

MathUtility::~MathUtility() {
}


int MathUtility::nearestPow2(int val) {
    if (val <= 0) {
        return 0;
    }
    
    // 1 << (int)(log2f(v - 1.f) + 1.f);
    val--;
    val |= val >> 1;
    val |= val >> 2;
    val |= val >> 4;
    val |= val >> 8;
    val |= val >> 16;
    val++;
    return val;
}


int MathUtility::countSetBits(int n) {
    int count = 0;
    while (n) {
        n &= n - 1;
        count++;
    }
    return count;
}

// Take the most significant bits in |n| starting from a set bit,
// and set it at a starting position of |pos|-th bit (0-based).
int MathUtility::shiftNumberToBitPos(int n, int pos) {
    int p2 = nearestPow2(n);
    int bitsCount = log((float)p2) / log(2.f);
    if (bitsCount > pos) {
        n >>= bitsCount - pos;
    } else {
        n <<= pos - bitsCount;
    }
    return n;
}


float MathUtility::Sign(float fValue)
{
    if ( fValue > 0.0 )
        return 1.0;
    
    if ( fValue < 0.0 )
        return -1.0;
    
    return 0.0;
}

float MathUtility::UnitRandom ()
{
    // TODO(jhuang):
    return 0;
//    return rand() / rand_max();
}

float MathUtility::RangeRandom (float fLow, float fHigh)
{
    return (fHigh-fLow)*UnitRandom() + fLow;
}


float MathUtility::SymmetricRandom ()
{
    return 2.0f * UnitRandom() - 1.0f;
}


Vector2 MathUtility::AngularCoordToCartesian(float degree, float radius) {
    float rad = DegreesToRadians(degree);
    return Vector2(radius * cosf(rad), radius * sinf(rad));
}


bool MathUtility::pointInTri2D(const Vector2& p, const Vector2& a,
                    const Vector2& b, const Vector2& c)
{
    // Winding must be consistent from all edges for point to be inside
    Vector2 v1, v2;
    float dot[3];
    bool zeroDot[3];

    v1 = b - a;
    v2 = p - a;

    // Note we don't care about normalisation here since sign is all we need
    // It means we don't have to worry about magnitude of cross products either
    dot[0] = v1.crossProduct(v2);
    zeroDot[0] = MathUtility::floatEqual(dot[0], 0.0f, 1e-3);


    v1 = c - b;
    v2 = p - b;

    dot[1] = v1.crossProduct(v2);
    zeroDot[1] = MathUtility::floatEqual(dot[1], 0.0f, 1e-3);

    // Compare signs (ignore colinear / coincident points)
    if(!zeroDot[0] && !zeroDot[1]
       && MathUtility::Sign(dot[0]) != MathUtility::Sign(dot[1]))
    {
        return false;
    }

    v1 = a - c;
    v2 = p - c;

    dot[2] = v1.crossProduct(v2);
    zeroDot[2] = MathUtility::floatEqual(dot[2], 0.0f, 1e-3);
    // Compare signs (ignore colinear / coincident points)
    if((!zeroDot[0] && !zeroDot[2]
        && MathUtility::Sign(dot[0]) != MathUtility::Sign(dot[2])) ||
       (!zeroDot[1] && !zeroDot[2]
        && MathUtility::Sign(dot[1]) != MathUtility::Sign(dot[2])))
    {
        return false;
    }


    return true;
}

bool MathUtility::pointInTri3D(const Vector3& p, const Vector3& a,
                    const Vector3& b, const Vector3& c, const Vector3& normal)
{
    // Winding must be consistent from all edges for point to be inside
    Vector3 v1, v2;
    float dot[3];
    bool zeroDot[3];

    v1 = b - a;
    v2 = p - a;

    // Note we don't care about normalisation here since sign is all we need
    // It means we don't have to worry about magnitude of cross products either
    dot[0] = v1.crossProduct(v2).dotProduct(normal);
    zeroDot[0] = MathUtility::floatEqual(dot[0], 0.0f, 1e-3);


    v1 = c - b;
    v2 = p - b;

    dot[1] = v1.crossProduct(v2).dotProduct(normal);
    zeroDot[1] = MathUtility::floatEqual(dot[1], 0.0f, 1e-3);

    // Compare signs (ignore colinear / coincident points)
    if(!zeroDot[0] && !zeroDot[1]
       && MathUtility::Sign(dot[0]) != MathUtility::Sign(dot[1]))
    {
        return false;
    }

    v1 = a - c;
    v2 = p - c;

    dot[2] = v1.crossProduct(v2).dotProduct(normal);
    zeroDot[2] = MathUtility::floatEqual(dot[2], 0.0f, 1e-3);
    // Compare signs (ignore colinear / coincident points)
    if((!zeroDot[0] && !zeroDot[2]
        && MathUtility::Sign(dot[0]) != MathUtility::Sign(dot[2])) ||
       (!zeroDot[1] && !zeroDot[2]
        && MathUtility::Sign(dot[1]) != MathUtility::Sign(dot[2])))
    {
        return false;
    }


    return true;
}

bool MathUtility::floatEqual( float a, float b, float tolerance )
{
    if (fabs(b-a) <= tolerance)
        return true;
    else
        return false;
}


std::pair<bool, float> MathUtility::intersects(const Line& Line, const Plane& plane) {
    const Vector3& normal = plane.getNormal();
    float denom = normal.dotProduct(Line.getDirection());
    if (fabs(denom) < std::numeric_limits<float>::epsilon()) {
        // Parallel
        return std::pair<bool, float>(false, 0);
    } else {
        float nom = normal.dotProduct(Line.getOrigin()) + plane.getDistance();
        float t = -(nom / denom);
        return std::pair<bool, float>(t >= 0, t);
    }
}

std::pair<bool, float> MathUtility::intersects(const Line& Line,
                                   const vector<Plane>& planes, bool normalIsOutside)
{
    vector<Plane> planesList;
    for (vector<Plane>::const_iterator i = planes.begin(); i != planes.end(); ++i)
    {
        planesList.push_back(*i);
    }
    return intersects(Line, planesList, normalIsOutside);
}

//std::pair<bool, float> MathUtility::intersects(const Line& Line,
//                                   const list<Plane>& planes, bool normalIsOutside)
//{
//    list<Plane>::const_iterator planeit, planeitend;
//    planeitend = planes.end();
//    bool allInside = true;
//    std::pair<bool, float> ret;
//    std::pair<bool, float> end;
//    ret.first = false;
//    ret.second = 0.0f;
//    end.first = false;
//    end.second = 0;
//
//
//    // derive side
//    // NB we don't pass directly since that would require Plane::Side in
//    // interface, which results in recursive includes since Math is so fundamental
//    Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;
//
//    for (planeit = planes.begin(); planeit != planeitend; ++planeit)
//    {
//        const Plane& plane = *planeit;
//        // is origin outside?
//        if (plane.getSide(Line.getOrigin()) == outside)
//        {
//            allInside = false;
//            // Test single plane
//            std::pair<bool, float> planeRes =
//            Line.intersects(plane);
//            if (planeRes.first)
//            {
//                // Ok, we intersected
//                ret.first = true;
//                // Use the most distant result since convex volume
//                ret.second = std::max(ret.second, planeRes.second);
//            }
//            else
//            {
//                ret.first =false;
//                ret.second=0.0f;
//                return ret;
//            }
//        }
//        else
//        {
//            std::pair<bool, float> planeRes =
//            Line.intersects(plane);
//            if (planeRes.first)
//            {
//                if( !end.first )
//                {
//                    end.first = true;
//                    end.second = planeRes.second;
//                }
//                else
//                {
//                    end.second = std::min( planeRes.second, end.second );
//                }
//                
//            }
//            
//        }
//    }
//
//    if (allInside)
//    {
//        // Intersecting at 0 distance since inside the volume!
//        ret.first = true;
//        ret.second = 0.0f;
//        return ret;
//    }
//
//    if( end.first )
//    {
//        if( end.second < ret.second )
//        {
//            ret.first = false;
//            return ret;
//        }
//    }
//    return ret;
//}

std::pair<bool, float> MathUtility::intersects(const Line& Line, const Sphere& sphere,
                                   bool discardInside)
{
    const Vector3& lineDir = Line.getDirection();
    // Adjust Line origin relative to sphere center
    const Vector3& lineOrig = Line.getOrigin() - sphere.getCenter();
    float radius = sphere.getRadius();

    // Check origin inside first
    if (lineOrig.squaredLength() <= radius*radius && discardInside)
    {
        return std::pair<bool, float>(true, 0);
    }

    // Mmm, quadratics
    // Build coeffs which can be used with std quadratic solver
    // ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
    float a = lineDir.dotProduct(lineDir);
    float b = 2 * lineOrig.dotProduct(lineDir);
    float c = lineOrig.dotProduct(lineOrig) - radius*radius;

    // Calc determinant
    float d = (b*b) - (4 * a * c);
    if (d < 0)
    {
        // No intersection
        return std::pair<bool, float>(false, 0);
    }
    else
    {
        // BTW, if d=0 there is one intersection, if d > 0 there are 2
        // But we only want the closest one, so that's ok, just use the
        // '-' version of the solver
        float t = ( -b - MathUtility::Sqrt(d) ) / (2 * a);
        if (t < 0)
            t = ( -b + MathUtility::Sqrt(d) ) / (2 * a);
        return std::pair<bool, float>(true, t);
    }
}


//bool Line::RayIntersectsAABB(const Vertex& boundMin, const Vertex& boundMax) {
//	Vertex dir = end - start;
//	dir.Normalize();
//
//	// Check x and y.
//	float tmin = (boundMin.x - start.x) / dir.x;
//	float tmax = (boundMax.x - start.x) / dir.x;
//	if (tmin > tmax) {
//		std::swap(tmin, tmax);
//	}
//	float tymin = (boundMin.y - start.y) / dir.y;
//	float tymax = (boundMax.y - start.y) / dir.y;
//	if (tymin > tymax) {
//		std::swap(tymin, tymax);
//	}
//	if (tmin > tymax || (tymin > tmax)) {
//		return false;
//	}
//
//	// Check z.
//	if (tymin > tmin) {
//		tmin = tymin;
//	}
//	if (tymax < tmax) {
//		tmax = tymax;
//	}
//	float tzmin = (boundMin.z - start.z) / dir.z;
//	float tzmax = (boundMax.z - start.z) / dir.z;
//	if (tzmin > tzmax) {
//		std::swap(tzmin, tzmax);
//	}
//	if (tmin > tzmax || tzmin > tmax) {
//		return false;
//	}
//
//    //	if (tzmin > tmin) {
//    //		tmin = tzmin;
//    //	}
//    //	if (tzmax < tmax) {
//    //		tmax = tzmax;
//    //	}
//
//	// Check if tmin and tmax are outside the ray range (ray.tmin, ray.tmax).
//    //	if (tmin > ray.tmax || tmax < ray.tmin) {
//    //		return false;
//    //	}
//    //	if (ray.tmin < tmin) {
//    //		ray.tmin = tmin;
//    //	}
//    //	if (ray.tmax > tmax) {
//    //		ray.tmax = tmax;
//    //	}
//
//	return true;
//}
std::pair<bool, float> MathUtility::intersects(const Line& line, const AxisAlignedBox& box) {
    if (box.isNull()) {
        return std::pair<bool, float>(false, 0);
    }
    if (box.isInfinite()) {
        return std::pair<bool, float>(true, 0);
    }

    const Vector3& min = box.getMinimum();
    const Vector3& max = box.getMaximum();
    const Vector3& lineOrig = line.getOrigin();
    const Vector3& lineDir = line.getDirection();

    // Check if the origin is inside first.
    if ( lineOrig > min && lineOrig < max) {
        return std::pair<bool, float>(true, 0);
    }

    bool hit = false;
    Vector3 hitpoint;
    float lowt = 0.0f;
    float t;

    // Check each face in turn, only check closest 3.

    // Min x
    if (lineOrig.x <= min.x && lineDir.x > 0) {
        t = (min.x - lineOrig.x) / lineDir.x;
        if (t >= 0) {
            // Substitute t back into Line and check bounds and dist
            hitpoint = lineOrig + lineDir * t;
            if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt)) {
                hit = true;
                lowt = t;
            }
        }
    }
    // Max x
    if (lineOrig.x >= max.x && lineDir.x < 0) {
        t = (max.x - lineOrig.x) / lineDir.x;
        if (t >= 0) {
            // Substitute t back into Line and check bounds and dist
            hitpoint = lineOrig + lineDir * t;
            if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt)) {
                hit = true;
                lowt = t;
            }
        }
    }

    // Min y
    if (lineOrig.y <= min.y && lineDir.y > 0) {
        t = (min.y - lineOrig.y) / lineDir.y;
        if (t >= 0)
        {
            // Substitute t back into Line and check bounds and dist
            hitpoint = lineOrig + lineDir * t;
            if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt)) {
                hit = true;
                lowt = t;
            }
        }
    }
    // Max y
    if (lineOrig.y >= max.y && lineDir.y < 0) {
        t = (max.y - lineOrig.y) / lineDir.y;
        if (t >= 0) {
            // Substitute t back into Line and check bounds and dist
            hitpoint = lineOrig + lineDir * t;
            if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.z >= min.z && hitpoint.z <= max.z &&
                (!hit || t < lowt)) {
                hit = true;
                lowt = t;
            }
        }
    }

    // Min z
    if (lineOrig.z <= min.z && lineDir.z > 0) {
        t = (min.z - lineOrig.z) / lineDir.z;
        if (t >= 0) {
            // Substitute t back into Line and check bounds and dist
            hitpoint = lineOrig + lineDir * t;
            if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.y >= min.y && hitpoint.y <= max.y &&
                (!hit || t < lowt)) {
                hit = true;
                lowt = t;
            }
        }
    }
    // Max z
    if (lineOrig.z >= max.z && lineDir.z < 0) {
        t = (max.z - lineOrig.z) / lineDir.z;
        if (t >= 0) {
            // Substitute t back into Line and check bounds and dist
            hitpoint = lineOrig + lineDir * t;
            if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
                hitpoint.y >= min.y && hitpoint.y <= max.y &&
                (!hit || t < lowt)) {
                hit = true;
                lowt = t;
            }
        }
    }

    return std::pair<bool, float>(hit, lowt);
}

//bool MathUtility::intersects(const Line& Line, const AxisAlignedBox& box,
//                  float* d1, float* d2)
//{
//    if (box.isNull())
//        return false;
//
//    if (box.isInfinite())
//    {
//        if (d1) *d1 = 0;
//        if (d2) *d2 = MathUtility::POS_INFINITY;
//        return true;
//    }
//
//    const Vector3& min = box.getMinimum();
//    const Vector3& max = box.getMaximum();
//    const Vector3& lineOrig = Line.getOrigin();
//    const Vector3& lineDir = Line.getDirection();
//
//    Vector3 absDir;
//    absDir[0] = fabs(lineDir[0]);
//    absDir[1] = fabs(lineDir[1]);
//    absDir[2] = fabs(lineDir[2]);
//
//    // Sort the axis, ensure check minimise floating error axis first
//    int imax = 0, imid = 1, imin = 2;
//    if (absDir[0] < absDir[2])
//    {
//        imax = 2;
//        imin = 0;
//    }
//    if (absDir[1] < absDir[imin])
//    {
//        imid = imin;
//        imin = 1;
//    }
//    else if (absDir[1] > absDir[imax])
//    {
//        imid = imax;
//        imax = 1;
//    }
//
//    float start = 0, end = MathUtility::POS_INFINITY;
//
//    #define _CALC_AXIS(i)                                       \
//    do {                                                    \
//    float denom = 1 / lineDir[i];                         \
//    float newstart = (min[i] - lineOrig[i]) * denom;      \
//    float newend = (max[i] - lineOrig[i]) * denom;        \
//    if (newstart > newend) std::swap(newstart, newend); \
//    if (newstart > end || newend < start) return false; \
//    if (newstart > start) start = newstart;             \
//    if (newend < end) end = newend;                     \
//    } while(0)
//
//    // Check each axis in turn
//
//    _CALC_AXIS(imax);
//
//    if (absDir[imid] < std::numeric_limits<float>::epsilon())
//    {
//        // Parallel with middle and minimise axis, check bounds only
//        if (lineOrig[imid] < min[imid] || lineOrig[imid] > max[imid] ||
//            lineOrig[imin] < min[imin] || lineOrig[imin] > max[imin])
//            return false;
//    }
//    else
//    {
//        _CALC_AXIS(imid);
//        
//        if (absDir[imin] < std::numeric_limits<float>::epsilon())
//        {
//            // Parallel with minimise axis, check bounds only
//            if (lineOrig[imin] < min[imin] || lineOrig[imin] > max[imin])
//                return false;
//        }
//        else
//        {
//            _CALC_AXIS(imin);
//        }
//    }
//    #undef _CALC_AXIS
//
//    if (d1) *d1 = start;
//    if (d2) *d2 = end;
//
//    return true;
//}

std::pair<bool, float> MathUtility::intersects(const Line& Line, const Vector3& a,
                                   const Vector3& b, const Vector3& c, const Vector3& normal,
                                   bool positiveSide, bool negativeSide)
{
    //
    // Calculate intersection with plane.
    //
    float t;
    {
        float denom = normal.dotProduct(Line.getDirection());
        
        // Check intersect side
        if (denom > + std::numeric_limits<float>::epsilon())
        {
            if (!negativeSide)
                return std::pair<bool, float>(false, 0);
        }
        else if (denom < - std::numeric_limits<float>::epsilon())
        {
            if (!positiveSide)
                return std::pair<bool, float>(false, 0);
        }
        else
        {
            // Parallel or triangle area is close to zero when
            // the plane normal not normalised.
            return std::pair<bool, float>(false, 0);
        }
        
        t = normal.dotProduct(a - Line.getOrigin()) / denom;
        
        if (t < 0)
        {
            // Intersection is behind origin
            return std::pair<bool, float>(false, 0);
        }
    }

    //
    // Calculate the largest area projection plane in X, Y or Z.
    //
    size_t i0, i1;
    {
        float n0 = fabs(normal[0]);
        float n1 = fabs(normal[1]);
        float n2 = fabs(normal[2]);
        
        i0 = 1; i1 = 2;
        if (n1 > n2)
        {
            if (n1 > n0) i0 = 0;
        }
        else
        {
            if (n2 > n0) i1 = 0;
        }
    }

    //
    // Check the intersection point is inside the triangle.
    //
    {
        float u1 = b[i0] - a[i0];
        float v1 = b[i1] - a[i1];
        float u2 = c[i0] - a[i0];
        float v2 = c[i1] - a[i1];
        float u0 = t * Line.getDirection()[i0] + Line.getOrigin()[i0] - a[i0];
        float v0 = t * Line.getDirection()[i1] + Line.getOrigin()[i1] - a[i1];
        
        float alpha = u0 * v2 - u2 * v0;
        float beta  = u1 * v0 - u0 * v1;
        float area  = u1 * v2 - u2 * v1;
        
        // epsilon to avoid float precision error
        const float EPSILON = 1e-6f;
        
        float tolerance = - EPSILON * area;
        
        if (area > 0)
        {
            if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
                return std::pair<bool, float>(false, 0);
        }
        else
        {
            if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
                return std::pair<bool, float>(false, 0);
        }
    }

    return std::pair<bool, float>(true, t);
}

std::pair<bool, float> MathUtility::intersects(const Line& Line, const Vector3& a,
                                   const Vector3& b, const Vector3& c,
                                   bool positiveSide, bool negativeSide)
{
    Vector3 normal = calculateBasicFaceNormalWithoutNormalize(a, b, c);
    return intersects(Line, a, b, c, normal, positiveSide, negativeSide);
}

//bool MathUtility::intersects(const Sphere& sphere, const AxisAlignedBox& box)
//{
//    if (box.isNull()) return false;
//    if (box.isInfinite()) return true;
//
//    // Use splitting planes
//    const Vector3& center = sphere.getCenter();
//    float radius = sphere.getRadius();
//    const Vector3& min = box.getMinimum();
//    const Vector3& max = box.getMaximum();
//
//    // Arvo's algorithm
//    float s, d = 0;
//    for (int i = 0; i < 3; ++i)
//    {
//        if (center.ptr()[i] < min.ptr()[i])
//        {
//            s = center.ptr()[i] - min.ptr()[i];
//            d += s * s;
//        }
//        else if(center.ptr()[i] > max.ptr()[i])
//        {
//            s = center.ptr()[i] - max.ptr()[i];
//            d += s * s;
//        }
//    }
//    return d <= radius * radius;
//
//}
//
//bool MathUtility::intersects(const Plane& plane, const AxisAlignedBox& box)
//{
//    return (plane.getSide(box) == Plane::BOTH_SIDE);
//}
//
//bool MathUtility::intersects(const Sphere& sphere, const Plane& plane)
//{
//    return (
//            fabs(plane.getDistance(sphere.getCenter()))
//            <= sphere.getRadius() );
//}

Vector3 MathUtility::calculateTangentSpaceVector(
                                      const Vector3& position1, const Vector3& position2, const Vector3& position3,
                                      float u1, float v1, float u2, float v2, float u3, float v3)
{
    //side0 is the vector along one side of the triangle of vertices passed in,
    //and side1 is the vector along another side. Taking the cross product of these returns the normal.
    Vector3 side0 = position1 - position2;
    Vector3 side1 = position3 - position1;
    //Calculate face normal
    Vector3 normal = side1.crossProduct(side0);
    normal.normalise();
    //Now we use a formula to calculate the tangent.
    float deltaV0 = v1 - v2;
    float deltaV1 = v3 - v1;
    Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
    tangent.normalise();
    //Calculate binormal
    float deltaU0 = u1 - u2;
    float deltaU1 = u3 - u1;
    Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
    binormal.normalise();
    //Now, we take the cross product of the tangents to get a vector which
    //should point in the same direction as our normal calculated above.
    //If it points in the opposite direction (the dot product between the normals is less than zero),
    //then we need to reverse the s and t tangents.
    //This is because the triangle has been mirrored when going from tangent space to object space.
    //reverse tangents if necessary
    Vector3 tangentCross = tangent.crossProduct(binormal);
    if (tangentCross.dotProduct(normal) < 0.0f)
    {
        tangent = -tangent;
        binormal = -binormal;
    }

    return tangent;

}

Matrix4 MathUtility::buildReflectionMatrix(const Plane& p)
{
    const Vector3& normal = p.getNormal();
    float distance = p.getDistance();
    return Matrix4(
       -2 * normal.x * normal.x + 1,   -2 * normal.x * normal.y,       -2 * normal.x * normal.z,       -2 * normal.x * distance,
       -2 * normal.y * normal.x,       -2 * normal.y * normal.y + 1,   -2 * normal.y * normal.z,       -2 * normal.y * distance,
       -2 * normal.z * normal.x,       -2 * normal.z * normal.y,       -2 * normal.z * normal.z + 1,   -2 * normal.z * distance,
       0,                                  0,                                  0,                                  1);
}

Vector4 MathUtility::calculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    Vector3 normal = calculateBasicFaceNormal(v1, v2, v3);
    // Now set up the w (distance of tri from origin
    return Vector4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
}

Vector3 MathUtility::calculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
    normal.normalise();
    return normal;
}

Vector4 MathUtility::calculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    Vector3 normal = calculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
    // Now set up the w (distance of tri from origin)
    return Vector4(normal.x, normal.y, normal.z, -(normal.dotProduct(v1)));
}

Vector3 MathUtility::calculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
    Vector3 normal = (v2 - v1).crossProduct(v3 - v1);
    return normal;
}

float MathUtility::gaussianDistribution(float x, float offset, float scale)
{
    float nom = (float)exp(
                         -MathUtility::Sqr(x - offset) / (2 * MathUtility::Sqr(scale)));
    float denom = scale * MathUtility::Sqrt(2 * MathUtility::PI);

    return nom / denom;
}

Matrix4 MathUtility::makeViewMatrix(const Vector3& position, const Quaternion& orientation,
                         const Matrix4* reflectMatrix)
{
    Matrix4 viewMatrix;

    // View matrix is:
    //
    //  [ Lx  Uy  Dz  Tx  ]
    //  [ Lx  Uy  Dz  Ty  ]
    //  [ Lx  Uy  Dz  Tz  ]
    //  [ 0   0   0   1   ]
    //
    // Where T = -(Transposed(Rot) * Pos)

    // This is most efficiently done using 3x3 Matrices
    Matrix3 rot;
    orientation.ToRotationMatrix(rot);

    // Make the translation relative to new axes
    Matrix3 rotT = rot.Transpose();
    Vector3 trans = -rotT * position;

    // Make final matrix
    viewMatrix = Matrix4::IDENTITY;
    viewMatrix = rotT; // fills upper 3x3
    viewMatrix[0][3] = trans.x;
    viewMatrix[1][3] = trans.y;
    viewMatrix[2][3] = trans.z;

    // Deal with reflections
    if (reflectMatrix)
    {
        viewMatrix = viewMatrix * (*reflectMatrix);
    }

    return viewMatrix;

}

float MathUtility::boundingRadiusCenteredAtOriginFromAABB(const AxisAlignedBox& aabb) {
    Vector3 max = aabb.getMaximum();
    Vector3 min = aabb.getMinimum();

    Vector3 magnitude = max;
    magnitude.makeCeil(-max);
    magnitude.makeCeil(min);
    magnitude.makeCeil(-min);

    return magnitude.length();
}
