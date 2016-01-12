#ifndef Matrix3_h
#define Matrix3_h

#include "Point3.h"
#include <iostream>

#define M3AE_EPSILON 1e-12

class Matrix3
{
public:
    Matrix3();
    Matrix3(const Vector3& row0, const Vector3& row1, const Vector3& row2);
    Matrix3(const Matrix3& m);
  
    Matrix3& operator=(const Matrix3& m);
  
    // The indexing is this way to match OpenGL
    int index( int row, int col ) const;

    const double & operator()( int row, int col ) const;
    double & operator()( int row, int col );
  
    Vector3 row(int r) const;
    Vector3 column(int c) const;
    Matrix3 transpose() const;
  
    Matrix3 operator+( const Matrix3 &m) const;
    Matrix3& operator*=(double s);
    // pre-multiply column vector by a 3x3 matrix
    Vector3 operator*(const Vector3& v) const;
    Point3 operator*(const Point3& p) const;

    static Matrix3 identity();
    static Matrix3 rotationXYZtoUVW(Vector3 u, Vector3 v, Vector3 w);
    static double det2x2(double a, double b, double c, double d);

	double determinant() const;
    Matrix3 inverse() const;
    bool operator==( const Matrix3 &m ) const;
	bool approxEqual( const Matrix3 &m, double eps = M3AE_EPSILON ) const;
    void print() const;

private:
    double mat[9];
};

// post-multiply row vector by a 3x3 matrix
inline Vector3 operator*(const Vector3& v, const Matrix3& m)
{
    return Vector3(m(0,0) * v[0] + m(1,0) * v[1] + m(2,0) * v[2],
                   m(0,1) * v[0] + m(1,1) * v[1] + m(2,1) * v[2],
                   m(0,2) * v[0] + m(1,2) * v[1] + m(2,2) * v[2]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix3& m)
{
    os << m.row(0) << std::endl;
    os << m.row(1) << std::endl;
    os << m.row(2) << std::endl;
    return os;
}

#endif /* Matrix_3 */
