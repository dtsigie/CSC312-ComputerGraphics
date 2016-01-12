#ifndef _LIGHT_DEFS_H
#define _LIGHT_DEFS_H

#include "Color.h"
#include "Point3.h"

/*
 * Parsed light data is stored here
 */
class Light {
public:
    Light();
    ~Light() {}

    enum LightType {
        POINTLIGHT,
        DIRECTIONAL,
        SPOTLIGHT
    };

    LightType getType( ) const { return type; }
    Point3    getPos() const { return Point3(pos[0], pos[1], pos[2]); }
    Vector3   getDir() const { return Vector3(dir[0], dir[1], dir[2]); }
    double    getAperture() const { return aperture; }
    double    getExponent() const { return exponent; }
    Color     getColor() const { return color; }
    Point3    getFalloff() const { return Point3( f0, f1, f2 ); }

    void setType( LightType in_type );
    void setPosition( const Point3 &pt ) { pos[0] = static_cast<float>(pt[0]); pos[1] = static_cast<float>(pt[1]); pos[2] = static_cast<float>(pt[2]); }
    void setDirection( const Vector3 &vec) { dir[0] = static_cast<float>(vec[0]); dir[1] = static_cast<float>(vec[1]); dir[2] = static_cast<float>(vec[2]); }
    void setColor( const Color &col ) { color = col; }
    void setFalloff( double df0, double df1, double df2 ) { f0 = df0; f1 = df1; f2 = df2; }
    void setAperture( double a ) { aperture = static_cast<float>(a);}
    void setExponent( double e ) { exponent = static_cast<float>(e);}

    void setOpenGLLight (GLenum lightid) const;

private:
    LightType type;
    float pos[4];
    float dir[4];
    Color color;
    double f0, f1, f2;

    float aperture;
    float exponent;
};

inline
Light::Light()  : 
type( POINTLIGHT), 
color(0.5,0.5,0.5), 
f0(1), f1(0), f2(0), 
aperture(180.0),
exponent(1.0)
{
    // OpenGL's brain-dead way of distinguishing between point and directional lights
    // is to look at the 4th parameter - 0 means a vector
    pos[0] = pos[1] = pos[2] = pos[3] = pos[4] = 1.0;
    dir[0] = dir[1] = dir[2] = 1.0;
    dir[3] = 0.0; // forces OpenGL to treat this as a directional light
}

inline
void Light::setType( LightType in_type ) 
{ 
    type = in_type; 
}

inline
void Light::setOpenGLLight (GLenum lightid) const
{
    const float black[4] = { 0,0,0,1 };
    glLightfv( lightid, GL_AMBIENT       ,  black);
    glLightfv( lightid, GL_DIFFUSE       , &color[0]);
    glLightfv( lightid, GL_SPECULAR      , &color[0]);
    glLightf(  lightid, GL_SPOT_EXPONENT        , exponent  );
    glLightf(  lightid, GL_SPOT_CUTOFF          , 180.0f    );
    glLightf(  lightid, GL_CONSTANT_ATTENUATION , static_cast<float>(f0) );
    glLightf(  lightid, GL_LINEAR_ATTENUATION   , static_cast<float>(f1) );
    glLightf(  lightid, GL_QUADRATIC_ATTENUATION, static_cast<float>(f2) );

    switch (type) {
        case Light::POINTLIGHT: {
            glLightfv(lightid, GL_POSITION, pos  );
        } break;
        case Light::DIRECTIONAL: {
            glLightfv(lightid, GL_POSITION, dir  );
        } break;
        case Light::SPOTLIGHT: {
            glLightfv(lightid, GL_POSITION, pos  );
            glLightfv(lightid, GL_SPOT_DIRECTION, dir  );
            glLightf(lightid, GL_SPOT_CUTOFF          , aperture );
        } break;
        default: break;
    }
    glEnable( lightid );
}
  
#endif
