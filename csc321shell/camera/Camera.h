#ifndef _MY_CAMERA_H_
#define _MY_CAMERA_H_

#include "csc321.h"
#include "Matrix4.h"

// you must implement all of the following methods

class Camera {
public:
    Camera();
    ~Camera();
    
    // Perspective plus scale
    Matrix4 getProjection() const;
    // Rotation and translation
    Matrix4 getWorldToCamera() const;
    Matrix4 getCameraToWorld() const;
    Matrix4 getRotationFromXYZ() const;

    // screen width/height
    int getWidth() const;
    int getHeight() const;
    // Camera orientation, position
    Point3  getEye() const;
    Vector3 getRight() const;
    Vector3 getUp() const;
    Vector3 getLook() const;
    // Perspective data
    Point3 getProjectionCenter() const;
    double getZoom() const;
    double getSkew() const;
    double getAspectRatioScale() const;

    // For setting camera
    void setFrom(const Point3& from);
    void setAt(const Point3& at);
    void setLook(const Vector3& l);
    void setUp(const Vector3& up);
    void setWidthHeight(int w, int h);
    void setZoom(double z);
    void setNearFar(double n, double f);
    void setProjectionCenter( const Point3 &in_pt );
    void setAspectRatioScale( double );
    void setSkew( double );

    void moveKeyboard();

    // user interaction methods
    void moveForward(double dist);
    void moveSideways(double dist);
    void moveVertical(double dist);
    void rotateYaw(double angle);
    void rotatePitch(double angle);

    // IBar
    void rotateAroundAtPoint(int axis, double angle, double focusDist);

private:
    // declare your variables here:
    Point3 eye;
    Vector3 up, look;
    double znear, zfar, zoom, aspectRatio;
    double skew, arScale;
    Point3 ptCOP;
    int width, height;
};

#endif /* _MY_CAMERA_H_ */