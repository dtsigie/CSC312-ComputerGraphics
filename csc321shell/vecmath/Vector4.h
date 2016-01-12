#ifndef Vector4_h
#define Vector4_h

#include <iostream>

#define V4AE_EPSILON 1e-12

class Vector4
{
public:
    Vector4();
    Vector4(const Vector4& v);
    Vector4(double _x, double _y, double _z, double _w);
  
    Vector4& operator=(const Vector4& a);
    const double& operator[](int n) const;
    double& operator[](int n);
  
    Vector4& operator+=(const Vector4& a);
    Vector4& operator-=(const Vector4& a);
    Vector4& operator*=(double s);
    Vector4 operator-();
    Vector4 operator+();
    Vector4 operator+( const Vector4 &v ) const;
    Vector4 operator-( const Vector4 &v ) const;
    Vector4 operator/( const double s ) const;
    Vector4 operator*( const double s ) const;
  
    // Dot
    double operator*( const Vector4 &v ) const;
  
    double length() const;
    double lengthSquared() const;
    void normalize();

	bool operator==( const Vector4 &v ) const;
    bool operator!=( const Vector4 &v ) const;

	bool approxEqual( const Vector4 &v, double eps = V4AE_EPSILON ) const;
    void print() const;
  
private:
    double x, y, z, w;
};

inline Vector4 operator*( const double s, const Vector4 &v )
{
    return Vector4( v[0] * s, v[1] * s, v[2] * s, v[3] * s );
}

inline double length( const Vector4 &v )
{
	return v.length();
}

inline Vector4 unit( const Vector4 &v )
{
	return v / v.length();
}

inline std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
    os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
    return os;
}

#endif /* Vector4_h */
