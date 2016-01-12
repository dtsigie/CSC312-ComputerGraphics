#include "Vector4.h"

#include <math.h>
#include <assert.h>

#include "../csc321.h"

Vector4::Vector4() : x(0), y(0), z(0), w(0)
{
}

Vector4::Vector4(const Vector4& v) : x(v[0]), y(v[1]), z(v[2]), w(v[3])
{
}

Vector4::Vector4(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w)
{
}

Vector4& Vector4::operator=(const Vector4& a)
{
    x = a[0]; y = a[1]; z = a[2]; w = a[3];
    return *this;
}

const double& Vector4::operator[](int n) const
{
	return ((double *) this)[n];
}

double& Vector4::operator[](int n)
{
	return ((double *) this)[n];
}

Vector4& Vector4::operator+=(const Vector4& a)
{
    x += a[0]; y += a[1]; z += a[2]; w += a[3];
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& a)
{
    x -= a[0]; y -= a[1]; z -= a[2]; w -= a[3];
    return *this;
}

Vector4& Vector4::operator*=(double s)
{
    x *= s; y *= s; z *= s; w *= s;
    return *this;
}

Vector4 Vector4::operator-()
{
    return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator+()
{
    return *this;
}

Vector4 Vector4::operator+( const Vector4 &v ) const
{
    return Vector4( x + v.x, y + v.y, z + v.z, w + v.w );
}

Vector4 Vector4::operator-( const Vector4 &v ) const
{
    return Vector4( x - v.x, y - v.y, z - v.z, w - v.w );
}

Vector4 Vector4::operator/( const double s ) const
{
    assert( s > 0.0 );
    return Vector4( x / s, y / s, z / s, w / s );
}

Vector4 Vector4::operator*( const double s ) const
{
    return Vector4( x * s, y * s, z * s, w * s );
}

// Dot
double Vector4::operator*( const Vector4 &v ) const
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

double Vector4::length() const
{
    return (double) sqrt(x * x + y * y + z * z + w * w);
}

double Vector4::lengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

void Vector4::normalize()
{
    double s = 1.0 / length();
    x *= s; y *= s; z *= s; w *= s;
}

bool Vector4::operator==( const Vector4 &v ) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator!=( const Vector4 &v ) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool Vector4::approxEqual( const Vector4 &v, double eps ) const
{
    return isZero( x - v.x, eps ) && isZero( y - v.y, eps ) && isZero( z - v.z, eps ) && isZero( w - v.w, eps );
}

void Vector4::print() const
{
    std::cout << x << " " << y << " " << z << " " << w << "\n";
}
