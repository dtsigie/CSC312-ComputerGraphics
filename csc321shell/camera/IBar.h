#ifndef _CAM_IBAR_DEFS_H__
#define _CAM_IBAR_DEFS_H__

#include "Camera.h"
#include "ScreenPoint.h"
#include <vector>

class IBar
{
protected:
    
   typedef enum {
        NONE = 0,
        PAN = 1,
        ZOOM = 2,
        ROTATE = 3,
        LEFT = 4,
        RIGHT =5,
        TOP_L = 6,
        TOP_R = 7,
        BOTTOM_L = 8,
        BOTTOM_R = 9,
        ASPECT_RATIO = 10,
        SKEW = 11,
        ALL = 12
    } HandleType;

    typedef enum {
        UNCOMMITTED = 0,
        SCALE_ONLY = 1,
        ANGLE_ONLY = 2,
        BOTH = 3
    } CommitType;

    Camera &m_cam;

    // In camera coordinates
    double m_dClickDistance;
    double m_dFocusDist;

    // Drawing state
    HandleType m_handle;
    CommitType m_commit;
    bool       m_bCameraMode;     // Use bottom limbs for camera mode
    Point3     m_ptDown;          // Mouse down point in [-1,1] X [-1,1]
    Point3     m_ptDrag;          // Mouse move point in [-1,1] X [-1,1]
    int        m_iClosest;        // Which cube point was selected

    static IBar m_cameraOrig;

    double m_dCubeHeight;

    Vector3 Limb( const std::vector<Point3> &in_aptCube, const Matrix4 &in_mat ) const;
    Point3 LimbBase( const std::vector<Point3> &in_aptClick, const Matrix4 &in_mat ) const;
    std::vector<Point3> MakeCube( double &out_dTStem ) const ;
    Vector3 GetCOPOffset() const;
    Point3 FlTkToCamera( const ScreenPoint &in_ptScreen );

    void MouseMoveObject() ;

public:

    /**@name Data */
    //@{
    ///
    double GetFocusDist() const { return m_dFocusDist; }
    //@}

    /**@name IBar controls */
    //@{
    ///
    void Draw( const bool in_bIsMouseOver, const bool in_help ) const;
    ///
    void SetClickDistance( const double in_d ) ;
    ///
    bool MouseMove( const ScreenPoint &in_pt, bool &out_bRedraw ) ;
    ///
    bool MouseDown( const ScreenPoint &in_pt, const bool in_bShift ) ;
    ///
    void MouseDrag( const ScreenPoint &in_pt, const bool in_bShift ) ;
    ///
    void MouseRelease( const ScreenPoint &in_pt, const bool in_bShift ) ;
    /// 
    void SetFocusPoint( const Point3 &in_pt ) ;
	//@}

    const Camera &getCamera() const;

    ///
    IBar &operator=( const IBar &in_cam );
    ///
    IBar( const IBar &in_cam );
    ///
    IBar( Camera &cam );
    ///
    ~IBar() {}
};

#endif
