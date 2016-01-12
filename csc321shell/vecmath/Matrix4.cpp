#include "Matrix4.h"

#include <math.h>
#include <assert.h>
#include <cstring>

#include "../csc321.h"

Matrix4::Matrix4()
{
    for ( int i = 0; i < 4; i++ )
        for ( int j = 0; j < 4; j++ )
            mat[ index(i,j) ] = (i == j) ? 1.0 : 0.0;
}

Matrix4::Matrix4(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3)
{
    for ( int i = 0; i < 4; i++ )
	{
        mat[ index( 0, i ) ] = row0[i];
        mat[ index( 1, i ) ] = row1[i];
        mat[ index( 2, i ) ] = row2[i];
        mat[ index( 3, i ) ] = row3[i];
    }
}

Matrix4::Matrix4(const Vector3& row0, const Vector3& row1, const Vector3& row2 )
{
    for ( int i = 0; i < 3; i++ )
	{
        mat[ index( 0, i ) ] = row0[i];
        mat[ index( 1, i ) ] = row1[i];
        mat[ index( 2, i ) ] = row2[i];
        mat[ index(i,3) ] = 0.0;
        mat[ index(3,i) ] = 0.0;
    }
    mat[ index(3,3) ] = 1.0;
}

Matrix4::Matrix4(const Matrix4& m)
{
    (*this) = m;
}

Matrix4& Matrix4::operator=(const Matrix4& m)
{
    memcpy( &mat[0], &m.mat[0], sizeof(double) * 16 );
    return *this;
}

// The indexing is this way to match OpenGL
int Matrix4::index( int row, int col ) const
{
	assert( row >= 0 && row < 4 );
	assert( col >= 0 && col < 4 );
	return col * 4 + row;
}

const double& Matrix4::operator()( int row, int col ) const
{
	return mat[ index(row,col) ];
}

double& Matrix4::operator()( int row, int col )
{
	return mat[ index(row,col) ];
}

Vector4 Matrix4::row(int r) const
{
    return Vector4( mat[index(r,0)], mat[index(r,1)], mat[index(r,2)], mat[index(r,3)] );
}

Vector4 Matrix4::column(int c) const
{
    return Vector4( mat[index(0,c)], mat[index(1,c)], mat[index(2,c)], mat[index(3,c)] );
}

Matrix4& Matrix4::operator *=( const Matrix4 &m )
{
    const Matrix4 matRet = (*this) * m;
    (*this) = matRet;
    return *this;
}

Matrix4& Matrix4::operator +=( const Matrix4 &m )
{
    const Matrix4 matRet = (*this) + m;
    (*this) = matRet;
    return *this;
}

Matrix4& Matrix4::operator -=( const Matrix4 &m )
{
    const Matrix4 matRet = (*this) - m;
    (*this) = matRet;
    return *this;
}

Matrix4 Matrix4::transpose() const
{
    Matrix4 matRet;
    for ( int i = 0; i < 4; i++ )
        for ( int j = 0; j < 4; j++ )
            matRet(i,j) = (*this)(j,i);
    return matRet;
}

Matrix4 Matrix4::operator+( const Matrix4 &m) const
{
    Matrix4 matRet;
    for ( int i = 0; i < 16; i++ )
        matRet.mat[i] = mat[i] + m.mat[i];
    return matRet;
}

Matrix4 Matrix4::operator-( const Matrix4 &m) const
{
    Matrix4 matRet;
    for ( int i = 0; i < 16; i++ )
        matRet.mat[i] = mat[i] - m.mat[i];
    return matRet;
}

Vector3 Matrix4::operator*(const Vector3& v) const
{
    return Vector3((*this)(0,0) * v[0] + (*this)(0,1) * v[1] + (*this)(0,2) * v[2],
                    (*this)(1,0) * v[0] + (*this)(1,1) * v[1] + (*this)(1,2) * v[2],
                    (*this)(2,0) * v[0] + (*this)(2,1) * v[1] + (*this)(2,2) * v[2]);
}

