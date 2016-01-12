#ifndef ScreenVector_h
#define ScreenVector_h

#include <iostream>

#define SVAE_EPSILON 1e-12

class ScreenVector
{
public:
    ScreenVector();
    ScreenVector(const ScreenVector& v);
    ScreenVector(int _x, int _y);
  
    ScreenVector& operator=(const ScreenVector& a);

    const int &operator[](int n) const;
          int &operator[](int n);

    ScreenVector& operator+=(const ScreenVector& a);
    ScreenVector& operator-=(const ScreenVector& a);
    ScreenVector& operator*=(int s);
    ScreenVector operator-() const;
    ScreenVector operator+() const;
    ScreenVector operator+( const ScreenVector &v ) const;
    ScreenVector operator-( const ScreenVector &v ) const;
    ScreenVector operator*( const double s ) const;

	// Dot
    int operator*( const ScreenVector &v ) const;

	double length() const;
    int lengthSquared() const;

	bool operator==( const ScreenVector &v ) const;
    bool operator!=( const ScreenVector &v ) const;

	void print() const;
  
private:
    int x, y;
};

inline ScreenVector operator*( const double s, const ScreenVector &v )
{
    return ScreenVector( static_cast<int>(v[0] * s), static_cast<int>(v[1] * s) );
}

inline std::ostream& operator<<(std::ostream& os, const ScreenVector& v)
{
    os << "(" << v[0] << ", " << v[1] << ")";
    return os;
}

#endif /* ScreenPoint_h */
