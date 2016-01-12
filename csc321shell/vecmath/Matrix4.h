#ifndef Matrix4_h
#define Matrix4_h

#include "Vector3.h"
#include "Point3.h"
#include "Vector4.h"
#include <iostream>
#include <math.h>
#include <assert.h>

#define M4AE_EPSILON 1e-12

class Matrix4
{
public:
    Matrix4();
    Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);
    Matrix4(const Vector3& row0, const Vector3& row1, const Vector3& row2 );
    Matrix4(const Matrix4& m);
  
    Matrix4& operator=(const Matrix4& m);
  
    // The indexing is this way to match OpenGL
    int index( int row, int col ) const;

    const double& operator()( int row, int col ) const;
    double& operator()( int row, int col );
  
    Vector4 row(int r) const;
    Vector4 column(int c) const;

    Matrix4 & operator *=( const Matrix4 &m );
    Matrix4 & operator +=( const Matrix4 &m );
    Matrix4 & operator -=( const Matrix4 &m );

	Matrix4 transpose() const;

	Matrix4 operator+( const Matrix4 &m) const;
    Matrix4 operator-( const Matrix4 &m) const;
    Vector3 operator*(const Vector3& v) const;
    Point3 operator*(const Point3& p) const;
    Vector4 operator*(const Vector4& v) const;

	Matrix4 operator*(const Matrix4& b) const;
	Matrix4 inverse() const;

    bool operator==( const Matrix4 &m ) const;
    bool approxEqual( const Matrix4 &m, double eps = 1e-12 ) const;
    void print() const;

    static Matrix4 identity();
    static Matrix4 translation(const Point3& p);
    static Matrix4 translation(const Vector3& v);
    static Matrix4 rotation(const Vector3& u, const Vector3& v, const Vector3& w);
    static Matrix4 rotation(const Vector3& axis, double angle);
    static Matrix4 xrotation(double angle);
    static Matrix4 yrotation(double angle);
    static Matrix4 zrotation(double angle);
    static Matrix4 scaling(const Vector3& s);
    static Matrix4 scaling( double x, double y, double z);
    static Matrix4 scaling(double scale);

private:
    double mat[16];
};

inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
{
    os << m.row(0) << std::endl;
    os << m.row(1) << std::endl;
    os << m.row(2) << std::endl;
    os << m.row(3) << std::endl;
    return os;
}

#endif /* Matrix4_h */