Point3 Matrix4::operator*(const Point3& p) const
{
    const Point3 pt((*this)(0,0) * p[0] + (*this)(0,1) * p[1] + (*this)(0,2) * p[2] + (*this)(0,3),
                    (*this)(1,0) * p[0] + (*this)(1,1) * p[1] + (*this)(1,2) * p[2] + (*this)(1,3),
                    (*this)(2,0) * p[0] + (*this)(2,1) * p[1] + (*this)(2,2) * p[2] + (*this)(2,3));
    const double w = (*this)(3,0) * p[0] + (*this)(3,1) * p[1] + (*this)(3,2) * p[2] + (*this)(3,3);
    assert( isZero( w ) == false );
    return Point3( pt[0] / w, pt[1] / w, pt[2] / w );
}

Vector4 Matrix4::operator*(const Vector4& v) const
{
    return Vector4((*this)(0,0) * v[0] + (*this)(0,1) * v[1] + (*this)(0,2) * v[2] + (*this)(0,3) * v[3],
                    (*this)(1,0) * v[0] + (*this)(1,1) * v[1] + (*this)(1,2) * v[2] + (*this)(1,3) * v[3],
                    (*this)(2,0) * v[0] + (*this)(2,1) * v[1] + (*this)(2,2) * v[2] + (*this)(2,3) * v[3],
                    (*this)(3,0) * v[0] + (*this)(3,1) * v[1] + (*this)(3,2) * v[2] + (*this)(3,3) * v[3]);
}

Matrix4 Matrix4::operator*(const Matrix4& b) const
{
    Matrix4 matRet;
    for ( int i = 0; i < 4; i++ )
	{
        for ( int j = 0; j < 4; j++ )
		{
            matRet(i,j) = 0.0;
            for ( int k = 0; k < 4; k++ )
                matRet(i,j) += (*this)(i,k) * b(k,j);
        }
    }
    return matRet;
}
Matrix4 Matrix4::inverse() const
{
    // Compute inverse using Gauss-Jordan elimination; caller is responsible
    // for ensuring that the matrix isn't singular.
    Matrix4 a(*this);
    Matrix4 b(Matrix4::identity());
    int i, j;
    int p;

    for (j = 0; j < 4; j++)
	{
        p = j;
        for (i = j + 1; i < 4; i++)
		{
            if (fabs(a(i,j)) > fabs(a(p,j)))
                p = i;
        }
        // Swap rows p and j
        if ( p != j )
		{
            for ( i = 0; i < 4; i++ )
			{
                const double ta = a(p,i);
                a(p,i) = a(j,i);
                a(j,i) = ta;

                const double tb = b(p,i);
                b(p,i) = b(j,i);
                b(j,i) = tb;
            }
        }

        const double s = a(j,j);  // if s == 0, the matrix is singular
        assert( isZero( s ) == false );
        for ( i = 0; i < 4; i++ )
		{
            a(j,i) *= (1.0 / s);
            b(j,i) *= (1.0 / s);
        }
        // Eliminate off-diagonal elements
        for (i = 0; i < 4; i++)
		{
            if (i != j)
			{
				double param = a(i, j);
				for ( int k = 0; k < 4; k++ )
				{
                    b(i,k) -= param * b(j,k);
                    a(i,k) -= param * a(j,k);
                }
            }
        }
    }
    return b;
}

bool Matrix4::operator==( const Matrix4 &m ) const
{
    for ( int i = 0; i < 16; i++ )
        if ( mat[i] != m.mat[i] )
            return false;
    return true;
}

bool Matrix4::approxEqual( const Matrix4 &m, double eps ) const
{
    for ( int i = 0; i < 16; i++ )
        if ( isZero( mat[i] - m.mat[i], eps ) == false)
            return false;
    return true;
}

