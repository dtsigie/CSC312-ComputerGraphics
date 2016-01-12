#include "Matrix3.h"

#include <math.h>
#include <assert.h>
#include <cstring>


#include "../csc321.h"

Matrix3::Matrix3()
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            mat[ index(i,j) ] = (i == j) ? 1.0 : 0.0;
}

Matrix3::Matrix3(const Vector3& row0, const Vector3& row1, const Vector3& row2)
{
    for ( int i = 0; i < 3; i++ ) {
        mat[ index( 0, i ) ] = row0[i];
        mat[ index( 1, i ) ] = row1[i];
        mat[ index( 2, i ) ] = row2[i];
    }
}

Matrix3::Matrix3(const Matrix3& m)
{
    (*this) = m;
}

Matrix3& Matrix3::operator=(const Matrix3& m)
{
    memcpy( &mat[0], &m.mat[0], sizeof(double) * 16 );
    return *this;
}

// The indexing is this way to match OpenGL
int Matrix3::index( int row, int col ) const
{
	assert( row >= 0 && row < 3 ); assert( col >= 0 && col < 3 ); return col * 3 + row;
}

const double& Matrix3::operator()( int row, int col ) const
{
	return mat[ index(row,col) ];
}

double& Matrix3::operator()( int row, int col )
{
	return mat[ index(row,col) ];
}

Vector3 Matrix3::row(int r) const
{
    return Vector3( mat[index(r,0)], mat[index(r,1)], mat[index(r,2)] );
}

Vector3 Matrix3::column(int c) const
{
    return Vector3( mat[index(0,c)], mat[index(1,c)], mat[index(2,c)] );
}

Matrix3 Matrix3::transpose() const
{
    Matrix3 matRet;
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            matRet(i,j) = (*this)(j,i);
	return matRet;
}

Matrix3 Matrix3::operator+( const Matrix3 &m) const
{
    Matrix3 matRet;
    for ( int i = 0; i < 9; i++ )
        matRet.mat[i] = mat[i] + m.mat[i];
    return matRet;
}

Matrix3& Matrix3::operator*=(double s)
{
    for ( int i = 0; i < 9; i++ )
        mat[i] *= s;
    return *this;
}

// pre-multiply column vector by a 3x3 matrix
Vector3 Matrix3::operator*(const Vector3& v) const
{
    return Vector3((*this)(0,0) * v[0] + (*this)(0,1) * v[1] + (*this)(0,2) * v[2],
                   (*this)(1,0) * v[0] + (*this)(1,1) * v[1] + (*this)(1,2) * v[2],
                   (*this)(2,0) * v[0] + (*this)(2,1) * v[1] + (*this)(2,2) * v[2]);
}

Point3 Matrix3::operator*(const Point3& p) const
{
    return Point3((*this)(0,0) * p[0] + (*this)(0,1) * p[1] + (*this)(0,2) * p[2],
                  (*this)(1,0) * p[0] + (*this)(1,1) * p[1] + (*this)(1,2) * p[2],
                  (*this)(2,0) * p[0] + (*this)(2,1) * p[1] + (*this)(2,2) * p[2]);
}

Matrix3 Matrix3::identity()
{
    return Matrix3(Vector3(1, 0, 0),
                   Vector3(0, 1, 0),
                   Vector3(0, 0, 1));
}

Matrix3 Matrix3::rotationXYZtoUVW(Vector3 u, Vector3 v, Vector3 w)
{
    return Matrix3(Vector3(u[0], v[0], w[0]),
                   Vector3(u[1], v[1], w[1]),
                   Vector3(u[2], v[2], w[2]));
}

double Matrix3::det2x2(double a, double b, double c, double d)
{
    return a * d - b * c;
}

double Matrix3::determinant() const
{
    return ((*this)(0,0) * (*this)(1,1) * (*this)(2,2) +
            (*this)(0,1) * (*this)(1,2) * (*this)(2,0) +
            (*this)(0,2) * (*this)(1,0) * (*this)(2,1) -
            (*this)(0,2) * (*this)(1,1) * (*this)(2,0) -
            (*this)(0,0) * (*this)(1,2) * (*this)(2,1) -
            (*this)(0,1) * (*this)(1,0) * (*this)(2,2));
}

Matrix3 Matrix3::inverse() const
{
    Matrix3 adjoint( (Vector3(  det2x2((*this)(1,1), (*this)(1,2), (*this)(2,1), (*this)(2,2)),
                                -det2x2((*this)(1,0), (*this)(1,2), (*this)(2,0), (*this)(2,2)),
                                det2x2((*this)(1,0), (*this)(1,1), (*this)(2,0), (*this)(2,1)) )),
                        (Vector3( -det2x2((*this)(0,1), (*this)(0,2), (*this)(2,1), (*this)(2,2)),
                                det2x2((*this)(0,0), (*this)(0,2), (*this)(2,0), (*this)(2,2)),
                                -det2x2((*this)(0,0), (*this)(0,1), (*this)(2,0), (*this)(2,1)) )),
                        (Vector3(  det2x2((*this)(0,1), (*this)(0,2), (*this)(1,1), (*this)(1,2)),
                                -det2x2((*this)(0,0), (*this)(0,2), (*this)(1,0), (*this)(1,2)),
                                det2x2((*this)(0,0), (*this)(0,1), (*this)(1,0), (*this)(1,1)) )) );
    const double dDet = determinant();

    assert( isZero( dDet ) == false );
    adjoint *= 1.0 / dDet;

    return adjoint;
}

bool Matrix3::operator==( const Matrix3 &m ) const
{
    for ( int i = 0; i < 9; i++ )
        if ( mat[i] != m.mat[i] )
            return false;
    return true;
}

bool Matrix3::approxEqual( const Matrix3 &m, double eps) const
{
    for ( int i = 0; i < 9; i++ )
        if ( isZero( mat[i] - m.mat[i], eps ) == false)
            return false;
    return true;
}

void Matrix3::print() const
{
    std::cout << "( " << (*this)(0,0) << ", " << (*this)(0,1) << ", " << (*this)(0,2) << "\n";
    std::cout << "  " << (*this)(1,0) << ", " << (*this)(1,1) << ", " << (*this)(1,2) << "\n";
    std::cout << "  " << (*this)(2,0) << ", " << (*this)(2,1) << ", " << (*this)(2,2) << ")\n";
}
