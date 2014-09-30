//
//  Matrix4T.h
//
//  Created by Jingshu Huang on 1/17/13.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _Matrix4T_h
#define _Matrix4T_h

#include "platformDef.h"
#include "matrix3.h"
#include "quaternion.h"
#include "vector4.h"


template <class T>
class Matrix4T {
public:
    union {
        T m[4][4];  // index by [row][col]
        T data[16];
    };

    inline Matrix4T() {
    }
    
    inline Matrix4T(T m00, T m01, T m02, T m03,
                   T m10, T m11, T m12, T m13,
                   T m20, T m21, T m22, T m23,
                   T m30, T m31, T m32, T m33) {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }
    
    /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling 3x3 matrix.
     */    
    inline Matrix4T(const Matrix3& m) {
        operator=(IDENTITY);
        operator=(m);
    }

    /** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling Quaternion.
     */
    inline Matrix4T(const Quaternion& rot) {
        Matrix3 m3x3;
        rot.ToRotationMatrix(m3x3);
        operator=(IDENTITY);
        operator=(m3x3);
    }
    
    inline void swap(Matrix4T& other) {
        std::swap(m[0][0], other.m[0][0]);
        std::swap(m[0][1], other.m[0][1]);
        std::swap(m[0][2], other.m[0][2]);
        std::swap(m[0][3], other.m[0][3]);
        std::swap(m[1][0], other.m[1][0]);
        std::swap(m[1][1], other.m[1][1]);
        std::swap(m[1][2], other.m[1][2]);
        std::swap(m[1][3], other.m[1][3]);
        std::swap(m[2][0], other.m[2][0]);
        std::swap(m[2][1], other.m[2][1]);
        std::swap(m[2][2], other.m[2][2]);
        std::swap(m[2][3], other.m[2][3]);
        std::swap(m[3][0], other.m[3][0]);
        std::swap(m[3][1], other.m[3][1]);
        std::swap(m[3][2], other.m[3][2]);
        std::swap(m[3][3], other.m[3][3]);
    }
    
    inline T* operator [] (size_t iRow) {
        assert(iRow < 4);
        return m[iRow];
    }
    
    inline const T *operator [] (size_t iRow) const {
        assert(iRow < 4);
        return m[iRow];
    }
    
    inline Matrix4T concatenate(const Matrix4T &m2) const
    {
        Matrix4T r;
        r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
        r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
        r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
        r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];
        
        r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
        r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
        r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
        r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];
        
        r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
        r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
        r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
        r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];
        
        r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
        r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
        r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
        r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];
        
        return r;
    }
    
    /** Matrix concatenation using '*'.
     */
    inline Matrix4T operator * (const Matrix4T &m2) const
    {
        return concatenate(m2);
    }
    
    /** Vector transformation using '*'.
     @remarks
     Transforms the given 3-D vector by the matrix, projecting the 
     result back into <i>w</i> = 1.
     @note
     This means that the initial <i>w</i> is considered to be 1.0,
     and then all the tree elements of the resulting 3-D vector are
     divided by the resulting <i>w</i>.
     */
    inline Vector3 operator * (const Vector3 &v) const
    {
        Vector3 r;
        
        T fInvW = 1.0f / (m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3]);
        
        r.x = (m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3]) * fInvW;
        r.y = (m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3]) * fInvW;
        r.z = (m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]) * fInvW;
        
        return r;
    }
    inline Vector4 operator * (const Vector4& v) const
    {
        return Vector4(
                       m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                       m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                       m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                       m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
                      );
    }
