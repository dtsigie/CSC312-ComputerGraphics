#include "../csc321.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>

Camera::Camera() :
arScale(1.0), 
skew(0.0),
ptCOP(0,0,0)
{
    // initialize your data here
}

Camera::~Camera() {
    // destroy your data here
}

Vector3 Camera::getRight() const
{
    return getRotationFromXYZ() * Vector3(1,0,0);
}

Vector3 Camera::getUp() const
{
    return getRotationFromXYZ() * Vector3(0,1,0);
}

Vector3 Camera::getLook() const
{
    return getRotationFromXYZ() * Vector3(0,0,-1);
}

double Camera::getSkew() const
{
    return skew;
}

double Camera::getAspectRatioScale() const
{
    return arScale;
}

Point3 Camera::getProjectionCenter() const
{
    return ptCOP;
}

Matrix4 Camera::getProjection() const {
    // return the current projection matrix
    Matrix4 p = Matrix4::identity();

    double k = znear / zfar;
    p(0,2) = ptCOP[0];
    p(1,2) = ptCOP[1];
    p(0,0) = arScale;
    p(0,1) = skew;
    p(2,2) = 1.0 / (k - 1.0);
    p(2,3) = k / (k - 1.0);
    p(3,2) = -1.0;
    p(3,3) = 0.0;
    double sx = 1.0 / aspectRatio * (1.0 / tan(zoom * 0.5));
    double sy = 1.0 / tan(zoom * 0.5);
    Matrix4 scale = Matrix4::scaling(Vector3(sx / zfar, sy / zfar, 1.0 / zfar));
    return p * scale;
}

Matrix4 Camera::getWorldToCamera() const {
    // return the current world to camera matrix
    Vector3 n = -look;
    n.normalize();
    Vector3 u = up ^ n;
    u.normalize();
    Vector3 v = n ^ u;
    Point3 o;
//	double sx = 1/aspectRatio * tan(zoom * 0.5);
  //  double sy = 1/tan(zoom * 0.5);
  //  Matrix4 scale = Matrix4::scaling(Vector3(sx, sy, 1.0));
    Matrix4 trans = Matrix4::translation(o - eye);
    Matrix4 rotation = Matrix4::rotation(u, v, n);
    return rotation * trans;
}

Matrix4 Camera::getRotationFromXYZ() const
{
    // return the current camera to world matrix
    Vector3 n = -look;
    n.normalize();
    Vector3 u = up ^ n;
    u.normalize();
    Vector3 v = n ^ u;
    Point3 o;
    return Matrix4::rotation(u, v, n).transpose();
}

Matrix4 Camera::getCameraToWorld() const {
    // return the current camera to world matrix
    Vector3 n = -look;
    n.normalize();
    Vector3 u = up ^ n;
    u.normalize();
    Vector3 v = n ^ u;
    Point3 o;
    Matrix4 trans = Matrix4::translation(eye - o);
    Matrix4 rotation = Matrix4::rotation(u, v, n).transpose();
    double sx = aspectRatio * tan(zoom * 0.5);
    double sy = tan(zoom * 0.5);
    Matrix4 scale = Matrix4::scaling(Vector3(sx, sy, 1.0));
    return trans * rotation * scale;
}

int Camera::getWidth()  const{
    // return the current image width
    return width;
}

int Camera::getHeight()  const{
    // return the current image height
    return height;
}

Point3 Camera::getEye()  const{
    // return the current eye location
    return eye;
}

double Camera::getZoom() const
{
    return zoom;
}

void Camera::setFrom(const Point3& from) {
    // set the current viewpoint (eye point)
    eye = from;
}

void Camera::setAt(const Point3& at) {
    // set the point the camera is looking at
    // calling this requires that the from (or eye) point already be valid
    look = at - eye;
    look.normalize();
}

void Camera::setLook(const Vector3& l) {
    // set the direction the camera is looking at
    look = l;
    look.normalize();
}

void Camera::setUp(const Vector3& up) {
    // set the upwards direction of the camera
    this->up = up;
    this->up.normalize();
}

void Camera::setWidthHeight(int w, int h) {
    // set the current width and height of the film plane
    aspectRatio = (double) w / (double) h;
    width = w;
    height = h;
}