void Matrix4::print() const
{
    std::cout << "( " << (*this)(0,0) << ", " << (*this)(0,1) << ", " << (*this)(0,2) << ", " << (*this)(0,3) << "\n";
    std::cout << "  " << (*this)(1,0) << ", " << (*this)(1,1) << ", " << (*this)(1,2) << ", " << (*this)(1,3) << "\n";
    std::cout << "  " << (*this)(2,0) << ", " << (*this)(2,1) << ", " << (*this)(2,2) << ", " << (*this)(2,3) << "\n";
    std::cout << "  " << (*this)(3,0) << ", " << (*this)(3,1) << ", " << (*this)(3,2) << ", " << (*this)(3,3) << ")\n";
}

Matrix4 Matrix4::identity()
{
    return Matrix4(Vector4(1, 0, 0, 0),
                   Vector4(0, 1, 0, 0),
                   Vector4(0, 0, 1, 0),
                   Vector4(0, 0, 0, 1));
}

Matrix4 Matrix4::translation(const Point3& p)
{
    return Matrix4(Vector4(1, 0, 0, p[0]),
                   Vector4(0, 1, 0, p[1]),
                   Vector4(0, 0, 1, p[2]),
                   Vector4(0, 0, 0, 1));
}

Matrix4 Matrix4::translation(const Vector3& v)
{
    return Matrix4(Vector4(1, 0, 0, v[0]),
                   Vector4(0, 1, 0, v[1]),
                   Vector4(0, 0, 1, v[2]),
                   Vector4(0, 0, 0, 1));
}

Matrix4 Matrix4::rotation(const Vector3& u, const Vector3& v, const Vector3& w)
{
    return Matrix4(Vector4(u[0], u[1], u[2], 0),
                   Vector4(v[0], v[1], v[2], 0),
                   Vector4(w[0], w[1], w[2], 0),
                   Vector4(0  , 0  , 0  , 1));
}

Matrix4 Matrix4::rotation(const Vector3& axis, double angle)
{
    Vector3 a = axis;
    a.normalize();
    const double c = cos(angle);
    const double s = sin(angle);
    const double t = 1 - c;

    return Matrix4(Vector4(t * a[0] * a[0] + c,
                           t * a[0] * a[1] - s * a[2],
                           t * a[0] * a[2] + s * a[1],
                           0),
                   Vector4(t * a[0] * a[1] + s * a[2],
                           t * a[1] * a[1] + c,
                           t * a[1] * a[2] - s * a[0],
                           0),
                   Vector4(t * a[0] * a[2] - s * a[1],
                           t * a[1] * a[2] + s * a[0],
                           t * a[2] * a[2] + c,
                           0),
                   Vector4(0, 0, 0, 1));
}

Matrix4 Matrix4::xrotation(double angle)
{
    const double c = cos(angle);
    const double s = sin(angle);

    return Matrix4(Vector4(1, 0,  0, 0),
                   Vector4(0, c, -s, 0),
                   Vector4(0, s,  c, 0),
                   Vector4(0, 0,  0, 1));
}

Matrix4 Matrix4::yrotation(double angle)
{
    double c = cos(angle);
    double s = sin(angle);

    return Matrix4(Vector4( c, 0, s, 0),
                   Vector4( 0, 1, 0, 0),
                   Vector4(-s, 0, c, 0),
                   Vector4( 0, 0, 0, 1));
}

Matrix4 Matrix4::zrotation(double angle)
{
    const double c = cos(angle);
    const double s = sin(angle);

    return Matrix4(Vector4(c, -s, 0, 0),
                   Vector4(s,  c, 0, 0),
                   Vector4(0,  0, 1, 0),
                   Vector4(0,  0, 0, 1));
}

Matrix4 Matrix4::scaling(const Vector3& s)
{
    return Matrix4(Vector4(s[0], 0   , 0   , 0),
                    Vector4(0  , s[1], 0   , 0),
                    Vector4(0  , 0   , s[2], 0),
                    Vector4(0  , 0   , 0   , 1));
}

Matrix4 Matrix4::scaling( double x, double y, double z)
{
    return Matrix4(Vector4(x, 0, 0, 0),
                   Vector4(0, y, 0, 0),
                   Vector4(0, 0, z, 0),
                   Vector4(0, 0, 0, 1));
}

Matrix4 Matrix4::scaling(double scale)
{
    return scaling(Vector3(scale, scale, scale));
}