//    inline Plane operator * (const Plane& p) const
//    {
//        Plane ret;
//        Matrix4T invTrans = inverse().transpose();
//        Vector4 v4(p.normal.x, p.normal.y, p.normal.z, p.d);
//        v4 = invTrans * v4;
//        ret.normal.x = v4.x; 
//        ret.normal.y = v4.y; 
//        ret.normal.z = v4.z;
//        ret.d = v4.w / ret.normal.normalise();
//        
//        return ret;
//    }
    
    
    /** Matrix addition.
     */
    inline Matrix4T operator + (const Matrix4T &m2) const
    {
        Matrix4T r;
        
        r.m[0][0] = m[0][0] + m2.m[0][0];
        r.m[0][1] = m[0][1] + m2.m[0][1];
        r.m[0][2] = m[0][2] + m2.m[0][2];
        r.m[0][3] = m[0][3] + m2.m[0][3];
        
        r.m[1][0] = m[1][0] + m2.m[1][0];
        r.m[1][1] = m[1][1] + m2.m[1][1];
        r.m[1][2] = m[1][2] + m2.m[1][2];
        r.m[1][3] = m[1][3] + m2.m[1][3];
        
        r.m[2][0] = m[2][0] + m2.m[2][0];
        r.m[2][1] = m[2][1] + m2.m[2][1];
        r.m[2][2] = m[2][2] + m2.m[2][2];
        r.m[2][3] = m[2][3] + m2.m[2][3];
        
        r.m[3][0] = m[3][0] + m2.m[3][0];
        r.m[3][1] = m[3][1] + m2.m[3][1];
        r.m[3][2] = m[3][2] + m2.m[3][2];
        r.m[3][3] = m[3][3] + m2.m[3][3];
        
        return r;
    }
    
    /** Matrix subtraction.
     */
    inline Matrix4T operator - (const Matrix4T &m2) const
    {
        Matrix4T r;
        r.m[0][0] = m[0][0] - m2.m[0][0];
        r.m[0][1] = m[0][1] - m2.m[0][1];
        r.m[0][2] = m[0][2] - m2.m[0][2];
        r.m[0][3] = m[0][3] - m2.m[0][3];
        
        r.m[1][0] = m[1][0] - m2.m[1][0];
        r.m[1][1] = m[1][1] - m2.m[1][1];
        r.m[1][2] = m[1][2] - m2.m[1][2];
        r.m[1][3] = m[1][3] - m2.m[1][3];
        
        r.m[2][0] = m[2][0] - m2.m[2][0];
        r.m[2][1] = m[2][1] - m2.m[2][1];
        r.m[2][2] = m[2][2] - m2.m[2][2];
        r.m[2][3] = m[2][3] - m2.m[2][3];
        
        r.m[3][0] = m[3][0] - m2.m[3][0];
        r.m[3][1] = m[3][1] - m2.m[3][1];
        r.m[3][2] = m[3][2] - m2.m[3][2];
        r.m[3][3] = m[3][3] - m2.m[3][3];
        
        return r;
    }
    
    /** Tests 2 matrices for equality.
     */
    inline bool operator == (const Matrix4T& m2) const
    {
        if(
           m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
           m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
           m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
           m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3])
            return false;
        return true;
    }
    
    /** Tests 2 matrices for inequality.
     */
    inline bool operator != (const Matrix4T& m2) const
    {
        if(
           m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
           m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
           m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
           m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3])
            return true;
        return false;
    }
    
    /** Assignment from 3x3 matrix.
     */
    inline void operator = (const Matrix3& mat3)
    {
        m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2];
        m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2];
        m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2];
    }
    
    inline Matrix4T transpose(void) const
    {
        return Matrix4T(m[0][0], m[1][0], m[2][0], m[3][0],
                       m[0][1], m[1][1], m[2][1], m[3][1],
                       m[0][2], m[1][2], m[2][2], m[3][2],
                       m[0][3], m[1][3], m[2][3], m[3][3]);
    }
    
    /*
     -----------------------------------------------------------------------
     Translation Transformation
     -----------------------------------------------------------------------
     */
    /** Sets the translation transformation part of the matrix.
     */
    inline void setTrans(const Vector3& v)
    {
        m[0][3] = v.x;
        m[1][3] = v.y;
        m[2][3] = v.z;
    }
    
    /** Extracts the translation transformation part of the matrix.
     */
    inline Vector3 getTrans() const
    {
        return Vector3(m[0][3], m[1][3], m[2][3]);
    }
    
    
    /** Builds a translation matrix
     */
    inline void makeTrans(const Vector3& v)
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }
    
    inline void makeTrans(T tx, T ty, T tz)
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }
    
    /** Gets a translation matrix.
     */
    inline static Matrix4T getTrans(const Vector3& v)
    {
        Matrix4T r;
        
        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = v.x;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = v.y;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = v.z;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;
        
        return r;
    }
    
    /** Gets a translation matrix - variation for not using a vector.
     */
    inline static Matrix4T getTrans(T t_x, T t_y, T t_z)
    {
        Matrix4T r;
        
        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = t_x;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = t_y;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = t_z;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;
        
        return r;
    }
    
    /*
     -----------------------------------------------------------------------
     Scale Transformation
     -----------------------------------------------------------------------
     */
    /** Sets the scale part of the matrix.
     */
    inline void setScale(const Vector3& v)
    {
        m[0][0] = v.x;
        m[1][1] = v.y;
        m[2][2] = v.z;
    }
    
    /** Gets a scale matrix.
     */
    inline static Matrix4T getScale(const Vector3& v)
    {
        Matrix4T r;
        r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;
        
        return r;
    }
    
    /** Gets a scale matrix - variation for not using a vector.
     */
    inline static Matrix4T getScale(T s_x, T s_y, T s_z)
    {
        Matrix4T r;
        r.m[0][0] = s_x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = s_y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = s_z; r.m[2][3] = 0.0;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;
        
        return r;
    }
    
    /** Extracts the rotation / scaling part of the Matrix as a 3x3 matrix. 
     @param m3x3 Destination Matrix3
     */
    inline void extract3x3Matrix(Matrix3& m3x3) const
    {
        m3x3.m[0][0] = m[0][0];
        m3x3.m[0][1] = m[0][1];
        m3x3.m[0][2] = m[0][2];
        m3x3.m[1][0] = m[1][0];
        m3x3.m[1][1] = m[1][1];
        m3x3.m[1][2] = m[1][2];
        m3x3.m[2][0] = m[2][0];
        m3x3.m[2][1] = m[2][1];
        m3x3.m[2][2] = m[2][2];
        
    }

    inline bool TEqual(T x, T val, T tolerance) const {
        return fabs(x - val) <= tolerance;
    }

    /** Determines if this matrix involves a scaling. */
    inline bool hasScale() const
    {
        // check magnitude of column vectors (==local axes)
        T t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
        if (!TEqual(t, 1.0, (T)1e-04))
            return true;
        t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
        if (!TEqual(t, 1.0, (T)1e-04))
            return true;
        t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
        if (!TEqual(t, 1.0, (T)1e-04))
            return true;
        
        return false;
    }
    
    /** Determines if this matrix involves a negative scaling. */
    inline bool hasNegativeScale() const
    {
        return determinant() < 0;
    }
    
    /** Extracts the rotation / scaling part as a quaternion from the Matrix.
     */
    inline Quaternion extractQuaternion() const
    {
        Matrix3 m3x3;
        extract3x3Matrix(m3x3);
        return Quaternion(m3x3);
    }
    
	static const Matrix4T ZERO;
	static const Matrix4T ZEROAFFINE;
	static const Matrix4T IDENTITY;
    /** Useful little matrix which takes 2D clipspace {-1, 1} to {0,1}
     and inverts the Y. */
    static const Matrix4T CLIPSPACE2DTOIMAGESPACE;
    
    inline Matrix4T operator*(T scalar) const
    {
        return Matrix4T(
                       scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
                       scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
                       scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
                       scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
    }
    
    /** Function for writing to a stream.
     */
    inline friend std::ostream& operator << (std::ostream& o, const Matrix4T& mat) {
        o << "Matrix4T(";
        for (size_t i = 0; i < 4; ++i)
        {
            o << " row" << (unsigned)i << "{";
            for(size_t j = 0; j < 4; ++j)
            {
                o << mat[i][j] << " ";
            }
            o << "}";
        }
        o << ")";
        return o;
    }
    
    Matrix4T adjoint() const;
    T determinant() const;
    Matrix4T inverse() const;
    
    /** Building a Matrix4T from orientation / scale / position.
     @remarks
     Transform is performed in the order scale, rotate, translation, i.e. translation is independent
     of orientation axes, scale does not affect size of translation, rotation and scaling are always
     centered on the origin.
     */
    void makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);
    
    /** Building an inverse Matrix4T from orientation / scale / position.
     @remarks
     As makeTransform except it build the inverse given the same data as makeTransform, so
     performing -translation, -rotate, 1/scale in that order.
     */
    void makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);
    
    /** Decompose a Matrix4T to orientation / scale / position.
     */
    void decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const;
    
    /** Check whether or not the matrix is affine matrix.
     @remarks
     An affine matrix is a 4x4 matrix with row 3 equal to (0, 0, 0, 1),
     e.g. no projective coefficients.
     */
    inline bool isAffine(void) const
    {
        return m[3][0] == 0 && m[3][1] == 0 && m[3][2] == 0 && m[3][3] == 1;
    }
    
    /** Returns the inverse of the affine matrix.
     @note
     The matrix must be an affine matrix. @see Matrix4T::isAffine.
     */
    Matrix4T inverseAffine(void) const;
    
    /** Concatenate two affine matrices.
     @note
     The matrices must be affine matrix. @see Matrix4T::isAffine.
     */
    inline Matrix4T concatenateAffine(const Matrix4T &m2) const
    {
        assert(isAffine() && m2.isAffine());
        
        return Matrix4T(
                       m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0],
                       m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1],
                       m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2],
                       m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3],
                       
                       m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0],
                       m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1],
                       m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2],
                       m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3],
                       
                       m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0],
                       m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1],
                       m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2],
                       m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3],
                       
                       0, 0, 0, 1);
    }
    
    /** 3-D Vector transformation specially for an affine matrix.
     @remarks
     Transforms the given 3-D vector by the matrix, projecting the 
     result back into <i>w</i> = 1.
     @note
     The matrix must be an affine matrix. @see Matrix4T::isAffine.
     */
    inline Vector3 transformAffine(const Vector3& v) const
    {
        assert(isAffine());
        
        return Vector3(
                       m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3], 
                       m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
                       m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]);
    }
    
    /** 4-D Vector transformation specially for an affine matrix.
     @note
     The matrix must be an affine matrix. @see Matrix4T::isAffine.
     */
    inline Vector4 transformAffine(const Vector4& v) const
    {
        assert(isAffine());
        
        return Vector4(
                       m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w, 
                       m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
                       m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
                       v.w);
    }
};

typedef Matrix4T<float>  Matrix4;

#endif
