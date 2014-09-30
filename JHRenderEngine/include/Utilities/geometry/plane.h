//
//  plane.h
//
//  Created by Jingshu Huang on 5/13/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __plane__
#define __plane__


#include "shape.h"
#include "vector3.h"

class AxisAlignedBox;


// Defines a plane in 3D space. A plane is defined in 3D space by the equation:
// Ax + By + Cz + D = 0.
// That is: dot(vertex_on_plane(x, y, z), plane_normal(a, b, c)) = constant;
//
// This equates to a vector (the normal of the plane, whose x, y
// and z components equate to the coefficients A, B and C
// respectively), and a constant (D) which is the distance along
// the normal you have to go to move the plane back to the origin.
//
// Also note that by defining a normal, we also define the positive and negative
// side of a plane. See Plane::Side enums.
//
class Plane : public Shape {
public:
    // The "positive side" of the plane is the half space to which the
    // plane normal points. The "negative side" is the other half
    // space. "no side" indicates the plane itself. "both sides" is used only
    // when doing intersection test with other objects.
    enum Side {
        NO_SIDE,
        POSITIVE_SIDE,
        NEGATIVE_SIDE,
        BOTH_SIDE
    };

    Plane();
    Plane(const Vector3& normal, const float dist);
    ~Plane();

    inline void setNormal(const Vector3& normal) {
        mNormal = normal;
        mNormal.normalise();
    }
    inline const Vector3& getNormal(void) const { return mNormal; }
    inline void setDistance(const float d) { mDistance = d; }
    inline float getDistance(void) const { return mDistance; }

    // Calculate the distance betweent he point to the plane.
    // The sign of the return value is positive if the point is on the positive
    // side of the plane, negative if the point is on the negative side, and
    // zero if the point is on the plane.
    // Note the absolute value of the return value is the true distance only
    // when the plane normal is a unit length vector.
    float getDistance(const Vector3& point) const;

    // Resturn teh side where the point is relative to the plane.
    Side getSide(const Vector3& rkPoint) const;

    // Returns the side where the alignedBox is. The flag BOTH_SIDE indicates an
    // intersecting box.One corner ON the plane is sufficient to consider the
    // box and the plane intersecting.
    Side getSide(const AxisAlignedBox& rkBox) const;

    // Returns which side of the plane that the given box lies on.
    // The box is defined as centre/half-size pairs for effectively.
    // @param center The center of the box.
    // @param halfSize The half-size of the box.
    // @return
    // POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
    // NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
    // BOTH_SIDE if the box intersects the plane.
    Side getSide (const Vector3& center, const Vector3& halfSize) const;

    // Output plane info.
	inline friend std::ostream& operator<< (std::ostream& o, const Plane& p) {
		o << "Plane(normal=" << p.mNormal << ", d=" << p.mDistance << ")";
		return o;
    }

private:
    Vector3 mNormal;
    float mDistance;
};

#endif
