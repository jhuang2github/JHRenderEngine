//
//  axisAlignedBox.h
//
//  Created by Jingshu Huang on 5/14/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef __axisAlignedBox__
#define __axisAlignedBox__

#include <vector>

#include "mathUtility.h"
#include "matrix4.h"
#include "plane.h"
#include "shape.h"
#include "vector3.h"


//class Mesh;

/*
 A 3D box aligned with the x/y/z axes. It stores 2 points as the extremeties
 of the box. One is the minima of all 3 axes, and the other is the maxima of
 all 3 axes.

  1-------2
  /|     /|
 / |    / |
 5-----4  |
 |  0--|--3
 | /   | /
 |/    |/
 6-----7
 */
class AxisAlignedBox : public Shape {
public:
    typedef enum {
        FAR_LEFT_BOTTOM = 0,
        FAR_LEFT_TOP = 1,
        FAR_RIGHT_TOP = 2,
        FAR_RIGHT_BOTTOM = 3,
        NEAR_RIGHT_TOP = 4,
        NEAR_LEFT_TOP = 5,
        NEAR_LEFT_BOTTOM = 6,
        NEAR_RIGHT_BOTTOM = 7
    } EBoxCorner;

    inline AxisAlignedBox() {
        // default to invalid shape
        setMinMax(Vector3::MAX_VEC, Vector3::MIN_VEC);
    }

    inline AxisAlignedBox(const AxisAlignedBox& rkBox) {
        *this = rkBox;
    }

    inline AxisAlignedBox(const Vector3& min, const Vector3& max) {
        setMinMax(min, max);
    }
    
    inline AxisAlignedBox(float ix, float iy, float iz,
                          float ax, float ay, float az ) {
        setMinMax(Vector3(ix, iy, iz), Vector3(ax, ay, az));
    }

    AxisAlignedBox& operator=(const AxisAlignedBox& rhs) {
        setMinMax(rhs.mMinimum, rhs.mMaximum);
        return *this;
    }

    ~AxisAlignedBox() {
        mCorners.clear();
    }

    inline bool isValid(void) const { return *this != BOX_INVALID; }
    inline bool isNull(void) const { return *this == BOX_NULL; }
    inline bool isInfinite(void) const { return *this == BOX_INFINITE; }

    inline void setMinMax(const Vector3& min, const Vector3& max) {
        mMinimum = min;
        mMaximum = max;
    }

    inline void setMinimum(const Vector3& vec) { mMinimum = vec; }
    inline void setMinimum(const float x, const float y, const float z) { setMinimum(Vector3(x, y, z)); }
    inline void setMinimumX(float x) { mMinimum.x = x; }
    inline void setMinimumY(float y) { mMinimum.y = y; }
    inline void setMinimumZ(float z) { mMinimum.z = z; }
    inline void setMaximum(const Vector3& vec) { mMaximum = vec; }
    inline void setMaximum(float x, float y, float z) { setMaximum(Vector3(x, y, z)); }
    inline void setMaximumX(float x) { mMaximum.x = x; }
    inline void setMaximumY(float y) { mMaximum.y = y; }
    inline void setMaximumZ(float z) { mMaximum.z = z; }

    inline const Vector3& getMinimum(void) const { return mMinimum; }
    inline const Vector3& getMaximum(void) const { return mMaximum; }

    const std::vector<Vector3>& getCorners(void);
    Vector3 getCorner(EBoxCorner cornerToGet) const;
    Vector3 getCenter(void) const;

    inline Vector3 getSize(void) const { return mMaximum - mMinimum; }
    inline Vector3 getHalfSize(void) const { return getSize() * 0.5f; }

    /// Calculate the volume of this box
    float volume(void) const;

    /** Scales the AABB by the vector given. */
    void scale(const Vector3& s);

    // Merges the passed in box into the current box. The result is the
    // box which encompasses both.
    void merge(const AxisAlignedBox& rhs);
    // Extends the box to encompass the specified point if needed.
    void merge(const Vector3& point);

    /** Transforms the box according to the matrix supplied.
     By calling this method you get the axis-aligned box which
     surrounds the transformed version of this box. Therefore each
     corner of the box is transformed by the matrix, then the
     extents are mapped back onto the axes to produce another
     AABB. Useful when you have a local AABB for an object which
     is then transformed.
     */
    void transform(const Matrix4& matrix);
    
    /** Transforms the box according to the affine matrix supplied.
     By calling this method you get the axis-aligned box which
     surrounds the transformed version of this box. Therefore each
     corner of the box is transformed by the matrix, then the
     extents are mapped back onto the axes to produce another
     AABB. Useful when you have a local AABB for an object which
     is then transformed.
     The matrix must be an affine matrix. @see Matrix4::isAffine.
     */
    void transformAffine(const Matrix4& m);

    /** Returns whether or not this box intersects another. */
    bool intersects(const AxisAlignedBox& b2) const;
    // Calculate the area of intersection of this box and another
    inline AxisAlignedBox intersection(const AxisAlignedBox& b2) const;
    /** Tests whether this box intersects a sphere. */
    inline bool intersects(const Sphere& s) const {
        return MathUtility::intersects(s, *this);
    }
    /** Tests whether this box intersects a plane. */
    inline bool intersects(const Plane& p) const {
        return MathUtility::intersects(p, *this);
    }

    /** Tests whether another box contained by this box.
     */
    bool contains(const AxisAlignedBox& other) const;
    /** Tests whether the vector point is within this box. */
    bool contains(const Vector3& v) const;

    /** Returns the minimum distance between a given point and any part of the box. */
    float minDistance(const Vector3& v) const;

    /** Tests 2 boxes for equality.
     */
    bool operator== (const AxisAlignedBox& rhs) const {
        return this->mMinimum == rhs.mMinimum &&
                this->mMaximum == rhs.mMaximum;
    }
    
    /** Tests 2 boxes for inequality.
     */
    bool operator!= (const AxisAlignedBox& rhs) const {
        return !(*this == rhs);
    }

    inline friend std::ostream& operator<<(std::ostream& os,
                                           const AxisAlignedBox& aab) {
        os << "AxisAlignedBox:\n (min = " << aab.mMinimum
            << "\n max=" << aab.mMaximum << ")\n";
		return os;
    }

    // Override.
    void loadIntoMesh(Mesh &mesh);
    void updateMesh(Mesh &mesh);

    // special values
    static const AxisAlignedBox BOX_INVALID;  // Uninitialized box.
    static const AxisAlignedBox BOX_NULL;
    static const AxisAlignedBox BOX_INFINITE;
    static const AxisAlignedBox BOX_ORIGIN_CENTER_UNIT;
    static const AxisAlignedBox BOX_ORIGIN_START_UNIT;

protected:
    void updateCorners();

    Vector3 mMinimum;
    Vector3 mMaximum;
    mutable std::vector<Vector3> mCorners;
};

#endif
