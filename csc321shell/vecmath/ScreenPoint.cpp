#include "ScreenPoint.h"

#include <math.h>
#include <assert.h>

ScreenPoint::ScreenPoint() : x(0), y(0)
{
}

ScreenPoint::ScreenPoint(const ScreenPoint & p) : x(p[0]), y(p[1])
{
}

ScreenPoint::ScreenPoint(int _x, int _y) : x(_x), y(_y)
{
}

ScreenPoint& ScreenPoint::operator=(const ScreenPoint& a)
{
    x = a[0]; y = a[1];
    return *this;
}

const int& ScreenPoint::operator[](int n) const
{
	return (&x)[n];
}

int& ScreenPoint::operator[](int n)
{
	return (&x)[n];
}

ScreenPoint& ScreenPoint::operator+=(const ScreenVector& v)
{
    x += v[0]; y += v[1];
    return *this;
}

ScreenPoint& ScreenPoint::operator-=(const ScreenVector& v)
{
    x -= v[0]; y -= v[1];
    return *this;
}

ScreenPoint& ScreenPoint::operator*=(int s) 
{
    x *= s; y *= s;
    return *this;
}

ScreenPoint ScreenPoint::operator+(const ScreenVector& v) const 
{
    return ScreenPoint( x + v[0], y + v[1] );
}

ScreenVector ScreenPoint::operator-(const ScreenPoint& p) const 
{
    return ScreenVector( x - p.x, y - p.y );
}

ScreenPoint ScreenPoint::operator-(const ScreenVector& v) const 
{
    return ScreenPoint( x - v[0], y - v[1] );
}

bool ScreenPoint::operator==( const ScreenPoint &p ) const 
{
    return x == p.x && y == p.y;
}

bool ScreenPoint::operator!=( const ScreenPoint &p ) const 
{
    return x != p.x || y != p.y;
}

void ScreenPoint::print() const 
{
    std::cout << "(" << x << ", " << y << ")";
}
