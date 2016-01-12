#ifndef _COLOR_H_
#define _COLOR_H_

#include "csc321.h"

class Color {
public:
    Color() : r(0.0), g(0.0), b(0.0) {}
    Color(const Color& c) : r(c[0]), g(c[1]), b(c[2]) {}
    Color(GLfloat _r, GLfloat _g, GLfloat _b) : r(_r), g(_g), b(_b) {}

    const GLfloat &operator[](int n) const { return ((GLfloat *) this)[n]; }
          GLfloat &operator[](int n)       { return ((GLfloat *) this)[n]; }

    Color& operator=(const Color& c) {
        r = c[0];
        g = c[1];
        b = c[2];
        return *this;
    }

    Color& operator+=(const Color& c) {
        r += c[0]; g += c[1]; b += c[2];
        return *this;
    }

    Color& operator-=(const Color& c) {
        r -= c[0]; g -= c[1]; b -= c[2];
        return *this;
    }

    Color& operator*=(const Color& c) {
        r *= c[0]; g *= c[1]; b *= c[2];
        return *this;
    }

    Color& operator/=(const Color& c) {
        r /= c[0]; g /= c[1]; b /= c[2];
        return *this;
    }

    Color& operator*=(GLfloat s) {
        r *= s; g *= s; b *= s;
        return *this;
    }
  
    Color& operator/=(GLfloat s) {
        assert( isZero( s ) == false );
        GLfloat is = 1.0f / s;
        r *= is; g *= is; b *= is;
        return *this;
    }

    Color operator-() const {
        return Color(-r, -g, -b);
    }

    Color operator+() const {
        return *this;
    }

    int toRGB() const {
        int red   = (int) (r * 255.0);
        int green = (int) (g * 255.0);
        int blue  = (int) (b * 255.0);

        if (red < 0)   red = 0;   else if (red > 255)   red = 255;
        if (green < 0) green = 0; else if (green > 255) green = 255;
        if (blue < 0)  blue = 0;  else if (blue > 255)  blue = 255;

        return (red << 16) | (green << 8) | blue;
    }

    GLfloat getMax() const {
        GLfloat max = (r > g) ? r : g;
        if (max < b)
            max = b;
        return max;
    }

    GLfloat getMin() const {
        GLfloat min = (r < g) ? r : g;
        if (min > b)
            min = b;
        return min;
    }

    GLfloat getAverage() const {
        return (r + g + b) / 3.0f;
    }

    void print() const {
        std::cout << r << " " << g << " " << b << "\n";
    }
private:
    GLfloat r, g, b;
};

inline Color operator+(const Color& a, const Color& b) {
    return Color(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

inline Color operator-(const Color& a, const Color& b) {
    return Color(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

inline Color operator*(const Color& a, const Color& b) {
    return Color(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

inline Color operator/(const Color& a, const Color& b) {
    return Color(a[0] / b[0], a[1] / b[1], a[2] / b[2]);
}

inline Color operator*(GLfloat s, const Color& c) {
    return Color(s * c[0], s * c[1], s * c[2]);
}

inline Color operator*(const Color& c, GLfloat s) {
    return Color(s * c[0], s * c[1], s * c[2]);
}

inline Color operator/(const Color& c, GLfloat s) {
    assert( isZero( s ) == false );
    GLfloat is = 1.0f / s;
    return Color(is * c[0], is * c[1], is * c[2]);
}

#endif /* _COLOR_H_ */
