#ifndef ScreenPoint_h
#define ScreenPoint_h

#include <iostream>
#include "ScreenVector.h"

#define SPAE_EPSILON 1e-12

class ScreenPoint
{
public:
    ScreenPoint();
    ScreenPoint(const ScreenPoint & p);
    ScreenPoint(int _x, int _y);
  
    ScreenPoint& operator=(const ScreenPoint& a);

	const int& operator[](int n) const;
    int& operator[](int n);

	ScreenPoint& operator+=(const ScreenVector& v);
    ScreenPoint& operator-=(const ScreenVector& v);
    ScreenPoint& operator*=(int s);
    ScreenPoint operator+(const ScreenVector& v) const;
    ScreenVector operator-(const ScreenPoint& p) const;
    ScreenPoint operator-(const ScreenVector& v) const;

	bool operator==( const ScreenPoint &p ) const;
    bool operator!=( const ScreenPoint &p ) const;

	void print() const;

private:
    int x, y;
};

inline std::ostream& operator<<(std::ostream& os, const ScreenPoint& p)
{
    os << "(" << p[0] << ", " << p[1] << ")";
    return os;
}

#endif /* ScreenPoint_h */