void Camera::setZoom(double z) {
    // set field of view (specified in degrees)
    
    // angle supplied in degrees, stored in radians
    zoom = z * M_PI / 180.0;
}

void Camera::setNearFar(double n, double f) {
    // set the near and far clipping planes
    znear = n;
    zfar = f;
}

void Camera::setSkew( double d )
{
    skew = d;
}

void Camera::setAspectRatioScale( double d )
{
    arScale = d;
}

void Camera::setProjectionCenter( const Point3 &p )
{
    ptCOP = p;
}

void Camera::moveForward(double dist) {
    // move the camera forward (in the viewing direction)
    // by the amount dist
    
    eye += look * dist;
}

void Camera::moveSideways(double dist) {
    // move the camera sideways (orthogonal to the viewing direction)
    // by the amount dist
    
    Vector3 side = look ^ up;
    side.normalize();
    eye += side * dist;
}

void Camera::moveVertical(double dist) {
    // move the camera vertically (along the up vector)
    // by the amount dist
    
    eye += dist * up;
}

void Camera::rotateYaw(double angle) {
    // rotate the camera left/right (around the up vector)
    
    Matrix4 r = Matrix4::rotation(up, angle);
    look = r * look;
}

void Camera::rotatePitch(double angle) {
    // rotate the camera up/down (pitch angle)
    
    Vector3 side = look ^ up;
    side.normalize();
    Matrix4 r = Matrix4::rotation(side, angle);
    look = r * look;
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
	Matrix4 matRot;
    if ( axis == 0 ) matRot = Matrix4::xrotation(angle);
    if ( axis == 1 ) matRot = Matrix4::yrotation(angle);
    if ( axis == 2 ) matRot = Matrix4::zrotation(angle);

    const Point3 ptFocus = getEye() + getLook() * focusDist;
	
    const Matrix4 matRotCameraInv = getRotationFromXYZ();
    const Matrix4 matAll = matRotCameraInv * matRot * matRotCameraInv.transpose();

    const double dScl = focusDist * tan( getZoom() / 2.0 );
    const double dXOff = 1.0 / arScale * ptCOP[0] * dScl - skew * ptCOP[1];
    const double dYOff = ptCOP[1] * dScl;

    // Undo center of projection pan to find true at point
    const Vector3 vecUndoCOPPan = dXOff * getRight() +
                                  dYOff * getUp();

	// Should keep unit and ortho, but reset just to make sure
	const Vector3 vecFrom = matAll * (getEye() - ptFocus);
	const Vector3 vecUp = unit( matAll * getUp() );
	const Vector3 vecRight = unit( matAll * getRight() );

    // Undo center of projection pan to find true at point

    const Vector3 vecRedoCOPPan = dXOff * vecRight +
                                  dYOff * vecUp;

    // Find from point if we rotated around the correct at point, then fixed the pan
    const Point3 ptFrom = (ptFocus + vecUndoCOPPan) + vecFrom - vecRedoCOPPan;

    // Correct the at point for the COP pan, then add in the new COP pan
    const Point3 ptAt = (ptFocus + vecUndoCOPPan) - vecRedoCOPPan;

    setFrom( ptFrom );
    setAt( ptAt );
    setUp( vecUp );
}


void Camera::moveKeyboard( )
{
    // you may change key controls for the interactive
    // camera controls here, make sure you document your changes
    // in your README file
    if (Fl::event_key('w'))
        moveForward(+0.05);
    if (Fl::event_key('s'))
        moveForward(-0.05);
    if (Fl::event_key('a'))
        moveSideways(-0.05);
    if (Fl::event_key('d'))
        moveSideways(+0.05);
    if (Fl::event_key(FL_Up))
        moveVertical(+0.05);
    if (Fl::event_key(FL_Down))
        moveVertical(-0.05);
    if (Fl::event_key(FL_Left))
        rotateYaw(+0.05);
    if (Fl::event_key(FL_Right))
        rotateYaw(-0.05);
    if (Fl::event_key(FL_Page_Up))
        rotatePitch(+0.05);
    if (Fl::event_key(FL_Page_Down))
        rotatePitch(-0.05);
}