#include "ScreenVector.h"

#include <math.h>
#include <assert.h>

ScreenVector::ScreenVector() : x(0), y(0)
{
}

ScreenVector::ScreenVector(const ScreenVector& v) : x(v[0]), y(v[1])
{
}

ScreenVector::ScreenVector(int _x, int _y) : x(_x), y(_y)
{
}

ScreenVector& ScreenVector::operator=(const ScreenVector& a)
{
    x = a[0]; y = a[1];
    return *this;
}

const int& ScreenVector::operator[](int n) const
{
	return (&x)[n];
}

int& ScreenVector::operator[](int n)
{
	return (&x)[n];
}

ScreenVector& ScreenVector::operator+=(const ScreenVector& a)
{
    x += a[0]; y += a[1];
    return *this;
}

ScreenVector& ScreenVector::operator-=(const ScreenVector& a)
{
    x -= a[0]; y -= a[1];
    return *this;
}

ScreenVector& ScreenVector::operator*=(int s)
{
    x *= s; y *= s;
    return *this;
}

ScreenVector ScreenVector::operator-() const
{
    return ScreenVector(-x, -y);
}

ScreenVector ScreenVector::operator+() const
{
    return *this;
}

ScreenVector ScreenVector::operator+( const ScreenVector &v ) const
{
    return ScreenVector( x + v.x, y + v.y );
}

ScreenVector ScreenVector::operator-( const ScreenVector &v ) const
{
    return ScreenVector( x - v.x, y - v.y );
}

ScreenVector ScreenVector::operator*( const double s ) const
{
    return ScreenVector( static_cast<int>(x * s), static_cast<int>(y * s) );
}

// Dot
int ScreenVector::operator*( const ScreenVector &v ) const
{
    return x * v.x + y * v.y;
}

double ScreenVector::length() const
{
    return (double) sqrt( (double) (x * x + y * y) );
}

int ScreenVector::lengthSquared() const
{
    return x * x + y * y;
}

bool ScreenVector::operator==( const ScreenVector &v ) const
{
    return x == v.x && y == v.y;
}

bool ScreenVector::operator!=( const ScreenVector &v ) const
{
    return x != v.x || y != v.y;
}

void ScreenVector::print() const
{
    std::cout << "(" << x << ", " << y << ")";
}
